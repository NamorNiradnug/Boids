#include <QRect>
#include <QPainter>

#include <cmath>

#include "circle.hpp"
#include "boid.hpp"

double frandom()
{
    return (double)rand() / RAND_MAX;
}

double Boid::view_radius = 70.0;
const QRectF Boid::WORLD_BORDER = QRectF(-2000, -2000, 4000, 4000);

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
    QPointF from_border = QPointF();
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

    if (abs(pos.x()) >= WORLD_BORDER.width() / 2 - view_radius)
    {
        if (pos.x() < 0)
        {
            from_border.setX((WORLD_BORDER.width() + pos.x()));
        }
        else
        {
            from_border.setX(-(WORLD_BORDER.width() - pos.x()));
        }
    }

    if (abs(pos.y()) >= WORLD_BORDER.height() / 2 - view_radius)
    {
        if (pos.y() < 0)
        {
            from_border.setY((WORLD_BORDER.height() + pos.y()));
        }
        else
        {
            from_border.setY(-(WORLD_BORDER.height() - pos.y()));
        }
    }

    speed += from_circles + from_border / 60000;
    speed /= sqrt(squeresSum(speed));
    pos = pos + 2 * speed;
    pos.setX(std::min(pos.x(), WORLD_BORDER.right()));
    pos.setX(std::max(pos.x(), WORLD_BORDER.left()));
    pos.setY(std::min(pos.y(), WORLD_BORDER.bottom()));
    pos.setY(std::max(pos.y(), WORLD_BORDER.top()));
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

