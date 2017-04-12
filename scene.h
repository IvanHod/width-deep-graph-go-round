#ifndef SCENE_H
#define SCENE_H

#include <qdebug.h>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <qpoint.h>
#include "graph.h"

#define DEFAULT_RADIUS 15;
#define DEFAULT_FONT_SIZE 16;

class Scene : public QGraphicsScene
{
public:
    Scene();

    QGraphicsEllipseItem* addCicle(QPoint center, QString value);
    void addCicles(QMap<QString, graph*> tops);

    void addLink(graph*, graph*);
    void changeColor(QString topname, QString color);

    QSize viewSize;

    void setSize(QSize size);
    void clearScene();

    QMap<QString, QPoint> points;
    QMap<QString, QGraphicsEllipseItem*> cicles;
    QMap<QString, QGraphicsPathItem*> labels;
    QList<QGraphicsLineItem*> lines;
};

#endif // SCENE_H
