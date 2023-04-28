//
// Created by Joe Yu on 4/25/23.
//

#include "Integer.h"

const BigInteger &Integer::asBigInteger() const {
    if (!useBigInt) {
        throw std::runtime_error("Integer is using BigInteger");
    }
    return bigIntegerValue;
}

long long Integer::asLongLong() const {
    if (useBigInt) {
        throw std::runtime_error("Integer is using BigInteger");
    }
    return intValue;
}

Integer &Integer::operator++() {
    if (!useBigInt) {
        if (intValue == std::numeric_limits<long long>::max()) {
            changeToBigInt();
        } else {
            ++intValue;
            return *this;
        }
    }
    ++bigIntegerValue;
    return *this;
}


Integer &Integer::operator--() {
    if (!useBigInt) {
        if (intValue == std::numeric_limits<long long>::min()) {
            changeToBigInt();
        } else {
            --intValue;
            return *this;
        }
    }
    --bigIntegerValue;
    return *this;
}

Integer Integer::operator++(int) {
    Integer oldValue(*this); // Make a copy of the current object
    ++*this;                 // Increment the value
    return oldValue;         // Return the old value (before decrement
}

Integer Integer::operator--(int) {
    Integer oldValue(*this); // Make a copy of the current object
    --*this;                 // Decrement the value
    return oldValue;         // Return the old value (before decrement
}

bool Integer::operator<(const Integer &h) const {
    if (useBigInt) {
        if (h.useBigInt) return bigIntegerValue < h.bigIntegerValue;
        else return BigInteger(intValue) < h.bigIntegerValue;
    } else {
        if (h.useBigInt) return bigIntegerValue < BigInteger(h.intValue);
        else return intValue < h.intValue;
    }
}

bool Integer::operator<=(const Integer &h) const {
    if (*this == h) return true;
    return *this < h;
}

bool Integer::operator>(const Integer &h) const {
    if (useBigInt) {
        if (h.useBigInt) return bigIntegerValue > h.bigIntegerValue;
        else return BigInteger(intValue) > h.bigIntegerValue;
    } else {
        if (h.useBigInt) return bigIntegerValue > BigInteger(h.intValue);
        else return intValue > h.intValue;
    }
}

bool Integer::operator>=(const Integer &h) const {
    if (*this == h) return true;
    return *this > h;
}

bool Integer::operator==(const Integer &h) const {
    if (useBigInt) {
        if (h.useBigInt) return h.bigIntegerValue == bigIntegerValue;
        else return BigInteger(h.intValue) == bigIntegerValue;
    } else {
        if (h.useBigInt) return h.bigIntegerValue == BigInteger(intValue);
        else return h.intValue == intValue;
    }
}

bool Integer::operator!=(const Integer &h) const {
    return !(*this == h); // Reuse the == operator
}


Integer Integer::operator+(const Integer &h) const {
    Integer rtn(*this);
    rtn += h;
    return rtn;
}

Integer Integer::operator-(const Integer &h) const {
    Integer rtn(*this);
    rtn -= h;
    return rtn;
}

Integer Integer::operator*(const Integer &h) const {
    Integer rtn(*this);
    rtn *= h;
    return rtn;
}

Integer Integer::operator/(const Integer &h) const {
    Integer rtn(*this);
    rtn /= h;
    return rtn;
}

Integer Integer::operator%(const Integer &h) const {
    Integer rtn(*this);
    rtn %= h;
    return rtn;
}

Integer Integer::operator<<(const Integer &h) const {
    Integer rtn(*this);
    rtn <<= h;
    return rtn;
}

Integer Integer::operator>>(const Integer &h) const {
    Integer rtn(*this);
    rtn >>= h;
    return rtn;
}

Integer &Integer::operator+=(const Integer &h) {
    if (usingBigInteger() || h.usingBigInteger()) {
        if(usingBigInteger() && !h.usingBigInteger()) *this = Integer(asBigInteger() + h.asLongLong());
        else if(!usingBigInteger() && h.usingBigInteger()) *this = Integer( h.asBigInteger() + asLongLong() );
        else *this = Integer(asBigInteger() + h.asBigInteger());
    } else {
        long long result;
        if (addition_overflow(asLongLong(), h.asLongLong(), result)) {
            *this = Integer(BigInteger(asLongLong()) + BigInteger(h.asLongLong()));
        } else {
            intValue = result;
        }
    }
    return *this;
}

Integer &Integer::operator-=(const Integer &h) {
    if (usingBigInteger() || h.usingBigInteger()) {
        if(usingBigInteger() && !h.usingBigInteger()) *this = Integer(asBigInteger() - h.asLongLong());
        else if(!usingBigInteger() && h.usingBigInteger()) *this = Integer( h.asBigInteger() - asLongLong() );
        else *this = Integer(asBigInteger() - h.asBigInteger());
    } else {
        long long result;
        if (addition_overflow(asLongLong(), -h.asLongLong(), result)) {
            *this = Integer(BigInteger(asLongLong()) - BigInteger(h.asLongLong()));
        } else {
            intValue = result;
        }
    }
    return *this;
}

Integer &Integer::operator*=(const Integer &h) {
    if (usingBigInteger() || h.usingBigInteger()) {
        if(usingBigInteger() && !h.usingBigInteger()) *this = Integer(asBigInteger() * h.asLongLong());
        else if(!usingBigInteger() && h.usingBigInteger()) *this = Integer( h.asBigInteger() * asLongLong() );
        else *this = Integer(asBigInteger() * h.asBigInteger());
    } else {
        long long result;
        if (multiplication_overflow(asLongLong(), h.asLongLong(), result)) {
            *this = Integer(BigInteger(asLongLong()) * BigInteger(h.asLongLong()));
        } else {
            intValue = result;
        }
    }
    return *this;
}

Integer &Integer::operator/=(const Integer &h) {
    if (usingBigInteger() || h.usingBigInteger()) {
        if(usingBigInteger() && !h.usingBigInteger()) *this = Integer(asBigInteger() / h.asLongLong());
        else if(!usingBigInteger() && h.usingBigInteger()) *this = Integer( h.asBigInteger() / asLongLong() );
        else *this = Integer(asBigInteger() / h.asBigInteger());
    } else {
        if (asLongLong() == std::numeric_limits<long long>::min() || h.asLongLong() == -1) {
            *this = Integer(LONGMAX() + 1);
        }else intValue /= h.asLongLong();
    }
    return *this;
}

Integer &Integer::operator%=(const Integer &h) {
    if (usingBigInteger() || h.usingBigInteger()) {
        if(usingBigInteger() && !h.usingBigInteger()) *this = Integer(asBigInteger() % h.asLongLong());
        else if(!usingBigInteger() && h.usingBigInteger()) *this = Integer( h.asBigInteger() % asLongLong() );
        else *this = Integer(asBigInteger() % h.asBigInteger());
    } else {
        intValue %= h.asLongLong();
    }
    return *this;
}

Integer &Integer::operator<<=(const Integer &h) {
    for (Integer i = 0; i < h; ++i) {
        (*this) *= 2;
    }
    return *this;
}

Integer &Integer::operator>>=(const Integer &h) {
    for (Integer i = 0; i < h; ++i) {
        (*this) /= 2;
    }
    return *this;
}

void Integer::changeToBigInt() {
    bigIntegerValue = BigInteger(intValue);
    useBigInt = true;
}

std::optional<long long> Integer::changeToLongLong() {
    if (!useBigInt) return intValue;
    if (bigIntegerValue > LONGMAX() || bigIntegerValue < LONGMIN()) {
        return {};
    }
    useBigInt = false;
    intValue = 0;
    long long mul = 1;
    for (int i = 0; i < bigIntegerValue.size(); ++i) {
        intValue += bigIntegerValue[i] * mul;
        mul *= 10;
    }
    return intValue;
}

bool Integer::addition_overflow(long long a, long long b, long long &result) {
    if ((b > 0) && (a > std::numeric_limits<long long>::max() - b)) {
        return true;
    }
    if ((b < 0) && (a < std::numeric_limits<long long>::min() - b)) {
        return true;
    }
    result = a + b;
    return false;
}

bool Integer::multiplication_overflow(long long a, long long b, long long &result) {
    if (a > 0) {
        if (b > 0) {
            if (a > std::numeric_limits<long long>::max() / b) {
                return true;
            }
        } else {
            if (b < std::numeric_limits<long long>::min() / a) {
                return true;
            }
        }
    } else {
        if (b > 0) {
            if (a < std::numeric_limits<long long>::min() / b) {
                return true;
            }
        } else {
            if ((a != 0) && (b < std::numeric_limits<long long>::max() / a)) {
                return true;
            }
        }
    }
    result = a * b;
    return false;
}

std::string Integer::toString() const {
    if(!useBigInt) return std::to_string(intValue);
    return bigIntegerValue.toString();
}
