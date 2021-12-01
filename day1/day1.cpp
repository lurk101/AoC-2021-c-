// day1.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <numeric>
using namespace std;

int part(int window_size) {
    ifstream file("day1_data.txt");
    list<int> depths;
    int depth, last_sum, count = 0;
    for (int i = 0; i < window_size; i++) {
        file >> depth;
        depths.push_back(depth);
    }
    last_sum = accumulate(begin(depths), end(depths), 0);
    while (file >> depth) {
        depths.push_back(depth); depths.pop_front();
        int sum = accumulate(begin(depths), end(depths), 0);
        if (sum > last_sum)
            count++;
        last_sum = sum;
    }
    file.close();   //close the file object.
    return count;
}

int main() {
    cout << "Part 1 - " << part(1) << endl;
    cout << "Part 2 - " << part(3) << endl;
}

