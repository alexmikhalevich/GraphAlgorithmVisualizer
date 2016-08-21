#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QFileDialog>
#include <QStringList>
#include <fstream>
#include "cgraph.hpp"
#include "cfruchtermanreingold.h"
#include "cprocessor.h"

#define NODE_RADIUS 15.0

namespace Ui {
class CMainWindow;
}

class CMainWindow : public QMainWindow
{
       Q_OBJECT

public:
       explicit CMainWindow(QWidget *parent = 0);
       int get_area_width() const;
       int get_area_height() const;
       ~CMainWindow();

private:
       Ui::CMainWindow* ui;
       QGraphicsScene* m_scene;
       QTimer m_timer;
       CFruchtermanReingold* m_fruchtermanReingold;
       QVector<QGraphicsEllipseItem*> m_verticesItems;
       QVector<QGraphicsLineItem*> m_edgesItems;
       QVector<QPoint> m_coordinates;
       CProcessor m_algorithm_processor;
       CGraph* m_graph;
       IAlgorithm* m_algorithm;
       bool m_isPaused;
       bool m_positioned;
       void ChangeState(EState st, int id, bool vertex);
       void AddVertex(const QPoint& coord);
       void AddEdge(const QPoint& source, const QPoint& dest);
       void Init(const QString& filename);

public slots:
       void pause();
       void start();
       void next_step();
       void previous_step();
       void open_graph();
       void exit_program();
       void about_program();
       void redraw();
       void load_algorithm();
};

#endif // CMAINWINDOW_H
