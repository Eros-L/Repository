#ifndef _SOLVER_HH_
#define _SOLVER_HH_

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cfloat>
#include <sys/timeb.h>
#include <unordered_set>

class Solver {
public:
	Solver(const int);
	std::pair<double, double> greedy(const bool = true);
	std::pair<double, double> tabu();

	template <typename T>
	friend std::ostream& operator<<(std::ostream&, const std::vector<T>&);

private:
	int sn;									// sequence number of the current instance
	int nfacility;							// number of facilities
	int ncustomer;							// number of customers

	std::vector<double> a;					// a[j], the demand of customer j
	std::vector<int> b;						// b[i], the capacity of facility i
	std::vector<int> f;						// f[i], the fixed cost for using/opening facility i
	std::vector<std::vector<double>> c;		// c[i][j], the cost for assigning customer j to facility i

	std::vector<int> y;						// y[i] equals 1 if facility i is available, otherwise equals 0
	std::vector<std::vector<int>> x;		// x[i] means facility i serves customer x[i]

	std::unordered_set<std::string> ts;		// tabu search list

	std::string encode(const std::vector<int>&);

	bool load();
};

#endif