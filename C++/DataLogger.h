#ifndef DATALOGGER_H_INCLUDED
#define DATALOGGER_H_INCLUDED
#include <set>
#include <cmath>
#include <iostream>
class DataLogger
{
private:
    unsigned long long& timer;
    unsigned long long totalCpuTime;
    unsigned long long totalIoTime;
    unsigned long long totalTimeWaitingForTheReadyQueueTime;
    int totalProcessesLoaded;
    unsigned long long totalTurnaroundTime;
    int cpuIdleTime;
    unsigned long long totalWaitingTime;
    std::multiset<int>waitingTimes;
public:
    DataLogger(unsigned long long&);
    /**< Stores the total amount of processes in the simulation */
    void setTotalProcesses(int);
    /**< Adds the finsihed process' turnaround time to the total */
    void addToTotalTurnaroundTime(unsigned long long);
    /**< Increments the amount of time spent where a process isn't execting it's CPU burst */
    void incrementCpuIdleTime();
    long double getCpuUtilization();
    long double getThroughput();
    long double getAverageTurnaroundTime();
    long double getMeanWaitingTime();
    long double getMedianWaitingTime();
    long double getStandardDeviationOfWaitingTime();
    void addWaitingTime(unsigned long long);
    int getCpuIdleTime();
    void addWaitingForReadyQueueTime(unsigned long long);
    void addToTotalIoTime(unsigned long long);
    void addToTotalCpuTime(unsigned long long);
    void addTotalWaitingForReadyQueueTime(unsigned long long);
    long double getMeanCpuTime();
    long double getMeanIoTime();
    long double getMeanTimeWaitingForTheReadyQueue();
    unsigned long long getTotalCpuTime();
    unsigned long long getTotalIoTime();
    unsigned long long getTotalTimeWaitingForTheReadyQueue();
    unsigned long long getTotalTurnAroundTime();
    unsigned long long getTotalWaitingTime();
};

#endif
