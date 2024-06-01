#ifndef PRINTS_HPP
#define PRINTS_HPP

#include "defs.hpp"
#include "solution.hpp"

#include <ctime>
#include <thread>
#include <vector>

void print_problem(Problem *p);
void print_solution(Info_Solution s, Problem *p);
void print_org_table(const char *org, std::vector<Info_Solution> solutions,
                     Problem *p, const char *inst);

void fill_info_print(Solution *solution, Problem *p,
                     std::chrono::steady_clock::time_point start,
                     Info_Solution *info_solutions);

#endif
