#pragma once
#include <QMainWindow>
#include <qevent.h>
#include <QTimer>
#include <QWidget>
#include <QCheckBox>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QLayout>
#include <boid.h>
#include <cmath>


class Frame : public QMainWindow
{
    Q_OBJECT

public:
    Frame();
    ~Frame();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseRealeseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent *_);

public slots:
    void tick();
    void reloadBoids();
    void updateBoidsRadius();

private:
    //TODO chunks!!
    std::vector<Boid *> boids = std::vector<Boid *>(100);
    std::vector<Circle> circles = {
        Circle(QPointF(), 20.0),
        Circle(QPointF(200.0, 300.0), 70.0)
    };
    QPoint translate, last_pos;
    double scale = 1.0;
    Qt::MouseButton last_button = Qt::NoButton;
    QWidget *tool_bar = new QWidget(this);
    QCheckBox *radius_show_check_box = new QCheckBox("Show boids view", tool_bar);
    QPushButton *quit_button = new QPushButton("Quit", tool_bar);
    QPushButton *reload_button = new QPushButton("Reaload boids", tool_bar);
    QSlider *radius_slider = new QSlider(Qt::Horizontal, tool_bar);
    QLabel *radius_slider_text = new QLabel("Boids\nview", tool_bar);
    QTimer *tick_timer;
};
