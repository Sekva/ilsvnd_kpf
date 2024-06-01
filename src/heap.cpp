#include "heap.hpp"
#include <cstdint>
#include <cstdlib>
#include <iostream>

uint32_t parent(uint32_t i) { return (i - 1) / 2; }
uint32_t left(uint32_t i) { return (2 * i + 1); }
uint32_t right(uint32_t i) { return (2 * i + 2); }

Heap::Heap(uint32_t cap) {
  _heap = new uint64_t[cap];
  _pos = new uint64_t[cap];

  _cap = cap;
  _n = 0;
}

Heap::Heap(Heap *h) {
  _heap = new uint64_t[h->_cap];
  _pos = new uint64_t[h->_cap];

  _cap = h->_cap;
  _n = h->_n;

  for (uint32_t i = 0; i < _cap; i++) {
    _heap[i] = h->_heap[i];
    _pos[i] = h->_pos[i];
  }
}

void Heap::insert(uint64_t val) {

  if (_n == _cap) {
    return;
  }

  _n += 1;
  uint32_t index = _n - 1;
  _heap[index] = val;
  _pos[KEY(val)] = index;

  while (index != 0 && _heap[parent(index)] < _heap[index]) {
    swap(_heap[parent(index)], _heap[index]);
    index = parent(index);
  }
}
void Heap::remove(uint64_t val) {

  uint32_t index = _pos[KEY(val)];

  _heap[index] = COMPOSE(KEY(_heap[index]), 0xFFFFFFFF);

  while (index != 0 && _heap[parent(index)] < _heap[index]) {
    swap(_heap[parent(index)], _heap[index]);
    index = parent(index);
  }

  if (_n == 0) {
    return;
  } else if (_n == 1) {
    _n = 0;
  } else {
    _heap[0] = _heap[_n - 1];
    _pos[KEY(_heap[_n - 1])] = 0;
    _n = _n - 1;
    heapify(0);
  }
}

void Heap::print() {

  std::cerr << _n << " elements\n";

  for (uint32_t i = 0; i < _n; i++) {
    std::cerr << KEY(_heap[i]) << ", ";
  }

  std::cerr << "\n";
}

void Heap::swap(uint64_t val1, uint64_t val2) {
  uint32_t pos_val1 = _pos[KEY(val1)];
  uint32_t pos_val2 = _pos[KEY(val2)];

  uint64_t t = _heap[pos_val1];
  _heap[pos_val1] = _heap[pos_val2];
  _heap[pos_val2] = t;

  _pos[KEY(val1)] = pos_val2;
  _pos[KEY(val2)] = pos_val1;
}

void Heap::heapify(uint32_t i) {

  uint32_t l = left(i);
  uint32_t r = right(i);
  uint32_t largest = i;

  if (l < _n && _heap[l] > _heap[i])
    largest = l;

  if (r < _n && _heap[r] > _heap[largest])
    largest = r;

  if (largest != i) {
    swap(_heap[i], _heap[largest]);
    heapify(largest);
  }
}

uint64_t Heap::min() {

  uint64_t min = _heap[_n / 2];

  for (uint64_t i = 1 + _n / 2; i < _n; i++) {
    uint64_t t = _heap[i];
    min = (min < t ? min : t);
  }

  return min;
}

uint32_t Heap::get_key(uint32_t index) { return KEY(_heap[index]); }
uint32_t Heap::get_val(uint32_t index) { return VALUE(_heap[index]); }
