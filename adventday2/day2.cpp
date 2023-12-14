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


class Showing{
  public:
    int red;
    int blue;
    int green;

    Showing(): red(0), blue(0), green(0) {}
    Showing(int red, int blue, int green): red(red), blue(blue), green(green) {}
};

void ReadData(vector<vector<Showing>>& games);
int SolvePart1(const vector<vector<Showing>> games, int red, int green, int blue);
int SolvePart2(const vector<vector<Showing>> games);

int main() {
  vector<vector<Showing>> games;
  ReadData(games);
  cout << SolvePart1(games, 12,13,14) << endl;
  cout << SolvePart2(games) << endl;
  return 0;
}

void ReadData(vector<vector<Showing>>& games) {
  fstream file;
  file.open("input.txt");
  if(file.is_open()) {
    string line;
    int index = 1;
    while (getline(file, line)) {
      vector<Showing> shows;
      vector<string> showings = split(line, ';');
      showings[0].erase(showings[0].begin(), showings[0].begin() + 6 + to_string(index).length());
      for (auto showing: showings){
        vector<string> colors = split(showing, ',');
        Showing show = Showing();
        for (auto color: colors) {
          bool green = color.find('g') != string::npos;
          bool blue = color.find('b') != string::npos;
          int number = stoi(split(color, ' ')[1]);
          if (green) {
            show.green = number;
          } else if (blue) {
            show.blue = number;
          } else {
            show.red = number;
          }
        }
        shows.push_back(show);
      }
      index++;
      games.push_back(shows);
    }
  }
  file.close();
}

int SolvePart1(const vector<vector<Showing>> games, int red, int green, int blue) {
  int sum = 0;
  int index = 1;
  for (auto& gameSet: games) {
    bool fineSet = true;
    for (auto& game: gameSet) {
      if (game.red > red || game.green > green || game.blue > blue) {
        fineSet = false;
        break;
      }
    }
    if (fineSet) {
      sum += index;
    }
    index++;
  }
  return sum;
}

int SolvePart2(const vector<vector<Showing>> games) {
  int sum = 0;
  for (auto& gameSet: games){
    int max_red = 0;
    int max_green = 0;
    int max_blue = 0;
    for (auto& game: gameSet) {
      max_red = game.red > max_red ? game.red : max_red;
      max_green = game.green > max_green ? game.green : max_green;
      max_blue = game.blue > max_blue ? game.blue : max_blue;
    }
    sum += max_red * max_green * max_blue;
  }
  return sum;
}