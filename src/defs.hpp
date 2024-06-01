#ifndef NUM_THREADS
#define NUM_THREADS 1
#endif

#ifndef DEFS_HPP
#define DEFS_HPP

#include <random>
#include <string>
#include <tuple>
#include <vector>

extern int tabu_tenure;
extern long rand_seed;

#define USE_HEAP 1

typedef struct solution_t {
  std::string instance_name;
  std::vector<int> items;
  std::vector<int> forfeits;
  std::vector<int> best_updated_times;
  unsigned long time; // in microseconds
  unsigned long iters;
  unsigned long seed;
} Info_Solution;

typedef struct problem_t {

  int n, b, l;

  std::vector<int> values;
  std::vector<int> weights;

  std::vector<int> forfeits_i;
  std::vector<int> forfeits_j;
  std::vector<int> forfeits_values;

} Problem;

std::tuple<int, int, int> calculate_weight_value_forfeits(Info_Solution s,
                                                          Problem *p);
std::mt19937 *get_rng();
void init_rand();

extern const char *instance_name;

Problem *problem_from_file(const char *filename);
void proccess_args(int argc, char **argv);

#endif
