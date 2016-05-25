/* 
 * File:   analytics.cpp
 * Author: CMPE 275 Gash Team - Ravi,Sarthak & Promod
 *
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <omp.h>
#include "../headers/statistics.h"
#include "../headers/utilities.h"

using namespace std;

void analyzer(char* lang, char** inputs, unsigned int count, unsigned int itr);
void save_stats(LangStats* stats, char* lang);

int main(int argc, char** argv) {


    if (argc < 4) {
        const char* args = "<thread-count> <Language> <file1> [<file2> ...]";
        usage(argv[0], args);
    }

    int threads = atoi(argv[1]);
    char* lang = argv[2];
    char** inputs = &argv[3];
    unsigned int inputs_count = (unsigned int) argc - 3;

    if (threads != 0) {
        omp_set_num_threads(threads);
    }
    
    unsigned int itr = 10;
    
    analyzer(lang, inputs, inputs_count, itr);
    Timer* timer = Timer::instance();
    cout << (*timer) << timer->delimiter;

    return 0;
}

void analyzer(char* lang, char** inputs, unsigned int count, unsigned int itr) {

    LangStats stats(lang);
    for (unsigned int i = 0; i < itr; i++)
        stats.process(inputs, count);

    stats.save_stats();

}


