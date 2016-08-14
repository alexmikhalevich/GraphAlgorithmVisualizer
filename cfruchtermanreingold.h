#ifndef CFRUCHTERMANREINGOLD_H
#define CFRUCHTERMANREINGOLD_H
#include <QVector>
#include <QVector2D>
#include <QPoint>
#include <fstream>
#include <cmath>
#include <random>
#include <fstream>
#include "flownetwork.hpp"

#define TIME_DELTA 0.02
#define SPRING_CONSTANT 10.0
#define SPRING_LENGTH 0.2
#define NODE_MASS 100.0

class CFruchtermanReingold
{
private:
       struct Vertex {
              QPoint v_coordinates;       //coordinates of the top-left corner of bounding box
              QVector2D v_force;
       };

       QVector<Vertex> vgc_vertices;
       CFlowNetwork* vgc_graph;
       double vgc_area;
       double vgc_areaWidth;
       double vgc_areaHeight;
       double vgc_nodeRadius;
       double vgc_coeff;
       int vgc_temperature;
       int vgc_numOfNodes;
       int vgc_numOfEdges;
       void InitGraph(std::ifstream& in);
       void GenerateRandomCoordinates();
       inline double Distance(const QPoint& first, const QPoint& second) const;
       inline double DistanceX(const QPoint& first, const QPoint& second) const;
       inline double DistanceY(const QPoint& first, const QPoint& second) const;
       inline double Abs(const QPoint& p) const;
       inline QVector2D CoulombForce(const int firstVertexIndex, const int secondVertexIndex) const;
       inline QVector2D HookeForce(const int firstVertexIndex, const int secondVertexIndex) const;
       void Reflect(int v);
       void CalculateForces();
       inline void Cool();
public:
       CFruchtermanReingold(const double width, const double height, const double nodeRadius, std::ifstream& in);
       ~CFruchtermanReingold();
       void getCoordinates(QVector<QPoint>& coordinates);
       void calculateCoordinates();
       bool isStable() const;
       void setSceneWidth(int w);
       void setSceneHeight(int h);
       CFlowNetwork* getGraph();
};

#endif // CFRUCHTERMANREINGOLD_H
