#include "graph.h"

graph::graph()
{
    status = BEFORE;
}

graph::graph(QString _name)
{
    status = BEFORE;
    name = _name;
}

void graph::setName(QString _name)
{
    name = _name;
}

QString graph::getName()
{
    return name;
}

QVector<graph*> graph::getBrothers()
{
    return brothers;
}

bool graph::isContain(graph* top)
{
    bool isContain = false;
    foreach (graph* brother, brothers) {
        if(top->getName() == brother->getName()) {
            isContain = true;
        }
    }
    return isContain;
}

void graph::add(graph* top)
{
    if(!isContain(top))
    {
        brothers.append(top);
    }
}

void graph::clear()
{

}

graph* graph::nextStatus()
{
    switch(status) {
        case BEFORE: status = IN_PROCESS; break;
        case IN_PROCESS: status = FINISH; break;
        default: break;
    }
    return this;
}

graph* graph::prevStatus()
{
    switch(status) {
        case FINISH: status = IN_PROCESS; break;
        case IN_PROCESS: status = BEFORE; break;
        default: break;
    }
    return this;
}
