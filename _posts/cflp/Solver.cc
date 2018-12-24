#include <Solver.hh>

using namespace std;

template <typename T>
ostream& operator<<(ostream& out, const vector<T>& src) {
    for (int i = 0; i < src.size(); ++i) {
        out << src[i] << " ";
    }
    return out;
}

Solver::Solver(const int sn) {
    this->sn = sn;
}

/* load instance p[sn]
 * @return      whether p[sn] is loaded successfully
 */
bool Solver::load() {
    try {
        string path = "Instances/p" + to_string(this->sn);
        ifstream fin(path);

        fin >> this->nfacility >> this->ncustomer;

        this->b = this->f = vector<int>(this->nfacility);
        for (int i = 0; i < this->nfacility; ++i) {
            fin >> this->b[i] >> this->f[i];
        }
        this->a = vector<double>(this->ncustomer);
        for(int i = 0; i < this->ncustomer; ++i) {
            fin >> this->a[i];
        }
        this->c = vector<vector<double>>(this->nfacility, vector<double>(this->ncustomer));
        for (int i = 0; i < this->nfacility; ++i) {
            for (int j = 0; j < this->ncustomer; ++j) {
                fin >> this->c[i][j];
            }
        }
        this->y = vector<int>(this->nfacility, 1);
    } catch(exception e) {
        cerr << "Failed to load the target" << endl;
        return false;
    }

    return true;
}

/* solve the problem with greedy algorithm
 * @return      make_pair(cost, time)
 */
pair<double, double> Solver::greedy(bool init) {
    if (init && !Solver::load()) {
        return make_pair(-1.0, -1.0);
    }

    struct timeb t0, t1;
    ftime(&t0);
    double cost = 0.0;
    vector<int> capacity = this->b;
    this->x = vector<vector<int>>(this->nfacility);
    for (int j = 0; j < this->ncustomer; ++j) {
        int best_facility = -1;
        double best_cost = DBL_MAX;
        for (int i = 0; i < this->nfacility; ++i) {
            if (this->y[i] && capacity[i] >= this->a[j] && this->c[i][j] < best_cost) {
                best_facility = i;
                best_cost = this->c[i][j];
            }
        }
        if (best_facility == -1) {
            if (init) {
                cerr << "Failed to solve with greedy" << endl;
            }
            return make_pair(-1.0, -1.0);
        } else {
            capacity[best_facility] -= this->a[j];
            cost += best_cost;
            x[best_facility].push_back(j);
        }
    }
    ftime(&t1);

    string status = "";
    for (int i = 0; i < this->nfacility; ++i) {
        if (this->x[i].empty()) {
            status += "0 ";
        } else {
            status += "1 ";
            cost += this->f[i];
        }
    }
    if (init) {
        string path = "Results/p" + to_string(this->sn);
        ofstream fout(path);
        fout << fixed << setprecision(1) << cost << endl;
        fout << status << endl;
        for (int i = 0; i < this->nfacility; ++i) {
            fout << x[i] << endl;
        }
        fout.close();
    }

    return make_pair(cost, (t1.time-t0.time)+(t1.millitm-t0.millitm)*1e-3);
}

/* solve the problem with tabu search
 * @return      make_pair(cost, time)
 */
pair<double, double> Solver::tabu() {
    if (!Solver::load()) {
        return make_pair(-1.0, -1.0);
    }

    int T = 100;
    struct timeb t0, t1;
    ftime(&t0);
    double global_cost = Solver::greedy(false).first;
    if (global_cost < 0) {
        cerr << "Failed to solve with greedy" << endl;
        return make_pair(-1.0, -1.0);
    }
    vector<int> global_y = this->y;
    vector<vector<int>> global_x = this->x;
    while (T-- != 0) {
        double local_cost = DBL_MAX;
        vector<int> local_y = this->y;
        vector<vector<int>> local_x = this->x;
        for (int i = 0; i < this->nfacility; ++i) {
            this->y[i] ^= 1;
            pair<double, double> result = Solver::greedy(false);
            if (result.first >= 0 && ((result.first < local_cost && ts.find(Solver::encode(this->y)) == ts.end()) || (result.first < global_cost))) {
                local_cost = result.first;
                local_y = this->y;
                local_x = this->x;
            }
            this->y[i] ^= 1;
        }
        if (local_cost == DBL_MAX) {
            break;
        } else if (local_cost < global_cost) {
            global_cost = local_cost;
            global_y = local_y;
            global_x = local_x;
        }
        this->y = local_y;
        this->x = local_x;
        this->ts.insert(Solver::encode(local_y));
    }
    ftime(&t1);

    string status = "";
    for (int i = 0; i < this->nfacility; ++i) {
        if (global_x[i].empty()) {
            status += "0 ";
        } else {
            status += "1 ";
        }
    }
    string path = "Results/p" + to_string(this->sn);
    ofstream fout(path);
    fout << fixed << setprecision(1) << global_cost << endl;
    fout << status << endl;
    for (int i = 0; i < this->nfacility; ++i) {
        fout << global_x[i] << endl;
    }
    fout.close();

    return make_pair(global_cost, (t1.time-t0.time)+(t1.millitm-t0.millitm)*1e-3);
}

string Solver::encode(const vector<int>& local_y) {
    string code = "";
    for (int i = 0; i < this->nfacility; ++i) {
        code += to_string(local_y[i]);
    }
    return code;
}