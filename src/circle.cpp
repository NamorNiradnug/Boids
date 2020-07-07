#include "circle.hpp"

double squeresSum(QPointF point)
{
    return point.x() * point.x() + point.y() * point.y();
}

Circle::Circle(QPointF center, double radius)
{
    center_ = center;
    radius_ = radius;
}

QPointF Circle::center()
{
    return center_;
}

double Circle::radius()
{
    return radius_;
}

bool Circle::contains(QPointF point)
{
    return squeresSum(point - center_) <= radius_ * radius_;
}
