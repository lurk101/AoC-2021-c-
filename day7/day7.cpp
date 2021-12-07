// day7.cpp
#include <fstream>
#include <iostream>
#include <list>
using namespace std;

list<unsigned> x;

unsigned fuel_cost(unsigned n, bool part1) {
    if (part1 || (n < 2))
        return n;
    return (n * (1 + n)) / 2;
}

unsigned part(bool part1) {
    unsigned min = 999999, max = 0;
    for (auto n : x) {
        if (n > max)
            max = n;
        if (n < min)
            min = n;
    }
    unsigned min_fuel = 4294967295;
    for (unsigned n = min; n <= max; n++) {
        unsigned fuel = 0;
        for (auto& xn : x)
            fuel += fuel_cost(xn > n ? xn - n : n - xn, part1);
        if (fuel < min_fuel)
            min_fuel = fuel;
    }
    return min_fuel;
}

int main() {
    ifstream file("day7.txt");
    char c;
    unsigned n;
    while (file >> n) {
        x.push_back(n);
        file >> c;
    }
    cout << "Part 1 - " << part(true) << endl;
    cout << "Part 2 - " << part(false) << endl;
}
