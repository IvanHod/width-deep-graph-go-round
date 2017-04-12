#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QTimer>
#include "graph.h"
#include "scene.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum State {
        TOP,
        BROTHER
    } nextState;

    enum Method {
        DEEP,
        WIDTH
    } currentMethod;

    graph* load(QString filename);
    void stepWidth();
    void stepDeep();

    graph* _graph;
    QMap<QString, graph*> tops;
    QMap<QString, QStringList> graphStruct;

    QList<graph*> stack;
    QList<graph*> pastTops;

    Scene *scene;

    bool inList(graph* gr);
    bool stackContaionStatus(QList<graph*> list, Status status);

    void drawGraph();
    void unstep();

    bool autoProcces;
    QTimer *timer;

private slots:
    void on_loadBtn_clicked();

    void on_clearBtn_clicked();

    void on_stepBtn_clicked();

    void on_beginBtn_clicked();

    void on_stopBtn_clicked();

    void on_deleteBtn_clicked();

public slots:
    void step();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
