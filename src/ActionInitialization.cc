#include "ActionInitialization.hh"

#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

ActionInitialization::ActionInitialization() {}
ActionInitialization::~ActionInitialization() {}


void ActionInitialization::BuildForMaster() const
{
    SetUserAction(new RunAction());
}


void ActionInitialization::Build() const
{
    // Primary generator 
    SetUserAction(new PrimaryGeneratorAction());

    // Run action
    auto runAction = new RunAction();
    SetUserAction(runAction);

    // Event action
    auto eventAction = new EventAction();
    SetUserAction(eventAction);

    // Stepping action
    SetUserAction(new SteppingAction(eventAction));
}
