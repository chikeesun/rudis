#ifndef RUDIS_TIMER_HPP
#define RUDIS_TIMER_HPP
#include <cstdint>

class Timer{
public:
    Timer(uint64_t expire, std::function<void(void)> cb):expire(expire), void* args);

};

#endif