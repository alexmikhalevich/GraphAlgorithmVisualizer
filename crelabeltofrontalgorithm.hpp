/*
 * The implementation of the relabel-to-front algorithm
 * Assuming vertices.begin - source, vertices.end - sink
 */

#ifndef CRELABELTOFRONTALGORITHM_H
#define CRELABELTOFRONTALGORITHM_H

#include <iostream>
#include <assert.h>
#include <limits>
#include <istream>
#include "imaxflowalgorithm.hpp"
#include "cbaseedge.h"
#include "cbasevertex.h"

template<class CapacityType, class CompareFunction>
class CRelabelToFrontAlgorithm : public IMaxFlowAlgorithm<CapacityType, CompareFunction> {
private:
       class CVertex : public CBaseVertex {
       private:
              std::size_t v_id;
              std::size_t v_height;
              CapacityType v_excessFlow;
       public:
              CVertex(const std::size_t id) : CBaseVertex () {
                     v_id = id;
                     v_height = 0;
              }

              ~CVertex() {}

              std::size_t getId() const {
                     return v_id;
              }

              void setExcessFlow(const CapacityType& flow) {
                     v_excessFlow = flow;
              }

              CapacityType getExcessFlow() const {
                     return v_excessFlow;
              }

              void setHeight(const std::size_t height) {
                     v_height = height;
              }

              std::size_t getHeight() const {
                     return v_height;
              }
       };

       class CEdge : public CBaseEdge {
       private:
              CapacityType e_flow;
              CapacityType e_capacity;
       public:
              CEdge(const std::size_t source, const std::size_t destination, CapacityType flow, CapacityType capacity)
                     : CBaseEdge(source, destination) {
                     e_flow = flow;
                     e_capacity = capacity;
              }

              void setFlow(const CapacityType& flow) {
                     e_flow = flow;
              }

              CapacityType getFlow() const {
                     return e_flow;
              }

              void setCapacity(const CapacityType& capacity) {
                     e_capacity = capacity;
              }

              CapacityType getCapacity() const {
                     return e_capacity;
              }
       };

       CCompare<CapacityType, CompareFunction>* ppa_comparator;
       std::vector<std::vector<CEdge*> > ppa_residualEdges;
       std::vector<CVertex*> ppa_residualVertices;

       inline void Push(const std::size_t firstVertexId, const std::size_t secondVertexId) {
              CapacityType maxPossibleFlow = ppa_comparator->less(ppa_residualVertices[firstVertexId]->getExcessFlow(),
                                                                  ppa_residualEdges[firstVertexId][secondVertexId]->getCapacity())
                            ? ppa_residualVertices[firstVertexId]->getExcessFlow()
                            : ppa_residualEdges[firstVertexId][secondVertexId]->getCapacity();

              ppa_residualVertices[firstVertexId]->setExcessFlow(ppa_residualVertices[firstVertexId]->getExcessFlow() - maxPossibleFlow);
              ppa_residualVertices[secondVertexId]->setExcessFlow(ppa_residualVertices[secondVertexId]->getExcessFlow()  +  maxPossibleFlow);

              ppa_residualEdges[firstVertexId][secondVertexId] -= maxPossibleFlow;
              ppa_residualEdges[secondVertexId][firstVertexId] += maxPossibleFlow;
       }

       inline void Relabel(const std::size_t vertexId) {
              std::size_t minimumHeight = std::numeric_limits<std::size_t>::max();

              for(std::size_t i = 0; i < ppa_residualVertices.size(); ++i)
                     if(ppa_residualEdges[vertexId][i])
                            minimumHeight = std::min(minimumHeight, ppa_residualVertices[i]->getHeight());
              if(minimumHeight >= ppa_residualVertices[vertexId]->getHeight())
                     ppa_residualVertices[vertexId]->setHeight(minimumHeight + 1);
       }

       void Discharge(const std::size_t vertexId) {
              while(ppa_comparator->less(0, ppa_residualVertices[vertexId]->getExcessFlow())) {
                     Relabel(vertexId);

                     for(size_t i = 0; i < ppa_residualVertices.size(); ++i) {
                            if(ppa_residualVertices[vertexId]->getHeight() == ppa_residualVertices[i]->getHeight() + 1)
                                   Push(vertexId, i);
                     }
              }
       }

       //FIXME: optimization for this method is necessary
       void Init(const std::vector<std::vector<int> >& edges, const std::size_t edgesAmount, std::istream& stream) {
              ppa_residualEdges.resize(edges.size());
              ppa_residualVertices.resize(edges.size());

              for(std::size_t i = 0; i < edges.size(); ++i) {
                     ppa_residualEdges[i].resize(edges.size());
                     ppa_residualVertices[i] = NULL;
                     for(std::size_t j = 0; j < edges.size(); ++j)
                            ppa_residualEdges[i][j] = NULL;
              }

              for(std::size_t i = 0; i < edgesAmount; ++i) {
                     std::size_t sourceVertex, destinationVertex;
                     CapacityType capacity;
                     stream >> sourceVertex >> destinationVertex >> capacity;
                     --sourceVertex;
                     --destinationVertex;

                     assert(sourceVertex != destinationVertex);                            //TODO: replace with exceptions
                     assert(sourceVertex >= 0);
                     assert(destinationVertex >= 0);
                     assert(sourceVertex < ppa_residualVertices.size());
                     assert(destinationVertex < ppa_residualVertices.size());

                     ppa_residualEdges[sourceVertex][destinationVertex] = new CEdge(sourceVertex, destinationVertex, *(new CapacityType), capacity);
                     ppa_residualEdges[destinationVertex][sourceVertex] = new CEdge(destinationVertex, sourceVertex, *(new CapacityType), capacity);
                     if(ppa_residualVertices[sourceVertex] == NULL)
                            ppa_residualVertices[sourceVertex] = new CVertex(sourceVertex);
                     if(ppa_residualVertices[destinationVertex] == NULL)
                            ppa_residualVertices[destinationVertex] = new CVertex(destinationVertex);
              }

              for(std::size_t i = 0; i < ppa_residualVertices.size(); ++i) {
                     ppa_residualVertices[i]->setHeight(0);
                     ppa_residualVertices[i]->setExcessFlow(ppa_residualEdges[0][i]->getCapacity());
                     ppa_residualEdges[i][0]->setFlow(ppa_residualEdges[0][i]->getFlow() + ppa_residualEdges[i][0]->getFlow());
                     ppa_residualEdges[0][i]->setFlow(0);       //FIXME: the implicit conversion from int to CapacityType
                     }

              ppa_residualVertices[0]->setHeight(ppa_residualVertices.size());
       }
public:
       CRelabelToFrontAlgorithm() {
              ppa_comparator = NULL;
       }

       virtual ~CRelabelToFrontAlgorithm() {
              if(ppa_comparator)
                     delete ppa_comparator;
       }

       void applyAlgorithm(const std::vector<std::vector<int> >& edges, const std::size_t edgesAmount, CompareFunction func, std::istream& stream) {
              ppa_comparator = new CCompare<CapacityType, CompareFunction>(func);
              Init(edges, edgesAmount, stream);

              bool discharged = true;
              while(discharged) {
                     discharged = false;
                     for(size_t i = 1; i < edges.size() - 1; ++i) {          //ignoring source and sink
                            if(ppa_comparator->less(0, ppa_residualVertices[i]->getExcessFlow())) {
                                   Discharge(i);
                                   discharged = true;
                            }
                     }
              }
       }

       CapacityType getMaxFlowCapacity() {
              return ppa_comparator->less(0, (*(ppa_residualVertices.end() - 1))->getExcessFlow()) ? (*(ppa_residualVertices.end() - 1))->getExcessFlow() : 0;
       }

       std::vector<std::vector<CapacityType> > getMaxFlow() {
              std::vector<std::vector<CapacityType> > maxFlow(ppa_residualEdges.size(), std::vector<CapacityType>(ppa_residualEdges.size()));
              for(std::size_t i = 0; i < ppa_residualEdges.size(); ++i)
                     for(std::size_t j = 0; j < ppa_residualEdges.size(); ++j)
                            maxFlow[i][j] = ppa_residualEdges[i][j]->getFlow();
              return maxFlow;
       }
};

#endif // CRELABELTOFRONTALGORITHM_H
