#ifndef RUDIS_TIMER_HPP
#define RUDIS_TIMER_HPP
#include <cstdint>
#include <functional>

class Timer{
public:
    Timer(uint64_t expire, std::function<void(void)> cb, 

};

#endif