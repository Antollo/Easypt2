#ifndef COROUTINEEVENT_H
#define COROUTINEEVENT_H

#include <semaphore>
#include <limits>

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
    static inline std::counting_semaphore<std::numeric_limits<int>::max()> sem{0};
};

#endif /* COROUTINEEVENT_H */
