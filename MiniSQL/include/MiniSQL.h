#ifndef _MINISQL_H
#define _MINISQL_H

#include <sys/time.h>
#include <unistd.h>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <typeinfo>
#include <vector>

constexpr int MINI_FLOAT = 3;
constexpr int MINI_INT = 1;
constexpr int MINI_STRING = 2;
constexpr int MINI_CREATE = 0;
constexpr int MINI_DELETE = 1;
constexpr int MINI_SELECT = 2;
constexpr int MINI_DROP = 3;
constexpr int MINI_INSERT = 4;
constexpr int MINI_TABLE = 0;
constexpr int MINI_INDEX = 1;
constexpr int EQU = 0;
constexpr int LT = 1;
constexpr int GT = 2;
constexpr int LE = 3;
constexpr int GE = 4;
constexpr int NE = 5;
constexpr int SINGLE = 0;
constexpr int LARGER = 1;
constexpr int NOTLESS = 2 ;
constexpr int NOTLARGER = 3;
constexpr int LESS = 4;
constexpr int EQUAL = 5;
constexpr int PRECISE = 6;
constexpr int MAX_CHAR = 15;
constexpr int MAX_INT = 509;
constexpr int MAX_FLOAT = 509;
constexpr int BLOCK_SIZE = 4096;
constexpr int MAX_BLOCK_NUM = 1024;

#define _CRT_SECURE_NO_WARNINGS

using namespace std::chrono;
using namespace std;

struct index_info {
  int num;
  int offset;
};

typedef struct aav {
  bool isPrimary = 0, isUnique = 0, isNull = 0;
  int length, type;  // type={MINI_FLOAT,MINI_INT,MINI_STRING}
  string str_value;  // if attribute is string
  int int_value;     // if attribute is int
  float float_value;
} attrAndvalue;

typedef struct AattrNode {
  bool isPrimary, isUnique, hasIndex;
  int length, type, offset;  // type={MINI_FLOAT,MINI_INT,MINI_STRING}
  string attrName;
  AattrNode()
      : isPrimary(0),
        isUnique(0),
        hasIndex(0),
        length(0),
        type(0),
        offset(0),
        attrName("") {}
} attrNode;

#endif
