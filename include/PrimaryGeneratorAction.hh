#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"

class G4Event;

// ------------------------------------------------------------
// PrimaryGeneratorAction
// Defines the primary particles (gamma source)
// ------------------------------------------------------------
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction() override;

    virtual void GeneratePrimaries(G4Event* event) override;

  private:
    G4ParticleGun* fParticleGun;
};

#endif
