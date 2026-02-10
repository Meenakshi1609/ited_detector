#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class EventAction : public G4UserEventAction
{
  public:
    EventAction();
    ~EventAction() override;

    void BeginOfEventAction(const G4Event*) override;
    void EndOfEventAction(const G4Event*) override;

    // Energy deposition
    void AddScatterEnergy(G4double edep);
    void AddAbsorberEnergy(G4double edep);

    // Photon counts
    void AddScatterPhoton();
    void AddAbsorberPhoton();

    // Getters
    G4double GetScatterEnergy() const;
    G4double GetAbsorberEnergy() const;
    G4int    GetScatterPhotons() const;
    G4int    GetAbsorberPhotons() const;

  private:
    G4double fScatterEnergy;
    G4double fAbsorberEnergy;
    G4int    fScatterPhotons;
    G4int    fAbsorberPhotons;
};

#endif
