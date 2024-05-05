#ifndef RUDIS_TIMER_HPP
#define RUDIS_TIMER_HPP
#include <cstdint>
#include <functional>

class Timer{
public:
    Timer(uint64_t expire, std::function<void(void)> cb, void* args)
        :m_expire(expire), m_cb(cb){}
    inline void activate() {m_cb();}
    inline uint64_t getExpire() const {return m_expire;}
private:
    uint64_t m_expire;
    std::function<void(void)> m_cb;
};

#endif