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
void wordleHelper(const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict, int index, int remaining, std::set<std::string>& generated);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    std::set<std::string> generated;
    int remaining = 0;
    // we need to know how many non-fixed chars there are
    for(size_t i = 0; i < in.size(); i++)
    {
        if(in[i] == '-') remaining++;
    }
    // start recursion
    wordleHelper(in, floating, dict, 0, remaining, generated);
    return generated; // return final set
}

// Define any helper functions here
void wordleHelper(const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict, int index, int remaining, std::set<std::string>& generated)
{
    // base case: if we've traversed to the end of the string, and used all the floating chars, and if the word is valid, then add
    if(index == (int)in.size())
    {
        if(floating.empty())
        {
            if(dict.find(in) != dict.end())
            {
              // insert also already accounts for duplicates
                generated.insert(in); // if word exists in dictionary, insert into the set i created
            }
        }
        return;
    }
    if(in[index] != '-')
    {
        wordleHelper(in, floating, dict, index + 1, remaining, generated); // just skip the index if this is a fixed value
    }
    else
    {
        for(size_t k = 0; k < floating.size(); k++)
        {
            // for each floating char, recurse the rest of the string with this char in place of current index
            std::string word = in;
            word[index] = floating[k]; // replace '-' with floating
            string newFloating = floating.substr(0, k) + floating.substr(k + 1, floating.size()); // remove the current floating letter
            wordleHelper(word, newFloating, dict, index + 1, remaining - 1, generated);
        }
        // only go into this case if we know that there are more blanks than floating, speeds up processing time
        if((int)floating.size() < remaining)
        {
            for(int i = 0; i < 26; i++)
            {
                std::string word = in;
                word[index] = 'a' + i; // loop through every character in alpha and then recurse w each option in the blank
                wordleHelper(word, floating, dict, index + 1, remaining - 1, generated);
            }
        }
    }
    return;
}
/* initial logic ideas: start out with each floating letter in each available spot. In the empty spots cycle through all possible letters
cycle through different combinations of floating letters (only need to do one of each because if it is repeated it will be covered)
eg: if floating letters are n and d and the fixed letters are -i-- then you can do nid_ (cycle through everything) then din_ (cycle
through everything) and then ni_d and then di_n)

recursion parameters:
try first parameter and lock it in
insert new string
try first parameter and lock it in
cycle through all 

for however many blanks that there aren't floating
*/
