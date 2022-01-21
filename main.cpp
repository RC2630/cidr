#include "general/vectorUtility.h"
#include "general/stringUtility.h"
#include "general/parseArguments.h"
#include "binaryUtility.h"
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

  pair<IP, IP> startAndEndIP() const {
    // TODO
    throw '!';
  }

  vector<CIDR> divide(int numRanges) const {
    // TODO
    throw '!';
  }

  bool contains(const IP& ip) const {
    // TODO
    throw '!';
  }

  IP netmask() const {
    // TODO
    throw '!';
  }

  CIDR simplify() const {
    // TODO
    throw '!';
  }

  int numHostsThatCanExist() const {
    // TODO
    throw '!';
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