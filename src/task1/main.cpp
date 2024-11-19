#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <vector>

using namespace std;

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
    string digits;
public:
    BigInteger(string&);
    BigInteger(BigInteger&);
    BigInteger(unsigned long long n = 0);
    BigInteger(const char*);
    BigInteger(const BigInteger& a);
    friend bool Null(const BigInteger&);
    int operator[](const int)const;
    string toString();
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

BigInteger convertHexToDecimal(string hexValue)
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
int main(int argc, char** argv)
{
    LargeNumberOperations numberOperations;
    string hexInput;
    int outputResult;
    BigInteger decimalValue;
    string testFileId;
    if (argc < 3) cout << "Not enough Command Line Arguments passed!" << endl;
    else
    {
        ifstream inputTestFile(argv[1]);
        inputTestFile >> hexInput;
        inputTestFile.close();
        decimalValue = convertHexToDecimal(hexInput);
        LargeNumber largeNumber;
        string largeNumberStr = decimalValue.toString();
        largeNumber = numberOperations.convertStringToLargeNumber(largeNumberStr);
        int result;
        result = numberOperations.isPrimeNumber(largeNumber);
        ofstream outputTestFile(argv[2]);
        outputTestFile << result;
        outputTestFile.close();
    }
    return 0;
}

LargeNumber LargeNumberOperations::convertStringToLargeNumber(std::string& str)
{
    LargeNumber final;
    bool neg = false;
    if (str[0] == '-')
    {
        neg = true;
        str.erase(0, 1);
    }
    reverse(begin(str), end(str));
    int val = 0, itr = 1, Out = 0;
    for (int i = 0; i < str.size(); i++)
    {
        val += (str[i] - '0') * itr;
        itr *= 10;
        if (itr == 100)
        {
            final.digits[Out] = val;
            Out++;
            itr = 1;
            val = 0;
        }
    }
    if (val != 0)
        final.digits[Out] = val;
    final.is_negative = neg;
    return final;
}

LargeNumber LargeNumberOperations::copyLargeNumber(const LargeNumber& number)
{
    LargeNumber Result;
    Result.is_negative = number.is_negative;
    for (int i = 0; i < MAX_DIGITS; i++)
        Result.digits[i] = number.digits[i];
    return Result;
}

LargeNumber LargeNumberOperations::addLeadingDigit(const LargeNumber& number, int value)
{
    LargeNumber Result;
    Result.is_negative = number.is_negative;
    for (int i = 0; i < MAX_DIGITS - 1; i++)
        Result.digits[i + 1] = number.digits[i];
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
    if (isEqualToZero(first))
        return second;
    if (isEqualToZero(second))
        return first;
    LargeNumber Result;
    int val = 0, carry = 0;
    bool BothNegative = false;
    if (first.is_negative && second.is_negative)
        BothNegative = true;
    else if (first.is_negative)
    {
        first.is_negative = false;
        Result = subtractLargeNumbers(second, first);
        return Result;
    }
    else if (second.is_negative)
    {
        second.is_negative = false;
        Result = subtractLargeNumbers(first, second);
        return Result;
    }
    int i = 0;
    for (i; i < MAX_DIGITS; i++)
    {
        val = (first.digits[i] + second.digits[i] + carry) % 100;
        carry = (first.digits[i] + second.digits[i] + carry) / 100;
        Result.digits[i] = val;
    }
    if (carry != 0)
        Result.digits[i] = carry;
    Result.is_negative = BothNegative;
    return Result;
}

LargeNumber LargeNumberOperations::subtractLargeNumbers(LargeNumber minuend, LargeNumber subtrahend)
{
    if (isEqualToZero(subtrahend))
        return minuend;
    if (isEqualToZero(minuend))
    {
        subtrahend.is_negative = true;
        return subtrahend;
    }
    LargeNumber Result, tempResult, first;
    first = copyLargeNumber(minuend);
    int val = 0, NextToMe = 0;
    bool LastNum = false;
    if (subtrahend.is_negative)
    {
        if (first.is_negative)
        {
            first.is_negative = false;
            subtrahend.is_negative = false;
            Result = subtractLargeNumbers(subtrahend, first);
            return Result;
        }
        else
        {
            subtrahend.is_negative = false;
            Result = addLargeNumbers(first, subtrahend);
            return Result;
        }
    }
    else
    {
        if (first.is_negative)
        {
            first.is_negative = false;
            subtrahend.is_negative = false;
            Result = addLargeNumbers(first, subtrahend);
            Result.is_negative = true;
            return Result;
        }
    }
    int i = 0;
    for (i; i < MAX_DIGITS; i++)
    {
        if (LastNum)
            break;
        if (first.digits[i] >= subtrahend.digits[i])
        {
            val = first.digits[i] - subtrahend.digits[i];
            Result.digits[i] = val;
        }
        else
        {
            if (i == MAX_DIGITS)
                LastNum = true;
            int temp = i;
            while (temp < MAX_DIGITS - 1)
            {
                temp++;
                NextToMe++;
                if (first.digits[temp] != 0)
                {
                    first.digits[temp] -= 1;
                    first.digits[i] = first.digits[i] + 100;
                    NextToMe--;
                    temp = i + 1;
                    i--;
                    while (NextToMe != 0)
                    {
                        first.digits[temp] = 99;
                        NextToMe--;
                        temp++;
                        ;
                    }
                    break;
                }
                else if (first.digits[temp] == 0 && temp == MAX_DIGITS - 1)
                    LastNum = true;
            }
        }
    }
    if (LastNum == true)
    {
        Result = subtractLargeNumbers(subtrahend, minuend);
        Result.is_negative = true;
        return Result;
    }
    return Result;
}

LargeNumber LargeNumberOperations::multiplyLargeNumbers(LargeNumber first, LargeNumber second)
{
    LargeNumber Result;
    if (isEqualToZero(first))
        return first;
    if (isEqualToZero(second))
        return second;
    int val = 0, carry = 0;
    bool neg = false;
    if (first.is_negative && second.is_negative)
    {
        first.is_negative = false;
        second.is_negative = false;
    }
    else if (first.is_negative)
    {
        first.is_negative = false;
        neg = true;
    }
    else if (second.is_negative)
    {
        second.is_negative = false;
        neg = true;
    }
    int i, j;
    for (i = 0; i < MAX_DIGITS; i++)
    {
        LargeNumber temp;
        for (j = 0; j < MAX_DIGITS; j++)
        {
            val = ((first.digits[i] * second.digits[j]) + carry) % 100;
            carry = ((first.digits[i] * second.digits[j]) + carry) / 100;
            temp.digits[j] += val;
        }
        if (i != 0)
        {
            for (int k = 308; k != 0; k--)
                temp.digits[k] = temp.digits[k - i];
            for (int k = 0; k < i; k++)
                temp.digits[k] = 0;
        }
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
    reverse(begin(dividend.digits), end(dividend.digits));
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

bool LargeNumberOperations::isPrimeNumber(const LargeNumber& number)
{
    bool Result = false;
    DivisionResult DR;
    LargeNumber n = copyLargeNumber(number);
    LargeNumber One, two, a, NMinusOne, K, tempK, Q, test, Calc, power;
    One.digits[0] = 1;
    two.digits[0] = 2;
    a.digits[0] = 2;
    if (subtractLargeNumbers(n, a).is_negative)
        return false;
    DR = divideByLargeNumber(n, two);
    if (isEqualToZero(DR.remainder))
        return false;
    NMinusOne = subtractLargeNumbers(n, One);
    do
    {
        Q = NMinusOne;
        K = tempK;
        DR = divideByLargeNumber(NMinusOne, two);
        NMinusOne = DR.quotient;
        if (K.digits[0] != 99)
            tempK.digits[0] = K.digits[0] + 1;
        else
            tempK = addLargeNumbers(K, One);
    } while (isEqualToZero(DR.remainder));
    NMinusOne = subtractLargeNumbers(n, One);
    Calc = modularExponentiation(a, Q, n);
    DR = divideByLargeNumber(Calc, n);
    if (isEqualToZero(subtractLargeNumbers(DR.remainder, One)) || isEqualToZero(subtractLargeNumbers(DR.remainder, NMinusOne)))
        return true;
    if (test.digits[0] != 99)
        test.digits[0] = test.digits[0] + 1;
    else
        test = addLargeNumbers(test, One);
    while (!isEqualToZero(subtractLargeNumbers(K, test)))
    {
        Calc = modularExponentiation(Calc, two, n);
        DR = divideByLargeNumber(Calc, n);
        if (isEqualToZero(subtractLargeNumbers(DR.remainder, One)))
        {
            Result = false;
            break;
        }
        else if (isEqualToZero(subtractLargeNumbers(DR.remainder, NMinusOne)))
        {
            Result = true;
            break;
        }
        if (test.digits[0] != 99)
            test.digits[0] = test.digits[0] + 1;
        else
            test = addLargeNumbers(test, One);
    }
    return Result;
}

BigInteger::BigInteger(string& s) 
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

BigInteger::BigInteger(const char* s)
{
    digits = "";
    for (int i = strlen(s) - 1; i >= 0; i--) 
    {
        if (!isdigit(s[i]))
            throw("ERROR CONVERT");
        else
            digits.push_back(s[i] - '0');
    }
}

BigInteger::BigInteger(BigInteger& a) 
{
    digits = a.digits;
}

BigInteger::BigInteger(const BigInteger& a) 
{
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
    vector<int> v(n + m, 0);
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
    vector<int> quotient(n, 0);
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

string BigInteger::toString() {
    string res="";
    for (int i = this->digits.size() - 1; i >= 0; i--)
         res = res +to_string((short)this->digits[i]);
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