#pragma once

#include <stdlib.h>
#include <intrin.h>
#include <iostream>

#define max(a, b)  (((a) > (b)) ? (a) : (b))
#define oldest(a) ( !!(a&(1<<(sizeof(a)*8-1))) )

typedef unsigned __int16 hword;
typedef unsigned __int32 word;

#define MIN_SHORT_VAL (1 << (BigInteger::BITS_PER_DIGIT - 1))

template <typename T>
std::pair<T, T> multiply_with_carry(T a, T b) {
    throw "Not Implemented";
}

template <>
inline std::pair<unsigned __int64, unsigned __int64> multiply_with_carry(unsigned __int64 a, unsigned __int64 b) {
    unsigned __int64 result_high;
    unsigned __int64 result_low = _umul128(a, b, &result_high);
    return std::pair<unsigned __int64, unsigned __int64>(result_high, result_low);
}

template <>
inline std::pair<unsigned __int32, unsigned __int32> multiply_with_carry(unsigned __int32 a, unsigned __int32 b) {
    unsigned __int64 result = a;
    result *= b;
    return std::pair<unsigned __int32, unsigned __int32>(result >> 32, result);
}

template <>
inline std::pair<unsigned __int16, unsigned __int16> multiply_with_carry(unsigned __int16 a, unsigned __int16 b) {
    unsigned __int32 result = a;
    result *= b;
    return std::pair<unsigned __int16, unsigned __int16>(result >> 16, result);
}

template <typename T>
std::pair<unsigned __int8, T> add_with_carry(unsigned __int8 carry, T a, T b) {
    throw "Not Implemented";
}

template <>
inline std::pair<unsigned __int8, unsigned __int64> add_with_carry(unsigned __int8 carry, unsigned __int64 a, unsigned __int64 b) {
    unsigned __int64 result_low;
    carry = _addcarryx_u64(carry, a, b, &result_low);
    return std::pair<unsigned __int8, unsigned __int64>(carry, result_low);
}

template <>
inline std::pair<unsigned __int8, unsigned __int32> add_with_carry(unsigned __int8 carry, unsigned __int32 a, unsigned __int32 b) {
    unsigned __int32 result_low;
    carry = _addcarryx_u32(carry, a, b, &result_low);
    return std::pair<unsigned __int8, unsigned __int32>(carry, result_low);
}

template <>
inline std::pair<unsigned __int8, unsigned __int16> add_with_carry(unsigned __int8 carry, unsigned __int16 a, unsigned __int16 b) {
    unsigned __int16 result_low;
    carry = _addcarry_u16(carry, a, b, &result_low);
    return std::pair<unsigned __int8, unsigned __int16>(carry, result_low);
}

union _word
{
    word whole;
    struct
    {
        hword second_half;
        hword first_half;
    };
};

class BigInteger
{
public:
    BigInteger() {
        size = 1;
        buff = (hword*)malloc(sizeof(hword));
        *buff = 0;
    }

    BigInteger(int num) {
        size = sizeof(num) / sizeof(hword);
        buff = (hword*)malloc(size * sizeof(hword));
        for (int i = 0; i < sizeof(num) / sizeof(hword); i++) {
            buff[i] = hword(num >> (i * BITS_PER_DIGIT));
        }
        trim();
    }

    BigInteger(unsigned int num) {
        size = 1 + sizeof(num) / sizeof(hword);
        buff = (hword*)malloc(size * sizeof(hword));
        for (int i = 0; i < sizeof(num) / sizeof(hword); i++) {
            buff[i] = hword(num >> (i * BITS_PER_DIGIT));
        }
        buff[sizeof(num) / sizeof(hword)] = 0;
        trim();
    }

    BigInteger(std::pair<hword, hword> num) {
        size = 3;
        buff = (hword*)malloc(size * sizeof(hword));
        buff[0] = num.second;
        buff[1] = num.first;
        buff[2] = 0;
        trim();
    }

    BigInteger(const char* str) {
        BigInteger t;
        t = str;
        size = t.size;
        buff = (hword*)malloc(size * sizeof(hword));
        memcpy(buff, t.buff, size * sizeof(hword));
    }

    BigInteger(const BigInteger& lnum) {
        size = lnum.size;
        buff = (hword*)malloc(size * sizeof(hword));
        memcpy(buff, lnum.buff, size * sizeof(hword));
        trim();
    }

    ~BigInteger() {
        free(buff);
    }

    BigInteger operator~() const {
        BigInteger res(*this);
        for (int i = 0; i < res.size; ++i)
            res.buff[i] = ~res.buff[i];
        return res;
    }

    BigInteger operator-() const {
        BigInteger res(*this);
        --res;
        return ~res;
    }

    const BigInteger& operator+() const {
        return (*this);
    }

    BigInteger operator+(const BigInteger& r) const {
        int m = max(this->size, r.size);
        int s = this->sign() != r.sign();
        BigInteger res;
        res.set_size(m + 1 - s);
        unsigned __int8 carry = 0;
        for (int i = 0; i < res.size; ++i) {
            auto result = add_with_carry(carry, (*this)[i], r[i]);
            carry = result.first;
            res.buff[i] = result.second;
        }
        return res;
    }

    BigInteger operator+(int r) const {
        return *this + BigInteger(r); // TODO: optimize
    }

    BigInteger& operator+=(const BigInteger& r) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) + r;
        return *this;
    }

    BigInteger& operator+=(int r) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) + r;
        return *this;
    }

    BigInteger operator-(const BigInteger& r) const {
        return (*this) + (-r);
    }

    BigInteger operator-(int r) const {
        return *this - BigInteger(r); // TODO: optimize
    }

    BigInteger& operator-=(const BigInteger& r) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) - r;
        return *this;
    }

    BigInteger& operator-=(int r) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) - r;
        return *this;
    }

    BigInteger operator*(const BigInteger& lnum) const {
        BigInteger res;
        BigInteger l = sign() ? -(*this) : (*this);
        BigInteger r = lnum.sign() ? -lnum : lnum;
        int s = sign() ^ lnum.sign();
        for (int i = 0; i < r.size; i++) {
            res += (l * r[i]) << (i * BITS_PER_DIGIT);
        }
        return s ? -res : res;
    }

    BigInteger operator*(int r) const {
        return (*this) * BigInteger(r);
    }

    BigInteger operator*(hword r) const {
        BigInteger res;
        bool s = sign();
        BigInteger l_positive = s ? -(*this) : (*this);
        res.set_size(l_positive.size + 1);
        for (int i = 0; i < l_positive.size; i++) {
            BigInteger t = multiply_with_carry(l_positive[i], r);
            res += t << (i * BITS_PER_DIGIT);
        }
        return s ? -res : res;
    }

    BigInteger& operator*=(const BigInteger& r) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) * r;
        return *this;
    }

    BigInteger& operator*=(int r) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) * r;
        return *this;
    }

    BigInteger operator/(const BigInteger& lnum) const {
        BigInteger l = sign() ? -(*this) : (*this);
        BigInteger r = lnum.sign() ? -lnum : lnum;
        int s = sign() ^ lnum.sign();
        BigInteger res = l.divmod(r).first;

        return s ? -res : res;
    }

    BigInteger operator/(int r) const {
        return (*this) / BigInteger(r);
    }

    BigInteger operator/(hword r) const {
        int s = sign();
        BigInteger res;
        BigInteger l = s ? -(*this) : (*this);
        _word t;
        res.set_size(l.size);
        for (int i = l.size; i > 0; --i)
        {
            t.first_half = l[i];
            t.second_half = l[i - 1];
            res.buff[i - 1] = (hword)(t.whole / r);
            l.buff[i - 1] = (hword)(t.whole % r);
        }
        return s ? -res : res;
    }

    BigInteger& operator/=(const BigInteger& r) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) / r;
        return *this;
    }

    BigInteger& operator/=(int r) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) / r;
        return *this;
    }

    BigInteger operator%(const BigInteger& lnum) const {
        BigInteger l = sign() ? -(*this) : (*this);
        BigInteger r = lnum.sign() ? -lnum : lnum;
        int s = sign() ^ lnum.sign();
        BigInteger res = l.divmod(r).second;

        return s ? -res : res;
    }

    BigInteger operator%(int r) const {
        return (*this) % BigInteger(r);
    }

    BigInteger operator%(hword r) const {
        // TODO: optimize -- implement a divmod method and use it here
        return (*this) - (((*this) / r) * r);
    }

    BigInteger& operator%=(const BigInteger& r) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) % r;
        return *this;
    }

    BigInteger& operator%=(int r) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) % r;
        return *this;
    }

    bool operator<(const BigInteger& other) const {
        // TODO: optimize -- do not iterate numbers of different sizes
        for (int i = max(this->size, other.size) - 1; i >= 0; i--) {
            if ((*this)[i] < other[i]) {
                return true;
            }
            if ((*this)[i] > other[i]) {
                return false;
            }
        }
        return false;
    }

    bool operator>(const BigInteger& r) const {
        return r < *this;
    }

    bool operator<=(const BigInteger& r) const {
        return !(*this > r);
    }

    bool operator>=(const BigInteger& r) const {
        return !(*this < r);
    }

    bool operator==(const BigInteger& other) const {
        // TODO: optimize -- do not iterate numbers of different sizes
        for (int i = 0; i < max(this->size, other.size); i++) {
            if ((*this)[i] != other[i]) {
                return false;
            }
        }

        return true;
    }

    bool operator!=(const BigInteger& r) const {
        return !(*this == r);
    }

    const BigInteger& operator=(const BigInteger& lnum) {
        if (this != &lnum) {
            size = lnum.size;
            free(buff);
            buff = (hword*)malloc(size * sizeof(hword));
            memcpy(buff, lnum.buff, size * sizeof(hword));
        }
        trim();
        return *this;
    }

    const BigInteger& operator=(int num) {
        size = sizeof(num) / sizeof(hword);
        free(buff);
        buff = (hword*)malloc(size * sizeof(hword));
        for (int i = 0; i < sizeof(num) / sizeof(hword); i++) {
            buff[i] = hword(num >> (i * BITS_PER_DIGIT));
        }
        trim();
        return *this;
    }

    const BigInteger& operator=(unsigned num) {
        size = 1 + sizeof(num) / sizeof(hword);
        free(buff);
        buff = (hword*)malloc(size * sizeof(hword));
        for (int i = 0; i < sizeof(num) / sizeof(hword); i++) {
            buff[i] = hword(num >> (i * BITS_PER_DIGIT));
        }
        buff[sizeof(num) / sizeof(hword)] = 0;
        trim();
        return *this;
    }

    const BigInteger& operator=(const char* str) {
        (*this) = 0;
        int s = 0;

        if (*str == '+') {
            ++str;
        }
        else if (*str == '-') {
            s = 1;
            ++str;
        }

        for (int i = 0; str[i]; ++i) {
            // TODO: read multiple decimal digits at once
            if (str[i] < '0' || str[i] > '9') break;
            hword digit = str[i] - '0';
            (*this) *= IO_BASE;
            (*this) += digit;
        }

        *this = s ? -(*this) : (*this);
        trim();
        return *this;
    }

    explicit operator long double() const {
        long double result = 0.0;
        long double multiplier = pow(2.0, BITS_PER_DIGIT);
        for (int i = size - 1; i >= 0; i--) {
            result *= multiplier;
            result += buff[i];
        }
        return result;
    }

    explicit operator hword() const {
        return buff[0];
    }

    BigInteger operator++(int) {
        BigInteger old = *this;
        ++(*this);
        return old;
    }

    BigInteger& operator++() {
        for (int i = 0; i < size; ++i) {
            buff[i] += 1;

            if (buff[i] != 0) {
                break;
            }
        }
        return *this;
    }

    BigInteger operator--(int) {
        BigInteger old = *this;
        --(*this);
        return old;
    }

    BigInteger& operator--() {
        for (int i = 0; i < size - 1; ++i) {
            buff[i] -= 1;

            if (buff[i] != hword(~0ll)) {
                return *this;
            }
        }

        int leading_hword_index = size - 1;
        if (buff[leading_hword_index] == MIN_SHORT_VAL) {
            set_size(size + 1);
        }
        buff[leading_hword_index] -= 1;

        return *this;
    }

    BigInteger operator<<(unsigned shift) const {
        unsigned hword_shift = shift / BITS_PER_DIGIT;
        unsigned bit_shift = shift % BITS_PER_DIGIT;

        BigInteger res;
        res.set_size(size + hword_shift + 1);
        for (int i = 0; i <= size; ++i) {
            res.buff[res.size - i - 1] = ((*this)[size - i - 1] >> (BITS_PER_DIGIT - bit_shift)) | ((*this)[size - i] << bit_shift);
        }
        return res;
    }

    BigInteger operator>>(unsigned shift) const {
        unsigned hword_shift = shift / BITS_PER_DIGIT;
        unsigned bit_shift = shift % BITS_PER_DIGIT;

        if (hword_shift >= size) return sign() ? ~0 : 0;
        BigInteger res;
        res.set_size(size - hword_shift);
        for (int i = 0; i < res.size; ++i) {
            res.buff[res.size - i - 1] = ((*this)[size - i - 1] >> bit_shift) | ((*this)[size - i] << (BITS_PER_DIGIT - bit_shift));
        }
        return res;
    }

    BigInteger& operator<<=(unsigned shift) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) << shift;
        return *this;
    }

    BigInteger& operator>>=(unsigned shift) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) >> shift;
        return *this;
    }

    template <typename T> friend std::basic_ostream<T>& operator<<(std::basic_ostream<T>&, const BigInteger&);
    template <typename T> friend std::basic_istream<T>& operator>>(std::basic_istream<T>&, BigInteger&);

    std::pair<BigInteger, BigInteger> divmod(const BigInteger& other) {
        if (other == 0) {
            // This will blow up with a CPU error
            return std::pair<BigInteger, BigInteger>(1 / other[0], 1 % other[0]);
        }

        BigInteger scaled_divisor = other;
        BigInteger remain = *this;
        BigInteger result;
        BigInteger multiple = 1;

        while (scaled_divisor < *this) {
            scaled_divisor <<= 1;
            multiple <<= 1;
        }

        do {
            if (remain >= scaled_divisor) {
                remain -= scaled_divisor;
                result += multiple;
            }
            scaled_divisor >>= 1;
            multiple >>= 1;
        } while (multiple != 0);

        return std::pair<BigInteger, BigInteger>(result, remain);
    }

    void printbin() const {
        for (int i = size - 1; i >= 0; --i) {
            // TODO: use simpler iteration
            for (int mask = MIN_SHORT_VAL; mask; mask >>= 1) {
                std::cout << ((buff[i] & mask) ? 1 : 0);
            }
        }
    }

    void printhex() const {
        for (int i = size - 1; i >= 0; --i) {
            std::cout << std::hex << buff[i];
        }
    }

    static const hword IO_BASE=10;
    static const int BITS_PER_DIGIT = sizeof(hword) * 8;
private:

    hword operator[](unsigned position) const {
        if (position >= size) return oldest(this->buff[size - 1]) ? ~0 : 0;
        return this->buff[position];
    }

    void set_size(int new_size) {
        hword sign = oldest(this->buff[size - 1]) ? ~0 : 0;
        buff = (hword*)realloc(buff, new_size * sizeof(hword));
        for (int i = size; i < new_size; ++i) buff[i] = sign;
        size = new_size;
    }

    void trim() {
        hword s = sign() ? ~0 : 0;
        int i;
        for (i = 0; i < size - 1 && buff[size - i - 1] == s; ++i) {
            if ((s != 0 && oldest(buff[size - i - 2]) == 0) || (s == 0 && oldest(buff[size - i - 2]) != 0)) break;
        }
        set_size(size - i);
    }

    bool sign() const {
        return oldest(this->buff[size - 1]);
    }

    hword* buff;
    int size;
};

template <typename T> BigInteger operator+(T l, const BigInteger& r) {
    return r + l;
}

template <typename T> BigInteger operator-(T l, const BigInteger& r) {
    return (-r) + l;
}

template <typename T> BigInteger operator*(T l, const BigInteger& r) {
    return r * l;
}

template <typename T> BigInteger operator/(T l, const BigInteger& r) {
    return BigInteger(l) / r;
}

template <typename T> BigInteger operator%(T l, const BigInteger& r) {
    return BigInteger(l) % r;
}

template <typename T> bool operator<(T l, const BigInteger& r) {
    return r > l;
}

template <typename T> bool operator>(T l, const BigInteger& r) {
    return r < l;
}

template <typename T> bool operator<=(T l, const BigInteger& r) {
    return r >= l;
}

template <typename T> bool operator>=(T l, const BigInteger& r) {
    return r <= l;
}

template <typename T> bool operator==(T l, const BigInteger& r) {
    return r == l;
}

template <typename T> bool operator!=(T l, const BigInteger& r) {
    return r != l;
}

template <typename T> std::basic_ostream<T>& operator<<(std::basic_ostream<T>& os, const BigInteger& ln) {
    BigInteger ln_positive;
    if (ln.sign()) {
        ln_positive = -ln;
        os << '-';
    }
    else {
        ln_positive = ln;
    }
    int i;
    T* buff;
    try {
        // Долния ред заделя (много) повече място от необходимото. Горния заделя точно колкото е нужно, но не го ползвам, защото изглежда безумно...
        //buff=new T[1+(log( double(1<< (sizeof(hword)*8)) ) / log( double(LongNumber::IO_BASE) ))*ln.size ];
        buff = new T[ln_positive.size * BigInteger::BITS_PER_DIGIT];
    }
    catch (...) {
        throw;
    }

    for (i = 0; ; ++i) {
        buff[i] = '0' + (ln_positive % BigInteger::IO_BASE).buff[0];
        ln_positive = ln_positive / BigInteger::IO_BASE;
        if (ln_positive == 0) break;
    }
    for (; i >= 0; --i) {
        os << buff[i];
    }
    delete[] buff;

    return os;
}

template <typename T> std::basic_istream<T>& operator>>(std::basic_istream<T>& is, BigInteger& ln) {
    ln = 0;
    int s = 0;
    T ch;
    is >> ch;
    if (ch == '-') {
        s = 1;
        is >> ch;
    }
    else if (ch == '+') {
        s = 0;
        is >> ch;
    }

    for (;;) {
        ln = ln * BigInteger::IO_BASE;
        ln = ln + (ch - '0');
        ch = is.peek();
        if (ch < '0' || ch > '9' || isspace(ch)) break;
        is >> ch;
    }
    if (s) ln = -ln;
    return is;
}
