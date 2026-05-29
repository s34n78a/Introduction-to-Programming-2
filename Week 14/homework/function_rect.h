#pragma once
#include <string>

class Rectangle {
private:
    std::string name;
    int left, top, right, bottom; // We store the rectangle using 4 edges instead of 2 points.
    bool isValid; // A flag to determine if the rectangle is valid (area > 0).

public:
    Rectangle();

    Rectangle(std::string n, int _x1, int _y1, int _x2, int _y2);

    Rectangle operator&(const Rectangle& other) const;

    Rectangle operator|(const Rectangle& other) const;

    Rectangle& operator=(const Rectangle& other);

    long long getArea() const;
};