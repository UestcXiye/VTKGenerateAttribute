#include "VTKGenerateAttribute.h"

#include <vtkSphereSource.h>
#include <vtkNamedColors.h>
#include <vtkElevationFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkScalarBarActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

VTKGenerateAttribute::VTKGenerateAttribute(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	_pVTKWidget = new QVTKOpenGLNativeWidget();
	this->setCentralWidget(_pVTKWidget);

	vtkNew<vtkRenderer> renderer;
	this->_pVTKWidget->renderWindow()->AddRenderer(renderer);
	this->_pVTKWidget->renderWindow()->Render();

	vtkNew<vtkNamedColors> colors;
	renderer->SetBackground(colors->GetColor3d("DarkSlateGray").GetData());

	vtkNew<vtkSphereSource> sphere;
	sphere->SetThetaResolution(51);
	sphere->SetPhiResolution(17);
	sphere->SetRadius(0.5);

	// vtkElevationFilter：高程属性过滤器
	// 由用户设置一条线，将数据集里的所有点投影到这条线上，根据投影结果确定每个点的属性数据
	// 简单地说，就是进行颜色映射，让 RGB 的值从 LowPoint 到 HighPoint 进行变化
	vtkNew<vtkElevationFilter> elevation;
	elevation->SetInputConnection(sphere->GetOutputPort());
	elevation->SetLowPoint(0, 0, -0.5);
	elevation->SetHighPoint(0, 0, 0.5);
	double range[] = { -1, 1 };
	elevation->SetScalarRange(range);
	elevation->Update();
	elevation->GetOutput()->Print(std::cout);

	vtkNew<vtkPolyDataMapper> mapper;
	mapper->SetInputConnection(elevation->GetOutputPort());
	mapper->SetScalarRange(range);

	vtkNew<vtkActor> actor;
	actor->SetMapper(mapper);
	// 颜色条
	vtkNew<vtkScalarBarActor> colorBarActor;
	colorBarActor->SetLookupTable(mapper->GetLookupTable());
	colorBarActor->SetMaximumHeightInPixels(500);

	renderer->AddActor(actor);
	renderer->AddActor2D(colorBarActor);

	// 打开坐标系
	cameraOrientationWidget->SetParentRenderer(renderer);
	cameraOrientationWidget->On();
}

VTKGenerateAttribute::~VTKGenerateAttribute()
{}
