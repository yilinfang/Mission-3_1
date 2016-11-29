#ifndef MUTEX_H_INCLUDED
#define MUTEX_H_INCLUDED
#include <windows.h>
class Mutex
{
public:
    Mutex(void);
    ~Mutex(void);
    void Lock();
    void UnLock();
private:
    CRITICAL_SECTION m_criticalSection;
};
#endif // MUTEX_H_INCLUDED
