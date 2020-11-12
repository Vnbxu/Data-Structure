//
//  storage.hpp
//  polynormial_calculator
//
//  Created by VNB on 2020/9/24.
//  Copyright © 2020 VNB. All rights reserved.
//

#ifndef storage_hpp
#define storage_hpp

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

struct term {
    double coef;
    int expn;
    term* next;
    term(double c = 0.0, int e = 0, term* n = nullptr)
        : coef(c), expn(e), next(n) {}
};

class storage {
    term* head;
    term* tail;
    int length;
    string funcName;

   public:
    storage();
    storage(const storage& s);
    ~storage();

    void creatPolynomial();
    void clear();

    void appendTerm(term* t, int idx = 0);
    void eraseTerm(int idx);

    void sortPolynomial();
    void simplify();

    void setLength();
    int getLength();
    string getName();
    void printPolynomial();

    term* add(term* ha, term* hb);
    storage operator+(const storage& b);
    storage operator-() const;
    storage operator-(const storage& b);
    storage operator*(const storage& b);
    storage operator=(const storage& b);
    bool operator==(const storage& b);
    storage derivative();
    double evaluation(const double num);

    friend std::istream& operator>>(std::istream& is, storage& p);
    friend std::ostream& operator<<(std::ostream& os, storage& p);
};

#endif /* storage_hpp */
