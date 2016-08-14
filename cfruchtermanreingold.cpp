#include "cfruchtermanreingold.h"

CFruchtermanReingold::CFruchtermanReingold(const double width, const double height, const double nodeRadius, std::ifstream& in)
{
       vgc_graph = new CFlowNetwork();
       vgc_areaHeight = height;
       vgc_areaWidth = width;
       vgc_area = width * height;
       vgc_nodeRadius = nodeRadius;
       vgc_temperature = vgc_areaWidth / 10;
       InitGraph(in);
       vgc_coeff = std::sqrt(vgc_area / vgc_numOfNodes);
}

CFruchtermanReingold::~CFruchtermanReingold() {
       delete vgc_graph;
}

inline double CFruchtermanReingold::Distance(const QPoint &first, const QPoint &second) const {
       return std::sqrt((first.x() - second.x()) * (first.x() - second.x()) + (first.y() - second.y()) * (first.y() - second.y()));
}

inline double CFruchtermanReingold::DistanceX(const QPoint &first, const QPoint &second) const {
       return std::abs(first.x() - second.x());
}

inline double CFruchtermanReingold::DistanceY(const QPoint &first, const QPoint &second) const {
       return std::abs(first.y() - second.y());
}

inline double CFruchtermanReingold::Abs(const QPoint &p) const {
       return std::sqrt(p.x() * p.x() + p.y() * p.y());
}

inline QVector2D CFruchtermanReingold::CoulombForce(const int firstVertexIndex, const int secondVertexIndex) const {
       QVector2D direction = QVector2D(vgc_vertices[firstVertexIndex].v_coordinates - vgc_vertices[secondVertexIndex].v_coordinates);
       return vgc_coeff * vgc_coeff  / Distance(vgc_vertices[firstVertexIndex].v_coordinates,
                                                vgc_vertices[secondVertexIndex].v_coordinates) * direction.normalized();
}

inline QVector2D CFruchtermanReingold::HookeForce(const int firstVertexIndex, const int secondVertexIndex) const {
       QVector2D direction = QVector2D(vgc_vertices[secondVertexIndex].v_coordinates - vgc_vertices[firstVertexIndex].v_coordinates);
       return  (SPRING_CONSTANT / vgc_coeff) * (Distance(vgc_vertices[firstVertexIndex].v_coordinates,
                                 vgc_vertices[secondVertexIndex].v_coordinates) / SPRING_LENGTH) * direction.normalized();
}

inline void CFruchtermanReingold::Cool() {
       vgc_temperature = std::max(vgc_temperature - 1, 0);
}

void CFruchtermanReingold::setSceneHeight(int h) {
       vgc_areaHeight = h;
       vgc_area = vgc_areaWidth * h;
}

void CFruchtermanReingold::setSceneWidth(int w) {
       vgc_areaWidth = w;
       vgc_area = vgc_areaHeight * w;
}

void CFruchtermanReingold::Reflect(int v) {
       const QPoint& currentPoint = vgc_vertices[v].v_coordinates;
       if(currentPoint.y() - 2 * vgc_nodeRadius <= 0 || currentPoint.y() + 2 * vgc_nodeRadius >= vgc_areaHeight ||
                     currentPoint.x() - 2 * vgc_nodeRadius <= 0 || currentPoint.x() + 2 * vgc_nodeRadius >= vgc_areaWidth) {
              vgc_vertices[v].v_force = -vgc_vertices[v].v_force;
       }
}

void CFruchtermanReingold::CalculateForces() {
       //Coulomb force
       for(int i = 0; i < vgc_numOfNodes; ++i) {
              QVector2D force;
              for(int j = 0; j < vgc_numOfNodes; ++j) {
                     if(i == j) continue;
                     force += CoulombForce(i, j);
              }
              vgc_vertices[i].v_force = force;
       }

       //Hooke force
       for(int i = 0; i < vgc_numOfNodes; ++i) {
              QVector2D force;
              for(int j = 0; j < vgc_numOfNodes; ++j) {
                     if(i == j) continue;
                     else if(vgc_graph->edgeExists(i, j)) force += HookeForce(i, j);
              }
              vgc_vertices[i].v_force += force;
       }

       //Barrier force
       for(int i = 0; i < vgc_numOfNodes; ++i) Reflect(i);
}

void CFruchtermanReingold::calculateCoordinates() {
       CalculateForces();
       for(int i = 0; i < vgc_numOfNodes; ++i) {
              QVector2D delta = vgc_vertices[i].v_force * NODE_MASS * TIME_DELTA * TIME_DELTA;
              vgc_vertices[i].v_coordinates += delta.toPoint() * std::min(delta.length(), (double)vgc_temperature) / delta.length();
       }
       Cool();
}

bool CFruchtermanReingold::isStable() const {
       return (vgc_temperature == 0 ? true : false);
}

void CFruchtermanReingold::GenerateRandomCoordinates() {
       std::default_random_engine generator;
       std::uniform_int_distribution<int> height_distribution(vgc_nodeRadius, vgc_areaHeight - vgc_nodeRadius);
       std::uniform_int_distribution<int> width_distribution(vgc_nodeRadius, vgc_areaWidth - vgc_nodeRadius);

       for(int i =0; i < vgc_numOfNodes; ++i) {
              vgc_vertices[i].v_coordinates.setX(width_distribution(generator));
              vgc_vertices[i].v_coordinates.setY(height_distribution(generator));
       }
}

void CFruchtermanReingold::InitGraph(std::ifstream& in) {
       vgc_graph->read(in);
       vgc_numOfNodes = vgc_graph->getVerticesAmount();
       vgc_numOfEdges = vgc_graph->getEdgesAmount();
       vgc_vertices.resize(vgc_numOfNodes);
       GenerateRandomCoordinates();
}

void CFruchtermanReingold::getCoordinates(QVector<QPoint>& coordinates)  {
       coordinates.clear();
       for(int i= 0; i < vgc_vertices.size(); ++i)
              coordinates.push_back(vgc_vertices[i].v_coordinates);
}

CFlowNetwork* CFruchtermanReingold::getGraph() {
       return vgc_graph;
}
