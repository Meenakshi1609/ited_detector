#include "SteppingAction.hh"
#include "EventAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"

SteppingAction::SteppingAction(EventAction* eventAction)
  : fEventAction(eventAction)
{}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    G4cout << ">>> SteppingAction called <<<" << G4endl;

    const G4Track* track = step->GetTrack();

    // -------------------------------------------------
    // 1) ENERGY DEPOSITION (charged particles)
    // -------------------------------------------------
    G4double edep = step->GetTotalEnergyDeposit();

    if (edep > 0.) {
        auto volume =
          step->GetPreStepPoint()->GetTouchableHandle()
              ->GetVolume()->GetLogicalVolume();

        if (volume->GetName() == "Scatterer") {
            fEventAction->AddScatterEnergy(edep);
        }
        else if (volume->GetName() == "AbsorberCrystal") {
            fEventAction->AddAbsorberEnergy(edep);
        }
    }

    // -------------------------------------------------
    // 2) SCINTILLATION PHOTONS
    // -------------------------------------------------
    if (track->GetDefinition() != G4OpticalPhoton::Definition()) return;

    // Count each photon only once
    if (track->GetCurrentStepNumber() != 1) return;

    const G4VProcess* creator = track->GetCreatorProcess();
    if (!creator) return;

    if (creator->GetProcessName() != "Scintillation") return;

    auto volume =
      step->GetPreStepPoint()->GetTouchableHandle()
          ->GetVolume()->GetLogicalVolume();

    if (volume->GetName() == "Scatterer") {
        fEventAction->AddScatterPhoton();
    }
    else if (volume->GetName() == "AbsorberCrystal") {
        fEventAction->AddAbsorberPhoton();
    }

    // Kill photon (no PMTs yet)
    step->GetTrack()->SetTrackStatus(fStopAndKill);
}
