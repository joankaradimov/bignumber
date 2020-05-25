#pragma once

#include <algorithm>
#include <stdlib.h>
#include <intrin.h>
#include <iostream>

template <typename Digit> class DigitBuffer {
private:
    struct DigitProxy {
        DigitProxy(DigitBuffer& digits_buffer, unsigned index) : digits_buffer(digits_buffer), index(index) {}

        operator Digit() const {
            if (index >= digits_buffer.size)
                return digits_buffer.sign() ? ~0 : 0;
            return digits_buffer.at(index);
        }

        Digit operator=(Digit digit) {
            if (digits_buffer.size <= index) {
                digits_buffer.set_size(index + 1);
            }
            return digits_buffer.at(index) = digit;
        }

        DigitProxy& operator++() {
            ++digits_buffer.at(index);
            return *this;
        }

        DigitProxy& operator--() {
            --digits_buffer.at(index);
            return *this;
        }

        bool operator==(Digit other) const {
            return digits_buffer.at(index) == other;
        }

        bool operator!=(Digit other) const {
            return digits_buffer.at(index) != other;
        }

        DigitBuffer& digits_buffer;
        unsigned index;
    };

public:
    DigitBuffer(std::initializer_list<Digit> l) {
        size = l.size();
        if (size > IMMEDIATE_BUFFER_SIZE) {
            buffer = new Digit[l.size()];
        }
        int i = 0;
        for (Digit digit : l) {
            at(i++) = digit;
        }
    }

    template <typename T, typename std::enable_if_t<std::is_integral_v<T>&& std::is_signed_v<T>>* = nullptr> DigitBuffer(T number) {
        size = max_digit_count<T>;
        if (size > IMMEDIATE_BUFFER_SIZE) {
            buffer = new Digit[size];
        }
        for (unsigned i = 0; i < size; i++) {
            at(i) = Digit(number >> (i * BITS_PER_DIGIT));
        }

        trim();
    }

    template <typename T, typename std::enable_if_t<std::is_integral_v<T>&& std::is_unsigned_v<T>>* = nullptr> DigitBuffer(T number) {
        size = max_digit_count<T> + 1;
        if (size > IMMEDIATE_BUFFER_SIZE) {
            buffer = new Digit[size];
        }
        for (unsigned i = 0; i < size - 1; i++) {
            at(i) = Digit(number >> (i * BITS_PER_DIGIT));
        }
        at(size - 1) = 0;

        trim();
    }

    DigitBuffer(const DigitBuffer& other) {
        size = other.size;
        if (size > IMMEDIATE_BUFFER_SIZE) {
            buffer = new Digit[size];
        }
        for (unsigned i = 0; i < size; i++) {
            at(i) = other[i];
        }
        trim();
    }

    ~DigitBuffer() {
        if (size > IMMEDIATE_BUFFER_SIZE) {
            delete[] buffer;
        }
    }

    DigitBuffer& operator=(const DigitBuffer& other) {
        if (this != &other) {
            DigitBuffer temporary = other;
            swap(temporary);
        }
        return *this;
    }

    void set_size(unsigned new_size) {
        DigitBuffer old_digits = {};
        swap(old_digits);

        size = new_size;
        if (size > IMMEDIATE_BUFFER_SIZE) {
            buffer = new Digit[size];
        }

        for (unsigned i = 0; i < size; i++) {
            at(i) = old_digits[i];
        }
    }

    unsigned get_size() const {
        return size;
    }

    DigitProxy operator[](unsigned index) {
        return DigitProxy(*this, index);
    }

    Digit operator[](unsigned index) const {
        if (index >= size) {
            return sign() ? ~0 : 0;
        }
        else if (size <= IMMEDIATE_BUFFER_SIZE) {
            return immediate_buffer[index];
        }
        else {
            return buffer[index];
        }
    }

    void trim() {
        Digit s = sign() ? ~0 : 0;
        unsigned i;
        for (i = 0; i < size - 1 && (*this)[size - i - 1] == s; ++i) {
            if ((s != 0 && oldest_bit(size - i - 2) == 0) || (s == 0 && oldest_bit(size - i - 2) != 0)) {
                break;
            }
        }
        set_size(size - i);
    }

    bool sign() const {
        return oldest_bit(size - 1);
    }

    const static constexpr size_t BITS_PER_DIGIT = sizeof(Digit) * 8;

private:

    void swap(DigitBuffer& other) {
        std::swap(buffer, other.buffer);
        std::swap(size, other.size);
    }

    Digit& at(unsigned index) {
        return size <= IMMEDIATE_BUFFER_SIZE ? immediate_buffer[index] : buffer[index];
    }

    bool oldest_bit(unsigned digit_index) const {
        return !!((*this)[digit_index] & (Digit(1) << (BITS_PER_DIGIT - 1)));
    }

    template <typename T> constexpr const static unsigned max_digit_count = std::max<unsigned>(1, sizeof(T) / sizeof(Digit));

    const static constexpr unsigned IMMEDIATE_BUFFER_SIZE = sizeof(Digit*) / sizeof(Digit);

    union {
        Digit* buffer;
        Digit immediate_buffer[IMMEDIATE_BUFFER_SIZE];
    };
    unsigned size;
};

typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

template <typename T> struct doubled_size {};
template <> struct doubled_size<uint8> { typedef uint16 type; };
template <> struct doubled_size<uint16> { typedef uint32 type; };
template <> struct doubled_size<uint32> { typedef uint64 type; };

template <typename T> std::pair<T, T> multiply_with_carry(T a, T b) {
    typename doubled_size<T>::type result = a;
    result *= b;
    return std::pair<T, T>(result >> (sizeof(T) * 8), result);
}

template <> inline std::pair<uint64, uint64> multiply_with_carry(uint64 a, uint64 b) {
    uint64 result_high;
    uint64 result_low = _umul128(a, b, &result_high);
    return std::pair<uint64, uint64>(result_high, result_low);
}

template <typename T> std::pair<uint8, T> add_with_carry(uint8 carry, T a, T b) {
    typename doubled_size<T>::type result = carry;
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
    typename doubled_size<T>::type dividend = (doubled_size<T>::type(high_dividend) << (sizeof(T) * 8)) | low_dividend;
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

typedef unsigned __int64 Digit;

class BigInteger
{
public:
    BigInteger() : digits({ 0 }) { }

    BigInteger(std::pair<Digit, Digit> num) : digits({ num.second, num.first, 0 }) { }

    BigInteger(const BigInteger& other) : digits(other.digits) { }

    template <typename T, typename std::enable_if_t<std::is_integral_v<T>>* = nullptr> BigInteger(T number) : digits(number) { }

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
            Digit digit = Digit(str[i] - '0');
            (*this) *= IO_BASE;
            (*this) += digit;
        }

        if (is_negative) {
            *this = -(*this);
        }

        digits.trim();
    }

    BigInteger operator~() const {
        BigInteger res(*this);
        for (unsigned i = 0; i < res.digits.get_size(); ++i)
            res.digits[i] = ~res.digits[i];
        return res;
    }

    BigInteger operator-() const {
        BigInteger res = ~*this;
        return ++res;
    }

    const BigInteger& operator+() const {
        return *this;
    }

    BigInteger operator+(const BigInteger& r) const {
        int m = std::max(digits.get_size(), r.digits.get_size());
        int s = digits.sign() != r.digits.sign();
        BigInteger res;
        res.digits.set_size(m + 1 - s);
        unsigned __int8 carry = 0;
        for (unsigned i = 0; i < res.digits.get_size(); ++i) {
            auto result = add_with_carry(carry, digits[i], r.digits[i]);
            carry = result.first;
            res.digits[i] = result.second;
        }
        return res;
    }

    BigInteger operator+(Digit other) const {
        BigInteger result = *this;
        return result += other;
    }

    template <typename T, typename std::enable_if_t<std::is_integral_v<T>>* = nullptr> BigInteger operator+(T other) const {
        return *this + BigInteger(other);
    }

    BigInteger& operator+=(const BigInteger& r) {
        // TODO: optimize -- do not create extra instances
        *this = (*this) + r;
        return *this;
    }

    BigInteger& operator+=(Digit other) {
        bool was_negative = digits.sign();
        auto result = add_with_carry<Digit>(0, digits[0], other);
        uint8 carry = result.first;
        digits[0] = result.second;

        for (unsigned i = 1; i < digits.get_size() && carry; ++i) {
            ++digits[i];
            carry = (digits[i] == Digit(0));
        }

        bool is_negative = digits.sign();
        if (is_negative && !was_negative) {
            digits[digits.get_size()] = 0;
        }
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
        BigInteger l = abs();
        BigInteger r = lnum.abs();
        int s = digits.sign() ^ lnum.digits.sign();
        for (unsigned i = r.digits.get_size(); i > 0; --i) {
            res += (l * r.digits[i - 1]) << ((i - 1) * digits.BITS_PER_DIGIT);
        }
        return s ? -res : res;
    }

    BigInteger operator*(Digit r) const {
        BigInteger res;
        bool s = digits.sign();
        BigInteger l_positive = abs();
        res.digits.set_size(l_positive.digits.get_size() + 1);
        for (unsigned i = 0; i < l_positive.digits.get_size(); i++) {
            BigInteger t = multiply_with_carry<Digit>(l_positive.digits[i], r);
            res += t << (i * digits.BITS_PER_DIGIT);
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
        int s = digits.sign() ^ lnum.digits.sign();
        BigInteger result = abs().divmod(lnum.abs()).first;

        return s ? -result : result;
    }

    BigInteger operator/(Digit other) const {
        int s = digits.sign();
        auto result = abs().divmod(other).first;

        return s ? -result : result;
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
        int s = digits.sign() ^ lnum.digits.sign();
        BigInteger result = abs().divmod(lnum.abs()).second;

        return s ? -result : result;
    }

    BigInteger operator%(Digit other) const {
        int s = digits.sign();
        auto result = abs().divmod(other).second;

        return s ? -BigInteger(result) : result;
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
        if (digits.sign() != other.digits.sign()) {
            return digits.sign();
        }

        // TODO: optimize -- do not iterate numbers of different sizes
        for (int i = std::max(digits.get_size(), other.digits.get_size()) - 1; i >= 0; i--) {
            if (digits[i] < other.digits[i]) {
                return true;
            }
            if (digits[i] > other.digits[i]) {
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
        for (unsigned i = 0; i < std::max(digits.get_size(), other.digits.get_size()); i++) {
            if (digits[i] != other.digits[i]) {
                return false;
            }
        }

        return true;
    }

    bool operator!=(const BigInteger& r) const {
        return !(*this == r);
    }

    const BigInteger& operator=(const BigInteger& number) {
        digits = number.digits;
        return *this;
    }

    template <typename T, typename std::enable_if_t<std::is_integral_v<T>>* = nullptr> const BigInteger& operator=(T number) {
        BigInteger temporary = number;
        std::swap(*this, temporary);
        return *this;
    }

    template<typename T> explicit operator std::basic_string<T>() const {
        if (!*this) {
            return "0";
        }

        std::string result;
        bool is_negative = digits.sign();
        BigInteger temporary = abs();

        for (int i = 0; temporary; ++i) {
            auto divmod_result = temporary.divmod(BigInteger::IO_BASE);
            result.push_back('0' + T(divmod_result.second));
            temporary = divmod_result.first;
        }

        if (is_negative) {
            result.push_back('-');
        }

        std::reverse(result.begin(), result.end());
        return result;
    }

    explicit operator long double() const {
        long double result = 0.0;
        long double multiplier = pow(2.0, digits.BITS_PER_DIGIT);
        BigInteger positive = abs();

        for (int i = positive.digits.get_size() - 1; i >= 0; i--) {
            result *= multiplier;
            result += positive.digits[i];
        }
        return digits.sign() ? -result : result;
    }

    explicit operator Digit() const {
        return digits[0];
    }

    explicit operator bool() const {
        for (unsigned i = 0; i < digits.get_size(); i++) {
            if (digits[i] != 0) {
                return true;
            }
        }

        return false;
    }

    BigInteger operator++(int) {
        BigInteger old = *this;
        (*this) += 1;
        return old;
    }

    BigInteger& operator++() {
        return *this += 1;
    }

    BigInteger operator--(int) {
        BigInteger old = *this;
        --(*this);
        return old;
    }

    BigInteger& operator--() {
        for (unsigned i = 0; i < digits.get_size() - 1; ++i) {
            --digits[i];

            if (digits[i] != Digit(~0ll)) {
                return *this;
            }
        }

        int leading_hword_index = digits.get_size() - 1;
        if (digits[leading_hword_index] == (Digit(1) << (digits.BITS_PER_DIGIT - 1))) {
            digits.set_size(digits.get_size() + 1);
        }
        --digits[leading_hword_index];

        return *this;
    }

    // TODO: implement | ^ |= ^=

    BigInteger operator&(const BigInteger& other) const {
        BigInteger result = *this;
        result &= other;
        return result;
    }

    Digit operator&(Digit other) const {
        return digits[0] & other;
    }

    template <typename T, typename std::enable_if_t<std::is_integral_v<T>>* = nullptr> BigInteger operator&(T other) const {
        return *this & BigInteger(other);
    }

    BigInteger& operator&=(const BigInteger& other) {
        digits.set_size(std::min(digits.get_size(), other.digits.get_size()));
        for (int i = 0; i < digits.get_size(); i++) {
            digits[i] = digits[i] & other.digits[i];
        }
        return *this;
    }

    BigInteger& operator&=(Digit other) {
        digits[0] = digits[0] & other;
        digits.set_size(1);
        return *this;
    }

    template <typename T, typename std::enable_if_t<std::is_integral_v<T>>* = nullptr> BigInteger& operator&=(T other) {
        *this &= BigInteger(other);
        return *this;
    }

    BigInteger operator<<(unsigned shift) const {
        unsigned hword_shift = shift / digits.BITS_PER_DIGIT;
        unsigned bit_shift = shift % digits.BITS_PER_DIGIT;

        BigInteger res;
        for (unsigned i = digits.get_size(); i > 0; --i) {
            res.digits[hword_shift + i] = shift_left(digits[i], digits[i - 1], bit_shift);
        }
        res.digits[hword_shift] = shift_left(digits[0], Digit(0), bit_shift);
        return res;
    }

    BigInteger operator>>(unsigned shift) const {
        unsigned hword_shift = shift / digits.BITS_PER_DIGIT;
        unsigned bit_shift = shift % digits.BITS_PER_DIGIT;

        if (hword_shift >= digits.get_size()) return digits.sign() ? -1 : 0;
        BigInteger res;
        for (unsigned i = digits.get_size() - hword_shift; i > 0; --i) {
            res.digits[i - 1] = shift_right(digits[hword_shift + i], digits[hword_shift + i - 1], bit_shift);
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

    std::pair<BigInteger, BigInteger> divmod(const BigInteger& other) const {
        // TODO: Can this be optimized by implementing division via multiplication https://research.swtch.com/divmult?
        if (!other) {
            // This will blow up with a CPU error
            return std::pair<BigInteger, BigInteger>(1 / Digit(other), 1 % Digit(other));
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

    std::pair<BigInteger, Digit> divmod(Digit r) const {
        BigInteger result;
        BigInteger remain = *this;

        for (int i = remain.digits.get_size(); i > 0; --i) {
            auto divmod_resuilt = udivmod<Digit>(remain.digits[i], remain.digits[i - 1], r);
            result.digits[i - 1] = divmod_resuilt.first;
            remain.digits[i - 1] = divmod_resuilt.second;
        }
        return std::pair<BigInteger, Digit>(result, remain);
    }

    void printbin() const {
        for (int i = digits.get_size() - 1; i >= 0; --i) {
            for (int bit_index = digits.BITS_PER_DIGIT - 1; bit_index >= 0; bit_index--) {
                Digit mask = Digit(1) << bit_index;
                std::cout << ((digits[i] & mask) ? 1 : 0);
            }
        }
    }

    void printhex() const {
        for (int i = digits.get_size() - 1; i >= 0; --i) {
            std::cout << std::hex << digits[i];
        }
    }

    BigInteger abs() const {
        return digits.sign() ? -*this : *this;
    }

    static const Digit IO_BASE = 10;

private:
    DigitBuffer<Digit> digits;
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
