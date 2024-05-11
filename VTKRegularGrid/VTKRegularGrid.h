#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_VTKRegularGrid.h"

#include <QVTKOpenGLNativeWidget.h>

class VTKRegularGrid : public QMainWindow
{
    Q_OBJECT

public:
    VTKRegularGrid(QWidget *parent = nullptr);
    ~VTKRegularGrid();

private:
    Ui::VTKRegularGridClass ui;

    QVTKOpenGLNativeWidget* _pVTKWidget = nullptr;
};
