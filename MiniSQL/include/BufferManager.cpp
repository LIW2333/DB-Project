#include "BufferManager.h"

BufferNode::BufferNode() {
  this->data_field = nullptr;
  this->offset = 0;
  this->type = TABLE;
  this->dirty = false;
  this->pin = false;
  this->io_cnt = 0;
}

BufferNode::BufferNode(const BufferNode& _buf_node_src) {
  this->data_field = new char[BLOCK_SIZE];
  memcpy(data_field, _buf_node_src.data_field, BLOCK_SIZE);
  this->offset = _buf_node_src.offset;
  this->dirty = _buf_node_src.dirty;
  this->type = _buf_node_src.type;
  this->file_name = _buf_node_src.file_name;
  this->pin = _buf_node_src.pin;
  this->io_cnt = _buf_node_src.io_cnt;
}

BufferNode::~BufferNode() { delete[] data_field; }

BufferManager::BufferManager() {
  this->lru_base = false;
  this->cur_io_num = 0;
  this->buf_pool = new BufferNode[MAX_BLOCK_NUM];
}

BufferManager::~BufferManager() {
  flushAll();
  delete[] buf_pool;
}

void BufferNode::setBufferNode(Type type, string file_name, int offset) {
  flush();
  delete[] data_field;
  data_field = new char[BLOCK_SIZE];
  // clog << "Set buffer node " << hex << data_field << endl;
  memset(data_field, 0, BLOCK_SIZE);
  int del = 2;
  memcpy(data_field, (char*)&del, 4);
  this->file_name = file_name;
  this->offset = offset;
  this->type = type;
  this->io_cnt = 0;
  dirty = false;
  pin = false;
  fstream diskFile;
  diskFile.open(this->file_name, ios::in | ios::out | ios::app | ios::binary);
  if (!diskFile.good()) cerr << "File open error" << endl;
  diskFile.seekg(ios::beg + offset * BLOCK_SIZE);
  diskFile.read(data_field, BLOCK_SIZE);
  diskFile.close();
}

void BufferNode::flush() {
  if (dirty) {
    fstream out;
    out.open(this->file_name, ios::out | ios::in | ios::binary);
    if (!out.good()) cerr << "file open error" << endl;
    out.seekp(ios::beg + this->offset * BLOCK_SIZE);
    out.write(data_field, BLOCK_SIZE);
    out.close();
  }
}

bool BufferNode::isMatch(Type type, string file_name, int offset) {
  return (this->type == type && this->file_name == file_name &&
          this->offset == offset);
}

bool BufferNode::isEmpty() { return (this->data_field == NULL); }

void BufferNode::writeBlock(string data, int length, int offset) {
  this->dirty = true;
  const char* c = data.c_str();
  memcpy(this->data_field + offset, c, data.size());
  memset(this->data_field + offset + data.size(), 0, length - data.size());
}

void BufferNode::readBlock(string& dest, int length, int offset) {
  char* c = new char[length + 1];
  memcpy(c, this->data_field + offset, length);
  c[length] = '\0';
  dest = c;
  delete[] c;
}

void BufferManager::flushAll() {
  for (int i = 0; i < MAX_BLOCK_NUM; ++i) {
    buf_pool[i].flush();
  }
}

void BufferManager::usingBlock(int i) {
  this->buf_pool[i].io_cnt = ++this->cur_io_num;
}

int BufferManager::leastRecentlyUsed() {
  // cerr << "Least Recently Used" << this->min_io_num << endl;
  return this->min_index;
}

BufferNode& BufferManager::getBlock(Type type, string file_name, int offset) {
  this->lru_base = false;

  for (int i = 0; i < MAX_BLOCK_NUM; ++i) {
    if (buf_pool[i].isEmpty()) {
      buf_pool[i].setBufferNode(type, file_name, offset);
      usingBlock(i);
      return buf_pool[i];
    }
    if (!this->lru_base && !buf_pool[i].pin) {  // Find first not pinned
      min_io_num = buf_pool[i].io_cnt;
      min_index = i;
      this->lru_base = true;
    }
    if (!buf_pool[i].pin && buf_pool[i].io_cnt < min_io_num) {
      min_io_num = buf_pool[i].io_cnt;
      min_index = i;
    }
    if (buf_pool[i].isMatch(type, file_name, offset)) {
      usingBlock(i);
      return buf_pool[i];
    }
  }
  // full
  int i = leastRecentlyUsed();
  buf_pool[i].setBufferNode(type, file_name, offset);
  usingBlock(i);
  return buf_pool[i];
}

BufferNode* BufferManager::getBlockPointer(Type type, string file_name,
                                           int offset) {
  return &getBlock(type, file_name, offset);
}
