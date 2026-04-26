#pragma once
class VendingMachine {
private:
    int* data;      
    int capacity;   
    int size;       // current amount

    long long totalRevenue; 
    int totalSold;          

public: //these functions need to be implemented by you.
    VendingMachine(); 

    ~VendingMachine();

    void store(int price); 

    void sell();

    void printResult();
};
