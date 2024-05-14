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

	// vtkElevationFilter���߳����Թ�����
	// ���û�����һ���ߣ������ݼ�������е�ͶӰ���������ϣ�����ͶӰ���ȷ��ÿ�������������
	// �򵥵�˵�����ǽ�����ɫӳ�䣬�� RGB ��ֵ�� LowPoint �� HighPoint ���б仯
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
	// ��ɫ��
	vtkNew<vtkScalarBarActor> colorBarActor;
	colorBarActor->SetLookupTable(mapper->GetLookupTable());
	colorBarActor->SetMaximumHeightInPixels(500);

	renderer->AddActor(actor);
	renderer->AddActor2D(colorBarActor);

	// ������ϵ
	cameraOrientationWidget->SetParentRenderer(renderer);
	cameraOrientationWidget->On();
}

VTKGenerateAttribute::~VTKGenerateAttribute()
{}
