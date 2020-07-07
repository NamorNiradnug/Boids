#pragma once
#include <QMainWindow>
#include <QTimer>

#include <cmath>

#include "settingsbox.hpp"
#include "boid.hpp"
#include "circle.hpp"

class Frame : public QMainWindow
{
    Q_OBJECT

public:
    Frame();
    ~Frame();
    enum Mode
    {
        Boids,
        ChangeCicrleCenter,
        ChangeCircleRadius
    };
    int boidsNumber();
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent *_);

public slots:
    void tick();
    void reloadBoids();
    void updateBoidsRadius();
    void updateBoidsNumber();
    void startAddCircle();
    void changeScale();

private:
    QRect boidsSpawnRect();
    QPointF onMap(QPointF pos);
    void setMode(Mode mode);

    std::vector<Boid *> boids = std::vector<Boid *>(100);
    std::vector<Circle> circles =
    {
        Circle(QPointF(), 20.0),
        Circle(QPointF(200.0, 300.0), 70.0)
    };
    QPoint translate, last_pos;
    double scale = 1.0, not_scaled = 0.0;
    Qt::MouseButton last_button = Qt::NoButton;
    SettingsBox *settings = new SettingsBox(this);
    QTimer *tick_timer, *draw_timer;
    Mode mode = Mode::Boids;
    QPointF new_circle_center;
    bool paused = false;
};
