#ifndef _RecordManager_H
#define _RecordManager_H

#include "BufferManager.h"

#include "MiniSQL.h"

struct Union {
  bool isNull = false;
  string s = "";
  int n = 0;
  float f = 0.0;
};

struct TreeNode {
  int id = -1;
  int type = 0;
  int op = 0;  // 0: =; 1: <>; 2: <; 3: >; 4: <=; 5: >=;
  Union value;
};

struct Definition {
  string var = "";
  int type = 0;  // 1: int; 2: char; 3: float
  int len = 0;
};

class Interface {
 public:
  int whichOperation() { return operation; }
  int whichObject() { return object; }
  int getPrimaryKeyIndex() { return pkey; }
  attrNode getPrimaryKey() { return definitions[pkey]; }
  attrNode getAttrNode(int i) { return definitions[i]; }
  vector<attrNode> getDefinitions() { return definitions; }
  string indexOnWhichTable() { return tableName; }
  int indexOnWhichColumn() { return column; }
  string getIndexName() { return indexName; }
  string getTableName() { return tableName; }
  vector<TreeNode> getConditions() { return conditions; }
  TreeNode getConditionByIndex(int index) { return conditions[index]; }
  vector<Union> getData() { return data; }
  Union getDatumByIndex(int index) { return data[index]; }
  void setOperation(int i) { operation = i; }
  void setObject(int i) { object = i; }
  void setTableName(string name) { tableName = name; }
  void setIndexName(string name) { indexName = name; }
  void addCondition(TreeNode t) { conditions.push_back(t); }
  void addData(Union u) { data.push_back(u); }
  void setColumn(int i) { column = i; }
  void addDefinition(attrNode def) { definitions.push_back(def); }
  int findIndexOfDef(string name) {
    for (int i = 0; i < definitions.size(); ++i)
      if (definitions[i].attrName == name) {
        definitions[i].isPrimary = true;
        definitions[i].isUnique = true;
        return i;
      }
    return -1;
  }
  void setPrimaryKeyIndex(int index) { pkey = index; }

 private:
  int operation;  // create, drop, select, insert, delete
  int object;     // table, index
  string tableName;
  string indexName;
  vector<TreeNode> conditions;  // delete from <tableName> where <conditions>
  // select * from <tableName> where <conditions>
  int pkey;
  vector<attrNode> definitions;  // create <object(table)> <definitions> primary
                                 // key definitions[<pkey>]
  int column;          // create <object(index)> on <tableName>(<column>)
  vector<Union> data;  // insert <data> into <tableName>
  // drop <object> <name>
};

struct attrValue {
  string attrName;
  index_info ii;
  Union value;
  int type;  // 1：int ，2：char* ， 3：float

  attrValue(int type, string attrName, int num, int offset)
      : type(type), attrName(attrName) {
    ii.num = num;
    ii.offset = offset;
  }
}; /*seletct_attr返回的类*/

struct deleted_node {
  int type;
  int att_id;
  Union value;
  deleted_node(int type, int att_id) : type(type), att_id(att_id) {}
}; /*delete返回的类*/

class RecordManager : public BufferManager {
 private:
  int count;
  vector<vector<deleted_node> > del_ind;
  vector<attrValue> sel_attr; /*返回值的引用*/
  index_info insert_ii;

 public:
  RecordManager();
  ~RecordManager();

  void judge(bool &notMatch, vector<TreeNode> &v_tn, vector<attrNode> &v_an,
             BufferNode *p_temp_bn, int &offset);
  void print(vector<attrNode> &v_an, BufferNode *p_temp_bn, int offset);
  void delAndGetValue(vector<vector<deleted_node> > &del_ind,
                      vector<attrNode> &v_an, vector<int> &v_indexNum,
                      map<int, int> &m, BufferNode *p_temp_bn, int &offset);

  void initial_Table(string table_name);
  vector<attrValue> &select_attr(string table_name, vector<attrNode> &v_an,
                                 int num);
  /* 返回某个属性的所有值 */
  int select(string table_name, vector<TreeNode> &v_tn, vector<attrNode> &v_an,
             vector<index_info> &join_ii);
  /* v_an 提供该表所有属性信息 */
  index_info &insert(string table_name, vector<attrAndvalue> &v_aav,
                     vector<attrNode> &v_an);
  /*	*/
  vector<vector<deleted_node> > &delete_tuple(string table_name,
                                              vector<TreeNode> &v_tn,
                                              vector<attrNode> &v_an,
                                              vector<index_info> &join_ii);
  /* 根据不同属性返回若干aav的list */
  int getCount() { return count; }
};

#endif
