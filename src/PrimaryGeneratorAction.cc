#include "PrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    // One primary particle per event
    fParticleGun = new G4ParticleGun(1);

    // ---------------------------------
    // Particle definition: gamma
    // ---------------------------------
    auto particleDefinition =
        G4ParticleTable::GetParticleTable()->FindParticle("gamma");

    fParticleGun->SetParticleDefinition(particleDefinition);


    // Energy
    fParticleGun->SetParticleEnergy(1.0 * MeV);

    
    // Position: inside water phantom
    // (phantom is 20 cm long along Z)
    
    fParticleGun->SetParticlePosition(
        G4ThreeVector(0., 0., 0.0 * cm));
    // Direction: along +Z
    
    fParticleGun->SetParticleMomentumDirection(
        G4ThreeVector(0., 0., 1.));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    fParticleGun->GeneratePrimaryVertex(event);
}
