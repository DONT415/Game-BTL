#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include "Commonfunc.h"

class TIMER
{
public:
    TIMER();
    ~TIMER();

    void start();
    void restart();
    int  getTicks();

private:
    int start_ticks_;
    bool started_;

};

#endif // TIMER_H_INCLUDED
#pragma once
