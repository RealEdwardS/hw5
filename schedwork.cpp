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
bool helper(const AvailabilityMatrix& avail, long unsigned int currRow, long unsigned int currCol, const size_t dailyNeed, const size_t maxShifts, DailySchedule& sched, std::map<Worker_T, int> maxStatus);

bool checkMaxShiftStatus(std::map<Worker_T, int>& maxStatus, Worker_T id, const size_t maxShifts); 

void insertWorker(DailySchedule& sched, long unsigned int currRow, std::map<Worker_T, int>& maxStatus, Worker_T workerID); 

void removeWorker(DailySchedule& sched, long unsigned int currRow, std::map<Worker_T, int>& maxStatus, Worker_T workerID);

bool checkIfSameDay(DailySchedule& sched, long unsigned int currRow, long unsigned int currCol, Worker_T workerID); 
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

    // A map that stores pairs of all workers and their # of filled shifts
    std::map<Worker_T, int> maxStatus; 

    // First (1) for loop - Loop through the avilability columns and insert each worker by their col # as their ID and 0 as starting value of filled shifts
    for (long unsigned int i = 0; i < avail.at(0).size(); ++i){
        maxStatus.insert(std::pair<Worker_T, int>(static_cast<Worker_T>(i), 0)); 
    }

    // Second (2) for loop - Create row vectors for all days 
    for (long unsigned int i = 0; i < avail.size(); ++i){
        sched.push_back(std::vector<Worker_T>());
    }

    // Start with row 0, col 0
    return helper(avail, 0, 0, dailyNeed, maxShifts, sched, maxStatus); 
    
}

bool helper(const AvailabilityMatrix& avail, long unsigned int currRow, long unsigned int currCol, const size_t dailyNeed, const size_t maxShifts, DailySchedule& sched, std::map<Worker_T, int> maxStatus){
    
    // Base case, If currRow is greater than the # of days, that means we reached the end
    if (currRow >= avail.size()){
        return true; 
    }

    // If currCol is greater than the dailyNeed then that means we filled the day, move on to next row/day
    // This also ensures that we don't push more than dailyNeed in a single row
    if (currCol >= dailyNeed){
        return helper(avail, currRow + 1, 0, dailyNeed, maxShifts, sched, maxStatus); 
    }
 
    // Third (3) for loop - Loop through each worker for the day - This should be the same for all days
    for (long unsigned int i = 0; i < avail.at(currRow).size(); ++i){
        
        // If the worker is available and the worker hasn't reached their maxShifts yet and the worker is not already booked for that day
        // (A worker cannot work twice the same day since they're only available once that day)
        // (If worker is not available or the worker reached their maxShifts, go to next worker) 
        if (avail.at(currRow).at(i) == 1 && checkMaxShiftStatus(maxStatus, i, maxShifts) == false && checkIfSameDay(sched, currRow, 0, i) == true){
            // Insert worker
            insertWorker(sched, currRow, maxStatus, i); 

            // If the next recursive call returns true, then return true
            if (helper(avail, currRow, currCol+1, dailyNeed, maxShifts, sched, maxStatus) == true){
                return true; 
            }

            // If the next recursive call returns false, then remove the worker and continue the for loop for the next worker
            else{
                removeWorker(sched, currRow, maxStatus, i);
                continue;
            }
        }
    }

    // if the for loop concludes, that means that no worker was found so return false
    return false;
}

// Checks if a worker has reached its max shift already
// Returns true is so
// Returns false if not
bool checkMaxShiftStatus(std::map<Worker_T, int>& maxStatus, Worker_T workerID, const size_t maxShifts){
    // Uses iterator to find the user in the map
    std::map<Worker_T, int>::iterator it = maxStatus.find(workerID); 
    
    // if (it == maxStatus.end()){
    //     return true; 
    // }

    if (it->second >= static_cast<int>(maxShifts)){
        return true; 
    }

    return false;
}

// Inserts worker into the sched vector and taking care of updating the person's shift status
void insertWorker(DailySchedule& sched, long unsigned int currRow, std::map<Worker_T, int>& maxStatus, Worker_T workerID){
    
    // Insert a worker into sched
    sched.at(currRow).push_back(workerID); 

    // Increase worker maxStatus
    std::map<Worker_T, int>::iterator it = maxStatus.find(workerID); 
    ++(it->second);
}

// Removes  worker into the sched vector and taking care of updating the person's shift status
void removeWorker(DailySchedule& sched, long unsigned int currRow, std::map<Worker_T, int>& maxStatus, Worker_T workerID){
    

    // Remove the last worker in row
    sched.at(currRow).pop_back(); 

    // Decrease worker maxStatus
    std::map<Worker_T, int>::iterator it = maxStatus.find(workerID); 
    --(it->second);
}

// A worker cannot work twice in the same day, so check if a worker is already working/scheduled in the day/row already
bool checkIfSameDay(DailySchedule& sched, long unsigned int currRow, long unsigned int currCol, Worker_T workerID){
    if (currCol >= sched.at(currRow).size()){
        return true; 
    }

    if (sched.at(currRow).at(currCol) == workerID){
        return false; 
    }

    return checkIfSameDay(sched, currRow, currCol + 1, workerID); 
}

