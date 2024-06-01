#include "ils.hpp"
#include "defs.hpp"
#include "solution.hpp"

#include <cassert>
#include <chrono>
#include <vector>

int ils_c1 = 1;
int ils_c2 = 13;
int ils_c3 = 1;
int ils_c4 = 22;

static int current_iter[NUM_THREADS] = {0};
int get_current_iter(int thr_id) { return current_iter[thr_id]; }

Solution *ils(Solution *sp, Problem *p, int max_iters, Info_Solution *info,
              int thr_id) {

  sp->_thr_id = thr_id;
  std::vector<unsigned long> temps;

  auto start = std::chrono::steady_clock::now();

  Solution s = (*sp);
  assert(s.check());

  s.complete();
  s.local_search();

  Solution best_solution(&s);
  assert(best_solution.check());

  int k = 1;
  int local_best = s.value();
  current_iter[thr_id] = 0;

#ifdef TIME_LIMIT
  auto start_time = std::chrono::high_resolution_clock::now();
  (void)max_iters;
#endif

  for (current_iter[thr_id] = 0;
#ifndef TIME_LIMIT
       current_iter[thr_id] < max_iters
#endif
       ;
       current_iter[thr_id]++) {

#ifdef TIME_LIMIT
    auto current_time = std::chrono::high_resolution_clock::now();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(
                       current_time - start_time)
                       .count();
    if (seconds >= TIME_LIMIT) {
      break;
    }
#endif

    Solution next_solution(&s);
    assert(next_solution.check());

    next_solution.perturb(ils_c1);
    assert(next_solution.check());

    next_solution.local_search();
    assert(next_solution.check());

    if (next_solution.value() > s.value()) {
      k = 1;
      s = next_solution;

      if (local_best < s.value()) {
        k -= s.size() / ils_c2;
        local_best = s.value();
      }

      if (best_solution.value() < s.value()) {
        best_solution = Solution(&s);

        auto now_time = std::chrono::steady_clock::now();
        temps.push_back(std::chrono::duration_cast<std::chrono::microseconds>(
                            now_time - start)
                            .count());

        assert(best_solution.check());
        k -= s.size() * ils_c3;
      }
    } else if (k <= s.size() / ils_c2) {
      k++;
    } else {
      local_best = s.value();
      s.perturb(ils_c4);
      assert(s.check());
      k = 1;
    }
  }

  assert(s.check());
  assert(best_solution.check());

  auto end = std::chrono::steady_clock::now();

  info->iters = current_iter[thr_id];

  for (auto i : temps) {
    info->best_updated_times.push_back(i);
  }

  info->time =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start)
          .count();

  for (auto i : best_solution.final_solution()) {
    info->items[i] = 1;
  }

  for (int k = 0; k < p->l; k++) {
    int i = p->forfeits_i[k];
    int j = p->forfeits_j[k];
    for (auto i_sol : best_solution.final_solution()) {
      for (auto j_sol : best_solution.final_solution()) {
        if (i != j) {
          if (i == i_sol && j == j_sol) {
            info->forfeits[k] = 1;
          }
        }
      }
    }
  }

  Solution *s_out = new Solution(best_solution);
  return s_out;
}
