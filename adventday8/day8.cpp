#include <iostream>
#include <vector> 
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>
#include <map>
#include <algorithm>
#include <numeric>

#include "../util/util.h"

using namespace std;

#define stepPair pair<string,string>

unsigned long long LCM(vector<unsigned long long> numbers);
unsigned long long SolvePart1(const vector<char> movementVector, const map<string,stepPair>& pairs); 
unsigned long long SolvePart2(const vector<char> movementVector, const map<string,stepPair>& pairs); 
void ReadData(string fileName, vector<char>& movementVector, map<string, stepPair>& pairs);

int main() {
  vector<char> movementVector;
  map<string, stepPair> pairs;
  ReadData("input.txt", movementVector, pairs);
  // cout << SolvePart1(movementVector, pairs) << endl;
  cout << SolvePart2(movementVector, pairs) << endl;
  return 0;
}

void ReadData(string fileName, vector<char>& movementVector, map<string, stepPair>& pairs) {
  fstream file;
  file.open(fileName);
  if (file.is_open()) {
    string firstLine;
    getline(file, firstLine);
    vector<char> tmp(firstLine.begin(), firstLine.end());
    movementVector = tmp;
    string line;
    getline(file,line); // second line is blank
    while(getline(file, line)) {
      vector<string> parts = split(line, ' ');
      pairs[parts[0]] = make_pair<string, string>(parts[2].substr(1,3), parts[3].substr(0, 3));
    }
  }
  file.close();
}

unsigned long long SolvePart1(const vector<char> movementVector, const map<string,stepPair>& pairs) {
  string current = pairs.begin()->first;
  int index = 0;
  unsigned long long stepCount = 0;
  while (current != "ZZZ") {
    switch (movementVector[index]) {
      case 'R':
        current = pairs.at(current).second;
        break;
      case 'L':
        current = pairs.at(current).first;
        break;
    }
    stepCount++;
    index++;
    if (index >= movementVector.size()) {
      index = 0;
    }
  }
  return stepCount;
}

unsigned long long SolvePart2(const vector<char> movementVector, const map<string, stepPair>& pairs) {
  vector<string> startingStrings;
  for(auto& kvp: pairs){
    if(kvp.first[2] == 'A') {
      startingStrings.push_back(kvp.first);
    }
  }
  vector<unsigned long long> allValues;
  for(auto st: startingStrings) {
    string current = st;
    int index = 0;
    unsigned long long stepCount = 0;
    while (current[2] != 'Z') {
      switch(movementVector[index]) {
        case 'R':
          current = pairs.at(current).second;
          break;
        case 'L':
          current = pairs.at(current).first;
          break;
      }
      stepCount++;
      index++;
      if (index >= movementVector.size()) {
        index = 0;
      }
    }
    allValues.push_back(stepCount);
  }
  return LCM(allValues);
}

unsigned long long LCM(vector<unsigned long long> numbers) {
  unsigned long long result = numbers[0];
  for (auto number: numbers) {
    result = lcm(result, number);
  }
  return result;
}