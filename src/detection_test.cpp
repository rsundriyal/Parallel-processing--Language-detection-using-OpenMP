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
#include "../headers/detector.h"


using namespace std;


void detect(char* text_fname, bool print_results, unsigned int iterations);

int main(int argc, char** argv) {

    if (argc != 3) {
        const char* args = "<Thread-count> <file>";
        usage(argv[0], args);
    } else {
        int threads = atoi(argv[1]);
        char* text_fname = argv[2];

        if (threads != 0) {
            omp_set_num_threads(threads);
        }

        bool print_results = true;
        unsigned int iterations = 10;
        detect(text_fname, print_results, iterations);
        Timer* timer = Timer::instance();
        cout << (*timer) << timer->delimiter;

        return 0;
    }

}

void detect(char* text_fname, bool print_results, unsigned int iterations) {

    Detector detector;
    detector.text_feed(text_fname);
    for (unsigned int i = 0; i < iterations; i++)
        detector.process();

    if (print_results) {
        for (unsigned int i = 0; i < detector.ref_lang_count; i++) {
            Detector::entry entry = detector[i];
            cout << "[" << entry.lang << "] compliance ratio: " << ": " << entry.probability << " / 1.0" << endl;
        }
    }
}
