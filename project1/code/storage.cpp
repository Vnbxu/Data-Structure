//
//  storage.cpp
//  polynormial_calculator
//
//  Created by VNB on 2020/9/24.
//  Copyright © 2020 VNB. All rights reserved.
//

#include "storage.hpp"

storage::storage() : head(nullptr), tail(nullptr), length(0), funcName("f") {}

storage::storage(const storage& s)
    : head(nullptr), tail(nullptr), funcName(s.funcName) {
    term* cur = s.head;
    for (int i = 0; i < s.length; ++i) {
        term* t = new term(cur->coef, cur->expn);
        appendTerm(t, i);
        if (this->head == nullptr) this->head = t;
        cur = cur->next;
    }
    this->setLength();
}
storage::~storage() { clear(); }

void storage::creatPolynomial() { cin >> *this; }

void storage::clear() {
    while (head && length) {
        term* temp = head;
        head = head->next;
        if (temp) delete temp;
        --length;
    }
    length = 0;
    head = tail = nullptr;
}

void storage::appendTerm(term* t, int idx) {
    if (idx == 0) {
        t->next = head;
        if (tail == nullptr) tail = t;
        head = t;
        if (length == 0) tail = head;
        ++length;
    } else {
        term* cur = head;
        while (--idx) {
            cur = cur->next;
        }
        t->next = cur->next;
        cur->next = t;
        if (t->next == nullptr) tail = t;
        ++length;
    }
}

void storage::eraseTerm(int idx) {
    if (idx == 0) {
        term* temp = head;
        head = head->next;
        delete temp;
    } else {
        term* cur = head;
        for (int i = 1; i < idx; ++i) {
            cur = cur->next;
        }
        term* temp = cur->next;
        cur->next = temp->next;
        delete temp;
    }
    --length;
    if (length == 0) tail = nullptr;
}

// sort polynomial's exponent from large to small
term* findMiddle(term* head) {
    term *slow = head, *fast = head, *pre = nullptr;
    while (fast && fast->next) {
        pre = slow;
        slow = slow->next;
        fast = fast->next->next;
    }
    pre->next = nullptr;
    return slow;
}
term* mergeSort(term* head) {
    if (head == nullptr || head->next == nullptr) return head;
    term* mid = findMiddle(head);
    term* l1 = mergeSort(head);
    term* l2 = mergeSort(mid);
    term* dummy = new term();
    term* cur = dummy;
    while (l1 && l2) {
        if (l1->expn < l2->expn) {
            cur->next = l2;
            l2 = l2->next;
        } else {
            cur->next = l1;
            l1 = l1->next;
        }
        cur = cur->next;
        cur->next = nullptr;
    }
    cur->next = (l1 == nullptr ? l2 : l1);
    term* newHead = dummy->next;
    delete dummy;
    return newHead;
}
void storage::sortPolynomial() {
    if (head && head->next) head = mergeSort(head);
}

void storage::simplify() {
    if (this->length <= 1) return;
    term* pre = nullptr;
    term* cur = this->head;
    int idx = 0;
    while (cur) {
        if (cur->coef == 0 && this->length > 1) {
            pre = cur;
            cur = cur->next;
            eraseTerm(idx);
            if (!cur) break;
        }
        if (pre && cur->expn == pre->expn) {
            pre->coef += cur->coef;
            cur = cur->next;
            eraseTerm(idx);
        } else {
            pre = cur;
            cur = cur->next;
            ++idx;
        }
    }
}

void storage::setLength() {
    term* cur = this->head;
    this->length = 0;
    while (cur) {
        ++this->length;
        if (cur->next == nullptr) this->tail = cur;
        cur = cur->next;
    }
}

int storage::getLength() { return length; }

string storage::getName() { return funcName; }

void storage::printPolynomial() {
    vector<int> e;
    vector<double> c;
    term* cur = this->head;
    while (cur) {
        e.push_back(cur->expn);
        c.push_back(cur->coef);
        cur = cur->next;
    }
    //    display the polynormial in format
    cout << "| "
         << "Polynormial " << this->funcName << ": " << this->length << " ";
    for (int i = 0; i < this->length; ++i) {
        cout << c[i] << " " << e[i] << " ";
    }
}

term* storage::add(term* ha, term* hb) {
    term* dummy = new term();
    term* cur = dummy;
    int l = 0;
    while (ha && hb) {
        int haExpn = ha->expn, hbExpn = hb->expn;
        if (haExpn == hbExpn) {
            double c = ha->coef + hb->coef;
            term* t = new term(c, haExpn);
            cur->next = t;
            cur = cur->next;
            ++l;
        } else {
            term* h = (haExpn > hbExpn ? ha : hb);
            if (h->coef != 0) {
                term* t = new term(h->coef, h->expn);
                cur->next = t;
                cur = cur->next;
                ++l;
            }
        }
        if (haExpn >= hbExpn) ha = ha->next;
        if (haExpn <= hbExpn) hb = hb->next;
    }
    term* h = (ha == nullptr ? hb : ha);
    while (h) {
        term* t = new term(h->coef, h->expn);
        cur->next = t;
        cur = cur->next;
        ++l;
        h = h->next;
    }
    term* head = dummy->next;
    delete dummy;
    return head;
}

// add up two polynormials
storage storage::operator+(const storage& b) {
    term* newHead = add(this->head, b.head);
    storage p;
    p.head = newHead;
    p.setLength();
    p.simplify();
    return p;
}

// reverse each terms of polynormial
storage storage::operator-() const {
    storage p;
    term* cur = this->head;
    while (cur) {
        double c = cur->coef * (-1);
        int e = cur->expn;
        p.appendTerm(new term(c, e));
        cur = cur->next;
    }
    p.sortPolynomial();
    return p;
}
// subtract two polynormials
storage storage::operator-(const storage& b) { return *this + (-b); }

// multiply two polynormials
storage storage::operator*(const storage& b) {
    storage p;
    term* ans = new term();
    term* curA = this->head;
    while (curA) {
        term* t = new term();
        term* cur = t;
        term* curB = b.head;
        // multiply one term of polynomial A by polynomial B
        while (curB) {
            int e = curA->expn + curB->expn;
            double c = curA->coef * curB->coef;
            if (c != 0) {
                term* p = new term(c, e);
                cur->next = p;
                cur = cur->next;
            }
            curB = curB->next;
        }
        ans->next = add(ans->next, t->next);
        delete t;
        curA = curA->next;
    }
    p.head = ans->next;
    if (p.head == nullptr) {
        p.head = new term();
    }
    p.setLength();
    delete ans;
    return p;
}

// copy polynomial
storage storage::operator=(const storage& b) {
    this->clear();
    term* dummy = new term();
    term* curA = dummy;
    term* curB = b.head;
    while (curB) {
        term* t = new term(curB->coef, curB->expn);
        curA->next = t;
        curA = curA->next;
        curB = curB->next;
        ++this->length;
    }
    this->head = dummy->next;
    this->setLength();
    delete dummy;
    return *this;
}

// determine whether the two polynomials are equal
bool storage::operator==(const storage& b) {
    if (this->length != b.length) return false;
    term* curA = this->head;
    term* curB = b.head;
    while (curA && curB) {
        if (curA->coef != curB->coef)
            return false;
        else if (curA->expn != curB->expn)
            return false;
        curA = curA->next;
        curB = curB->next;
    }
    if (!curA && !curB)
        return true;
    else
        return false;
}

// calculate the derivative of polynomial
storage storage::derivative() {
    storage p;
    term* cur = this->head;
    term* dummy = new term();
    term* c = dummy;
    int idx = 0;
    while (cur) {
        term* t = new term();
        if (!(cur->expn == 0 && this->length > 1)) {
            t->coef = cur->coef * cur->expn;
            t->expn = cur->expn - 1;
        }
        c->next = t;
        c = c->next;
        cur = cur->next;
        ++idx;
    }
    p.head = dummy->next;
    p.setLength();
    p.sortPolynomial();
    p.simplify();
    delete dummy;
    return p;
}

double storage::evaluation(const double num) {
    double sum = 0.0;
    term* cur = this->head;
    while (cur) {
        double e = cur->expn;
        double n = num;
        if (e < 0) n = 1 / n;
        e = fabs(e);
        sum += (pow(n, e) * cur->coef);
        cur = cur->next;
    }
    return sum;
}

std::istream& operator>>(std::istream& is, storage& p) {
    int termNum;
    p.funcName = "f";
    cout << "| "
         << "Enter the number of polynormial term(s): ";
    is >> termNum;
    if (termNum == 0) return is;
    for (int i = 1; i <= termNum; ++i) {
        double coef;
        int expn = 0;
        cout << "| "
             << "Enter " << i << " term's coefficient: ";
        scanf("%lf", &coef);
        if (coef) {
            cout << "| "
                 << "Enter " << i << " term's exponent: ";
            is >> expn;
        }
        term* newTerm = new term(coef, expn);
        p.appendTerm(newTerm);
    }
    p.sortPolynomial();
    p.simplify();
    cout << "| " << endl
         << "| "
         << "The polynormial entered is: " << p;
    return is;
}

std::ostream& operator<<(std::ostream& os, storage& p) {
    vector<int> e;
    vector<double> c;
    term* cur = p.head;
    while (cur) {
        e.push_back(cur->expn);
        c.push_back(cur->coef);
        cur = cur->next;
    }
    // display the polynormial in form of f(x)
    os << p.funcName << "(x) = ";
    for (int i = 0; i < p.length; ++i) {
        if (c[i] != 0) {
            if (i > 0 && c[i] > 0) os << "+";
            if (c[i] == -1 && e[i] != 0) {
                os << "-";
            } else if (c[i] != 1 || (e[i] == 0 && c[i] != 0)) {
                os << c[i];
            }
            if (e[i] != 0) os << "x";
            if (e[i] != 1) {
                if (e[i] > 0) {
                    os << "^" << e[i];
                } else if (e[i] < 0) {
                    os << "^(" << e[i] << ")";
                }
            }
            os << " ";
        } else if (p.length == 1 && c[i] == 0) {
            os << "0";
        }
    }
    return os;
}
