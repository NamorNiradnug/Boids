#include "frame.hpp"
#include "settingsbox.hpp"

SettingsBox::SettingsBox(Frame *parent) : QGroupBox(parent)
{
    setupUi(this);
    radius_slider->setSliderPosition(Boid::view_radius);
    boids_number_slider->setSliderPosition(parent->boidsNumber());
    connect(radius_slider, SIGNAL(valueChanged(int)), parent, SLOT(updateBoidsRadius()));
    connect(boids_number_slider, SIGNAL(valueChanged(int)), parent, SLOT(updateBoidsNumber()));
    connect(add_circle_button, SIGNAL(released()), parent, SLOT(addCircle()));
    connect(delete_obstacle_button, SIGNAL(released()), parent, SLOT(deleteCircle()));
    connect(reload_button, SIGNAL(released()), parent, SLOT(reloadBoids()));
    connect(quit_button, SIGNAL(released()), parent, SLOT(close()));
}

bool SettingsBox::showBoidsView()
{
    return boids_view_show->isChecked();
}

int SettingsBox::boidsRadius()
{
    return radius_slider->value();
}

int SettingsBox::boidsNumber()
{
    return boids_number_slider->value();
}


