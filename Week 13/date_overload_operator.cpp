#include <iostream>

using namespace std;

class Date {
    public:
        Date(int d, int m, int y) : day(d), month(m), year(y) {}
        Date &operator++(); // prefix increment operator
        Date operator++(int); // postfix increment operator

    private:
        int day;
        int month;
        int year;

        void helpIncrement() {
            // Increment the date by one day (simplified)
            day++;
            if (day > 30) { // Assume all months have 30 days for simplicity
                day = 1;
                month++;
                if (month > 12) {
                    month = 1;
                    year++;
                }
            }
        }
};

// overloaded prefix increment operator
Date &Date::operator++()
{
    helpIncrement(); // increment date
    return *this; // reference return to create an Ivalue
} // end function operator++

// overloaded postfix increment operator; note that the
// dummy integer parameter does not have a parameter name
Date Date::operator++(int)
{
    Date temp = *this; // hold current state of object
    helpIncrement();
    // return unincremented, saved, temporary object
    return temp; // value return; not a reference return
} // end function operator++

int main(void) {
    return 0;
}