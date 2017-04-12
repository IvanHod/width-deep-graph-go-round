#include "scene.h"

Scene::Scene()
{

}

void Scene::setSize(QSize _size)
{
    viewSize = _size;
}

QGraphicsEllipseItem* Scene::addCicle(QPoint center, QString value)
{
    int radius = DEFAULT_RADIUS;

    QPen pen = QPen(QColor("green"), 3);
    QBrush brush = QBrush("white");

    QGraphicsEllipseItem* item
            = addEllipse(center.x() - radius, center.y() - radius, radius*2, radius*2, pen, brush);


    QPainterPath path;

    QFont font;
    int fontSize = DEFAULT_FONT_SIZE;
    font.setPixelSize(fontSize);

    path.addText(center.x() - fontSize/3, center.y() + fontSize/3, font,  value);

    labels.insert(value, addPath(path));

    return item;
}

void Scene::addCicles(QMap<QString, graph*> tops)
{
    QList<QString> values = tops.keys();
    int count = values.size();
    int radius = DEFAULT_RADIUS;
    int lenght = (viewSize.width() < viewSize.height() ? viewSize.width() : viewSize.height()) - radius * 2 - 10;
    int x0 = viewSize.width()/2, y0 = viewSize.height()/2;
    float dAngle = 360 / count;

    for(int i = 0; i < count; i++) {
        float angle = i * dAngle * 3.14 / 180;
        QPoint point(x0 + sin(angle) * lenght/2, y0 - cos(angle) * lenght/2);

        points.insert(values[i], point);
    }

    for(int i = 0; i < count; i++) {
        graph* _graph = tops[values[i]];
        foreach (graph* child, _graph->getBrothers()) {
            addLink(_graph, child);
        }
    }

    for(int i = 0; i < count; i++) {
        cicles.insert(values[i], addCicle(points[values[i]], values[i]));
    }
}

void Scene::addLink(graph* _graph, graph* brother)
{
    int x1 = points[_graph->getName()].x();
    int y1 = points[_graph->getName()].y();

    int x2 = points[brother->getName()].x();
    int y2 = points[brother->getName()].y();

    // найдем вектор вертикали
    QPair<float, float> ver(0, -10);
    QPair<float, float> line(x2 - x1, y2 - y1);

    float cosEl = ver.first * line.first + ver.second * line.second;
    float cosDel = sqrt(ver.first * ver.first + ver.second * ver.second) *
            sqrt(line.first * line.first + line.second * line.second);

    float cos = cosEl / cosDel;

    //qDebug() << _graph->getName() << QString::number(cos) << brother->getName();

    QPen pen(QColor("red"));
    pen.setWidth(3);

    lines.append(addLine(QLineF(x1, y1, x2, y2), pen));
}

void Scene::changeColor(QString topname, QString color)
{
    QGraphicsEllipseItem* cicle = cicles[topname];
    cicle->setBrush(QBrush(QColor(color)));
}

void Scene::clearScene()
{
    if(!cicles.isEmpty()) {
        foreach (QString key, cicles.keys()) {
            removeItem(cicles[key]);
            removeItem(labels[key]);
        }
        cicles.clear();
        labels.clear();
        points.clear();
    }
    if(!lines.isEmpty()) {
        foreach (QGraphicsLineItem* line, lines) {
            removeItem(line);
        }
    }
}
