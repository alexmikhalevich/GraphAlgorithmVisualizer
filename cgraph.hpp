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
private:
       class Vertex;
       class Edge;
       std::vector<Vertex*> m_vertices;
       std::vector<Edge*> m_edges;
       bool m_oriented;
       bool m_weighted;

       class Edge {
       private:
              Vertex* m_source;
              Vertex* m_destination;
              int  m_weight;
              EState m_state;
       public:
              Edge(Vertex* s, Vertex* d) : m_source(s), m_destination(d), m_weight(0), m_state(EState::NEW) {}
              Edge(Vertex* s, Vertex* d, int w) : m_source(s), m_destination(d), m_weight(w), m_state(EState::NEW) {}
              Edge() = delete;
              ~Edge() {
                     delete m_source;
                     delete m_destination;
              }
              void set_state(EState state) { m_state = state; }
              void set_weight(int w) { m_weight = w; }
              int get_dest_id() const { return m_destination->get_id(); }
              int get_source_id() const { return m_source->get_id(); }
              int get_weight() const { return m_weight; }
       };

       class Vertex {
       private:
              std::vector<Edge*> m_inEdges;
              std::vector<Edge*> m_outEdges;
              int m_id;
              EState m_state;
       public:
              Vertex(int id) : m_id(id), m_state(EState::NEW) {}
              Vertex(const std::vector<Edge*>& in, const std::vector<Edge*>& out, int id) : m_inEdges(in),
                     m_outEdges(out), m_id(id), m_state(EState::NEW) {}
              Vertex() = delete;
              void add_in_edge(Edge* e) { m_inEdges.push_back(e); }
              void add_out_edge(Edge* e) { m_outEdges.push_back(e); }
              void set_state(EState state) { m_state = state; }
              int get_id() const { return m_id; }
              bool in_edge_exists(int v) const {
                     for(Edge* e : m_inEdges)
                            if(e->get_source_id() == v) return true;
                     return false;
              }
              bool out_edge_exists(int v) const {
                     for(Edge* e : m_outEdges)
                            if(e->get_dest_id() == v) return true;
                     return false;
              }
       };

public:
       CGraph(bool oriented = false, bool weighted = false) : m_oriented(oriented), m_weighted(weighted) {}

       template<class CompareFunction>
       void apply_algorithm(IAlgorithm<CompareFunction>* algorithm, CompareFunction func, std::istream& stream) {
              algorithm->apply_algorithm(m_edges, m_edges.size(), func, stream);
       }
       std::size_t get_vertices_amount() const { return m_vertices.size(); }
       std::size_t get_edges_amount() const { return m_edges.size(); }
       bool edge_exists(const int i, const int j) const {
              if(m_oriented)
                     return (m_vertices[i]->out_edge_exists(j) && m_vertices[j]->in_edge_exists(i));
              else
                     return m_vertices[i]->out_edge_exists(j);
       }
       void read(std::istream& stream) {
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
                     m_edges.push_back(new Edge(m_vertices[sourceVertIndex], m_vertices[destinationVertIndex], weight));
                     m_vertices[sourceVertIndex]->add_out_edge(*(m_edges.end() - 1));
                     if(m_oriented) m_vertices[destinationVertIndex]->add_in_edge(*(m_edges.end() - 1));
              }
       }
       void next_step() {

       }
       void previous_step() {

       }
};
#endif // CGRAPH_HPP
