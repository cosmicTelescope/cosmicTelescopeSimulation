// June 4, 2019: hexc, James and Nadia
// First implementation of cosmic ray telescope simulation based on B4a example.
//     The telescope consists of three layers of plastic scintillator
// August 15, 2019: hexc, James and Nadia
//     Added logical volume name for the panels
//
//
/// \file B4DetectorConstruction.hh
/// \brief Definition of the B4DetectorConstruction class

#ifndef CTDetectorConstruction_h
#define CTDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4GlobalMagFieldMessenger;

/// Detector construction class to define materials and geometry.
/// The calorimeter is a box made of a given number of layers. A layer consists
/// of an absorber plate and of a detection gap. The layer is replicated.
///
/// Four parameters define the geometry of the calorimeter :
///
/// - the thickness of an absorber plate,
/// - the thickness of a gap,
/// - the number of layers,
/// - the transverse size of the calorimeter (the input face is a square).
///
/// In addition a transverse uniform magnetic field is defined 
/// via G4GlobalMagFieldMessenger class.

class CTDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    CTDetectorConstruction();
    virtual ~CTDetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    // get methods
    //
    const G4VPhysicalVolume* GetScintLayer0PV() const;
    const G4VPhysicalVolume* GetScintLayer1PV() const;
    const G4VPhysicalVolume* GetScintLayer2PV() const;
    const G4VPhysicalVolume* GetNeutronCellPV() const;

    const G4LogicalVolume* GetScintLayer0LV() const;
    const G4LogicalVolume* GetScintLayer1LV() const;
    const G4LogicalVolume* GetScintLayer2LV() const;
    const G4LogicalVolume* GetNeutronCellLV() const;
     
  private:
    // methods
    //
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();
  
    // data members
    //
    static G4ThreadLocal G4GlobalMagFieldMessenger*  fMagFieldMessenger; 
                                      // magnetic field messenger
     
    //    G4VPhysicalVolume*   fAbsorberPV; // the absorber physical volume
    
    G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps
    
    G4double           worldSizeX, worldSizeY, worldSizeZ;
    G4double           dist12, dist23;
    G4double           scintX, scintY, scintZ;
    G4int                 nofLayers;

    G4VPhysicalVolume  *physiScint0, *physiScint1, *physiScint2, *physiNeutronCell;
    G4LogicalVolume  *LVScint0, *LVScint1, *LVScint2, *LVNeutronCell;

};

// inline functions

inline const G4VPhysicalVolume* CTDetectorConstruction::GetScintLayer0PV() const { 
  return physiScint0; 
}

inline const G4VPhysicalVolume* CTDetectorConstruction::GetScintLayer1PV() const { 
  return physiScint1; 
}

inline const G4VPhysicalVolume* CTDetectorConstruction::GetScintLayer2PV() const { 
  return physiScint2; 
}

inline const G4VPhysicalVolume* CTDetectorConstruction::GetNeutronCellPV() const { 
  return physiNeutronCell; 
}

inline const G4LogicalVolume* CTDetectorConstruction::GetScintLayer0LV() const { 
  return LVScint0; 
}

inline const G4LogicalVolume* CTDetectorConstruction::GetScintLayer1LV() const { 
  return LVScint1; 
}

inline const G4LogicalVolume* CTDetectorConstruction::GetScintLayer2LV() const { 
  return LVScint2; 
}

inline const G4LogicalVolume* CTDetectorConstruction::GetNeutronCellLV() const { 
  return LVNeutronCell; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

