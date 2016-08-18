#ifndef CFRUCHTERMANREINGOLD_H
#define CFRUCHTERMANREINGOLD_H
#include <QVector>
#include <QVector2D>
#include <QPoint>
#include <fstream>
#include <cmath>
#include <random>
#include <fstream>
#include "cgraph.hpp"

#define TIME_DELTA 0.02
#define SPRING_CONSTANT 10.0
#define SPRING_LENGTH 0.2
#define NODE_MASS 100.0

class CFruchtermanReingold
{
private:
       struct Vertex {
              QPoint v_coordinates;       //coordinates of the top-left corner of the bounding box
              QVector2D v_force;
       };

       QVector<Vertex> vgc_vertices;
       CGraph* vgc_graph;
       double vgc_area;
       double vgc_area_width;
       double vgc_area_height;
       double vgc_node_radius;
       double vgc_coeff;
       int vgc_temperature;
       int vgc_nodes_num;
       int vgc_edges_num;
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
       void get_coordinates(QVector<QPoint>& coordinates);
       void calculate_coordinates();
       bool stable() const;
       void set_scene_width(int w);
       void set_scene_height(int h);
       CGraph* getGraph();
};

#endif // CFRUCHTERMANREINGOLD_H
