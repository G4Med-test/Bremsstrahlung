//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file medical/electronScattering/src/DetectorMessenger.cc
/// \brief Implementation of the DetectorMessenger class
//
// $Id: DetectorMessenger.cc 86064 2014-11-07 08:49:32Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction * det)
:fDetector(det),
 fTestemDir(0), fDetDir(0), fMaterCmd(0), fThickCmd(0), fUpdateCmd(0)
{ 
  fTestemDir = new G4UIdirectory("/testem/");
  fTestemDir->SetGuidance("UI commands specific to this example.");
  
  fDetDir = new G4UIdirectory("/testem/det/");
  fDetDir->SetGuidance("detector construction commands");
      
  fMaterCmd = new G4UIcmdWithAString("/testem/det/setMaterial",this);
  fMaterCmd->SetGuidance("Select Material of the Scatter Foil");
  fMaterCmd->SetParameterName("material",false);
  
  fThickCmd = new G4UIcmdWithADoubleAndUnit("/testem/det/setThickness",this);
  fThickCmd->SetGuidance("Set Thickness of the Scatter Foil");
  fThickCmd->SetParameterName("thickness",false);  
  fThickCmd->SetRange("thickness>0.");
  fThickCmd->SetUnitCategory("Length");  
  
  fRadiiCmd = new G4UIcmdWithADoubleAndUnit("/testem/det/setRadius",this);
  fRadiiCmd->SetGuidance("Set Radius of the Scatter Foil");
  fRadiiCmd->SetParameterName("radius",false);  
  fRadiiCmd->SetRange("radius>0.");
  fRadiiCmd->SetUnitCategory("Length");  

  fParACmd = new G4UIcmdWithADoubleAndUnit("/testem/det/setParameterA",this);
  fParACmd->SetGuidance("Set parameter A, see Faddegon et al, (2008). Medical Physics.");
  fParACmd->SetParameterName("parameterA",false);  
  fParACmd->SetUnitCategory("Length");  

  fParBCmd = new G4UIcmdWithADoubleAndUnit("/testem/det/setParameterB",this);
  fParBCmd->SetGuidance("Set parameter B, see Faddegon et al, (2008). Medical Physics.");
  fParBCmd->SetParameterName("parameterB",false);  
  fParBCmd->SetUnitCategory("Length");  

  fParCCmd = new G4UIcmdWithADoubleAndUnit("/testem/det/setParameterC",this);
  fParCCmd->SetGuidance("Set parameter C, see Faddegon et al, (2008). Medical Physics.");
  fParCCmd->SetParameterName("parameterC",false);  
  fParCCmd->SetUnitCategory("Length");  

  fParDCmd = new G4UIcmdWithADoubleAndUnit("/testem/det/setParameterD",this);
  fParDCmd->SetGuidance("Set parameter D, see Faddegon et al, (2008). Medical Physics.");
  fParDCmd->SetParameterName("parameterD",false);  
  fParDCmd->SetUnitCategory("Length");  

  fParECmd = new G4UIcmdWithADoubleAndUnit("/testem/det/setParameterE",this);
  fParECmd->SetGuidance("Set parameter E, see Faddegon et al, (2008). Medical Physics.");
  fParECmd->SetParameterName("parameterE",false);  
  fParECmd->SetUnitCategory("Length");  

  fParFCmd = new G4UIcmdWithADoubleAndUnit("/testem/det/setParameterF",this);
  fParFCmd->SetGuidance("Set parameter F, see Faddegon et al, (2008). Medical Physics.");
  fParFCmd->SetParameterName("parameterF",false);  
  fParFCmd->SetUnitCategory("Length");  

  fUpdateCmd = new G4UIcmdWithoutParameter("/testem/det/update",this);
  fUpdateCmd->SetGuidance("Update calorimeter geometry.");
  fUpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  fUpdateCmd->SetGuidance("if you changed geometrical value(s).");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete fMaterCmd; 
  delete fThickCmd; 
  delete fRadiiCmd; 
  delete fParACmd;
  delete fParBCmd;
  delete fParCCmd;
  delete fParDCmd;
  delete fParECmd;
  delete fParFCmd;
  delete fUpdateCmd;
  delete fDetDir;  
  delete fTestemDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if ( command == fMaterCmd )
   {fDetector->SetMaterialTarget(newValue);}
   
  if ( command == fThickCmd )
   {fDetector->SetThicknessTarget(fThickCmd->GetNewDoubleValue(newValue));}
   
  if ( command == fRadiiCmd )
   {fDetector->SetRadiusTarget(fRadiiCmd->GetNewDoubleValue(newValue));}

  if ( command == fParACmd )
   {fDetector->SetParameterA(fParACmd->GetNewDoubleValue(newValue));}

  if ( command == fParBCmd )
   {fDetector->SetParameterB(fParBCmd->GetNewDoubleValue(newValue));}

  if ( command == fParCCmd )
   {fDetector->SetParameterC(fParCCmd->GetNewDoubleValue(newValue));}

  if ( command == fParDCmd )
   {fDetector->SetParameterD(fParDCmd->GetNewDoubleValue(newValue));}

  if ( command == fParECmd )
   {fDetector->SetParameterE(fParECmd->GetNewDoubleValue(newValue));}

  if ( command == fParFCmd )
   {fDetector->SetParameterF(fParFCmd->GetNewDoubleValue(newValue));}

  if  ( command == fUpdateCmd )
   {fDetector->UpdateGeometry(); }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
