#include "main.h"

#define CNT_OVR 0x10000

class Data
{
public:
    uint32_t m;
    uint32_t n;
};

class DBuf
{
private:
    
    Data *p0;
    
    Data *p;

    uint32_t m_size;

    uint32_t cnt;
    bool full;
public:
    bool is_ready;
    uint32_t c0;

    DBuf():p0(0), p(0), m_size(0), cnt(0), full(false),
            is_ready(false), c0(0) {}

    DBuf(uint32_t _size):p0(0), p(0), m_size(0), cnt(0),
            full(false), is_ready(false), c0(0)
    {
        if(_size != 0)
        {
            p = p0 = new Data[_size];
            m_size = _size;
        }
    }

    void clear() volatile
    {
        delete[] p0;
        p0 = 0;
        p = 0;
        m_size = 0;
        cnt = 0;
        full = false;
        is_ready = false;
        c0 = 0;
    }

    void reset(uint32_t _size) volatile
    {
        clear();
        if(_size != 0)
        {
            p = p0 = new Data[_size];
            m_size = _size;
        }
    }

    inline bool is_full() const volatile {return full;}

    inline uint32_t get_cnt() const volatile {return cnt;}

    uint32_t size() const volatile
    {
        return (full) ? m_size : p-p0;
    }

    inline uint32_t max_size() const volatile {return m_size;}

    void push(const Data &d) volatile
    {
        if(m_size != 0)
        {
            *p = d;
            ++p;
            if(p >= p0 + m_size)
            {
                full = true;
                p = p0;
            }
            cnt++;
        }
    }
    Data *first() volatile
    {
        if(!full)
            return (p == p0) ? 0 : p0;

        return p;
    }
    
    Data *last() volatile
    {
        if(!full)
            return (p == p0) ? 0 : p-1;
        Data *px = p - 1;
        if(px < p0)
            px += m_size;
        return px;
    }
};

void _update_fm_vars();
void fm_init();
void fm_stop();
bool fm_start(uint32_t timer_div1, uint32_t timer_div2, uint32_t k);
bool get_result(uint32_t &m, uint32_t &n, uint32_t &cnt, uint32_t &c0);
