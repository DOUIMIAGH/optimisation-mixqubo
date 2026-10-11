//-----------------------------------------------------------------------------
/** 
 *  Update best known solutions
 *   each csv files with 4 columns :
 *      id: instance id
 *      f: double, value of f
 *      z: "0.1,-0.2,...": vector of continuous variables
 *      x: 00011 vector of binary values
 *
 * Author:
 *  Sébastien Verel
 * 
 * Date:
 *  2026/04/01
 * 
 */
//-----------------------------------------------------------------------------

// standard includes
#include <stdexcept>  
#include <iostream>   
#include <fstream>
#include <string.h>
#include <random>
#include <chrono>

//-----------------------------------------------------------------------------
// solution of the problem
#include <solution.hpp>

// objective function
#include <mixQUBO.hpp>

// declaration of the namespace
using namespace std;

// Main function
//-----------------------------------------------------------------------------
int main_function(int argc, char **argv)
{
    /* =========================================================
     *
     * Eval fitness function (full evaluation)
     *
     * ========================================================= */

    if (argc != 4) {
        cerr << "Wrong number of arguments. Usage: ./src/update_best ../../instances/example_results.csv ../../instances/best_known.csv best_known_updated.csv" << endl;
        cerr << "Each file with 4 columns: id f z x" << endl ;
        exit(0); 
    }

    // new solutions: id f z x
    string filename_new(argv[1]);

    // best known solutions: id f z x
    string filename_best(argv[2]);

    // update best known solutions: id f z x
    string filename_new_best(argv[3]);

    /* =========================================================
     *
     * Function MixQUBO
     *
     * ========================================================= */

    MixQUBO mxqubo;

    /* =========================================================
     *
     * 
     *
     * ========================================================= */

    ofstream file_newbest(filename_new_best, std::ios::out);

    if (!file_newbest) {
        cerr << "Impossible to open: " << filename_new_best << endl;
        exit(0);
    }

    /* =========================================================
     *
     * 
     *
     * ========================================================= */

    ifstream file_best(filename_best);

    if (!file_best) {
        cerr << "Impossible to open: " << filename_best << endl;
        exit(0);
    }

    // best known solution for the 16 instances
    vector<Solution> best_known(16);

    // header
    string header;
    std::getline(file_best, header);

    // id of the instance
    int id ;

    // Solution
    Solution solution;

    int n_best = 0;

    while (file_best >> id) {
        n_best++;

        file_best >> solution ;

        best_known[id] = solution;        
    }

    if (n_best != 16) {
        cout << "Error in the best known file, there is " << n_best << " instead of 16." << endl;
    }

    file_best.close();

    // check
    /*
    for(int i = 0; i < best_known.size(); i++) {
        cout << i << " " << best_known[i] << endl;
    }
    */

    /* =========================================================
     *
     * 
     *
     * ========================================================= */

    ifstream file_new(filename_new);

    if (!file_new) {
        cerr << "Impossible to open: " << filename_new << endl;
        exit(0);
    }

    // header
    std::getline(file_new, header);

    // id of the instance
    int previous_id = -1;

    vector<Solution> new_best(best_known.size());
    double default_value = 1000003;

    for(int i = 0; i < new_best.size(); i++)
        new_best[i].f = default_value;

    while (file_new >> id) {
        if (previous_id != id) {
            string filename = "../../instances/mxqubo_v0_" + std::to_string(id) + ".json" ;
            mxqubo.read_json(filename);
            previous_id = id;
        }

        file_new >> solution ;

        mxqubo.eval(solution);

        if (solution.f < new_best[id].f) {
            new_best[id] = solution;
        }
    }

    file_new.close();

    // compare best

    file_newbest << "id f z x" << endl;

    for(int i = 0; i < best_known.size(); i++) {
        file_newbest << id << " ";
        if (new_best[i].f != default_value) {
            if (new_best[i].f < best_known[i].f) {
                cout << "New best instance " << i << " : " << new_best[i].f << " (previously " << best_known[i].f << ")" << endl;
                file_newbest << new_best[i] << endl;
            } else
                file_newbest << best_known[i] << endl;            
        } else
            file_newbest << best_known[i] << endl;            
    }

    file_newbest.close();

    return 1;
}

// A main that catches the exceptions

int main(int argc, char **argv)
{
    try {
        main_function(argc, argv);
    }
    catch (exception& e) {
        cout << "Exception: " << e.what() << '\n';
    }
    return 1;
}
