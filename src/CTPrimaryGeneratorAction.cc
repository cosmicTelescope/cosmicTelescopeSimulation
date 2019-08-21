// June 4, 2019: hexc, James and Nadia
//     First implementation of cosmic ray telescope simulation based on B4a example.
//     The telescope consists of three layers of plastic scintillator
// June 5, 2019
//     Try out particle gun properties
// August 20, 2019: hexc, James and Nadia
//     Update the primary generator to simulation "real" cosmic ray flux
/// 
/// \file B4PrimaryGeneratorAction.cc
/// \brief Implementation of the B4PrimaryGeneratorAction class

#include "CTPrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CTPrimaryGeneratorAction::CTPrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(nullptr)
{
    // set event type
    ev_type = 1;

    G4int nofParticles = 1;
    fParticleGun = new G4ParticleGun(nofParticles);
    
  
    //create a messenger for this class
    gunMessenger = new CTPrimaryGeneratorMessenger(this);

    // default particle kinematic
    //
    auto particleDefinition 
	= G4ParticleTable::GetParticleTable()->FindParticle("mu-");
    fParticleGun->SetParticleDefinition(particleDefinition);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,1.,0.));
    fParticleGun->SetParticleEnergy(50.*MeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CTPrimaryGeneratorAction::~CTPrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CTPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    // This function is called at the begining of event
    G4ParticleDefinition* particle;

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    //particleTable ->DumpTable("ALL");
    
    // In order to avoid dependence of PrimaryGeneratorAction
    // on DetectorConstruction class we get world volume 
    // from G4LogicalVolumeStore
    //
    G4double worldZHalfLength = 0.;
    auto worldLV = G4LogicalVolumeStore::GetInstance()->GetVolume("world_volume");
    
    // Check that the world volume has box shape
    G4Box* worldBox = nullptr;
    if (  worldLV ) {
	G4cout << "We found the world! " << G4endl;
	worldBox = dynamic_cast<G4Box*>(worldLV->GetSolid());
    }
    
    if ( worldBox ) {
	worldZHalfLength = worldBox->GetZHalfLength();  
	G4cout << " worldZHalfLength = " << worldZHalfLength << G4endl;
    }
    else  {
	G4ExceptionDescription msg;
	msg << "World volume of box shape not found." << G4endl;
	msg << "Perhaps you have changed geometry." << G4endl;
	msg << "The gun will be place in the center.";
	G4Exception("CTPrimaryGeneratorAction::GeneratePrimaries()",
		    "MyCode0002", JustWarning, msg);
    } 
    
    // Set up cosmic ray events
    if (ev_type == 0) {
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,-1.,0.));
	fParticleGun->SetParticleEnergy(1.*GeV);
	fParticleGun->SetParticlePosition(G4ThreeVector( (G4UniformRand()-0.5)*30.0*cm, 
							 25.0*cm,
							 (G4UniformRand()-0.5)*30.0*cm) );
	G4double sigmaAngle = 140.*deg;
		
	G4double theta = (G4UniformRand()-0.5)*sigmaAngle;
	G4double phi = G4UniformRand()*360.*deg;
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(std::sin(phi)*std::sin(theta), -std::cos(theta), std::cos(phi)*std::sin(theta)));
	
	fParticleGun->GeneratePrimaryVertex(anEvent);

    }  else if (ev_type == 1) {                  
	particle = particleTable->FindParticle("mu-");
	fParticleGun->SetParticleDefinition(particle);
	
	//fParticleGun->SetParticleTime(0.0*ns);
	//fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
	
	G4double momentum = 300.*MeV;
	G4double sigmaMomentum = 10000.*MeV;
	
	G4double mass = particle->GetPDGMass();
	
	G4double sigmaAngle = 140.*deg;
	
	G4int npart = 10;
	for (G4int i=1; i<=npart; i++) {
	    G4double pp = momentum + G4UniformRand()*sigmaMomentum;  // 0 GeV < pp << 10.3 GeV    
	    G4double Ekin = std::sqrt(pp*pp+mass*mass) - mass;
	    fParticleGun->SetParticleEnergy(Ekin);
	    fParticleGun->SetParticlePosition(G4ThreeVector( (G4UniformRand()-0.5)*30.0*cm, 
							     25.0*cm,
							     (G4UniformRand()-0.5)*30.0*cm) );
	        
	    G4double theta = (G4UniformRand()-0.5)*sigmaAngle;
	    G4double phi = G4UniformRand()*360.*deg;
	    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(std::sin(phi)*std::sin(theta), -std::cos(theta), std::cos(phi)*std::sin(theta)));
	    
	    fParticleGun->GeneratePrimaryVertex(anEvent);
	}
    } else if (ev_type == 2) 
	{
	    particle = particleTable->FindParticle("neutron");
	    fParticleGun->SetParticleDefinition(particle);
	    
	    G4double momentum = 1.*MeV;
	    G4double sigmaMomentum = 100.*MeV;
	    
	    G4double mass = particle->GetPDGMass();
	    
	    G4double sigmaAngle = 140.*deg;
	    
	    G4int npart = 10;
	    for (G4int i=1; i<=npart; i++) {
		G4double pp = momentum + G4UniformRand()*sigmaMomentum;  // 0 GeV < pp << 10.3 GeV    
		G4double Ekin = std::sqrt(pp*pp+mass*mass) - mass;
		fParticleGun->SetParticleEnergy(Ekin);
		fParticleGun->SetParticlePosition(G4ThreeVector( (G4UniformRand()-0.5)*30.0*cm, 
								 25.0*cm,
								 (G4UniformRand()-0.5)*30.0*cm) );
	        
		G4double theta = (G4UniformRand()-0.5)*sigmaAngle;
		G4double phi = G4UniformRand()*360.*deg;
		fParticleGun->SetParticleMomentumDirection(G4ThreeVector(std::sin(phi)*std::sin(theta), -std::cos(theta), std::cos(phi)*std::sin(theta)));
		
		fParticleGun->GeneratePrimaryVertex(anEvent);
	    }
	} else {
	G4cout << "Dummy! you need to pick a proper event type. Choices are: 0, 1 or 2 for now." << G4endl;
    }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......	
