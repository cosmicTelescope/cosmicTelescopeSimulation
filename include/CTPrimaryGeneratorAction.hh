// June 4, 2019: hexc, James and Nadia
//     First implementation of cosmic ray telescope simulation based on B4a example.
//     The telescope consists of three layers of plastic scintillator
// August 20, 2019: hexc, James, and Nadia
//     Add generator private members
// 
/// \
/// \file B4PrimaryGeneratorAction.hh
/// \brief Definition of the B4PrimaryGeneratorAction class

#ifndef CTPrimaryGeneratorAction_h
#define CTPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "CTPrimaryGeneratorMessenger.hh"

#include "globals.hh"

class G4ParticleGun;
class G4Event;

/// The primary generator action class with particle gum.
///
/// It defines a single particle which hits the calorimeter 
/// perpendicular to the input face. The type of the particle
/// can be changed via the G4 build-in commands of G4ParticleGun class 
/// (see the macros provided with this example).

class CTPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    CTPrimaryGeneratorAction();    
    virtual ~CTPrimaryGeneratorAction();
    
    virtual void GeneratePrimaries(G4Event* event);
    
    // set methods
    void SetRandomFlag(G4bool value);
    void SetEventType(G4int eventType) { ev_type = eventType; };
    
private:
    G4ParticleGun*  fParticleGun; // G4 particle gun
    CTPrimaryGeneratorMessenger* gunMessenger;
    G4int ev_type, evtID;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
