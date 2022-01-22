#include "general/vectorUtility.h"
#include "general/stringUtility.h"
#include "general/parseArguments.h"
#include "general/binaryUtility.h"

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

  pair<IP, IP> startAndEndIP() const {
    // TODO
    throw '!';
  }

  vector<CIDR> divide(int numRanges) const {
    // TODO
    throw '!';
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
    // TODO
    throw '!';
  }

  unsigned int numHostsThatCanExist() const {
    return pow(2, 32 - networkPortion) - (networkPortion == 31 ? 0 : 2);
  }

};

vector<CIDR> aggregate(const vector<CIDR>& ranges) {
  // TODO
  throw '!';
}

vector<bool> IP::containedInEachOf(const vector<CIDR>& ranges) const {
  // TODO
  throw '!';
}

CIDR IP::longestPrefixMatch(const vector<CIDR>& candidates) const {
  // TODO
  throw '!';
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