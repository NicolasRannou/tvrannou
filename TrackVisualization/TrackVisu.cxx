#include "vtkPolyData.h"
#include "vtkPolyDataReader.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"

#include "vtkPoints.h"

#include "vtkSphereSource.h"

#include <stdio.h>
#include <stdlib.h>


int main( int argc, char *argv[] )
{
  
  if (argc != 2) {
    cout << "You must specify one filename (something like /home/user/data/figure.vtk)" << endl;
    return 1;
  }  

  // Read the Target polydata.
  vtkPolyDataReader *reader = vtkPolyDataReader::New();
  reader->SetFileName(argv[1]);
  reader->Update();

  // Extract the points
  vtkPoints *dataPoints = reader->GetOutput()->GetPoints();
  int numberOfPoints = dataPoints->GetNumberOfPoints(); 
  cout << "number of points= " << numberOfPoints << endl;

  if (numberOfPoints < 1) {
    cout << "Your polydata does not contain any point" << endl;
    return 1;
  } 

  double *barycenter = new double[3];
  // Initialization of the barycenter
  barycenter[0] = 0; barycenter[1] = 0; barycenter[2] = 0;
  //cout<< "Barycenter: "<< barycenter[0] << " "<<barycenter[1] << " " <<barycenter[2] << endl;
  
  // Get barycenter of all the points
  for (int i = 0; i < numberOfPoints; i++) {
    // Print position of each point
    //cout<< "Point " << i << ": " << dataPoints->GetPoint(i)[0] << " " << dataPoints->GetPoint(i)[1] << " " << dataPoints->GetPoint(i)[2] << endl;
    barycenter[0] += dataPoints->GetPoint(i)[0];
    barycenter[1] += dataPoints->GetPoint(i)[1];
    barycenter[2] += dataPoints->GetPoint(i)[2];
  }

  barycenter[0] = barycenter[0]/numberOfPoints;
  barycenter[1] = barycenter[1]/numberOfPoints;
  barycenter[2] = barycenter[2]/numberOfPoints;

  cout<< "Barycenter: "<< barycenter[0] << " "<<barycenter[1] << " " <<barycenter[2] << endl;

//VISUALIZATION

  // Creation of the sphere/barycenter actor
  vtkSphereSource *barycenterSphere = vtkSphereSource::New();
  barycenterSphere->SetCenter(barycenter[0],barycenter[1],barycenter[2]);

  vtkPolyDataMapper *sphereMapper = vtkPolyDataMapper::New();
  sphereMapper->SetInput(barycenterSphere->GetOutput());

  double rgb[3];
  rgb[0] = 1; rgb[1] = 0; rgb[2] = 0;
  
  vtkActor *sphereActor = vtkActor::New();
  sphereActor->SetMapper(sphereMapper);
  sphereMapper->Delete();
  sphereActor->GetProperty()->SetColor(rgb);

  barycenterSphere->Delete();

  // Creation of the polydata actor

  // Mapper
  // The mapper is responsible for pushing the geometry into the graphics
  // library. It may also do color mapping, if scalars or other attributes
  // are defined.
  //
  vtkPolyDataMapper *dataMapper = vtkPolyDataMapper::New();
  dataMapper->SetInputConnection(reader->GetOutputPort());

  // The actor is a grouping mechanism: besides the geometry (mapper), it
  // also has a property, transformation matrix, and/or texture map.
  // Here we set its color and rotate it -22.5 degrees.
  vtkActor *dataActor = vtkActor::New();
  dataActor->SetMapper(dataMapper);
  //dataActor->GetProperty()->SetColor(1.0000, 0.3882, 0.2784);
  //dataActor->RotateX(30.0);
  //dataActor->RotateY(-45.0);

  // Create the graphics structure. The renderer renders into the 
  // render window. The render window interactor captures mouse events
  // and will perform appropriate camera or actor manipulation
  // depending on the nature of the events.
  //
  vtkRenderer *ren1 = vtkRenderer::New();
  vtkRenderWindow *renWin = vtkRenderWindow::New();
  renWin->AddRenderer(ren1);
  vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
  iren->SetRenderWindow(renWin);

  // Add the actors to the renderer, set the background and size
  //
  ren1->AddActor(dataActor);
  ren1->AddActor(sphereActor);
  //ren1->SetBackground(0.1, 0.2, 0.4);
  //renWin->SetSize(200, 200);

  // We'll zoom in a little by accessing the camera and invoking a "Zoom"
  // method on it.
  ren1->ResetCamera();
  //ren1->GetActiveCamera()->Zoom(1.5);
  renWin->Render();
  
  // This starts the event loop and as a side effect causes an initial render.
  iren->Start();

  // Exiting from here, we have to delete all the instances that
  // have been created.
  dataMapper->Delete();
  dataActor->Delete();
  ren1->Delete();
  renWin->Delete();
  iren->Delete();

  return 0;
}


