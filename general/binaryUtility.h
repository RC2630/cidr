#ifndef BINARY_UTILITY_DOT_H
#define BINARY_UTILITY_DOT_H

#include <vector>
#include <cmath>

using namespace std;

namespace binUtil {

  vector<int> decToBin1Byte(int dec) {
    vector<int> v;
    for (int i = 7; i >= 0; i--) {
      if (dec - pow(2, i) >= 0) {
        v.push_back(1);
        dec -= pow(2, i);
      } else {
        v.push_back(0);
      }
    }
    return v;
  }

  int binToDec1Byte(const vector<int>& bin) {
    int d = 0;
    for (int i = 0; i < bin.size(); i++) {
      if (bin.at(i) == 1) {
        d += pow(2, 7 - i);
      }
    }
    return d;
  }

}

#endif