#include <QRect>
#include <QPainter>

#include <cmath>

#include "circle.hpp"
#include "boid.hpp"

double frandom()
{
    return (double)rand() / RAND_MAX;
}

double squeresSum(QPointF point)
{
    return point.x() * point.x() + point.y() * point.y();
}

double Boid::view_radius = 70.0;

Boid::Boid(QRect rect)
{
    pos = QPointF(frandom() * rect.width(), frandom() * rect.height()) + rect.topLeft();
    double x_speed = frandom();
    speed = QPointF(x_speed, sqrt(1 - x_speed * x_speed));
}

QPointF Boid::getPos()
{
    return pos;
}

QPointF Boid::getSpeed()
{
    return speed;
}

void Boid::tick(std::vector<Boid *> boids, std::vector<Circle> circles)
{
    int counter = 0;
    QPointF center = QPointF();
    QPointF speed_sum = speed;
    QPointF from_others = QPointF();
    QPointF from_circles = QPointF();
    QPointF delta;
    double dist;

    for (Circle circle : circles)
    {
        delta = pos - circle.center();
        dist = sqrt(squeresSum(delta)) - circle.radius();

        if (dist <= 0)
        {
            pos = circle.center() + delta * (0.01 + circle.radius() / sqrt(squeresSum(delta)));
        }

        if (dist <= Boid::view_radius)
        {
            from_circles += 4 * delta / (dist * dist * 3);
        }

        delta = pos - circle.center();
    }

    for (Boid *boid : boids)
    {
        delta = pos - boid->getPos();

        if (boid != this && squeresSum(delta) < Boid::view_radius * Boid::view_radius)
        {
            center += boid->getPos();
            speed_sum += boid->getSpeed();

            if (delta != QPointF())
            {
                from_others += delta / sqrt(squeresSum(delta));
            }
            else
            {
                from_others -= boid->getSpeed();
            }

            counter++;
        }
    }

    if (counter)
    {
        center /= counter;

        if (center == pos)
        {
            center += QPointF(1.0, 0.0);
        }

        speed = ((center - pos) / sqrt(squeresSum(center - pos))
                 + from_others / (double)counter
                 + speed_sum / (double)counter
                );
        speed /= sqrt(squeresSum(speed));
    }

    speed += from_circles;
    speed /= sqrt(squeresSum(speed));
    pos = pos + 2 * speed;
}

void Boid::draw(QPainter *painter)
{
    const QPointF triang[3] =
    {
        pos + speed * 12,
        pos - speed * 3 - QPointF(-speed.y(), speed.x()) * 5,
        pos - speed * 3 + QPointF(-speed.y(), speed.x()) * 5
    };
    painter->drawPolygon(triang, 3);
}

