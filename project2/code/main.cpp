//
//  main.cpp
//  algorithm_expression
//
//  Created by VNB on 2020/10/12.
//

#include <cmath>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class Exception {
   public:
    string msg;
    Exception(string s) : msg(s) {}
};

unordered_map<char, int> priority = {
    {'+', 1}, {'-', 1}, {'*', 2}, {'/', 2}, {'^', 3}};
const vector<char> ops = {'+', '-', '*', '/', '^', '#', '!'};

string convertStr(stack<char> s);
string convertRPN(const string& str);
double calculateRPN(const string& str);
int factorial(int num);
bool validCharacter(char s);
bool isOperator(char s);
bool isNum(char s);
bool isBracket(char s);


int main(int argc, const char* argv[]) {
    while (true) {
        cout << "Input the expression (or input 'q' exit program) : ";
        string str;
        cin >> str;
        if ("q" == str) break;
        try {
            double res = calculateRPN(convertRPN(str));
            cout << str.substr(0, str.find('#')) << " = " << res << endl
                 << endl;
        } catch (Exception e) {
            cout << e.msg << endl << endl;
        }
    }
    cout << "Bye" << endl << endl;
    return 0;
}

string convertStr(stack<char> s) {
    string res;
    while (!s.empty()) {
        res = s.top() + res;
        s.pop();
    }
    return res;
}

string convertRPN(const string& str) {
    stack<char> s1;
    stack<char> s2;
    bool lastIsNum = true;
    int leftBracket = 0;
    for (int i = 0; i < str.size(); ++i) {
        char s = str[i];
        if (!validCharacter(s) || (s == ')' && !leftBracket))
            throw Exception("You entered an illegal expression.");
        if (s == '#') break;
        if (s == '-' && (i == 0 || !isNum(str[i - 1]))) {
            if (lastIsNum) s1.push(' ');
            s1.push('0');
            s2.push(s);
            continue;
        }
        if (s == '!') {
            s1.push(s);
            continue;
        }
        if (isNum(s)) {
            if (lastIsNum) s1.push(' ');
            lastIsNum = false;
            s1.push(s);
            continue;
        } else if (s == '(') {
            s2.push(s);
            ++leftBracket;
        } else if (s == ')' && !s2.empty()) {
            if (s1.empty() || str[i - 1] == '(' || leftBracket <= 0)
                throw Exception("You entered an illegal expression.");
            while (s2.top() != '(') {
                s1.push(s2.top());
                s2.pop();
            }
            --leftBracket;
            s2.pop();
        } else {
            if (!s2.empty()) {
                if (priority[s] > priority[s2.top()]) {
                    s2.push(s);
                } else {
                    while (!s2.empty() && priority[s] <= priority[s2.top()]) {
                        s1.push(s2.top());
                        s2.pop();
                    }
                    s2.push(s);
                }
            } else {
                s2.push(s);
            }
        }
        lastIsNum = true;
    }
    if (leftBracket) throw Exception("You entered an illegal expression.");
    while (!s2.empty()) {
        s1.push(s2.top());
        s2.pop();
    }
    return convertStr(s1);
}

double calculateRPN(const string& str) {
    if (str.empty()) throw Exception("You entered an illegal expression.");
    stack<double> st;
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] == ' ') {
            continue;
        } else if (isOperator(str[i])) {
            double num1, num2;
            if (!st.empty()) {
                num2 = st.top();
                st.pop();
            } else {
                throw Exception("You entered an illegal expression.");
            }
            // calculate factorial
            if (str[i] == '!') {
                st.push(factorial(num2));
                continue;
            }
            if (!st.empty()) {
                num1 = st.top();
                st.pop();
            } else {
                throw Exception("You entered an illegal expression.");
            }
            if (str[i] == '+')
                st.push(num1 + num2);
            else if (str[i] == '-')
                st.push(num1 - num2);
            else if (str[i] == '*')
                st.push(num1 * num2);
            else if (str[i] == '/') {
                if (num2 == 0) throw Exception("The divisor cannot be zero.");
                st.push(num1 / num2);
            } else if (str[i] == '^') {
                st.push(pow(num1, num2));
            }
        } else {
            int num = 0;
            while (i < str.size() && str[i] >= '0' && str[i] <= '9') {
                num = num * 10 + (str[i++] - '0');
            }
            st.push(num);
            if (i < str.size() && str[i] != ' ') --i;
        }
    }
    if (st.size() > 1) throw Exception("You entered an illegal expression.");
    return st.top();
}

int factorial(int num) {
    if (num < 0) throw Exception("You entered an illegal expression.");
    int sum = 1;
    for (int i = 2; i <= num; ++i) sum *= i;
    return sum;
}

bool validCharacter(char s) {
    return isOperator(s) || isNum(s) || isBracket(s);
}

bool isOperator(char s) {
    for (const auto& op : ops)
        if (op == s) return true;
    return false;
}

bool isNum(char s) { return s >= '0' && s <= '9'; }

bool isBracket(char s) { return s == '(' || s == ')'; }
