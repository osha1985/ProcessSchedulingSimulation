#include "ShortestJobFirstWithExponentialAveraging.h"
ShortestJobFirstWithExponentialAveraging::ShortestJobFirstWithExponentialAveraging(char* readyQueueLimit, char* contextSwitchDelaySetting):ShortestJobFirst(readyQueueLimit, contextSwitchDelaySetting)
{
}
/**< This is exactly like Shortest Job First except where noted */
void ShortestJobFirstWithExponentialAveraging::addProcessesToReadyQueue()
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
        if(i->getCurrentOperation() == Process::CPU  && (unlimitedSizeReadyQueue == true || readyQueue.size() < readyQueueLimit))
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
            /**< The priority queue compares based on exponential average */
            temporary.setCurrentComparator(Process::EXPONENTIAL_AVERAGE_COMPARATOR);
            temporary.setReadyQueueArrivalTime(timeElapsed);
            readyQueue.push(temporary);
            jobQueue.erase(jobQueue.begin());
        }
        else
        {
            Process temporary = (*jobQueue.begin());
            /**< The priority queue compares based on exponential average */
            temporary.setCurrentComparator(Process::EXPONENTIAL_AVERAGE_COMPARATOR);
            temporary.setWaitingQueueArrivalTime(timeElapsed);
            waitingQueue.push(temporary);
            jobQueue.erase(jobQueue.begin());
        }
    }
}


