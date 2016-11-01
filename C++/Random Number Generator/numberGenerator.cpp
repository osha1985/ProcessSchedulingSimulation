#include <vector>
#include <iostream>
#include <string>
#include <chrono>
#include <random>
#include <algorithm>
#include <fstream>

struct burst
{
     enum
     {
          CPU = 0,
          IO
     };
     int type;
     int time;
};

int main()
{
     std::vector<int> distribution;
     std::vector<burst> result;
     std::ofstream outfile("output.txt");
     int numberOfCpuBursts;
     int numberOfIoBursts;
     int arrivalTime;
     burst temporaryBurst;
     unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
     std::mt19937 generator(seed);
     std::normal_distribution<double> cpuBurstsDistribution(750, 50);
     std::normal_distribution<double> ioBurstsDistribution(250, 50);
     for (int i=0; i<1000; ++i)
     {
          numberOfCpuBursts = cpuBurstsDistribution(generator);
          numberOfIoBursts = ioBurstsDistribution(generator);
          //fill the vector with as many 1's as io bursts
          distribution.insert(distribution.end(), numberOfIoBursts, 1);

          //add 1 less then cpubursts of 0's to the vector
          //the final cpuburst will be added at the end
          distribution.insert(distribution.end(), numberOfCpuBursts - numberOfIoBursts - 1, 0);
          std::shuffle(distribution.begin(), distribution.end(), generator);
          for (auto iter = distribution.begin(); iter != distribution.end(); ++iter)
          {
               temporaryBurst.type = burst::CPU;
               temporaryBurst.time = generator() % 955 + 5;
               result.push_back(temporaryBurst);
               if (*iter == burst::IO)
               {
                    temporaryBurst.type = burst::IO;
                    temporaryBurst.time = generator() % 9500 + 500;
                    result.push_back(temporaryBurst);
               }
          }
          //now we add the final cpu burst
          temporaryBurst.type = burst::CPU;
          temporaryBurst.time = generator() % 955 + 5;
          result.push_back(temporaryBurst);
          arrivalTime = generator() % 1000;
          outfile << "P" << i << ", ARRIVED:" << arrivalTime;
          for (auto n: result)
          {
               outfile << ", " << ((n.type == burst::CPU) ? "CPU" : "IO") << ":" << n.time;
          }
          outfile << "\n";
          distribution.clear();
          result.clear();
     }
     outfile.close();
}

