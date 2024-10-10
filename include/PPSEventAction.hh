/*
   PPSIM code.   
   Olivier Dadoun
 */
#ifndef PPSEventAction_h
#define PPSEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class PPSEventActionMessenger;
class PPSRunAction;

class PPSEventAction : public G4UserEventAction
{
	public:
		PPSEventAction(PPSRunAction *ra);
		~PPSEventAction();

	public:
		void 	BeginOfEventAction(const G4Event*);
		void	EndOfEventAction(const G4Event*);

		void SetPrintModulo(G4int val) {printModulo = val;};
   		G4int stepNumber;
	
	private:
		G4int                 		printModulo;                    
		PPSEventActionMessenger* 	eventMessenger;
		PPSRunAction*            	runAction;
		G4int SamplerCollID;
		G4int TargetCaloCollID;
};

#endif
