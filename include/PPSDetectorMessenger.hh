//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
   PPSIM code.   
   dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#ifndef PPSDetectorMessenger_h
#define PPSDetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class PPSDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PPSDetectorMessenger: public G4UImessenger
{
	public:

		PPSDetectorMessenger(PPSDetectorConstruction* );
		~PPSDetectorMessenger();

		void SetNewValue(G4UIcommand*, G4String);

	private:

		PPSDetectorConstruction*   			Detector;

		G4UIdirectory*             			ppsimuiDir;
		G4UIdirectory*             			detDir;    
		G4UIdirectory*	        		  	CaptureDir;

		G4UIcmdWithAString*        			MaterCmd;
		G4UIcmdWithADoubleAndUnit* 			TargetSizeCmd;
		G4UIcmdWithADoubleAndUnit* 			TargetThicknessCmd;
		//crystal stuff

		G4UIcmdWithADoubleAndUnit*        	AmdSizeCmd;
		G4UIcmdWithADoubleAndUnit*        	AmdLengthCmd;
		G4UIcmdWithADoubleAndUnit*        	AmdB0Cmd;

		G4UIcmdWithoutParameter*   			UpdateCmd;
		G4UIcmdWithAnInteger*      			CellRCmd;
		G4UIcmdWithAnInteger*      			CellZCmd;
		G4UIcmdWithAnInteger*      			BinPedd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

