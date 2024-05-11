#include "VTKRegularGrid.h"

#include <vtkNamedColors.h>
#include <vtkImageData.h>
#include <vtkRectilinearGrid.h>
#include <vtkDoubleArray.h>
#include <vtkStructuredGrid.h>
#include <vtkDataSetMapper.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

VTKRegularGrid::VTKRegularGrid(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	_pVTKWidget = new QVTKOpenGLNativeWidget();
	this->setCentralWidget(_pVTKWidget);

	vtkNew<vtkNamedColors> colors;

	// vtkImageData
	int gridSize = 5;
	vtkNew<vtkImageData> imageData;
	imageData->SetExtent(0, gridSize - 1, 0, gridSize - 1, 0, gridSize - 1);
	double dbSpace[] = { 1, 2, 3 };
	imageData->SetSpacing(dbSpace);

	vtkNew<vtkDataSetMapper> imageDataMapper;
	imageDataMapper->SetInputData(imageData);

	vtkNew<vtkActor> imageDataActor;
	imageDataActor->GetProperty()->SetRepresentationToWireframe(); // 设置为线框模式
	imageDataActor->SetMapper(imageDataMapper);

	// vtkRectilinearGrid
	vtkNew<vtkRectilinearGrid> rectilinearGrid;
	rectilinearGrid->SetExtent(0, gridSize - 1, 0, gridSize - 1, 0, gridSize - 1);
	vtkNew<vtkDoubleArray> xCoords;
	xCoords->SetNumberOfComponents(1);
	vtkNew<vtkDoubleArray> yCoords;
	yCoords->SetNumberOfComponents(1);
	vtkNew<vtkDoubleArray> zCoords;
	zCoords->SetNumberOfComponents(1);
	for (size_t i = 0; i < gridSize; i++)
	{
		if (i == 0)
		{
			xCoords->InsertNextValue(0);
			yCoords->InsertNextValue(0);
			zCoords->InsertNextValue(0);
		}
		else
		{
			double oldX = xCoords->GetValue(i - 1);
			xCoords->InsertNextValue(oldX + i * i);
			double oldY = yCoords->GetValue(i - 1);
			yCoords->InsertNextValue(oldY + i * i);
			double oldZ = zCoords->GetValue(i - 1);
			zCoords->InsertNextValue(oldZ + i * i);
		}
	}
	rectilinearGrid->SetXCoordinates(xCoords);
	rectilinearGrid->SetYCoordinates(yCoords);
	rectilinearGrid->SetZCoordinates(zCoords);

	vtkNew<vtkDataSetMapper> rectilinearGridMapper;
	rectilinearGridMapper->SetInputData(rectilinearGrid);

	vtkNew<vtkActor> rectilinearGridActor;
	rectilinearGridActor->GetProperty()->SetRepresentationToWireframe(); // 设置为线框模式
	rectilinearGridActor->SetMapper(rectilinearGridMapper);

	// vtkStructuredGrid
	vtkNew<vtkStructuredGrid> structuredGrid;
	structuredGrid->SetExtent(0, gridSize - 1, 0, gridSize - 1, 0, gridSize - 1);
	vtkNew<vtkPoints> points;
	for (int i = 0; i < gridSize; i++)
	{
		for (int j = 0; j < gridSize; j++)
		{
			for (int k = 0; k < gridSize; k++)
			{
				double p[3] = { i, j + 0.5 * i, k };
				points->InsertNextPoint(p[0], p[1], p[2]);
			}
		}
	}
	structuredGrid->SetPoints(points);

	vtkNew<vtkDataSetMapper> structuredGridMapper;
	structuredGridMapper->SetInputData(structuredGrid);

	vtkNew<vtkActor> structuredGridActor;
	structuredGridActor->GetProperty()->SetRepresentationToWireframe(); // 设置为线框模式
	structuredGridActor->SetMapper(structuredGridMapper);


	// 设置 3 个视口
	vtkNew<vtkRenderer> leftRenderer;
	double leftViewport[4] = { 0, 0, 0.33, 1 };
	leftRenderer->SetViewport(leftViewport);
	leftRenderer->SetBackground(colors->GetColor3d("BurlyWood").GetData());
	leftRenderer->AddActor(imageDataActor);
	this->_pVTKWidget->renderWindow()->AddRenderer(leftRenderer);

	vtkNew<vtkRenderer> midRenderer;
	double midViewport[4] = { 0.33, 0, 0.67, 1 };
	midRenderer->SetViewport(midViewport);
	midRenderer->SetBackground(colors->GetColor3d("CadetBlue").GetData());
	midRenderer->AddActor(rectilinearGridActor);
	this->_pVTKWidget->renderWindow()->AddRenderer(midRenderer);

	vtkNew<vtkRenderer> rightRenderer;
	double rightViewport[4] = { 0.67, 0, 1, 1 };
	rightRenderer->SetViewport(rightViewport);
	rightRenderer->SetBackground(colors->GetColor3d("Plum").GetData());
	rightRenderer->AddActor(structuredGridActor);
	this->_pVTKWidget->renderWindow()->AddRenderer(rightRenderer);
	this->_pVTKWidget->renderWindow()->Render();
}

VTKRegularGrid::~VTKRegularGrid()
{}
