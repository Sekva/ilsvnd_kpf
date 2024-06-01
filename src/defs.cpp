#include "defs.hpp"
#include "ils.hpp"
#include "solution.hpp"

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>

std::mt19937 *rng = NULL;

int tabu_tenure = 1;

const char *instance_name;
bool instance_name_ok = false;

std::tuple<int, int, int> calculate_weight_value_forfeits(Info_Solution s,
                                                          Problem *p) {

  int total_value = 0;
  int total_weight = 0;
  int total_forfeit = 0;
  int n_forfeits_applied = 0;

  for (int idx_item = 0; idx_item < p->n; idx_item++) {
    total_value += s.items[idx_item] * p->values[idx_item];
    total_weight += s.items[idx_item] * p->weights[idx_item];
  }

  for (int idx_forfeits = 0; idx_forfeits < p->l; idx_forfeits++) {
    total_forfeit +=
        s.forfeits[idx_forfeits] * p->forfeits_values[idx_forfeits];
    n_forfeits_applied += s.forfeits[idx_forfeits];
  }

  return std::make_tuple(total_weight, total_value - total_forfeit,
                         n_forfeits_applied);
}

long rand_seed = time(NULL);

void init_rand() {
  srand(rand_seed);
  rng = new std::mt19937(rand_seed);
}

std::mt19937 *get_rng() { return rng; }

Problem *problem_from_file(const char *filename) {
  Problem *p = new Problem();

  int n, b, l;

  std::vector<int> values;
  std::vector<int> weights;

  std::vector<int> forfeits_i;
  std::vector<int> forfeits_j;
  std::vector<int> values_forfeits;

  std::fstream file;
  file.open(filename, std::ios::in);

  file >> n;
  file >> l;
  file >> b;

  for (int i = 0; i < n; i++) {
    int p_i;
    file >> p_i;
    values.push_back(p_i);
  }

  for (int i = 0; i < n; i++) {
    int w_i;
    file >> w_i;
    weights.push_back(w_i);
  }

  for (int k = 0; k < l; k++) {
    int unused;
    int i_k;
    int j_k;
    int d_k;
    file >> unused; // always 1
    file >> d_k;
    file >> unused; // always 2
    file >> i_k;
    file >> j_k;
    forfeits_i.push_back(i_k);
    forfeits_j.push_back(j_k);
    values_forfeits.push_back(d_k);
  }

  file.close();

  p->forfeits_values = values_forfeits;
  p->weights = weights;
  p->forfeits_i = forfeits_i;
  p->forfeits_j = forfeits_j;
  p->values = values;
  p->b = b;
  p->n = n;
  p->l = l;

  return p;
}

void proccess_args(int argc, char **argv) {

  argc--;
  argv++;

  if (argc < 1) {
    std::cerr << "At least one instance file should be given\n";
    exit(EXIT_FAILURE);
  }

  instance_name = argv[0];
  argc--;
  argv++;

  int idx_arg = -1;
  while ((++idx_arg) < argc) {

    std::string arg(argv[idx_arg]);

    // ---------
    if (arg == "--rand_seed") {
      if (idx_arg >= argc) {
        std::cerr << "Random seed should be given\n";
        exit(EXIT_FAILURE);
      } else {
        idx_arg++;
        rand_seed = atol(argv[idx_arg]);
        if (rand_seed == 0) {
          std::cerr << "Invalid random seed\n";
          exit(EXIT_FAILURE);
        }
      }
    }

    // ---------
    if (arg == "--swap21_filter") {
      if (idx_arg >= argc) {
        std::cerr << "Filter value should be given\n";
        exit(EXIT_FAILURE);
      } else {
        idx_arg++;
        swap21_filter = atoi(argv[idx_arg]);
        if (swap21_filter == 0) {
          std::cerr << "Invalid filter value\n";
          exit(EXIT_FAILURE);
        }
      }
    }

    // ---------
    if (arg == "--ils_c1") {
      if (idx_arg >= argc) {
        std::cerr << "C1 value should be given\n";
        exit(EXIT_FAILURE);
      } else {
        idx_arg++;
        ils_c1 = atoi(argv[idx_arg]);
        if (ils_c1 == 0) {
          std::cerr << "Invalid C1 value\n";
          exit(EXIT_FAILURE);
        }
      }
    }

    // ---------
    if (arg == "--ils_c2") {
      if (idx_arg >= argc) {
        std::cerr << "C2 value should be given\n";
        exit(EXIT_FAILURE);
      } else {
        idx_arg++;
        ils_c2 = atoi(argv[idx_arg]);
        if (ils_c2 == 0) {
          std::cerr << "Invalid C2 value\n";
          exit(EXIT_FAILURE);
        }
      }
    }

    // ---------
    if (arg == "--ils_c3") {
      if (idx_arg >= argc) {
        std::cerr << "C3 value should be given\n";
        exit(EXIT_FAILURE);
      } else {
        idx_arg++;
        ils_c3 = atoi(argv[idx_arg]);
        if (ils_c3 == 0) {
          std::cerr << "Invalid C3 value\n";
          exit(EXIT_FAILURE);
        }
      }
    }

    // ---------
    if (arg == "--ils_c4") {
      if (idx_arg >= argc) {
        std::cerr << "C4 value should be given\n";
        exit(EXIT_FAILURE);
      } else {
        idx_arg++;
        ils_c4 = atoi(argv[idx_arg]);
        if (ils_c4 == 0) {
          std::cerr << "Invalid C4 value\n";
          exit(EXIT_FAILURE);
        }
      }
    }

    // ---------
    if (arg == "--tabu_tenure") {
      if (idx_arg >= argc) {
        std::cerr << "Tabu tenure should be given\n";
        exit(EXIT_FAILURE);
      } else {
        idx_arg++;
        tabu_tenure = atoi(argv[idx_arg]);
        if (tabu_tenure == 0) {
          std::cerr << "Invalid tabu tenure\n";
          exit(EXIT_FAILURE);
        }
      }
    }
    // ---------
  }
}
