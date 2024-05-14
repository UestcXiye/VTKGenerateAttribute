#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_VTKGenerateAttribute.h"

#include <QVTKOpenGLNativeWidget.h>
#include <vtkCameraOrientationWidget.h>

class VTKGenerateAttribute : public QMainWindow
{
	Q_OBJECT

public:
	VTKGenerateAttribute(QWidget* parent = nullptr);
	~VTKGenerateAttribute();

private:
	Ui::VTKGenerateAttributeClass ui;

	QVTKOpenGLNativeWidget* _pVTKWidget = nullptr;
	vtkNew<vtkCameraOrientationWidget> cameraOrientationWidget; // ×ø±êÏµ
};
