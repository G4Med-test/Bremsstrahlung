#include "SteppingAction.hh"
#include "Run.hh"
#include "HistoManager.hh"

#include "G4SteppingManager.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

#include "G4RegionStore.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction()
:G4UserSteppingAction()
{
	fScoringRegion = G4RegionStore::GetInstance()->FindOrCreateRegion("scorer");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    
    if( step->GetTrack()->GetVolume()->GetLogicalVolume()->GetRegion() == fScoringRegion ) {
	G4double ekin = step->GetTrack()->GetKineticEnergy();
	if ( step->GetTrack()->GetParticleDefinition()->GetPDGEncoding() == 22) { 

 	    G4StepPoint* prePoint = step->GetPreStepPoint();
			
	    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

	    const G4TouchableHandle touchable = prePoint->GetTouchableHandle();
	    G4int copyID = touchable->GetCopyNumber();

	    G4double weight = prePoint->GetWeight();
	    analysisManager->FillH1(copyID, ekin, weight);
	    step->GetTrack()->SetTrackStatus(fStopAndKill);
	}
    } else {
        return;
    }
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


