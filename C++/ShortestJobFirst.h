#ifndef SHORTESTJOBFIRST_H_INCLUDED
#define SHORTESTJOBFIRST_H_INCLUDED
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <queue>
#include "Process.h"
#include "DataLogger.h"
class ShortestJobFirst
{
protected:
    /**< Stores the simulation time */
    unsigned long long timeElapsed;
    std::multiset<Process>jobQueue;
    std::vector<Process>ioQueue;
    std::priority_queue <Process> readyQueue;
    std::queue <Process> waitingQueue;
    Process currentProcess;
    int contextSwitchDelayTimer;
    int contextSwitchDelaySetting;
    int readyQueueLimit;
    bool unlimitedSizeReadyQueue;
    DataLogger dataLogger;
public:
    /**< Constructor which recieves command line arguments */
    ShortestJobFirst(char*, char*);
    /**< Recieves the process information form the input file */
    void recieveProcessInformationFromFile(const char*);
    /**< starts the simulation */
    virtual void run();
    /**< Adds the processes to the ready queue */
    virtual void addProcessesToReadyQueue();
    /**< Decrements the CPU burst of the currently executing process */
    void tickCurrentProcess();
    /**< Decrements the IO bursts of the processes on the IO queue */
    void tickIoQueueProcesses();
    /**< Print the scheduling algorithm results */
    void printLog();
    /**< When executed, this function performs a context switch */
    virtual void performContextSwitch();
    /**< Helper function for performContextSwitch */
    bool contextSwitchAllowed();
    /**< Used for debugging. Prints out the state of the currently executing function */
    void debugger();
};

#endif // SHORTESTJOBFIRST_H_INCLUDED
