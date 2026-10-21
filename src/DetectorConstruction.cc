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
/// \file medical/electronScattering/src/DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
// $Id: DetectorConstruction.cc 86064 2014-11-07 08:49:32Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
:fMaterial_World(0), fMaterial_Scorer(0),
fMaterial_ExitWindow(0), fMaterial_AirSlab1(0), fMaterial_TCM(0),
fMaterial_AirSlab2(0), fMaterial_Chamber(0), fMaterial_Vacuum1(0),
fMaterial_Target(0),
fPvol_World(0), fDetectorMessenger(0)
{
    // materials
    DefineMaterials();
 
    // geometry
    //GeometryParameters();
    
    // create commands for interactive definition of the calorimeter
    fDetectorMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
    delete fDetectorMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    return ConstructVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::GeometryParameters()
{
    // For Pb 10-30 MV
    fZback_ExitWindow = fParameterA; //-3.313*cm;
    fZback_AirSlab1   = fParameterB; //-3.3*cm;
    fZback_TCM        = fParameterC; //-2.915*cm;
    fZback_AirSlab2   = fParameterD; //-2.9*cm;
    fZback_Chamber    = fParameterE; //-1.6051*cm;
    fZback_Vacuum1    = fParameterF; //-1.6*cm;
    fZback_Target     = 0*cm;
    //fRadiusTarget     = 1.583*cm; //fRadiusTarget; //fParameterA; //1.583*cm;
    
    fThickness_ExitWindow = std::abs(fZback_AirSlab1-fZback_ExitWindow);
    fThickness_AirSlab1   = std::abs(fZback_TCM-fZback_AirSlab1);
    fThickness_TCM        = std::abs(fZback_AirSlab2-fZback_TCM);
    fThickness_AirSlab2   = std::abs(fZback_Chamber-fZback_AirSlab2);
    fThickness_Chamber    = std::abs(fZback_Vacuum1-fZback_Chamber);
    fThickness_Vacuum1    = std::abs(fZback_Target-fZback_Vacuum1);
    //fThickness_Target     = 1.85*cm ; //, 9.13, 11.43, 11.43, and 13.68 (10-30 MV);
    
}


void DetectorConstruction::DefineMaterials()
{
    G4double a, z, density;
    G4int ncomponents; 
    G4double fractionmass;
    G4double temperature, pressure;
    
    // define Elements
    //
    G4NistManager* nistManager = G4NistManager::Instance();
   
  
    G4Element* Be = nistManager->FindOrBuildElement("Be"); //new G4Element("Beryllium","Be", z= 4, a=   9.1218*g/mole);
    G4Element* C  = nistManager->FindOrBuildElement("C"); //new G4Element("Carbon",   "C",  z= 6, a=  12.0107*g/mole);
    G4Element* N  = nistManager->FindOrBuildElement("N"); //new G4Element("Nitrogen", "N",  z= 7, a=  14.0067*g/mole);
    G4Element* O  = nistManager->FindOrBuildElement("O"); //new G4Element("Oxygen",   "O",  z= 8, a=  15.9994*g/mole);
    G4Element* Al = nistManager->FindOrBuildElement("Al"); //new G4Element("Aluminum","Al", z=13, a=  26.9815*g/mole);
    G4Element* Ar = nistManager->FindOrBuildElement("Ar"); //new G4Element("Argon",    "Ar", z=18, a=  39.9480*g/mole);
    G4Element* Ti = nistManager->FindOrBuildElement("Ti"); //new G4Element("Titanium", "Ti", z=22, a=  47.8670*g/mole);
    G4Element* Si = nistManager->FindOrBuildElement("Si"); //new G4Element("Titanium", "Ti", z=22, a=  47.8670*g/mole);
    G4Element* Pb = nistManager->FindOrBuildElement("Pb"); //new G4Element("Gold",     "Au", z=79, a= 196.9666*g/mole);
    G4Element* Cr = nistManager->FindOrBuildElement("Cr"); //new G4Element("Gold",     "Au", z=79, a= 196.9666*g/mole);
    G4Element* Mn = nistManager->FindOrBuildElement("Mn"); //new G4Element("Gold",     "Au", z=79, a= 196.9666*g/mole);
    G4Element* Ni = nistManager->FindOrBuildElement("Ni"); //new G4Element("Gold",     "Au", z=79, a= 196.9666*g/mole);
    G4Element* Fe = nistManager->FindOrBuildElement("Fe"); //new G4Element("Gold",     "Au", z=79, a= 196.9666*g/mole);
    
    // Air
    //
    G4Material* Air =
    new G4Material("Air", density= 1.205*mg/cm3, ncomponents=4,
                   kStateGas, 293.*kelvin, 1.*atmosphere);
    Air->AddElement(C, fractionmass=0.000124);
    Air->AddElement(N, fractionmass=0.755267);
    Air->AddElement(O, fractionmass=0.231782);
    Air->AddElement(Ar,fractionmass=0.012827);
    
    // Titanium
    //
    G4Material* Titanium =
    new G4Material("Titanium", density= 4.54*g/cm3, ncomponents=1);
    Titanium->AddElement(Ti, fractionmass=1.0);
 
    // Silicon
    //
    G4Material* Silicon =
    new G4Material("Silicon", density= 2.33*g/cm3, ncomponents=1);
    Silicon->AddElement(Si, fractionmass=1.0);
   
    // Aluminum
    //
    G4Material* Aluminum =
    new G4Material("Aluminum", density= 2.699*g/cm3, ncomponents=1);
    Aluminum->AddElement(Al, fractionmass=1.0);
    
    // Beryllium
    //
    G4Material* Beryllium =
    new G4Material("Beryllium", density= 1.848*g/cm3, ncomponents=1);
    Beryllium->AddElement(Be, fractionmass=1.0);
    
    // Lead
    //
    G4Material* Lead =
    new G4Material("Lead", density= 11.34*g/cm3, ncomponents=1);
    Lead->AddElement(Pb, fractionmass=1.0);
    
    // StainlessSteel
    //
    G4Material* StainlessSteel =
    new G4Material("StainlessSteel", density= 8.06*g/cm3, ncomponents=6);
    StainlessSteel->AddElement(C,  fractionmass=0.001);
    StainlessSteel->AddElement(Si, fractionmass=0.007);
    StainlessSteel->AddElement(Cr, fractionmass=0.18);
    StainlessSteel->AddElement(Mn, fractionmass=0.01);
    StainlessSteel->AddElement(Fe, fractionmass=0.712);
    StainlessSteel->AddElement(Ni, fractionmass=0.09);

 // example of vacuum
    //
    density     = universe_mean_density;    //from PhysicalConstants.h
    pressure    = 3.e-18*pascal;
    temperature = 2.73*kelvin;
    G4Material* Vacuum =
    new G4Material("Galactic", z=1, a=1.01*g/mole,density,
                   kStateGas,temperature,pressure);
    
    //print
    //
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
    
    
    // assign materials
    //
    fMaterial_World       = Vacuum;
    fMaterial_ExitWindow  = Titanium;
    fMaterial_AirSlab1    = Air;
    fMaterial_TCM         = Silicon;
    fMaterial_AirSlab2    = Air;
    fMaterial_Chamber     = StainlessSteel;
    fMaterial_Vacuum1     = Vacuum;
    fMaterial_Target      = Lead;
    fMaterial_Scorer      = Vacuum;

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::ConstructVolumes()
{
  // Cleanup old geometry
  //
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  GeometryParameters();

  G4cout << "Parameter A (cm): " << fParameterA/cm << G4endl;
  G4cout << "Parameter B (cm): " << fParameterB/cm << G4endl;
  G4cout << "Parameter C (cm): " << fParameterC/cm << G4endl;
  G4cout << "Parameter D (cm): " << fParameterD/cm << G4endl;
  G4cout << "Parameter E (cm): " << fParameterE/cm << G4endl;
  G4cout << "Parameter F (cm): " << fParameterF/cm << G4endl;

    // World
    //
    G4Box*
    svol_World = new G4Box("World",                        //name
                           101*cm,
                           101*cm,
                           101*cm);
    
    G4LogicalVolume*
    lvol_World = new G4LogicalVolume(svol_World,            //its solid
                                     fMaterial_World,       //its material
                                     "World");              //its name
    
    fPvol_World = new G4PVPlacement(0,                      //no rotation
                                    G4ThreeVector(),        //no translation
                                    lvol_World,             //its logical volume
                                    "World",                //its name
                                    0,                      //its mother  volume
                                    false,                  //no boolean operation
                                    0);                     //copy number
    
    G4double radius = 2.0 * cm;
    //  Solids
    G4Tubs* exitWindow = new G4Tubs("exitWindow",
                                    0.0 * cm,
                                    radius,
                                    0.5 * fThickness_ExitWindow,
                                    0 * deg,
                                    360 * deg);
    
    G4Tubs* airSlab1 = new G4Tubs("airSlab1",
                                    0.0 * cm,
                                    radius,
                                    0.5 * fThickness_AirSlab1,
                                    0 * deg,
                                    360 * deg);

    G4Tubs* TCM = new G4Tubs("TCM",
                                    0.0 * cm,
                                    radius,
                                    0.5 * fThickness_TCM,
                                    0 * deg,
                                    360 * deg);

    G4Tubs* airSlab2 = new G4Tubs("airSlab2",
                                    0.0 * cm,
                                    radius,
                                    0.5 * fThickness_AirSlab2,
                                    0 * deg,
                                    360 * deg);

    
    G4Tubs* chamber = new G4Tubs("chamber",
                                    0.0 * cm,
                                    radius,
                                    0.5 * fThickness_Chamber,
                                    0 * deg,
                                    360 * deg);

    G4Tubs* vacuum1 = new G4Tubs("vacuum1",
                                    0.0 * cm,
                                    radius,
                                    0.5 * fThickness_Vacuum1,
                                    0 * deg,
                                    360 * deg);

    G4Tubs* target = new G4Tubs("target",
                                    0.0 * cm,
                                    fRadiusTarget,
                                    0.5 * fThickness_Target,
                                    0 * deg,
                                    360 * deg);

    G4double rMin = 100*cm;
    G4double rMax = 100.005*cm;
    G4double SPhi = 0.0*deg;
    G4double DPhi = 360*deg;
    G4double DTheta = 1.0*deg;
    G4double STheta = 0.0*deg;

    const G4double angles[8] = {0.0*deg, 1*deg, 2*deg, 4*deg, 10*deg, 30.0*deg, 60.0*deg, 90*deg}; 

    STheta = 0.0*deg;
    G4Sphere* scorer0 = new G4Sphere("scorer0",  rMin, rMax, SPhi, DPhi, STheta, DTheta/2); //0.5*deg);
    STheta = angles[1]-(DTheta/4)*deg; //0.75*deg;
    G4Sphere* scorer1 = new G4Sphere("scorer1",  rMin, rMax, SPhi, DPhi, STheta, DTheta/2); //0.5*deg);
    STheta = angles[2]-(DTheta/4)*deg;
    G4Sphere* scorer2 = new G4Sphere("scorer2",  rMin, rMax, SPhi, DPhi, STheta, DTheta/2); 
    STheta = angles[3]-(DTheta/2)*deg;
    G4Sphere* scorer3 = new G4Sphere("scorer3",  rMin, rMax, SPhi, DPhi, STheta, DTheta); 
    STheta = angles[4]-(DTheta/2)*deg;
    G4Sphere* scorer4 = new G4Sphere("scorer4",  rMin, rMax, SPhi, DPhi, STheta, DTheta); 
    STheta = angles[5]-(DTheta/2)*deg;
    G4Sphere* scorer5 = new G4Sphere("scorer5",  rMin, rMax, SPhi, DPhi, STheta, DTheta); 
    STheta = angles[6]-(DTheta/2)*deg;
    G4Sphere* scorer6 = new G4Sphere("scorer6",  rMin, rMax, SPhi, DPhi, STheta, DTheta); 
    STheta = angles[7]-(DTheta/2)*deg;
    G4Sphere* scorer7 = new G4Sphere("scorer7",  rMin, rMax, SPhi, DPhi, STheta, DTheta); 

    // Logicals
    G4LogicalVolume* exitWindow_log = new G4LogicalVolume(exitWindow, fMaterial_ExitWindow, "exitWindow"); 
    G4LogicalVolume* airSlab1_log   = new G4LogicalVolume(airSlab1, fMaterial_AirSlab1, "airSlab1"); 
    G4LogicalVolume* TCM_log        = new G4LogicalVolume(TCM, fMaterial_TCM, "TCM"); 
    G4LogicalVolume* airSlab2_log   = new G4LogicalVolume(airSlab2, fMaterial_AirSlab2, "airSlab2"); 
    G4LogicalVolume* chamber_log    = new G4LogicalVolume(chamber, fMaterial_Chamber, "chamber"); 
    G4LogicalVolume* vacuum1_log    = new G4LogicalVolume(vacuum1, fMaterial_Vacuum1, "vacuum1"); 
    G4LogicalVolume* target_log     = new G4LogicalVolume(target, fMaterial_Target, "target"); 
    G4LogicalVolume* scorer0_log     = new G4LogicalVolume(scorer0, fMaterial_Scorer, "scorer0"); 
    G4LogicalVolume* scorer1_log     = new G4LogicalVolume(scorer1, fMaterial_Scorer, "scorer1"); 
    G4LogicalVolume* scorer2_log     = new G4LogicalVolume(scorer2, fMaterial_Scorer, "scorer2"); 
    G4LogicalVolume* scorer3_log     = new G4LogicalVolume(scorer3, fMaterial_Scorer, "scorer3"); 
    G4LogicalVolume* scorer4_log     = new G4LogicalVolume(scorer4, fMaterial_Scorer, "scorer4"); 
    G4LogicalVolume* scorer5_log     = new G4LogicalVolume(scorer5, fMaterial_Scorer, "scorer5"); 
    G4LogicalVolume* scorer6_log     = new G4LogicalVolume(scorer6, fMaterial_Scorer, "scorer6"); 
    G4LogicalVolume* scorer7_log     = new G4LogicalVolume(scorer7, fMaterial_Scorer, "scorer7"); 
 
    // Positions
    G4double zpos;

    zpos = fZback_ExitWindow + 0.5 * fThickness_ExitWindow;
    new G4PVPlacement(0, G4ThreeVector(0,0,zpos), exitWindow_log, "exitWindow", lvol_World, false, 0);

    zpos = fZback_AirSlab1 + 0.5 * fThickness_AirSlab1;
    new G4PVPlacement(0, G4ThreeVector(0,0,zpos), airSlab1_log, "airSlab1", lvol_World, false, 0);

    zpos = fZback_TCM + 0.5 * fThickness_TCM;
    new G4PVPlacement(0, G4ThreeVector(0,0,zpos), TCM_log, "TCM", lvol_World, false, 0);

    zpos = fZback_AirSlab2 + 0.5 * fThickness_AirSlab2;
    new G4PVPlacement(0, G4ThreeVector(0,0,zpos), airSlab2_log, "airSlab2", lvol_World, false, 0);

   zpos = fZback_Chamber + 0.5 * fThickness_Chamber;
    new G4PVPlacement(0, G4ThreeVector(0,0,zpos), chamber_log, "chamber", lvol_World, false, 0);
    
    zpos = fZback_Vacuum1 + 0.5 * fThickness_Vacuum1;
    new G4PVPlacement(0, G4ThreeVector(0,0,zpos), vacuum1_log, "vacuum1", lvol_World, false, 0);
  
    zpos = fZback_Target + 0.5 * fThickness_Target;
    new G4PVPlacement(0, G4ThreeVector(0,0,zpos), target_log, "target", lvol_World, false, 0);

    new G4PVPlacement(0, G4ThreeVector(), scorer0_log, "scorer0", lvol_World, false, 0);
    new G4PVPlacement(0, G4ThreeVector(), scorer1_log, "scorer1", lvol_World, false, 1);
    new G4PVPlacement(0, G4ThreeVector(), scorer2_log, "scorer2", lvol_World, false, 2);
    new G4PVPlacement(0, G4ThreeVector(), scorer3_log, "scorer3", lvol_World, false, 3);
    new G4PVPlacement(0, G4ThreeVector(), scorer4_log, "scorer4", lvol_World, false, 4);
    new G4PVPlacement(0, G4ThreeVector(), scorer5_log, "scorer5", lvol_World, false, 5);
    new G4PVPlacement(0, G4ThreeVector(), scorer6_log, "scorer6", lvol_World, false, 6);
    new G4PVPlacement(0, G4ThreeVector(), scorer7_log, "scorer7", lvol_World, false, 7);

    // Visualization attributes
    G4VisAttributes * visCarbon = new G4VisAttributes(G4Colour(0.5,0.5,0.5));
    visCarbon->SetForceSolid(true);

    G4VisAttributes * visSteel = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
    visSteel->SetForceSolid(true);

    G4VisAttributes * visAluminum = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
    visAluminum->SetForceSolid(true);

    G4VisAttributes * visMylar = new G4VisAttributes(G4Colour(1,1,0));
    visMylar->SetForceSolid(true);

    G4VisAttributes * visKapton = new G4VisAttributes(G4Colour(241.0/255,224.0/255,0));
    visKapton->SetForceSolid(true);

    G4VisAttributes * visTantalum = new G4VisAttributes(G4Colour(0,1,0));
    visTantalum->SetForceSolid(true);

    exitWindow_log->SetVisAttributes(visCarbon);
    airSlab1_log->SetVisAttributes(visSteel);
    TCM_log->SetVisAttributes(visAluminum);
    airSlab2_log->SetVisAttributes(visMylar);
    chamber_log->SetVisAttributes(visKapton);
    target_log->SetVisAttributes(visTantalum);

    G4Region* region = G4RegionStore::GetInstance()->FindOrCreateRegion("split");
    target_log->SetRegion(region);
    region->AddRootLogicalVolume(target_log);

    G4Region* regionSc = G4RegionStore::GetInstance()->FindOrCreateRegion("scorer");
    scorer0_log->SetRegion(regionSc);
    scorer1_log->SetRegion(regionSc);
    scorer2_log->SetRegion(regionSc);
    scorer3_log->SetRegion(regionSc);
    scorer4_log->SetRegion(regionSc);
    scorer5_log->SetRegion(regionSc);
    scorer6_log->SetRegion(regionSc);
    scorer7_log->SetRegion(regionSc);

    regionSc->AddRootLogicalVolume(scorer0_log);
    regionSc->AddRootLogicalVolume(scorer1_log);
    regionSc->AddRootLogicalVolume(scorer2_log);
    regionSc->AddRootLogicalVolume(scorer3_log);
    regionSc->AddRootLogicalVolume(scorer4_log);
    regionSc->AddRootLogicalVolume(scorer5_log);
    regionSc->AddRootLogicalVolume(scorer6_log);
    regionSc->AddRootLogicalVolume(scorer7_log);

    return fPvol_World;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetMaterialTarget(G4String material)
{
    // search the material by its name
    G4Material* pMaterial = G4Material::GetMaterial(material);
    
    if (pMaterial) fMaterial_Target = pMaterial;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetThicknessTarget(G4double val)
{
    fThickness_Target = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void DetectorConstruction::SetRadiusTarget(G4double val)
{
    fRadiusTarget = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void DetectorConstruction::SetParameterA(G4double val)
{
    fParameterA = val;
}

void DetectorConstruction::SetParameterB(G4double val)
{
    fParameterB = val;
}

void DetectorConstruction::SetParameterC(G4double val)
{
    fParameterC = val;
}

void DetectorConstruction::SetParameterD(G4double val)
{
    fParameterD = val;
}

void DetectorConstruction::SetParameterE(G4double val)
{
    fParameterE = val;
}

void DetectorConstruction::SetParameterF(G4double val)
{
    fParameterF = val;
}


////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

#include "G4RunManager.hh"

void DetectorConstruction::UpdateGeometry()
{
    G4RunManager::GetRunManager()->DefineWorldVolume(ConstructVolumes());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4Material* DetectorConstruction::GetMaterialTarget()        
{
	return fMaterial_Target;
}

G4double    DetectorConstruction::GetThicknessTarget()       
{
	return fThickness_Target;
}

G4double    DetectorConstruction::GetRadiusTarget()       
{
	return fRadiusTarget;
}


