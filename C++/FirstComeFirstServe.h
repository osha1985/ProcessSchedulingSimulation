#ifndef FIRSTCOMEFIRSTSERVE_H_INCLUDED
#define FIRSTCOMEFIRSTSERVE_H_INCLUDED
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include "Process.h"
#include "DataLogger.h"

/**< First come first serve operates exactly like shortest job first except it doesn't use a priority queue. It doesn't need one since it doesn't need to select the process with the shortest cpu burst. */
class FirstComeFirstServe
{
protected:
    unsigned long long timeElapsed;
    std::multiset<Process>jobQueue;
    std::vector<Process>ioQueue;
    std::queue<Process>readyQueue;
    std::queue<Process>waitingQueue;
    bool unlimitedSizeReadyQueue;
    Process currentProcess;
    int contextSwitchDelayTimer;
    int contextSwitchDelaySetting;
    int readyQueueLimit;
    DataLogger dataLogger;
public:
    FirstComeFirstServe(char*, char*);
    void recieveProcessInformationFromFile(const char*);
    void printList();
    virtual void run();
    virtual void addProcessesToReadyQueue();
    void tickCurrentProcess();
    void tickIoQueueProcesses();
    void printLog();
    void performContextSwitch();
    bool contextSwitchAllowed();
    void debugger();
};


#endif // SHORTESTJOBFIRST_H_INCLUDED
