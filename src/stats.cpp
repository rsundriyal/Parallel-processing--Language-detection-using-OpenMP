/*
 * File:   stats.cpp
 * Author: CMPE 275 Gash Team - Ravi,Sarthak & Promod
 *
 */

#include "../headers/statistics.h"
#include <omp.h>

const char * LangStats::DAT_SUFFIX = ".dat";

LangStats::LangStats() {

    init();
}

LangStats::LangStats(char* lang) {
    init();
    this->lang = lang;
}

void LangStats::init() {
    this->occurances = new tr_int[POSSIBILITIES];
    this->total = 0;
    timer = Timer::instance();

}

LangStats::~LangStats() {

    if (occurances) {
        delete [] occurances;
    }
}

void LangStats::process(char** input_fnames, unsigned int count) {

    for ( unsigned int i = 0; i < count; i++) { // For each given file

        const char* input_fname = input_fnames[i];
        process(input_fname);
    }
}

void LangStats::process(const char* input_fname) {
    char* text = NULL;
    ifstream ifs;
    ifs.open(input_fname);

    if (ifs.is_open()) {

        // Find file length
        ifs.seekg(0, ios::end);
        unsigned long txt_len = (unsigned long) ifs.tellg();
        ifs.seekg(0, ios::beg);
        txt_len -= (unsigned long) ifs.tellg();

        cout<< " <> "; //ravi
        cout<< txt_len; //ravi
        cout<< " <> "; //ravi

        text = new char[txt_len];

        ifs.read(text, txt_len);
        if (!(ifs.fail())) {
            total += txt_len;
            process_text(text, txt_len);
        } else {
            cerr << "Cannot read file " << input_fname << endl;
        }
        delete[] text;
        ifs.close();
    } else {
        cerr << "Cannot open file " << input_fname << endl;
    }
}

void LangStats::trigram2index(char* trigram, tr_int* index) {

    trigram[0] = (trigram[0] < 32) ? ' ' : trigram[0];
    trigram[1] = (trigram[1] < 32) ? ' ' : trigram[1];
    trigram[2] = (trigram[2] < 32) ? ' ' : trigram[2];
    (*index) = 0;
    memcpy(index, trigram, LEN * sizeof (char));
}
// Ravi Changes long
void LangStats::process_text(char* text, unsigned long txt_len) {

    double time = omp_get_wtime();
    #pragma omp parallel for
    for (tr_int tr_i = 0; tr_i < txt_len - 2; tr_i++) {

        tr_int index;
        trigram2index(text + tr_i, &index);
        if (index != 0x202000 && index != 0x002020) {
            #pragma omp atomic
            occurances[index]++;
        }
    }
    time = (omp_get_wtime() - time);
    timer->add(time);
}

void LangStats::save_stats() {
    ofstream ofs;

    ofs.open(string(lang).append(DAT_SUFFIX).c_str());
    for (tr_int i = 0; i < POSSIBILITIES; i++) {
        if (occurances[i] > 0) {
            // content of `(char*) &i` equals to string of i'th available trigram
            ofs << (char*) &i << TAB_CH << occurances[i] << endl;
        }
    }
    ofs.close();

}

void LangStats::open_stats() {

    ifstream ifs(string(lang).append(DAT_SUFFIX).c_str());
    if (ifs.is_open()) {

        char trigram[LEN + 1];
        trigram[LEN] = NULL_CH;
        tr_int index;
        while (!ifs.eof()) {
            ifs.getline(trigram, LEN + 1, TAB_CH);
            trigram2index(trigram, &index);
            char occ_str[100];
            memset(occ_str, NULL_CH, 50);
            ifs.getline(occ_str, 50, NL_CH);
            total += (occurances[index] = a2long(occ_str));

        }
    }

    ifs.close();
}

