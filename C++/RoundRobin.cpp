#include "RoundRobin.h"

RoundRobin::RoundRobin(char* readyQueueLimit, char* contextSwitchDelaySetting, char* timeQuantumSetting):FirstComeFirstServe(readyQueueLimit, contextSwitchDelaySetting)
{
    std::istringstream stream(timeQuantumSetting);
    stream>>this->timeQuantumSetting;
    this->timeQuantum = this->timeQuantumSetting;
}

void RoundRobin::run()
{
	//char temp;
    while(currentProcess.getCurrentOperation() != Process::DONE || !jobQueue.empty() || !readyQueue.empty() || !ioQueue.empty())
    {
        timeElapsed++;
        tickCurrentProcess();
        tickIoQueueProcesses();
        tickQuantum();
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
        /**< When the time quantum is equal to zero, a context switch is performed */
        if(timeQuantum <= 0 && currentProcess.getCurrentOperation() != Process::BLOCKED)
        {
            if(unlimitedSizeReadyQueue == true || readyQueue.size() < readyQueueLimit)
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

void RoundRobin::tickQuantum()
{
  timeQuantum--;
}

void RoundRobin::performContextSwitch()
{
    if(!readyQueue.empty() && contextSwitchAllowed())
    {
        currentProcess = readyQueue.front();
        currentProcess.setReadyQueueDepartureTime(timeElapsed);
        currentProcess.calculateWaitingTime();
        readyQueue.pop();
        contextSwitchDelayTimer = contextSwitchDelaySetting;
        /**< Resets the time quantum after the context switch is finished */
        timeQuantum = timeQuantumSetting;
    }
    else
    {
        dataLogger.incrementCpuIdleTime();
    }
}
