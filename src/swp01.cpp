#include "solution.hpp"

inline void Solution::swap_0_1(int v_ad) {
  assert(_position[v_ad] >= _solution_size &&
         _position[v_ad] < _solution_size + _free_size);
  this->add_vertice(v_ad);
}

bool Solution::first_improvement_swap_0_1() {
  assert(_free_size > 0);

  for (int position_v_ad = _solution_size;
       position_v_ad < _solution_size + _free_size; position_v_ad++) {

    int v_ad = _solution[position_v_ad];

    if (_g->weight(v_ad) > this->remaining_cap()) {
      continue;
    }

    if (_mu[v_ad] > 0) {
      this->swap_0_1(v_ad);
      return true;
    }
  }

  return false;
}
