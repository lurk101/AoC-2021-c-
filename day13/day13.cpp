// day13.cpp
#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include <string>
using namespace std;
using namespace std::chrono;

typedef struct {
    unsigned y, x;
} dot_t;

bool operator<(const dot_t& a, const dot_t& b) {
    if (a.y < b.y)
        return true;
    if (a.y > b.y)
        return false;
    return a.x < b.x;
};

set<dot_t> paper;
ifstream f;

void do_x_fold(unsigned x) {
    list<dot_t> found;
    for (auto& dot : paper)
        if (dot.x > x)
            found.push_back(dot);
    for (auto& dot : found) {
        paper.erase(dot);
        dot.x = x - (dot.x - x);
        if (paper.find(dot) == paper.end())
            paper.insert(dot);
    }
}

void do_y_fold(unsigned y) {
    list<dot_t> found;
    for (auto& dot : paper)
        if (dot.y > y)
            found.push_back(dot);
    for (auto& dot : found) {
        paper.erase(dot);
        dot.y = y - (dot.y - y);
        if (paper.find(dot) == paper.end())
            paper.insert(dot);
    }
}

int main() {
    auto start = high_resolution_clock::now();
    f.open("day13.txt");
    string s;
    for (;;) {
        getline(f, s);
        if (s.length() == 0)
            break;
        stringstream ss(s);
        dot_t dot;
        char c;
        ss >> dot.x >> c >> dot.y;
        paper.insert(dot);
    }
    unsigned row_col;
    bool is_x;
    bool first = true;
    for (;;) {
        getline(f, s);
        if (s.length() == 0)
            break;
        s = s.substr(11);
        stringstream ss(s);
        char c;
        ss >> c;
        is_x = c == 'x';
        ss >> c >> row_col;
        if (is_x)
            do_x_fold(row_col);
        else
            do_y_fold(row_col);
        if (first) {
            first = false;
            cout << "Part 1 - " << paper.size() << endl << "Part 2" << endl;
        }
    }
    unsigned max_x = 0, max_y = 0;
    for (auto& dot : paper) {
        if (dot.x > max_x)
            max_x = dot.x;
        if (dot.y > max_y)
            max_y = dot.y;
    }
    for (unsigned y = 0; y <= max_y; y++) {
        for (unsigned x = 0; x <= max_x; x++) {
            dot_t dot = {y, x};
            cout << ((paper.find(dot) == paper.end()) ? ' ' : '#');
        }
        cout << endl;
    }
    cout << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
