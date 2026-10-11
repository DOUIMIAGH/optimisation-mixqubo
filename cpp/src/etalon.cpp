//-----------------------------------------------------------------------------
/** 
 *
 * Code étalon pour connaitre le temps d'excution sur votre machine.
 *   Temps d'exécution sur ma machine 1000ms = 10s
 *
 * Author:
 *  Sébastien Verel
 * 
 * Date:
 *  2026/03/27
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

    //string filename("../../instances/mxqubo_test.json");
    string filename(argc >= 2 ? argv[1] : "../../instances/mxqubo_v0_15.json");
    
    MixQUBO feval;
    feval.read_json(filename);

    //cout << eval.n << endl;

    /* =========================================================
     *
     * executes from random solutions
     *
     * ========================================================= */

    // The solution
    Solution solution(feval.d, feval.n);

    // Apply random initialization
    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    std::chrono::duration<long long, std::ratio<1, 1000>>::rep dur;
    auto start = std::chrono::steady_clock::now();

    for(int i = 0; i < 30000; i++) {
        for(unsigned i = 0; i < solution.d; i++)
            solution.z[i] = dist(rng) * 2 - 1;

        for(unsigned i = 0; i < solution.n; i++)
            solution.x[i] = (dist(rng) < 0.5);

        feval.eval(solution);
        
        //cout << solution << endl; 
    }

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    dur = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_seconds).count();

    cout << dur << " milliseconds" << endl;

    return 0;
}

// A main that catches the exceptions

int main(int argc, char **argv)
{
    try {
        return main_function(argc, argv);
    }
    catch (exception& e) {
        cerr << "Exception: " << e.what() << '\n';
    }
    return 1;
}
