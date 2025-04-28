#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool schedHelper(const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched, size_t n, size_t k, std::vector<size_t>& amt);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below
    // create empty matrix to track workers' shift amts
    std::vector<size_t> amt(avail[0].size());

    // dive into rec. function, extra vars to track are day index, worker index, and tracking amt of shifts
    return schedHelper(avail, dailyNeed, maxShifts, sched, 0, 0, amt);
}

bool schedHelper(const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched, size_t n, size_t k, std::vector<size_t>& amt)
{
    // if the num of days gets past the num of days in avail, then we have found solution
    if(n == avail.size())
    {
        return true;
    }
    // since sched doesn't have any entries, we need to push an empty vector for every new day
    if(sched.size() <= n)
    {
        sched.push_back(std::vector<unsigned int>());
    }
    // check if we have enough ppl for a given day, and then recurse to the next day
    if(sched[n].size() == dailyNeed)
    {
        return schedHelper(avail, dailyNeed, maxShifts, sched, n + 1, 0, amt);
    }
    // having i = k allows for backtracking if it doesn't work
    for(size_t i = k; i < avail[n].size(); i++)
    {
        // check that workers are available and that they haven't hit their max shifts
        if(avail[n][i] == 1 && amt[i] < maxShifts)
        {
            // if so, add them to the schedule and up their shifts
            sched[n].push_back(i);
            amt[i]++;
            // if(amt[i] > maxShifts)
            // {
            //     amt[i]--;
            //     sched[n].pop_back();
            // }
            // check the next workers until daily is met
            bool status = schedHelper(avail, dailyNeed, maxShifts, sched, n, i + 1, amt);
            if(status)
            {
                return true;
            }
            // if it doesn't work then we have to backtrack
            amt[i]--;
            sched[n].pop_back();
        }
        // else
        // {
        //     return schedHelper(avail, dailyNeed, maxShifts, sched, n, i + 1, amt);
        // }
    }
    return false;
}
