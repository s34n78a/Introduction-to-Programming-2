#include <iostream>
#include <string>
#include "function_vending.h"

VendingMachine::VendingMachine() {
    data = new int[100];
    capacity = 100;   
    size = 0;

    totalRevenue = 0; 
    totalSold = 0; 
}

void VendingMachine::store(int price) {
    if (size == capacity)
    {
        int* bigger_data = new int[capacity*2];
        for (long long i = 0; i < size; i++)
        {
            bigger_data[i] = data[i];
        }
        delete[] data;
        data = bigger_data;
        capacity *= 2;
    }
    
    data[size] = price;
    long long index = size;
    size++;

    long long temp = 0;
    while (index > 0 && data[index] < data[(index-1)/2])
    {
        long long temp = data[(index-1)/2];
        data[(index-1)/2] = data[index];
        data[index] = temp;
        index = (index-1)/2;
    }
} 

void VendingMachine::sell() {
    if (size == 0)
    {
        return;
    }

    totalRevenue += data[0];
    totalSold++;
    
    size--;
    data[0] = data[size];
    data[size] = 0; // Optional: Clear the last element

    long long index = 0;
    while (true)
    {
        long long left_child = index*2 + 1;
        long long right_child = index*2 + 2;
        long long min = index;

        if (left_child < size && data[left_child] < data[min])
        {
            min = left_child;
        }
        if (right_child < size && data[right_child] < data[min])
        {
            min = right_child;
        }
        
        if (min == index) //already at the right place
        {
            break;
        }
        else
        {
            long long temp = data[index];
            data[index] = data[min];
            data[min] = temp;

            index = min;
        }
        
    }
}

void VendingMachine::printResult() {
    std::cout << totalSold << " " << totalRevenue << std::endl;
}

VendingMachine::~VendingMachine() {
    delete[] data;
}