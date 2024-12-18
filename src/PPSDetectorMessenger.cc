#include "PPSDetectorMessenger.hh"

#include "PPSDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAnInteger.hh"

	PPSDetectorMessenger::PPSDetectorMessenger(PPSDetectorConstruction * Det)
:Detector(Det)
{ 
	ppsimuiDir = new G4UIdirectory("/ppsimui/");
	ppsimuiDir->SetGuidance("Commands specific to PPSIM");

	detDir = new G4UIdirectory("/ppsimui/det/");
	detDir->SetGuidance("Detector construction");

	MaterCmd = new G4UIcmdWithAString("/ppsimui/det/setMat",this);
	MaterCmd->SetGuidance("Select material of the box.");
	MaterCmd->SetParameterName("choice",false);
	MaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	TargetSizeCmd = new G4UIcmdWithADoubleAndUnit("/ppsimui/det/setTargetSize",this);
	TargetSizeCmd->SetGuidance("Set radius of the target");
	TargetSizeCmd->SetParameterName("TargetSize",false);
	TargetSizeCmd->SetRange("TargetSize>0.");
	TargetSizeCmd->SetUnitCategory("Length");
	TargetSizeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	TargetThicknessCmd = new G4UIcmdWithADoubleAndUnit("/ppsimui/det/setTargetThickness",this);
	TargetThicknessCmd->SetGuidance("Set thickness of the target");
	TargetThicknessCmd->SetParameterName("TargetThicknes",false);
	TargetThicknessCmd->SetRange("TargetThicknes>=0.");
	TargetThicknessCmd->SetUnitCategory("Length");
	TargetThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);


	CellRCmd = new G4UIcmdWithAnInteger("/ppsimui/det/setCellR",this);
	CellRCmd->SetGuidance("Set cell in r");
	CellRCmd->AvailableForStates(G4State_PreInit,G4State_Idle);


	CellZCmd = new G4UIcmdWithAnInteger("/ppsimui/det/setCellZ",this);
	CellZCmd->SetGuidance("Set cell in Z");
	CellZCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	BinPedd = new G4UIcmdWithAnInteger("/ppsimui/det/setbinpedd",this);
	BinPedd->SetGuidance("Set bin in x for pedd calculation");
        BinPedd->AvailableForStates(G4State_PreInit,G4State_Idle);

	CaptureDir = new G4UIdirectory("/ppsimui/det/Capture/"); 
	CaptureDir->SetGuidance("Capture caracteristics");

	UpdateCmd = new G4UIcmdWithoutParameter("/ppsimui/det/update",this);
	UpdateCmd->SetGuidance("Update calorimeter geometry.");
	UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
	UpdateCmd->SetGuidance("if you changed geometrical value(s).");
	UpdateCmd->AvailableForStates(G4State_Idle);
}

PPSDetectorMessenger::~PPSDetectorMessenger()
{ 
	delete MaterCmd;
	delete TargetSizeCmd;
	delete TargetThicknessCmd; 
	delete UpdateCmd;
	delete detDir;  
	delete ppsimuiDir;
}

void PPSDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
	if( command == MaterCmd )
	{ Detector->SetTargetMaterial(newValue);}

	if( command == TargetSizeCmd )
	{ Detector->SetTargetSize(TargetSizeCmd->GetNewDoubleValue(newValue));}
	if( command == TargetThicknessCmd )
	{ Detector->SetTargetThickness(TargetThicknessCmd->GetNewDoubleValue(newValue));}

	if( command == CellRCmd)
	{ Detector->SetCellR(CellRCmd->GetNewIntValue(newValue));}

	if( command == CellZCmd)
	{ Detector->SetCellZ(CellZCmd->GetNewIntValue(newValue));}
	
	if( command == BinPedd)
	{ Detector->SetBinPedd(BinPedd->GetNewIntValue(newValue));}

	if( command == UpdateCmd )
	{ Detector->UpdateGeometry(); }
}
