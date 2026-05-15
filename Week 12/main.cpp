#include <iostream>
#include <string>
#include "Employee.h"
#include "SalariedEmployee.h"
#include "CommissionEmployee.h"
#include "BasePlusCommissionEmployee.h"

using namespace std;

int main() {
    // create derived-class objects
    SalariedEmployee salariedEmployee("John", "Smith", "111-11-1111", 800);
    CommissionEmployee commissionEmployee("Sue", "Jones", "333-33-3333", 10000, .06);
    BasePlusCommissionEmployee basePlusCommissionEmployee("Bob", "Lewis", "444-44-4444", 5000, .04, 300);

    // create vector of three base-class pointers
    Employee *employees[3] = { &salariedEmployee, &commissionEmployee, &basePlusCommissionEmployee };

    cout << "Employees processed polymorphically via dynamic binding:\n\n";

    // call virtual functions print and earnings for each Employee
    for (size_t i = 0; i < 3; ++i) {
        employees[i]->print();
        cout << "\nearned $" << employees[i]->earnings() << "\n\n";
    }

    return 0;
}