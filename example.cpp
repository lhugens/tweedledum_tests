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
#include <tweedledum/Synthesis/xag_synth.h>
#include <tweedledum/Utils/Visualization/string_utf8.h>

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

void test1(){
    /* create a xag from a dimacs string and print it*/
    mockturtle::xag_network xag;
    std::string filename ("/home/hugens/shared/uni/project/tweedledum/examples/3sat.dimacs");
    lorina::read_dimacs(filename, mockturtle::dimacs_reader(xag));
    mockturtle::write_dot(xag, std::cout);
}

void test2(){
    /* check if this dimacs instance gives same oracle in both python and cpp implementations*/
    std::string dimacs_str ("p cnf 3 5\n-1 -2 -3 0\n1 -2 3 0\n1 2 -3 0\n1 -2 -3 0\n-1 2 3 0");
    std::stringstream ss(dimacs_str);
    mockturtle::xag_network xag;
    lorina::read_dimacs(ss, mockturtle::dimacs_reader(xag));
    tweedledum::Circuit oracle = tweedledum::xag_synth(xag);
    tweedledum::print(oracle, 1000);
}

void test3(){
    /* extracting number of qubits and circuit depth */
    std::string dimacs_str ("p cnf 3 5\n-1 -2 -3 0\n1 -2 3 0\n1 2 -3 0\n1 -2 -3 0\n-1 2 3 0");
    std::stringstream ss(dimacs_str);
    mockturtle::xag_network xag;
    lorina::read_dimacs(ss, mockturtle::dimacs_reader(xag));
    tweedledum::Circuit oracle = tweedledum::xag_synth(xag);
    tweedledum::print(oracle, 1000);

    cout << "num_qubits = " << oracle.num_qubits() << "\n";
    cout << "num_instructions = " << oracle.num_instructions() << "\n";
}

void test4(){
    /* this test shows that sizeof(xag) returns the size of the mockturtle::xag_network type
     * and not necessarily of what is stored in that type */
    std::string dimacs_str ("p cnf 3 5\n-1 -2 -3 0\n1 -2 3 0\n1 2 -3 0\n1 -2 -3 0\n-1 2 3 0");
    std::stringstream ss(dimacs_str);
    mockturtle::xag_network xag;
    lorina::read_dimacs(ss, mockturtle::dimacs_reader(xag));
    cout << sizeof(xag) << endl;

    std::stringstream ss1(random_SAT(300, 3, 1290));
    mockturtle::xag_network xag1;
    lorina::read_dimacs(ss1, mockturtle::dimacs_reader(xag1));
    cout << sizeof(xag1) << endl;
}

void test5(){
    /* since memory usage of a XAG seems to be impossible to 
     * figure out, let's focus on network size instead */
    std::string dimacs_str ("p cnf 3 5\n-1 -2 -3 0\n1 -2 3 0\n1 2 -3 0\n1 -2 -3 0\n-1 2 3 0");
    std::stringstream ss(dimacs_str);
    mockturtle::xag_network xag;
    lorina::read_dimacs(ss, mockturtle::dimacs_reader(xag));
    mockturtle::write_dot(xag, std::cout);
    cout << "size = " << xag.size() << endl;
    cout << "num_cis = " << xag.num_cis() << endl;
    cout << "num_cos = " << xag.num_cos() << endl;
    cout << "num_latches = " << xag.num_latches() << endl;
    cout << "num_pis = " << xag.num_pis() << endl;
    cout << "num_pos = " << xag.num_pos() << endl;
    cout << "num_registers = " << xag.num_registers() << endl;
    cout << "num_gates = " << xag.num_gates() << endl;

}


int main(){

    /* intialize random seed for the rand() generator */
    srand (time(NULL));

    test5();
}

