#include "cmainwindow.h"
#include "ui_cmainwindow.h"

CMainWindow::CMainWindow(QWidget* parent) :
       QMainWindow(parent),
       ui(new Ui::CMainWindow) {
       ui->setupUi(this);
       m_isPaused = true;
       m_positioned = false;

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
       connect(&m_timer, SIGNAL(timeout()), this, SLOT(redraw()));

       m_scene = new QGraphicsScene(this);
       ui->graphicsView->setRenderHint(QPainter::Antialiasing);
       ui->graphicsView->setScene(m_scene);
}

CMainWindow::~CMainWindow() {
       for(QGraphicsEllipseItem* item : m_verticesItems) if(item) delete item;
       for(QGraphicsLineItem* item : m_edgesItems) if(item) delete item;
       delete m_fruchtermanReingold;
       delete m_scene;
       delete ui;
}

int CMainWindow::get_area_height() const {
       return ui->graphicsView->height();
}

int CMainWindow::get_area_width() const {
       return ui->graphicsView->width();
}

void CMainWindow::redraw() {
       if(!m_positioned) {
              m_fruchtermanReingold->setSceneHeight((int)(0.9 * ui->graphicsView->height()));
              m_fruchtermanReingold->setSceneWidth((int)(0.9 * ui->graphicsView->width()));
              if(m_fruchtermanReingold->isStable()) {
                     m_positioned = true;
                     m_timer.stop();
              }
              m_fruchtermanReingold->calculateCoordinates();
              m_fruchtermanReingold->getCoordinates(m_coordinates);
              for(int i = 0; i < m_verticesItems.size(); ++i) {
                     if(!m_verticesItems[i]->isVisible()) continue;
                     m_verticesItems[i]->setRect(m_coordinates[i].x() - NODE_RADIUS, m_coordinates[i].y() - NODE_RADIUS,
                                                     NODE_RADIUS * 2, NODE_RADIUS * 2);
              }
              for(int i = 0; i < m_edgesItems.size(); ++i) {
                     if(!m_edgesItems[i]->isVisible()) continue;
                      m_edgesItems[i]->setLine(m_coordinates[m_graph->get_edge_source_id(i)].x(),
                                    m_coordinates[m_graph->get_edge_source_id(i)].y(),
                                    m_coordinates[m_graph->get_edge_dest_id(i)].x(),
                                    m_coordinates[m_graph->get_edge_dest_id(i)].y());
              }
       }
}

void CMainWindow::Init(const QString& filename) {
       std::ifstream in(filename);
       m_fruchtermanReingold = new CFruchtermanReingold(ui->graphicsView->width(), ui->graphicsView->height(), NODE_RADIUS, in); //TODO: check re-loading supporting
       m_fruchtermanReingold->getCoordinates(m_coordinates);
       m_graph = m_fruchtermanReingold->getGraph();
       int vert_amount = graph->get_vertices_amount();
       int edge_amount = graph->get_edges_amount();
       for(int i = 0; i < edge_amount; ++i)
              AddEdge(m_coordinates[m_graph->get_edge_source_id(i)], m_coordinates[m_graph->get_edge_dest_id(i)]);
       for(int i = 0; i < vert_amount; ++i) AddVertex(m_coordinates[i]);
}

void CMainWindow::AddVertex(const QPoint& coord) {
       m_verticesItems.push_back(new QGraphicsEllipseItem(coord.x() - NODE_RADIUS, coord.y() - NODE_RADIUS,
                                                           NODE_RADIUS * 2, NODE_RADIUS * 2, 0, m_scene));
       *(m_verticesItems.end() - 1)->setBrush(QBrush(Qt::gray));
}

void CMainWindow::AddEdge(const QPoint& source, const QPoint& dest) {
       m_edgesItems.push_back(new QGraphicsLineItem(source, dest, 0, m_scene));
}

void CMainWindow::ChangeState(EState st, int id, bool vertex) {
       if(vertex) {
              switch(st) {
                     case EState::NEW:
                            m_positioned = false;
                            if(id < m_verticesItems.size()) m_verticesItems[id]->setVisible(true);
                            else AddVertex(QPoint(4 * NODE_RADIUS, 4 * NODE_RADIUS));
                            m_verticesItems[id]->setBrush(QBrush(Qt::gray));
                            m_timer.start(30);
                            break;
                     case EState::VISITED:
                            m_verticesItems[id]->setBrush(QBrush(Qt::black));
                            break;
                     case EState::PROCESSING:
                            m_verticesItems[id]->setBrush(QBrush(Qt::red));
                            break;
                     case EState::REMOVED:
                            m_verticesItems[id]->setVisible(false);
                            m_positioned = false;
                            m_timer.start(30);
                            break;
              }
       }
       else {
              switch(st) {
                     case EState::NEW:
                            m_positioned = false;
                            if(id < m_edgesItems.size()) m_edgesItems[id]->setVisible(true);
                            else AddEdge(m_coordinates[m_graph->get_edge_source_id(id)],
                                          m_coordinates[m_graph->get_edge_dest_id(id)]);
                            m_edgesItems[id]->setVisible(true);
                            m_edgesItems[id]->setBrush(QBrush(Qt::black));
                            m_timer.start(30);
                            break;
                     case EState::VISITED:
                            m_edgesItems[id]->setBrush(QBrush(Qt::gray));
                            break;
                     case EState::PROCESSING:
                            m_edgesItems[id]->setBrush(QBrush(Qt::red));
                            break;
                     case EState::REMOVED:
                            m_edgesItems[id]->setVisible(false);
                            m_positioned = false;
                            m_timer.start(30);
                            break;
              }
       }
}

void CMainWindow::start() {
       m_isPaused = false;
       ui->pushButton_pause->setEnabled(true);
       ui->pushButton_prev->setEnabled(false);
       ui->pushButton_next->setEnabled(false);
       ui->pushButton_start->setEnabled(false);
       m_timer.start(30);
}

void CMainWindow::pause() {
       m_isPaused = true;
       ui->pushButton_pause->setEnabled(false);
       ui->pushButton_prev->setEnabled(true);
       ui->pushButton_next->setEnabled(true);
       ui->pushButton_start->setEnabled(true);
       m_timer.stop();
}

void CMainWindow::next_step() {
       CGraph::StepSlice slice = m_graph.next_step();
       ChangeState(slice.m_state, slice.m_id, slice.m_is_vertex);
}

void CMainWindow::previous_step() {
       CGraph::StepSlice slice = m_graph.previous_step();
       ChangeState(slice.m_state, slice.m_id, slice.m_is_vertex);
}

void CMainWindow::about_program() {

}

void CMainWindow::open_graph() {
       m_scene->clear();
       for(QGraphicsEllipseItem* item : m_verticesItems) if(item) delete item;
       for(QGraphicsLineItem* item : m_edgesItems) if(item) delete item;
       if(m_fruchtermanReingold) delete m_fruchtermanReingold;
       QString filename = QFileDialog::getOpenFileName(this, "Open graph", QString(), tr("Graph data(*.dat, *.txt)"));
       Init(filename);
}

void CMainWindow::exit_program() {
       exit(0);
}
