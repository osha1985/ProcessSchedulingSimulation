#include "DataLogger.h"

DataLogger::DataLogger(unsigned long long& timeElapsed):timer(timeElapsed)
{
    totalProcessesLoaded = 0;
    totalTurnaroundTime = 0;
    cpuIdleTime = 0;
    totalWaitingTime = 0;
    totalCpuTime = 0;
    totalIoTime = 0;
    totalTimeWaitingForTheReadyQueueTime = 0;
}

void DataLogger::addToTotalTurnaroundTime(unsigned long long time)
{
    totalTurnaroundTime += time;
}

void DataLogger::addWaitingTime(unsigned long long time)
{
    waitingTimes.insert(time);
    totalWaitingTime += time;
}

long double DataLogger::getAverageTurnaroundTime()
{
    return static_cast<long double>(totalTurnaroundTime) / totalProcessesLoaded;
}

int DataLogger::getCpuIdleTime()
{
    return cpuIdleTime;
}

long double DataLogger::getCpuUtilization()
{
    return (timer - cpuIdleTime) / static_cast<long double>(timer) * 100;
}

long double DataLogger::getMeanWaitingTime()
{
    return static_cast<long double>(totalWaitingTime) / totalProcessesLoaded;
}

void DataLogger::setTotalProcesses(int processes)
{
    totalProcessesLoaded = processes;
}

void DataLogger::incrementCpuIdleTime()
{
    cpuIdleTime++;
}

long double DataLogger::getThroughput()
{
    return static_cast<long double>(totalProcessesLoaded) / (timer / 1000.0);
}

long double DataLogger::getStandardDeviationOfWaitingTime()
{
    long double accumulator = 0.0;
    for(auto i = waitingTimes.begin(); i != waitingTimes.end(); i++)
    {
        accumulator += (*i - getMeanWaitingTime()) * (*i - getMeanWaitingTime());
    }
    return sqrt(accumulator / totalProcessesLoaded - 1);
}

long double DataLogger::getMedianWaitingTime()
{
    long double m = 0, n = 0;
    auto iter = waitingTimes.begin();
    for(int i = 1; i < (totalProcessesLoaded - 1) / 2; i++)
    {
        iter++;
    }
    if(totalProcessesLoaded % 2 == 1)
    {
        return *iter;
    }
    else
    {
        m = *iter;
        iter++;
        n = *iter;
        return (m + n / 2);
    }
}


void DataLogger::addToTotalIoTime(unsigned long long ioTime)
{
    totalIoTime+=ioTime;
}

void DataLogger::addToTotalCpuTime(unsigned long long cpuTime)
{
    totalCpuTime+=cpuTime;
}

void DataLogger::addTotalWaitingForReadyQueueTime(unsigned long long waitingForReadyQueue)
{
    totalTimeWaitingForTheReadyQueueTime+=waitingForReadyQueue;
}

long double DataLogger::getMeanCpuTime()
{
    return static_cast<long double>(totalCpuTime)/totalProcessesLoaded;
}

long double DataLogger::getMeanIoTime()
{
    return static_cast<long double>(totalIoTime)/totalProcessesLoaded;
}

long double DataLogger::getMeanTimeWaitingForTheReadyQueue()
{
    return static_cast<long double>(totalTimeWaitingForTheReadyQueueTime)/totalProcessesLoaded;
}

unsigned long long DataLogger::getTotalCpuTime()
{
    return totalCpuTime;
}

unsigned long long DataLogger::getTotalIoTime()
{
    return totalIoTime;
}

unsigned long long DataLogger::getTotalTimeWaitingForTheReadyQueue()
{
    return totalTimeWaitingForTheReadyQueueTime;
}

unsigned long long DataLogger::getTotalTurnAroundTime()
{
    return totalTurnaroundTime;
}

unsigned long long DataLogger::getTotalWaitingTime()
{
    return totalWaitingTime;
}
