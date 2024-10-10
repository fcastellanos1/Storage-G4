#include "PPSPhysicsListMessenger.hh"
#include "PPSPhysicsList.hh"
//#include "PPSOutput.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"

//extern PPSOutput rootOutput;

PPSPhysicsListMessenger::PPSPhysicsListMessenger(PPSPhysicsList* pPhys)
:pPhysicsList(pPhys)
{ 
  physDir = new G4UIdirectory("/ppsimui/phys/");
  physDir->SetGuidance("physics list commands");

  gammaCutCmd = new G4UIcmdWithADoubleAndUnit("/ppsimui/phys/setGCut",this);  
  gammaCutCmd->SetGuidance("Set gamma cut.");
  gammaCutCmd->SetParameterName("Gcut",false);
  gammaCutCmd->SetUnitCategory("Length");
  gammaCutCmd->SetRange("Gcut>0.0");
  gammaCutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  electCutCmd = new G4UIcmdWithADoubleAndUnit("/ppsimui/phys/setECut",this);  
  electCutCmd->SetGuidance("Set electron cut.");
  electCutCmd->SetParameterName("Ecut",false);
  electCutCmd->SetUnitCategory("Length");
  electCutCmd->SetRange("Ecut>0.0");
  electCutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  protoCutCmd = new G4UIcmdWithADoubleAndUnit("/ppsimui/phys/setPCut",this);  
  protoCutCmd->SetGuidance("Set positron cut.");
  protoCutCmd->SetParameterName("Pcut",false);
  protoCutCmd->SetUnitCategory("Length");
  protoCutCmd->SetRange("Pcut>0.0");
  protoCutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);  

  allCutCmd = new G4UIcmdWithADoubleAndUnit("/ppsimui/phys/setCuts",this);  
  allCutCmd->SetGuidance("Set cut for all.");
  allCutCmd->SetParameterName("cut",false);
  allCutCmd->SetUnitCategory("Length");
  allCutCmd->SetRange("cut>0.0");
  allCutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  pListCmd = new G4UIcmdWithAString("/ppsimui/phys/addPhysics",this);  
  pListCmd->SetGuidance("Select standard/polarized physics list.");
  pListCmd->SetParameterName("PList",false);
  pListCmd->AvailableForStates(G4State_PreInit);  

  PhotoElectroNuclearCmd = new G4UIcmdWithoutParameter("/ppsimui/phys/addPhotoElectroNuclear",this);
  PhotoElectroNuclearCmd ->SetGuidance("If defined set Photonuclear and electronuclear reaction ON");	
 // SetParameterName("addPhotoElectroNuclear",false) ; 
}

PPSPhysicsListMessenger::~PPSPhysicsListMessenger()
{
  delete physDir;
  delete gammaCutCmd;
  delete electCutCmd;
  delete protoCutCmd;
  delete allCutCmd;
  delete pListCmd;
  delete PhotoElectroNuclearCmd;
}

void PPSPhysicsListMessenger::SetNewValue(G4UIcommand* command,
                                          G4String newValue)
{       
  if( command == gammaCutCmd )
   { pPhysicsList->SetCutForGamma(gammaCutCmd->GetNewDoubleValue(newValue));}
     
  if( command == electCutCmd )
   { pPhysicsList->SetCutForElectron(electCutCmd->GetNewDoubleValue(newValue));}
     
  if( command == protoCutCmd )
   { pPhysicsList->SetCutForPositron(protoCutCmd->GetNewDoubleValue(newValue));}

  if( command == allCutCmd )
    {
      G4double cut = allCutCmd->GetNewDoubleValue(newValue);
      pPhysicsList->SetCutForGamma(cut);
      pPhysicsList->SetCutForElectron(cut);
      pPhysicsList->SetCutForPositron(cut);
    }
   
  if( command == pListCmd )
   { pPhysicsList->AddPhysicsList(newValue);}

 if( command ==  PhotoElectroNuclearCmd)
 { 
	 pPhysicsList->AddPhotoElectroNuclear();
     //rootOutput.StoreNoneEMParticles=true; 	 
 }
}
