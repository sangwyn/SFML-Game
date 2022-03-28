#include <SFML/Graphics.hpp>
#include <cmath>
#include "MathFunctions.h"

float Dist(sf::Vector2f v1, sf::Vector2f v2) {
    return sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
}

int StrToInt(std::string str) {
    int res = 0;
    int pcf_govno = 1;
    for (int i = str.size() - 1; i >= 0; --i) {
        res += (str[i] - '0') * pcf_govno;
        pcf_govno *= 10;
    }
    return res;
}