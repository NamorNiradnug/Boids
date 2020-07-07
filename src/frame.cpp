#include <QMouseEvent>
#include <QKeyEvent>
#include <QPainter>

#include "frame.hpp"

Frame::Frame()
{
    reloadBoids();
    tick_timer = new QTimer(this);
    draw_timer = new QTimer(this);
    connect(tick_timer, SIGNAL(timeout()), this, SLOT(tick()));
    connect(draw_timer, SIGNAL(timeout()), this, SLOT(update()));
    tick_timer->setInterval(60);
    draw_timer->setInterval(20);
    tick_timer->start();
    draw_timer->start();
}

Frame::~Frame()
{
    tick_timer->stop();
    draw_timer->stop();
    delete tick_timer;
    delete draw_timer;
}

int Frame::boidsNumber()
{
    return boids.size();
}

void Frame::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        if (paused)
        {
            setMode(Mode::Boids);
        }
        else
        {
            paused = true;
        }
    }
}

void Frame::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        last_pos = event->pos();
    }

    last_button = event->button();
}

void Frame::mouseMoveEvent(QMouseEvent *event)
{
    if (last_button == Qt::RightButton)
    {
        translate += (event->pos() - last_pos) / scale;
    }

    last_pos = event->pos();
}

void Frame::mouseReleaseEvent(QMouseEvent *event)
{
    last_button = Qt::NoButton;

    if (event->button() == Qt::LeftButton)
    {
        switch (mode)
        {

            case (Mode::ChangeCicrleCenter):

                new_circle_center = onMap(event->pos());
                setMode(Mode::ChangeCircleRadius);
                break;

            case (Mode::ChangeCircleRadius):
                circles.push_back(Circle(new_circle_center, sqrt(squeresSum(new_circle_center
                                         - onMap(event->pos())))));
                setMode(Mode::ChangeCicrleCenter);
                break;

            case (Mode::DeleteCircle):
                for (int i = circles.size() - 1; i >= 0; i--)
                {
                    if (circles[i].contains(onMap(event->pos())))
                    {
                        circles.erase(circles.begin() + i);
                        break;
                    }
                }

                break;
        }
    }
}

void Frame::wheelEvent(QWheelEvent *event)
{
    not_scaled += event->angleDelta().y() / 480.0;
}

void Frame::paintEvent(QPaintEvent *_)
{
    QPainter *painter = new QPainter(this);
    QPoint cursor_pos = cursor().pos() - geometry().topLeft();
    painter->setPen(Qt::black);
    painter->save();
    painter->scale(scale, scale);
    painter->translate(translate + QPoint(width(), height()) / (2 * scale));
    painter->setBrush(Qt::red);

    if (mode != Mode::DeleteCircle)
    {
        for (Circle circle : circles)
        {
            painter->drawEllipse(circle.center(), circle.radius(), circle.radius());
        }
    }
    else
    {
        for (Circle circle : circles)
        {
            if (circle.contains(onMap(QPointF(cursor_pos.x(), cursor_pos.y()))))
            {
                painter->setOpacity(0.5);
                painter->drawEllipse(circle.center(), circle.radius(), circle.radius());
                painter->setOpacity(1.0);
            }
            else
            {
                painter->drawEllipse(circle.center(), circle.radius(), circle.radius());

            }
        }
    }

    if (settings->showBoidsView())
    {
        painter->setPen(Qt::blue);
        painter->setBrush(QColor(0, 0, 128, 10));

        for (Boid *boid : boids)
        {
            painter->drawEllipse(boid->getPos(), Boid::view_radius, Boid::view_radius);
        }
    }

    painter->setPen(Qt::black);
    painter->setBrush(Qt::white);

    for (Boid *boid : boids)
    {
        boid->draw(painter);
    }

    painter->restore();
    painter->setBrush(Qt::red);
    painter->setOpacity(0.5);

    switch (mode)
    {
        case (Mode::ChangeCicrleCenter):
            painter->drawEllipse(cursor_pos, 40 * scale, 40 * scale);
            break;

        case (Mode::ChangeCircleRadius):
            double radius = scale * sqrt(squeresSum(new_circle_center - onMap(cursor_pos)));
            painter->drawEllipse(scale * (new_circle_center
                                          + translate
                                          + QPoint(width(), height()) / (2 * scale)),
                                 radius, radius);
    }

    painter->end();
}

void Frame::tick()
{
    if (!paused)
    {
        for (Boid *boid : boids)
        {
            boid->tick(boids, circles);
        }
    }

    changeScale();
}

void Frame::reloadBoids()
{
    QRect spawn_rect = boidsSpawnRect();

    for (unsigned i = 0; i < boids.size(); i++)
    {
        boids[i] = new Boid(spawn_rect);
    }
}

void Frame::updateBoidsRadius()
{
    Boid::view_radius = (double)settings->boidsRadius();
}

void Frame::updateBoidsNumber()
{
    int n = boidsNumber();
    QRect spawn_rect = boidsSpawnRect();

    while (n < settings->boidsNumber())
    {
        boids.push_back(new Boid(spawn_rect));
        n++;
    }

    while (n > settings->boidsNumber())
    {
        boids.pop_back();
        n--;
    }

}

void Frame::addCircle()
{
    setMode(Mode::ChangeCicrleCenter);
}

void Frame::deleteCircle()
{
    setMode(Mode::DeleteCircle);
}

void Frame::changeScale()
{
    if (abs(not_scaled) > 0.1)
    {
        scale += not_scaled / 5;
        not_scaled -= not_scaled / 5;
        scale = std::min(scale, 6.5);
        scale = std::max(scale, 0.3);
    }
}

QRect Frame::boidsSpawnRect()
{
    return QRect(-translate
                 - QPoint(geometry().width(), geometry().height()) / (2 * scale), size() / scale);
}

QPointF Frame::onMap(QPointF pos)
{
    return pos / scale - translate - QPoint(geometry().width(),
                                            geometry().height()) / (2 * scale);

}

void Frame::setMode(Frame::Mode mode)
{
    switch (mode)
    {
        case Mode::Boids:
            paused = false;
            break;

        default:
            paused = true;
    }

    this->mode = mode;
}
