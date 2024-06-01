#include "graph.hpp"

#include <algorithm>
#include <assert.h>
#include <cstdlib>
#include <list>

bool edgeCompare(Edge &e1, Edge &e2) { return (e1.v2 < e2.v2); }

Graph::Graph(int n, int l)
  : _weights(n, 0), _values(n, 0), _n(n), _l(l), _adj_l(n, std::vector<Edge>()) {

  _adj_matrix = (int **)calloc(n, sizeof(int *));

  for (int i = 0; i < n; i++) {
    _adj_matrix[i] = (int *)calloc(n, sizeof(int));
  }
}

void Graph::add_edge(int i, int j, int index, int weight) {
  assert(i < _n && j < _n);

  _adj_matrix[i][j] = weight;
  _adj_matrix[j][i] = weight;

  addNeighbor(i, j, index, weight);
  addNeighbor(j, i, index, weight);
}

void Graph::sort() {
  for (int v = 0; v < _n; v++) {
    std::sort(_adj_l[v].begin(), _adj_l[v].end(), edgeCompare);
  }
}

Graph *graph_from_problem(Problem p) {

  Graph *grafo = new Graph(p.n, p.l);

  for (int i = 0; i < p.n; i++) {
    grafo->set_value(i, p.values[i]);
    grafo->set_weight(i, p.weights[i]);
  }

  for (int i = 0; i < p.l; i++) {
    grafo->add_edge(p.forfeits_i[i], p.forfeits_j[i], i, p.forfeits_values[i]);
  }

  grafo->sort();
  return grafo;
}

int Graph::adj_matrix(int a, int b) { return _adj_matrix[a][b]; }
