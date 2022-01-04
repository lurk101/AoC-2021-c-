#define title "--- Day 19: Beacon Scanner ---"
#include <array>
#include <chrono>
#include <deque>
#include <functional>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;
using namespace chrono;

typedef array<pair<int, int>, 3> rotation_t;
const int X = 0, Y = 1, Z = 2, POS = 1, NEG = -1;
const rotation_t rotations[24] = {
    {{{X, POS}, {Y, POS}, {Z, POS}}}, {{{X, POS}, {Z, POS}, {Y, NEG}}},
    {{{X, POS}, {Y, NEG}, {Z, NEG}}}, {{{X, POS}, {Z, NEG}, {Y, POS}}},
    {{{X, NEG}, {Y, POS}, {Z, NEG}}}, {{{X, NEG}, {Z, NEG}, {Y, NEG}}},
    {{{X, NEG}, {Y, NEG}, {Z, POS}}}, {{{X, NEG}, {Z, POS}, {Y, POS}}},
    {{{Y, POS}, {X, POS}, {Z, NEG}}}, {{{Y, POS}, {Z, NEG}, {X, NEG}}},
    {{{Y, POS}, {X, NEG}, {Z, POS}}}, {{{Y, POS}, {Z, POS}, {X, POS}}},
    {{{Y, NEG}, {X, POS}, {Z, POS}}}, {{{Y, NEG}, {Z, POS}, {X, NEG}}},
    {{{Y, NEG}, {X, NEG}, {Z, NEG}}}, {{{Y, NEG}, {Z, NEG}, {X, POS}}},
    {{{Z, POS}, {X, POS}, {Y, POS}}}, {{{Z, POS}, {Y, POS}, {X, NEG}}},
    {{{Z, POS}, {X, NEG}, {Y, NEG}}}, {{{Z, POS}, {Y, NEG}, {X, POS}}},
    {{{Z, NEG}, {X, POS}, {Y, NEG}}}, {{{Z, NEG}, {Y, NEG}, {X, NEG}}},
    {{{Z, NEG}, {X, NEG}, {Y, POS}}}, {{{Z, NEG}, {Y, POS}, {X, POS}}}};

struct point_s {
    int64_t x, y, z;
    int64_t manhattan() { return abs(x) + abs(y) + abs(z); }
    point_s rotate(const rotation_t& rot) const {
        const int64_t* vals[3] = {&x, &y, &z};
        return {(*vals[rot[0].first]) * rot[0].second, (*vals[rot[1].first]) * rot[1].second,
                (*vals[rot[2].first]) * rot[2].second};
    }
    bool parse(string& s) {
        if (s.length() == 0)
            return false;
        stringstream ss(s);
        char c;
        ss >> x >> c >> y >> c >> z;
        return true;
    }
};

point_s operator-(const point_s& lhs, const point_s& rhs) {
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

point_s operator+(const point_s& lhs, const point_s& rhs) {
    return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

bool operator==(const point_s& lhs, const point_s& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

struct hasher {
    size_t operator()(point_s const& s) const {
        return hash<int64_t>{}(s.x) ^ (hash<int64_t>{}(s.y) << 1) ^ (hash<int64_t>{}(s.z) << 2);
    }
};

typedef point_s beacon_t;

struct scanner_orientation {
    rotation_t rotation;
    point_s position;
};

struct scanner_s {
    int64_t scanner_id;
    vector<beacon_t> beacons;
    point_s position;
    void normalize(const scanner_orientation& orientation) {
        for (auto& beacon : beacons)
            beacon = beacon.rotate(orientation.rotation) + orientation.position;
        position = orientation.position;
    }
    bool overlaps(const scanner_s& other, uint64_t threshold, scanner_orientation& o) const {
        for (auto& rotation : rotations) {
            unordered_map<beacon_t, uint32_t, hasher> cnts;
            for (auto& lhs : beacons)
                for (auto& rhs : other.beacons)
                    cnts[lhs - rhs.rotate(rotation)]++;
            for (auto& el : cnts)
                if (el.second >= threshold) {
                    o = scanner_orientation{rotation, el.first};
                    return true;
                }
        }
        return false;
    }
};

void normalize(vector<scanner_s>& scanners, uint64_t threshold);

void normalize(vector<scanner_s>& scanners, uint64_t threshold) {
    unordered_set<int64_t> fixed;
    deque<int64_t> queue;
    fixed.insert(0);
    queue.push_back(0);
    scanners[0].position = point_s{0, 0, 0};
    while (!queue.empty()) {
        auto tested = queue.front();
        queue.pop_front();
        for (size_t other = 0; other < scanners.size(); other++) {
            if (fixed.find(uint64_t(other)) != fixed.end())
                continue;
            scanner_orientation rot;
            if (!scanners[tested].overlaps(scanners[other], threshold, rot))
                continue;
            scanners[other].normalize(rot);
            queue.push_back(other);
            fixed.insert(other);
        }
    }
}

const char* lines[] = {
#include "day19.txt"
};

int main() {
    auto start = high_resolution_clock::now();
    cout << title << endl;
    vector<scanner_s> scanners;
    string line;
    scanner_s scan;
    scan.beacons.clear();
    for (auto& l : lines) {
        string line(l);
        if (line.length() == 0) {
            scanners.push_back(scan);
            scan.beacons.clear();
            continue;
        }
        if (line[1] == '-') {
            scan.scanner_id = stoll(line.substr(12));
            continue;
        }
        beacon_t b;
        if (b.parse(line))
            scan.beacons.push_back(b);
    }
    scanners.push_back(scan);
    normalize(scanners, 12);
    unordered_set<beacon_t, hasher> unique_beacons;
    for (const auto& s : scanners)
        for (const auto& b : s.beacons)
            unique_beacons.insert(b);
    cout << "Part 1  - " << unique_beacons.size() << endl;
    int64_t best = 0;
    for (auto lhs : scanners)
        for (auto rhs : scanners)
            best = max(best, (lhs.position - rhs.position).manhattan());
    cout << "Part 2  - " << best << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
