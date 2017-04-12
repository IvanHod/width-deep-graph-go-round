#ifndef GRAPH_H
#define GRAPH_H

#include <qstring.h>
#include <qvector.h>
#include <qfile.h>
#include <qdebug.h>

enum Status {
    BEFORE,
    IN_PROCESS,
    FINISH
};

class graph
{
public:
    graph();
    graph(QString name);

    void setName(QString name);
    QString getName();
    QVector<graph*> getBrothers();
    bool isContain(graph* top);
    void add(graph*);
    graph* nextStatus();
    graph* prevStatus();

    void clear();

    Status status;

private:

    QString name;

    QVector<graph*> brothers;

};

#endif // GRAPH_H
