#include "Process.h"

Process::Process()
{
    processID = -1;
    totalWaitingTime = 0;
    waitingTime = 0;
    readyQueueArrivalTime = 0;
    readyQueueDepartureTime = 0;
    waitingQueueArrivalTime = 0;
    waitingQueueDepartureTime = 0;
    totalTimeWaitingForReadyQueue = 0;
    timeWaitingForReadyQueue = 0;
    arrivalTime = -1;
    currentOperationTime = 0;
    recentHistory = 0;
    history = 10;
    alpha = 0.5;
    totalIoTime = 0;
    totalCpuTime = 0;
    currentOperation = Process::BLOCKED;
    currentComparator = ARRIVAL_TIME_COMPARATOR;
}

Process::Process(std::string processID)
{
    std::stringstream stream(processID);
    stream>>this->processID;
    totalWaitingTime = 0;
    waitingTime = 0;
    readyQueueArrivalTime = 0;
    readyQueueDepartureTime = 0;
    waitingQueueArrivalTime = 0;
    waitingQueueDepartureTime = 0;
    totalTimeWaitingForReadyQueue = 0;
    timeWaitingForReadyQueue = 0;
    arrivalTime = -1;
    currentOperationTime = 0;
    recentHistory = 0;
    history = 10;
    alpha = 0.5;
    totalIoTime = 0;
    totalCpuTime = 0;
    currentOperation = Process::BLOCKED;
    currentComparator = ARRIVAL_TIME_COMPARATOR;
}

void Process::setOperation()
{
    if(storage.empty())
    {
        currentOperation = DONE;
        return;
    }
    std::string operation = storage.front();
    storage.pop();
    std::istringstream buffer(operation);
    getline(buffer,operation,':');
    if(operation == "ARRIVED")
    {
        buffer>>arrivalTime;
    }
    else if(operation == "CPU")
    {
        calculateExponentialAverage();
        buffer>>currentOperationTime;
        currentOperation = CPU;
        storeRecentHistory();
        totalCpuTime+=currentOperationTime;
    }
    else if(operation == "IO")
    {
        buffer>>currentOperationTime;
        currentOperation = IO;
        totalIoTime+=currentOperationTime;
    }
}

void Process::addOperation(std::string operation)
{
    storage.push(operation);
}

int Process::getCurrentOperation() const
{
    return currentOperation;
}

int Process::getCurrentOperationTime() const
{
    return currentOperationTime;
}

int Process::getProcessID() const
{
    return processID;
}

unsigned int Process::getArrivalTime() const
{
    return arrivalTime;
}

void Process::tick()
{
    if(currentOperation != Process::DONE && currentOperation != Process::BLOCKED)
    {
        currentOperationTime--;
        if(currentOperationTime == 0)
        {
            if(currentOperation == CPU)
            {
                setOperation();
                if(currentOperation == CPU)
                {
                    currentOperation = SECOND_CPU;
                }
            }
            else
            {
                setOperation();
            }
        }
    }
}

void Process::block()
{
    currentOperation = Process::BLOCKED;
}


unsigned long long Process::getTotalWaitingTime() const
{
    return totalWaitingTime;
}

bool Process::operator<(const Process& rhs) const
{
    switch(currentComparator)
    {
    case ARRIVAL_TIME_COMPARATOR:
        return arrivalTime < rhs.arrivalTime;
        break;
    case OPERATION_TIME_COMPARATOR:
        return currentOperationTime < rhs.currentOperationTime ? false : true;
        break;
    case EXPONENTIAL_AVERAGE_COMPARATOR:
        return history < rhs.history ? false : true;
        break;
    default:
        return arrivalTime < rhs.arrivalTime;
    }
}

void Process::calculateExponentialAverage()
{
    /**< The formula for calculating exponential average */
    history = (alpha * recentHistory + (1 - alpha) * history);
}

void Process::storeRecentHistory()
{
    if(currentOperation == Process::CPU)
    {
        recentHistory = currentOperationTime;
    }
}

long double Process::getHistory()
{
    return history;
}

void Process::setCurrentComparator(int comparator)
{
    currentComparator = comparator;
}


bool Process::operator<=(const Process& rhs) const
{
    switch(currentComparator)
    {
    case ARRIVAL_TIME_COMPARATOR:
        return arrivalTime <= rhs.arrivalTime;
        break;
    case OPERATION_TIME_COMPARATOR:
        return currentOperationTime <= rhs.currentOperationTime;
        break;
    case EXPONENTIAL_AVERAGE_COMPARATOR:
        return history <= rhs.history;
        break;
    default:
        return arrivalTime <= rhs.arrivalTime;
    }
}

void Process::setReadyQueueArrivalTime(unsigned long long timeElapsed)
{
    readyQueueArrivalTime = timeElapsed;
}

void Process::setReadyQueueDepartureTime(unsigned long long timeElapsed)
{
    readyQueueDepartureTime = timeElapsed;
}

void Process::setWaitingQueueArrivalTime(unsigned long long timeElapsed)
{
    waitingQueueArrivalTime = timeElapsed;
}

void Process::setWaitingQueueDepartureTime(unsigned long long timeElapsed)
{
    waitingQueueDepartureTime = timeElapsed;
}

void Process::calculateTimeWaitingForReadyQueue()
{
    timeWaitingForReadyQueue = (waitingQueueDepartureTime - waitingQueueArrivalTime);
    totalTimeWaitingForReadyQueue += timeWaitingForReadyQueue;
    waitingQueueDepartureTime = 0;
    waitingQueueArrivalTime = 0;
}

void Process::calculateWaitingTime()
{
    waitingTime = (readyQueueDepartureTime - readyQueueArrivalTime);
    totalWaitingTime += waitingTime;
}

void Process::setCpuFlag()
{
    currentOperation = CPU;
}

unsigned long long Process::getTotalCpuTime()
{
    return totalCpuTime;
}

unsigned long long Process::getTotalIoTime()
{
    return totalIoTime;
}

unsigned long long Process::getTotalTimeWaitingForReadyQueue()
{
    return totalTimeWaitingForReadyQueue;
}
