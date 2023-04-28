//
// Created by Joe Yu on 4/24/23.
//
#include <vector>
#include <type_traits>
#include <string>
#include <algorithm>
#include <stdexcept>

#ifndef BIGINTEGER_BIGINTEGER_H
#define BIGINTEGER_BIGINTEGER_H

class BigInteger {
public:
    static BigInteger ZERO() {
        static BigInteger zeroInstance(0);
        return zeroInstance;
    }

    static BigInteger ONE() {
        static BigInteger oneInstance(1);
        return oneInstance;
    }

    static BigInteger TWO() {
        static BigInteger twoInstance(2);
        return twoInstance;
    }

    template<typename T, typename std::enable_if<std::is_integral<T>::value>::type * = nullptr>
    BigInteger(T x) {
        if (x < 0) {
            x = -x;
            negative = true;
        } else negative = false;
        while (x > 0) {
            data.push_back((char) (x % 10));
            x /= 10;
        }
        if (data.empty()) data.push_back(0);
    }

    template<typename T, typename std::enable_if<std::is_integral<T>::value>::type * = nullptr>
    BigInteger(const std::vector<T> &input_vector, bool negative=false) : negative(negative){
        for (const T &value: input_vector) {
            if (value > 9 || value < 0) throw std::runtime_error("Invalid integer");
            data.push_back(static_cast<char>(value));
        }
    }

    // Constructor for std::string containing numbers
    BigInteger(std::string number_string) {
        if (number_string.empty()) {
            data.push_back(0);
            negative = false;
            return;
        }
        if (number_string[0] == '-') {
            negative = true;
            number_string = number_string.substr(1);
            if (number_string.empty()) {
                throw std::runtime_error("Invalid integer");
            } else if (number_string[0] == '0') {
                data.push_back(0);
                negative = false;
                return;
            }
        } else negative = false;
        for (const char &c: number_string) {
            // Subtract '0' from each character and push the result into the char_vector
            if (c > '9' || c < '0') throw std::runtime_error("Invalid integer");
            data.push_back(c - '0');
        }
        std::reverse(data.begin(), data.end());
    }

    BigInteger(): BigInteger(0) {}

    BigInteger &operator++();

    BigInteger &operator--();

    BigInteger operator++(int);

    BigInteger operator--(int);

    bool operator<(const BigInteger &h) const;

    bool operator>(const BigInteger &h) const;

    bool operator<=(const BigInteger &h) const;

    bool operator>=(const BigInteger &h) const;

    bool operator==(const BigInteger &h) const;

    bool operator!=(const BigInteger &h) const;


    BigInteger operator+(const BigInteger &h) const;

    BigInteger operator-(const BigInteger &h) const;

    BigInteger operator*(const BigInteger &h) const;

    BigInteger operator/(const BigInteger &h) const;

    BigInteger operator%(const BigInteger &h) const;

    BigInteger operator<<(const BigInteger &h) const;

    BigInteger operator>>(const BigInteger &h) const;

    BigInteger &operator+=(const BigInteger &h);

    BigInteger &operator-=(const BigInteger &h);

    BigInteger &operator*=(const BigInteger &h);

    BigInteger &operator/=(const BigInteger &h);

    BigInteger &operator%=(const BigInteger &h);

    BigInteger &operator<<=(const BigInteger &h);

    BigInteger &operator>>=(const BigInteger &h);


    static char compareAbsolute(const BigInteger &num1, const BigInteger &num2);

    [[nodiscard]] std::string toString() const;

    [[nodiscard]] char at(size_t index) const;

    char& operator[](size_t index);

    size_t size() const;

private:
    bool negative;

    void selfPlus(const BigInteger &h);

    void selfMinus(const BigInteger &h);

    std::vector<char> data;
    void divideAndRemainder
        (const BigInteger &divisor, BigInteger &quotient, BigInteger &remainder) const;

};

#endif //BIGINTEGER_BIGINTEGER_H
