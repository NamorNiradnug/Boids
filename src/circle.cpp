#include "circle.hpp"

Circle::Circle(QPointF center, double radius)
{
    setRadius(radius);
    setCenter(center);
}

void Circle::setRadius(double radius)
{
    if (radius >= 0)
    {
        radius_ = radius;
    }
}

void Circle::setCenter(QPointF center)
{
    center_ = center;
}

QPointF Circle::center()
{
    return center_;
}

double Circle::radius()
{
    return radius_;
}
