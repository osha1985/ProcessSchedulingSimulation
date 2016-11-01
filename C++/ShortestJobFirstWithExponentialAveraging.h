#ifndef SHORTESTJOBFIRSTWITHEXPONENTIALAVERAGING_H_INCLUDED
#define SHORTESTJOBFIRSTWITHEXPONENTIALAVERAGING_H_INCLUDED
#include "ShortestJobFirst.h"

/**< This is exactly like Shortest Job First except where noted */
class ShortestJobFirstWithExponentialAveraging:public ShortestJobFirst
{
    void addProcessesToReadyQueue();
    public:
    ShortestJobFirstWithExponentialAveraging(char*, char*);
};

#endif
