#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <fstream>
#include "cgraph.hpp"
#include "cfruchtermanreingold.h"

#define NODE_RADIUS 15.0

namespace Ui {
class CMainWindow;
}

class CMainWindow : public QMainWindow
{
       Q_OBJECT

public:
       explicit CMainWindow(QWidget *parent = 0);
       int getAreaWidth() const;
       int getAreaHeight() const;
       ~CMainWindow();

private:
       Ui::CMainWindow *ui;
       QGraphicsScene* mw_scene;
       QTimer mw_timer;
       CFruchtermanReingold* mw_fruchtermanReingold;
       QVector<QGraphicsEllipseItem*> mw_verticesItems;
       QVector<QVector<QGraphicsLineItem*> > mw_edgesItems;
       QVector<QPoint> mw_coordinates;
       CGraph mw_graph;
       bool mw_isPaused;
       bool mw_positioned;
       void Draw(const int step);

public slots:
       void pause();
       void start();
       void next_step();
       void previous_step();
       void open_graph();
       void exit_program();
       void about_program();
       void init();
       void redraw();
       void load_algorithm();
};

#endif // CMAINWINDOW_H
