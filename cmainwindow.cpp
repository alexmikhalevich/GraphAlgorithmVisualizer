#include "cmainwindow.h"
#include "ui_cmainwindow.h"

CMainWindow::CMainWindow(QWidget* parent) :
       QMainWindow(parent),
       ui(new Ui::CMainWindow) {
       ui->setupUi(this);
       mw_isPaused = true;
       mw_positioned = false;

       connect(ui->pushButton_next, SIGNAL(clicked()), this, SLOT(next_step()));
       connect(ui->pushButton_prev, SIGNAL(clicked()), this, SLOT(previous_step()));
       connect(ui->pushButton_pause, SIGNAL(clicked()), this, SLOT(pause()));
       connect(ui->pushButton_start, SIGNAL(clicked()), this, SLOT(start()));
       connect(ui->actionNext_step, SIGNAL(triggered()), this, SLOT(next_step()));
       connect(ui->action_Start, SIGNAL(triggered()), this, SLOT(start()));
       connect(ui->actionPrevious_step, SIGNAL(triggered()), this, SLOT(previous_step()));
       connect(ui->action_Pause, SIGNAL(triggered()), this, SLOT(pause()));
       connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exit_program()));
       connect(ui->actionOpen_graph, SIGNAL(triggered()), this, SLOT(open_graph()));
       connect(ui->actionAbout_program, SIGNAL(triggered()), this, SLOT(about_program()));
       connect(&mw_timer, SIGNAL(timeout()), this, SLOT(redraw()));

       mw_scene = new QGraphicsScene(this);
       ui->graphicsView->setRenderHint(QPainter::Antialiasing);
       ui->graphicsView->setScene(mw_scene);
}

CMainWindow::~CMainWindow() {
       delete mw_fruchtermanReingold;
       delete mw_scene;
       delete ui;
}

int CMainWindow::get_area_height() const {
       return ui->graphicsView->height();
}

int CMainWindow::get_area_width() const {
       return ui->graphicsView->width();
}

void CMainWindow::redraw() {
       if(!mw_positioned) {               //TODO: mw_positioned should be changed when an edge/vertex appears/disappears
              mw_fruchtermanReingold->setSceneHeight((int)(0.9 * ui->graphicsView->height()));
              mw_fruchtermanReingold->setSceneWidth((int)(0.9 * ui->graphicsView->width()));

              if(mw_fruchtermanReingold->isStable()) {
                     mw_positioned = true;
                     mw_timer.stop();
              }

              mw_fruchtermanReingold->calculateCoordinates();
              mw_fruchtermanReingold->getCoordinates(mw_coordinates);
              for(int i = 0; i < mw_verticesItems.size(); ++i) {
                     mw_verticesItems.at(i)->setRect(mw_coordinates[i].x() - NODE_RADIUS, mw_coordinates[i].y() - NODE_RADIUS,
                                                     NODE_RADIUS * 2, NODE_RADIUS * 2);
                     for(int j = 0; j < mw_edgesItems[i].size(); ++j)
                            if(mw_edgesItems[i][j])
                                   mw_edgesItems[i][j]->setLine(mw_coordinates[i].x(), mw_coordinates[i].y(), mw_coordinates[j].x(),
                                                         mw_coordinates[j].y());
              }
       }
       else {
              for(int i = 0; i < mw_verticesItems.size(); ++i) {
              }
       }
}

void CMainWindow::init() {
       std::ifstream in("input.dat");
       mw_fruchtermanReingold = new CFruchtermanReingold(ui->graphicsView->width(), ui->graphicsView->height(), NODE_RADIUS, in);
       mw_fruchtermanReingold->getCoordinates(mw_coordinates);
       mw_verticesItems.resize(mw_fruchtermanReingold->getGraph()->get_vertices_amount());
       mw_edgesItems.resize(mw_fruchtermanReingold->getGraph()->get_vertices_amount());
       for(int i = 0; i < mw_verticesItems.size(); ++i) {
              mw_edgesItems[i].resize(mw_fruchtermanReingold->getGraph()->get_vertices_amount());
              for(int j = 0; j < mw_edgesItems.size(); ++j) {
                     if(mw_fruchtermanReingold->getGraph()->edge_exists(i, j)) {
                            mw_edgesItems[i][j] = new QGraphicsLineItem(mw_coordinates[i].x(), mw_coordinates[i].y(), mw_coordinates[j].x(),
                                                                        mw_coordinates[j].y(), 0, mw_scene);
                     }
                     else mw_edgesItems[i][j] = NULL;
              }
              mw_verticesItems[i] = new QGraphicsEllipseItem(mw_coordinates[i].x() - NODE_RADIUS, mw_coordinates[i].y() - NODE_RADIUS,
                                                             NODE_RADIUS * 2, NODE_RADIUS * 2, 0, mw_scene);
              mw_verticesItems[i]->setBrush(QBrush(Qt::gray));
       }
}

void CMainWindow::Draw(const int step) {

}

void CMainWindow::start() {
       mw_isPaused = false;
       ui->pushButton_pause->setEnabled(true);
       ui->pushButton_prev->setEnabled(false);
       ui->pushButton_next->setEnabled(false);
       ui->pushButton_start->setEnabled(false);
       mw_timer.start(30);
}

void CMainWindow::pause() {
       mw_isPaused = true;
       ui->pushButton_pause->setEnabled(false);
       ui->pushButton_prev->setEnabled(true);
       ui->pushButton_next->setEnabled(true);
       ui->pushButton_start->setEnabled(true);
       mw_timer.stop();
}

void CMainWindow::next_step() {
       CGraph::StepSlice slice = mw_graph.next_step();
}

void CMainWindow::previous_step() {
       CGraph::StepSlice slice = mw_graph.previous_step();
}

void CMainWindow::about_program() {

}

void CMainWindow::open_graph() {

}

void CMainWindow::exit_program() {
       exit(0);
}
