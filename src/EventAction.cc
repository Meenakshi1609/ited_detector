#include "EventAction.hh"
#include "RunAction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

EventAction::EventAction()
  : fScatterEnergy(0.),
    fAbsorberEnergy(0.),
    fScatterPhotons(0),
    fAbsorberPhotons(0)
{}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event*)
{
    fScatterEnergy   = 0.;
    fAbsorberEnergy  = 0.;
    fScatterPhotons  = 0;
    fAbsorberPhotons = 0;
}

void EventAction::AddScatterEnergy(G4double edep)
{
    fScatterEnergy += edep;
}

void EventAction::AddAbsorberEnergy(G4double edep)
{
    fAbsorberEnergy += edep;
}

void EventAction::AddScatterPhoton()
{
    fScatterPhotons++;
}

void EventAction::AddAbsorberPhoton()
{
    fAbsorberPhotons++;
}

G4double EventAction::GetScatterEnergy() const { return fScatterEnergy; }
G4double EventAction::GetAbsorberEnergy() const { return fAbsorberEnergy; }
G4int    EventAction::GetScatterPhotons() const { return fScatterPhotons; }
G4int    EventAction::GetAbsorberPhotons() const { return fAbsorberPhotons; }

void EventAction::EndOfEventAction(const G4Event*)
{
     RunAction* runAction =
    static_cast<RunAction*>(
        const_cast<G4UserRunAction*>(
            G4RunManager::GetRunManager()->GetUserRunAction()));

runAction->WriteEvent(this);


    G4cout << "[Event] "
           << "E_scat = " << fScatterEnergy/keV << " keV, "
           << "E_abs = "  << fAbsorberEnergy/keV << " keV, "
           << "N_scat = " << fScatterPhotons << ", "
           << "N_abs = "  << fAbsorberPhotons
           << G4endl;
}
