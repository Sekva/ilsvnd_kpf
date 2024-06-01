#include "ils.hpp"
#include "solution.hpp"

inline void Solution::swap_1_1(int v_rm, int v_ad) {

  assert(_position[v_rm] < _solution_size);
  assert(_position[v_ad] >= _solution_size &&
         _position[v_ad] < _solution_size + _free_size);

  this->rm_vertice(v_rm);
  this->add_vertice(v_ad);
}

bool Solution::first_improvement_swap_1_1() {

  assert(_solution_size > 0);
  assert(_free_size > 0);

  for (int position_v_rm = 0; position_v_rm < _solution_size; position_v_rm++) {
    int v_rm = _solution[position_v_rm];
    int out_value = _mu[v_rm];

#ifdef TABU
    if (_tabu[v_rm] > get_current_iter(_thr_id)) {
      continue;
    }
#endif

    for (int position_v_ad = _solution_size;
         position_v_ad < _solution_size + _free_size; position_v_ad++) {

      int v_ad = _solution[position_v_ad];
      int in_value = _mu[v_ad];

      if (_g->weight(v_ad) > this->remaining_cap() + _g->weight(v_rm)) {
        continue;
      }

      in_value += _g->_adj_matrix[v_rm][v_ad];

      if (in_value > out_value) {
        this->swap_1_1(v_rm, v_ad);
        return true;
      }
    }
  }

  return false;
}
