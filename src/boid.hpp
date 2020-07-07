#pragma once
#include <QPointF>
#include <QRectF>

#include <vector>

class Circle;
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
    static const QRectF WORLD_BORDER;
private:
    QPointF speed, pos;
};
