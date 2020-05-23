#pragma once

#include <algorithm>
#include <stdlib.h>
#include <intrin.h>
#include <iostream>

typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

template <typename T> std::pair<T, T> multiply_with_carry(T a, T b) {
    throw "Not Implemented";
}

template <> inline std::pair<uint64, uint64> multiply_with_carry(uint64 a, uint64 b) {
    uint64 result_high;
    uint64 result_low = _umul128(a, b, &result_high);
    return std::pair<uint64, uint64>(result_high, result_low);
}

template <> inline std::pair<uint32, uint32> multiply_with_carry(uint32 a, uint32 b) {
    uint64 result = a;
    result *= b;
    return std::pair<uint32, uint32>(result >> 32, result);
}

template <> inline std::pair<uint16, uint16> multiply_with_carry(uint16 a, uint16 b) {
    uint32 result = a;
    result *= b;
    return std::pair<uint16, uint16>(result >> 16, result);
}

template <typename T> std::pair<uint8, T> add_with_carry(uint8 carry, T a, T b) {
    throw "Not Implemented";
}

template <> inline std::pair<uint8, uint64> add_with_carry(uint8 carry, uint64 a, uint64 b) {
    uint64 result_low;
    carry = _addcarryx_u64(carry, a, b, &result_low);
    return std::pair<uint8, uint64>(carry, result_low);
}

template <> inline std::pair<uint8, uint32> add_with_carry(uint8 carry, uint32 a, uint32 b) {
    uint32 result_low;
    carry = _addcarryx_u32(carry, a, b, &result_low);
    return std::pair<uint8, uint32>(carry, result_low);
}

template <> inline std::pair<uint8, uint16> add_with_carry(uint8 carry, uint16 a, uint16 b) {
    uint16 result_low;
    carry = _addcarry_u16(carry, a, b, &result_low);
    return std::pair<uint8, uint16>(carry, result_low);
}

template <typename T> std::pair<T, T> udivmod(T high_dividend, T low_dividend, T divisor) {
    throw "Not Implemented";
}

template <> inline std::pair<uint64, uint64> udivmod(uint64 high_dividend, uint64 low_dividend, uint64 divisor) {
    uint64 remainder;
    uint64 result = _udiv128(high_dividend, low_dividend, divisor, &remainder);
    return std::pair<uint64, uint64>(result, remainder);
}

template <> inline std::pair<uint32, uint32> udivmod(uint32 high_dividend, uint32 low_dividend, uint32 divisor) {
    uint32 remainder;
    uint64 dividend = (uint64(high_dividend) << 32) | low_dividend;
    uint32 result = _udiv64(dividend, divisor, &remainder);
    return std::pair<uint32, uint32>(result, remainder);
}

template <> inline std::pair<uint16, uint16> udivmod(uint16 high_dividend, uint16 low_dividend, uint16 divisor) {
    uint32 dividend = (uint32(high_dividend) << 16) | low_dividend;
    return std::pair<uint16, uint16>(dividend / divisor, dividend % divisor);
}

template <typename T> T shift_left(T high, T low, uint8 shift) {
    throw "Not Implemented";
}

template <> inline uint64 shift_left(uint64 high, uint64 low, uint8 shift) {
    throw __shiftleft128(low, high, shift);
}

template <> inline uint32 shift_left(uint32 high, uint32 low, uint8 shift) {
    return (uint64(low) >> (32 - shift)) | (uint64(high) << shift);
}

template <> inline uint16 shift_left(uint16 high, uint16 low, uint8 shift) {
    return (uint32(low) >> (16 - shift)) | (uint32(high) << shift);
}

template <typename T> T shift_right(T high, T low, uint8 shift) {
    throw "Not Implemented";
}

template <> inline uint64 shift_right(uint64 high, uint64 low, uint8 shift) {
    throw __shiftright128(low, high, shift);
}

template <> inline uint32 shift_right(uint32 high, uint32 low, uint8 shift) {
    return (uint64(low) >> shift) | (uint64(high) << (32 - shift));
}

template <> inline uint16 shift_right(uint16 high, uint16 low, uint8 shift) {
    return (uint32(low) >> shift) | (uint32(high) << (16 - shift));
}

typedef unsigned __int32 Digit;

class BigInteger
{
public:
    BigInteger() {
        size = 1;
        buff = new Digit[1]{ 0 };
    }

    template <typename T> BigInteger(T num) {
        size = digits_for(num);
        if (std::is_unsigned<T>::value) {
            size += 1;
        }

        buff = new Digit[size];
        for (int i = 0; i < digits_for(num); i++) {
            buff[i] = Digit(num >> (i * BITS_PER_DIGIT));
        }
        if (std::is_unsigned<T>::value) {
            buff[digits_for(num)] = 0;
        }
        trim();
    }

    BigInteger(std::pair<Digit, Digit> num) {
        size = 3;
        buff = new Digit[size]{ num.second, num.first, 0 };
        trim();
    }

    BigInteger(const char* str): BigInteger() {
        bool is_negative = false;

        if (*str == '+') {
            ++str;
        }
        else if (*str == '-') {
            is_negative = true;
            ++str;
        }

        for (int i = 0; str[i]; ++i) {
            // TODO: read multiple decimal digits at once
            if (str[i] < '0' || str[i] > '9') break;
            Digit digit = str[i] - '0';
            (*this) *= IO_BASE;
            (*this) += digit;
        }

        if (is_negative) {
            *this = -(*this);
        }

        trim();
    }

    BigInteger(const BigInteger& lnum) {
        size = lnum.size;
        buff = new Digit[size];
        memcpy(buff, lnum.buff, size * sizeof(Digit));
        trim();
    }

    ~BigInteger() {
        delete[] buff;
    }

    BigInteger operator~() const {
        BigInteger res(*this);
        for (unsigned i = 0; i < res.size; ++i)
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
        int m = std::max(this->size, r.size);
        int s = this->sign() != r.sign();
        BigInteger res;
        res.set_size(m + 1 - s);
        unsigned __int8 carry = 0;
        for (unsigned i = 0; i < res.size; ++i) {
            auto result = add_with_carry(carry, (*this)[i], r[i]);
            carry = result.first;
            res.buff[i] = result.second;
        }
        return res;
    }

    BigInteger operator+(Digit r) const {
        return *this + BigInteger(r); // TODO: optimize
    }

    template <typename T> BigInteger operator+(T other) const {
        return *this + BigInteger(other);
    }

    BigInteger& operator+=(const BigInteger& r) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) + r;
        return *this;
    }

    BigInteger& operator+=(Digit r) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) + r;
        return *this;
    }

    template <typename T> BigInteger& operator+=(T other) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) + other;
        return *this;
    }

    BigInteger operator-(const BigInteger& r) const {
        return (*this) + (-r);
    }

    BigInteger operator-(Digit r) const {
        return *this - BigInteger(r); // TODO: optimize
    }

    template <typename T> BigInteger operator-(T other) const {
        return *this - BigInteger(other);
    }

    BigInteger& operator-=(const BigInteger& r) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) - r;
        return *this;
    }

    BigInteger& operator-=(Digit r) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) - r;
        return *this;
    }

    template <typename T> BigInteger& operator-=(T other) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) - other;
        return *this;
    }

    BigInteger operator*(const BigInteger& lnum) const {
        // TODO: implement Karatsuba, maybe?
        BigInteger res;
        BigInteger l = sign() ? -(*this) : (*this);
        BigInteger r = lnum.sign() ? -lnum : lnum;
        int s = sign() ^ lnum.sign();
        for (unsigned i = 0; i < r.size; i++) {
            res += (l * r[i]) << (i * BITS_PER_DIGIT);
        }
        return s ? -res : res;
    }

    BigInteger operator*(Digit r) const {
        BigInteger res;
        bool s = sign();
        BigInteger l_positive = s ? -(*this) : (*this);
        res.set_size(l_positive.size + 1);
        for (unsigned i = 0; i < l_positive.size; i++) {
            BigInteger t = multiply_with_carry(l_positive[i], r);
            res += t << (i * BITS_PER_DIGIT);
        }
        return s ? -res : res;
    }

    template <typename T> BigInteger operator*(T other) const {
        return *this * BigInteger(other);
    }

    BigInteger& operator*=(const BigInteger& r) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) * r;
        return *this;
    }

    BigInteger& operator*=(Digit r) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) * r;
        return *this;
    }

    template <typename T> BigInteger& operator*=(T other) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) * other;
        return *this;
    }

    BigInteger operator/(const BigInteger& lnum) const {
        BigInteger l = sign() ? -(*this) : (*this);
        BigInteger r = lnum.sign() ? -lnum : lnum;
        int s = sign() ^ lnum.sign();
        BigInteger res = l.divmod(r).first;

        return s ? -res : res;
    }

    BigInteger operator/(Digit r) const {
        int s = sign();
        BigInteger res;
        BigInteger l = s ? -(*this) : (*this);

        res.set_size(l.size);
        for (int i = l.size; i > 0; --i) {
            auto result = udivmod(l[i], l[i - 1], r);
            res.buff[i - 1] = result.first;
            l.buff[i - 1] = result.second;
        }
        return s ? -res : res;
    }

    template <typename T> BigInteger operator/(T other) const {
        return *this / BigInteger(other);
    }

    BigInteger& operator/=(const BigInteger& r) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) / r;
        return *this;
    }

    BigInteger& operator/=(Digit r) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) / r;
        return *this;
    }

    template <typename T> BigInteger& operator/=(T other) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) / other;
        return *this;
    }

    BigInteger operator%(const BigInteger& lnum) const {
        BigInteger l = sign() ? -(*this) : (*this);
        BigInteger r = lnum.sign() ? -lnum : lnum;
        int s = sign() ^ lnum.sign();
        BigInteger res = l.divmod(r).second;

        return s ? -res : res;
    }

    BigInteger operator%(Digit r) const {
        // TODO: optimize -- implement a divmod method and use it here
        return (*this) - (((*this) / r) * r);
    }

    template <typename T> BigInteger operator%(T other) const {
        return *this % BigInteger(other);
    }

    BigInteger& operator%=(const BigInteger& r) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) % r;
        return *this;
    }

    BigInteger& operator%=(Digit r) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) % r;
        return *this;
    }

    template <typename T> BigInteger& operator%=(T other) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) % other;
        return *this;
    }

    bool operator<(const BigInteger& other) const {
        // TODO: optimize -- do not iterate numbers of different sizes
        for (int i = std::max(this->size, other.size) - 1; i >= 0; i--) {
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
        for (unsigned i = 0; i < std::max(this->size, other.size); i++) {
            if ((*this)[i] != other[i]) {
                return false;
            }
        }

        return true;
    }

    bool operator!=(const BigInteger& r) const {
        return !(*this == r);
    }

    const BigInteger& operator=(const BigInteger& number) {
        if (this != &number) {
            size = number.size;
            delete[] buff;
            buff = new Digit[size];
            memcpy(buff, number.buff, size * sizeof(Digit));
        }
        return *this;
    }

    template <typename T> const BigInteger& operator=(T number) {
        std::swap(*this, BigInteger(number));
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

    explicit operator Digit() const {
        return buff[0];
    }

    explicit operator bool() const {
        for (unsigned i = 0; i < this->size; i++) {
            if ((*this)[i] != 0) {
                return true;
            }
        }

        return false;
    }

    BigInteger operator++(int) {
        BigInteger old = *this;
        ++(*this);
        return old;
    }

    BigInteger& operator++() {
        for (unsigned i = 0; i < size; ++i) {
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
        for (unsigned i = 0; i < size - 1; ++i) {
            buff[i] -= 1;

            if (buff[i] != Digit(~0ll)) {
                return *this;
            }
        }

        int leading_hword_index = size - 1;
        if (buff[leading_hword_index] == (Digit(1) << (BigInteger::BITS_PER_DIGIT - 1))) {
            set_size(size + 1);
        }
        buff[leading_hword_index] -= 1;

        return *this;
    }

    // TODO: implement & | ^ &= |= ^=

    BigInteger operator<<(unsigned shift) const {
        unsigned hword_shift = shift / BITS_PER_DIGIT;
        unsigned bit_shift = shift % BITS_PER_DIGIT;

        BigInteger res;
        res.set_size(size + hword_shift + 1);
        res.buff[hword_shift] = shift_left((*this)[0], Digit(0), bit_shift);
        for (unsigned i = 0; i < size; ++i) {
            res.buff[hword_shift + i + 1] = shift_left((*this)[i + 1], (*this)[i], bit_shift);
        }
        return res;
    }

    BigInteger operator>>(unsigned shift) const {
        unsigned hword_shift = shift / BITS_PER_DIGIT;
        unsigned bit_shift = shift % BITS_PER_DIGIT;

        if (hword_shift >= size) return sign() ? -1 : 0;
        BigInteger res;
        res.set_size(size - hword_shift);
        for (unsigned i = 0; i < res.size; ++i) {
            res.buff[res.size - i - 1] = shift_right((*this)[size - i], (*this)[size - i - 1], bit_shift);
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
        // TODO: Can this be optimized by implementing division via multiplication https://research.swtch.com/divmult?
        if (!other) {
            // This will blow up with a CPU error
            return std::pair<BigInteger, BigInteger>(1 / other[0], 1 % other[0]);
        }

        BigInteger scaled_divisor = other;
        BigInteger remain = *this;
        BigInteger result;
        BigInteger multiple = Digit(1);

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
        } while (multiple);

        return std::pair<BigInteger, BigInteger>(result, remain);
    }

    void printbin() const {
        for (int i = size - 1; i >= 0; --i) {
            for (int bit_index = BITS_PER_DIGIT - 1; bit_index >= 0; bit_index--) {
                Digit mask = 1 << bit_index;
                std::cout << ((buff[i] & mask) ? 1 : 0);
            }
        }
    }

    void printhex() const {
        for (int i = size - 1; i >= 0; --i) {
            std::cout << std::hex << buff[i];
        }
    }

    static const Digit IO_BASE = 10;
    static const size_t BITS_PER_DIGIT = sizeof(Digit) * 8;
private:

    Digit operator[](unsigned position) const {
        if (position >= size) return sign() ? ~0 : 0;
        return this->buff[position];
    }

    void set_size(unsigned new_size) {
        Digit* new_buff = new Digit[new_size];
        memcpy(new_buff, buff, std::min(size, new_size) * sizeof(Digit));
        for (unsigned i = size; i < new_size; ++i) new_buff[i] = sign() ? ~0 : 0;
        delete[] buff;

        buff = new_buff;
        size = new_size;
    }

    void trim() {
        Digit s = sign() ? ~0 : 0;
        unsigned i;
        for (i = 0; i < size - 1 && buff[size - i - 1] == s; ++i) {
            if ((s != 0 && oldest_bit(size - i - 2) == 0) || (s == 0 && oldest_bit(size - i - 2) != 0)) {
                break;
            }
        }
        set_size(size - i);
    }

    bool sign() const {
        return oldest_bit(size - 1);
    }

    bool oldest_bit(unsigned digit_index) const {
        return !!(buff[digit_index] & (1 << (BITS_PER_DIGIT - 1)));
    }

    template <typename T> static int digits_for(T number) {
        return std::max(size_t(1), sizeof(number) / sizeof(Digit));
    }

    Digit* buff;
    unsigned size;
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
        buff = new T[BigInteger::BITS_PER_DIGIT * ln_positive.size];
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
