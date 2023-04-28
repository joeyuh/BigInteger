//
// Created by Joe Yu on 4/25/23.
//

#ifndef HIGHPRECISION_INTEGER_H
#define HIGHPRECISION_INTEGER_H


#include "BigInteger.h"
#include <limits>
#include <type_traits>
#include <optional>

class Integer {
public:
    static BigInteger LONGMAX () {
        static BigInteger maxInstance(std::numeric_limits<long long>::max());
        return maxInstance;
    }

    static BigInteger LONGMIN () {
        static BigInteger minInstance(std::numeric_limits<long long>::min());
        return minInstance;
    }

    template<typename T, typename std::enable_if<std::is_integral<T>::value>::type * = nullptr>
    Integer(T value = 0) : intValue(value), bigIntegerValue(), useBigInt(false) {}

    Integer(const BigInteger &value) : intValue(0), bigIntegerValue(value), useBigInt(true) {}

    Integer &operator=(long long value) {
        intValue = value;
        useBigInt = false;
        return *this;
    }

    Integer &operator=(const BigInteger &value) {
        bigIntegerValue = value;
        useBigInt = true;
        return *this;
    }

    [[nodiscard]] bool usingBigInteger() const { return useBigInt; }

    [[nodiscard]] long long asLongLong() const;

    [[nodiscard]] const BigInteger &asBigInteger() const;
    // Add other operators, e.g. +, -, *, /, etc., with overflow checking and BigInteger conversion
    Integer &operator++();

    Integer &operator--();

    Integer operator++(int);

    Integer operator--(int);

    bool operator<(const Integer &h) const;

    bool operator>(const Integer &h) const;

    bool operator<=(const Integer &h) const;

    bool operator>=(const Integer &h) const;

    bool operator==(const Integer &h) const;

    bool operator!=(const Integer &h) const;

    Integer operator+(const Integer &h) const;

    Integer operator-(const Integer &h) const;

    Integer operator*(const Integer &h) const;

    Integer operator/(const Integer &h) const;

    Integer operator%(const Integer &h) const;

    Integer operator<<(const Integer &h) const;

    Integer operator>>(const Integer &h) const;

    Integer &operator+=(const Integer &h);

    Integer &operator-=(const Integer &h);

    Integer &operator*=(const Integer &h);

    Integer &operator/=(const Integer &h);

    Integer &operator%=(const Integer &h);

    Integer &operator<<=(const Integer &h);

    Integer &operator>>=(const Integer &h);

    std::optional<long long> changeToLongLong();

    [[nodiscard]] std::string toString() const;

    static bool addition_overflow(long long a, long long b, long long& result);
    static bool multiplication_overflow(long long a, long long b, long long& result);

private:
    long long intValue;
    BigInteger bigIntegerValue;
    bool useBigInt;

    void changeToBigInt();
};


#endif //HIGHPRECISION_INTEGER_H
