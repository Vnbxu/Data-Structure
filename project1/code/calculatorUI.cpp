//
//  calculatorUI.cpp
//  polynormial_calculator
//
//  Created by VNB on 2020/9/22.
//  Copyright © 2020 VNB. All rights reserved.
//

#include "calculatorUI.hpp"

bool checkOperation(string& op) {
    const std::vector<string> operation = {"q",   "prt", "ent", "sum", "sub",
                                           "mul", "der", "val", "del", "lis"};
    for (const auto& ele : operation)
        if (ele == op) return true;
    return false;
}

void calculatorUI::OperationLoop() {
    while (executeOperation(getOperation())) continue;
}

string calculatorUI::getOperation() {
    while (true) {
        printMenu();
        string op;
        cin >> op;
        if (checkOperation(op)) {
            return op;
        } else {
            cout << "Enter the correct option." << endl;
        }
    }
}

bool calculatorUI::executeOperation(const string& t_operation) {
    string operation = t_operation;
    if (operation == "q") {
        return false;
    }
    if (operation == "ent") creat();
    if (vec.size() >= 1) {
        if (operation == "der") der();
        if (operation == "val") val();
        if (operation == "del") remove();
        if (operation == "lis") listPolynomial();
        if (operation == "sum") sum();
        if (operation == "sub") sub();
        if (operation == "mul") mul();
        if (operation == "prt") printPolynomial();
    }
    return true;
}

void calculatorUI::printPolynomial() {
    cout << endl << "PRINT POLYNOMIAL" << endl;
    cout << "| "
         << "CHOOSE 1 POLYNOMIAL TO PRINT" << endl
         << "|";
    auto it = choose(false);
    vec[it.first].printPolynomial();
    cout << endl
         << "END OF PRINT" << endl;
}

void calculatorUI::printMenu() {
    cout << endl;
    cout << "---------------------------- Polynomial calculator "
            "----------------------------"
         << endl;
    cout << "Action : " << endl;
    cout << "ent  - enter new polynomial" << endl;
    if (vec.size() >= 1) {
        cout << "lis  - list of existing polynomial" << endl;
        cout << "prt  - print polynomial in format" << endl;
        cout << "del  - delete a polynomial" << endl << endl;
        cout << "sum  - add up two polynomials" << endl;
        cout << "sub  - subtract two polynomials" << endl;
        cout << "mul  - multiply two polynomials" << endl;
        cout << "der  - solve the derivative funtion" << endl;
        cout << "val  - calculate the value of the polynomial" << endl << endl;
    }
    cout << "q    - quit polynomial calculator" << endl;
    cout << "------------------------------------------------------------------"
            "-------------"
         << endl;
    cout << "Option: ";
}

void calculatorUI::listPolynomial() {
    cout << endl
         << "| "
         << "LIST OF POLYNOMIAL" << endl;
    for (int i = 1; i <= vec.size(); ++i) {
        cout << "|"
             << " NO." << i << endl;
        cout << "| " << vec[i - 1] << endl;
    }
}

std::pair<int, int> calculatorUI::choose(bool flag) {
    int c1 = 0, c2 = 0;
    listPolynomial();
    while (true) {
        cout << "| " << endl
             << "| "
             << "Enter the number to select a polynomial: ";
        if (!scanf("%d", &c1) || c1 > vec.size()||c1<1) {
            fflush(stdin);
            cout << "| "
                 << "Enter the correct number." << endl;
            continue;
        }
        break;
    }
    if (flag) {
        while (true) {
            cout << "| "
                 << "Enter the number to select another polynomial: ";
            if (!scanf("%d", &c2) || c2 > vec.size()||c2<1) {
                fflush(stdin);
                cout << "| "
                     << "Enter the correct number." << endl;
                continue;
            }
            break;
        }
    }
    return std::make_pair(c1 - 1, c2 - 1);
}

bool calculatorUI::record() {
    if (exist()) return false;
    cout
        << "|" << endl
        << "| "
        << "Press 'Y' or 'y' to save the result, or press any other key to end."
        << endl
        << "| ";
    string res;
    cin >> res;
    return res == "Y" || res == "y";
}

bool calculatorUI::exist() {
    //    determine whether the polynomial is already in the list set
    for (auto it = vec.begin(); it != vec.end() - 1; ++it) {
        if (*it == vec.back()) return true;
    }
    return false;
}

void calculatorUI::creat(bool flag) {
    cout << endl << "FUNCTION - CREATION" << endl;
    storage p;
    p.creatPolynomial();
    if (p.getLength()) vec.push_back(storage(p));
    if (exist()) {
        cout << endl
             << "| "
             << "The polynomial is already in the list.";
        vec.pop_back();
    }
    cout << endl << "END OF CREATION" << endl;
}

void calculatorUI::sum() {
    cout << endl << "FUNCTION - SUMMATION" << endl;
    cout << "| "
         << "CHOOSE 2 POLYNOMIALS" << endl
         << "|";
    auto idx = choose();
    vec.push_back(storage(vec[idx.first] + vec[idx.second]));
    vec.back().setLength();
    cout << "|" << endl
         << "| "
         << "The sum of " << vec[idx.first] << " and " << vec[idx.second]
         << " is:" << endl
         << "| " << vec.back() << endl;
    if (!record()) {
        vec.pop_back();
    } else {
        cout << "| "
             << "Result is successfully saved." << endl;
    }
    cout << "END OF SUMMATION" << endl;
}

void calculatorUI::sub() {
    cout << endl << "FUNCTION - SUBTRACTION" << endl;
    cout << "| "
         << "CHOOSE 2 POLYNOMIALS" << endl
         << "|";
    auto idx = choose();
    vec.push_back(storage(vec[idx.first] - vec[idx.second]));
    vec.back().setLength();
    cout << "|" << endl
         << "| "
         << "The difference of " << vec[idx.first] << " and " << vec[idx.second]
         << " is:" << endl
         << "| " << vec.back() << endl;
    if (!record()) {
        vec.pop_back();
    } else {
        cout << "| "
             << "Result is successfully saved." << endl;
    }
    cout << "END OF SUBTRACTION" << endl;
}

void calculatorUI::mul() {
    cout << endl << "FUNCTION - MULTIPLYCATION" << endl;
    cout << "| "
         << "CHOOSE 2 POLYNOMIALS" << endl
         << "|";
    auto idx = choose();
    vec.push_back(storage(vec[idx.first] * vec[idx.second]));
    vec.back().setLength();
    cout << "|" << endl
         << "| "
         << "The product of " << vec[idx.first] << " and " << vec[idx.second]
         << " is:" << endl
         << "| " << vec.back() << endl;
    if (!record()) {
        vec.pop_back();
    } else {
        cout << "| "
             << "Result is successfully saved." << endl;
    }
    cout << "END OF MULTIPLYCATION" << endl;
}

void calculatorUI::der() {
    cout << endl << "FUNCTION - DERIVATION" << endl;
    cout << "| "
         << "CHOOSE 1 POLYNOMIAL" << endl
         << "|";
    auto idx = choose(false);
    vec.push_back(storage(vec[idx.first].derivative()));
    cout << "| "
         << "The derivative of "<<vec[idx.first]<<"is: " <<endl<<"| "<< vec.back() << endl;
    if (!record()) {
        vec.pop_back();
    } else {
        cout << "| "
             << "Result is successfully saved." << endl;
    }
    cout << "END OF DERIVATION" << endl;
}

void calculatorUI::val() {
    cout << endl << "FUNCTION - EVALUATION" << endl;
    cout << "| "
         << "CHOOSE 1 POLYNOMIAL" << endl
         << "|";
    auto idx = choose(false);
    double x = 0;
    cout << "| "
         << "Enter the 'x': ";
    scanf("%lf", &x);
    cout << "| "
         << "When x = " << x << ", the value of " << vec[idx.first]
         << "is: " << vec[idx.first].evaluation(x) << endl;
    cout << "END OF EVALUATION" << endl;
}

void calculatorUI::remove() {
    cout << endl << "FUNCTION - DELETION" << endl;
    cout << "| "
         << "CHOOSE 1 POLYNOMIAL" << endl
         << "|";
    auto idx = choose(false);
    storage temp = vec[idx.first];
    auto it = vec.begin();
    for (int i = 0; i < idx.first; ++i) {
        ++it;
    }
    vec.erase(it);
    cout << "| "
         << "The polynomial " << temp << " have been deleted.\n";
    cout << "END OF DELETION" << endl;
}
