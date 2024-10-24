#include "PPSPrimaryGeneratorMessenger.hh"
#include "PPSPrimaryGeneratorAction.hh"
//#include "PPSOutput.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"

//extern PPSOutput rootOutput;

PPSPrimaryGeneratorMessenger::PPSPrimaryGeneratorMessenger(PPSPrimaryGeneratorAction* PPSGun)
:PPSAction(PPSGun)
{
	gunDir = new G4UIdirectory("/ppsimui/gun/");
	gunDir->SetGuidance("PrimaryGenerator control");

	outputCmd = new G4UIcmdWithAString("/ppsimui/gun/output",this);
	outputCmd->SetGuidance("Set the name of the output root file");
	outputCmd->SetParameterName("filename",false);
	outputCmd->SetDefaultValue("tuple.root");
	outputCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
	
	EnergyCmd = new G4UIcmdWithADoubleAndUnit("/ppsimui/gun/energy",this);
	EnergyCmd->SetGuidance("Set Primary energy beam (force all the primary energy)");
	EnergyCmd->SetParameterName("Energy",false);
	EnergyCmd->SetRange("Energy>0.");
	EnergyCmd->SetUnitCategory("Energy");
	EnergyCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
	
	PolarizationCmd = new G4UIcmdWith3Vector("/ppsimui/gun/polarization",this);  
	PolarizationCmd->SetGuidance("Set Primary Polarization beam (force all the primary polarization)");
	PolarizationCmd->SetParameterName("Px","Py","Pz",true,true); 
	PolarizationCmd->SetRange("Px>=-1.&&Px<=1.&&Py>=-1.&&Py<=1.&&Pz>=-1.&&Pz<=1."); 
    
	ZpositionCmd=new G4UIcmdWithADoubleAndUnit("/ppsimui/gun/setZposition",this);
	ZpositionCmd->SetGuidance("Set Primary Z position beam (force all the primary Z position)");
	ZpositionCmd->SetParameterName("Zposition",false);
	//ZpositionCmd->SetRange("Zposition<=0.");
	ZpositionCmd->SetUnitCategory("Length");
	ZpositionCmd->SetUnitCandidates("m");
	ZpositionCmd->AvailableForStates(G4State_PreInit,G4State_Idle);	
    
	GaussianRMSCmd=new G4UIcmdWithADouble("/ppsimui/gun/setGaussRMS",this);
	GaussianRMSCmd->SetGuidance("Set RMS value for r spatial coordinate in mm");
	GaussianRMSCmd->SetParameterName("GaussRMS",false);
	//  GaussianRMSCmd->SetDefaultUnit("mm");
	GaussianRMSCmd->SetRange("GaussRMS>=0.");
	//  GaussianRMSCmd->SetUnitCategory("Length");
	GaussianRMSCmd->AvailableForStates(G4State_PreInit,G4State_Idle); 
	
	RndmCmd = new G4UIcmdWithAString("/ppsimui/gun/rndm",this);
	RndmCmd->SetGuidance("Shoot randomly the incident particle.");
	RndmCmd->SetGuidance("Choice : on(default), off");
	RndmCmd->SetParameterName("choice",true);
	RndmCmd->SetDefaultValue("on");
	RndmCmd->SetCandidates("on off");
	RndmCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

PPSPrimaryGeneratorMessenger::~PPSPrimaryGeneratorMessenger()
{
	delete EnergyCmd;
	delete PolarizationCmd;
	delete RndmCmd;
	delete gunDir;
	delete ZpositionCmd;
	delete outputCmd;
}

void PPSPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
	if( command == RndmCmd )
	{
		PPSAction->SetRndmFlag(newValue);
	}
	if( command == outputCmd )
	{
		PPSAction->SetOutputFilename(newValue);
	}
	if( command == EnergyCmd )
	{
		PPSAction->SetPrimaryEnergy(EnergyCmd->GetNewDoubleValue(newValue));
	}
	if( command == ZpositionCmd)
	{ 
		PPSAction->SetZposition(ZpositionCmd->GetNewDoubleValue(newValue));
		//rootOutput.startZ=atof(newValue.c_str());
	}
    if (command == GaussianRMSCmd)
	{
		PPSAction->SetRMS(GaussianRMSCmd->GetNewDoubleValue(newValue));
	}	  
if( command == PolarizationCmd )
	{ 	
		(PPSAction->SetPrimaryPolarization(PolarizationCmd->GetNew3VectorValue(newValue)));
	}
	
}

