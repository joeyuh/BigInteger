//
// Created by Joe Yu on 4/24/23.
//
#include "BigInteger.h"

BigInteger BigInteger::operator+(const BigInteger &h) const {
    BigInteger rtn(*this);
    rtn += h;
    return rtn;
}

BigInteger BigInteger::operator-(const BigInteger &h) const {
    BigInteger rtn(*this);
    rtn -= h;
    return rtn;
}

BigInteger BigInteger::operator*(const BigInteger &h) const {
    BigInteger rtn(*this);
    rtn *= h;
    return rtn;
}

BigInteger BigInteger::operator/(const BigInteger &h) const {
    BigInteger rtn(*this);
    rtn /= h;
    return rtn;
}

BigInteger BigInteger::operator%(const BigInteger &h) const {
    BigInteger rtn(*this);
    rtn %= h;
    return rtn;
}

BigInteger BigInteger::operator<<(const BigInteger &h) const {
    BigInteger rtn(*this);
    rtn <<= h;
    return rtn;
}

BigInteger BigInteger::operator>>(const BigInteger &h) const {
    BigInteger rtn(*this);
    rtn >>= h;
    return rtn;
}


BigInteger &BigInteger::operator+=(const BigInteger &h) {
    if (negative == h.negative) {
        selfPlus(h);
    } else {
        if (compareAbsolute(*this, h) >= 0) {
            selfMinus(h);
        } else {
            BigInteger temp = h;
            temp.selfMinus(*this);
            *this = temp;
        }
    }
    return *this;
}

void BigInteger::selfPlus(const BigInteger &h) {
    char carry = 0;
    size_t n = std::max(data.size(), h.data.size());
    std::vector<char> result;

    for (size_t i = 0; i < n; i++) {
        char aValue = i < data.size() ? data[i] : (char) 0;
        char bValue = i < h.data.size() ? h.data[i] : (char) 0;

        char sum = aValue + bValue + carry;
        carry = sum / 10;
        result.push_back(sum % (char) 10);
    }

    if (carry > 0) {
        result.push_back(carry);
    }
    data = std::move(result);
}


BigInteger &BigInteger::operator-=(const BigInteger &h) {
    // If the operands have different signs, delegate to addition.
    if (negative != h.negative) {
        negative = !negative;
        selfPlus(h);
    }

    // If the current number is smaller than the given number, swap them and negate the result.
    if (compareAbsolute(*this, h) < 0) {
        BigInteger temp = h;
        temp -= *this;
        *this = temp;
        negative = !negative;
    } else selfMinus(h);

    return *this;
}

void BigInteger::selfMinus(const BigInteger &h) {
    int carry = 0;
    for (size_t i = 0; i < h.data.size() || carry; ++i) {
        if (i < h.data.size()) {
            data[i] -= h.data[i] + carry;
        } else {
            data[i] -= carry;
        }
        if (data[i] < 0) {
            carry = 1;
            data[i] += 10;
        } else {
            carry = 0;
        }
    }

    // Remove leading zeros.
    while (data.size() > 1 && data.back() == 0) {
        data.pop_back();
    }

    // Check for zero.
    if (data.size() == 1 && data[0] == 0) {
        negative = false;
    }
}

BigInteger &BigInteger::operator*=(const BigInteger &h) {
    negative = negative ^ h.negative;
    std::vector<char> res(data.size() + h.data.size());
    std::fill(res.begin(), res.end(), 0);

    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < h.data.size(); j++) {
            res[i + j] += data[i] * h.data[j];
            res[i + j + 1] += res[i + j] / 10;
            res[i + j] %= 10;
        }
    }

    while (res.size() > 1 && res.back() == 0) {
        res.pop_back();
    }

    data = std::move(res);
    return *this;
}

BigInteger &BigInteger::operator/=(const BigInteger &h) {
    BigInteger quotient = ZERO(), remainder = ZERO();
    divideAndRemainder(h, quotient, remainder);
    *this = quotient;
    return *this;
}

BigInteger &BigInteger::operator%=(const BigInteger &h) {
    BigInteger quotient = ZERO(), remainder = ZERO();
    divideAndRemainder(h, quotient, remainder);
    *this = remainder;
    return *this;
}

BigInteger &BigInteger::operator<<=(const BigInteger &h) {
    for (BigInteger i = ZERO(); i < h; ++i) {
        (*this) *= TWO();
    }
    return *this;
}

BigInteger &BigInteger::operator>>=(const BigInteger &h) {
    for (BigInteger i = ZERO(); i < h; ++i) {
        (*this) /= TWO();
    }
    return *this;
}

bool BigInteger::operator>(const BigInteger &h) const {
    if (negative && h.negative) {
        return compareAbsolute(h, *this) > 0;
    } else if (!negative && !h.negative) {
        return compareAbsolute(h, *this) < 0;
    } else if (!negative && h.negative) {
        return true;
    } else {
        return false;
    }
}

bool BigInteger::operator<(const BigInteger &h) const {
    if (negative && h.negative) {
        return compareAbsolute(h, *this) < 0;
    } else if (!negative && !h.negative) {
        return compareAbsolute(h, *this) > 0;
    } else if (!negative && h.negative) {
        return false;
    } else {
        return true;
    }
}

bool BigInteger::operator<=(const BigInteger &h) const {
    if (*this == h) return true;
    return *this < h;
}

bool BigInteger::operator>=(const BigInteger &h) const {
    if (*this == h) return true;
    return *this > h;
}

bool BigInteger::operator==(const BigInteger &h) const {
    return (negative == h.negative) && (compareAbsolute(*this, h) == 0);
}

bool BigInteger::operator!=(const BigInteger &h) const {
    return !(*this == h); // Reuse the == operator
}

BigInteger &BigInteger::operator++() {
    *this += 1;
    return *this;
}

BigInteger &BigInteger::operator--() {
    *this -= 1;
    return *this;
}

BigInteger BigInteger::operator++(int) {
    BigInteger oldValue(*this); // Make a copy of the current object
    ++*this;                 // Increment the value
    return oldValue;         // Return the old value (before decrement
}

BigInteger BigInteger::operator--(int) {
    BigInteger oldValue(*this); // Make a copy of the current object
    --*this;                 // Decrement the value
    return oldValue;         // Return the old value (before decrement
}


char BigInteger::compareAbsolute(const BigInteger &num1, const BigInteger &num2) {
    if (num1.data.size() > num2.data.size()) return 1;
    if (num1.data.size() < num2.data.size()) return -1;

    for (size_t i = num1.data.size() - 1;; --i) {
        if (num1.data[i] > num2.data[i]) return 1;
        if (num1.data[i] < num2.data[i]) return -1;
        if (i == 0) return 0;
    }
}

std::string BigInteger::toString() const {
    std::string rtn;
    if (negative) rtn = "-";
    for (auto it = data.rbegin(); it < data.rend(); ++it) {
        rtn += ('0' + *it);
    }
    return rtn;
}

void BigInteger::divideAndRemainder(const BigInteger &divisor, BigInteger &quotient, BigInteger &remainder) const {
    if (divisor == ZERO()) {
        throw std::runtime_error("Division by zero");
    }
    if (*this == ZERO()) {
        quotient = ZERO();
        remainder = ZERO();
        return;
    }

    BigInteger dividend = *this;
    dividend.negative = false;
    BigInteger tmpDivisor = divisor;
    tmpDivisor.negative = false;

    quotient = ZERO();
    remainder = dividend;

    while (remainder >= tmpDivisor) {
        BigInteger currentDivisor = tmpDivisor;
        BigInteger currentQuotient(1);

        while ((currentDivisor * TWO()) <= remainder) {
            currentDivisor *= TWO();
            currentQuotient *= TWO();
        }

        remainder -= currentDivisor;
        quotient += currentQuotient;
    }

    quotient.negative = (this->negative != divisor.negative);
    remainder.negative = this->negative;
}

char BigInteger::at(size_t index) const {
    return data[data.size() - 1 - index];
}

char &BigInteger::operator[](size_t index) {
    return data[data.size() - 1 - index];
}

size_t BigInteger::size() const {
    return data.size();
}
