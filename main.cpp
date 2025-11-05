#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

bool isValidDouble(const string& s) {
    int len = s.length();
    if (len == 0) return false;

    int i = 0;
    if (s[i] == '+' || s[i] == '-') i++;

    bool hasDigitsBeforeDot = false;
    while (i < len && isdigit(s[i])) {
        hasDigitsBeforeDot = true;
        i++;
    }

    bool hasDot = false;
    bool hasDigitsAfterDot = false;
    if (i < len && s[i] == '.') {
        hasDot = true;
        i++;
        while (i < len && isdigit(s[i])) {
            hasDigitsAfterDot = true;
            i++;
        }
    }
    
    if (i != len) return false;
    if (hasDot && (!hasDigitsBeforeDot || !hasDigitsAfterDot)) return false;
    if (!hasDot && !hasDigitsBeforeDot) return false;

    return true;
}

string trimLeadingZeros(const string& s) {
    int i = 0;
    while (i < s.length() && s[i] == '0') i++;
    return (i == s.length()) ? "0" : s.substr(i);
}

void splitNumber(const string& s, string& sign, string& intPart, string& fracPart) {
    int i = 0;
    sign = (s[0] == '+' || s[0] == '-') ? string(1, s[0]) : "+";
    if (s[0] == '+' || s[0] == '-') i++;

    size_t dotPos = s.find('.', i);
    if (dotPos != string::npos) {
        intPart = s.substr(i, dotPos - i);
        fracPart = s.substr(dotPos + 1);
    } else {
        intPart = s.substr(i);
        fracPart = "";
    }

    intPart = trimLeadingZeros(intPart);
}

string addStrings(const string& a, const string& b, int& carry) {
    string result = "";
    for (int i = a.length() - 1; i >= 0; i--) {
        int sum = (a[i] - '0') + (b[i] - '0') + carry;
        carry = sum / 10;
        result += (sum % 10) + '0';
    }
    reverse(result.begin(), result.end());
    return result;
}

string addDoubles(const string& a, const string& b) {
    string signA, intA, fracA;
    string signB, intB, fracB;
    splitNumber(a, signA, intA, fracA);
    splitNumber(b, signB, intB, fracB);

    while (fracA.length() < fracB.length()) fracA += '0';
    while (fracB.length() < fracA.length()) fracB += '0';

    while (intA.length() < intB.length()) intA = '0' + intA;
    while (intB.length() < intA.length()) intB = '0' + intB;

    int carry = 0;
    string fracSum = addStrings(fracA, fracB, carry);
    string intSum = addStrings(intA, intB, carry);

    if (carry > 0) intSum = char(carry + '0') + intSum;

    string result = intSum;
    if (!fracSum.empty()) result += "." + fracSum;
    return result;
}

int main() {
    ifstream infile("input.txt");
    if (!infile) {
        cout << "Error opening input.txt\n";
        return 1;
    }

    string num1, num2;
    int lineNum = 1;
    while (infile >> num1 >> num2) {
        cout << "Line " << lineNum << ": ";
        if (!isValidDouble(num1)) {
            cout << "Invalid number: " << num1 << endl;
        } else if (!isValidDouble(num2)) {
            cout << "Invalid number: " << num2 << endl;
        } else {
            string sum = addDoubles(num1, num2);
            cout << num1 << " + " << num2 << " = " << sum << endl;
        }
        lineNum++;
    }

    infile.close();
    return 0;
}
