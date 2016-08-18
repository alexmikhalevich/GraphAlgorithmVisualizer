#ifndef CGRAPH_HPP
#define CGRAPH_HPP

#include <iostream>
#include <assert.h>
#include <vector>
#include "ialgorithm.h"
#include "cexception.hpp"

enum EState {
       NEW,
       PROCESSING,
       VISITED
};

class CGraph {
public:
       struct StepSlice;
private:
       class Vertex;
       class Edge;
       std::vector<Vertex*> m_vertices;
       std::vector<Edge*> m_edges;
       std::vector<StepSlice> m_steps;
       int m_algo_step;
       bool m_oriented;
       bool m_weighted;

       class Edge {         //TODO: add capacity
       private:
              Vertex* m_source;
              Vertex* m_destination;
              int  m_weight;
              int m_id;
              EState m_state;
              bool m_is_visible;
       public:
              Edge(Vertex* s, Vertex* d) : m_source(s), m_destination(d), m_weight(0), m_id(0), m_state(EState::NEW) {}
              Edge(Vertex* s, Vertex* d, int id) : m_source(s), m_destination(d), m_weight(0), m_id(id), m_state(EState::NEW) {}
              Edge(Vertex* s, Vertex* d, int w) : m_source(s), m_destination(d), m_weight(w), m_id(0), m_state(EState::NEW) {}
              Edge(Vertex* s, Vertex* d, int w, int id) : m_source(s), m_destination(d), m_weight(w), m_id(id), m_state(EState::NEW) {}
              Edge() = delete;
              void set_state(EState state) { m_state = state; }
              void set_weight(int w) { m_weight = w; }
              void remove() { m_is_visible = false; }
              void reborn() { m_is_visible = true; }
              int get_dest_id() const { return m_destination->get_id(); }
              int get_source_id() const { return m_source->get_id(); }
              int get_id() const { return m_id; }
              int get_weight() const { return m_weight; }
              bool visible() const { return m_is_visible; }
       };

       class Vertex {
       private:
              std::vector<Edge*> m_inEdges;
              std::vector<Edge*> m_outEdges;
              int m_id;
              EState m_state;
              bool m_is_visible;
       public:
              Vertex(int id) : m_id(id), m_state(EState::NEW) {}
              Vertex(const std::vector<Edge*>& in, const std::vector<Edge*>& out, int id) : m_inEdges(in),
                     m_outEdges(out), m_id(id), m_state(EState::NEW) {}
              Vertex() = delete;
              ~Vertex() {
                     for(Edge* e : m_inEdges) if(e) delete e;
                     for(Edge* e : m_outEdges) if(e) delete e;
              }
              void add_in_edge(Edge* e) { m_inEdges.push_back(e); }
              void add_out_edge(Edge* e) { m_outEdges.push_back(e); }
              void set_state(EState state) { m_state = state; }
              void remove() {
                     m_is_visible = false;
                     for(Edge* e : m_inEdges) e->remove();
                     for(Edge* e : m_outEdges) e->remove();
              }
              void reborn() {
                     m_is_visible = true;
                     for(Edge* e : m_inEdges) e->reborn();
                     for(Edge* e : m_outEdges) e->reborn();
              }

              int get_id() const { return m_id; }
              bool in_edge_exists(int v) const {
                     for(Edge* e : m_inEdges)
                            if(e->get_source_id() == v && e->visible()) return true;
                     return false;
              }
              bool out_edge_exists(int v) const {
                     for(Edge* e : m_outEdges)
                            if(e->get_dest_id() == v && e->visible()) return true;
                     return false;
              }
              bool visible() const { return m_is_visible; }
       };

public:
       CGraph(bool oriented = false, bool weighted = false) : m_algo_step(0), m_oriented(oriented), m_weighted(weighted) {}
       ~CGraph() {
              for(Vertex* v : m_vertices) if(v) delete v;
       }
       struct StepSlice {
              int m_id;
              EState m_state;
              bool m_is_vertex;
              StepSlice(int id, int st, bool isv) : m_id(id), m_state(st), m_is_vertex(isv) {}
              StepSlice() = delete;
       };
       template<class CompareFunction>
       void apply_algorithm(IAlgorithm<CompareFunction>* algorithm, CompareFunction func, std::istream& stream) {
              algorithm->apply_algorithm(m_edges, m_edges.size(), func, stream);
       }
       std::size_t get_vertices_amount() const { return m_vertices.size(); }
       std::size_t get_edges_amount() const { return m_edges.size(); }
       bool vertice_exists(int v) const { return v < m_vertices.size() && m_vertices[v]->visible(); }
       bool edge_exists(int i, int j) const {
              if(m_oriented)
                     return (m_vertices[i]->out_edge_exists(j) && m_vertices[j]->in_edge_exists(i));
              else
                     return m_vertices[i]->out_edge_exists(j);
       }
       void read(std::istream& stream) {         //TODO: set m_oriented and m_weighted here
              std::size_t numOfVertices, numOfEdges;
              stream >> numOfVertices >> numOfEdges;
              m_edges.resize(numOfEdges);
              m_vertices.resize(numOfVertices);

              for(std::size_t i = 0; i < numOfEdges; ++i) {
                     std::size_t sourceVertIndex, destinationVertIndex, weight = 0;
                     stream >> sourceVertIndex >> destinationVertIndex;
                     if(m_weighted) stream >> weight;
                     if(sourceVertIndex - 1 < numOfVertices || destinationVertIndex - 1 < numOfVertices) throw new ExInvalidInput;

                     if(!m_vertices[sourceVertIndex]) m_vertices[sourceVertIndex] = new Vertex(sourceVertIndex);
                     if(!m_vertices[destinationVertIndex]) m_vertices[destinationVertIndex] = new Vertex(destinationVertIndex);
                     m_edges.push_back(new Edge(m_vertices[sourceVertIndex], m_vertices[destinationVertIndex], weight, i));
                     m_vertices[sourceVertIndex]->add_out_edge(*(m_edges.end() - 1));
                     if(m_oriented) m_vertices[destinationVertIndex]->add_in_edge(*(m_edges.end() - 1));
              }
       }
       void set_vertice_state(int v, EState state) {
              m_vertices[v]->set_state(state);
              m_steps.push_back(StepSlice(v, state, true));
       }
       void set_edge_state(int e, EState state) {
              m_edges[e]->set_state(state);
              m_steps.push_back(StepSlice(v, state, false));
       }
       StepSlice next_step() {
              if(m_algo_step + 1 == m_steps.size()) throw new ExLastStep;
              ++m_algo_step;
              return m_steps[m_algo_step];
       }
       StepSlice previous_step() {
              if(m_algo_step - 1 < 0) throw new ExFirstStep;
              --m_algo_step;
              return m_steps[m_algo_step];
       }
       void add_vertex(int id) {
              if(id < 0) throw new ExInvalidId;
              if(id >= m_vertices.size()) m_vertices.push_back(new Vertex(id));
              else if(!m_vertices[id]->visible()) m_vertices[id]->reborn();
              else throw new ExExistingVertex;
       }
       void add_edge(int id, int source_id, int dest_id, int weight = 0) {
              if(id < 0) throw new ExInvalidId;
              if(source_id >= m_vertices.size() || dest_id >= m_vertices.size()) throw new ExInvalidVertex;
              if(id >= m_edges.size()) {
                     Edge* e = new Edge(m_vertices[source_id], m_vertices[dest_id], weight, id);
                     m_edges.push_back(e);
                     m_vertices[source_id]->add_out_edge(e);
                     if(m_oriented) m_vertices[dest_id]->add_in_edge(e);
              }
              else if(!m_edges[id]->visible()) m_edges[id]->reborn();
              else throw new ExExistingEdge;
       }
};

#endif // CGRAPH_HPP
