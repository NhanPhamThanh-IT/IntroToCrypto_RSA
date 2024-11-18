#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <stack>
#include <vector>

using namespace std;

#define MAX_DIGITS 309

struct BigNum
{
    int Num[MAX_DIGITS] = {};
    bool negative = false;
};

struct DivResult
{
    BigNum Result;
    BigNum Remainder;
};

struct ArrayOfArray
{
    BigNum Result;
    BigNum Count;
};

class LongNumbers
{
public:
    const int Size2048 = MAX_DIGITS;
    BigNum StringToArray(string input);
    BigNum CopyOf(BigNum input);
    BigNum AddFront(BigNum input, int val);
    bool EqualZero(BigNum input);
    BigNum Add(BigNum first, BigNum second);
    BigNum Sub(BigNum firstOriginal, BigNum second);
    BigNum Mul(BigNum first, BigNum second);
    DivResult DivSmall(BigNum first, BigNum second);
    DivResult DivLarge(BigNum first, BigNum second);
    BigNum PwrMod(BigNum firstOriginal, BigNum secondOriginal, BigNum Mod);
    bool IsPrime(BigNum input);
    BigNum Inverse(BigNum input, BigNum mod);
};

BigNum LongNumbers::StringToArray(string input)
{
    BigNum final;
    bool neg = false;
    if (input[0] == '-')
    {
        neg = true;
        input.erase(0, 1);
    }
    reverse(begin(input), end(input));
    int val = 0, itr = 1, Out = 0;
    for (int i = 0; i < input.size(); i++)
    {
        val += (input[i] - '0') * itr;
        itr *= 10;
        if (itr == 100)
        {
            final.Num[Out] = val;
            Out++;
            itr = 1;
            val = 0;
        }
    }
    if (val != 0)
        final.Num[Out] = val;
    final.negative = neg;
    return final;
}

BigNum LongNumbers::CopyOf(BigNum input)
{
    BigNum Result;
    Result.negative = input.negative;
    for (int i = 0; i < MAX_DIGITS; i++)
        Result.Num[i] = input.Num[i];
    return Result;
}

BigNum LongNumbers::AddFront(BigNum input, int val)
{
    BigNum Result;
    Result.negative = input.negative;
    for (int i = 0; i < MAX_DIGITS - 1; i++)
        Result.Num[i + 1] = input.Num[i];
    Result.Num[0] = val;
    return Result;
}

bool LongNumbers::EqualZero(BigNum input)
{
    for (int i = 0; i < Size2048; i++)
        if (input.Num[i] != 0)
            return false;
    return true;
}

BigNum LongNumbers::Add(BigNum first, BigNum second)
{
    if (EqualZero(first))
        return second;
    if (EqualZero(second))
        return first;
    BigNum Result;
    int val = 0, carry = 0;
    bool BothNegative = false;
    if (first.negative && second.negative)
        BothNegative = true;
    else if (first.negative)
    {
        first.negative = false;
        Result = Sub(second, first);
        return Result;
    }
    else if (second.negative)
    {
        second.negative = false;
        Result = Sub(first, second);
        return Result;
    }
    int i = 0;
    for (i; i < Size2048; i++)
    {
        val = (first.Num[i] + second.Num[i] + carry) % 100;
        carry = (first.Num[i] + second.Num[i] + carry) / 100;
        Result.Num[i] = val;
    }
    if (carry != 0)
        Result.Num[i] = carry;
    Result.negative = BothNegative;
    return Result;
}

BigNum LongNumbers::Sub(BigNum firstOriginal, BigNum second)
{
    if (EqualZero(second))
        return firstOriginal;
    if (EqualZero(firstOriginal))
    {
        second.negative = true;
        return second;
    }
    BigNum Result, tempResult, first;
    first = CopyOf(firstOriginal);
    int val = 0, NextToMe = 0;
    bool LastNum = false;
    if (second.negative)
    {
        if (first.negative)
        {
            first.negative = false;
            second.negative = false;
            Result = Sub(second, first);
            return Result;
        }
        else
        {
            second.negative = false;
            Result = Add(first, second);
            return Result;
        }
    }
    else
    {
        if (first.negative)
        {
            first.negative = false;
            second.negative = false;
            Result = Add(first, second);
            Result.negative = true;
            return Result;
        }
    }
    int i = 0;
    for (i; i < Size2048; i++)
    {
        if (LastNum)
            break;
        if (first.Num[i] >= second.Num[i])
        {
            val = first.Num[i] - second.Num[i];
            Result.Num[i] = val;
        }
        else
        {
            if (i == Size2048)
                LastNum = true;
            int temp = i;
            while (temp < MAX_DIGITS - 1)
            {
                temp++;
                NextToMe++;
                if (first.Num[temp] != 0)
                {
                    first.Num[temp] -= 1;
                    first.Num[i] = first.Num[i] + 100;
                    NextToMe--;
                    temp = i + 1;
                    i--;
                    while (NextToMe != 0)
                    {
                        first.Num[temp] = 99;
                        NextToMe--;
                        temp++;
                        ;
                    }
                    break;
                }
                else if (first.Num[temp] == 0 && temp == MAX_DIGITS - 1)
                    LastNum = true;
            }
        }
    }
    if (LastNum == true)
    {
        Result = Sub(second, firstOriginal);
        Result.negative = true;
        return Result;
    }
    return Result;
}

BigNum LongNumbers::Mul(BigNum first, BigNum second)
{
    BigNum Result;
    if (EqualZero(first))
        return first;
    if (EqualZero(second))
        return second;
    int val = 0, carry = 0;
    bool neg = false;
    if (first.negative && second.negative)
    {
        first.negative = false;
        second.negative = false;
    }
    else if (first.negative)
    {
        first.negative = false;
        neg = true;
    }
    else if (second.negative)
    {
        second.negative = false;
        neg = true;
    }
    int i, j;
    for (i = 0; i < Size2048; i++)
    {
        BigNum temp;
        for (j = 0; j < Size2048; j++)
        {
            val = ((first.Num[i] * second.Num[j]) + carry) % 100;
            carry = ((first.Num[i] * second.Num[j]) + carry) / 100;
            temp.Num[j] += val;
        }
        if (i != 0)
        {
            for (int k = 308; k != 0; k--)
                temp.Num[k] = temp.Num[k - i];
            for (int k = 0; k < i; k++)
                temp.Num[k] = 0;
        }
        Result = Add(Result, temp);
    }
    Result.negative = neg;
    return Result;
}

DivResult LongNumbers::DivSmall(BigNum first, BigNum second)
{
    DivResult Result;
    if (EqualZero(first))
    {
        Result.Result = first;
        Result.Remainder = first;
        return Result;
    }
    BigNum countArray, One, tempResult = first;
    One.Num[0] = 1;
    bool CheckNeg = tempResult.negative;
    do
    {
        tempResult = Sub(tempResult, second);
        CheckNeg = tempResult.negative;
        if (countArray.Num[0] != 99)
            countArray.Num[0] = countArray.Num[0] + 1;
        else
            countArray = Add(countArray, One);
    } while (!CheckNeg);
    if (countArray.Num[0] != 0)
    {
        countArray.Num[0] = countArray.Num[0] - 1;
        Result.Result = countArray;
    }
    else
        Result.Result = Sub(countArray, One);
    Result.Remainder = Add(tempResult, second);
    return Result;
}

DivResult LongNumbers::DivLarge(BigNum first, BigNum second)
{
    DivResult Result, tempResult;
    BigNum partOffirst, test;
    if (EqualZero(first))
    {
        Result.Result = first;
        Result.Remainder = first;
    }
    bool neg = false;
    if (first.negative && second.negative)
    {
        first.negative = false;
        second.negative = false;
    }
    else if (first.negative)
    {
        first.negative = false;
        neg = true;
    }
    else if (second.negative)
    {
        second.negative = false;
        neg = true;
    }
    reverse(begin(first.Num), end(first.Num));
    int i = 0;
    while (i != 309)
    {
        do
        {
            partOffirst = AddFront(partOffirst, first.Num[i]);
            test = Sub(partOffirst, second);
            i++;
        } while (test.negative && i != 309);
        tempResult = DivSmall(partOffirst, second);
        Result.Result.Num[309 - i] = tempResult.Result.Num[0];
        partOffirst = tempResult.Remainder;
    }
    Result.Remainder = partOffirst;
    Result.Remainder.negative = neg;
    Result.Result.negative = neg;
    return Result;
}

BigNum LongNumbers::PwrMod(BigNum firstOriginal, BigNum secondOriginal, BigNum Mod)
{
    if (EqualZero(firstOriginal))
        return firstOriginal;
    if (EqualZero(secondOriginal))
    {
        BigNum hop;
        hop.Num[0] = 1;
        return hop;
    }
    BigNum first = CopyOf(firstOriginal);
    BigNum second = CopyOf(secondOriginal);
    stack<ArrayOfArray> memory;
    ArrayOfArray AoA;
    BigNum Result, tempResult, Count, tempCount, check, two;
    DivResult DR;
    two.Num[0] = 2;
    tempCount.Num[0] = 1;
    tempResult = first;
    Result = first;
    do
    {
        DR = DivLarge(tempResult, Mod);
        Result = DR.Remainder;
        Count = tempCount;
        AoA.Result = Result;
        AoA.Count = Count;
        memory.push(AoA);
        tempCount = Mul(Count, two);
        tempResult = Mul(Result, Result);
        check = Sub(second, tempCount);
    } while (!check.negative);
    Result = *new BigNum;
    Result.Num[0] = 1;
    while (!EqualZero(second))
    {
        AoA = memory.top();
        if (!Sub(second, AoA.Count).negative)
        {
            Result = Mul(Result, AoA.Result);
            DR = DivLarge(Result, Mod);
            Result = DR.Remainder;
            second = Sub(second, AoA.Count);
        }
        memory.pop();
    }
    return Result;
}

bool LongNumbers::IsPrime(BigNum input)
{
    bool Result = false;
    DivResult DR;
    BigNum n = CopyOf(input);
    BigNum One, two, a, NMinusOne, K, tempK, Q, test, Calc, power;
    One.Num[0] = 1;
    two.Num[0] = 2;
    a.Num[0] = 2;
    if (Sub(n, a).negative)
        return false;
    DR = DivLarge(n, two);
    if (EqualZero(DR.Remainder))
        return false;
    NMinusOne = Sub(n, One);
    do
    {
        Q = NMinusOne;
        K = tempK;
        DR = DivLarge(NMinusOne, two);
        NMinusOne = DR.Result;
        if (K.Num[0] != 99)
            tempK.Num[0] = K.Num[0] + 1;
        else
            tempK = Add(K, One);
    } while (EqualZero(DR.Remainder));
    NMinusOne = Sub(n, One);
    Calc = PwrMod(a, Q, n);
    DR = DivLarge(Calc, n);
    if (EqualZero(Sub(DR.Remainder, One)) || EqualZero(Sub(DR.Remainder, NMinusOne)))
        return true;
    if (test.Num[0] != 99)
        test.Num[0] = test.Num[0] + 1;
    else
        test = Add(test, One);
    while (!EqualZero(Sub(K, test)))
    {
        Calc = PwrMod(Calc, two, n);
        DR = DivLarge(Calc, n);
        if (EqualZero(Sub(DR.Remainder, One)))
        {
            Result = false;
            break;
        }
        else if (EqualZero(Sub(DR.Remainder, NMinusOne)))
        {
            Result = true;
            break;
        }
        if (test.Num[0] != 99)
            test.Num[0] = test.Num[0] + 1;
        else
            test = Add(test, One);
    }
    return Result;
}

BigNum LongNumbers::Inverse(BigNum input, BigNum mod)
{
    BigNum Result, Q, A2, A3, B2, B3, T2, T3, Zero, One, temp;
    One.Num[0] = 1;
    A2 = CopyOf(Zero);
    A3 = CopyOf(mod);
    B2 = CopyOf(One);
    B3 = CopyOf(input);
    while (!EqualZero(B3) && !EqualZero(Sub(B3, One)))
    {
        Q = DivLarge(A3, B3).Result;
        T2 = Sub(A2, Mul(Q, B2));
        T3 = Sub(A3, Mul(Q, B3));
        A2 = CopyOf(B2);
        A3 = CopyOf(B3);
        B2 = CopyOf(T2);
        B3 = CopyOf(T3);
    }
    if (EqualZero(Sub(B3, One)))
    {
        while (B2.negative)
            B2 = Add(B2, mod);
        Result = B2;
        temp = Sub(Result, mod);
        while (!temp.negative)
        {
            Result = temp;
            temp = Sub(Result, mod);
        }
    }
    return Result;
}

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
    friend bool operator!=(const BigIntHolder&, const BigIntHolder&);
    friend bool operator>(const BigIntHolder&, const BigIntHolder&);
    friend bool operator>=(const BigIntHolder&, const BigIntHolder&);
    friend bool operator<(const BigIntHolder&, const BigIntHolder&);
    friend bool operator<=(const BigIntHolder&, const BigIntHolder&);
    friend BigIntHolder& operator*=(BigIntHolder&, const BigIntHolder&);
    friend BigIntHolder operator*(const BigIntHolder&, const BigIntHolder&);
    friend BigIntHolder& operator/=(BigIntHolder&, const BigIntHolder&);
    friend BigIntHolder operator/(const BigIntHolder&, const BigIntHolder&);
    friend BigIntHolder operator%(const BigIntHolder&, const BigIntHolder&);
    friend BigIntHolder& operator%=(BigIntHolder&, const BigIntHolder&);
    friend BigIntHolder& operator^=(BigIntHolder&, const BigIntHolder&);
    friend BigIntHolder operator^(BigIntHolder&, const BigIntHolder&);
    friend BigIntHolder sqrt(BigIntHolder& a);
};

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

bool operator!=(const BigIntHolder& a, const BigIntHolder& b) 
{
    return !(a == b);
}

bool operator<(const BigIntHolder& a, const BigIntHolder& b) 
{
    int n = Length(a), m = Length(b);
    if (n != m)
        return n < m;
    while (n--)
        if (a.digits[n] != b.digits[n])
            return a.digits[n] < b.digits[n];
    return false;
}

bool operator>(const BigIntHolder& a, const BigIntHolder& b) 
{
    return b < a;
}

bool operator>=(const BigIntHolder& a, const BigIntHolder& b) 
{
    return !(a < b);
}

bool operator<=(const BigIntHolder& a, const BigIntHolder& b) 
{
    return !(a > b);
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
        for (cc = 9; cc * b > t; cc--);
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
        for (cc = 9; cc * b > t; cc--);
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

BigIntHolder& operator^=(BigIntHolder& a, const BigIntHolder& b)
{
    BigIntHolder Exponent, Base(a);
    Exponent = b;
    a = 1;
    while (!Null(Exponent)) 
    {
        if (Exponent[0] & 1)
            a *= Base;
        Base *= Base;
        divide_by_2(Exponent);
    }
    return a;
}

BigIntHolder operator^(BigIntHolder& a, BigIntHolder& b) 
{
    BigIntHolder temp(a);
    temp ^= b;
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

BigIntHolder sqrt(BigIntHolder& a) 
{
    BigIntHolder left(1), right(a), v(1), mid, prod;
    divide_by_2(right);
    while (left <= right) 
    {
        mid += left;
        mid += right;
        divide_by_2(mid);
        prod = (mid * mid);
        if (prod <= a) 
        {
            v = mid;
            ++mid;
            left = mid;
        }
        else 
        {
            --mid;
            right = mid;
        }
        mid = BigIntHolder();
    }
    return v;
}

string BigIntHolder::toString()
{
    string res="";
    for (int i = this->digits.size() - 1; i >= 0; i--)
         res = res +to_string((short)this->digits[i]);
    return res;
}

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
    LongNumbers MyNum;
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
        BigNum n;
        string nStr = k.toString();
        n = MyNum.StringToArray(nStr);
        int res;
        res = MyNum.IsPrime(n);
        ofstream outputTest(argv[2]);
        outputTest << res;
        outputTest.close();
    }
    return 0;
}