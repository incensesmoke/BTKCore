/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2010, Arnaud Barré
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name(s) of the copyright holders nor the names
 *       of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "Viz3DWidget.h"

#include <btkVTKAxesWidget.h>
#include <btkVTKPickerInteractionCallback.h>
#include <btkVTKCommandEvents.h>
#include <btkVTKInteractorStyleTrackballCamera.h>

#include <vtkRenderWindow.h>
#include <vtkCellPicker.h>
#include <vtkCallbackCommand.h>
#include <vtkAxesActor.h>

Viz3DWidget::Viz3DWidget(QWidget* parent)
: QVTKWidget(parent)
{
  // Member
  this->mp_Renderer = vtkRenderer::New();
  this->mp_AxesWidget = btk::VTKAxesWidget::New();
  this->mp_EventQtSlotConnections = vtkEventQtSlotConnect::New();
}

Viz3DWidget::~Viz3DWidget()
{
  this->mp_Renderer->Delete();
  this->mp_AxesWidget->Delete();
  this->mp_EventQtSlotConnections->Delete();
};

void Viz3DWidget::initialize()
{
  // VTK UI
  //this->mp_Renderer->TwoSidedLightingOn();
  vtkRenderWindow* renwin = vtkRenderWindow::New();
  renwin->AddRenderer(this->mp_Renderer);
  renwin->LineSmoothingOn();
  //renwin->PolygonSmoothingOn();
  this->SetRenderWindow(renwin);
  renwin->Delete();
  // VTK cell picker
  vtkCellPicker* cellPicker = vtkCellPicker::New();
  cellPicker->SetTolerance(0.001);
  this->GetRenderWindow()->GetInteractor()->SetPicker(cellPicker);
  cellPicker->Delete();
  // VTK interaction style
  btk::VTKInteractorStyleTrackballCamera* style = btk::VTKInteractorStyleTrackballCamera::New();
  vtkCallbackCommand* pickerMouseInteraction = vtkCallbackCommand::New();
  pickerMouseInteraction->SetClientData(style);
  pickerMouseInteraction->SetCallback(&btk::VTKPickerInteractionCallback);
  style->AddObserver(vtkCommand::LeftButtonPressEvent, pickerMouseInteraction);
  style->AddObserver(vtkCommand::MouseMoveEvent, pickerMouseInteraction);
  style->AddObserver(vtkCommand::LeftButtonReleaseEvent, pickerMouseInteraction);
  this->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);
  pickerMouseInteraction->Delete();
  style->Delete();
  
  // VTK WIDGET
  this->mp_AxesWidget->SetParentRenderer(this->mp_Renderer);
  this->mp_AxesWidget->SetInteractor(this->GetRenderWindow()->GetInteractor());
  this->mp_AxesWidget->SetViewport(0.0, 0.0, 0.15, 0.2);
  vtkAxesActor* axesActor = this->mp_AxesWidget->GetAxesActor();
  axesActor->SetShaftTypeToCylinder();
  axesActor->SetTotalLength( 1.25, 1.25, 1.25 );
  axesActor->SetCylinderRadius( 0.500 * axesActor->GetCylinderRadius() );
  axesActor->SetConeRadius( 1.025 * axesActor->GetConeRadius() );
  axesActor->SetSphereRadius( 1.500 * axesActor->GetSphereRadius() );
  this->mp_AxesWidget->SetEnabled(1);
  
  // Links between VTK & Qt
  this->mp_EventQtSlotConnections->Connect(
      this->GetRenderWindow()->GetInteractor(), 
      btk::VTKMarkerPickedEvent,
      this, 
      SLOT(selectPickedMarker(vtkObject*, unsigned long, void*, void*)));
  this->mp_EventQtSlotConnections->Connect(
      this->GetRenderWindow()->GetInteractor(), 
      btk::VTKMarkersPickedEvent,
      this, 
      SLOT(selectPickedMarkers(vtkObject*, unsigned long, void*, void*)));
};

void Viz3DWidget::selectPickedMarker(vtkObject* caller, unsigned long /* vtk_event */, void* /* client_data */, void* call_data)
{
  int id = *static_cast<int*>(call_data);
  emit pickedMarkerChanged(id);
};

void Viz3DWidget::selectPickedMarkers(vtkObject* caller, unsigned long /* vtk_event */, void* /* client_data */, void* call_data)
{
  int id = *static_cast<int*>(call_data);
  emit pickedMarkersChanged(id);
};

void Viz3DWidget::show(bool s)
{
  vtkActorCollection* actors = this->mp_Renderer->GetActors();
  actors->InitTraversal();
  vtkActor* actor = actors->GetNextItem(); // Ground
  actor = actors->GetNextItem();
  while (actor)
  {
    actor->SetVisibility(s ? true : false);
    actor = actors->GetNextItem();
  }
  this->GetRenderWindow()->Render();
};