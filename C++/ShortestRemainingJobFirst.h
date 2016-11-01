#ifndef SHORTESTREMAININGJOBFIRST_H_INCLUDED
#define SHORTESTREMAININGJOBFIRST_H_INCLUDED
#include "ShortestJobFirst.h"

/**< This is exactly like Shortest Job First except where noted */
class ShortestRemainingJobFirst:public ShortestJobFirst
{
    public:
    void run();
    ShortestRemainingJobFirst(char*, char*);
};

#endif
