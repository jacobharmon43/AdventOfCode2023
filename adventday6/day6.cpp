#include <iostream>
#include <vector> 
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>

#include "../util/util.h"

using namespace std;

class ToyBoat {
  public:
    int speed;
    int distance = 0;
    
    void Update(string argument) {
      if (argument == "Hold") {
        speed += 1;
      } else if (argument == "Release") {
        distance += speed;
      }
    }
};

void ReadData(string filename, vector<int>& times, vector<int>& distances);
void ReadDataPartTwo(string filename, long long& time, long long& distance);
int Solve(vector<int> times, vector<int> distances);
long long Solve(long long time, long long distance);

int main() {
  vector<int> times;
  vector<int> distances;
  long long time;
  long long distance;
  ReadData("input.txt", times, distances);
  ReadDataPartTwo("input.txt", time, distance);
  cout << Solve(times, distances) << endl;
  cout << Solve(time, distance) << endl;
}

void ReadData(string fileName, vector<int>& times, vector<int>& distances) {
  fstream file;
  file.open(fileName);
  if (file.is_open()) {
    string line1;
    string recentTitle;
    string line2;
    getline(file, line1);
    getline(file, line2);
    vector<string> timeStrings = split(line1, ' ');
    vector<string> distanceStrings =  split(line2, ' ');

    for (auto& str : timeStrings){
      if(!isdigit(str[0])) continue;
      times.push_back(stoi(str));
    }
    for (auto& str : distanceStrings){
      if(!isdigit(str[0])) continue;
      distances.push_back(stoi(str));
    }
  }
  file.close();
}

void ReadDataPartTwo(string fileName, long long& time, long long& distance) {
  fstream file;
  file.open(fileName);
  if (file.is_open()) {
    string line1;
    string recentTitle;
    string line2;
    getline(file, line1);
    getline(file, line2);
    vector<string> timeStrings = split(line1, ' ');
    vector<string> distanceStrings =  split(line2, ' ');

    string timeStr = "";
    string distanceStr = "";

    for (auto& str : timeStrings){
      if(!isdigit(str[0])) continue;
      timeStr.append(str);
    }
    for (auto& str : distanceStrings){
      if(!isdigit(str[0])) continue;
      distanceStr.append(str);
    }

    time = stoll(timeStr);
    distance = stoll(distanceStr);
  }
  file.close();
}

int Solve(vector<int> times, vector<int> distances) {
  int waysToWinMult = 1;
  for (int i = 0; i < times.size(); i++) {
    int t = times[i];
    int d = distances[i];
    int minVal = ceil((t - sqrt(pow(t,2)-4*d))/2);
    int maxVal =  floor((t + sqrt(pow(t,2)-4*d))/2);
    if ((t - minVal) * minVal == d) {
      minVal += 1;
    }
    if((t-maxVal) * maxVal == d) {
      maxVal -= 1;
    }
    waysToWinMult *= maxVal - minVal + 1;
  }
  return waysToWinMult;
}

long long Solve(long long time, long long distance) {
  long long t = time;
  long long d = distance;
  long long minVal = ceil((t - sqrt(pow(t,2)-4*d))/2);
  long long maxVal =  floor((t + sqrt(pow(t,2)-4*d))/2);
  if ((t - minVal) * minVal == d) {
    minVal += 1;
  }
  if((t-maxVal) * maxVal == d) {
    maxVal -= 1;
  }
  return maxVal - minVal + 1;
}