#ifndef RUDIS_TIMER_MANAGER_HPP
#define RUDIS_TIMER_MANAGER_HPP

#include "timer.hpp"

class TimerManager{
public:
    TimerManager(){}

    Timer *addTimer(int timeout, std::function<void(void)> cb, void* args = nullptr);
    void delTimer(Timer* timer);
    uint64_t getNextExpireTime();
};


#endif