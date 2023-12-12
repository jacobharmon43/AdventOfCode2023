#ifndef UTIL_H
#define UTIL_H

#include <vector> 
#include <string>

using namespace std;

/**
 * Generic vector print
 * Values you print must have << defined as an operator
*/
template <typename T>
void printVector(const std::vector<T>& vec) {
    std::cout << "[ ";
    for (const T& element : vec) {
        std::cout << element << ", ";
    }
    std::cout << "]\n";
}

vector<string> split(const string& input, char delimiter);

/**
 * Returns a vector of strings retrieved from splitting the string at a given character
*/
vector<string> split(const string &input, char delimiter) {
  vector<string> substrings;
  istringstream stream(input);
  string token;
  while(getline(stream, token, delimiter)) {
    substrings.push_back(token);
  }
  return substrings;
}


#endif