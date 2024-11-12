#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "Randomize.hh"
#include "G4PhysListFactory.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"


#include "PPSDetectorConstruction.hh"
#include "PPSPhysicsList.hh"
#include "PPSPrimaryGeneratorAction.hh"
#include "PPSRunAction.hh"
#include "PPSEventAction.hh"
#include "PPSSteppingAction.hh"
//#include "PPSOutput.hh"
#include "PPSInput.hh"

////#include "TPluginManager.h"
////PPSInput  input;
////PPSOutput rootOutput;
///

PPSInput  input;
 
int main(int argc,char** argv) {
  
  /**
     USAGE: main opt.in seed
     where opt.in is an option file (mandatory)
     where seed is a optional seed number (int), default is 12345
   **/

  // to have the possibility to change seed 
  G4long myseed = 12345;
  if( argc==3 ){ myseed = (G4long)atoi(argv[2]); }
  CLHEP::HepRandom::setTheSeed(myseed);
  // Construct the default run manager
  G4RunManager * runManager = new G4RunManager;
  
  // set mandatory initialization classes
  PPSDetectorConstruction* det;
  PPSPrimaryGeneratorAction* prim;
  runManager->SetUserInitialization(det = new PPSDetectorConstruction);
  //runManager->SetUserInitialization(new PPSPhysicsList);

  // Esto lo puse yo: seteamos la misma PhysList que el colega
  const G4String plName = "FTFP_BERT";
  G4PhysListFactory plFactory;
  G4VModularPhysicsList *pl = plFactory.GetReferencePhysList( plName );
  // Default Cut value = 0.7mm
  G4double cutValue = 1.0 * CLHEP::um;
  pl->SetDefaultCutValue(cutValue);
  // Cuidado porque deben estar comentados los cortes en el PPsim.in
  runManager->SetUserInitialization( pl );
  
  runManager->SetUserAction(prim = new PPSPrimaryGeneratorAction(det));
  
  // Open Target File
  // set user action classes
  PPSRunAction* run; 
  PPSEventAction* event;
  runManager->SetUserAction(run = new PPSRunAction(det,prim)); 
  runManager->SetUserAction(event = new PPSEventAction(run));
  runManager->SetUserAction(new PPSSteppingAction(det,prim,run,event));
  
  // get the pointer to the User Interface manager 
  G4UImanager* UI = G4UImanager::GetUIpointer();  
  //runManager->Initialize();
  
  ////rootOutput.Init();

  //initialize visualization
  G4VisManager* visManager = nullptr;
  //detect interactive mode (if no arguments) and define UI session
  G4UIExecutive* ui = nullptr;
  if (argc == 1) ui = new G4UIExecutive(argc,argv);
  
  if (ui)  {
    //interactive mode
    visManager = new G4VisExecutive;
    visManager->Initialize();
    ui->SessionStart();
    delete ui;
  }
  else  {
    //so-called "batch" mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    prim->isBatchMode(true);
    G4cout << " Batch mode : True " << G4endl;
    UI->ApplyCommand(command+fileName);
  }

  delete runManager;
  return 0;
}
