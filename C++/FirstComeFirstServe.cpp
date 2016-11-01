#include "FirstComeFirstServe.h"

void FirstComeFirstServe::debugger()
{
    std::cout<<"Process ID: "<<currentProcess.getProcessID()<<"\n"
             <<"Current Operation: "<<currentProcess.getCurrentOperation()<<"\n"
             <<"Current Operation Time: "<<currentProcess.getCurrentOperationTime()<<"\n"
             <<"Current Operation  Arrival Time: "<<currentProcess.getArrivalTime()<<"\n"
             <<"Current Time: "<<timeElapsed<<"\n";
}

void FirstComeFirstServe::printList()
{
    while(!(jobQueue.empty()))
    {
        std::cout<<jobQueue.begin()->getProcessID()<<" "<<jobQueue.begin()->getArrivalTime()<<" "<<"\n";
        jobQueue.erase(jobQueue.begin());
    }
}

void FirstComeFirstServe::recieveProcessInformationFromFile(const char* input)
{
    std::stringstream stream;
    std::string line;
    std::string processID;
    std::string operation;
    std::fstream fin(input, std::ios::in);
    while(getline(fin,line))
    {
        stream.str(line);
        stream.ignore(1);
        stream>>processID;
        Process object(processID);
        while(stream>>operation)
        {
            object.addOperation(operation);
        }
        stream.clear();
        object.setOperation();
        object.setOperation();
        jobQueue.insert(object);
    }
    dataLogger.setTotalProcesses(jobQueue.size());
}

FirstComeFirstServe::FirstComeFirstServe(char* readyQueueLimit, char* contextSwitchDelaySetting):dataLogger(timeElapsed)
{
    std::istringstream stream(readyQueueLimit);
    stream>>this->readyQueueLimit;
    stream.clear();
    stream.str(contextSwitchDelaySetting);
    stream>>this->contextSwitchDelaySetting;
    timeElapsed = 0;
    contextSwitchDelayTimer = 0;
    if(this->readyQueueLimit < 0)
    {
        unlimitedSizeReadyQueue = true;
    }
    else
    {
        unlimitedSizeReadyQueue = false;
    }
}

void FirstComeFirstServe::run()
{
    //char temp;
    while(currentProcess.getCurrentOperation() != Process::DONE || !jobQueue.empty() || !readyQueue.empty() || !ioQueue.empty())
    {
        timeElapsed++;
        tickCurrentProcess();
        tickIoQueueProcesses();
        addProcessesToReadyQueue();
        if(currentProcess.getCurrentOperation() == Process::IO)
        {
            ioQueue.push_back(currentProcess);
            currentProcess.block();
        }
        if(currentProcess.getCurrentOperation() == Process::DONE && (!jobQueue.empty() || !readyQueue.empty() || !ioQueue.empty()))
        {
            dataLogger.addToTotalTurnaroundTime(timeElapsed - currentProcess.getArrivalTime());
            dataLogger.addWaitingTime(currentProcess.getTotalWaitingTime());
            dataLogger.addToTotalCpuTime(currentProcess.getTotalCpuTime());
            dataLogger.addToTotalIoTime(currentProcess.getTotalIoTime());
            dataLogger.addTotalWaitingForReadyQueueTime(currentProcess.getTotalTimeWaitingForReadyQueue());
            currentProcess.block();
            std::cout<<"Process "<<currentProcess.getProcessID()<<" finished."<<"\n";
        }
        if(currentProcess.getCurrentOperation() == Process::SECOND_CPU)
        {
            currentProcess.setCpuFlag();
            if(unlimitedSizeReadyQueue  == true || readyQueue.size() < readyQueueLimit)
            {
                currentProcess.setReadyQueueArrivalTime(timeElapsed);
                readyQueue.push(currentProcess);
            }
            else
            {
                currentProcess.setWaitingQueueArrivalTime(timeElapsed);
                waitingQueue.push(currentProcess);
            }
            currentProcess.block();
        }
        if(currentProcess.getCurrentOperation() == Process::BLOCKED)
        {
            performContextSwitch();
        }
        //debugger();
        //std::cin>>temp;
    }
    if(currentProcess.getCurrentOperation() == Process::DONE)
    {
        dataLogger.addToTotalTurnaroundTime((timeElapsed - currentProcess.getArrivalTime()));
        dataLogger.addWaitingTime(currentProcess.getTotalWaitingTime());
        dataLogger.addToTotalCpuTime(currentProcess.getTotalCpuTime());
        dataLogger.addToTotalIoTime(currentProcess.getTotalIoTime());
        dataLogger.addTotalWaitingForReadyQueueTime(currentProcess.getTotalTimeWaitingForReadyQueue());
        std::cout<<"Process "<<currentProcess.getProcessID()<<" finished."<<"\n";
    }
    printLog();
}

void FirstComeFirstServe::tickCurrentProcess()
{
    if(currentProcess.getCurrentOperation() == Process::CPU)
    {
        currentProcess.tick();
    }
}

void FirstComeFirstServe::tickIoQueueProcesses()
{
    for(auto i = ioQueue.begin(); i != ioQueue.end(); i++)
    {
        if(i->getCurrentOperation() == Process::IO)
        {
            i->tick();
        }
    }
}

void FirstComeFirstServe::addProcessesToReadyQueue()
{
    while(!waitingQueue.empty() && (unlimitedSizeReadyQueue  == true || readyQueue.size() < readyQueueLimit))
    {
        Process temporary = waitingQueue.front();
        waitingQueue.pop();
        temporary.setWaitingQueueDepartureTime(timeElapsed);
        temporary.calculateTimeWaitingForReadyQueue();
        temporary.setReadyQueueArrivalTime(timeElapsed);
        readyQueue.push(temporary);
    }
    for(auto i = ioQueue.begin(); i != ioQueue.end();)
    {
        if(i->getCurrentOperation() == Process::CPU && (unlimitedSizeReadyQueue == true || readyQueue.size() < readyQueueLimit))
        {
            i->setReadyQueueArrivalTime(timeElapsed);
            readyQueue.push(*i);
            i = ioQueue.erase(i);
        }
        else if(i->getCurrentOperation() == Process::CPU)
        {
            i->setWaitingQueueArrivalTime(timeElapsed);
            waitingQueue.push(*i);
            i = ioQueue.erase(i);
        }
        else
        {
            i++;
        }
    }
    while(!jobQueue.empty() && jobQueue.begin()->getArrivalTime() <= timeElapsed)
    {
        if (unlimitedSizeReadyQueue == true || readyQueue.size() < readyQueueLimit)
        {
            Process temporary = (*jobQueue.begin());
            temporary.setCurrentComparator(Process::OPERATION_TIME_COMPARATOR);
            temporary.setReadyQueueArrivalTime(timeElapsed);
            readyQueue.push(temporary);
            jobQueue.erase(jobQueue.begin());
        }
        else
        {
            Process temporary = (*jobQueue.begin());
            temporary.setCurrentComparator(Process::OPERATION_TIME_COMPARATOR);
            temporary.setWaitingQueueArrivalTime(timeElapsed);
            waitingQueue.push(temporary);
            jobQueue.erase(jobQueue.begin());
        }
    }
}

void FirstComeFirstServe::printLog()
{
    std::cout<<std::fixed
             <<"Results:"<<"\n"
             <<"CPU Utilization: "<<dataLogger.getCpuUtilization()<<"\n"
             <<"Average Time Spent Doing IO: "<<dataLogger.getMeanIoTime()<<"\n"
             <<"Average Time Spent Doing CPU: "<<dataLogger.getMeanCpuTime()<<"\n"
             <<"Average Time Waiting To Get Onto Ready Queue: "<<dataLogger.getMeanTimeWaitingForTheReadyQueue()<<"\n"
             <<"Total Time Spent Doing CPU: "<<dataLogger.getTotalCpuTime()<<"\n"
             <<"Total Time Spent Doing IO: "<<dataLogger.getTotalIoTime()<<"\n"
             <<"Total Time Waiting To Get Onto Ready Queue: "<<dataLogger.getTotalTimeWaitingForTheReadyQueue()<<"\n"
             <<"Total Waiting Time: "<<dataLogger.getTotalWaitingTime()<<"\n"
             <<"Total Turnaround Time: "<<dataLogger.getTotalTurnAroundTime()<<"\n"
             <<"Throughput: "<<dataLogger.getThroughput()<<" processes per 1000 time units"<<"\n"
             <<"Average turnaround time: "<<dataLogger.getAverageTurnaroundTime()<<"\n"
             <<"Waiting Time Statistics: "<<"\n"
             <<"Mean: "<<dataLogger.getMeanWaitingTime()<<"\n"
             <<"Median: "<<dataLogger.getMedianWaitingTime()<<"\n"
             <<"Standard Deviation: "<<dataLogger.getStandardDeviationOfWaitingTime()<< "\n\n";

    std::ofstream fout("output.txt", std::ios::app);
    fout<<std::fixed
        <<"Results:"<<"\n"
        <<"CPU Utilization: "<<dataLogger.getCpuUtilization()<<"\n"
        <<"Average Time Spent Doing IO: "<<dataLogger.getMeanIoTime()<<"\n"
        <<"Average Time Spent Doing CPU: "<<dataLogger.getMeanCpuTime()<<"\n"
        <<"Average Time Waiting To Get Onto Ready Queue: "<<dataLogger.getMeanTimeWaitingForTheReadyQueue()<<"\n"
        <<"Total Time Spent Doing CPU: "<<dataLogger.getTotalCpuTime()<<"\n"
        <<"Total Time Spent Doing IO: "<<dataLogger.getTotalIoTime()<<"\n"
        <<"Total Time Waiting To Get Onto Ready Queue: "<<dataLogger.getTotalTimeWaitingForTheReadyQueue()<<"\n"
        <<"Total Waiting Time: "<<dataLogger.getTotalWaitingTime()<<"\n"
        <<"Total Turnaround Time: "<<dataLogger.getTotalTurnAroundTime()<<"\n"
        <<"Throughput: "<<dataLogger.getThroughput()<<" processes per 1000 time units"<<"\n"
        <<"Average turnaround time: "<<dataLogger.getAverageTurnaroundTime()<<"\n"
        <<"Waiting Time Statistics: "<<"\n"
        <<"Mean: "<<dataLogger.getMeanWaitingTime()<<"\n"
        <<"Median: "<<dataLogger.getMedianWaitingTime()<<"\n"
        <<"Standard Deviation: "<<dataLogger.getStandardDeviationOfWaitingTime()<< "\n\n";

}

void FirstComeFirstServe::performContextSwitch()
{
    if(!readyQueue.empty() && contextSwitchAllowed())
    {
        currentProcess = readyQueue.front();
        currentProcess.setReadyQueueDepartureTime(timeElapsed);
        currentProcess.calculateWaitingTime();
        readyQueue.pop();
        contextSwitchDelayTimer = contextSwitchDelaySetting;
    }
    else
    {
        dataLogger.incrementCpuIdleTime();
    }
}

bool FirstComeFirstServe::contextSwitchAllowed()
{
    if(contextSwitchDelayTimer == 0)
    {
        contextSwitchDelayTimer = contextSwitchDelaySetting;
        return true;
    }
    else
    {
        contextSwitchDelayTimer--;
        return false;
    }
}
