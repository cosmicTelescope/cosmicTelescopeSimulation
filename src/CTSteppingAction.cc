// June 4, 2019: hexc, James and Nadia
// First implementation of cosmic ray telescope simulation based on B4a example.
//     The telescope consists of three layers of plastic scintillator
// 
/// \file B4aSteppingAction.cc
/// \brief Implementation of the B4aSteppingAction class

#include "CTSteppingAction.hh"
#include "CTEventAction.hh"
#include "CTDetectorConstruction.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CTSteppingAction::CTSteppingAction(
                      const CTDetectorConstruction* detectorConstruction,
                      CTEventAction* eventAction)
  : G4UserSteppingAction(),
    fDetConstruction(detectorConstruction),
    fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CTSteppingAction::~CTSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CTSteppingAction::UserSteppingAction(const G4Step* step)
{
    // Collect energy and track length step by step
    
    // get volume of the current step
    auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    
    // energy deposit
    auto edep = step->GetTotalEnergyDeposit();
    
    // step length
    G4double stepLength = 0.;
    if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) {
	stepLength = step->GetStepLength();
    }
    
    if ( volume == fDetConstruction->GetScintLayer0LV() ) {
	fEventAction->AddEloss_TrackLength(0, edep,stepLength);
    } else if (volume == fDetConstruction->GetScintLayer1LV()) {
	fEventAction->AddEloss_TrackLength(1, edep,stepLength);
    } else if (volume == fDetConstruction->GetScintLayer2LV()) {
	fEventAction->AddEloss_TrackLength(2, edep,stepLength);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
