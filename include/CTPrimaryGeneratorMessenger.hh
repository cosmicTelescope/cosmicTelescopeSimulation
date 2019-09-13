// This is the primary generator messenger class for cosmic ray telescope simulation
// Created on August 8/21/2019: hexc, James and Nadia
//    As a starter, we want to implement an option for selecting different event generator types
//        ev_type = 0     for arbitrary single particle
//        ev_type = 1     cosmic ray muons
//        ev_type = 2     cosmic ray neutrons   


#ifndef CTPrimaryGeneratorMessenger_h
#define CTPrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class CTPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class CTPrimaryGeneratorMessenger: public G4UImessenger
{
public:
  CTPrimaryGeneratorMessenger(CTPrimaryGeneratorAction*);
  ~CTPrimaryGeneratorMessenger();
  
  void SetNewValue(G4UIcommand*, G4String);
  
private:
  CTPrimaryGeneratorAction* CTAction;
  G4UIdirectory*               gunDir; 
  G4UIcmdWithAnInteger*        newEventType;
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
