#ifndef PROCESSES_H_INCLUDED
#define PROCESSES_H_INCLUDED
#include <sstream>
#include <string>
#include <iostream>
#include <queue>
class Process
{
private:
    unsigned long long readyQueueArrivalTime;
    unsigned long long readyQueueDepartureTime;
    unsigned long long waitingQueueArrivalTime;
    unsigned long long waitingQueueDepartureTime;
    int currentOperationTime;
    int numberOfOperations;
    unsigned long long totalWaitingTime;
    int waitingTime;
    int currentOperation;
    unsigned long long totalTimeWaitingForReadyQueue;
    int timeWaitingForReadyQueue;
    int processID;
    unsigned int arrivalTime;
    unsigned long long turnAroundTime;
    unsigned long long totalCpuTime;
    unsigned long long totalIoTime;
    int currentComparator;
    long double history;
    int recentHistory;
    double alpha;
    std::queue <std::string> storage;
public:
    /**< Flag indicating the process is done */
    static const int DONE = 0;
    /**< Flag inidicating that the currently executing process is doing CPU */
    static const int CPU = 1;
    /**< Flag inidicating the currently executing process needs to do IO */
    static const int IO = 2;
    /**< Flag which indicates that a context switch needs to be performed */
    static const int BLOCKED = 3;
    /**< Flag which indicates that the currently executing process has another CPU burst after the current one */
    static const int SECOND_CPU = 4;
    /**< Flag which indicates that the process should be compared based arrival time */
    static const int ARRIVAL_TIME_COMPARATOR = 5;
    /**< Flag which indicates that the process should be compared based CPU burst length */
    static const int OPERATION_TIME_COMPARATOR = 6;
    /**< Flag which indicates that the process should be compared based on exponential average */
    static const int EXPONENTIAL_AVERAGE_COMPARATOR = 7;
    Process();
    Process(std::string);
    /**< Adds a string containing an operation name (ARRIVAL, CPU, IO) and operation time (arrival time, cpu burst, io burst) */
    void addOperation(std::string);
    /**< Parses an operation string to get the operation name and it's time */
    void setOperation();
    /**< Returns a value that represents a flag that represents the process state */
    int getCurrentOperation() const;
    /**< Returns the process ID of the process */
    int getProcessID() const;
    /**< Returns the current operation time of the current process (arrival time, cpu burst, io burst)*/
    int getCurrentOperationTime() const;
    /**< Return the arrival time of the process */
    unsigned int getArrivalTime() const;
    /**< Decrements the operation time (CPU burst, IO burst ) of the process */
    void tick();
    /**< Sets the process's state flag to BLOCKED */
    void block();
    /**< Stores the simulation time that the process arrived onto the ready queue */
    void setReadyQueueArrivalTime(unsigned long long);
    /**< Stores the simulation time that the process exited the ready queue */
    void setReadyQueueDepartureTime(unsigned long long);
    /**< Calculates the waiting time by subtracting ready queue departure and arrival times */
    void calculateWaitingTime();
    /**< Returns the total amount of time the process spent waiting on the ready queue */
    unsigned long long getTotalWaitingTime() const;
    bool operator<(const Process& rhs) const;
    bool operator<=(const Process& rhs) const;
    /**< Calculates the exponential average for this process */
    void calculateExponentialAverage();
    /**< Used to store the last CPU burst (used to calculate exponential average) */
    void storeRecentHistory();
    /**< Returns the exponential average */
    long double getHistory();
    /**< Tells the process what to compare by when it is compared using the < operator */
    void setCurrentComparator(int);
    /**< Used to set the process state flag to CPU after the second CPU flag has been set and before the process has blocked */
    void setCpuFlag();
    /**< Calculates the time spent on the "waiting to get on the ready queue" queue */
    void calculateTimeWaitingForReadyQueue();
    /**< Stores the simulation time when the process entered the "waiting to get on the ready queue" queue  */
    void setWaitingQueueArrivalTime(unsigned long long);
    /**< Stores the simulation time when the process exited the "waiting to get on the ready queue" queue  */
    void setWaitingQueueDepartureTime(unsigned long long);
    /**< Returns the total time the process spent waiting on the "waiting to get on the ready queue" queue */
    unsigned long long getTotalTimeWaitingForReadyQueue();
    /**< Returns the total amount of time the process spent doing CPU */
    unsigned long long getTotalCpuTime();
    /**< Returns the total amount of time the process spent doing IO */
    unsigned long long getTotalIoTime();
};
#endif
