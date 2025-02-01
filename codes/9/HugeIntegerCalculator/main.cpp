#include <iostream>
#include <string>
using namespace std;

class HugeInteger {
    friend ostream& operator<<(ostream &output, const HugeInteger &out) {
        if (out.isNegative) {
            output << '-';
        }
        for (int i = out.length - 1; i >= 0; --i) {
            output << out.number[i];
        }
        return output;
    }

private:
    int *number;
    int length;
    bool isNegative;

    string RemoveLeadingZeros(const string& input) const {
        int firstNonZero = 0;
        while (firstNonZero < input.size() && input[firstNonZero] == '0') {
            ++firstNonZero;
        }
        if (firstNonZero == input.size()) {
            return "0";
        }
        return input.substr(firstNonZero);
    }

    bool max(HugeInteger& a,HugeInteger& b){
        if(a.length > b.length) return 1;
        else if(a.length < b.length) return 0;
        if(a.number[a.length-1] > b.number[b.length-1]) return 1;
        return 0;
    }

    HugeInteger HelpOperator(HugeInteger& a) {
        int *num1,*num2,len1,len2;
        bool resultSign;
        if(max(*this,a)) {
            len1 = length;
            num1 = new int[len1];
            for (int i{0} ; i < len1 ; ++i) {
                num1[i] = number[i];
            }
            num2 = a.number;
            len2 = a.length;
            resultSign = isNegative;
        }
        else{
            len1 = a.length;
            num1 = new int[len1];
            for (int i{0} ; i < len1 ; ++i) {
                num1[i] = a.number[i];
            }
            num2 = number;
            len2 = length;
            resultSign = a.isNegative;
        }

        int carry{0},sum{0};
        for (int i{0}; i < len1 ; ++i) {
            int digit1 = num1[i];
            int digit2 = (i < len2) ? num2[i] : 0;

            if(isNegative != a.isNegative) digit2 = -digit2;
            sum = digit1 + digit2 + carry;
            if(sum < 0){
                num1[i] = sum + 10;
                carry = -1;
            }
            else{
                num1[i] = sum % 10;
                carry = sum / 10;
            }
        }
        string str;
        for (int i = len1 - 1; i >= 0; --i) {
            if (i == len1 - 1 && num1[i] == 0) continue;
            str += ('0' + num1[i]);
        }


        str = RemoveLeadingZeros(str);

        if (resultSign && str != "0") {
            str = "-" + str;
        }

        return HugeInteger(str);


    }

public:
    HugeInteger(string input) {
        input = RemoveLeadingZeros(input);
        isNegative = (input[0] == '-');
        length = input.length() - isNegative;
        number = new int[length];

        for (int i = 0; i < length; ++i) {
            number[i] = input[input.length() - 1 - i] - '0';
        }
    }

    HugeInteger(const HugeInteger& other) {
        length = other.length;
        number = new int[length];
        isNegative = other.isNegative;
        for (int i = 0; i < length; ++i) {
            number[i] = other.number[i];
        }
    }

    ~HugeInteger() {
        delete[] number;
    }

    HugeInteger& operator=(const HugeInteger& other) {
        if (this != &other) {
            delete[] number;

            length = other.length;
            isNegative = other.isNegative;
            number = new int[length];
            for (int i = 0; i < length; ++i) {
                number[i] = other.number[i];
            }
        }
        return *this;
    }

    HugeInteger operator+(HugeInteger& a) {
        return HelpOperator(a);
    }

    HugeInteger operator-(HugeInteger& a) {
        a.isNegative = !a.isNegative;
        return HelpOperator(a);
    }

    HugeInteger operator*(HugeInteger& a) {
        int *num1, *num2, len1, len2;
        bool resultSign;

        if (max(*this, a)) {
            num1 = number;
            len1 = length;
            num2 = a.number;
            len2 = a.length;
            resultSign = (isNegative != a.isNegative);
        } else {
            num1 = a.number;
            len1 = a.length;
            num2 = number;
            len2 = length;
            resultSign = (isNegative != a.isNegative);
        }


        int *result = new int[len1 + len2]();


        for (int i{0}; i < len1; ++i) {
            int carry = 0;
            for (int j = 0; j < len2; ++j) {
                int prod = num1[i] * num2[j] + result[i + j] + carry;
                result[i + j] = prod % 10;
                carry = prod / 10;
            }
            result[i + len2] += carry;
        }

        string str;
        for (int i = len1 + len2 - 1; i >= 0; --i) {
            str += (result[i] + '0');
        }

        delete[] result;
        str = RemoveLeadingZeros(str);

        if (resultSign && str != "0") {
            str = "-" + str;
        }

        return HugeInteger(str);
    }




    bool operator==(HugeInteger& a) {
        if (length != a.length) return false;
        for (int i = 0; i < length; ++i) {
            if (number[i] != a.number[i]) return false;
        }
        return true;
    }

};

int main() {
    HugeInteger a("5734574076409769047690475903759057349057495743097649065");
    HugeInteger b("10");
    cout << a + b  << endl;


    return 0;
}