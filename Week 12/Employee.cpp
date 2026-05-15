// Fig. 12.10: Employee.cpp
// Employee class member-function definitions.
#include <iostream>
#include <stdexcept>
#include "Employee.h"

using namespace std;

// constructor
Employee::Employee(const string &first, const string &last, const string &ssn)
{
    setFirstName(first);
    setLastName(last);
    setSocialSecurityNumber(ssn);
}

// set first name
void Employee::setFirstName(const string &first)
{
    firstName = first;
}

// return first name
string Employee::getFirstName() const
{
    return firstName;
}

// set last name
void Employee::setLastName(const string &last)
{
    lastName = last;
}

// return last name
string Employee::getLastName() const
{
    return lastName;
}

// set social security number
void Employee::setSocialSecurityNumber(const string &ssn)
{
    socialSecurityNumber = ssn;
}

// return social security number
string Employee::getSocialSecurityNumber() const
{
    return socialSecurityNumber;
}

// print Employee's information
void Employee::print() const
{
    cout << getFirstName() << ' ' << getLastName()
         << "\nsocial security number: " << getSocialSecurityNumber();
}
