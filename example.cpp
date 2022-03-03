#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <vector>
#include <algorithm>

#include <mockturtle/io/dimacs_reader.hpp>
#include <mockturtle/io/write_dot.hpp>
#include <mockturtle/networks/xag.hpp>

using namespace mockturtle;
using namespace std;

/* generate either 0 or 1 with equal probability */
int rbool(){
    return rand() % 2;
}

/* generate random int between 1 and i, including i */
int rint(int i){
    return rand() % i + 1;
}

string random_SAT(const int N, int K, const int M){
    string dimacs ("p cnf " + to_string(N) + " " + to_string(M));

    /* for each clause*/
    for(int m = 0; m<M; m++){

        /* choose K DISTINCT variables */ 
        vector<int> clause_vars (K, 0);
        clause_vars[0] = rint(N);

        for(int k=1; k<K;){
            int new_var = rint(N);

            /* if the element is not in the vector clause_vars, add it and increment k*/
            if (find(begin(clause_vars), end(clause_vars), new_var) == end(clause_vars)){
                clause_vars[k] = new_var;
                k++;
            }
        }

        /* create clause string and append it to dimacs string */
        string clause ("\n");
        for(int k=0; k<K; k++){
            clause += (rbool() ? "" : "-") + to_string(clause_vars[k]) + " ";
        }
        clause += "0";

        dimacs += clause;
    }
    return dimacs;
}

int main(){

    /* intialize random seed for the rand() generator */
    srand (time(NULL));

    //string s = random_SAT(10, 4, 3);
    
    /* this works
    xag_network xag;
    std::string filename ("/home/hugens/shared/uni/project/tweedledum/examples/3sat.dimacs");
    lorina::read_dimacs(filename, dimacs_reader(xag));
    write_dot(xag, std::cout);
    */

    xag_network xag;

    //std::string dimacs_str ("p cnf 3 5\n-1 -2 -3 0\n1 -2 3 0\n1 2 -3 0\n1 -2 -3 0\n-1 2 3 0");
    //
    string dimacs = random_SAT(300, 3, 1290);

    cout << dimacs << "\n";

    std::stringstream ss(dimacs);

    lorina::read_dimacs(ss, dimacs_reader(xag));
    write_dot(xag, std::cout);

    std::cout << "it fucking worked" << std::endl;
}
