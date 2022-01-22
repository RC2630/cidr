#include "general/vectorUtility.h"
#include "general/stringUtility.h"
#include "general/parseArguments.h"
#include "general/binaryUtility.h"
#include "general/numberUtility.h"
#include "general/ansi_codes.h"

#include <cmath>
#include <tuple>
#include <utility>

struct CIDR; // forward declaration

struct IP {

  int a;
  int b;
  int c;
  int d;

  IP() {
    // nothing
  }

  IP(const string& ip) {
    string temp = ip;
    strUtil::replaceAllChar1WithChar2(temp, '.', ' ');
    temp = "/cmd " + temp;
    a = parse::parseNumericalArgument(temp, 1);
    b = parse::parseNumericalArgument(temp, 2);
    c = parse::parseNumericalArgument(temp, 3);
    d = parse::parseNumericalArgument(temp, 4);
  }

  IP(const vector<int>& digits) {
    vector<int> result;
    for (int i = 0; i <= 24; i += 8) {
      result.push_back(binUtil::binToDec1Byte(vecUtil::subvector(digits, i, i + 7)));
    }
    a = result.at(0);
    b = result.at(1);
    c = result.at(2);
    d = result.at(3);
  }

  bool operator == (const IP& other) const {
    return tie(a, b, c, d) == tie(other.a, other.b, other.c, other.d);
  }

  bool operator < (const IP& other) const {
    return a < other.a ? true : (a > other.a ? false : (
      b < other.b ? true : (b > other.b ? false : (
        c < other.c ? true : (c > other.c ? false : (
          d < other.d))))));
  }

  vector<int> binaryDigits() const {
    vector<int> digits;
    vector<int> temp = binUtil::decToBin1Byte(a);
    digits.insert(digits.end(), temp.begin(), temp.end());
    temp = binUtil::decToBin1Byte(b);
    digits.insert(digits.end(), temp.begin(), temp.end());
    temp = binUtil::decToBin1Byte(c);
    digits.insert(digits.end(), temp.begin(), temp.end());
    temp = binUtil::decToBin1Byte(d);
    digits.insert(digits.end(), temp.begin(), temp.end());
    return digits;
  }

  vector<bool> containedInEachOf(const vector<CIDR>& ranges) const;
  CIDR longestPrefixMatch(const vector<CIDR>& candidates) const;

};

struct CIDR : public IP {

  int networkPortion;

  CIDR(const string& cidr) {
    string temp = cidr;
    strUtil::replaceAllChar1WithChar2(temp, '.', ' ');
    strUtil::replaceAllChar1WithChar2(temp, '/', ' ');
    temp = "/cmd " + temp;
    a = parse::parseNumericalArgument(temp, 1);
    b = parse::parseNumericalArgument(temp, 2);
    c = parse::parseNumericalArgument(temp, 3);
    d = parse::parseNumericalArgument(temp, 4);
    networkPortion = parse::parseNumericalArgument(temp, 5);
  }

  CIDR(const IP& ip, int networkPortion_) {
    a = ip.a;
    b = ip.b;
    c = ip.c;
    d = ip.d;
    networkPortion = networkPortion_;
  }

  bool operator == (const CIDR& other) const {
    return (IP) *this == (IP) other && networkPortion == other.networkPortion;
  }

  IP netmask() const {
    vector<int> digits;
    for (int i = 1; i <= 32; i++) {
      if (i <= networkPortion) {
        digits.push_back(1);
      } else {
        digits.push_back(0);
      }
    }
    return IP(digits);
  }

  CIDR simplify() const {
    vector<int> digits = binaryDigits();
    vector<int> mask = netmask().binaryDigits();
    for (int i = 0; i < digits.size(); i++) {
      digits.at(i) &= mask.at(i);
    }
    return CIDR(IP(digits), networkPortion);
  }

  bool contains(const IP& ip) const {
    CIDR other(ip, networkPortion);
    return other.simplify() == simplify();
  }

  unsigned int numHostsThatCanExist() const {
    return pow(2, 32 - networkPortion) - (networkPortion == 31 ? 0 : 2);
  }

  pair<IP, IP> startAndEndIP() const {
    vector<int> start = simplify().binaryDigits();
    vector<int> end = vecUtil::subvector(start, 0, networkPortion - 1);
    for (int i = networkPortion + 1; i <= 32; i++) {
      end.push_back(1);
    }
    return {IP(start), IP(end)};
  }

  vector<CIDR> divide(int numRanges) const {
    vector<int> networkPart = vecUtil::subvector(simplify().binaryDigits(), 0, networkPortion - 1);
    vector<vector<int>> hostParts;
    for (int i = 0; i < numRanges; i++) {
      vector<int> temp = binUtil::decToBinNdigits(i, numUtil::log(numRanges, 2));
      int tempSize = temp.size();
      for (int i = 1; i <= 32 - networkPortion - tempSize; i++) {
        temp.push_back(0);
      }
      hostParts.push_back(temp);
    }
    vector<CIDR> divisions;
    for (const auto& hostPart : hostParts) {
      divisions.push_back(CIDR(IP(vecUtil::concatenate<int>({networkPart, hostPart})), networkPortion + numUtil::log(numRanges, 2)));
    }
    return divisions;
  }

};

vector<CIDR> aggregate(const vector<CIDR>& ranges) {
  // TODO
  throw '!';
}

vector<bool> IP::containedInEachOf(const vector<CIDR>& ranges) const {
  vector<bool> v;
  for (const CIDR& cidr : ranges) {
    v.push_back(cidr.contains(*this));
  }
  return v;
}

CIDR IP::longestPrefixMatch(const vector<CIDR>& candidates) const {
  vector<CIDR> filteredCandidates;
  for (const CIDR& cidr : candidates) {
    if (cidr.contains(*this)) {
      filteredCandidates.push_back(cidr);
    }
  }
  return *max_element(filteredCandidates.begin(), filteredCandidates.end(), [] (const CIDR& cidr1, const CIDR& cidr2) {
    return cidr1.networkPortion < cidr2.networkPortion;
  });
}

ostream& operator << (ostream& out, const IP& ip) {
  out << ip.a << "." << ip.b << "." << ip.c << "." << ip.d;
  return out;
}

ostream& operator << (ostream& out, const CIDR& cidr) {
  out << (IP) cidr << "/" << cidr.networkPortion;
  return out;
}

int main() {
  // ...
}