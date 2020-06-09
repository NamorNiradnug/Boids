#pragma once
#include <QtCore>
#include <QPainter>
#include <vector>


double frandom();
double squeresSum(QPointF point);


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


class Boid
{
public:
    Boid(QRect rect);
    QPointF getPos();
    QPointF getSpeed();
    void tick(std::vector<Boid *> boids, std::vector<Circle> circles);
    void draw(QPainter *painter);
    static double view_radius;
private:
    QPointF speed, pos;
};
