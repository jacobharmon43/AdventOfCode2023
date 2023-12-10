#include <iostream>
#include <vector> 
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <stack>

#include "../util/util.h"

using namespace std;

#define Seed long long

class SeedMap {
  public:
    Seed seedMin;
    Seed seedMax;
    Seed min;
    Seed max;

    SeedMap(Seed seedMin, Seed seedMax, Seed min, Seed max) : seedMin(seedMin), seedMax(seedMax), min(min), max(max) {}

    SeedMap(Seed seedMin, Seed seedMax) : seedMin(seedMin), seedMax(seedMax), min(seedMin), max(seedMax) {}
};

class MapValue {
  public:
    Seed diff;
    Seed min;
    Seed max;
    void SetData(string line) {
      vector<string> numbers = split(line, ' '); // We assume here that there are 3 numbers per line
      Seed destinationRangeStart = stoll(numbers[0]);
      Seed sourceRangeStart = stoll(numbers[1]);
      Seed rangeLength = stoll(numbers[2]);

      diff = destinationRangeStart - sourceRangeStart;
      min = sourceRangeStart;
      max = sourceRangeStart + rangeLength;
    }

    bool ChangeSeed(Seed& seed) {
      if (this->min <= seed && seed <= this->max) {
        seed += diff;
        return true;
      }
      return false;
    }

    bool isOverlap(const SeedMap& other) const {
      return !(max < other.min || min > other.max);
    }

    /**
     * -- returns a atleast size 1 vector that is the new current seed value. Any extra values are the leftover ranges that were cut off
    */
    vector<SeedMap> computeNew(const SeedMap& other) const {
      vector<SeedMap> v = {};
      // Do any values fit?
      if (!isOverlap(other)) {
        v.push_back(other);
      } else {
        SeedMap m = SeedMap(other.seedMin, other.seedMax, std::max(min, other.min), std::min(max, other.max));
        int leftCutoff = m.min - other.min;
        int rightCutoff = other.max - m.max;

        m.min += diff;
        m.max += diff;
        v.push_back(m);
        if(leftCutoff > 0){
          SeedMap leftSideLoss = SeedMap(other.seedMin, other.seedMin + leftCutoff - 1);
          leftSideLoss.seedMin = other.seedMin;
          leftSideLoss.seedMax = other.seedMin + leftCutoff - 1;
          v.push_back(leftSideLoss);
        }
        if(rightCutoff > 0){
          SeedMap rightSideLoss = SeedMap(other.seedMax - rightCutoff + 1, other.seedMax);
          rightSideLoss.seedMin = other.seedMax - rightCutoff + 1;
          rightSideLoss.seedMax = other.seedMax;
          v.push_back(rightSideLoss);
        }

      }
      return v;
    }
};

// Pre-Declaration
void FillPartTwoData(stack<SeedMap>& seedMaps);
void FillPartOneData(vector<Seed>& seeds, vector<vector<MapValue>>& maps);
Seed PartOneSolve(vector<Seed> seeds, const vector<vector<MapValue>> maps);
Seed PartTwoSolve(stack<SeedMap> seedMaps, const vector<vector<MapValue>> maps);

// Globals
const vector<string> MAP_NAMES = {"seed-to-soil", "soil-to-fertilizer", "fertilizer-to-water", "water-to-light", "light-to-temperature", "temperature-to-humidity", "humidity-to-location"};


int main() {
  vector<Seed> allSeeds;
  stack<SeedMap> seedMaps;
  vector<vector<MapValue>> maps = {{},{},{},{},{},{},{}};

  FillPartOneData(allSeeds, maps);
  FillPartTwoData(seedMaps);

  cout << "Part 1: ";
  cout << PartOneSolve(allSeeds, maps) << endl;
  cout << "Part 2: ";
  cout << PartTwoSolve(seedMaps, maps) << endl;
}

/**
 * Iterates over the file, grabbing seeds from line 1
 * Grabbing and creating maps for the remaining lines
*/
void FillPartOneData(vector<Seed>& seeds, vector<vector<MapValue>>& maps) {
  // Reading file for info
  fstream file;
  file.open("input.txt");
  if (file.is_open()) {
    string line;
    string recentTitle;
    while (getline(file, line)) {
      if(line.length() == 0) continue;
      if (isalpha(line[0])) { // Look for titles
        recentTitle = split(line, ' ')[0];
        if (recentTitle == "seeds:") { // Special case for same line text
          for (auto& num : split(line, ' ')) {
            if(isalpha(num[0])) continue; // If its the first word, ignore it
            seeds.push_back(stoll(num));
          }
        }
      } else {
        // Fill data
        MapValue mv = MapValue();
        mv.SetData(line);
        auto it = find(MAP_NAMES.begin(), MAP_NAMES.end(), recentTitle);
        size_t index = distance(MAP_NAMES.begin(), it);
        maps[index].push_back(mv);
      }
    }
  }
  file.close();
}

/**
 * Gets the seed ranges from line 1 of the input file
*/
void FillPartTwoData(stack<SeedMap>& seedMaps) {
  // Reading file for info
  fstream file;
  file.open("input.txt");
  if (file.is_open()) {
    string line;
    getline(file, line);
    vector<string> splits = split(line, ' ');
    splits.erase(splits.begin());
    for(int i = 0; i < splits.size(); i++) {
      SeedMap m = SeedMap(stoll(splits[i]), stoll(splits[i]) + stoll(splits[i+1]));
      i++;
      seedMaps.push(m);
    }
  }
  file.close();
}

/**
 * iterates over every individual seed looking for its corresponding location value
*/
Seed PartOneSolve(vector<Seed> seeds, vector<vector<MapValue>> maps) {
  // Calculation part 
  Seed minResult = LLONG_MAX;
  for (auto& seed: seeds) {
    for (auto& map: maps){
      for (auto& mapVal: map) {
        if (mapVal.ChangeSeed(seed)) { break; }
      }
    }
    if (seed < minResult) {
      minResult = seed;
    }
  }
  return minResult;
}

/**
 * Iterates over every seed range looking for the possible location ranges
 * Bucketing approach
*/
Seed PartTwoSolve(stack<SeedMap> seedMaps, const vector<vector<MapValue>> maps) {
  Seed minResult = LLONG_MAX;
  while (!seedMaps.empty()) {
    SeedMap seed = seedMaps.top();
    seedMaps.pop();
    for (const auto& map: maps) {
      for (const auto& mapVal: map) {
        if (mapVal.isOverlap(seed)) {
          vector<SeedMap> v = mapVal.computeNew(seed);
          seed = v[0];
          for(int i = 1; i < v.size(); i++) {
            seedMaps.push(v[i]);
          }
          break;
        }
      }
    }
    if(seed.min < minResult) {
      minResult = seed.min;
    }
  }
  return minResult;
}