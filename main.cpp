//
// Created by JaycoGAO on 2019-08-02.
//
#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// 减法结果为负，留给除法检测；
// positive=1: 结果为负；
static bool indicator = 0;

// SchoolMethod_Addition
string school_addition(string num01, string num02, int base) {

    // ensure the biggest one is num01
    // swap two numbers
    if (num01.size() < num02.size()) {
        string tmp = num01;
        num01 = num02;
        num02 = tmp;
    }

    int num01_length = num01.size();
    int num02_length = num02.size();
    int parta;
    int partb;
    int sum_current = 0;
    int carry_current = 0;

    // add(char addition) from LSB to MSB in string
    while (num01_length > 0) {
        parta = num01[num01_length - 1] - '0';
        // if num02 bits exist
        if (num02_length > 0) {
            partb = num02[num02_length - 1] - '0';
        } else {
            // num02 no bit
            partb = 0;
        }
        sum_current = parta + partb + carry_current;
        num01[num01_length - 1] = '0' + sum_current % base;
//        cout << "num01[num01_length]: " << "\t" << num01[num01_length]<< endl;
        carry_current = sum_current / base;
        num01_length--;
        num02_length--;
    }
    // if bit overflow, add 1 as MSB (string addition)
    if (carry_current == 1) {
        num01 = "1" + num01;
    }
    return num01;
}

// SchoolMethod_Minus
string school_minus(string num01, string num02, int base) {

    string result_minus;
    int num01_length = num01.length();
    int num02_length = num02.length();
    int gap_length = num01_length - num02_length;
    int carry = 0;
    if (gap_length < 0) {
        // when num01 < num02, go on but the indicator will record it
        for (int i = 0; i < num02_length - num01_length; ++i) {
            num01 = "0" + num01;
        }
        //num01 smaller than num02
//        return 0;
    } else {
        while (gap_length > 0) {
            num02 = "0" + num02;
            gap_length--;
        }
    }

    // from LSB to MSB
    for (int i = num01.length() - 1; i >= 0; i--) {
        int parta = num01[i] - '0';
        int partb = num02[i] - '0';
        int minus_current = parta - partb + carry;
        // when parta < partb
        if (minus_current < 0) {
            carry = -1;
            minus_current += base;
        } else {
            carry = 0;
        }
        // number into string
        result_minus += to_string(minus_current);
    }
    // reverse to be the result
    for (unsigned int i = 0; i < result_minus.length() / 2; i++) {
        swap(result_minus[i], result_minus[result_minus.length() - i - 1]);
    }
    // 如果num01 < num02, 这里预留以备除法检测；
    if (carry == -1) {
        indicator = 1;
    }
    return result_minus;
}

// school_Multiplication
string school_multiply(string num01, string num02, int base) {

    string result_multiply = "";
    int num01_length = num01.size();
    int num02_length = num02.size();
    int num01_index = 0;
    int num02_index = 0;
    if (num01_length == 0 || num02_length == 0)
        return "0";

    // from LSB to MSB into vector container
    vector<int> result(num01_length + num02_length, 0);
    // from LSB to MSB of num01
    for (int i = num01_length - 1; i >= 0; i--) {
        int carry = 0;
        int num01_current = num01[i] - '0';
        num02_index = 0;
        int j = num02_length - 1;
        while (j >= 0) {
            int num02_current = num02[j] - '0';
            int sum = num01_current * num02_current + result[num01_index + num02_index] + carry;
            result[num01_index + num02_index] = sum % base;
            carry = sum / base;
            num02_index++;
            j--;
        }
        if (carry > 0)
            result[num01_index + num02_index] += carry;
        num01_index++;
    }
    int i = result.size() - 1;
    while (i >= 0 && result[i] == 0)
        i--;
    if (i == -1)
        return "0";
    while (i >= 0)
        result_multiply += to_string(result[i--]);
    return result_multiply;
}

// delete leading 0 for division
string delete_leading_zero(string input) {

    string result;
    int index = 0;
    int count = 0;
    int tmp_length = input.size();
    while (index < tmp_length) {
        if (input.at(index) == '0') {
            count++;
        } else
            break;
        index++;
    }
    result = input.substr(count, input.size());
    return result;
}

// Karatsuba Multiplication
string karatsuba(string num01, string num02, int base) {

    if (num01.length() == 1 || num02.length() == 1) {
        int parta;
        int partb;

//      from string to int
        std::istringstream tmp01(num01);
        tmp01 >> parta;

        std::istringstream tmp02(num02);
        tmp02 >> partb;

        int result_current = parta * partb;
        int carry = 0;
        if (result_current >= base) {
            while (result_current >= base) {
                result_current -= base;
                carry++;
            }
        }
        string sum_current = to_string(carry) + to_string(result_current);
        return sum_current;
    }

    if (num01.length() < num02.length()) {
        for (unsigned int i = 0; i < num02.length() - num01.length(); i++) {
            num01.insert(0, "0");
        }
    } else if (num01.length() > num02.length()) {
        for (unsigned int i = 0; i < num01.length() - num02.length(); i++) {
            num02.insert(0, "0");
        }
    }

    int middle = num01.length() / 2;

    // split strings
    string a = num01.substr(0, num01.length() - middle);
    string b = num01.substr(num01.length() - middle, num01.size());

    string c = num02.substr(0, num02.length() - middle);
    string d = num02.substr(num02.length() - middle, num02.size());

    // when bits <= 3, algorithm optimization
    if (num01.length() <= 3) {
        string tmp01 = school_multiply(a, c, base);
        string tmp02 = school_multiply(b, d, base);
        string result_school_multiply = school_addition(school_multiply(a, d, base), school_multiply(b, c, base), base);
        int i = 0;
        while (i < middle * 2) {
            tmp01 += '0';
            i++;
        }
        int j = 0;
        while (j < middle) {
            result_school_multiply += '0';
            j++;
        }
        return school_addition(tmp01, school_addition(result_school_multiply, tmp02, base), base);
    } else {
        string ac_based = school_multiply(a, c, base);
        string bd_based = school_multiply(b, d, base);
        string ab_cd_based = school_multiply(school_addition(a, b, base), school_addition(c, d, base), base);
        ab_cd_based = school_minus(ab_cd_based, school_addition(ac_based, bd_based, base), base);
        int i = 0;
        while (i < middle * 2) {
            ac_based += '0';
            i++;
        }
        int j = 0;
        while (j < middle) {
            ab_cd_based += '0';
            j++;
        }
        string tmp = school_addition(ac_based, school_addition(ab_cd_based, bd_based, base), base);
        return delete_leading_zero(tmp);
    }
}

// 基于加法的除法：a - b*n;
int school_division(string num01, string num02, int base) {
    indicator = 0;
    int result_division = 0;
    // num01-num02初次减法结果默认为正
    string tmp = school_minus(num01, num02, base);
    result_division++;
    // 当indicator=1说明减法结果为负，否则为正；
    while (indicator != 1) {
        tmp = school_minus(tmp, num02, base);
        result_division++;
        // 减法过程测试:
//        cout << "result_division: " << result_division << endl;
//        cout << "num01: " << num01 << endl;
//        cout << "num02: " << num02 << endl;
//        cout << "tmp: " << tmp << endl;
//        cout << "---" << endl;
    }
    return result_division - 1;
}


// Division for big integer
string division(string num01, string num02, int base) {

    string result = "";

    int num01_length = num01.size();
    int num02_length = num02.size();
    string num01_left = num01.substr(0, num02_length);
    string num01_right = num01.substr(num02_length, num01_length);

    string division_current;
    unsigned int i = 0;
    string module = num01_left;

    string result_first = to_string(school_division(num01_left, num02, base));
    result += result_first;
    module = school_minus(num01_left, school_multiply(num02, result_first, base), base);

//    cout << "result_first: " << result_first << endl;
//    cout << "result: " << result << endl;
//    cout << "module: " << module << endl;

    while (i < num01_right.size()) {
        num01_left = module + num01_right[i];
        string result_current = to_string(school_division(num01_left, num02, base));
        result += result_current;
        module = school_minus(num01_left, school_multiply(num02, result_current, base), base);
        i++;
//        cout << "result_current: " << "\t" << result_current << endl;
//        cout << "module: " << "\t" << module << endl;
//        cout << "num01_right[i]: " << "\t" << num01_right[i] << endl;
    }
    return delete_leading_zero(result);
}

int main() {
    // get input of three numbers
    string num01, num02;
    int base;
    cin >> num01;
    cin >> num02;
    cin >> base;

    string result_addition;
    string result_minus;
    string result_multiplication;
    string result_division;

//    cout << "result_addition " << school_addition(num01, num02, base) << endl;
//    cout << "result_minus: " << school_minus(num01, num02, base) << endl;
//    cout << "result_schoolMultiply " << school_multiply(num01, num02, base) << endl;
//    cout << "result_multiplication " << karatsuba(num01, num02, base) << endl;
//    cout << "result_division: " << school_division(num01, num02, base) << endl;
//    cout << "result_division: " << division(num01, num02, base) << endl;

//     加法 + 乘法 + 除法结果导出
    cout << school_addition(num01, num02, base) << "\t" << karatsuba(num01, num02, base) << "\t"
         << division(num01, num02, base);
    return 0;
}