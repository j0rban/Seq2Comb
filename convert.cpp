#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "circuit.h"

using namespace std;

circuit final_c, inputs_c, outputs_c, gates_c, scanins_c, scanouts_c;
string og_bench, final_bench;

void create_subcircuits(){
    string line;
    ifstream og(og_bench.c_str());

    if(og.is_open()){

        int inCount = 0,outCount = 0,gateCount = 0,dffCount = 0;

        while (getline(og,line)){

            if (line == "") continue;       //skip blank line

            node n;
            int input,output,comment,dff;

            //check if line is input
            input = line.find("INPUT");
            if(input != string::npos){
                n.parse_line(line);
                inputs_c.add_node(n);
                inCount++;

                continue;
            }

            //check if line is output
            output = line.find("OUTPUT");
            if(output != string::npos){  
                n.parse_line(line);
                outputs_c.add_node(n);
                outCount++;

                continue;
            }

            //check if line is dff
            dff = line.find("DFF(");
            if(dff != string::npos){
                n.parse_line(line);

                node scan = node(n.id, false, "INPUT");
                scanins_c.add_node(scan);
                scan = node(n.fanins[0], false, "OUTPUT");
                scanouts_c.add_node(scan);

                dffCount++;

                continue;
            }

            //check if line is other
            comment = line.find("#");
            if((input == string::npos) && (output == string::npos) && (comment == string::npos)){
                n.parse_line(line);
                gates_c.add_node(n);
                gateCount++;
            }
            
        }
        cout << "# Inputs: " << inCount << endl;
        cout << "# Outputs: " << outCount << endl;
        cout << "# DFFs: " << dffCount << endl;
        cout << "# Gates: " << gateCount << endl;
    }
}


void create_final(){
    ofstream final_b(final_bench.c_str(), fstream::app);

    final_b << endl << "#Primary Inputs" << endl;
    inputs_c.write_to_bench(final_bench);

    final_b << endl << "#Scan Inputs" << endl;
    scanins_c.write_to_bench(final_bench);

    final_b << endl << "#Primary Outputs" << endl;
    outputs_c.write_to_bench(final_bench);

    final_b << endl << "#Scan Outputs" << endl;
    scanouts_c.write_to_bench(final_bench);

    final_b << endl << "#Gates" << endl;
    gates_c.write_to_bench(final_bench);

    cout << endl << final_bench << " has been created!" << endl;
}


int main(){
    int num_dffs;

    //Get bench to encrypt name
    cout << "Enter benchmark file name (do not include '.bench'): ";
    cin >> og_bench;
    final_bench = "combinational/" + og_bench + "_comb.bench";\
    og_bench.append(".bench");

    create_subcircuits();
    create_final();

    return 0;
}