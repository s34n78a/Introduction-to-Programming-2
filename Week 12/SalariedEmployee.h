// Fig. 12.11: SalariedEmployee.h
// SalariedEmployee class derived from Employee.

#ifndef SALARIED_H
#define SALARIED_H

#include <string> 
#include "Employee.h" 

class SalariedEmployee : public Employee {
public:
    // Constructor
    SalariedEmployee(const std::string &, const std::string &, 
                     const std::string &, double = 0.0);
    
    virtual ~SalariedEmployee() { } // Virtual destructor

    void setWeeklySalary(double); // Set weekly salary
    double getWeeklySalary() const; // Get weekly salary
    
    // key word override signals intent to override a base class virtual function
    virtual double earnings() const override; 
    virtual void print() const override; 
private:
    double weeklySalary; // Weekly salary
}; // end class SalariedEmployee

#endif // SALARIED_H
