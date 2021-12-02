// day2.cpp

#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int part1() {
    ifstream file("day2.txt");
    string command;
    int distance;
    int x = 0, z = 0;
    while (file >> command) {
        file >> distance;
        if (command == "forward")
            x += distance;
        else if (command == "down")
            z += distance;
        else
            z -= distance;
    }
    file.close();
    return x * z;
}

int part2() {
    ifstream file("day2.txt");
    string command;
    int distance;
    int x = 0, z = 0, aim = 0;
    while (file >> command) {
        file >> distance;
        if (command == "forward") {
            x += distance;
            z += aim * distance;
        } else if (command == "down")
            aim += distance;
        else
            aim -= distance;
    }
    file.close();
    return x * z;
}

int main() {
    cout << "Part 1 - " << part1() << endl;
    cout << "Part 2 - " << part2() << endl;
}

