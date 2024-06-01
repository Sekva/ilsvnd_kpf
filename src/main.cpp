#include "defs.hpp"
#include "graph.hpp"
#include "ils.hpp"
#include "prints.hpp"
#include "solution.hpp"

#include <cassert>
#include <cstdlib>
#include <string>
#include <thread>

int main(int argc, char **argv) {

#ifndef NUM_THREADS
  std::cerr << "Threads number should be given...\n";
  exit(-1);
#endif

  if (NUM_THREADS < 0) {
    std::cerr << "Invalid thread number...\n";
    exit(-1);
  }

  proccess_args(argc, argv);

  assert(0);

  Problem *p = problem_from_file(instance_name);

  init_rand();
  std::cout << "Executing " << instance_name << " on " << NUM_THREADS
            << " threads" << std::endl;
  Graph *graph = graph_from_problem(*p);

  Info_Solution *info_solutions =
      (Info_Solution *)calloc(NUM_THREADS + 1, sizeof(Info_Solution));
  std::vector<Solution *> solutions(NUM_THREADS);
  std::vector<std::thread> threads(NUM_THREADS);

  for (unsigned long i = 0; i < threads.size(); i++) {
    threads[i] = std::thread([i, &info_solutions, p, graph, &solutions]() {
      Info_Solution sol_ils = info_solutions[i];
      sol_ils.instance_name = std::string(instance_name);
      sol_ils.items = std::vector<int>(p->n);
      sol_ils.forfeits = std::vector<int>(p->l);
      sol_ils.best_updated_times = std::vector<int>();
      sol_ils.seed = rand_seed;

      Solution s(graph, p);
      s.perturb(s.size() / 2);
      assert(s.check());
      Solution *out = ils(&s, p, 1000, &sol_ils, i);
      solutions[i] = out;

      print_solution(sol_ils, p);
    });
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    threads[i].join();
  }

  return 0;
}
