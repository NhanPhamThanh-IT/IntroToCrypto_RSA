#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

namespace IOHandler {
    std::vector<std::string> readInputFile(const std::string &filePath);
    bool writeOutputFile(const std::string &filePath, const std::string &content);
};

namespace HelperFunctions {
    bool validateFile(const std::string &filename);
    bool isHexadecimal(const std::string &str);
    bool areHexadecimal(const std::vector<std::string> &values);
}

struct LargeNumber
{
    static const size_t MAX_DIGITS = 309;
    std::vector<int> digits = std::vector<int>(MAX_DIGITS, 0);
    bool is_negative = false;
    LargeNumber() : digits(MAX_DIGITS, 0) {}
    LargeNumber(const std::vector<int> &input_digits) : digits(input_digits) {}
};

struct DivisionResult
{
    LargeNumber quotient;
    LargeNumber remainder;
};

namespace HandlerLargeNumbers
{
    LargeNumber processLargeNumbers(const std::string &hexP, const std::string &hexQ, const std::string &hexE);
    std::string formatLargeNumber(const LargeNumber &data);
}

namespace LargeNumberConversion
{
    LargeNumber copyLargeNumber(const LargeNumber &number);
    LargeNumber addLeadingDigit(const LargeNumber &number, int digit);
    bool isEqualToZero(const LargeNumber &number);
}

namespace LargeNumberArithmetic
{
    LargeNumber addLargeNumbers(LargeNumber first, LargeNumber second);
    LargeNumber subtractLargeNumbers(LargeNumber minuend, LargeNumber subtrahend);
    LargeNumber multiplyLargeNumbers(LargeNumber first, LargeNumber second);
    DivisionResult divideBySmallNumber(LargeNumber dividend, LargeNumber divisor);
    DivisionResult divideByLargeNumber(LargeNumber dividend, LargeNumber divisor);
}

namespace LargeNumberSpecialOperations
{
    LargeNumber inverse(LargeNumber input, LargeNumber modulus);
}

struct LargeNumberArray
{
    LargeNumber result;
    LargeNumber count;
};

class BigInteger
{
private:
    std::string digits;
public:
    BigInteger(std::string &);
    BigInteger(BigInteger &);
    BigInteger(unsigned long long n = 0);
    BigInteger(const char *);
    BigInteger(const BigInteger &a);
    friend bool Null(const BigInteger &);
    int operator[](const int) const;
    std::string toString();
    BigInteger &operator=(const BigInteger &);
    BigInteger &operator++();
    BigInteger operator++(int temp);
    friend BigInteger &operator+=(BigInteger &, const BigInteger &);
    friend BigInteger operator+(const BigInteger &, const BigInteger &);
    BigInteger &operator--();
    BigInteger operator--(int temp);
    friend BigInteger &operator-=(BigInteger &, const BigInteger &);
    friend BigInteger operator-(const BigInteger &, const BigInteger &);
    friend BigInteger &operator*=(BigInteger &, const BigInteger &);
    friend BigInteger operator*(const BigInteger &, const BigInteger &);
    friend BigInteger &operator/=(BigInteger &, const BigInteger &);
    friend BigInteger operator/(const BigInteger &, const BigInteger &);
    friend BigInteger operator%(const BigInteger &, const BigInteger &);
    friend BigInteger &operator%=(BigInteger &, const BigInteger &);
    friend bool operator==(const BigInteger &, const BigInteger &);
    friend bool operator<(const BigInteger &, const BigInteger &);
};

namespace ConversionOperations
{
    LargeNumber convertStringToLargeNumber(const std::string &str);
    std::string convertDecimalToHexBigEndian(LargeNumber decimal);
    BigInteger convertHexBigEndianToDecimal(std::string hexVal);
    std::string convertLargeNumberToString(LargeNumber input);
};

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        std::cerr << "Usage: <input_file> <output_file>" << std::endl;
        return 1;
    }
    const std::string inputFile = argv[1];
    const std::string outputFile = argv[2];
    if (!HelperFunctions::validateFile(inputFile))
    {
        std::cerr << "Error: Invalid input file!" << std::endl;
        return 1;
    }
    std::vector<std::string> hexValues = IOHandler::readInputFile(inputFile);
    if (hexValues.size() != 3 || !HelperFunctions::areHexadecimal(hexValues))
    {
        std::cerr << "Error: Non-hexadecimal or incomplete input values!" << std::endl;
        return 1;
    }
    try
    {
        LargeNumber D = HandlerLargeNumbers::processLargeNumbers(hexValues[0], hexValues[1], hexValues[2]);
        std::string output = HandlerLargeNumbers::formatLargeNumber(D);
        if (!IOHandler::writeOutputFile(outputFile, output))
        {
            std::cerr << "Error: Unable to write to output file!" << std::endl;
            return 1;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

bool HelperFunctions::validateFile(const std::string &filename)
{
    std::ifstream file(filename);
    return file.good();
}

std::vector<std::string> IOHandler::readInputFile(const std::string &filePath)
{
    std::ifstream file(filePath);
    std::vector<std::string> hexValues(3);
    if (file) file >> hexValues[0] >> hexValues[1] >> hexValues[2];
    return hexValues;
}

bool IOHandler::writeOutputFile(const std::string &filePath, const std::string &content)
{
    std::ofstream file(filePath);
    if (file)
    {
        file << content;
        return true;
    }
    return false;
}

bool HelperFunctions::isHexadecimal(const std::string &str)
{
    return std::all_of(str.begin(), str.end(), [](char c) { return std::isxdigit(c); });
}

bool HelperFunctions::areHexadecimal(const std::vector<std::string> &values)
{
    return std::all_of(values.begin(), values.end(), isHexadecimal);
}

LargeNumber LargeNumberConversion::copyLargeNumber(const LargeNumber &number)
{
    LargeNumber Result;
    Result.is_negative = number.is_negative;
    for (int i = 0; i < 309; i++) Result.digits[i] = number.digits[i];
    return Result;
}

LargeNumber LargeNumberConversion::addLeadingDigit(const LargeNumber &number, int digit)
{
    LargeNumber Result = number;
    Result.is_negative = number.is_negative;
    for (int i = 309 - 1; i > 0; --i) Result.digits[i] = Result.digits[i - 1];
    Result.digits[0] = digit;
    return Result;
}

bool LargeNumberConversion::isEqualToZero(const LargeNumber &number)
{
    for (int i = 0; i < 309; i++)
        if (number.digits[i] != 0)
            return false;
    return true;
}

LargeNumber LargeNumberArithmetic::addLargeNumbers(LargeNumber first, LargeNumber second)
{
    if (LargeNumberConversion::isEqualToZero(first)) return second;
    if (LargeNumberConversion::isEqualToZero(second)) return first;
    bool BothNegative = first.is_negative && second.is_negative;
    if (first.is_negative)
    {
        first.is_negative = false;
        return subtractLargeNumbers(second, first);
    }
    if (second.is_negative)
    {
        second.is_negative = false;
        return subtractLargeNumbers(first, second);
    }
    LargeNumber Result;
    int carry = 0;
    for (int i = 0; i < Result.MAX_DIGITS; i++)
    {
        int sum = first.digits[i] + second.digits[i] + carry;
        Result.digits[i] = sum % 100;
        carry = sum / 100;
    }
    if (carry) Result.digits[Result.MAX_DIGITS - 1] = carry;
    Result.is_negative = BothNegative;
    return Result;
}

LargeNumber LargeNumberArithmetic::subtractLargeNumbers(LargeNumber minuend, LargeNumber subtrahend)
{
    if (LargeNumberConversion::isEqualToZero(subtrahend)) return minuend;
    if (LargeNumberConversion::isEqualToZero(minuend))
    {
        subtrahend.is_negative = true;
        return subtrahend;
    }
    if (subtrahend.is_negative)
    {
        if (minuend.is_negative)
        {
            minuend.is_negative = subtrahend.is_negative = false;
            return subtractLargeNumbers(subtrahend, minuend);
        }
        subtrahend.is_negative = false;
        return addLargeNumbers(minuend, subtrahend);
    }
    if (minuend.is_negative)
    {
        minuend.is_negative = subtrahend.is_negative = false;
        LargeNumber result = addLargeNumbers(minuend, subtrahend);
        result.is_negative = true;
        return result;
    }
    LargeNumber Result = minuend;
    bool borrow = false;
    for (int i = 0; i < 309; i++)
    {
        int diff = Result.digits[i] - subtrahend.digits[i] - borrow;
        if (diff < 0)
        {
            diff += 100;
            borrow = true;
        }
        else
            borrow = false;
        Result.digits[i] = diff;
    }
    if (borrow)
    {
        LargeNumber temp = subtractLargeNumbers(subtrahend, minuend);
        temp.is_negative = true;
        return temp;
    }
    return Result;
}

LargeNumber LargeNumberArithmetic::multiplyLargeNumbers(LargeNumber first, LargeNumber second)
{
    if (LargeNumberConversion::isEqualToZero(first) || LargeNumberConversion::isEqualToZero(second))
        return LargeNumber();
    bool neg = first.is_negative ^ second.is_negative;
    first.is_negative = second.is_negative = false;
    LargeNumber Result;
    for (int i = 0; i < 309; i++)
    {
        LargeNumber temp;
        int carry = 0;
        for (int j = 0; j < 309; j++)
        {
            int val = first.digits[i] * second.digits[j] + carry;
            carry = val / 100;
            temp.digits[j] += val % 100;
        }
        for (int k = 309 - 1; k >= i; k--)
            temp.digits[k] = temp.digits[k - i];
        for (int k = 0; k < i; k++)
            temp.digits[k] = 0;
        Result = addLargeNumbers(Result, temp);
    }
    Result.is_negative = neg;
    return Result;
}

DivisionResult LargeNumberArithmetic::divideBySmallNumber(LargeNumber dividend, LargeNumber divisor)
{
    DivisionResult Result;
    if (LargeNumberConversion::isEqualToZero(dividend))
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
        if (countArray.digits[0] != 99) countArray.digits[0] = countArray.digits[0] + 1;
        else countArray = addLargeNumbers(countArray, One);
    } while (!CheckNeg);
    if (countArray.digits[0] != 0)
    {
        countArray.digits[0] = countArray.digits[0] - 1;
        Result.quotient = countArray;
    }
    else Result.quotient = subtractLargeNumbers(countArray, One);
    Result.remainder = addLargeNumbers(tempResult, divisor);
    return Result;
}

DivisionResult LargeNumberArithmetic::divideByLargeNumber(LargeNumber dividend, LargeNumber divisor)
{
    DivisionResult Result, tempResult;
    LargeNumber partOffirst, test;
    if (LargeNumberConversion::isEqualToZero(dividend))
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
    while (i != 309) {
        do {
            partOffirst = LargeNumberConversion::addLeadingDigit(partOffirst, dividend.digits[i]);
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

LargeNumber LargeNumberSpecialOperationsinverse(LargeNumber input, LargeNumber modulus)
{
    LargeNumber Result, Q, A2, A3, B2, B3, T2, T3, Zero, One, temp;
    One.digits[0] = 1;
    A2 = LargeNumberConversion::copyLargeNumber(Zero);
    A3 = LargeNumberConversion::copyLargeNumber(modulus);
    B2 = LargeNumberConversion::copyLargeNumber(One);
    B3 = LargeNumberConversion::copyLargeNumber(input);
    while (!LargeNumberConversion::isEqualToZero(B3) && !LargeNumberConversion::isEqualToZero(LargeNumberArithmetic::subtractLargeNumbers(B3, One)))
    {
        Q = LargeNumberArithmetic::divideByLargeNumber(A3, B3).quotient;
        T2 = LargeNumberArithmetic::subtractLargeNumbers(A2, LargeNumberArithmetic::multiplyLargeNumbers(Q, B2));
        T3 = LargeNumberArithmetic::subtractLargeNumbers(A3, LargeNumberArithmetic::multiplyLargeNumbers(Q, B3));
        A2 = LargeNumberConversion::copyLargeNumber(B2);
        A3 = LargeNumberConversion::copyLargeNumber(B3);
        B2 = LargeNumberConversion::copyLargeNumber(T2);
        B3 = LargeNumberConversion::copyLargeNumber(T3);
    }
    if (LargeNumberConversion::isEqualToZero(B3))
    {
        LargeNumber res;
        res.is_negative = true;
        res.digits[0] = 1;
        return res;
    }
    if (LargeNumberConversion::isEqualToZero(LargeNumberArithmetic::subtractLargeNumbers(B3, One)))
    {
        while (B2.is_negative)
            B2 = LargeNumberArithmetic::addLargeNumbers(B2, modulus);
        Result = B2;
        temp = LargeNumberArithmetic::subtractLargeNumbers(Result, modulus);
        while (!temp.is_negative)
        {
            Result = temp;
            temp = LargeNumberArithmetic::subtractLargeNumbers(Result, modulus);
        }
    }
    return Result;
}

LargeNumber LargeNumberSpecialOperations::inverse(LargeNumber input, LargeNumber modulus)
{
    LargeNumber Result, Q, A2, A3, B2, B3, T2, T3, Zero, One, temp;
    One.digits[0] = 1;
    A2 = LargeNumberConversion::copyLargeNumber(Zero);
    A3 = LargeNumberConversion::copyLargeNumber(modulus);
    B2 = LargeNumberConversion::copyLargeNumber(One);
    B3 = LargeNumberConversion::copyLargeNumber(input);
    while (!LargeNumberConversion::isEqualToZero(B3) && !LargeNumberConversion::isEqualToZero(LargeNumberArithmetic::subtractLargeNumbers(B3, One)))
    {
        Q = LargeNumberArithmetic::divideByLargeNumber(A3, B3).quotient;
        T2 = LargeNumberArithmetic::subtractLargeNumbers(A2, LargeNumberArithmetic::multiplyLargeNumbers(Q, B2));
        T3 = LargeNumberArithmetic::subtractLargeNumbers(A3, LargeNumberArithmetic::multiplyLargeNumbers(Q, B3));
        A2 = LargeNumberConversion::copyLargeNumber(B2);
        A3 = LargeNumberConversion::copyLargeNumber(B3);
        B2 = LargeNumberConversion::copyLargeNumber(T2);
        B3 = LargeNumberConversion::copyLargeNumber(T3);
    }
    if (LargeNumberConversion::isEqualToZero(B3))
    {
        LargeNumber res;
        res.is_negative = true;
        res.digits[0] = 1;
        return res;
    }
    if (LargeNumberConversion::isEqualToZero(LargeNumberArithmetic::subtractLargeNumbers(B3, One)))
    {
        while (B2.is_negative)
            B2 = LargeNumberArithmetic::addLargeNumbers(B2, modulus);
        Result = B2;
        temp = LargeNumberArithmetic::subtractLargeNumbers(Result, modulus);
        while (!temp.is_negative)
        {
            Result = temp;
            temp = LargeNumberArithmetic::subtractLargeNumbers(Result, modulus);
        }
    }
    return Result;
}

LargeNumber HandlerLargeNumbers::processLargeNumbers(const std::string &hexP, const std::string &hexQ, const std::string &hexE)
{
    LargeNumber P = ConversionOperations::convertStringToLargeNumber(ConversionOperations::convertHexBigEndianToDecimal(hexP).toString());
    LargeNumber Q = ConversionOperations::convertStringToLargeNumber(ConversionOperations::convertHexBigEndianToDecimal(hexQ).toString());
    LargeNumber E = ConversionOperations::convertStringToLargeNumber(ConversionOperations::convertHexBigEndianToDecimal(hexE).toString());
    LargeNumber One;
    One.digits[0] = 1;
    LargeNumber Phi = LargeNumberArithmetic::multiplyLargeNumbers(LargeNumberArithmetic::subtractLargeNumbers(P, One),LargeNumberArithmetic::subtractLargeNumbers(Q, One));
    return LargeNumberSpecialOperations::inverse(E, Phi);
}

std::string HandlerLargeNumbers::formatLargeNumber(const LargeNumber &data)
{
    return data.is_negative ? ConversionOperations::convertLargeNumberToString(data) : ConversionOperations::convertDecimalToHexBigEndian(data);
}

BigInteger::BigInteger(std::string &s)
{
    digits = "";
    int n = s.size();
    for (int i = n - 1; i >= 0; i--)
    {
        if (!isdigit(s[i])) throw("ERROR CONVERT");
        else digits.push_back(s[i] - '0');
    }
}

BigInteger::BigInteger(unsigned long long numConvert)
{
    do {
        digits.push_back(numConvert % 10);
        numConvert /= 10;
    } while (numConvert != 0);
}

BigInteger::BigInteger(const char *s)
{
    std::string str = std::string(s);
    digits = "";
    for (int i = str.length() - 1; i >= 0; i--)
    {
        if (!isdigit(s[i])) throw("ERROR CONVERT");
        else digits.push_back(s[i] - '0');
    }
}

BigInteger::BigInteger(BigInteger &a)
{
    digits = a.digits;
}

BigInteger::BigInteger(const BigInteger &a)
{
    this->digits = a.digits;
}

bool Null(const BigInteger &a)
{
    return (a.digits.size() == 1 && a.digits[0] == 0);
}

int BigInteger::operator[](const int index) const
{
    if (digits.size() <= index || index < 0) throw("ERROR");
    return digits[index];
}

BigInteger &BigInteger::operator=(const BigInteger &a)
{
    digits = a.digits;
    return *this;
}

BigInteger &BigInteger::operator++()
{
    int i, n = digits.size();
    for (i = 0; i < n && digits[i] == 9; i++) digits[i] = 0;
    if (i == n) digits.push_back(1);
    else digits[i]++;
    return *this;
}

BigInteger BigInteger::operator++(int temp)
{
    BigInteger aux;
    aux = *this;
    ++(*this);
    return aux;
}

BigInteger &operator+=(BigInteger &a, const BigInteger &b)
{
    int carry = 0, sum;
    size_t n = a.digits.size(), m = b.digits.size();
    if (m > n) a.digits.resize(m, 0);
    for (size_t i = 0; i < m || carry; ++i)
    {
        if (i == a.digits.size())
            a.digits.push_back(0);
        sum = a.digits[i] + carry + (i < m ? b.digits[i] : 0);
        a.digits[i] = sum % 10;
        carry = sum / 10;
    }
    return a;
}

BigInteger operator+(const BigInteger &a, const BigInteger &b)
{
    BigInteger temp(a);
    temp += b;
    return temp;
}

BigInteger &BigInteger::operator--()
{
    if (digits[0] == 0 && digits.size() == 1) throw("UNDERFLOW");
    int i, n = digits.size();
    for (i = 0; digits[i] == 0 && i < n; i++) digits[i] = 9;
    digits[i]--;
    if (n > 1 && digits[n - 1] == 0) digits.pop_back();
    return *this;
}

BigInteger BigInteger::operator--(int temp)
{
    BigInteger aux;
    aux = *this;
    --(*this);
    return aux;
}

BigInteger &operator-=(BigInteger &a, const BigInteger &b)
{
    if (a < b) throw("UNDERFLOW");
    int n = a.digits.size(), m = b.digits.size(), t = 0;
    for (int i = 0; i < n; i++)
    {
        int s = a.digits[i] - (i < m ? b.digits[i] : 0) + t;
        t = s < 0 ? -1 : 0;
        a.digits[i] = s < 0 ? s + 10 : s;
    }
    while (n > 1 && a.digits[n - 1] == 0) a.digits.pop_back(), n--;
    return a;
}

BigInteger operator-(const BigInteger &a, const BigInteger &b)
{
    BigInteger temp;
    temp = a;
    temp -= b;
    return temp;
}

BigInteger operator*(const BigInteger &a, const BigInteger &b)
{
    BigInteger temp;
    temp = a;
    temp *= b;
    return temp;
}

BigInteger &operator*=(BigInteger &a, const BigInteger &b)
{
    if (Null(a) || Null(b))
    {
        a = BigInteger();
        return a;
    }
    int n = a.digits.size(), m = b.digits.size();
    std::vector<int> v(n + m, 0);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            v[i + j] += a.digits[i] * b.digits[j];
    a.digits.resize(n + m);
    int carry = 0;
    for (int i = 0; i < a.digits.size(); i++)
    {
        v[i] += carry;
        a.digits[i] = v[i] % 10;
        carry = v[i] / 10;
    }
    while (a.digits.size() > 1 && a.digits.back() == 0) a.digits.pop_back();
    return a;
}

BigInteger &operator/=(BigInteger &a, const BigInteger &b)
{
    if (Null(b))
        throw("Arithmetic Error: Division By 0");
    if (a < b)
    {
        a = BigInteger();
        return a;
    }
    int n = a.digits.size(), m = b.digits.size();
    std::vector<int> quotient(n, 0);
    BigInteger temp;
    for (int i = n - 1; i >= 0; --i)
    {
        temp = temp * 10 + a.digits[i];
        int divisor = 0;
        while (b < temp || b == temp)
        {
            temp -= b;
            ++divisor;
        }
        quotient[n - i - 1] = divisor;
    }
    a.digits.clear();
    for (int digit : quotient)
        a.digits.push_back(digit);
    return a;
}

BigInteger operator/(const BigInteger &a, const BigInteger &b)
{
    BigInteger temp;
    temp = a;
    temp /= b;
    return temp;
}

BigInteger &operator%=(BigInteger &a, const BigInteger &b)
{
    if (Null(b))
        throw("Arithmetic Error: Division By 0");
    if (a < b)
        return a;
    if (a == b)
    {
        a = BigInteger();
        return a;
    }
    int n = a.digits.size();
    BigInteger t;
    for (int i = n - 1; t * 10 + a.digits[i] < b; i--)
    {
        t *= 10;
        t += a.digits[i];
    }
    for (int i = n - 1; i >= 0; i--)
    {
        t = t * 10 + a.digits[i];
        int cc = 0;
        while (b < t || b == t)
        {
            t -= b;
            ++cc;
        }
    }
    a = t;
    return a;
}

BigInteger operator%(const BigInteger &a, const BigInteger &b)
{
    BigInteger temp;
    temp = a;
    temp %= b;
    return temp;
}

std::string BigInteger::toString()
{
    std::string res = "";
    for (int i = this->digits.size() - 1; i >= 0; i--)
        res = res + std::to_string((short)this->digits[i]);
    return res;
}

bool operator==(const BigInteger &a, const BigInteger &b)
{
    return a.digits == b.digits;
}

bool operator<(const BigInteger &a, const BigInteger &b)
{
    size_t n = a.digits.size(), m = b.digits.size();
    if (n != m)
        return n < m;
    for (size_t i = n; i-- > 0;)
        if (a.digits[i] != b.digits[i])
            return a.digits[i] < b.digits[i];
    return false;
}

LargeNumber ConversionOperations::convertStringToLargeNumber(const std::string &str)
{
    LargeNumber final;
    std::string temp = str;
    final.is_negative = (temp[0] == '-');
    if (final.is_negative)
        temp.erase(0, 1);
    std::reverse(temp.begin(), temp.end());
    int val = 0, multiplier = 1, digit_index = 0;
    for (char c : temp)
    {
        val += (c - '0') * multiplier;
        multiplier *= 10;
        if (multiplier == 100)
        {
            final.digits[digit_index++] = val;
            val = 0;
            multiplier = 1;
        }
    }
    if (val > 0)
        final.digits[digit_index] = val;
    return final;
}

std::string ConversionOperations::convertDecimalToHexBigEndian(LargeNumber decimal)
{
    if (LargeNumberConversion::isEqualToZero(decimal))
        return "0";
    std::string res;
    LargeNumber Sixteen;
    Sixteen.digits[0] = 16;
    while (!LargeNumberConversion::isEqualToZero(decimal))
    {
        int rem = LargeNumberArithmetic::divideByLargeNumber(decimal, Sixteen).remainder.digits[0];
        res = (rem < 10) ? std::to_string(rem) + res : std::string(1, 'A' + rem - 10) + res;
        decimal = LargeNumberArithmetic::divideByLargeNumber(decimal, Sixteen).quotient;
    }
    return res;
}

BigInteger ConversionOperations::convertHexBigEndianToDecimal(std::string hexVal)
{
    int len = hexVal.size();
    BigInteger base("1");
    BigInteger dec_val("0");
    for (int i = len - 1; i >= 0; i--)
    {
        if (hexVal[i] >= '0' && hexVal[i] <= '9')
        {
            dec_val += (hexVal[i] - '0') * base;
            base = base * 16;
        }
        else if (hexVal[i] >= 'A' && hexVal[i] <= 'F')
        {
            dec_val += (hexVal[i] - '7') * base;
            base = base * 16;
        }
    }
    return dec_val;
}

std::string ConversionOperations::convertLargeNumberToString(LargeNumber input)
{
    std::string out;
    if (input.is_negative)
        out += "-";
    std::reverse(input.digits.begin(), input.digits.end());
    bool leadingZero = true;
    for (int i = 0; i < 309; ++i)
    {
        int value = input.digits[i];
        for (int div = 10; div > 0; div /= 10)
        {
            int digit = value / div;
            value %= div;
            if (digit != 0 || !leadingZero)
            {
                out += (digit + '0');
                leadingZero = false;
            }
        }
    }
    return out.empty() || out == "-" ? "0" : out;
}