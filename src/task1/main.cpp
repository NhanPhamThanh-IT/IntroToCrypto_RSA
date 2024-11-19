#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <vector>

#define MAX_DIGITS 309

struct LargeNumber
{
    int digits[MAX_DIGITS] = {};
    bool is_negative = false;
};

struct DivisionResult
{
    LargeNumber quotient;
    LargeNumber remainder;
};

struct LargeNumberArray
{
    LargeNumber result;
    LargeNumber count;
};

class LargeNumberOperations
{
public:
    LargeNumber convertStringToLargeNumber(std::string& str);
    LargeNumber copyLargeNumber(const LargeNumber& number);
    LargeNumber addLeadingDigit(const LargeNumber& number, int value);
    bool isEqualToZero(const LargeNumber& number);
    LargeNumber addLargeNumbers(LargeNumber first, LargeNumber second);
    LargeNumber subtractLargeNumbers(LargeNumber minuend, LargeNumber subtrahend);
    LargeNumber multiplyLargeNumbers(LargeNumber first, LargeNumber second);
    DivisionResult divideBySmallNumber(LargeNumber dividend, LargeNumber divisor);
    DivisionResult divideByLargeNumber(LargeNumber dividend, LargeNumber divisor);
    LargeNumber modularExponentiation(const LargeNumber& base, const LargeNumber& exponent, const LargeNumber& modulus);
    bool isPrimeNumber(const LargeNumber& number);
};

class BigInteger 
{
private:
    std::string digits;
public:
    BigInteger(std::string&);
    BigInteger(BigInteger&);
    BigInteger(unsigned long long n = 0);
    BigInteger(const char*);
    BigInteger(const BigInteger& a);
    friend bool Null(const BigInteger&);
    int operator[](const int)const;
    std::string toString();
    BigInteger& operator=(const BigInteger&);
    BigInteger& operator++();
    BigInteger operator++(int temp);
    friend BigInteger& operator+=(BigInteger&, const BigInteger&);
    friend BigInteger operator+(const BigInteger&, const BigInteger&);
    BigInteger& operator--();
    BigInteger operator--(int temp);
    friend BigInteger& operator-=(BigInteger&, const BigInteger&);
    friend BigInteger operator-(const BigInteger&, const BigInteger&);
    friend BigInteger& operator*=(BigInteger&, const BigInteger&);
    friend BigInteger operator*(const BigInteger&, const BigInteger&);
    friend BigInteger& operator/=(BigInteger&, const BigInteger&);
    friend BigInteger operator/(const BigInteger&, const BigInteger&);
    friend BigInteger operator%(const BigInteger&, const BigInteger&);
    friend BigInteger& operator%=(BigInteger&, const BigInteger&);
    friend bool operator==(const BigInteger&, const BigInteger&);
    friend bool operator<(const BigInteger&, const BigInteger&);
};

BigInteger convertHexToDecimal(std::string hexValue)
{
    int length = hexValue.size();
    BigInteger base("1");
    BigInteger decimalValue("0");
    for (int i = 0; i < length; i++)
    {
        if (hexValue[i] >= '0' && hexValue[i] <= '9')
        {
            decimalValue += (hexValue[i] - '0') * base;
            base = base * 16;
        }
        else if (hexValue[i] >= 'A' && hexValue[i] <= 'F')
        {
            decimalValue += (hexValue[i] - '7') * base;
            base = base * 16;
        }
    }
    return decimalValue;
}

int main(int argc, char** argv) {
    LargeNumberOperations numberOperations;
    std::string hexInput;
    int outputResult;
    BigInteger decimalValue;
    std::string testFileId;
    if (argc < 3) std::cout << "Not enough Command Line Arguments passed!" << std::endl;
    else {
        std::ifstream inputTestFile(argv[1]);
        inputTestFile >> hexInput;
        inputTestFile.close();
        decimalValue = convertHexToDecimal(hexInput);
        LargeNumber largeNumber;
        std::string largeNumberStr = decimalValue.toString();
        largeNumber = numberOperations.convertStringToLargeNumber(largeNumberStr);
        int result;
        result = numberOperations.isPrimeNumber(largeNumber);
        std::ofstream outputTestFile(argv[2]);
        outputTestFile << result;
        outputTestFile.close();
    }
    return 0;
}

LargeNumber LargeNumberOperations::convertStringToLargeNumber(std::string& str) {
    LargeNumber final;
    bool neg = (str[0] == '-');
    if (neg) str.erase(0, 1);
    reverse(begin(str), end(str));
    int val = 0, itr = 1, Out = 0;
    for (char c : str) {
        val += (c - '0') * itr;
        itr *= 10;
        if (itr == 100) {
            final.digits[Out++] = val;
            itr = 1;
            val = 0;
        }
    }
    if (val) final.digits[Out] = val;
    final.is_negative = neg;
    return final;
}

LargeNumber LargeNumberOperations::copyLargeNumber(const LargeNumber& number) {
    LargeNumber Result;
    Result.is_negative = number.is_negative;
    for (int i = 0; i < MAX_DIGITS; i++)
        Result.digits[i] = number.digits[i];
    return Result;
}

LargeNumber LargeNumberOperations::addLeadingDigit(const LargeNumber& number, int value) {
    LargeNumber Result = number;
    Result.is_negative = number.is_negative;
    for (int i = MAX_DIGITS - 1; i > 0; --i)
        Result.digits[i] = Result.digits[i - 1];
    Result.digits[0] = value;
    return Result;
}

bool LargeNumberOperations::isEqualToZero(const LargeNumber& number)
{
    for (int i = 0; i < MAX_DIGITS; i++)
        if (number.digits[i] != 0)
            return false;
    return true;
}

LargeNumber LargeNumberOperations::addLargeNumbers(LargeNumber first, LargeNumber second)
{
    if (isEqualToZero(first)) return second;
    if (isEqualToZero(second)) return first;
    bool BothNegative = first.is_negative && second.is_negative;
    if (first.is_negative) {
        first.is_negative = false;
        return subtractLargeNumbers(second, first);
    }
    if (second.is_negative) {
        second.is_negative = false;
        return subtractLargeNumbers(first, second);
    }
    LargeNumber Result;
    int carry = 0;
    for (int i = 0; i < MAX_DIGITS; i++) {
        int sum = first.digits[i] + second.digits[i] + carry;
        Result.digits[i] = sum % 100;
        carry = sum / 100;
    }
    if (carry) Result.digits[MAX_DIGITS - 1] = carry;
    Result.is_negative = BothNegative;
    return Result;
}

LargeNumber LargeNumberOperations::subtractLargeNumbers(LargeNumber minuend, LargeNumber subtrahend)
{
    if (isEqualToZero(subtrahend)) return minuend;
    if (isEqualToZero(minuend)) { subtrahend.is_negative = true; return subtrahend; }
    if (subtrahend.is_negative) {
        if (minuend.is_negative) { minuend.is_negative = subtrahend.is_negative = false; return subtractLargeNumbers(subtrahend, minuend); }
        subtrahend.is_negative = false;
        return addLargeNumbers(minuend, subtrahend);
    }
    if (minuend.is_negative) { minuend.is_negative = subtrahend.is_negative = false; LargeNumber result = addLargeNumbers(minuend, subtrahend); result.is_negative = true; return result; }
    LargeNumber Result = minuend;
    bool borrow = false;
    for (int i = 0; i < MAX_DIGITS; i++) {
        int diff = Result.digits[i] - subtrahend.digits[i] - borrow;
        if (diff < 0) { diff += 100; borrow = true; } else borrow = false;
        Result.digits[i] = diff;
    }
    if (borrow) { LargeNumber temp = subtractLargeNumbers(subtrahend, minuend); temp.is_negative = true; return temp; }
    return Result;
}

LargeNumber LargeNumberOperations::multiplyLargeNumbers(LargeNumber first, LargeNumber second)
{
    if (isEqualToZero(first) || isEqualToZero(second)) return LargeNumber();
    bool neg = first.is_negative ^ second.is_negative;
    first.is_negative = second.is_negative = false;
    LargeNumber Result;
    for (int i = 0; i < MAX_DIGITS; i++) {
        LargeNumber temp;
        int carry = 0;
        for (int j = 0; j < MAX_DIGITS; j++) {
            int val = first.digits[i] * second.digits[j] + carry;
            carry = val / 100;
            temp.digits[j] += val % 100;
        }
        for (int k = MAX_DIGITS - 1; k >= i; k--)
            temp.digits[k] = temp.digits[k - i];
        for (int k = 0; k < i; k++) temp.digits[k] = 0;
        Result = addLargeNumbers(Result, temp);
    }
    Result.is_negative = neg;
    return Result;
}

DivisionResult LargeNumberOperations::divideBySmallNumber(LargeNumber dividend, LargeNumber divisor)
{
    DivisionResult Result;
    if (isEqualToZero(dividend))
    {
        Result.quotient = dividend;
        Result.remainder = dividend;
        return Result;
    }
    LargeNumber countArray, One, tempResult = dividend;
    One.digits[0] = 1;
    bool CheckNeg = tempResult.is_negative;
    do
    {
        tempResult = subtractLargeNumbers(tempResult, divisor);
        CheckNeg = tempResult.is_negative;
        if (countArray.digits[0] != 99)
            countArray.digits[0] = countArray.digits[0] + 1;
        else
            countArray = addLargeNumbers(countArray, One);
    } while (!CheckNeg);
    if (countArray.digits[0] != 0)
    {
        countArray.digits[0] = countArray.digits[0] - 1;
        Result.quotient = countArray;
    }
    else
        Result.quotient = subtractLargeNumbers(countArray, One);
    Result.remainder = addLargeNumbers(tempResult, divisor);
    return Result;
}

DivisionResult LargeNumberOperations::divideByLargeNumber(LargeNumber dividend, LargeNumber divisor)
{
    DivisionResult Result, tempResult;
    LargeNumber partOffirst, test;
    if (isEqualToZero(dividend))
    {
        Result.quotient = dividend;
        Result.remainder = dividend;
    }
    bool neg = false;
    if (dividend.is_negative && divisor.is_negative)
    {
        dividend.is_negative = false;
        divisor.is_negative = false;
    }
    else if (dividend.is_negative)
    {
        dividend.is_negative = false;
        neg = true;
    }
    else if (divisor.is_negative)
    {
        divisor.is_negative = false;
        neg = true;
    }
    std::reverse(std::begin(dividend.digits), std::end(dividend.digits));
    int i = 0;
    while (i != 309)
    {
        do
        {
            partOffirst = addLeadingDigit(partOffirst, dividend.digits[i]);
            test = subtractLargeNumbers(partOffirst, divisor);
            i++;
        } while (test.is_negative && i != 309);
        tempResult = divideBySmallNumber(partOffirst, divisor);
        Result.quotient.digits[309 - i] = tempResult.quotient.digits[0];
        partOffirst = tempResult.remainder;
    }
    Result.remainder = partOffirst;
    Result.remainder.is_negative = neg;
    Result.quotient.is_negative = neg;
    return Result;
}

LargeNumber LargeNumberOperations::modularExponentiation(const LargeNumber& base, const LargeNumber& exponent, const LargeNumber& modulus) {
    if (isEqualToZero(base)) return base;
    if (isEqualToZero(exponent)) {
        LargeNumber result;
        result.digits[0] = 1;
        return result;
    }
    LargeNumber Result, Base, Exp;
    Result.digits[0] = 1;
    Base = copyLargeNumber(base);
    Exp = copyLargeNumber(exponent);
    DivisionResult DR = divideByLargeNumber(Base, modulus);
    Base = DR.remainder;
    while (!isEqualToZero(Exp)) {
        if (Exp.digits[0] % 2 == 1) {
            Result = multiplyLargeNumbers(Result, Base);
            DR = divideByLargeNumber(Result, modulus);
            Result = DR.remainder;
        }
        Base = multiplyLargeNumbers(Base, Base);
        DR = divideByLargeNumber(Base, modulus);
        Base = DR.remainder;
        Exp = divideByLargeNumber(Exp, *new LargeNumber{2}).quotient;
    }
    return Result;
}

bool LargeNumberOperations::isPrimeNumber(const LargeNumber& inputNumber) {
    LargeNumber number = copyLargeNumber(inputNumber);
    LargeNumber one, two, base, numberMinusOne, exponentK, calcResult, remainder;
    one.digits[0] = 1;
    two.digits[0] = 2;
    base.digits[0] = 2;
    if (subtractLargeNumbers(number, base).is_negative)
        return false;
    DivisionResult divisionResult = divideByLargeNumber(number, two);
    if (isEqualToZero(divisionResult.remainder))
        return false;
    numberMinusOne = subtractLargeNumbers(number, one);
    exponentK.digits[0] = 0;
    while (!isEqualToZero(divideByLargeNumber(numberMinusOne, two).remainder)) {
        numberMinusOne = divideByLargeNumber(numberMinusOne, two).quotient;
        exponentK.digits[0]++;
    }
    calcResult = modularExponentiation(base, numberMinusOne, number);
    divisionResult = divideByLargeNumber(calcResult, number);
    if (isEqualToZero(subtractLargeNumbers(divisionResult.remainder, one)) || 
        isEqualToZero(subtractLargeNumbers(divisionResult.remainder, numberMinusOne)))
        return true;
    LargeNumber currentExponent = one;
    while (!isEqualToZero(subtractLargeNumbers(exponentK, currentExponent))) {
        calcResult = modularExponentiation(calcResult, two, number);
        divisionResult = divideByLargeNumber(calcResult, number);
        if (isEqualToZero(subtractLargeNumbers(divisionResult.remainder, one)))
            return false;
        if (isEqualToZero(subtractLargeNumbers(divisionResult.remainder, numberMinusOne)))
            return true;
        currentExponent = addLargeNumbers(currentExponent, one);
    }
    return false;
}

BigInteger::BigInteger(std::string& s) 
{
    digits = "";
    int n = s.size();
    for (int i = n - 1; i >= 0; i--) 
    {
        if (!isdigit(s[i]))
            throw("ERROR CONVERT");
        else
            digits.push_back(s[i] - '0');
    }
}

BigInteger::BigInteger(unsigned long long numConvert) 
{
    do {
        digits.push_back(numConvert % 10);
        numConvert /= 10;
    } while (numConvert != 0);
}

BigInteger::BigInteger(const char* s) {
    digits = "";
    for (int i = strlen(s) - 1; i >= 0; i--) 
    {
        if (!isdigit(s[i]))
            throw("ERROR CONVERT");
        else
            digits.push_back(s[i] - '0');
    }
}

BigInteger::BigInteger(BigInteger& a) {
    digits = a.digits;
}

BigInteger::BigInteger(const BigInteger& a) {
    this->digits = a.digits;
}

bool Null(const BigInteger& a) {
    return (a.digits.size() == 1 && a.digits[0] == 0);
}

int BigInteger::operator[](const int index) const {
    if (digits.size() <= index || index < 0)
        throw("ERROR");
    return digits[index];
}

BigInteger& BigInteger::operator=(const BigInteger& a) {
    digits = a.digits;
    return *this;
}

BigInteger& BigInteger::operator++() {
    int i, n = digits.size();
    for (i = 0; i < n && digits[i] == 9; i++)
        digits[i] = 0;
    if (i == n)
        digits.push_back(1);
    else
        digits[i]++;
    return *this;
}

BigInteger BigInteger::operator++(int temp) {
    BigInteger aux;
    aux = *this;
    ++(*this);
    return aux;
}

BigInteger& operator+=(BigInteger& a, const BigInteger& b) {
    int carry = 0, sum;
    size_t n = a.digits.size(), m = b.digits.size();
    if (m > n) 
        a.digits.resize(m, 0);
    for (size_t i = 0; i < m || carry; ++i) {
        if (i == a.digits.size()) 
            a.digits.push_back(0);
        sum = a.digits[i] + carry + (i < m ? b.digits[i] : 0);
        a.digits[i] = sum % 10;
        carry = sum / 10;
    }
    return a;
}

BigInteger operator+(const BigInteger& a, const BigInteger& b) {
    BigInteger temp(a);
    temp += b;
    return temp;
}

BigInteger& BigInteger::operator--() {
    if (digits[0] == 0 && digits.size() == 1)
        throw("UNDERFLOW");
    int i, n = digits.size();
    for (i = 0; digits[i] == 0 && i < n; i++)
        digits[i] = 9;
    digits[i]--;
    if (n > 1 && digits[n - 1] == 0)
        digits.pop_back();
    return *this;
}

BigInteger BigInteger::operator--(int temp) {
    BigInteger aux;
    aux = *this;
    --(*this);
    return aux;
}

BigInteger& operator-=(BigInteger& a, const BigInteger& b) {
    if (a < b) throw("UNDERFLOW");
    int n = a.digits.size(), m = b.digits.size(), t = 0;
    for (int i = 0; i < n; i++) {
        int s = a.digits[i] - (i < m ? b.digits[i] : 0) + t;
        t = s < 0 ? -1 : 0;
        a.digits[i] = s < 0 ? s + 10 : s;
    }
    while (n > 1 && a.digits[n - 1] == 0) a.digits.pop_back(), n--;
    return a;
}

BigInteger operator-(const BigInteger& a, const BigInteger& b) {
    BigInteger temp;
    temp = a;
    temp -= b;
    return temp;
}

BigInteger operator*(const BigInteger& a, const BigInteger& b) {
    BigInteger temp;
    temp = a;
    temp *= b;
    return temp;
}

BigInteger& operator*=(BigInteger& a, const BigInteger& b) {
    if (Null(a) || Null(b)) { a = BigInteger(); return a; }
    int n = a.digits.size(), m = b.digits.size();
    std::vector<int> v(n + m, 0);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            v[i + j] += a.digits[i] * b.digits[j];
    a.digits.resize(n + m);
    int carry = 0;
    for (int i = 0; i < a.digits.size(); i++) {
        v[i] += carry;
        a.digits[i] = v[i] % 10;
        carry = v[i] / 10;
    }
    while (a.digits.size() > 1 && a.digits.back() == 0)
        a.digits.pop_back();
    return a;
}

BigInteger& operator/=(BigInteger& a, const BigInteger& b) {
    if (Null(b)) throw("Arithmetic Error: Division By 0");
    if (a < b) { a = BigInteger(); return a; }
    int n = a.digits.size(), m = b.digits.size();
    std::vector<int> quotient(n, 0);
    BigInteger temp;
    for (int i = n - 1; i >= 0; --i) {
        temp = temp * 10 + a.digits[i];
        int divisor = 0;
        while (b < temp || b == temp) { temp -= b; ++divisor; }
        quotient[n - i - 1] = divisor;
    }
    a.digits.clear();
    for (int digit : quotient) a.digits.push_back(digit);
    return a;
}

BigInteger operator/(const BigInteger& a, const BigInteger& b) {
    BigInteger temp;
    temp = a;
    temp /= b;
    return temp;
}

BigInteger& operator%=(BigInteger& a, const BigInteger& b) {
    if (Null(b)) throw("Arithmetic Error: Division By 0");
    if (a < b) return a;
    if (a == b) { a = BigInteger(); return a; }
    int n = a.digits.size();
    BigInteger t;
    for (int i = n - 1; t * 10 + a.digits[i] < b; i--) {
        t *= 10;
        t += a.digits[i];
    }
    for (int i = n - 1; i >= 0; i--) {
        t = t * 10 + a.digits[i];
        int cc = 0;
        while (b < t || b == t) { t -= b; ++cc; }
    }
    a = t;
    return a;
}

BigInteger operator%(const BigInteger& a, const BigInteger& b) {
    BigInteger temp;
    temp = a;
    temp %= b;
    return temp;
}

std::string BigInteger::toString() {
    std::string res="";
    for (int i = this->digits.size() - 1; i >= 0; i--)
         res = res + std::to_string((short)this->digits[i]);
    return res;
}

bool operator==(const BigInteger& a, const BigInteger& b) {
    return a.digits == b.digits;
}

bool operator<(const BigInteger& a, const BigInteger& b) {
    size_t n = a.digits.size(), m = b.digits.size();
    if (n != m)
        return n < m;
    for (size_t i = n; i-- > 0; ) 
        if (a.digits[i] != b.digits[i])
            return a.digits[i] < b.digits[i];
    return false;
}