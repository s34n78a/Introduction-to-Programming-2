#include <iostream>
#include <string>
#include "function.h"

int main() {
    VendingMachine vm;
    std::string cmd;

    while (std::cin >> cmd) {
        if (cmd == "store") {
            int price;
            std::cin >> price;
            vm.store(price);
        }
        else if (cmd == "sell") {
            vm.sell();
        }
    }

    vm.printResult();
    return 0;
}