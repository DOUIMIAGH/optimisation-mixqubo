//-----------------------------------------------------------------------------
/** 
 *
 * Example of MixedQUBO fitness function in c++
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

    // the fitness function is the Walsh function
    if (argc != 2) {
        cerr << "Wrong number of parameters. Usage: ./src/test_mixQUBO ../../instances/mxqubo_test.json" << endl;
        return 1;
    }

    string filename(argv[1]);

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

    solution.z = { -0.5, 0.0, 0.2 };
    solution.x = { false, true, true, false };

    feval.eval(solution);
    
    cout << solution << endl; 

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
