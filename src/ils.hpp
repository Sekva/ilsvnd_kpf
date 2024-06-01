#ifndef ILS_HPP__
#define ILS_HPP__

#include "defs.hpp"
#include "solution.hpp"

extern bool graphic_ils;

extern int ils_c1;
extern int ils_c2;
extern int ils_c3;
extern int ils_c4;
int get_current_iter(int thr_id);

Solution *ils(Solution *sp, Problem *p, int max_iters, Info_Solution *s,
              int thr_id);

#endif
