#include "ShortestJobFirst.h"

/**< Used to test the algorithm to see if it is executing correctly */
void ShortestJobFirst::debugger()
{
    std::cout<<"Process ID: "<<currentProcess.getProcessID()<<"\n"
             <<"Current Operation: "<<currentProcess.getCurrentOperation()<<"\n"
             <<"Current Operation Time: "<<currentProcess.getCurrentOperationTime()<<"\n"
             <<"Current Operation  Arrival Time: "<<currentProcess.getArrivalTime()<<"\n"
             <<"Current Time: "<<timeElapsed<<"\n";
}

/**< This reads in the processes from the file containing the processes */
void ShortestJobFirst::recieveProcessInformationFromFile(const char* input)
{
    std::stringstream stream;
    std::string line;
    std::string processID;
    std::string operation;
    std::fstream fin(input, std::ios::in);
    /**< This reads in every  */
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
        /**< The arrival time is stored, then the first CPU burst is stored */
        object.setOperation();
        object.setOperation();
        /**< Inserts the newly created process object into the jobQueue (Arrival queue) */
        jobQueue.insert(object);
    }
    dataLogger.setTotalProcesses(jobQueue.size());
}

ShortestJobFirst::ShortestJobFirst(char* readyQueueLimit, char* contextSwitchDelaySetting):dataLogger(timeElapsed)
{
    std::istringstream stream(readyQueueLimit);
    /**< I use stringstream to parse the readyQueueLimit and contextSwitchDelaySetting variables */
    stream>>this->readyQueueLimit;
    stream.clear();
    stream.str(contextSwitchDelaySetting);
    stream>>this->contextSwitchDelaySetting;
    timeElapsed = 0;
    contextSwitchDelayTimer = 0;
    /**< If the ready queue limit is negative, the unlimited size flag is set to true*/
    if(this->readyQueueLimit < 0)
    {
        unlimitedSizeReadyQueue = true;
    }
    else
    {
        unlimitedSizeReadyQueue = false;
    }
}

/**<  */
void ShortestJobFirst::run()
{
    //char temp;
    /**< This will only stop looping when all queues are empty and the currently executing process has finished executing */
    while(currentProcess.getCurrentOperation() != Process::DONE || !jobQueue.empty() || !readyQueue.empty() || !ioQueue.empty())
    {
        timeElapsed++;
        tickCurrentProcess();
        tickIoQueueProcesses();
        addProcessesToReadyQueue();
        /**< When the currently runring process finishes doind CPU, if it needs to do IO, it gets put onto the IO queue */
        if(currentProcess.getCurrentOperation() == Process::IO)
        {
            ioQueue.push_back(currentProcess);
            /**< Blocking indicates that a context switch needs to be done */
            currentProcess.block();
        }
        /**< The run loop ending is determined by the last process in the system ending. If the last process blocks, the run loop will loop forever. That is why I have the second condition */
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
        /**< The second cpu status flag indicates that their is another CPU burst after the current one, so the process needs to go back onto the ready queue */
        if(currentProcess.getCurrentOperation() == Process::SECOND_CPU)
        {
            /**< The status flag needs to be changed back to CPU before the currently executing process can be put back onto the ready queue*/
            currentProcess.setCpuFlag();
            /**< As long as their is enough room on the ready queue or the queue is unbounded, then the process can be put onto the ready queue */
            if(unlimitedSizeReadyQueue  == true || readyQueue.size() < readyQueueLimit)
            {
                currentProcess.setReadyQueueArrivalTime(timeElapsed);
                readyQueue.push(currentProcess);
            }
            /**< Otherwise the currently executing process goes onto the "waiting for the ready queue" queue */
            else
            {
                currentProcess.setWaitingQueueArrivalTime(timeElapsed);
                waitingQueue.push(currentProcess);
            }
            currentProcess.block();
        }
        /**< If the currently executing process is blocked, then a context switch is performed */
        if(currentProcess.getCurrentOperation() == Process::BLOCKED)
        {
            performContextSwitch();
        }
       //debugger();
       //std::cin>>temp;
    }
    /**< If a process has finished executing, then it's data must be added to the data logger */
    if(currentProcess.getCurrentOperation() == Process::DONE)
    {
        dataLogger.addToTotalTurnaroundTime((timeElapsed - currentProcess.getArrivalTime()));
        dataLogger.addWaitingTime(currentProcess.getTotalWaitingTime());
        dataLogger.addToTotalCpuTime(currentProcess.getTotalCpuTime());
        dataLogger.addToTotalIoTime(currentProcess.getTotalIoTime());
        dataLogger.addTotalWaitingForReadyQueueTime(currentProcess.getTotalTimeWaitingForReadyQueue());
        std::cout<<"Process "<<currentProcess.getProcessID()<<" finished."<<"\n";
    }
    /**< Prints the results of the algorithm */
    printLog();
}


void ShortestJobFirst::tickCurrentProcess()
{
    if(currentProcess.getCurrentOperation() == Process::CPU)
    {
        currentProcess.tick();
    }
}

void ShortestJobFirst::tickIoQueueProcesses()
{
    for(auto i = ioQueue.begin(); i != ioQueue.end(); i++)
    {
        if(i->getCurrentOperation() == Process::IO)
        {
            i->tick();
        }
    }
}

void ShortestJobFirst::addProcessesToReadyQueue()
{
    /**< Adds processes from the "waiting for the ready queue" queue to the ready queue as long as the ready queue limit has not been exceeded */
    while(!waitingQueue.empty() && (unlimitedSizeReadyQueue  == true || readyQueue.size() < readyQueueLimit))
    {
        Process temporary = waitingQueue.front();
        waitingQueue.pop();
        temporary.setWaitingQueueDepartureTime(timeElapsed);
        temporary.calculateTimeWaitingForReadyQueue();
        temporary.setReadyQueueArrivalTime(timeElapsed);
        readyQueue.push(temporary);
    }
        /**< Adds processes from the IO queue to the ready queue as long as the ready queue limit has not been exceeded */
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
    /**< Adds processes from the job queue (arrival queue) to the ready queue as long as the ready queue limit has not been exceeded */
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

/**< All the processes' stats are printed out */
void ShortestJobFirst::printLog()
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

void ShortestJobFirst::performContextSwitch()
{
    /**< Performs context switch if the context switch timer is 0. If it isn't the timer is decremented. */
    if(!readyQueue.empty() && contextSwitchAllowed())
    {
        /**< When a new process is taken from the ready queue and made into the currently executing process, the currently executing process is no longer blocked and the context switch function is no longer executed */
        currentProcess = readyQueue.top();
        currentProcess.setReadyQueueDepartureTime(timeElapsed);
        currentProcess.calculateWaitingTime();
        readyQueue.pop();
        contextSwitchDelayTimer = contextSwitchDelaySetting;
    }
    /**< If the context Switch timer isn't zero, the CPU idle time is incremented */
    else
    {
        dataLogger.incrementCpuIdleTime();
    }
}

/**< Helper function used by perform context switch */
bool ShortestJobFirst::contextSwitchAllowed()
{
    if(contextSwitchDelayTimer == 0)
    {

        contextSwitchDelayTimer = contextSwitchDelaySetting;
        return true;
    }
    /**< As long as the context switch delay timer isn't zero, the next process isn't popped form the queue and the timer is decremented*/
    else
    {
        contextSwitchDelayTimer--;
        return false;
    }
}
