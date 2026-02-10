#include "RunAction.hh"
#include "EventAction.hh"

#include "G4Run.hh"
#include "G4Threading.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction() : fEventID(0) {}

RunAction::~RunAction()
{
    if (fCSV.is_open()) fCSV.close();
}

void RunAction::BeginOfRunAction(const G4Run*)
{
    if (G4Threading::IsMasterThread()) {
        fCSV.open("scintillation_results.csv");
        fCSV << "event,E_scat_keV,E_abs_keV,"
             << "N_scat_photons,N_abs_photons\n";
    }
}

void RunAction::EndOfRunAction(const G4Run*)
{
    if (G4Threading::IsMasterThread()) {
        fCSV.close();
        G4cout << "CSV written: scintillation_results.csv" << G4endl;
    }
}

void RunAction::WriteEvent(const EventAction* ev)
{
    if (!G4Threading::IsMasterThread()) return;

    fCSV << fEventID++ << ","
         << ev->GetScatterEnergy()/keV << ","
         << ev->GetAbsorberEnergy()/keV << ","
         << ev->GetScatterPhotons() << ","
         << ev->GetAbsorberPhotons() << "\n";
}
