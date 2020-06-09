#include "frame.h"


Frame::Frame()
{
    tool_bar->setGeometry(0, 0, 174, 123);
    radius_show_check_box->setGeometry(12, 0, 160, 30);
    radius_slider->setGeometry(62, 35, 110, 30);
    radius_slider->setMinimum(40);
    radius_slider->setMaximum(100);
    radius_slider->setSliderPosition(Boid::view_radius);
    radius_slider->connect(
                radius_slider, SIGNAL(valueChanged(int)),
                this, SLOT(updateBoidsRadius())
                );
    radius_slider_text->setGeometry(12, 30, 50, 30);
    radius_slider_text->setAlignment(Qt::AlignCenter);
    reload_button->setGeometry(12, 60, 160, 30);
    reload_button->connect(reload_button, SIGNAL(released()), this, SLOT(reloadBoids()));
    quit_button->setGeometry(12, 90, 160, 30);
    quit_button->connect(quit_button, SIGNAL(released()), this, SLOT(close()));
    tick_timer = new QTimer(this);
    tick_timer->connect(tick_timer, SIGNAL(timeout()), this, SLOT(tick()));
    translate = QPoint();
    resize(QSize(640, 480));
    reloadBoids();
    tick_timer->start(60);
}

Frame::~Frame()
{
    tick_timer->stop();
    delete tick_timer;
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

void Frame::mouseRealeseEvent(QMouseEvent *event)
{
    last_button = Qt::NoButton;
}

void Frame::wheelEvent(QWheelEvent *event)
{
    double k = (double)event->angleDelta().y() / 240.0;
    scale += k * sqrt(scale);
    scale = std::min(scale, 8.0);
    scale = std::max(scale, 0.3);
}

void Frame::paintEvent(QPaintEvent *_)
{
    QPainter *painter = new QPainter(this);
    painter->save();
    painter->setPen(Qt::black);
    painter->scale(scale, scale);
    painter->translate(translate + QPoint(width(), height()) / (2 * scale));
    painter->setBrush(Qt::red);
    for (Circle circle : circles)
    {
        painter->drawEllipse(QRectF(circle.center() - QPointF(circle.radius(), circle.radius()),
                                    2 * QSizeF(circle.radius(), circle.radius())
                                 )
                             );
    }
    if (radius_show_check_box->isChecked())
    {
        painter->setPen(Qt::blue);
        painter->setBrush(QColor(0, 0, 128, 50));
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
    painter->end();
}

void Frame::tick()
{
    for (Boid *boid : boids)
    {
        boid->tick(boids, circles);
    }
    update();
}

void Frame::reloadBoids()
{
    for (unsigned i = 0; i < boids.size(); i++)
    {
        boids[i] = new Boid(QRect(
                                -translate - QPoint(width(), height()) / (2 * scale),
                                size() / scale
                                ));
    }
}

void Frame::updateBoidsRadius()
{
    Boid::view_radius = radius_slider->sliderPosition() * 1.0;
}
