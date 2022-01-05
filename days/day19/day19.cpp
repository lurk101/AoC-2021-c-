#define title "--- Day 19: Beacon Scanner ---"
#include <algorithm>
#include <chrono>
#include <cstring>
#include <deque>
#include <functional>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;
using namespace chrono;

typedef struct {
    int dimension, polarity;
} rotation_t[3];

const rotation_t rotations[24] = {
    {{0, 1}, {1, 1}, {2, 1}},    {{0, 1}, {2, 1}, {1, -1}},  {{0, 1}, {1, -1}, {2, -1}},
    {{0, 1}, {2, -1}, {1, 1}},   {{0, -1}, {1, 1}, {2, -1}}, {{0, -1}, {2, -1}, {1, -1}},
    {{0, -1}, {1, -1}, {2, 1}},  {{0, -1}, {2, 1}, {1, 1}},  {{1, 1}, {0, 1}, {2, -1}},
    {{1, 1}, {2, -1}, {0, -1}},  {{1, 1}, {0, -1}, {2, 1}},  {{1, 1}, {2, 1}, {0, 1}},
    {{1, -1}, {0, 1}, {2, 1}},   {{1, -1}, {2, 1}, {0, -1}}, {{1, -1}, {0, -1}, {2, -1}},
    {{1, -1}, {2, -1}, {0, 1}},  {{2, 1}, {0, 1}, {1, 1}},   {{2, 1}, {1, 1}, {0, -1}},
    {{2, 1}, {0, -1}, {1, -1}},  {{2, 1}, {1, -1}, {0, 1}},  {{2, -1}, {0, 1}, {1, -1}},
    {{2, -1}, {1, -1}, {0, -1}}, {{2, -1}, {0, -1}, {1, 1}}, {{2, -1}, {1, 1}, {0, 1}}};

struct beacon_s {
    int x, y, z;
    int manhattan() { return abs(x) + abs(y) + abs(z); }
    beacon_s rotate(const rotation_t& rot) const {
        const int vals[3] = {x, y, z};
        return {(vals[rot[0].dimension]) * rot[0].polarity,
                (vals[rot[1].dimension]) * rot[1].polarity,
                (vals[rot[2].dimension]) * rot[2].polarity};
    }
};

beacon_s operator-(const beacon_s& a, const beacon_s& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

beacon_s operator+(const beacon_s& a, const beacon_s& b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

bool operator==(const beacon_s& a, const beacon_s& b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

struct sig_hash {
    size_t operator()(beacon_s const& s) const { return s.x ^ s.y ^ s.z; }
};

struct scanner_orientation {
    rotation_t rotation;
    beacon_s position;
};

struct scanner_s {
    vector<beacon_s> beacons;
    vector<int16_t> sig;
    beacon_s position;
    void generate_sig(void) {
        sig.reserve(beacons.size() * (beacons.size() - 1) / 2);
        for (auto it1 = beacons.begin(); it1 != beacons.end() - 1; it1++)
            for (auto it2 = it1 + 1; it2 != beacons.end(); it2++) {
                auto dx = it1->x - it2->x;
                auto dy = it1->y - it2->y;
                auto dz = it1->z - it2->z;
                sig.push_back(uint64_t(dx) * dx + uint64_t(dy) * dy + uint64_t(dz) * dz);
            }
        sort(sig.begin(), sig.end());
    }
    bool check_sig(const scanner_s& other) const {
        unsigned match = 0;
        for (auto it = sig.begin(), ito = other.sig.begin();
             it != sig.end() && ito != other.sig.end();) {
            if (*it < *ito)
                it++;
            else if (*it > *ito)
                ito++;
            else {
                it++;
                ito++;
                match++;
            }
        }
        return match >= 66;
    }

    void normalize(const scanner_orientation& orientation) {
        for (auto& beacon : beacons)
            beacon = beacon.rotate(orientation.rotation) + orientation.position;
        position = orientation.position;
    }
    bool overlaps(const scanner_s& other, unsigned threshold, scanner_orientation& o) const {
        if (!check_sig(other))
            return false;
        for (auto& rotation : rotations) {
            unordered_map<beacon_s, unsigned, sig_hash> cnts;
            for (auto& a : beacons)
                for (auto& b : other.beacons)
                    cnts[a - b.rotate(rotation)]++;
            for (auto& el : cnts)
                if (el.second >= threshold) {
                    o.position = el.first;
                    o.rotation[0] = rotation[0];
                    o.rotation[1] = rotation[1];
                    o.rotation[2] = rotation[2];
                    return true;
                }
        }
        return false;
    }
};

vector<scanner_s> scanners;

void normalize(void) {
    unordered_set<int> fixed;
    deque<int> queue;
    fixed.insert(0);
    queue.push_back(0);
    scanners[0].position = beacon_s{0, 0, 0};
    while (!queue.empty()) {
        auto tested = queue.front();
        queue.pop_front();
        for (unsigned other = 0; other < scanners.size(); other++) {
            if (fixed.find(other) != fixed.end())
                continue;
            scanner_orientation rot;
            if (!scanners[tested].overlaps(scanners[other], 12, rot))
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
    string line;
    scanner_s scan;
    scan.beacons.clear();
    for (auto& line : lines) {
        if (strlen(line) == 0) {
            scanners.push_back(scan);
            scan.beacons.clear();
            continue;
        }
        if (line[1] == '-')
            continue;
        beacon_s b;
        sscanf(line, "%d,%d,%d", &b.x, &b.y, &b.z);
        scan.beacons.push_back(b);
    }
    scanners.push_back(scan);
    for (auto& scanner : scanners)
        scanner.generate_sig();
    normalize();
    unordered_set<beacon_s, sig_hash> unique_beacons;
    for (const auto& s : scanners)
        for (const auto& b : s.beacons)
            unique_beacons.insert(b);
    cout << "Part 1  - " << unique_beacons.size() << endl;
    int best = 0;
    for (auto a : scanners)
        for (auto b : scanners)
            best = max(best, (a.position - b.position).manhattan());
    cout << "Part 2  - " << best << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
