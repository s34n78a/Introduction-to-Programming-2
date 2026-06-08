#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>
#include "function_rect.h"

Rectangle::Rectangle() {
    name = "";
    left = top = right = bottom = 0;
    isValid = false;
}

Rectangle::Rectangle(std::string n, int _x1, int _y1, int _x2, int _y2) {
    name = n;

    if (_x1 == _x2 || _y1 == _y2)
    {
        left = right = top = bottom = 0;
        isValid = false;
    }
    else
    {
        left = std::min(_x1, _x2);
        right = std::max(_x1, _x2);
        top = std::min(_y1, _y2);
        bottom = std::max(_y1, _y2);
        isValid = true;
    }
}

Rectangle Rectangle::operator&(const Rectangle& other) const {
    if (!isValid || !other.isValid) {
        return Rectangle(); // Return an invalid rectangle if either is invalid.
    }
    int newLeft = std::max(left, other.left);
    int newRight = std::min(right, other.right);
    int newTop = std::max(top, other.top);
    int newBottom = std::min(bottom, other.bottom);
    return Rectangle(name + "&" + other.name, newLeft, newTop, newRight, newBottom);
}

Rectangle Rectangle::operator|(const Rectangle& other) const {
    if (!isValid || !other.isValid) {
        return Rectangle(); // Return an invalid rectangle if either is invalid.
    }
    int newLeft = std::min(left, other.left);
    int newRight = std::max(right, other.right);
    int newTop = std::min(top, other.top);
    int newBottom = std::max(bottom, other.bottom);
    return Rectangle(name + "|" + other.name, newLeft, newTop, newRight, newBottom);
}

Rectangle& Rectangle::operator=(const Rectangle& other) {
    if (this != &other) {
        name = other.name;
        left = other.left;
        top = other.top;
        right = other.right;
        bottom = other.bottom;
        isValid = other.isValid;
    }
    return *this;
}

long long Rectangle::getArea() const {
    if (!isValid) return 0;
    return (long long)(right - left) * (bottom - top);
}