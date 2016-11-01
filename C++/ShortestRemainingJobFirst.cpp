#include "ShortestRemainingJobFirst.h"

ShortestRemainingJobFirst::ShortestRemainingJobFirst(char* readyQueueLimit, char* contextSwitchDelaySetting):ShortestJobFirst(readyQueueLimit, contextSwitchDelaySetting)
{
}

/**< This is written exactly like in shortest job first except where indicated */
void ShortestRemainingJobFirst::run()
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
        /**< If the top of the priority queue is less than the currently executing process, then the currently executing process is pushed onto the ready queue and a context switch is performed*/
        /**<  Current process is < top because I overloaded the < operator such that if < is true, it returns false*/
        if(currentProcess.getCurrentOperation() != Process::BLOCKED && !readyQueue.empty() &&  currentProcess < (readyQueue.top()))
        {
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
       // debugger();
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
