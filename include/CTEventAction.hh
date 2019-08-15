// June 4, 2019: hexc, James and Nadia
// First implementation of cosmic ray telescope simulation based on B4a example.
//     The telescope consists of three layers of plastic scintillator
// June 6, 2019: hexc, James, Nadia & Chi-Chi
//     Modify the code to record energy deposition in the scintillator tiles.
//     Record energy loss and track length for each scintillator layer in an array

/// \file B4aEventAction.hh
/// \brief Definition of the B4aEventAction class

#ifndef CTEventAction_h
#define CTEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#define nLayer 3          // Number of scintillator layers

/// Event action class
///

class CTEventAction : public G4UserEventAction
{
  public:
    CTEventAction();
    virtual ~CTEventAction();

    virtual void  BeginOfEventAction(const G4Event* event);
    virtual void    EndOfEventAction(const G4Event* event);
    
    void AddEloss_TrackLength(G4int n, G4double de, G4double dl);
    
  private:
    G4double  fEnergyLayer[nLayer];  
    G4double  fTrackLayer[nLayer]; 
};

// inline functions

inline void CTEventAction::AddEloss_TrackLength(G4int layer, G4double de, G4double dl) {
  fEnergyLayer[layer] += de; 
  fTrackLayer[layer] += dl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
