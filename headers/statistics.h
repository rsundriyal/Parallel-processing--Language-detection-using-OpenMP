/* 
 * File:   Statistics.h
 * Author: CMPE 275 Gash Team - Ravi,Sarthak & Promod
 *
 */

#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>

#include "utilities.h"
#include "timer.h"

using namespace std;

#ifndef	STATS_H
#define     STATS_H

#define NULL_CH '\0'
#define NL_CH '\n'
#define TAB_CH '\t'

class LangStats {
public:

    typedef u_int32_t tr_int;
    static const tr_int POSSIBILITIES = 265 * 265 * 265 * sizeof (char);

    double inline frequency(tr_int index) {
        return (double) occurances[index] / (double) total;
    }

    char* lang;

    LangStats();
    LangStats(char* lang);

    ~LangStats();

    void process(char** input_fnames, unsigned int count);
    void process(const char* input_fname);

    void open_stats();
    void save_stats();

    static const char * DAT_SUFFIX;


private:

    Timer* timer;

    void trigram2index(char* trigram, tr_int* index);

    tr_int *occurances;

    unsigned long total;

    void process_text(char* text, unsigned long txt_len);

    void init();

    static const unsigned int LEN = 3;
};

#endif	

