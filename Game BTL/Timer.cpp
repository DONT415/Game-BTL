#include "Timer.h"

TIMER::TIMER()
{
    start_ticks_ = 0;
    started_ = false;
}
TIMER::~TIMER()
{

}

void TIMER::start()
{
    started_ = true;
    start_ticks_ = SDL_GetTicks();
}

void TIMER::restart()
{
    start_ticks_ = SDL_GetTicks();
}

int TIMER::getTicks()
{
    if (!started_) return 0;
    return SDL_GetTicks() - start_ticks_;
}
