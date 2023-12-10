#include <iostream>
#include <vector> 
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>
#include <map>
#include <algorithm>

#include "../util/util.h"

using namespace std;

template <typename KeyType, typename ValueType>
void printMap(const std::map<KeyType, ValueType>& myMap) {
    for (const auto& pair : myMap) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    }
}

unsigned long hexStringToDecimal(const std::string& hexString) {
    unsigned long decimalResult;
    std::stringstream(hexString) >> std::hex >> decimalResult;
    return decimalResult;
}

class Hand{
  public:
    string hand;
    int bid;
    int rank;
    unsigned long subrank;

    Hand(string hand, int bid, bool part2 = false) : hand(hand), bid(bid) {
      if (isFiveOfAKind(hand, part2)) {
        rank = 7;
      } else if (isFourOfAKind(hand, part2)) {
        rank = 6;
      } else if (isFullHouse(hand, part2)) {
        rank = 5;
      } else if (isThreeOfAKind(hand, part2)) {
        rank = 4;
      } else if (isTwoPair(hand, part2)) {
        rank = 3;
      } else if (isOnePair(hand, part2)) {
        rank = 2;
      } else {
        rank = 1;
      }
      subrank = hexStringToDecimal(GetHex(hand, part2));
      if(part2) {
        cout << "Hand: " << hand << " " << rank << " " << subrank << endl;
      }
    }

    bool operator<(const Hand& other){

      if(rank < other.rank) {
        return true;
      } else if (other.rank < rank) {
        return false;
      }
      return subrank < other.subrank;
    }

    string GetHex(string hand, bool part2 = false) {
      string s = "";
      map<char, char> m;
      if(!part2) {
        m.insert({'2', '0'});
        m.insert({'3', '1'});
        m.insert({'4', '2'});
        m.insert({'5', '3'});
        m.insert({'6', '4'});
        m.insert({'7', '5'});
        m.insert({'8', '6'});
        m.insert({'9', '7'});
        m.insert({'T', '8'});
        m.insert({'J', '9'});
      } else {
        m.insert({'J', '0'});
        m.insert({'2', '1'});
        m.insert({'3', '2'});
        m.insert({'4', '3'});
        m.insert({'5', '4'});
        m.insert({'6', '5'});
        m.insert({'7', '6'});
        m.insert({'8', '7'});
        m.insert({'9', '8'});
        m.insert({'T', '9'});
      }
      m.insert({'Q', 'A'});
      m.insert({'K', 'B'});
      m.insert({'A', 'C'});

      for (char c : hand) {
        s += m[c];
      }
      return s;
    }

    bool isFiveOfAKind(string hand, bool part2 = false) {
      map<char, int> m = {};
      for (char c : hand){
        if(m.count(c) == 0) {
          m[c] = 1;
        } else {
          m[c]++;
        }
        if(m[c] == 5) {
          return true;
        }
      }
      if(!part2) return false;
      if(!m.count('J')) {
        return m.size() == 1;
      } else {
        int countRequired = 5 - m['J'];
        if (countRequired <= 0) return true;
        for(auto& p: m) {
          if(p.first != 'J' && p.second == countRequired){
            return true;
          }
        }
      }
      return false;
    }
    
    bool isFourOfAKind(string hand, bool part2 = false) {
      map<char, int> m = {};
      for (char c : hand){
        if(m.count(c) == 0) {
          m[c] = 1;
        } else {
          m[c]++;
        }
        if(m[c] == 4) {
          return true;
        }
      }
      if(!part2) return false;
      if(m.count('J') == 0) {
        for(auto& p: m) {
          if(p.second == 4) {
            return true;
          }
        }
      } else {
        int countRequired = 4 - m['J'];
        if (countRequired <= 0) return true;
        for(auto& p: m) {
          if(p.first != 'J' && p.second == countRequired){
            return true;
          }
        }
      }
      return false;
    }
    
    bool isFullHouse(string hand, bool part2 = false) {
      map<char, int> m = {};
      for (char c : hand){
        if(m.count(c) == 0) {
          m[c] = 1;
        } else {
          m[c]++;
        }
      }
      if(!part2 || !m.count('J')) {
        return m.size() == 2;
      }
      return m.size() == 3 && m['J'] != 3; // JJJA2 is not a full house, JJA22 or JJAA2 is. or JAA22 or JAAA2 or JA222;
    }

    bool isThreeOfAKind(string hand, bool part2 = false) {
      map<char, int> m = {};
      for (char c : hand){
        if(m.count(c) == 0) {
          m[c] = 1;
        } else {
          m[c]++;
        }
        if(m[c] == 3) {
          return true;
        }
      }
      if(!part2) return false;
      if(!m.count('J')) {
        for(auto& p: m) {
          if(p.second == 3){
            return true;
          }
        }
      } else {
        int countRequired = 3 - m['J'];
        if (countRequired <= 0) return true;
        for(auto& p: m) {
          if(p.first != 'J' && p.second == countRequired){
            return true;
          }
        }
      }
      return false;
    }
    
    bool isTwoPair(string hand, bool part2 = false) {
      map<char, int> m = {};
      for (char c : hand){
        if(m.count(c) == 0) {
          m[c] = 1;
        } else {
          m[c]++;
        }
      }
      if (!part2 || !m.count('J')){
        return m.size() == 3;
      }
      return (m['J'] == 1 && m.size() <= 4) || m['J'] >= 2;
    }

    bool isOnePair(string hand, bool part2 = false) {
      map<char, int> m = {};
      for (char c : hand){
        if(m.count(c) == 0) {
          m[c] = 1;
        } else {
          m[c]++;
        }
        if(m[c] == 2) {
          return true;
        }
      }
      if(!part2) return false;
      if(!m.count('J')) {
        return m.size() == 4;
      } else {
        return true;
      } 
    }
};

void ReadData(string fileName, vector<Hand>& hands, bool part2 = false);

int main() {
  int sum = 0;
  vector<Hand> hands;
  ReadData("input.txt", hands, false);
  sort(hands.begin(), hands.end());
  int rank = 1;
  for (auto& hand: hands){
    sum += rank * hand.bid;
    rank++;
  }

  int sum2 = 0;
  vector<Hand> hands2;
  ReadData("input.txt", hands2, true);
  sort(hands2.begin(), hands2.end());
  int rank2 = 1;
  for (auto& hand: hands2){
    sum2 += rank2 * hand.bid;
    rank2++;
  }

  cout << "Part 1: " << sum << endl;
  cout << "Part 2: " << sum2 << endl;
  return 0;
}

void ReadData(string fileName, vector<Hand>& hands, bool part2) {
  fstream file;
  file.open(fileName);
  if (file.is_open()) {
    string line;
    while(getline(file, line)) {
      vector<string> spl = split(line, ' ');
      hands.push_back(Hand(spl[0], stoi(spl[1]), part2));
    }
  }
  file.close();
}