/*
 * File:   detector.cpp
 * Author: CMPE 275 Gash Team - Ravi,Sarthak & Promod
 *
 */

#include <omp.h>
#include <math.h>

#include "../headers/detector.h"

Detector::Detector() {

    timer = Timer::instance();

    char** langs;
    ref_lang_count = get_saved_stats_langs(&langs);
    refer_stats = new LangStats[ref_lang_count];
    for (unsigned int i = 0; i < ref_lang_count; i++) {
        refer_stats[i].lang = new char[5];
        strcpy(refer_stats[i].lang, langs[i]);
        refer_stats[i].open_stats();
    }
    for (unsigned int i = 0; i < ref_lang_count; i++) {
        delete [] langs[i];
    }
    delete [] langs;

    probabilities = new double[ref_lang_count];
    indexes = new unsigned int[ref_lang_count];
}

void Detector::text_feed(char* test_fname) {
    tst_stats = new LangStats();
    tst_stats->process(test_fname);
}

Detector::~Detector() {
    delete tst_stats;
    delete [] refer_stats;
}

void Detector::process() {

    memset(probabilities, 0.0, ref_lang_count * sizeof (double));
    double time = omp_get_wtime();
    
    // If there are at least 4 languages to test, let's test them in parallel.
    
    if (ref_lang_count >= 4) {
        #pragma omp parallel for
        for (unsigned int lang_i = 0; lang_i < ref_lang_count; lang_i++) {
            process(lang_i);
        }
    } else { // Otherwise inner loop will go parallel.
        for (unsigned int lang_i = 0; lang_i < ref_lang_count; lang_i++) {
            process(lang_i);
        }
    }
    time = (omp_get_wtime() - time);
    timer->add(time);
    fill_sorted_indexes();
}

void Detector::process(unsigned int lang_i) {
    LangStats* ref_stats = &refer_stats[lang_i];

    const LangStats::tr_int BINS_COUNT = LangStats::POSSIBILITIES - 1;
    double p = 1.0;

    #pragma omp parallel for reduction(*: p)
    
    for (LangStats::tr_int i = 1; i < BINS_COUNT; i++) {
    
        double dist_i = fabs(tst_stats->frequency(i) - ref_stats->frequency(i));
            
        p *= (1.0 - dist_i);
    }
    probabilities[lang_i] = p;
}

unsigned int Detector::get_saved_stats_langs(char*** langs) {
    const char* path = ".";
    DIR* directory;
    unsigned int count = 0;
    if ((directory = opendir(path))) {
        while ((readdir(directory))) {
            count++;
        }
    }
    if (count > 2 && (directory = opendir(path))) {
        (*langs) = new char*[count - 2];
        string suffix(LangStats::DAT_SUFFIX);
        dirent *file;
        unsigned int current = 0;
        while ((file = readdir(directory))) { // iterate through the directory
            string filename = string(file->d_name);
            int suffix_length = suffix.length(),
                    suffix_start = filename.length() - suffix.length(),
                    suffix_end = filename.length();
            if (filename.length() >= suffix.length() && !(filename.compare(suffix_start, suffix_end, suffix))) {

                filename.replace(suffix_start, suffix_length, string(""));
                (*langs)[current] = new char[5];
                strcpy((*langs)[current], filename.c_str());
                current++;
            }
        }
        return current;
    }
    return 0;
}

void Detector::fill_sorted_indexes() {
    memset(indexes, 0, ref_lang_count * sizeof (unsigned int));
    for (unsigned int i = 0; i < ref_lang_count; i++) {
        unsigned int left = 0;
        unsigned int larger = 0;

        unsigned int right = ref_lang_count - 1;
        unsigned int smaller = 0;

        for (unsigned int j = left; j < ref_lang_count; j++) {
            if (probabilities[j] > probabilities[i]) {
                larger++;
                left += (larger == left) ? 1 : 0;
            }
            if (probabilities[ref_lang_count - j - 1] > probabilities[ref_lang_count - i - 1]) {
                smaller++;
                right -= (smaller == right) ? 1 : 0;
            }
        }
        indexes[larger] = i;
        indexes[smaller] = i;
    }
}


Detector::entry Detector::operator[](unsigned int sorted_index) {
    entry e;
    unsigned int unsorted_index = indexes[sorted_index];
    e.lang = refer_stats[unsorted_index].lang;
    e.probability = probabilities[unsorted_index];
    return e;
}