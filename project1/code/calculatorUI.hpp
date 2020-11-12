//
//  calculatorUI.hpp
//  polynormial_calculator
//
//  Created by VNB on 2020/9/22.
//  Copyright © 2020 VNB. All rights reserved.
//

#ifndef calculatorUI_hpp
#define calculatorUI_hpp

#include <iostream>
#include <string>
#include <vector>

#include "storage.hpp"

class calculatorUI {
   public:
    calculatorUI(){};
    void OperationLoop(void);

   private:
    bool executeOperation(const std::string &t_operation);
    void printMenu();
    void printPolynomial();
    void listPolynomial();

    std::string getOperation();
    std::pair<int, int> choose(bool flag = true);
    bool record();
    bool exist();

    void creat(bool flag = true);
    void sum();
    void sub();
    void mul();
    void der();
    void val();
    void remove();

    vector<storage> vec;
};

#endif /* calculatorUI_hpp */
