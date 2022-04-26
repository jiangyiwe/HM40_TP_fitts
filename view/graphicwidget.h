#ifndef GRAPHICWIDGET_H
#define GRAPHICWIDGET_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QWindow>
class GraphicWidget : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicWidget();

signals:
    void mouseClicked(int,int);

protected:
    void mousePressEvent(QMouseEvent *event);
};

#endif // GRAPHICWIDGET_H
