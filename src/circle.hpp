#pragma once

#include <QPointF>

double squeresSum(QPointF point);

class Circle
{
public:
    Circle(QPointF center, double radius);
    QPointF center();
    double radius();
    bool contains(QPointF point);

private:
    QPointF center_;
    double radius_;
};
