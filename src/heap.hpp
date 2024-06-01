#ifndef HEAP_HPP
#define HEAP_HPP

#include <cstdint>
#include <iostream>

#include <queue>

#define COMPOSE(key, value) _COMPOSE((uint64_t)value, (uint32_t)key)
#define _COMPOSE(key, value) ((uint64_t)((key << 32) | value))
#define KEY(composed) (composed & 0xFFFFFFFF)
#define VALUE(composed) (composed >> 32)

#define MAX(a, b) (a > b ? a : b)

class Heap {

private:
  uint64_t *_heap;
  uint64_t *_pos;

  uint64_t _n;
  uint64_t _cap;

  void swap(uint64_t val1, uint64_t val2);

  void heapify(uint32_t i);

public:
  uint64_t n() { return _n; }
  uint64_t *heap() { return _heap; }

  Heap(uint32_t cap);
  Heap(Heap *h);
  void insert(uint64_t val);
  void remove(uint64_t val);
  uint64_t min();
  void print();

  uint32_t get_key(uint32_t index);
  uint32_t get_val(uint32_t index);
};
#endif
