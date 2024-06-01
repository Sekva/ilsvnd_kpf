#include "prints.hpp"
#include "defs.hpp"
#include "solution.hpp"

void print_problem(Problem *p) {
  std::cerr << "Number of items: " << p->n << "\n";
  std::cerr << "Knapsack capacity: " << p->b << "\n";
  std::cerr << "Number of forfeits: " << p->l << "\n";

  for (int i = 0; i < p->n; i++) {
    std::cerr << "Value and weight of item " << i << ": " << p->values[i]
              << ", " << p->weights[i] << "\n";
  }

  for (int i = 0; i < p->l; i++) {
    std::cerr << "Forfeit between " << p->forfeits_i[i] << " and "
              << p->forfeits_j[i] << ": " << p->forfeits_values[i] << "\n";
  }
}

void print_solution(Info_Solution s, Problem *p) {

  int total_value = 0;
  int total_weight = 0;
  int forfeits_applied = 0;
  std::tie(total_weight, total_value, forfeits_applied) =
      calculate_weight_value_forfeits(s, p);

  std::cerr << "seed: " << s.seed << "\n";

  std::cerr << s.iters << " iterations\n";

  std::cerr << "Best update times (micros): ";
  for (auto i : s.best_updated_times) {
    std::cerr << i << ", ";
  }
  std::cerr << std::endl;

  std::cerr << "Solution by ILS of " << s.instance_name << " in "
            << s.time / 1000 << "ms.\n";

  std::cerr << "With total weight of " << total_weight << " (max of " << p->b
            << ")\n";
  std::cerr << "With " << forfeits_applied << " forfeits applied "
            << "\n";

  int c = 0;

  for (unsigned long i = 0; i < s.items.size(); i++) {
    if (s.items[i]) {
      std::cerr << i << ", ";
    }
    c += s.items[i];
  }

  if (p->b < total_weight) {
    std::cerr << "TOO HEAVY\n";
  }
  std::cerr << "\nWith " << c << " items\n";
  std::cerr << "With total value of " << total_value << "\n\n\n";
  std::cout << total_value << std::endl;
}

void fill_info_print(Solution *solution, Problem *p,
                     std::chrono::steady_clock::time_point start,
                     Info_Solution *info_solutions) {

  Info_Solution sol_ils = info_solutions[0];
  sol_ils.instance_name = std::string(instance_name);
  sol_ils.items = std::vector<int>(p->n);
  sol_ils.forfeits = std::vector<int>(p->l);
  sol_ils.best_updated_times = std::vector<int>();
  sol_ils.seed = rand_seed;

  for (auto i : solution->final_solution()) {
    sol_ils.items[i] = 1;
  }

  for (int k = 0; k < p->l; k++) {
    int i = p->forfeits_i[k];
    int j = p->forfeits_j[k];
    for (auto i_sol : solution->final_solution()) {
      for (auto j_sol : solution->final_solution()) {
        if (i != j) {
          if (i == i_sol && j == j_sol) {
            sol_ils.forfeits[k] = 1;
          }
        }
      }
    }
  }

  auto end = std::chrono::steady_clock::now();

  sol_ils.time =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start)
          .count();

  print_solution(sol_ils, p);
}
