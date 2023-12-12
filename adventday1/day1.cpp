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

#define ull unsigned long long

ull PartOneSolve(const vector<string>& lines);
ull PartTwoSolve(const vector<string>& lines);
int Convert(string number);
int Convert(char c);
void ReadData(vector<string>& lines);

int main() {
  vector<string> lines;
  ReadData(lines);
  cout << PartOneSolve(lines) << endl;
  cout << PartTwoSolve(lines) << endl;
  return 0;
}

void ReadData(vector<string>& lines) {
  fstream f;
  f.open("input.txt");
  string line;
  while (getline(f, line)) {
    lines.push_back(line);
  }
  f.close();
}

ull PartOneSolve(const vector<string>& lines) {
  ull sum = 0;
  for (auto& line: lines) {
    char numOne;
    char numTwo;
    bool firstNumSet = false;
    for (auto& ch: line) {
      if (isdigit(ch)) {
        if (!firstNumSet) {
          numOne = ch;
          firstNumSet = true;
        }
        numTwo = ch;
      }
    }
    string s;
    s += numOne;
    s += numTwo;
    sum += stoi(s);
  }
  return sum;
}

ull PartTwoSolve(const vector<string>& lines) {
  ull sum = 0;
  const string WORDS[9] = {"one","two","three","four","five","six","seven","eight","nine"};

  for(auto& line: lines){
    char firstNum = 'E';
    int firstNumIndex = INT_MAX;
    char lastNum = 'E';
    int lastNumIndex = INT_MIN;

    int firstWord = 0;
    int firstWordIndex = (INT_MAX);
    int lastWord = 0;
    int lastWordIndex = INT_MIN;

    int index = 0;
    for (auto& ch: line) {
      if(isdigit(ch)) {
        if(firstNum == 'E') {
          firstNum = ch;
          firstNumIndex = index;
        }
        lastNum = ch;
        lastNumIndex = index;
      }
      index++;
    }

    for (auto& num: WORDS) {
      int val = line.find(num);
      if (val != string::npos && val < firstWordIndex) {
        firstWord = Convert(num);
        firstWordIndex = val;
      }
    }

    string reversedLine(line.rbegin(), line.rend());
    for (auto& num: WORDS) {
      string wordToFind(num.rbegin(), num.rend());
      int val = reversedLine.find(wordToFind);
      if(val != string::npos) {
        int valConvert = line.length() - (val + num.length());
        if(valConvert > lastWordIndex) {
          lastWord = Convert(num);
          lastWordIndex = valConvert;
        }
      }
    }


    cout << "Found: ";
    if (firstNumIndex < firstWordIndex) {
      sum += 10 * Convert(firstNum);
      cout << 10*Convert(firstNum);
    } else {
      sum += 10*firstWord;
      cout << 10*firstWord;
    }

    if(lastNumIndex > lastWordIndex) {
      sum += Convert(lastNum);
      cout << lastNum;
    } else {
      sum += lastWord;
    }
  }

  return sum;
}

int Convert(string number) {
  map<string, int> num = {
    {"one", 1},
    {"two",2},
    {"three",3},
    {"four", 4},
    {"five", 5},
    {"six", 6},
    {"seven", 7},
    {"eight", 8},
    {"nine", 9}
  };
  return num[number];
}

int Convert(char c) {
  return c - '0';
}

