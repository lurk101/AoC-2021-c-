// day5.cpp
#include <fstream>
#include <iostream>
#include <map>
using namespace std;

typedef struct {
    short x, y;
} key;

bool operator<(key const& a, key const& b) {
    if (a.x < b.x)
        return true;
    if (a.x > b.x)
        return false;
    return a.y < b.y;
}

map<key, short> vents;

void add(short x, short y) {
    key k = {x, y};
    auto v = vents.find(k);
    if (v == end(vents))
        vents.insert(pair<key, short>(k, 1));
    else
        v->second++;
}

void addy(short x, short y1, short y2) {
    if (y1 > y2)
        swap(y1, y2);
    for (; y1 <= y2; y1++)
        add(x, y1);
}

void addx(short y, short x1, short x2) {
    if (x1 > x2)
        swap(x1, x2);
    for (; x1 <= x2; x1++)
        add(x1, y);
}

void addxy(short x1, short x2, short y1, short y2) {
    if (x1 > x2) {
        swap(x1, x2);
        swap(y1, y2);
    }
    short y_incr = 1;
    if (y1 > y2)
        y_incr = -1;
    for (; x1 <= x2; x1++, y1 += y_incr)
        add(x1, y1);
}

ifstream file;

short part(bool part1) {
    file.clear();
    file.seekg(0);
    char c;
    short x1, y1, x2, y2;
    while (file >> x1) {
        file >> c >> y1 >> c >> c >> x2 >> c >> y2;
        if (part1) {
            if (x1 == x2)
                addy(x1, y1, y2);
            else if (y1 == y2)
                addx(y1, x1, x2);
        } else {
            if ((x1 != x2) && (y1 != y2) && (abs(x2 - x1) == abs(y2 - y1)))
                addxy(x1, x2, y1, y2);
        }
    }
    short count = 0;
    for (auto& v : vents)
        if (v.second > 1)
            count++;
    return count;
}

int main() {
    file.open("day5.txt");
    cout << "Part 1 - " << part(true) << endl;
    cout << "Part 2 - " << part(false) << endl;
}
