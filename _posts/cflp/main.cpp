#include <iostream>
#include <Solver.hh>

using namespace std;


int main() {
    string path = "Results/result.csv";
    ofstream fout(path);
    fout << ",Result,Time(s)" << endl;
    for (int i = 1; i <= 71; ++i) {
        pair<double, double> result = Solver(i).tabu();
        fout << "p" + to_string(i) + "," + to_string(result.first) + "," + to_string(result.second) << endl;
    }
    fout.close();
    return 0;
}

// g++ -std=c++11 main.cpp Solver.cc -I . -o main.exec