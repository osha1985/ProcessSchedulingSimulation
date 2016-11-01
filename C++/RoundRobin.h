#ifndef ROUNDROBIN_H_INCLUDED
#define ROUNDROBIN_H_INCLUDED
#include "FirstComeFirstServe.h"

/**< Exactly like First Come First Serve except where inidicated */
class RoundRobin:public FirstComeFirstServe
{
private:
    int timeQuantum;
    int timeQuantumSetting;
public:
    RoundRobin(char*, char*, char*);
    void run();
    /**< Decrements the quantum time */
    void tickQuantum();
    void performContextSwitch();
};
#endif
