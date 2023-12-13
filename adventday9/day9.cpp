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

#define ll long long

using namespace std;

void ReadData(string fileName, vector<vector<ll>>& numberLines);
ll SolvePart1(const vector<vector<ll>> numberLines);
ll SolvePart2(const vector<vector<ll>> numberLines);

int main() {
  vector<vector<ll>> numberLines;
  ReadData("input.txt", numberLines);
  cout << SolvePart1(numberLines) << endl;
  cout << SolvePart2(numberLines) << endl;
  return 0;
}

void ReadData(string fileName, vector<vector<ll>>& numberLines) {
  fstream file;
  file.open(fileName);
  if (file.is_open()) {
    string line;
    int i = 0;
    while (getline(file, line)) {
      vector<string> parts = split(line, ' ');
      numberLines.push_back(vector<ll>());
      for (auto part: parts){
        numberLines[i].push_back(stoll(part));
      }
      i++;
    }
  }
  file.close();
}

ll SolvePart1(const vector<vector<ll>> numberLines) {
  ll sum = 0;
  for (auto numberLine: numberLines) {
    vector<vector<ll>> newLines;
    newLines.push_back(numberLine);
    int currentIndex = 0;
    while(!all_of(newLines[currentIndex].begin(), newLines[currentIndex].end(), [](ll val) {return val == 0;})) {
      newLines.push_back(vector<ll>());
      size_t size = newLines[currentIndex].size();
      for(int i = 0; i < size - 1; i++) {
        newLines[currentIndex+1].push_back(newLines[currentIndex][i+1] - newLines[currentIndex][i]);
      }
      currentIndex++;
    }

    ll guess = 0;
    for (auto line: newLines) {
      guess += line[line.size() - 1];
    }

    sum += guess;
  }
  return sum;
}

ll SolvePart2(const vector<vector<ll>> numberLines) {
  ll sum = 0;
  for (auto numberLine: numberLines) {
    vector<vector<ll>> newLines;
    newLines.push_back(numberLine);
    int currentIndex = 0;
    while(!all_of(newLines[currentIndex].begin(), newLines[currentIndex].end(), [](ll val) {return val == 0;})) {
      newLines.push_back(vector<ll>());
      size_t size = newLines[currentIndex].size();
      // 6 i = 5 [1].push_back
      for(int i = size - 1; i > 0; i--) {
        newLines[currentIndex+1].insert(newLines[currentIndex+1].begin(), newLines[currentIndex][i-1] - newLines[currentIndex][i]);
      }
      currentIndex++;
    }

    ll guess = 0;
    for (auto line: newLines) {
      guess += line[0];
    }
    sum += guess;
  }
  return sum;
}