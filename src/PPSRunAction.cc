#include "PPSRunAction.hh"

#include "PPSDetectorConstruction.hh"
#include "PPSPrimaryGeneratorAction.hh"

#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4EmCalculator.hh"

#include "Randomize.hh"
#include "G4SystemOfUnits.hh"
#include <iomanip>
//#include "PPSOutput.hh"
#include <vector>
#include <numeric>

#define max(A,B) ((A>B)? A:B)

extern G4double sum_initial_E;
//extern PPSOutput rootOutput;

PPSRunAction::PPSRunAction(PPSDetectorConstruction* det, PPSPrimaryGeneratorAction* prim)
: CountKilled(0), detector(det), primary(prim), ProcCounter(0),itstottarget(0.),itstotsampler(0.)
{
	totalEventCount=0;
  
  // Create analysis manager
  // The choice of the output format is done via the specified
  // file extension.
  auto analysisManager = G4AnalysisManager::Instance();

  // Create directories
  //analysisManager->SetHistoDirectoryName("histograms");
  //analysisManager->SetNtupleDirectoryName("ntuple");
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
  // Note: merging ntuples is available only with Root output

  // Creating ntuple
  //
  analysisManager->CreateNtuple("MC", "MC");
  analysisManager->CreateNtupleIColumn("evtID"); //0
  analysisManager->CreateNtupleIColumn("pdg"); //1
  analysisManager->CreateNtupleDColumn("x"); //2
  analysisManager->CreateNtupleDColumn("y"); //3
  analysisManager->CreateNtupleDColumn("z"); //4
  analysisManager->CreateNtupleDColumn("px"); //5
  analysisManager->CreateNtupleDColumn("py"); //6
  analysisManager->CreateNtupleDColumn("pz"); //7
  analysisManager->CreateNtupleDColumn("E"); //8
  analysisManager->CreateNtupleIColumn("trackID"); //9
  analysisManager->CreateNtupleIColumn("samplerID"); //10
  analysisManager->CreateNtupleDColumn("x0"); //11
  analysisManager->CreateNtupleDColumn("y0"); //12
  analysisManager->CreateNtupleDColumn("z0"); //13
  analysisManager->CreateNtupleDColumn("t"); //14
  analysisManager->CreateNtupleDColumn("Edep"); //15
  
  analysisManager->FinishNtuple();
}

PPSRunAction::~PPSRunAction()   
{ 
}

void PPSRunAction::BeginOfRunAction(const G4Run* aRun)
{  
	G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
	//rootOutput.NbEvents=aRun->GetNumberOfEventToBeProcessed();
    
	if (ProcCounter) delete ProcCounter;
	ProcCounter = new ProcessesCount;
	totalEventCount = 0;
	photonStatsSampler.Clear();
	electronStatsSampler.Clear();
	positronStatsSampler.Clear();
  
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  G4String fileName = primary->GetOutputFilename();//"tuple.root";
  analysisManager->OpenFile(fileName);
  G4cout << "Using " << analysisManager->GetType() << G4endl;
	
}

void PPSRunAction::FillData(const G4String & particleName,G4bool target_yes,G4double depEnergy, G4double kinEnergy, G4double costheta, 
							G4double /* phi*/,
							G4double longitudinalPolarization)
{
	if (particleName=="gamma") 
		photonStatsSampler.FillData(target_yes,depEnergy,kinEnergy, costheta, longitudinalPolarization);
	else if (particleName=="e-") 
		electronStatsSampler.FillData(target_yes,depEnergy,kinEnergy, costheta, longitudinalPolarization);
	else if (particleName=="e+") 
		positronStatsSampler.FillData(target_yes,depEnergy,kinEnergy, costheta, longitudinalPolarization);
}
void PPSRunAction::CountProcesses(G4String procName)
{
	// is the process already counted ?
	// *AS* change to std::map?!
	size_t nbProc = ProcCounter->size();
	size_t i = 0;
	while ((i<nbProc)&&((*ProcCounter)[i]->GetName()!=procName)) i++;
	if (i == nbProc) ProcCounter->push_back( new OneProcessCount(procName));
	
	(*ProcCounter)[i]->Count();
}

G4int PPSRunAction::NbOfEvents = 0;
G4String PPSRunAction::Particle ="un_string";

void PPSRunAction::EndOfRunAction(const G4Run* aRun)
{
  auto analysisManager = G4AnalysisManager::Instance();
	ofstream fout("run_log.dat");
	G4cout << "------->>>>>>>   Nb of particles killed     " << CountKilled << G4endl;
	
	NbOfEvents = aRun->GetNumberOfEvent();
	if (NbOfEvents == 0) return;
	G4ParticleDefinition* particle = primary->GetParticleGun()->GetParticleDefinition();
	Particle = particle->GetParticleName(); 
	
	/*if(NbOfEvents!=rootOutput.HitSampler0)
    { 
		G4cout     << "\n#############" << rootOutput.buffer_datetime << "####################\n"<< G4endl;
        G4cout     << "\n .........WARNING ........... not all the particles have impinged the target \n"  << G4endl;
        fout       << "\n#############" << rootOutput.buffer_datetime << "####################\n"<< G4endl;
        fout       << "\n .........WARNING ........... not all the particles have impinged the target \n"  << G4endl;
    }
	else
	{
		G4cout     << "\n#############" << rootOutput.buffer_datetime << "####################\n"<< G4endl;
		G4cout     << "\n				All the particles have impinged the target 			\n"  << G4endl;
		fout       << "\n#############" << rootOutput.buffer_datetime << "####################\n"<< G4endl;
		fout	   << "\n				All the particles have impinged the target 			\n"  << G4endl;
	}
	*/
	RunCaracteristics(cout);
	RunCaracteristics(fout);
	
	G4cout << "\n Process calls frequency --->\n";
	for (size_t i=0; i< ProcCounter->size();i++) {
		G4String procName = (*ProcCounter)[i]->GetName();
		G4int    count    = (*ProcCounter)[i]->GetCounter(); 
		G4cout << "\t" << procName << " = " << count<<"\n";
		
	}
	
	if (totalEventCount == 0) return;
	
	char temp[256];
	
    	sprintf(temp,"Gammas");
	photonStatsSampler.MeanWriteFile(fout,temp,particle->GetPDGEncoding(),detector->GetTargetThickness(),detector->GetTargetMaterial()->GetName(),totalEventCount);
	photonStatsSampler.MeanWriteFile(cout,temp,particle->GetPDGEncoding(),detector->GetTargetThickness(),detector->GetTargetMaterial()->GetName(),totalEventCount);
	
	sprintf(temp,"Electrons");
	electronStatsSampler.MeanWriteFile(fout,temp,particle->GetPDGEncoding(),detector->GetTargetThickness(),detector->GetTargetMaterial()->GetName(),totalEventCount);
	electronStatsSampler.MeanWriteFile(cout,temp,particle->GetPDGEncoding(),detector->GetTargetThickness(),detector->GetTargetMaterial()->GetName(),totalEventCount);

	sprintf(temp,"Positrons");
	positronStatsSampler.MeanWriteFile(fout,temp,particle->GetPDGEncoding(),detector->GetTargetThickness(),detector->GetTargetMaterial()->GetName(),totalEventCount);
	positronStatsSampler.MeanWriteFile(cout,temp,particle->GetPDGEncoding(),detector->GetTargetThickness(),detector->GetTargetMaterial()->GetName(),totalEventCount);
    
    CLHEP::HepRandom::showEngineStatus();
    
    for(int i=0;i<detector->GetSlicesZ();i++)
    {

        G4double pedd = 0.;
        
        /*for(int j=0;j<rootOutput.nb_slices[0];j++)
        {   
            std::vector<G4double> vec = rootOutput.GetVectorEnergy()[i][j];
            G4double mean_edep = accumulate(vec.begin(), vec.end(),0.);
            
            pedd = max(mean_edep,pedd);
       (rootOutput.GetVectorHistoR())[i]->SetBinContent(j,mean_edep);
          //  G4cout << j << "  " <<  mean_edep << "  " << n << G4endl;
#if 0           
            for (std::vector <G4double>::iterator xi = vec.begin(); xi != vec.end(); ++xi)
            {
                G4double const x = *xi;
            }  
#endif
        }*/
    }
  
  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();
    
}


void PPSRunAction::RunCaracteristics(ostream &output_type)
{
    G4cout.precision(5);
	output_type  << "----------------------------------------"      << G4endl;
	//output_type  << "Number of particles : " << NbOfEvents << " ( " << (rootOutput.HitSampler0/(double)NbOfEvents)*100.
  output_type  << "% Hits Sampler 0)\n   "
    << "Particles Type      : " << Particle   << "\n" 
    << "Mean energy         : " << (sum_initial_E/NbOfEvents)/MeV   << " MeV " << G4endl;
	output_type     << "---------------------------------------- "  <<  G4endl;
    
	output_type     << "Material            : " << detector->GetTargetMaterial()->GetName()
    << " (density: "<< G4BestUnit(detector->GetTargetMaterial()->GetDensity(),"Volumic Mass") << ")\n"  
	<< "Thickness           : " << detector->GetTargetThickness()/mm  << " mm " << "\n"
	<< "Size     : " << detector->GetTargetSize()/mm << " mm " <<"\n"
	<< "Number of slices Z  : " << detector->GetSlicesZ()<< "\n"
	<< "Number of slices R : " << detector->GetSlicesR()<<"\n"
   // << "RMS                r: " << rootOutput.rms         << " mm"
    << G4endl;	
	output_type   << "-----------------------------------------"<<G4endl;
	output_type  << "Total energy deposited in the target	: " << GetEDep() / MeV << " MeV" << G4endl;
	output_type  << "Total energy crossing Sampler 1		: " << GetSumESampler1() / MeV << " MeV" << G4endl;
	output_type  << "---------------------------------------- "<<G4endl;



}
void PPSRunAction::EventFinished()
{
	++totalEventCount;
	photonStatsSampler.EventFinished();
	electronStatsSampler.EventFinished();
	positronStatsSampler.EventFinished();
}

PPSRunAction::ParticleStatistics::ParticleStatistics()
: currentNumberSampler(0),currentNumberTarget(0),
totalNumberSampler(0), totalNumberSampler2(0),
sumCosTheta(0), sumCosTheta2(0),
sumDepo(0),sumDepo2(0),
sumPolarization(0),sumPolarization2(0)
{}

PPSRunAction::ParticleStatistics::~ParticleStatistics()
{}

G4int PPSRunAction::ParticleStatistics::GetNpOfParticlesProduced()
{return totalNumberSampler;}


void PPSRunAction::ParticleStatistics::EventFinished()
{
	totalNumberSampler+=currentNumberSampler;
	totalNumberSampler2+=currentNumberSampler*currentNumberSampler;
	totalNumberTarget+=currentNumberTarget;
	totalNumberTarget2+=currentNumberTarget*currentNumberTarget;
	currentNumberSampler=0;
	currentNumberTarget=0;
}

void PPSRunAction::ParticleStatistics:: FillData(G4bool target_yes,G4double depEnergy,G4double kinEnergy, 
												 G4double costheta,
												 G4double longitudinalPolarization)
{
	if(target_yes==0)
	{
		++currentNumberSampler;
		sumEnergy+=kinEnergy;
		sumEnergy2+=kinEnergy*kinEnergy;
		sumPolarization+=longitudinalPolarization;
		sumPolarization2+=longitudinalPolarization*longitudinalPolarization;
		sumCosTheta+=costheta;
		sumCosTheta2+=costheta*costheta;
	}
	else 
	{
		++currentNumberTarget;
		sumDepo+=depEnergy;
		sumDepo2+=depEnergy*depEnergy;	
	}
}

void PPSRunAction::ParticleStatistics::MeanWriteFile
(ostream &output_type,char particle_name[256],G4int part,G4double targetlength,G4String matname,G4int totalNumberSamplerOfEvents)
{
	output_type << "------ particle name ---------- " << particle_name << " -- " << part << "\n" ;
  output_type << "------ target length ---------- " << targetlength << "\n" ;
  output_type << "------ material name ---------- " << matname << "\n" ;
	output_type << "Mean Number per Event : " << double(totalNumberSampler/double(totalNumberSamplerOfEvents)) << "\n" ;
	if (totalNumberSampler!=0) {
	  G4double energyMean=sumEnergy/totalNumberSampler;
	  G4double energyRms=std::sqrt(sumEnergy2/totalNumberSampler-energyMean*energyMean);
	  output_type << "Mean Kinetic Energy : " << energyMean / MeV << " +- " << energyRms / MeV << "\n" ;
	  G4double polarizationMean=sumPolarization/totalNumberSampler;
	  G4double polarizationRms=std::sqrt(sumPolarization2/totalNumberSampler-polarizationMean*polarizationMean);
	  output_type << "Mean Polarization : " << polarizationMean << " +- " <<polarizationRms << "\n";
	}
	output_type << "Energy deposited in the target per incident particles : " << (sumDepo /double(totalNumberSamplerOfEvents)) << " MeV " << G4endl;
}

void PPSRunAction::ParticleStatistics::PrintResults(G4int totalNumberSamplerOfEvents)
{
	G4cout<<"Mean Number per Event :" <<G4double(totalNumberSampler)/G4double(totalNumberSamplerOfEvents)<<"\n";
	
	if (totalNumberSampler!=0) { 
	  G4double energyMean=sumEnergy/totalNumberSampler;
	  G4double energyRms=std::sqrt(sumEnergy2/totalNumberSampler-energyMean*energyMean);
	  G4cout<<"Mean Energy :"<< energyMean <<" +- "<<energyRms << " MeV " << "\n";
	  G4double polarizationMean=sumPolarization/totalNumberSampler;
	  G4double polarizationRms=std::sqrt(sumPolarization2/totalNumberSampler-polarizationMean*polarizationMean);
	  G4cout<<"Mean Polarization :"<< polarizationMean <<" +- "<<polarizationRms<<"\n";
	}
}

void PPSRunAction::ParticleStatistics::Clear()
{
	currentNumberTarget=0;
	currentNumberSampler=0;
	totalNumberSampler=totalNumberSampler2=0;
	sumEnergy=sumEnergy2=0;
	sumDepo=sumDepo2=0;
	sumPolarization=sumPolarization2=0;
	sumCosTheta=sumCosTheta2=0;
}
