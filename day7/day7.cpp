// day7.cpp
#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <list>
using namespace std;

list<int> crabs;

int inline fuel_cost(int n, int part) {
    if ((part == 1) || (n < 2))
        return n;
    return n * (n + 1) / 2;
}

int solve(int part) {
    int min_x = INT_MAX, max_x = INT_MIN;
    for (auto cx : crabs) {
        if (cx > max_x)
            max_x = cx;
        if (cx < min_x)
            min_x = cx;
    }
    int min_fuel = INT_MAX;
    for (int x = min_x; x <= max_x; x++) {
        int fuel = 0;
        for (auto cx : crabs)
            fuel += fuel_cost(abs(cx - x), part);
        if (fuel < min_fuel)
            min_fuel = fuel;
    }
    return min_fuel;
}

int main() {
    ifstream file("day7.txt");
    char c;
    int n;
    while (file >> n) {
        crabs.push_back(n);
        file >> c;
    }
    cout << "Part 1 - " << solve(1) << endl;
    cout << "Part 2 - " << solve(2) << endl;
}
