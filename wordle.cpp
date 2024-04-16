#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void findWord(std::set<std::string>::iterator it, std::set<std::string>& result, const std::string& in, const std::string& floating, const std::set<std::string>& dict); 
bool checkIfUpper(const std::string& input); 
bool checkAllFloating(const std::string& dict, const std::string& floating);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    
    // Final result
    std::set<std::string> result; 

    // Iterator to point to the first word in the dictionary
    std::set<std::string>::iterator dictionaryIterator = dict.begin();

    // Call recursive helper
    findWord(dictionaryIterator, result, in, floating, dict); 

    return result;
}

// Define any helper functions here
void findWord(std::set<std::string>::iterator dictionaryIterator, std::set<std::string>& result, const std::string& in, const std::string& floating, const std::set<std::string>& dict){
    // Base case, if the dictionary iterator is at the end, that means we have went through all the list
    // Therefore return true
    if (dictionaryIterator == dict.end()){
        return; 
    }

    // If the current word that dictionaryIterator does not match the length of the input word
    // Skip word by recursing and ++
    if (dictionaryIterator->length() != in.length()){
        findWord(++dictionaryIterator, result, in, floating, dict); 
    }

    // If the current word matches the length
    else if (dictionaryIterator->length() == in.length()){
        // A boolean to check if the all floating letters are present in curr word
        // bool floatingStatus = false; 
        

        // First (1) for loop
        // Loop through the curr dictionary word (or input word since they're same length)
        // If the current location is not empty, '-', check the same letter in dict is in the same position 
        // as input
        for (long unsigned int i = 0; i < dictionaryIterator->length(); ++i){

            // If not, move on to next word in dict 
            if (in.at(i) != '-'){
                if (in.at(i) != dictionaryIterator->at(i)){
                    findWord(++dictionaryIterator, result, in, floating, dict); 
                    return;
                }
            }
        }

        // Returns true if all words in floating is present in the curr dictionary word in separate funciton
        bool checkFloat = checkAllFloating(*dictionaryIterator, floating); 
        
        // If not, move on to next word in dict 
        if (checkFloat == false){
            findWord(++dictionaryIterator, result, in, floating, dict); 
            return;
        }

        // If true
        else if (checkFloat == true){

            // Use separate function to see if there are any uppercase,
            // If not uppercase, insert it to result set
            if (checkIfUpper(*dictionaryIterator) == false){
                result.insert(*dictionaryIterator);
            }
            
            // Move on to next word in dict
            findWord(++dictionaryIterator, result, in, floating, dict); 
            return; 
        }
    }
}

bool checkIfUpper(const std::string& input){
    // Second (2) for loop

    // Loop through the word to see if there are any uppercase, return true if so
    for (long unsigned int i = 0; i < input.size(); ++i){
        if (input.at(i) >= 'A' && input.at(i) <= 'Z'){
            return true;
        }
    }

    return false; 
}

bool checkAllFloating(const std::string& dict, const std::string& floating){
    // Base case, If floating string is empty, return true
    if (floating.size() == 0){
        return true;
    }

    // Base case, if the dictionary size is 0, that means that floating > dict, so this word does not contain all letters of floating
    if (dict.size() == 0){
        return false; 
    }

    // Third (3) for loop 
    // Loop through the current dictionary word
    for (long unsigned int i = 0; i < dict.size(); ++i){

        // If the current floating letter is equal to the dictionary letter, 
        // Create a new string that removes that letter using substr
        if (floating.at(0) == dict.at(i)){
            std::string newDict = dict.substr(0, i);

            if (i != dict.size() -1){
                newDict += dict.substr(i+1, dict.size()-1); 
            } 

            // recurse with new substr of dict as well as a substring of floating that removes the front letter
            return checkAllFloating(newDict, floating.substr(1, floating.size()-1));
        }
    }

    return false; 
}
