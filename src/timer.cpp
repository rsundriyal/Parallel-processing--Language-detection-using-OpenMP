/*
 * File:   timer.cpp
 * Author: CMPE 275 Gash Team - Ravi,Sarthak & Promod
 *
 */

#include "../headers/timer.h"

Timer::Timer() {
        format = 'h';
        delimiter = '\n';

}

Timer* Timer::timer = NULL;

Timer* Timer::instance() {
    if (timer == NULL)
        timer = new Timer();
    return Timer::timer;
}

void Timer::add(double interval) {
    #pragma omp atomic
    time += interval;
}

std::ostream& operator<<(std::ostream& os, const Timer & timer) {
    switch (timer.format) {
        case 'h':
        default:
            os << timer.time * 1000.0 << "ms";
            break;
    }
    return os;
}
