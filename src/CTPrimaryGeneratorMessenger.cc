// This is the primary generator messenger class for cosmic ray telescope simulation
// Created on August 8/21/2019: hexc, James and Nadia
//    As a starter, we want to implement an option for selecting different event generator types
//        ev_type = 0     for arbitrary single particle
//        ev_type = 1     cosmic ray muons
//        ev_type = 2     cosmic ray neutrons          
// 
#include "globals.hh"

#include "CTPrimaryGeneratorMessenger.hh"

#include "CTPrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"

#include "CLHEP/Units/SystemOfUnits.h"

using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CTPrimaryGeneratorMessenger::CTPrimaryGeneratorMessenger(
                                          CTPrimaryGeneratorAction* CTGun)
:CTAction(CTGun)
{
  gunDir = new G4UIdirectory("/CT/gun/");
  gunDir->SetGuidance("PrimaryGenerator event type control");
   
  // New event type message
  newEventType = new G4UIcmdWithAnInteger("/CT/gun/newEventType", this);
  newEventType->SetGuidance("Set new event type");
  newEventType->SetGuidance("  Choice : 0, 1, or 2");
  newEventType->SetParameterName("ev_type", true);
  newEventType->SetDefaultValue(0);
  newEventType->AvailableForStates(G4State_PreInit, G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CTPrimaryGeneratorMessenger::~CTPrimaryGeneratorMessenger()
{
  delete newEventType;
  delete gunDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CTPrimaryGeneratorMessenger::SetNewValue(
                                        G4UIcommand* command, G4String newValue)
{ 
    if (command == newEventType ) {
	G4int eventType = newEventType->GetNewIntValue(newValue);
	CTAction->SetEventType(eventType);
    }
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

