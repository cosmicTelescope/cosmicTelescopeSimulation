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
/// \file B4DetectorConstruction.cc
/// \brief Implementation of the B4DetectorConstruction class

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
    //    parser.Read( "muonTelescope_v4.gdml");
    parser.Read( "muonNeutronTelescope_v5.gdml");
    G4cout << "Geometry loaded from  file .......muonNeutronTelescope.gdml " << G4endl;
    
    G4VPhysicalVolume* WorldLV = parser.GetWorldVolume();
    G4LogicalVolume* W_log = WorldLV->GetLogicalVolume();
    W_log->SetMaterial(defaultMaterial);

    LVScint0 = parser.GetVolume("panel0_EJ200");
    LVScint1 = parser.GetVolume("panel1_EJ200");
    LVScint2 = parser.GetVolume("panel2_EJ200");
    LVNeutronCell = parser.GetVolume("neutronCell_BC505");

    return WorldLV;
    
<<<<<<< HEAD
=======
    /*
    //     
    // World
    //
    auto worldS 
    = new G4Box("World",           // its name
    worldSizeX/2, worldSizeY/2, worldSizeZ/2); // its size
    
    auto worldLV
    = new G4LogicalVolume(
    worldS,           // its solid
    defaultMaterial,  // its material
    "World");         // its name
    
    auto physiWorld
    = new G4PVPlacement(
    0,                // no rotation
    G4ThreeVector(),  // at (0,0,0)
    worldLV,          // its logical volume                         
    "World",          // its name
    0,                // its mother  volume
    false,            // no boolean operation
    0,                // copy number
    fCheckOverlaps);  // checking overlaps     
    //                                 
    // Layer
    //
    auto scintLayer0 
      = new G4Box("scintLayer0",           // its name
		  scintX/2, scintY/2, scintZ/2); // its size
  
  auto scintLayerLV0
      = new G4LogicalVolume(
			    scintLayer0,                  // its solid
			    scintMaterial,              // its material
			    "scintLayerLV0");          // its name
  
  physiScint0 = new G4PVPlacement(0,			                                                //no rotation
				  G4ThreeVector(0.0, 0.0, -(scintZ/2. + dist12/2. + dist23/2.)),	//at (0,0,0)
				  "physiScintLayer0",		//its name
				  scintLayerLV0,		//its logical volume				 
				  physiWorld,			//its mother  volume
				  false,			//no boolean operation
				  0);			//copy number

 auto scintLayer1
      = new G4Box("scintLayer1",           // its name
		  scintX/2, scintY/2, scintZ/2); // its size
  
  auto scintLayerLV1
      = new G4LogicalVolume(
			    scintLayer1,                  // its solid
			    scintMaterial,              // its material
			    "scintLayerLV1");          // its name
  
  physiScint1 = new G4PVPlacement(0,			                                                //no rotation
				  G4ThreeVector(0.0, 0.0, -(scintZ/2. - dist12/2. + dist23/2.)),	//at (0,0,0)
				  "physiScintLayer1",		//its name
				  scintLayerLV1,		//its logical volume				 
				  physiWorld,			//its mother  volume
				  false,			//no boolean operation
				  0);			//copy number
    
 auto scintLayer2
      = new G4Box("scintLayer2",           // its name
		  scintX/2, scintY/2, scintZ/2); // its size
  
  auto scintLayerLV2
      = new G4LogicalVolume(
			    scintLayer2,                  // its solid
			    scintMaterial,              // its material
			    "scintLayerLV2");          // its name
  
  physiScint2 = new G4PVPlacement(0,			                                                //no rotation
				  G4ThreeVector(0.0, 0.0,  (scintZ/2. + dist12/2. + dist23/2.)),	//at (0,0,0)
				  "physiScintLayer2",		//its name
				  scintLayerLV2,		//its logical volume				 
				  physiWorld,			//its mother  volume
				  false,			//no boolean operation
				  0);			//copy number
    
  //
  // print parameters
  //
  G4cout
    << G4endl 
    << "------------------------------------------------------------" << G4endl
    << "---> Telescope has " << nofLayers << " layers  [ "
    << scintZ/cm << "cm of " << scintMaterial->GetName() 
    << "------------------------------------------------------------" << G4endl;
  
  //                                        
  // Visualization attributes
  //
  worldLV->SetVisAttributes (G4VisAttributes::GetInvisible());

  auto simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(true);
  scintLayerLV0->SetVisAttributes(simpleBoxVisAtt);
  scintLayerLV1->SetVisAttributes(simpleBoxVisAtt);
  scintLayerLV2->SetVisAttributes(simpleBoxVisAtt);

  //
  // Always return the physical World
  //
  return physiWorld;

  */
>>>>>>> ad172e105d29d48c9ff4eac0cdfecd28e0c739f4
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CTDetectorConstruction::ConstructSDandField()
{ 
  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
<<<<<<< HEAD
    G4ThreeVector fieldValue;
    fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
    fMagFieldMessenger->SetVerboseLevel(1);
    
    // Register the field messenger for deleting
    G4AutoDelete::Register(fMagFieldMessenger);
=======
  G4ThreeVector fieldValue;
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);
  
  // Register the field messenger for deleting
  G4AutoDelete::Register(fMagFieldMessenger);
>>>>>>> ad172e105d29d48c9ff4eac0cdfecd28e0c739f4
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
