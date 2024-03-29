// June 4, 2019: hexc, James and Nadia
// First implementation of cosmic ray telescope simulation based on B4a example.
//     The telescope consists of three layers of plastic scintillator
//
// June 5, 2019: hexc, James and Nadia
//     Add a test code to read in gdml detector description 
//
// June 27, 2019: hexc, James, Chi Chi and Emily
//     Use the 3D design as the default detector configuration
// 
// August 15,2019: hexc, James and Nadia
//     Set physiScint0, physiScint1 and physiScint2 member values 
//     based their gdml names.
// 


#include "CTDetectorConstruction.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4VisAttributes.hh"

#include "G4RegionStore.hh"
#include "G4GDMLParser.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal 
G4GlobalMagFieldMessenger* CTDetectorConstruction::fMagFieldMessenger = nullptr; 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CTDetectorConstruction::CTDetectorConstruction()
 : G4VUserDetectorConstruction(),
   physiScint0(nullptr), physiScint1(nullptr), physiScint2(nullptr),
   fCheckOverlaps(true)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CTDetectorConstruction::~CTDetectorConstruction()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* CTDetectorConstruction::Construct()
{
  // Define materials 
  DefineMaterials();
  
  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CTDetectorConstruction::DefineMaterials()
{ 
  // Lead material defined using NIST Manager
  auto nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_Pb");
 
  G4String symbol;  
 
  // Liquid argon material
  G4double a;  // mass of a mole;
  G4double z;  // z=mean number of protons;  
  G4double density, fractionmass; 
  G4int ncomponents, natoms;

  G4Element* H  = new G4Element("Hydrogen",symbol="H" , z= 1., a= 1.01*g/mole);
  G4Element* C  = new G4Element("Carbon"  ,symbol="C" , z= 6., a= 12.01*g/mole);
  G4Element* N  = new G4Element("Nitrogen",symbol="N" , z= 7., a= 14.01*g/mole);
  G4Element* O  = new G4Element("Oxygen"  ,symbol="O" , z= 8., a= 16.00*g/mole);

  new G4Material("liquidArgon", z=18., a= 39.95*g/mole, density= 1.390*g/cm3);
         // The argon by NIST Manager is a gas with a different density

  // Copied from pINT simulation
  G4Material* scintMat = new G4Material("scintillator", density= 1.032*g/cm3, ncomponents=2);
  scintMat->AddElement(C, natoms=10);
  scintMat->AddElement(H, natoms=11);

 
  G4Material* Air = new G4Material("Air", density= 1.290*mg/cm3, ncomponents=2);
  Air->AddElement(N, fractionmass=0.7);
  Air->AddElement(O, fractionmass=0.3);


  // Vacuum
  new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                  kStateGas, 2.73*kelvin, 3.e-18*pascal);

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* CTDetectorConstruction::DefineVolumes()
{
    // Define world volume
    worldSizeX  = 20.0*cm;
    worldSizeY = worldSizeX;
    worldSizeZ  = 50.0*cm; 
    
    // Geometry parameters
    nofLayers = 3;
    
    // Define scintillator dimension
    scintX  = scintY = 20.*cm;
    scintZ = 1.0*cm;
    
    dist12 = 10.0*cm;
    dist23 = 20.0*cm;
    
    // Get materials
    auto defaultMaterial = G4Material::GetMaterial("Air");
    auto scintMaterial = G4Material::GetMaterial("scintillator");
    //    auto gapMaterial = G4Material::GetMaterial("liquidArgon");
    
    if ( ! defaultMaterial || ! scintMaterial) {
	G4ExceptionDescription msg;
	msg << "Cannot retrieve materials already defined."; 
	G4Exception("CTDetectorConstruction::DefineVolumes()",
		    "MyCode0001", FatalException, msg);
    }  
    
    
    // Read in gdml detector description
    G4GDMLParser parser;
    //    parser.Read( "muonNeutronTelescope.gdml" );
    //    parser.Read( "MuonDetectorTelescope_v1.gdml" );
    //    parser.Read( "FullAssemblyNeutronCell.gdml" );
    //    parser.Read( "MuonDetectorTelescope_v2.gdml");
    parser.Read( "muonTelescope_v4.gdml");
    //    parser.Read( "muonNeutronTelescope_v5.gdml");
    G4cout << "Geometry loaded from  file .......muonNeutronTelescope.gdml " << G4endl;
    
    G4VPhysicalVolume* WorldLV = parser.GetWorldVolume();
    G4LogicalVolume* W_log = WorldLV->GetLogicalVolume();
    W_log->SetMaterial(defaultMaterial);

    LVScint0 = parser.GetVolume("panel0_EJ200");
    LVScint1 = parser.GetVolume("panel1_EJ200");
    LVScint2 = parser.GetVolume("panel2_EJ200");
    //    LVNeutronCell = parser.GetVolume("neutronCell_BC505");

    return WorldLV;
    

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CTDetectorConstruction::ConstructSDandField()
{ 

    G4ThreeVector fieldValue;
    fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
    fMagFieldMessenger->SetVerboseLevel(1);
    
    // Register the field messenger for deleting
    G4AutoDelete::Register(fMagFieldMessenger);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
