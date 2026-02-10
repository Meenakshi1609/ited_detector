#include "PhysicsList.hh"

#include "G4EmStandardPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4OpticalPhysics.hh"

PhysicsList::PhysicsList()
{
    // Electromagnetic physics
    RegisterPhysics(new G4EmStandardPhysics());


    RegisterPhysics(new G4DecayPhysics());

    // Optical physics (scintillation, absorption, boundary)
    RegisterPhysics(new G4OpticalPhysics());

    SetVerboseLevel(1);
}
PhysicsList::~PhysicsList() {}
