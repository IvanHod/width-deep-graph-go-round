#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qdebug.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new Scene();
    nextState = TOP;
    currentMethod = WIDTH;
    ui->isWidth->setChecked(true);
    ui->view->setScene(scene);
    autoProcces = false;
    timer = new QTimer();
    bool isConnect = this->connect(timer, SIGNAL(timeout()), this, SLOT(step()));
    qDebug() << "isConnect: " << isConnect;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadBtn_clicked()
{
    on_deleteBtn_clicked();

    QString filename = QFileDialog::getOpenFileName(0, "Open Dialog", "", "*.txt");
    _graph = load(filename);
    //_graph = load("/home/homepc/Документы/QtCreator/graphs/files/test.txt");
    drawGraph();
}

void MainWindow::on_clearBtn_clicked()
{
    if(!stack.isEmpty())
        stack.clear();
    if(!pastTops.isEmpty())
        pastTops.clear();
    if(!graphStruct.isEmpty()) {
        foreach (QString key, graphStruct.keys()) {
            tops[key]->status = Status::BEFORE;
            scene->changeColor(key, "white");
        }
    }
}

graph* MainWindow::load(QString filename)
{
    scene->clearScene();
    graph* _graph = NULL;
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly)) {
        QStringList matrix = QString::fromUtf8(file.readAll()).replace("\t", "").replace(" ", "").split("\n");
        if(matrix.size() == 0)
        {
            qDebug() << "Read Error: Count line = 0 in the file \"" + filename + "\"";
            return _graph;
        }

        // создать вершины
        foreach (QString line, matrix) {
            QString name = line.split("|")[0];
            tops.insert(name, new graph(name));
            QStringList list;
            graphStruct.insert(name, list);
        }
        // Создать связи
        foreach (QString line, matrix) {
            QString name = line.split("|")[0];
            QString brothersLine = line.split("|")[1];
            QStringList brothers = brothersLine.split(",");
            foreach (QString top, brothers) {
                tops[name]->add(tops[top]);
                graphStruct[name].append(top);
            }
        }
        _graph = tops.first();
    } else {
        qDebug() << "Read Error: Could not open file \"" + filename + "\"";
    }
    file.close();
    return _graph;
}

void MainWindow::drawGraph()
{
    scene->setSize(ui->view->size());
    scene->addCicles(tops);
}

void MainWindow::step()
{
    if(stack.isEmpty() && pastTops.isEmpty()) {
        stack.append(_graph);
    }
    if(ui->isWidth->isChecked())
        stepWidth();
    else
        stepDeep();

    qDebug() << "step auto = " << autoProcces;
    if(autoProcces) {
        timer->start(1000);
    } else {
        timer->stop();
    }
}

void MainWindow::unstep()
{

}

bool MainWindow::inList(graph* gr)
{
    bool inList = false;
    foreach (graph* elem, stack) {
        if(elem->getName() == gr->getName())
            inList = true;
    }
    return inList;
}

void MainWindow::stepWidth()
{
    foreach (graph* elem, stack) {
        if(elem->status == Status::BEFORE || elem->status == Status::IN_PROCESS) {
            scene->changeColor(elem->getName(), tr(elem->status == Status::BEFORE ? "gray" : "black"));
            elem->nextStatus();

            foreach (graph* other, elem->getBrothers()) {
                if(other->status == Status::BEFORE)
                {
                    if(!inList(other))
                        stack.append(other);

                    if(elem->status == Status::FINISH) {
                        other->nextStatus();
                        scene->changeColor(other->getName(), "gray");
                    }
                }
            }
            break;
        }
    }
    if(!stack.isEmpty() && stack[0]->status == Status::FINISH)
    {
        pastTops.append(stack.takeFirst());
    }
}

void MainWindow::stepDeep()
{
    if(!stack.isEmpty()) {
        graph* last = stack.last();
        if(last->status == Status::BEFORE) {
            scene->changeColor(last->getName(), "gray");
            last->nextStatus();
            foreach (graph* other, last->getBrothers()) {
                if(other->status == Status::BEFORE) {
                    stack.append(other);
                    break;
                }
            }
        }
        else if(last->status == Status::IN_PROCESS) {
            bool containInProcces = false;
            foreach (graph* other, last->getBrothers()) {
                if(other->status == Status::BEFORE) {
                    stack.append(other);
                    containInProcces = true;
                    break;
                }
            }
            if(!containInProcces)
            {
                scene->changeColor(last->getName(), "black");
                last->nextStatus();
                pastTops.append(stack.takeLast());
                if(!stack.isEmpty()) {
                    foreach (graph* other, stack.last()->getBrothers()) {
                        if(other->status == Status::BEFORE) {
                            stack.append(other);
                            break;
                        }
                    }
                }
            }
        }
    }
}

bool MainWindow::stackContaionStatus(QList<graph*> list, Status status)
{
    bool contain = false;
    foreach (graph* _graph, list) {
        if(_graph->status == status) {
            contain = true;
            break;
        }
    }
    return contain;
}

void MainWindow::on_stepBtn_clicked()
{
    step();
}

void MainWindow::on_beginBtn_clicked()
{
    autoProcces = true;
    step();
}

void MainWindow::on_stopBtn_clicked()
{
    autoProcces = false;
}

void MainWindow::on_deleteBtn_clicked()
{
    tops.clear();
    graphStruct.clear();
    on_clearBtn_clicked();
    scene->clearScene();
}
