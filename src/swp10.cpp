#include "ils.hpp"
#include "solution.hpp"

inline void Solution::swap_1_0(int v_rm) {
  assert(_position[v_rm] < _solution_size);
  this->rm_vertice(v_rm);
}

bool Solution::first_improvement_swap_1_0() {
  assert(_solution_size > 0);

  for (int position_v_rm = 0; position_v_rm < _solution_size; position_v_rm++) {

    int v_rm = _solution[position_v_rm];
    int out_value = _mu[v_rm];

#ifdef TABU
    if (_tabu[v_rm] > get_current_iter(_thr_id)) {
      continue;
    }
#endif

    if (out_value < 0) {
      this->swap_1_0(v_rm);
      return true;
    }
  }

  return false;
}
