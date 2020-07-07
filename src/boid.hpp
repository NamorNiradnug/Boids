#pragma once
#include <QPointF>

#include <vector>

class Circle;
class QRect;
class QPainter;

double frandom();

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
