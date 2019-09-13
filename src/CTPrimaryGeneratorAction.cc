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
#include "CTAnalysis.hh"

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

    G4double xPos, yPos, zPos, xVec, yVec, zVec, sigmaAngle, theta, phi, momentum, sigmaMomentum, mass, pp, Ekin;
    G4int pid;
    
    // Set up cosmic ray events
    if (ev_type == 0) {        // single particle simulation
	pid = 0;   // tmp pID
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,-1.,0.));
	//   fParticleGun->SetParticleEnergy(1.*GeV);
	xPos = (G4UniformRand()-0.5)*30.0*cm;
	zPos = (G4UniformRand()-0.5)*30.0*cm;
	yPos = 25.0*cm;
	fParticleGun->SetParticlePosition(G4ThreeVector(xPos, yPos, zPos)); 
	
	sigmaAngle = 140.*deg;
	theta = (G4UniformRand()-0.5)*sigmaAngle;
	phi = G4UniformRand()*360.*deg;
	xVec = std::sin(phi)*std::sin(theta);
	yVec = -std::cos(theta);
	zVec = std::cos(phi)*std::sin(theta);
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(xVec, yVec, zVec));
	
	fParticleGun->GeneratePrimaryVertex(anEvent);
	
    }  else if (ev_type == 1) {    // cosmic ray muon flux event
	pid = 1;   // tmp pID
	particle = particleTable->FindParticle("mu-");
	fParticleGun->SetParticleDefinition(particle);
	
	//fParticleGun->SetParticleTime(0.0*ns);
	//fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
	
	momentum = 300.*MeV;
	sigmaMomentum = 10000.*MeV;
	
	mass = particle->GetPDGMass();
	
	sigmaAngle = 140.*deg;
	
	G4int npart = 1;
	for (G4int i=1; i<=npart; i++) {
	    pp = momentum + G4UniformRand()*sigmaMomentum;  // 0 GeV < pp << 10.3 GeV    
	    Ekin = std::sqrt(pp*pp+mass*mass) - mass;
	    fParticleGun->SetParticleEnergy(Ekin);
	    xPos = (G4UniformRand()-0.5)*30.0*cm;
	    zPos = (G4UniformRand()-0.5)*30.0*cm;
	    yPos = 25.0*cm;
	    fParticleGun->SetParticlePosition(G4ThreeVector(xPos, yPos, zPos)); 
	    
	    theta = (G4UniformRand()-0.5)*sigmaAngle;
	    phi = G4UniformRand()*360.*deg;
	    xVec = std::sin(phi)*std::sin(theta);
	    yVec = -std::cos(theta);
	    zVec = std::cos(phi)*std::sin(theta);
	    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(xVec, yVec, zVec));
	    
	    fParticleGun->GeneratePrimaryVertex(anEvent);
	}
    } else if (ev_type == 2)  // cosmic ray neutron event
	{
	    pid = 2;   // tmp pID
	    particle = particleTable->FindParticle("neutron");
	    fParticleGun->SetParticleDefinition(particle);
	    
	    momentum = 1.*MeV;
	    sigmaMomentum = 100.*MeV;
	    
	    mass = particle->GetPDGMass();
	    
	    sigmaAngle = 140.*deg;
	    
	    G4int npart = 1;
	    for (G4int i=1; i<=npart; i++) {
		pp = momentum + G4UniformRand()*sigmaMomentum;  // 0 GeV < pp << 10.3 GeV    
		Ekin = std::sqrt(pp*pp+mass*mass) - mass;
		fParticleGun->SetParticleEnergy(Ekin);
		xPos = (G4UniformRand()-0.5)*30.0*cm;
		zPos = (G4UniformRand()-0.5)*30.0*cm;
		yPos = 25.0*cm;
		fParticleGun->SetParticlePosition(G4ThreeVector(xPos, yPos, zPos)); 
		
		theta = (G4UniformRand()-0.5)*sigmaAngle;
		phi = G4UniformRand()*360.*deg;
		xVec = std::sin(phi)*std::sin(theta);
		yVec = -std::cos(theta);
		zVec = std::cos(phi)*std::sin(theta);
		fParticleGun->SetParticleMomentumDirection(G4ThreeVector(xVec, yVec, zVec));
		
		fParticleGun->GeneratePrimaryVertex(anEvent);
	    }
	} else {
	G4cout << "Dummy! you need to pick a proper event type. Choices are: 0, 1 or 2 for now." << G4endl;
    }

    // get analysis manager
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(8, pid);
    analysisManager->FillNtupleDColumn(9, Ekin);
    analysisManager->FillNtupleDColumn(10, xPos);
    analysisManager->FillNtupleDColumn(11, yPos);
    analysisManager->FillNtupleDColumn(12, zPos);
    analysisManager->FillNtupleDColumn(13, xVec);
    analysisManager->FillNtupleDColumn(14, yVec);
    analysisManager->FillNtupleDColumn(15, zVec);
    
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......	
