// June 4, 2019: hexc, James and Nadia
// First implementation of cosmic ray telescope simulation based on B4a example.
//     The telescope consists of three layers of plastic scintillator

/// \file B4aActionInitialization.cc
/// \brief Implementation of the B4aActionInitialization class

#include "CTActionInitialization.hh"
#include "CTPrimaryGeneratorAction.hh"
#include "CTRunAction.hh"
#include "CTEventAction.hh"
#include "CTSteppingAction.hh"
#include "CTDetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CTActionInitialization::CTActionInitialization
                            (CTDetectorConstruction* detConstruction)
 : G4VUserActionInitialization(),
   fDetConstruction(detConstruction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CTActionInitialization::~CTActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CTActionInitialization::BuildForMaster() const
{
  SetUserAction(new CTRunAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CTActionInitialization::Build() const
{
  SetUserAction(new CTPrimaryGeneratorAction);
  SetUserAction(new CTRunAction);
  auto eventAction = new CTEventAction;
  SetUserAction(eventAction);
  SetUserAction(new CTSteppingAction(fDetConstruction,eventAction));
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
