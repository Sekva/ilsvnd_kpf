#include "ils.hpp"
#include "solution.hpp"
#include <cstdint>

unsigned int swap21_filter = 1;

inline void Solution::swap_2_1(int v_rm1, int v_rm2, int v_ad) {

  assert(_position[v_rm1] < _solution_size);
  assert(_position[v_rm2] < _solution_size);

  assert(_position[v_ad] >= _solution_size &&
         _position[v_ad] < _solution_size + _free_size);

  this->rm_vertice(v_rm1);
  this->rm_vertice(v_rm2);
  this->add_vertice(v_ad);
}

bool Solution::first_improvement_swap_2_1() {

  assert(_solution_size > 1);
  assert(_free_size > 0);

  int remaining_cap = this->remaining_cap();

  uint64_t solution_heap_size = _solution_heap->n();
  for (uint64_t i = 0; i < solution_heap_size; i++) {
    uint32_t v_i = _solution_heap->get_key(i);
    uint32_t w_i = _solution_heap->get_val(i);

    if (_tabu[v_i] > get_current_iter(_thr_id)) {
      continue;
    }

    if (w_i <= _free_heap->get_val(0) / swap21_filter) {
      continue;
    }

    for (uint64_t j = i + 1; j < solution_heap_size; j++) {
      uint32_t v_j = _solution_heap->get_key(j);
      uint32_t w_j = _solution_heap->get_val(j);

      if (_tabu[v_j] > get_current_iter(_thr_id)) {
        continue;
      }

      for (int k = _solution_size; k < _solution_size + _free_size; k++) {
        int v_k = _solution[k];

        if ((unsigned int)_g->weight(v_k) > remaining_cap + w_i + w_j) {
          continue;
        }

        int in_value =
            _mu[v_k] + _g->_adj_matrix[v_i][v_k] + _g->_adj_matrix[v_j][v_k];
        int out_value = _mu[v_i] + _mu[v_j] + _g->_adj_matrix[v_i][v_j];

        if (in_value > out_value) {
          this->swap_2_1(v_i, v_j, v_k);
          return true;
        }
      }
    }
  }

  return false;
}
