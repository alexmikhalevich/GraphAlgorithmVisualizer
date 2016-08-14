#ifndef CFLOWNETWORK_HPP
#define CFLOWNETWORK_HPP

#include <iostream>
#include <assert.h>
#include <vector>
#include "ialgorithm.h"

class CFlowNetwork {
private:
       std::vector<std::vector<int> > fn_edges;
       std::size_t fn_numOfEdges;

public:
       CFlowNetwork() {}

       template<class CompareFunction>
       void applyAlgorithm(IAlgorithm<CompareFunction>* algorithm, CompareFunction func, std::istream& stream) {
              algorithm->applyAlgorithm(fn_edges, fn_numOfEdges, func, stream);
       }

       std::size_t getEdgesArraySize() const {
              return fn_edges.size();
       }

       bool edgeExists(const int i, const int j) const {
              return (fn_edges[i][j] == -1) ? false : true;
       }

       void read(std::istream& stream) {
              std::size_t numOfVertices, numOfEdges;
              stream >> numOfVertices >> numOfEdges;
              fn_numOfEdges = numOfEdges;

              fn_edges.resize(numOfVertices);
              for(std::size_t i = 0; i < numOfVertices; ++i) {
                     fn_edges[i].resize(numOfVertices);
                     for(std::size_t j = 0; j < numOfVertices; ++j) {
                            fn_edges[i][j] = -1;
                     }
              }

              for(std::size_t i = 0; i < numOfEdges; ++i) {
                     std::size_t sourceVertIndex, destinationVertIndex;
                     stream >> sourceVertIndex >> destinationVertIndex;

                     assert(sourceVertIndex - 1 < numOfVertices);
                     assert(destinationVertIndex - 1 < numOfVertices);                        //TODO: replace asserts with exceptions

                     fn_edges[sourceVertIndex - 1][destinationVertIndex - 1] = 1;
              }
       }
};
#endif // CFLOWNETWORK_HPP
