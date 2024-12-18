#include "PPSPhysicsList.hh"
#include "PPSPhysicsListMessenger.hh"

#include "PPSPhysListEmStandard.hh"
//#include "PPSOutput.hh"

#include "PPSStepMax.hh"

#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmStandardPhysicsWVI.hh"
#include "G4EmStandardPhysicsGS.hh"
#include "G4EmStandardPhysicsSS.hh"

#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmLowEPPhysics.hh"

#include "G4EmDNAPhysics.hh"
#include "G4EmDNAPhysics_option2.hh"
#include "G4EmDNAPhysics_option4.hh"
#include "G4EmDNAPhysics_option6.hh"

#include "G4HadronElasticPhysics.hh"

#include "G4DecayPhysics.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4LossTableManager.hh"

// particles

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4DNAGenericIonsManager.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

//extern PPSOutput rootOutput;

PPSPhysicsList::PPSPhysicsList() 
: G4VModularPhysicsList(), FlagNuclear(false)
{
	G4LossTableManager::Instance();
	
	currentDefaultCut   = 1.*mm;//micrometer;//1.*mm;
	cutForGamma         = currentDefaultCut;
	cutForElectron      = currentDefaultCut;
	cutForPositron      = currentDefaultCut;
	
	pMessenger = new PPSPhysicsListMessenger(this);
  
	SetVerboseLevel(1);
	
	// EM physics
	//fEmName = G4String("emstandard_opt4");
	//fEmPhysicsList = new G4EmStandardPhysics_option4();
	// En vez de la opt4, ponemos la default
	fEmName = G4String("emstandard");
	fEmPhysicsList = new G4EmStandardPhysics();

	// Deacy physics and all particles
  fDecayPhysics =  new G4DecayPhysics(1);
  SetDefaultCutValue(currentDefaultCut);
}

PPSPhysicsList::~PPSPhysicsList()
{
	delete pMessenger;
	delete fEmPhysicsList;
	delete fDecayPhysics;
}

void PPSPhysicsList::ConstructParticle()
{
  G4BosonConstructor  pBosonConstructor;
  pBosonConstructor.ConstructParticle();

  G4LeptonConstructor pLeptonConstructor;
  pLeptonConstructor.ConstructParticle();

  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4IonConstructor pIonConstructor;
  pIonConstructor.ConstructParticle();

  G4ShortLivedConstructor sLivedConstructor;
  sLivedConstructor.ConstructParticle();

  // Geant4-DNA

  G4DNAGenericIonsManager* genericIonsManager;
  genericIonsManager=G4DNAGenericIonsManager::Instance();
  genericIonsManager->GetIon("alpha++");
  genericIonsManager->GetIon("alpha+");
  genericIonsManager->GetIon("helium");
  genericIonsManager->GetIon("hydrogen");
	
}

void PPSPhysicsList::ConstructProcess()
{
	// Transportation
	//
	AddTransportation();
	// Electromagnetic physics list
	fEmPhysicsList->ConstructProcess();
	// decay physics list
	//
  fDecayPhysics->ConstructProcess();
  if(fHadPhysicsList) { fHadPhysicsList->ConstructProcess(); }
	// step limitation (as a full process)
	AddStepMax();      
}

void  PPSPhysicsList::AddPhotoElectroNuclear()
{
	SetFlagNuclear();
}

void PPSPhysicsList::AddPhysicsList(const G4String& name)
{	
	if (verboseLevel>0) {
		G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
	}
	
	if (name == fEmName) return;
	
  if (name == "local") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new PPSPhysListEmStandard(name);

  } else if (name == "emstandard_opt0") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics();

  } else if (name == "emstandard_opt1") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics_option1();

  } else if (name == "emstandard_opt2") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics_option2();
    
  } else if (name == "emstandard_opt3") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics_option3();
    
  } else if (name == "emstandard_opt4") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics_option4();

  } else if (name == "emstandardSS") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysicsSS();

  } else if (name == "emstandardWVI") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysicsWVI();

  } else if (name == "emstandardGS") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysicsGS();

  } else if (name == "empenelope") {
    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmPenelopePhysics();

  } else if (name == "emlowenergy") {
    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmLowEPPhysics();

  } else if (name == "emlivermore") {
    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmLivermorePhysics();
                        
  } else if (name == "dna") {
    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmDNAPhysics();
                        
  } else if (name == "dna_opt2") {
    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmDNAPhysics_option2();
                        
  } else if (name == "dna_opt4") {
    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmDNAPhysics_option4();
                        
  } else if (name == "dna_opt6") {
    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmDNAPhysics_option6();

  } else if (name == "had_elastic" && !fHadPhysicsList) {
    fHadPhysicsList = new G4HadronElasticPhysics();
                        
  } else {

    G4cout << "PhysicsList::AddPhysicsList: <" << name << ">"
           << " is not defined"
           << G4endl;
  }
}

void PPSPhysicsList::AddStepMax()
{
	// Step limitation seen as a process
	PPSStepMax* stepMaxProcess = new PPSStepMax();
  
  auto particleIterator=GetParticleIterator();
  particleIterator->reset();
  while ((*particleIterator)()){
    G4ParticleDefinition* particle = particleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();

    if (stepMaxProcess->IsApplicable(*particle))
      {
        pmanager ->AddDiscreteProcess(stepMaxProcess);
      }
  }

}

void PPSPhysicsList::SetCuts()
{    
	// set cut values for gamma at first and for e- second and next for e+,
	// because some processes for e+/e- need cut values for gamma
	SetCutValue(cutForGamma, "gamma");
	SetCutValue(cutForElectron, "e-");
	SetCutValue(cutForPositron, "e+");
}

void PPSPhysicsList::SetCutForGamma(G4double cut)
{
	cutForGamma = cut;
	SetParticleCuts(cutForGamma, G4Gamma::Gamma());
}

void PPSPhysicsList::SetCutForElectron(G4double cut)
{
	cutForElectron = cut;
	SetParticleCuts(cutForElectron, G4Electron::Electron());
}

void PPSPhysicsList::SetCutForPositron(G4double cut)
{
	cutForPositron = cut;
	SetParticleCuts(cutForPositron, G4Positron::Positron());
}
