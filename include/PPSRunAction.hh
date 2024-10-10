#ifndef PPSRunAction_h
#define PPSRunAction_h 1

#include "G4UserRunAction.hh"
#include "PPSProcessesCount.hh"
#include "globals.hh"
#include <fstream>
using namespace std;

class PPSDetectorConstruction;
class PPSPrimaryGeneratorAction;
//class PPSHistoManager;
class G4Run;

class PPSRunAction : public G4UserRunAction
	{
		class ParticleStatistics {
		public:
			ParticleStatistics();
			~ParticleStatistics();
			void EventFinished();
			void FillData(G4bool target_yes,G4double depEnergy,G4double kinEnergy, G4double costheta,
						  G4double longitudinalPolarization);
			void PrintResults(G4int totalNumberSamplerOfEvents);
			void Clear();
			void MeanWriteFile(ostream &output_type,char particle_name[256],G4int part,G4double targetlength,G4String matname,G4int totalNumberSamplerOfEvents);
			G4int GetNpOfParticlesProduced();
		private:
			G4int currentNumberSampler;
			G4int currentNumberTarget;
			G4int totalNumberSampler, totalNumberSampler2;
			G4int totalNumberTarget, totalNumberTarget2;
			G4double sumEnergy, sumEnergy2;
			G4double sumCosTheta, sumCosTheta2;
			G4double sumDepo, sumDepo2;
			G4double sumPolarization, sumPolarization2;
		};
		
	public:
		PPSRunAction(PPSDetectorConstruction*, PPSPrimaryGeneratorAction*);
		virtual ~PPSRunAction();
		
		void   BeginOfRunAction(const G4Run*);
		void   EndOfRunAction(const G4Run*);
		
		void CountProcesses(G4String);
		
		void FillData(const G4String & particleName,G4bool target_yes,G4double depEnergy,
					  G4double kinEnergy, G4double costheta, G4double phi,
					  G4double longitudinalPolarization);
		void EventFinished();
		
		void SetEDep(G4double tot_target);
		void SetSumESampler1(G4double tot_sampler);
		inline G4double GetEDep();
		inline G4double GetSumESampler1();
		G4int CountKilled;	
		void RunCaracteristics(ostream &output_type);
		
		static G4int NbOfEvents;
		static G4String Particle;
		
	private:
		PPSDetectorConstruction*   detector;
		PPSPrimaryGeneratorAction* primary;
		ProcessesCount*         ProcCounter;
		
		G4int totalEventCount;
		
		ParticleStatistics photonStatsSampler;
		ParticleStatistics electronStatsSampler;
		ParticleStatistics positronStatsSampler;
		
		G4double E; 
		//  ofstream run_log;
		FILE* pFile;	 
		G4double itstottarget;	
		G4double itstotsampler;	
	};

inline void PPSRunAction::SetEDep(G4double tot_target)
{itstottarget = tot_target;}

inline  G4double PPSRunAction::GetEDep()
{return itstottarget;}

inline void PPSRunAction::SetSumESampler1(G4double tot_sampler)
{itstotsampler = tot_sampler;}

inline  G4double PPSRunAction::GetSumESampler1()
{return itstotsampler;}

#endif

