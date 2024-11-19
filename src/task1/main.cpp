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

class BigIntHolder {
    string digits;
public:
    BigIntHolder(string&);
    BigIntHolder(BigIntHolder&);
    BigIntHolder(unsigned long long n = 0);
    BigIntHolder(const char*);
    friend void divide_by_2(BigIntHolder& a);
    friend bool Null(const BigIntHolder&);
    friend int Length(const BigIntHolder&);
    int operator[](const int)const;
    string toString();
    BigIntHolder& operator = (const BigIntHolder&);
    BigIntHolder& operator++();
    BigIntHolder operator++(int temp);
    BigIntHolder& operator--();
    BigIntHolder operator--(int temp);
    friend BigIntHolder& operator+=(BigIntHolder&, const BigIntHolder&);
    friend BigIntHolder operator+(const BigIntHolder&, const BigIntHolder&);
    friend BigIntHolder& operator-=(BigIntHolder&, const BigIntHolder&);
    friend BigIntHolder operator-(const BigIntHolder&, const BigIntHolder&);
    friend bool operator==(const BigIntHolder&, const BigIntHolder&);
    friend bool operator<(const BigIntHolder&, const BigIntHolder&);
    friend BigIntHolder& operator*=(BigIntHolder&, const BigIntHolder&);
    friend BigIntHolder operator*(const BigIntHolder&, const BigIntHolder&);
    friend BigIntHolder& operator/=(BigIntHolder&, const BigIntHolder&);
    friend BigIntHolder operator/(const BigIntHolder&, const BigIntHolder&);
    friend BigIntHolder operator%(const BigIntHolder&, const BigIntHolder&);
    friend BigIntHolder& operator%=(BigIntHolder&, const BigIntHolder&);
};

BigIntHolder hexadecimalToDecimal(string hexVal) 
{
    int len = hexVal.size();
    BigIntHolder base("1");
    BigIntHolder dec_val("0");
    for (int i = 0; i < len; i++) 
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

int main(int argc,char**argv)
{
    LargeNumberOperations MyNum;
    string hex ;
    int ouput;
    BigIntHolder k;
    string test_id;
    if (argc < 3)
        cout << "Not enough Command Line Argument passed!";
    else {
        ifstream inputTest(argv[1]);
        inputTest >> hex;
        inputTest.close();
        k = hexadecimalToDecimal(hex);
        LargeNumber n;
        string nStr = k.toString();
        n = MyNum.convertStringToLargeNumber(nStr);
        int res;
        res = MyNum.isPrimeNumber(n);
        ofstream outputTest(argv[2]);
        outputTest << res;
        outputTest.close();
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

BigIntHolder::BigIntHolder(string& s) 
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

BigIntHolder::BigIntHolder(unsigned long long numConvert) 
{
    do {
        digits.push_back(numConvert % 10);
        numConvert /= 10;
    } while (numConvert != 0);
}

BigIntHolder::BigIntHolder(const char* s)
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

BigIntHolder::BigIntHolder(BigIntHolder& a) 
{
    digits = a.digits;
}

bool Null(const BigIntHolder& a) 
{
    if (a.digits.size() == 1 && a.digits[0] == 0)
        return true;
    return false;
}

int Length(const BigIntHolder& a) 
{
    return a.digits.size();
}

int BigIntHolder::operator[](const int index) const
{
    if (digits.size() <= index || index < 0)
        throw("ERROR");
    return digits[index];
}

bool operator==(const BigIntHolder& a, const BigIntHolder& b) 
{
    return a.digits == b.digits;
}

bool operator<(const BigIntHolder& a, const BigIntHolder& b) 
{
    int n = Length(a), m = Length(b);
    if (n != m)
        return n < m;
    while (n--)
        if (!(a.digits[n] == b.digits[n]))
            return a.digits[n] < b.digits[n];
    return false;
}

BigIntHolder& BigIntHolder::operator=(const BigIntHolder& a) 
{
    digits = a.digits;
    return *this;
}

BigIntHolder& BigIntHolder::operator++() 
{
    int i, n = digits.size();
    for (i = 0; i < n && digits[i] == 9; i++)
        digits[i] = 0;
    if (i == n)
        digits.push_back(1);
    else
        digits[i]++;
    return *this;
}

BigIntHolder BigIntHolder::operator++(int temp) 
{
    BigIntHolder aux;
    aux = *this;
    ++(*this);
    return aux;
}

BigIntHolder& BigIntHolder::operator--() 
{
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

BigIntHolder BigIntHolder::operator--(int temp) 
{
    BigIntHolder aux;
    aux = *this;
    --(*this);
    return aux;
}

BigIntHolder& operator+=(BigIntHolder& a, const BigIntHolder& b) 
{
    int t = 0, s, i;
    int n = Length(a), m = Length(b);
    if (m > n)
        a.digits.append(m - n, 0);
    n = Length(a);
    for (i = 0; i < n; i++) 
    {
        s = (i < m) ? (a.digits[i] + b.digits[i] + t) : (a.digits[i] + t);
        t = s / 10;
        a.digits[i] = (s % 10);
    }
    if (t)
        a.digits.push_back(t);
    return a;
}

BigIntHolder operator+(const BigIntHolder& a, const BigIntHolder& b) 
{
    BigIntHolder temp;
    temp = a;
    temp += b;
    return temp;
}

BigIntHolder& operator-=(BigIntHolder& a, const BigIntHolder& b) 
{
    if (a < b)
        throw("UNDERFLOW");
    int n = Length(a), m = Length(b);
    int i, t = 0, s;
    for (i = 0; i < n; i++) 
    {
        if (i < m)
            s = a.digits[i] - b.digits[i] + t;
        else
            s = a.digits[i] + t;
        if (s < 0)
            s += 10,
            t = -1;
        else
            t = 0;
        a.digits[i] = s;
    }
    while (n > 1 && a.digits[n - 1] == 0)
        a.digits.pop_back(),
        n--;
    return a;
}

BigIntHolder operator-(const BigIntHolder& a, const BigIntHolder& b)
{
    BigIntHolder temp;
    temp = a;
    temp -= b;
    return temp;
}

BigIntHolder& operator*=(BigIntHolder& a, const BigIntHolder& b)
{
    if (Null(a) || Null(b)) 
    {
        a = BigIntHolder();
        return a;
    }
    int n = a.digits.size(), m = b.digits.size();
    vector<int> v(n + m, 0);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            v[i + j] += (a.digits[i]) * (b.digits[j]);
    n += m;
    a.digits.resize(v.size());
    for (int s, i = 0, t = 0; i < n; i++)
    {
        s = t + v[i];
        v[i] = s % 10;
        t = s / 10;
        a.digits[i] = v[i];
    }
    for (int i = n - 1; i >= 1 && !v[i]; i--)
        a.digits.pop_back();
    return a;
}

BigIntHolder operator*(const BigIntHolder& a, const BigIntHolder& b) 
{
    BigIntHolder temp;
    temp = a;
    temp *= b;
    return temp;
}

BigIntHolder& operator/=(BigIntHolder& a, const BigIntHolder& b) 
{
    if (Null(b))
        throw("Arithmetic Error: Division By 0");
    if (a < b) {
        a = BigIntHolder();
        return a;
    }
    if (a == b) {
        a = BigIntHolder(1);
        return a;
    }
    int i, lgcat = 0, cc;
    int n = Length(a), m = Length(b);
    vector<int> cat(n, 0);
    BigIntHolder t;
    for (i = n - 1; t * 10 + a.digits[i] < b; i--) 
    {
        t *= 10;
        t += a.digits[i];
    }
    for (; i >= 0; i--)
    {
        t = t * 10 + a.digits[i];
        for (cc = 9; t < cc * b; cc--);
        t -= cc * b;
        cat[lgcat++] = cc;
    }
    a.digits.resize(cat.size());
    for (i = 0; i < lgcat; i++)
        a.digits[i] = cat[lgcat - i - 1];
    a.digits.resize(lgcat);
    return a;
}

BigIntHolder operator/(const BigIntHolder& a, const BigIntHolder& b) 
{
    BigIntHolder temp;
    temp = a;
    temp /= b;
    return temp;
}

BigIntHolder& operator%=(BigIntHolder& a, const BigIntHolder& b) 
{
    if (Null(b))
        throw("Arithmetic Error: Division By 0");
    if (a < b)
        return a;
    if (a == b) 
    {
        a = BigIntHolder();
        return a;
    }
    int i, lgcat = 0, cc;
    int n = Length(a), m = Length(b);
    vector<int> cat(n, 0);
    BigIntHolder t;
    for (i = n - 1; t * 10 + a.digits[i] < b; i--) 
    {
        t *= 10;
        t += a.digits[i];
    }
    for (; i >= 0; i--) 
    {
        t = t * 10 + a.digits[i];
        for (cc = 9; t < cc * b; cc--);
        t -= cc * b;
        cat[lgcat++] = cc;
    }
    a = t;
    return a;
}

BigIntHolder operator%(const BigIntHolder& a, const BigIntHolder& b) 
{
    BigIntHolder temp;
    temp = a;
    temp %= b;
    return temp;
}

void divide_by_2(BigIntHolder& a) 
{
    int add = 0;
    for (int i = a.digits.size() - 1; i >= 0; i--) 
    {
        int digit = (a.digits[i] >> 1) + add;
        add = ((a.digits[i] & 1) * 5);
        a.digits[i] = digit;
    }
    while (a.digits.size() > 1 && !a.digits.back())
        a.digits.pop_back();
}

string BigIntHolder::toString()
{
    string res="";
    for (int i = this->digits.size() - 1; i >= 0; i--)
         res = res +to_string((short)this->digits[i]);
    return res;
}