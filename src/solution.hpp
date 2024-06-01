#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include "defs.hpp"
#include "graph.hpp"
#include "heap.hpp"
#include <vector>

extern unsigned int swap21_filter;

class Solution {

public:
  Solution(Graph *g, Problem *p);
  Solution(Solution *s);

  void add_vertice(int v);
  void rm_vertice(int v);

  bool add_random_vertice();

  bool is_maximal();
  bool check() const;
  void recalculate();

  int value() { return _solution_value; }
  int size() { return _solution_size; }
  int forfeits() { return _n_forfeits; }
  int applied_forfeits() { return _n_forfeits_applied; }
  int remaining_cap() const { return _remaining_cap; }

  int free_items_size() { return _free_size; }
  int nonfree_items_size() { return _nonfree_size; }

  std::vector<int> final_solution();
  int solution(int v) { return _solution[v]; }
  int position(int v) { return _position[v]; }

  Graph *graph() { return _g; }
  Problem *problem() { return _p; }

  int swp(int idx) {
    switch (idx) {
    case 1:
      return _swp1;
    case 2:
      return _swp2;
    case 3:
      return _swp3;
    }

    return _swp1;
  }

  void perturb(int k);
  void local_search();
  void complete();

  int mu(int v) { return _mu[v]; }

  // 0-out 1-in
  inline void swap_0_1(int v_ad);
  bool first_improvement_swap_0_1();

  // 1-out 0-in
  inline void swap_1_0(int v_rm);
  bool first_improvement_swap_1_0();

  // 1-out 1-in
  inline void swap_1_1(int v_rm, int v_ad);
  bool first_improvement_swap_1_1();

  // 2-out 1-in
  inline void swap_2_1(int v_rm1, int v_rm2, int v_ad);
  bool first_improvement_swap_2_1();

  void greedy_solution(Info_Solution *s);
  void run_carousel(Info_Solution *s);

  Heap *_solution_heap;
  Heap *_free_heap;

  int _thr_id;

private:
  void add_edge(int index);
  void rm_edge(int index);

  void move_solution_to_free(int v);
  void move_solution_to_nonfree(int v);

  void move_free_to_solution(int v);
  void move_free_to_nonfree(int v);

  void move_nonfree_to_free(int v);
  void move_nonfree_to_solution(int v);

  Graph *_g;
  Problem *_p;

  int *_solution;
  int *_position;

  int _solution_size;
  int _free_size;
  int _nonfree_size;

  int _swp1;
  int _swp2;
  int _swp3;

  void swp_solution_position(int i, int j);

  int _n_forfeits;
  int _n_forfeits_applied;

  int _solution_value;
  int _remaining_cap;

  std::vector<int> _mu;
  std::vector<int> _tabu;

  std::vector<bool> _edge_status;

  void greedy_init();
  Info_Solution _initial_solution;
};

#endif
