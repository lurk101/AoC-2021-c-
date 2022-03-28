#include <chrono>
#include <dirent.h>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <regex>
#include <string>
#include <sys/types.h>
#include <unistd.h>

using namespace std;
using namespace chrono;

struct day {
    unsigned num;
    string name;
    string title;
    unsigned loc;
    double gcc_run;
    double clang_run;
};

bool operator<(const day& a, const day& b) { return a.num < b.num; }

map<unsigned, day> days;

void linecount(day& d) {
    string f_name = d.name + '/' + d.name + ".cpp";
    ifstream f(f_name);
    string l;
    getline(f, l);
    d.title = l.substr(19, l.length() - 23);
    unsigned loc = 1;
    while (getline(f, l))
        loc++;
    d.loc = loc;
    f.close();
}

void compile(day& d, string comp) {
    cout << "Compiling " << d.name << " with " << comp << "++" << endl;
    chdir(d.name.c_str());
    string cmd = comp + "++ -O3 -mtune=native -march=native -o " + comp +
                 ".out " + d.name + ".cpp";
    system(cmd.c_str());
    chdir("..");
}

void run(day& d, string comp) {
    cout << "Running " << d.name << endl;
    chdir(d.name.c_str());
    char buffer[128];
    FILE* pipe;
    double min_t = 1e99;
    for (int i = 0; i < 5; i++) {
        string cmd = string("./") + comp + ".out";
        pipe = popen(cmd.c_str(), "r");
        while (!feof(pipe))
            if (fgets(buffer, 128, pipe) != NULL) {
                if (i == 0)
                    cout << buffer;
                auto it = string(buffer).find("Elapsed - ");
                if (it != string::npos) {
                    double t = stod(buffer + it + 10);
                    if (t < min_t)
                        min_t = t;
                }
            }
        pclose(pipe);
    }
    if (comp == "g")
        d.gcc_run = min_t;
    else
        d.clang_run = min_t;
    chdir("..");
}

string time_to_string(double t) {
    stringstream ss;
    string s;
    double d;
    if (t < 1.0) {
        t *= 1e3;
        s = "us.";
    } else {
        if (t < 1e3)
            s = "ms.";
        else {
            t /= 1e3;
            s = "s. ";
        }
    }
    ss << t << ' ' << s;
    return ss.str();
}

int main(int ac, char* av[]) {
    bool gcc = false, clang = false, md = false, nocomp = false;
    for (int i = 1; i < ac; i++) {
        string s(av[i]);
        if (s == "md")
            md = true;
        else if (s == "noc")
            nocomp = true;
    }
    struct dirent* entry;
    DIR* dir = opendir(".");
    const regex rx("^(day)([0-9]+)$");
    while ((entry = readdir(dir)) != NULL) {
        day d;
        cmatch match;
        if (regex_match(entry->d_name, match, rx)) {
            d.name = entry->d_name;
            if (match.size() == 3) {
                d.num = stoi(match[2]);
                days[d.num] = d;
            }
        }
    }
    closedir(dir);
    cout << endl;
    for (auto& day : days) {
        linecount(day.second);
        if (!nocomp) {
            compile(day.second, "g");
            compile(day.second, "clang");
        }
    }
    for (auto& day : days) {
        run(day.second, "g");
        run(day.second, "clang");
    }
    if (!md)
        cout << "Title                             LOC  g++ time     clang++ time" << endl
             << "-------------------------------   ---  -----------  ------------" << endl;
    else
        cout << "| Title | LOC | g++ time | clang++ time |" << endl << "| --- | --- | --- | --- |" << endl;
    for (auto& d : days)
        if (md)
            cout << "| " << d.second.title << " | " << d.second.loc << " | "
                 << time_to_string(d.second.gcc_run) << " | " << time_to_string(d.second.clang_run)
                 << " |" << endl;
        else
            cout << left << setw(32) << d.second.title << right << setw(5) << d.second.loc << right
                 << setw(13) << time_to_string(d.second.gcc_run) << right << setw(14)
                 << time_to_string(d.second.clang_run) << endl;
}
