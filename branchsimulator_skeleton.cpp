#include <climits>
#include <iostream>
#include <fstream>
#include <string>

#include <vector>
#include <cstdlib>
#include <bitset>
#include <map>

using namespace std;

#define SN 00 // Strong Not Taken
#define WN 01 // Weak Not Taken
#define WT 10 // Weak Taken
#define ST 11 // Strong Taken


//extracting the LSB
string bit_extract(bitset<32> addr, int m)
{
    string addr2s = addr.to_string(); //converting to string
    string bits; // declaration
    bits = addr2s.substr(32-m, m); // extraction
    return bits;
}




int main(int argc, char* argv[]){


    ifstream config;
    int m;

    config.open(argv[1]);
    int k;
    config >> m >> k; //reading
    config.close();


    int e = (int)pow(2, m); //entry

    vector<unsigned long> s_c(e, ST); //vector declaration

    int miss =0; // initializing the misses



    ofstream out;
    string out_file_name;
    out_file_name = string(argv[2]) + ".out";
    out.open(out_file_name.c_str());

    ifstream trace;
    trace.open(argv[2]);


    bitset<32> pc_s; // the address from the memory trace store in the bitset;

    if (trace.is_open() && out.is_open()) {
        if (m <= 20){
            while (!trace.eof()) {
                unsigned long pc;
                bool taken;
                trace >> std::hex >> pc >> taken;

                pc_s = bitset<32>(pc);


                string counter_bits = bit_extract(pc_s, m);
                char *ptr;
                long bits = strtol(counter_bits.c_str(), &ptr, k);


                bool prediction;

                if (taken)
                {

                    if (s_c[bits] == WN){
                        s_c[bits] = WT;
                        prediction= false;
                    }

                    else if (s_c[bits] == SN){

                        s_c[bits]= WN;
                        prediction= false;
                    }

                    else if (s_c[bits] == ST || s_c[bits] == WT){

                        s_c[bits] = ST;
                        prediction = true;
                    }
                }

                else {

                    if (s_c[bits] == WN || s_c[bits] == SN){

                        s_c[bits] = SN;
                        prediction = false;
                    }

                    else if (s_c[bits] == WT){

                        s_c[bits] = WN;
                        prediction = true;
                    }

                    else if (s_c[bits] == ST){

                        s_c[bits] = WT;
                        prediction = true;
                    }

                }

                if (taken == !prediction)

                {
                    miss = miss+1;
                }

                out << prediction << endl;

            }
            cout << "The number of miss we have with m = "<< m << " is: " << miss <<endl;
            trace.close();
            out.close();
        }
        else
            cout << "Please enter value equal to or less than 20"
                 <<endl;
    } else
        cout << "\n Unable to open trace or traceout file ";



    return 0;
}
