#ifndef _INTERPRETER_H
#define _INTERPRETER_H

#include "MiniSQL.h"

#include "API.h"
#include "CatalogManager.h"
#include "RecordManager.h"

// enum operators { EQU, LT, GT, LE, GE, NE };

// double print_time;

class ShowSomething {
 public:
  string thing;
  ShowSomething(string a) : thing(a) {}
};
class BreakException {};
class SyntaxException {
 public:
  string errlog;
  SyntaxException(string a) : errlog(a) {}
};

class Interpreter {
 public:
  Interpreter();
  void readCommand(bool cmd);
  void pipeline();
  Interface parse();

 private:
  set<char> token;
  vector<string> statement;

  map<string, int> op;
  bool loop;
  bool cmd;
  int level;
  catalogManager catalog;
  API api;

 private:
  int split(string line);
  void refresh();
  void processCondition(string table, vector<string> &statement, int index,
                        Interface &interface);
};

#endif