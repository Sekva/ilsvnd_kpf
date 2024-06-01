/*
 *
 * Graph: Graph data structure (adjacency list)
 *
 *  Created on: 21/07/2019
 *      Author: bruno
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include "defs.hpp"
#include <algorithm>
#include <assert.h>
#include <string>
#include <vector>

#include <fstream>
#include <iostream>
#include <random>
#include <string>

struct Edge {
  Edge(int v1, int v2, int index, int weight) {
    this->v1 = v1;
    this->v2 = v2;
    this->index = index;
    this->weight = weight;
  };
  int v1, v2;
  int weight;
  int index;
};

class Graph {

public:
  // return the number of edges

  inline int m() { return _l; }

  // return the number of vertices

  inline int n() { return _n; }

  // return the weight of vertice v
  inline int weight(int v) { return _weights[v]; }

  // return the value of vertice v
  inline int value(int v) { return _values[v]; }

  void set_weight(int v, int weight) { _weights[v] = weight; }
  void set_value(int v, int value) { _values[v] = value; }

  // return the adjency list of vertex i

  std::vector<Edge> &adj_l(int i) {
    assert(i < _n);
    return _adj_l[i];
  }

  Graph(int n, int l);

  void add_edge(int i, int j, int index, int weight);

  // sort adjacency list

  void sort();

  int **_adj_matrix; // matriz adjacencia
  int adj_matrix(int a, int b);

private:
  std::vector<int> _weights;
  std::vector<int> _values;

  int _n; // number of vertices
  int _l; // number of edges

  std::vector<std::vector<Edge>> _adj_l; // adjaceny list

  void addNeighbor(int i, int j, int index, int weight) {
    auto a = Edge(i, j, index, weight);
    _adj_l[i].push_back(a);
  }

}; // class Graph

Graph *graph_from_problem(Problem p);

#endif // #ifndef GRAPH_H_
