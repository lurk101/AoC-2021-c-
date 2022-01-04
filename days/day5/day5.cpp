#define title "--- Day 5: Hydrothermal Venture ---"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <sstream>
using namespace std;
using namespace chrono;

struct vents {

    void addy(unsigned x, unsigned y1, unsigned y2) {
        if (y1 > y2)
            swap(y2, y1);
        for (; y1 <= y2; y1++)
            add(x, y1);
    }

    void addx(unsigned y, unsigned x1, unsigned x2) {
        if (x1 > x2)
            swap(x2, x1);
        for (; x1 <= x2; x1++)
            add(x1, y);
    }

    void addxy(unsigned x1, unsigned x2, unsigned y1, unsigned y2) {
        if (x1 > x2) {
            swap(y2, y1);
            swap(x2, x1);
        }
        int y_incr = 1;
        if (y1 > y2)
            y_incr = -1;
        for (; x1 <= x2; x1++, y1 += y_incr)
            add(x1, y1);
    }

    unsigned count(void) {
        unsigned n = 0;
        for (unsigned y = 0; y < 1000; y++)
            for (unsigned x = 0; x < 1000; x++)
                if (get(x, y) > 1)
                    n++;
        return n;
    }

  private:
    void add(unsigned x, unsigned y) { set(x, y, get(x, y) + 1); }

    unsigned get(unsigned x, unsigned y) { return v[y * 1000 + x]; }

    void set(unsigned x, unsigned y, unsigned n) { v[y * 1000 + x] = n; }

    unsigned v[1000 * 1000];
};

vents v;

const char* lines[] = {
#include "day5.txt"
};

unsigned part(int part_n) {
    for (unsigned i = 0; i < sizeof(lines) / sizeof(lines[0]); i++) {
        unsigned x1, y1, x2, y2;
        char c;
        stringstream ss(lines[i]);
        ss >> x1 >> c >> y1 >> c >> c >> x2 >> c >> y2;
        if (part_n == 2) {
            if ((x1 != x2) && (y1 != y2))
                v.addxy(x1, x2, y1, y2);
        } else {
            if (x1 == x2)
                v.addy(x1, y1, y2);
            else if (y1 == y2)
                v.addx(y1, x1, x2);
        }
    }
    return v.count();
}

int main() {
    auto start = high_resolution_clock::now();
    cout << title << endl
         << "Part 1 - " << part(1) << endl
         << "Part 2 - " << part(2) << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
