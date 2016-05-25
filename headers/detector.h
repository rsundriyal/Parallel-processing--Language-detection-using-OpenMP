/*
 * File:   detector.h
 * Author: CMPE 275 Gash Team - Ravi,Sarthak & Promod
 *
 */

#ifndef DETECTOR_H
#define	DETECTOR_H

#include <iostream>
#include <dirent.h>
#include "statistics.h"
#include "timer.h"

using std::cout;

class Detector {
public:

    Detector();
    
    void text_feed(char* test_fname);
    
    virtual ~Detector();
    
    struct entry {
        const char* lang;
        double probability;
    };

    void process();

    unsigned int ref_lang_count;

    entry operator[](unsigned int sorted_index);

private:
    Timer* timer;

    LangStats* refer_stats;
    LangStats* tst_stats;

    double* probabilities;
    unsigned int* indexes;
    void process(unsigned int index);

    static unsigned int get_saved_stats_langs(char*** langs);

    void fill_sorted_indexes();
};

#endif

