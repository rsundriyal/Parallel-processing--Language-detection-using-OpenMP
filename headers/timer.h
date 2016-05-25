/*
 * File:   timer.h
 * Author: CMPE 275 Gash Team - Ravi,Sarthak & Promod
 *
 */

#include <cstdlib>
#include <iostream>

#ifndef TIMER_H
#define	TIMER_H

class Timer {
private:
    static Timer* timer;
    double time;
    char format;
    static const char default_format;
    Timer();

public:
    char delimiter;
    static Timer* instance();
    void add(double interval);

    friend std::ostream& operator<<(std::ostream& os, const Timer& timer);
};

#endif

