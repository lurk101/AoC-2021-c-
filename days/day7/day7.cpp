#define title "--- Day 7: The Treachery of Whales ---"
#include <bits/stdc++.h>
#include <chrono>
#include <iostream>
#include <list>
#include <sstream>
using namespace std;
using namespace chrono;

list<int> crabs;
int part_n;

int inline fuel_cost(int n) {
    if ((part_n == 1) || (n < 2))
        return n;
    return n * (n + 1) / 2;
}

int part(int p_n) {
    part_n = p_n;
    int min_x = INT_MAX, max_x = INT_MIN;
    for (auto x : crabs) {
        if (x > max_x)
            max_x = x;
        if (x < min_x)
            min_x = x;
    }
    int min_fuel = INT_MAX;
    for (int x = min_x; x <= max_x; x++) {
        int fuel = 0;
        for (auto x2 : crabs)
            fuel += fuel_cost(abs(x2 - x));
        if (fuel < min_fuel)
            min_fuel = fuel;
    }
    return min_fuel;
}

const char* line =
#include "day7.txt"
    ;

int main() {
    auto start = high_resolution_clock::now();
    char c;
    int n;
    stringstream ss(line);
    while (ss >> n) {
        crabs.push_back(n);
        ss >> c;
    }
    cout << title << endl
         << "Part 1 - " << part(1) << endl
         << "Part 2 - " << part(2) << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
