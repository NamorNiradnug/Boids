#pragma once

#include "ui_settingsbox.h"

class Frame;

class SettingsBox : public QGroupBox, private Ui::SettingsBox
{
    Q_OBJECT

public:
    explicit SettingsBox(Frame *parent = nullptr);
    bool showBoidsView();
    int boidsRadius();
    int boidsNumber();
};

