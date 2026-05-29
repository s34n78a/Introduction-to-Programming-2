#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>
#include "function_rect.h"



int main() {
    int N;
    if (!(std::cin >> N)) return 0;

    std::map<std::string, Rectangle> rectDb;

    
    for (int i = 0; i < N; ++i) {
        std::string name;
        int x1, y1, x2, y2;
        std::cin >> name >> x1 >> y1 >> x2 >> y2;
        rectDb[name] = Rectangle(name, x1, y1, x2, y2);
    }

    
    std::string line;
    std::getline(std::cin >> std::ws, line); 
    std::stringstream ss(line);

    std::string rectName;
    if (!(ss >> rectName)) return 0; 

    Rectangle result = rectDb[rectName];

    std::string op;
    std::string nextName;

    
    while (ss >> op >> nextName) {
        if (op == "&") {
            result = result & rectDb[nextName];
        }
        else if (op == "|") {
            result = result | rectDb[nextName];
        }
    }

    
    std::cout << result.getArea() << std::endl;

    return 0;
}