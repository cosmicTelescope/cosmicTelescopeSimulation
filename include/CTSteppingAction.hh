// June 4, 2019: hexc, James and Nadia
// First implementation of cosmic ray telescope simulation based on B4a example.
//     The telescope consists of three layers of plastic scintillator
// 
/// \file B4aSteppingAction.hh
/// \brief Definition of the B4aSteppingAction class

#ifndef CTSteppingAction_h
#define CTSteppingAction_h 1

#include "G4UserSteppingAction.hh"

class CTDetectorConstruction;
class CTEventAction;

/// Stepping action class.
///
/// In UserSteppingAction() there are collected the energy deposit and track 
/// lengths of charged particles in Absober and Gap layers and
/// updated in B4aEventAction.

class CTSteppingAction : public G4UserSteppingAction
{
public:
  CTSteppingAction(const CTDetectorConstruction* detectorConstruction,
                    CTEventAction* eventAction);
  virtual ~CTSteppingAction();

  virtual void UserSteppingAction(const G4Step* step);
    
private:
  const CTDetectorConstruction* fDetConstruction;
  CTEventAction*  fEventAction;  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
