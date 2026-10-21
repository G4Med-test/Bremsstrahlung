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
/// \file medical/electronScattering/include/DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class
//
// $Id: DetectorConstruction.hh 88035 2015-01-26 10:22:08Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"

#include "globals.hh"

class G4Material;
class DetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    DetectorConstruction();
   ~DetectorConstruction();

  public:

     void SetMaterialTarget (G4String);
     void SetThicknessTarget(G4double);
     void SetRadiusTarget(G4double);
     void SetParameterA(G4double);
     void SetParameterB(G4double);
     void SetParameterC(G4double);
     void SetParameterD(G4double);
     void SetParameterE(G4double);
     void SetParameterF(G4double);

     virtual G4VPhysicalVolume* Construct();

     void UpdateGeometry();

  public:

     void PrintGeometry();

     G4Material* GetMaterialTarget();//        {return fMaterial_Target;};
     G4double    GetThicknessTarget() ;//      {return fThickness_Target;};
     G4double    GetRadiusTarget() ;//      {return fThickness_Target;};

     const G4VPhysicalVolume* GetpvolWorld() {return fPvol_World;};

  private:
  
     G4Material*        fMaterial_World;
     G4Material*        fMaterial_Scorer;

     G4Material*        fMaterial_ExitWindow;
     G4double           fThickness_ExitWindow;
     G4double           fZback_ExitWindow;

     G4Material*        fMaterial_AirSlab1;
     G4double           fThickness_AirSlab1;
     G4double           fZback_AirSlab1;

     G4Material*        fMaterial_TCM;
     G4double           fThickness_TCM;
     G4double           fZback_TCM;

     G4Material*        fMaterial_AirSlab2;
     G4double           fThickness_AirSlab2;
     G4double           fZback_AirSlab2;
     
     G4Material*        fMaterial_Chamber;
     G4double           fThickness_Chamber;
     G4double           fZback_Chamber;

     G4Material*        fMaterial_Vacuum1;
     G4double           fThickness_Vacuum1;
     G4double           fZback_Vacuum1;
     
     G4Material*        fMaterial_Target;
     G4double           fThickness_Target;
     G4double           fZback_Target;
     G4double           fRadiusTarget;


     G4double	        fParameterA;
     G4double	        fParameterB;
     G4double	        fParameterC;
     G4double	        fParameterD;
     G4double	        fParameterE;
     G4double	        fParameterF;

     G4VPhysicalVolume* fPvol_World;
     
     DetectorMessenger* fDetectorMessenger;
      
  private:
    
     void DefineMaterials();
     void GeometryParameters();
     G4VPhysicalVolume* ConstructVolumes();     
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#endif

