// June 4, 2019: hexc, James and Nadia
// First implementation of cosmic ray telescope simulation based on B4a example.
//     The telescope consists of three layers of plastic scintillator
//
/// \file B4aActionInitialization.hh
/// \brief Definition of the B4aActionInitialization class

#ifndef CTActionInitialization_h
#define CTActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class CTDetectorConstruction;

/// Action initialization class.
///

class CTActionInitialization : public G4VUserActionInitialization
{
  public:
    CTActionInitialization(CTDetectorConstruction*);
    virtual ~CTActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

  private:
    CTDetectorConstruction* fDetConstruction;
};

#endif

    
