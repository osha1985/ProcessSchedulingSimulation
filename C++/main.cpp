#include <string>
#include <iostream>
#include "RoundRobin.h"
#include "ShortestJobFirst.h"
#include "FirstComeFirstServe.h"
#include "ShortestJobFirstWithExponentialAveraging.h"
#include "ShortestRemainingJobFirst.h"

int main(int argc, char* argv[])
{

    if(argc >= 3)
    {
        std::string algorithmName = argv[1];
        if(algorithmName == "rr")
        {
            RoundRobin roundRobin(argv[3], argv[4], argv[5]);
            roundRobin.recieveProcessInformationFromFile(argv[2]);
            roundRobin.run();
        }
        else if(algorithmName == "fcfs")
        {
            FirstComeFirstServe firstComeFirstServe(argv[3], argv[4]);
            firstComeFirstServe.recieveProcessInformationFromFile(argv[2]);
            firstComeFirstServe.run();
        }
        else if(algorithmName == "sjf")
        {
            ShortestJobFirst shortestJobFirst(argv[3], argv[4]);
            shortestJobFirst.recieveProcessInformationFromFile(argv[2]);
            shortestJobFirst.run();
        }
        else if(algorithmName == "sjfwea")
        {
            ShortestJobFirstWithExponentialAveraging shortestJobFirstWithExponentialAveraging(argv[3], argv[4]);
            shortestJobFirstWithExponentialAveraging.recieveProcessInformationFromFile(argv[2]);
            shortestJobFirstWithExponentialAveraging.run();
        }
        else if(algorithmName == "srjf")
        {
            ShortestRemainingJobFirst shortestRemainingJobFirst(argv[3], argv[4]);
            shortestRemainingJobFirst.recieveProcessInformationFromFile(argv[2]);
            shortestRemainingJobFirst.run();
        }
    }
}
