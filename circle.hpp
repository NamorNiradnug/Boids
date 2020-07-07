#pragma once

#include <QPointF>


class Circle
{
public:
    Circle(QPointF center, double radius);
    void setRadius(double radius);
    void setCenter(QPointF center);
    QPointF center();
    double radius();

private:
    QPointF center_;
    double radius_;
};
