#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

int main(int argc, char** argv)
{
    
    // MT run manager (but force 1 thread)
 
    auto* runManager =
        G4RunManagerFactory::CreateRunManager(
            G4RunManagerType::Default);

    runManager->SetNumberOfThreads(1);   // 

    // Mandatory initializations
    
    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new PhysicsList());
    runManager->SetUserInitialization(new ActionInitialization());

    runManager->Initialize();

    
    // Visualization
    auto* visManager = new G4VisExecutive();
    visManager->Initialize();

    //auto* UImanager = G4UImanager::GetUIpointer();

    auto* UImanager = G4UImanager::GetUIpointer();

    // UImanager->ApplyCommand("/run/initialize");
    // UImanager->ApplyCommand("/run/numberOfThreads 1");
    // UImanager->ApplyCommand("/run/beamOn 1");

    if (argc == 1) {
        // Interactive + visualization
        auto* ui = new G4UIExecutive(argc, argv);

        UImanager->ApplyCommand("/control/macroPath ../macros");
        UImanager->ApplyCommand("/control/execute vis.mac");

        ui->SessionStart();
        delete ui;
    }
    else {
        // Batch mode (for data taking)
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    }

    delete visManager;
    delete runManager;
    return 0;
}
