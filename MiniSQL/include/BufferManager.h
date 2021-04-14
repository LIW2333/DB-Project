#ifndef _BUFFERMANAGER_H
#define _BUFFERMANAGER_H

#include "MiniSQL.h"

enum Type { TABLE, INDEX };

class BufferNode {
 public:
  BufferNode();
  BufferNode(const BufferNode& _buf_node_src);
  virtual ~BufferNode();

  bool pin;
  int io_cnt;

  void setBufferNode(Type type, string file_name, int offset);
  void flush();
  bool isMatch(Type type, string file_name, int offset);
  bool isEmpty();
  template <class T>
  void writeBlock(T data, int length, int offset) {
    this->dirty = true;
    memcpy(this->data_field + offset, (char*)&data, length);
  }
  void writeBlock(string data, int length, int offset);
  template <class T>
  void readBlock(T& dest, int length, int offset) {
    memcpy((char*)&dest, this->data_field + offset, length);
  }
  void readBlock(string& dest, int length, int offset);

 protected:
  char* data_field;
  int offset;
  Type type;
  bool dirty;
  string file_name;
};

class BufferManager {
 public:
  BufferManager();
  virtual ~BufferManager();
  int leastRecentlyUsed();
  void flushAll();
  void usingBlock(int i);
  BufferNode& getBlock(Type type, string file_name, int offset);
  BufferNode* getBlockPointer(Type type, string file_name, int offset);

 protected:
  BufferNode* buf_pool;
  int cur_io_num;
  int min_io_num;
  bool lru_base;
  int min_index;
};

#endif