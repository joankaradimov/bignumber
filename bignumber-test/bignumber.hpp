#pragma once

#include <algorithm>
#include <stdlib.h>
#include <intrin.h>
#include <iostream>

typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

template <typename T> struct doubled_size {};
template <> struct doubled_size<uint8> { typedef uint16 type; };
template <> struct doubled_size<uint16> { typedef uint32 type; };
template <> struct doubled_size<uint32> { typedef uint64 type; };

template <typename T> std::pair<T, T> multiply_with_carry(T a, T b) {
    doubled_size<T>::type result = a;
    result *= b;
    return std::pair<T, T>(result >> (sizeof(T) * 8), result);
}

template <> inline std::pair<uint64, uint64> multiply_with_carry(uint64 a, uint64 b) {
    uint64 result_high;
    uint64 result_low = _umul128(a, b, &result_high);
    return std::pair<uint64, uint64>(result_high, result_low);
}

template <typename T> std::pair<uint8, T> add_with_carry(uint8 carry, T a, T b) {
    doubled_size<T>::type result = carry;
    result += a;
    result += b;
    return std::pair<uint8, T>(result >> (sizeof(T) * 8), result);
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
    doubled_size<T>::type dividend = (doubled_size<T>::type(high_dividend) << (sizeof(T) * 8)) | low_dividend;
    return std::pair<T, T>(dividend / divisor, dividend % divisor);
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

template <typename T> T shift_left(T high, T low, uint8 shift) {
    return (doubled_size<T>::type(low) >> (sizeof(T) * 8 - shift)) | (doubled_size<T>::type(high) << shift);
}

template <> inline uint64 shift_left(uint64 high, uint64 low, uint8 shift) {
    return __shiftleft128(low, high, shift);
}

template <typename T> T shift_right(T high, T low, uint8 shift) {
    return (doubled_size<T>::type(low) >> shift) | (doubled_size<T>::type(high) << (sizeof(T) * 8 - shift));
}

template <> inline uint64 shift_right(uint64 high, uint64 low, uint8 shift) {
    return __shiftright128(low, high, shift);
}

typedef unsigned __int32 Digit;

class BigInteger
{
public:
    BigInteger() {
        size = 1;
        buff = new Digit[1]{ 0 };
    }

    template <typename T, typename std::enable_if_t<std::is_integral_v<T>&& std::is_signed_v<T>>* = nullptr> BigInteger(T num) {
        size = digits_for(num);
        buff = new Digit[size];
        for (unsigned i = 0; i < size; i++) {
            buff[i] = Digit(num >> (i * BITS_PER_DIGIT));
        }

        trim();
    }

    template <typename T, typename std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>>* = nullptr> BigInteger(T num) {
        size = digits_for(num) + 1;
        buff = new Digit[size];
        for (unsigned i = 0; i < size - 1; i++) {
            buff[i] = Digit(num >> (i * BITS_PER_DIGIT));
        }
        buff[size - 1] = 0;

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

    template <typename T, typename std::enable_if_t<std::is_integral_v<T>>* = nullptr> BigInteger operator+(T other) const {
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

    template <typename T, typename std::enable_if_t<std::is_integral_v<T>>* = nullptr> BigInteger& operator+=(T other) {
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

    template <typename T, typename std::enable_if_t<std::is_integral_v<T>>* = nullptr> BigInteger operator-(T other) const {
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

    template <typename T, typename std::enable_if_t<std::is_integral_v<T>>* = nullptr> BigInteger& operator-=(T other) {
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

    template <typename T, typename std::enable_if_t<std::is_integral_v<T>>* = nullptr> BigInteger operator*(T other) const {
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

    template <typename T, typename std::enable_if_t<std::is_integral_v<T>>* = nullptr> BigInteger& operator*=(T other) {
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

    template <typename T, typename std::enable_if_t<std::is_integral_v<T>>* = nullptr> BigInteger operator/(T other) const {
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

    template <typename T, typename std::enable_if_t<std::is_integral_v<T>>* = nullptr> BigInteger& operator/=(T other) {
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

    template <typename T, typename std::enable_if_t<std::is_integral_v<T>>* = nullptr> BigInteger operator%(T other) const {
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

    template <typename T, typename std::enable_if_t<std::is_integral_v<T>>* = nullptr> BigInteger& operator%=(T other) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) % other;
        return *this;
    }

    bool operator<(const BigInteger& other) const {
        if (this->sign() != other.sign()) {
            return this->sign();
        }

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

    template <typename T, typename std::enable_if_t<std::is_integral_v<T>>* = nullptr> const BigInteger& operator=(T number) {
        BigInteger temporary = number;
        std::swap(*this, temporary);
        return *this;
    }

    explicit operator std::string() const {
        if (!*this) {
            return "0";
        }

        std::string result;
        bool is_negative = sign();
        BigInteger temporary = is_negative ? -*this : *this;

        for (int i = 0; temporary; ++i) {
            result.push_back('0' + Digit(temporary % BigInteger::IO_BASE));
            temporary /= BigInteger::IO_BASE;
        }

        if (is_negative) {
            result.push_back('-');
        }

        std::reverse(result.begin(), result.end());
        return result;
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
                Digit mask = Digit(1) << bit_index;
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
        return !!(buff[digit_index] & (Digit(1) << (BITS_PER_DIGIT - 1)));
    }

    template <typename T> constexpr static unsigned digits_for(T number) {
        return std::max<unsigned>(1, sizeof(number) / sizeof(Digit));
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

template <typename T> std::basic_ostream<T>& operator<<(std::basic_ostream<T>& os, const BigInteger& number) {
    return os << std::string(number);
}

template <typename T> std::basic_istream<T>& operator>>(std::basic_istream<T>& is, BigInteger& ln) {
    ln = 0;
    int is_negative = false;
    T ch;
    is >> ch;
    if (ch == '-') {
        is_negative = true;
        is >> ch;
    }
    else if (ch == '+') {
        is >> ch;
    }

    for (;;) {
        ln *= BigInteger::IO_BASE;
        ln += ch - '0';
        ch = is.peek();
        if (ch < '0' || ch > '9' || isspace(ch)) break;
        is >> ch;
    }
    if (is_negative) ln = -ln;
    return is;
}
