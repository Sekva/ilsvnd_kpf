#include "solution.hpp"
#include "defs.hpp"
#include "graph.hpp"
#include "heap.hpp"
#include "ils.hpp"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <vector>

Solution::Solution(Graph *g, Problem *p) {

  _g = g;
  _p = p;

  _solution = (int *)calloc(_g->n(), sizeof(int));
  _position = (int *)calloc(_g->n(), sizeof(int));

  _solution_size = 0;
  _free_size = _g->n();
  _nonfree_size = 0;

  _swp1 = -1;
  _swp2 = -1;
  _swp3 = -1;

  _n_forfeits = _g->m();
  _n_forfeits_applied = 0;

  _solution_value = 0;
  _remaining_cap = _p->b;

  _mu = std::vector<int>(_g->n(), 0);

#ifdef TABU
  _tabu = std::vector<int>(_g->n(), -1);
#endif

  _solution_heap = new Heap(_g->n());
  _free_heap = new Heap(_g->n());

  for (int i = 0; i < _g->n(); i++) {
    _solution[i] = i;
    _position[i] = i;
    _mu[i] = _g->value(i);
    _free_heap->insert(COMPOSE(i, _g->weight(i)));
  }

  _edge_status = std::vector<bool>(_g->m(), false);

  auto start = std::chrono::steady_clock::now();

  this->greedy_init();

  auto diff = std::chrono::steady_clock::now() - start;

  Info_Solution s;
  s.items = this->final_solution();
  s.time = std::chrono::duration_cast<std::chrono::microseconds>(diff).count();

  _initial_solution = s;

  _thr_id = 0;
}

Solution::Solution(Solution *s) {

  _g = s->_g;
  _p = s->_p;

  _solution = (int *)calloc(_g->n(), sizeof(int));
  _position = (int *)calloc(_g->n(), sizeof(int));

  _solution_size = s->_solution_size;
  _free_size = s->_free_size;
  _nonfree_size = s->_nonfree_size;

  _swp1 = s->_swp1;
  _swp2 = s->_swp2;
  _swp3 = s->_swp3;

  _n_forfeits = s->_n_forfeits;
  _n_forfeits_applied = s->_n_forfeits_applied;

  _solution_value = s->_solution_value;
  _remaining_cap = s->_remaining_cap;

  _mu = std::vector<int>(_g->n(), 0);

#ifdef TABU
  _tabu = std::vector<int>(_g->n(), -1);
#endif

  _solution_heap = new Heap(s->_solution_heap);
  _free_heap = new Heap(s->_free_heap);

  for (int i = 0; i < _g->n(); i++) {
    _solution[i] = s->_solution[i];
    _position[i] = s->_position[i];
    _mu[i] = s->_mu[i];

#ifdef TABU
    _tabu[i] = s->_tabu[i];
#endif
  }

  _edge_status = std::vector<bool>(_g->m(), false); // Serio que era isso?

  for (long unsigned int i = 0; i < s->_edge_status.size(); i++) {
    _edge_status[i] = s->_edge_status[i];
  }

  _initial_solution = s->_initial_solution;
  _thr_id = s->_thr_id;
}

void Solution::greedy_init() {
  while (_free_size > 0) {
    std::vector<int> x_iter;
    for (int idx = 0; idx < _free_size; idx++) {
      int vertice = _solution[_solution_size + idx];

      if (_g->weight(vertice) <= this->remaining_cap()) {
        x_iter.push_back(vertice);
      }
    }

    if (x_iter.empty()) {
      return;
    }

    int best_vertice = -1;
    float ratio_best_vertice = -999999;

    for (int vertice : x_iter) {
      int value = _g->value(vertice);
      int weight = _g->weight(vertice);

      for (Edge edge_neighbor : _g->adj_l(vertice)) {
        int neighbor = edge_neighbor.v2;
        int neighbor_position = _position[neighbor];

        if (neighbor_position < _solution_size) {
          value = value - edge_neighbor.weight;
        }
      }

      float ratio = ((float)value) / ((float)weight);

      if (ratio > ratio_best_vertice) {
        best_vertice = vertice;
        ratio_best_vertice = ratio;
      }
    }

    if (ratio_best_vertice < 0) {
      return;
    }

    assert(best_vertice >= 0);
    this->add_vertice(best_vertice);
  }
}

void Solution::swp_solution_position(int i, int j) {
  std::swap(_position[_solution[i]], _position[_solution[j]]);
  std::swap(_solution[i], _solution[j]);
}

void Solution::move_free_to_solution(int v) {
  assert(v >= 0 && v < _free_size);
  this->swp_solution_position(_solution_size, _solution_size + v);
  _solution_size++;
  _free_size--;
}

void Solution::move_solution_to_nonfree(int v) {
  assert(v >= 0 && v < _solution_size);

  this->swp_solution_position(v, _solution_size - 1);
  _solution_size--;

  this->swp_solution_position(_solution_size, _solution_size + _free_size);

  _nonfree_size++;
}

void Solution::move_free_to_nonfree(int v) {
  assert(v >= 0 && v < _free_size);

  this->swp_solution_position(_solution_size + v,
                              _solution_size + _free_size - 1);

  _free_size--;
  _nonfree_size++;
}

void Solution::move_nonfree_to_solution(int v) {
  assert(v >= 0 && v < _nonfree_size);

  this->swp_solution_position(_solution_size + _free_size + v,
                              _solution_size + _free_size);

  this->swp_solution_position(_solution_size + _free_size, _solution_size);

  _solution_size++;
  _nonfree_size--;
}

void Solution::move_nonfree_to_free(int v) {
  assert(v >= 0 && v < _nonfree_size);
  this->swp_solution_position(_solution_size + _free_size + v,
                              _solution_size + _free_size);

  _nonfree_size--;
  _free_size++;
}

void Solution::move_solution_to_free(int v) {
  assert(v >= 0 && v < _solution_size);

  this->swp_solution_position(v, _solution_size - 1);

  _solution_size--;
  _free_size++;
}

std::vector<int> Solution::final_solution() {
  std::vector<int> r(_solution_size);

  for (int i = 0; i < _solution_size; i++) {
    r[i] = _solution[i];
  }

  return r;
}

void Solution::add_edge(int index) {
  this->_edge_status[index] = true;
  _n_forfeits_applied += 1;
}

void Solution::rm_edge(int index) {
  this->_edge_status[index] = false;
  _n_forfeits_applied -= 1;
}

void Solution::add_vertice(int v) {

  int vertice = v;
  int idx_vertice = _position[vertice];

  assert(idx_vertice >= _solution_size);
  assert(idx_vertice <= _solution_size + _free_size);

  int idx_vertice_on_free = idx_vertice - _solution_size;

  int vertice_weight = _g->weight(vertice);
  int vertice_value = _mu[vertice];

  for (Edge neighboor_edge : _g->adj_l(vertice)) {

    int neighboor = neighboor_edge.v2;
    int neighboor_position = _position[neighboor];
    _mu[neighboor] -= neighboor_edge.weight;

    if (neighboor_position < _solution_size) {
      this->add_edge(neighboor_edge.index);
    }
  }

  move_free_to_solution(idx_vertice_on_free);

  _free_heap->remove(COMPOSE(v, vertice_weight));
  _solution_heap->insert(COMPOSE(v, vertice_weight));

  _remaining_cap -= vertice_weight;
  _solution_value += vertice_value;

#ifdef TABU
  _tabu[vertice] = get_current_iter(_thr_id) + tabu_tenure;
#endif

  assert(this->check());
}

void Solution::rm_vertice(int v) {

  int vertice = v;
  int idx_vertice = _position[vertice];

  assert(idx_vertice < _solution_size);

  int vertice_weight = _g->weight(vertice);
  int vertice_value = _mu[vertice];

#ifdef TABU
  _tabu[vertice] = -1;
#endif

  for (Edge neighboor_edge : _g->adj_l(vertice)) {
    int neighboor_position = _position[neighboor_edge.v2];
    _mu[neighboor_edge.v2] += neighboor_edge.weight;

    if (neighboor_position < _solution_size) {
      this->rm_edge(neighboor_edge.index);
    }
  }

  move_solution_to_free(idx_vertice);
  _solution_heap->remove(COMPOSE(v, vertice_weight));
  _free_heap->insert(COMPOSE(v, vertice_weight));

  _remaining_cap += vertice_weight;
  _solution_value -= vertice_value;

  assert(this->check());
}

bool Solution::is_maximal() {

  for (int free_idx = _solution_size; free_idx < _free_size; free_idx++) {
    int free_vertice = _solution[free_idx];

    if (_g->weight(free_vertice) < this->remaining_cap() &&
        _mu[free_vertice] > 0) {
      return false;
    }
  }

  return true;
}

bool Solution::check() const {

  int new_cap = _p->b;

  for (int i = 0; i < _solution_size; i++) {
    new_cap -= _g->weight(_solution[i]);
  }

  if (new_cap != this->remaining_cap()) {
    std::cerr << "Wrong remaining capacity\n";
    std::cerr << "current remaining capacity: " << this->remaining_cap()
              << "\n";
    std::cerr << "correct remaining capacity: " << new_cap << "\n";
    return false;
  }

  std::vector<int> nmu(_g->n());
  for (int i = 0; i < _g->n(); i++) {
    nmu[i] = _g->value(i);
  }

  std::vector<bool> new_edges(_g->m());
  for (int i = 0; i < _g->m(); i++) {
    new_edges[i] = false;
  }

  for (int pos_vertice = 0; pos_vertice < _solution_size; pos_vertice++) {
    int vertice = _solution[pos_vertice];

    for (Edge naighboor_edge : _g->adj_l(vertice)) {
      int neighboor = naighboor_edge.v2;
      int neighboot_position = _position[neighboor];

      nmu[neighboor] -= naighboor_edge.weight;

      if (neighboot_position < _solution_size) {
        new_edges[naighboor_edge.index] = true;
      }
    }
  }

  for (int i = 0; i < _g->n(); i++) {
    if (_mu[i] != nmu[i]) {
      std::cerr << "mu, precalculated values wrong\n";
      return false;
    }
  }

  for (long unsigned int i = 0; i < _edge_status.size(); i++) {
    if (_edge_status[i] != new_edges[i]) {
      std::cerr << "edges differ\n";
      return false;
    }
  }

  if (this->remaining_cap() < 0) {
    std::cerr << "negative capacity\n";
    std::cerr << "Remaining cap: " << this->remaining_cap() << "\n";
    return false;
  }

  return true;
}

bool Solution::add_random_vertice() {

  if (_free_size < 1) {
    return false;
  }

  std::vector<int> candidates;
  candidates.reserve(_free_size);
  for (int i = _solution_size; i < _solution_size + _free_size; i++) {
    if (_mu[_solution[i]] > 0) {
      candidates.push_back(_solution[i]);
    }
  }

  std::shuffle(candidates.begin(), candidates.end(), (*get_rng()));

  for (long unsigned int i = 0; i < candidates.size(); i++) {
    int weight = _g->weight(candidates[i]);
    if (weight <= this->remaining_cap()) {
      this->add_vertice(candidates[i]);
      return true;
    }
  }

  return false;
}

void Solution::local_search() {

  int value_before = _solution_value;

  int i = 0;

#ifdef SWAPS
  i = 0;
  while (i++ < INT32_MAX) {
    if (this->first_improvement_swap_1_1()) {
      continue;
    }
    if (this->first_improvement_swap_2_1()) {
      continue;
    }
    break;
  }
#endif

#ifdef ADDRM
  i = 0;
  while (i++ < INT32_MAX) {
    if (this->first_improvement_swap_0_1()) {
      continue;
    }
    if (this->first_improvement_swap_1_0()) {
      continue;
    }
    break;
  }
#endif

  int diff_value = _solution_value - value_before;
  assert(diff_value >= 0);
  diff_value++;
}

void Solution::complete() {
  while (!this->is_maximal()) {
    this->add_random_vertice();
    assert(this->check());
  }
}

void Solution::perturb(int k) {

#ifdef TABU

  std::vector<int> maybe_out;
  int n_removed = 0;

  for (int i = 0; i < _solution_size; i++) {
    maybe_out.push_back(_solution[i]);
  }
  std::shuffle(maybe_out.begin(), maybe_out.end(), (*get_rng()));

  for (long unsigned int i = 0; i < maybe_out.size(); i++) {

    int v_rm = maybe_out[i];
    if (_tabu[v_rm] > get_current_iter(_thr_id)) {
      continue;
    }

    rm_vertice(v_rm);
    n_removed++;
    if (n_removed == k || _solution_size == 0) {
      break;
    }
  }

#else

  for (int i = 0; i < k; i++) {
    rm_vertice(_solution[rand() % _solution_size]);
    if (_solution_size == 0) {
      break;
    }
  }

#endif

  std::vector<int> maybe_in;
  for (int i = _solution_size; i < _solution_size + _free_size; i++) {
    maybe_in.push_back(_solution[i]);
  }
  std::shuffle(maybe_in.begin(), maybe_in.end(), (*get_rng()));

  int n_added = 0;

  for (long unsigned int i = 0; i < maybe_in.size(); i++) {
    if (this->remaining_cap() >= _g->weight(maybe_in[i])) {
      this->add_vertice(maybe_in[i]);
      n_added++;
      if (n_added == k) {
        break;
      }
    }
  }

  while (this->remaining_cap() < 0) {
    rm_vertice(_solution[rand() % _solution_size]);
    if (_solution_size == 0) {
      break;
    }
  }
}

void Solution::greedy_solution(Info_Solution *s) {

  s->time = _initial_solution.time;

  for (auto i : _initial_solution.items) {
    s->items[i] = 1;
  }

  for (int k = 0; k < _p->l; k++) {
    int i = _p->forfeits_i[k];
    int j = _p->forfeits_j[k];
    for (auto i_sol : _initial_solution.items) {
      for (auto j_sol : _initial_solution.items) {
        if (i != j) {
          if (i == i_sol && j == j_sol) {
            s->forfeits[k] = 1;
          }
        }
      }
    }
  }
}
