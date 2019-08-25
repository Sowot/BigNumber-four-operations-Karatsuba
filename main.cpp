
// BigNumber four operations by base, length of BigNumber can be up to 100-digit;
// addition, subtraction, multiplication(Karatsuba Algorithm), division;
// input: string num01, string num02, int base;
// output: bigNumber addition + Karatsuba multiplication + division;

// Assignment 1 for Algorithm and Data Structure Analysis of University of Adelaide;
// WARNING: DO NOT copy the document thoroughly or change the variables solely, plagiarism may occur;
// WARNING: The "main.cpp" is validated within the accepted running time by University WebSubmission system;
// Feel free to contact me by hello@jaycogao.com;

#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// set static variable for "school_subtraction" as the indicator if "a-b < 0" / 全局变量indicator用于服务循环减法;
// indicator = 1: a-b < 0 / 当indicator为1，说明 a-b < 0;
static bool indicator = 0;

// SchoolMethod_Addition / 教学式加法(按位累加，保留进位);
string school_addition(string num01, string num02, int base) {

    // ensure the biggest one is num01 / 保证num01是最大值;
    // swap two numbers / 否则交换num01和num02位置;
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

    // add(char addition) from LSB to MSB in string / 从字符串的最后一位往前加;
    while (num01_length > 0) {
        parta = num01[num01_length - 1] - '0';
        // if num02 bits exist / 当num02还有位数;
        if (num02_length > 0) {
            partb = num02[num02_length - 1] - '0';
        } else {
            // num02 no bit / 当num02没有位数;
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

// SchoolMethod_subtraction / 教学式减法;
string school_subtraction(string num01, string num02, int base) {

    string result_minus;
    int num01_length = num01.length();
    int num02_length = num02.length();
    int gap_length = num01_length - num02_length;
    int carry = 0;
    // unify the digits of num01 & num02 / 统一num01和num02的位数;
    if (gap_length < 0) {
        for (int i = 0; i < num02_length - num01_length; ++i) {
            num01 = "0" + num01;
        }
    } else {
        while (gap_length > 0) {
            num02 = "0" + num02;
            gap_length--;
        }
    }

    // from LSB to MSB / 从num01 or num02的最后一位向前减;
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
    // if num01 < num02, indicator will be set to 1 / 如果num01 < num02, indicator变成1, 预留以备除法检测；
    // Default solution here is 100 - 200 => "1" + "100" - "200" => 1100 - 200 = 900, combine "school_division" to pick up;
    // 默认这里将100-200按照100借一位也就是1100-200，结果为900，请结合"school_division"方法理解这里为何这么做；
    if (carry == -1) {
        indicator = 1;
    }
    return result_minus;
}

// school_Multiplication / 教学式乘法
string school_multiply(string num01, string num02, int base) {

    string result_multiply = "";
    int num01_length = num01.size();
    int num02_length = num02.size();
    int num01_index = 0;
    int num02_index = 0;
    if (num01_length == 0 || num02_length == 0)
        return "0";

    // from LSB to MSB into vector container / 将末位→第一位存入vector;
    // identical solution with that in "school_subtraction" / 与"school_subtraction"方法一样的解决思维;
    vector<int> result(num01_length + num02_length, 0);
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

// delete leading 0 for division / 删除结果开头无意义的'0';
// "00123456" => "123456";
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

// Karatsuba BigNumber Multiplication / Karatsuba 大数乘法
// logic induction provided by Wikipeadia / 算法逻辑推理请参考维基百科 => https://en.wikipedia.org/wiki/Karatsuba_algorithm
string karatsuba(string num01, string num02, int base) {

    if (num01.length() == 1 || num02.length() == 1) {
        int parta;
        int partb;

//      from string to int / C++ 中使用 #include <sstream> 将string转为int;
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

    // when digits not same, unify them / 当位数不一致，补齐位数;
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

    // when bits <= 3, algorithm optimization(by math induction) / 当只有三位(数学归纳法推理得出)，可直接使用学校乘法优化算法;
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
        ab_cd_based = school_subtraction(ab_cd_based, school_addition(ac_based, bd_based, base), base);
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
        // delete the leading zero / 删除结果的开头无意义的0;
        // Example: "00123456" => "123456";
        return delete_leading_zero(tmp);
    }
}

// school_division for smallNumber based on "school_subtraction" / 基于教学式减法的教学式除法;
// only prepare for "division", not used this method solely to carry out bigNumber division(running exceed)
// 该方法预留给"division"大数除法用，不作为大数除法的直接方法使用(running exceed);
int school_division(string num01, string num02, int base) {
    // reset the indicator / 重置 indicator;
    indicator = 0;
    int result_division = 0;
    // this tmp set to be positive / num01-num02初次减法结果默认为正;
    string tmp = school_subtraction(num01, num02, base);
    result_division++;
    // when indicator =0 means the last subtraction loop is negative => a<b, end up "result_division" counting;
    // 当indicator=1说明最后一次循环减法结果为负，result_division可以终止计数;
    while (indicator != 1) {
        tmp = school_subtraction(tmp, num02, base);
        result_division++;
    }
    return result_division - 1;
}

// Division for big integer / 大数除法
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
    module = school_subtraction(num01_left, school_multiply(num02, result_first, base), base);

    while (i < num01_right.size()) {
        num01_left = module + num01_right[i];
        string result_current = to_string(school_division(num01_left, num02, base));
        result += result_current;
        module = school_subtraction(num01_left, school_multiply(num02, result_current, base), base);
        i++;
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

    // testing lines for each independent method to debug / 各方法的debug独立测试行
//    cout << "result_addition " << school_addition(num01, num02, base) << endl;
//    cout << "result_minus: " << school_subtraction(num01, num02, base) << endl;
//    cout << "result_schoolMultiply " << school_multiply(num01, num02, base) << endl;
//    cout << "result_multiplication " << karatsuba(num01, num02, base) << endl;
//    cout << "result_division: " << school_division(num01, num02, base) << endl;
//    cout << "result_division: " << division(num01, num02, base) << endl;

//    bigNumber addition + Karatsuba multiplication + division / 加法 + 乘法 + 除法结果统一导出
    cout << school_addition(num01, num02, base) << "\t" << karatsuba(num01, num02, base) << "\t"
         << division(num01, num02, base);
    return 0;
}
