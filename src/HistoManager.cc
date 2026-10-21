#include "HistoManager.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
: fFileName("Fluence")
{
    Book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
  // delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book()
{
    // Create or get analysis manager
    // The choice of analysis technology is done via selection of a namespace
    // in HistoManager.hh
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetFileName(fFileName);
    analysisManager->SetActivation(true);
    analysisManager->SetVerboseLevel(0);
    analysisManager->SetDefaultFileType("root");
    
    // Define histograms start values*/
    const G4int nTuples = 8;
    
    const G4String name[] =
    {
        "Scorer_0_deg",
        "Scorer_1_deg",
        "Scorer_2_deg",
        "Scorer_4_deg",
        "Scorer_10_deg",
        "Scorer_30_deg",
        "Scorer_60_deg",
        "Scorer_90_deg"
    };

    G4int ih; 
 
    for ( int i = 0; i < nTuples; i++ ) {
	 ih = analysisManager->CreateH1(name[i], name[i], 100, 0.4, 114.0);
	 analysisManager->SetH1Activation(ih, false);
    }
    /*analysisManager->CreateNtuple("fluence", "Spatial fluence");

    analysisManager->CreateNtupleDColumn("x");
    analysisManager->CreateNtupleDColumn("y");
    analysisManager->CreateNtupleDColumn("z");
    analysisManager->CreateNtupleDColumn("ekin"); 
    analysisManager->CreateNtupleDColumn("tracklength");
    analysisManager->CreateNtupleDColumn("weight");

    analysisManager->FinishNtuple();*/


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
