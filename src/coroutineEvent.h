#ifndef CFD78E68_4D46_42A8_BDBD_F5C68657806A
#define CFD78E68_4D46_42A8_BDBD_F5C68657806A

#include "osDependent.h"

class coroutineEvent
{
public:
    coroutineEvent()
    {
        sem.acquire();
    }
    ~coroutineEvent()
    {
        sem.release();
    }

private:
    friend class coroutineBase;
    static inline semaphore sem;
};

#endif /* CFD78E68_4D46_42A8_BDBD_F5C68657806A */
