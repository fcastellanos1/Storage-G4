#ifndef PPSPhysicsList_h
#define PPSPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class PPSPhysicsListMessenger;
class G4VPhysicsConstructor;

class PPSPhysicsList: public G4VModularPhysicsList
{
  public:
    PPSPhysicsList();
   ~PPSPhysicsList();

    void ConstructParticle();
    void ConstructProcess();
    void AddPhysicsList(const G4String& name);
    
    void AddStepMax();           

    void SetCuts();
    void SetCutForGamma(G4double);
    void SetCutForElectron(G4double);
    void SetCutForPositron(G4double);
 
	void AddPhotoElectroNuclear();

  private:
    G4double cutForGamma;
    G4double cutForElectron;
    G4double cutForPositron;
    G4double currentDefaultCut;

    G4String fEmName = " ";
    G4VPhysicsConstructor*  fEmPhysicsList  = nullptr;
    G4VPhysicsConstructor*  fDecayPhysics   = nullptr;
    G4VPhysicsConstructor*  fHadPhysicsList = nullptr;

    PPSPhysicsListMessenger* pMessenger = nullptr;
    G4bool FlagNuclear;
    inline void SetFlagNuclear() {FlagNuclear=true;}
    G4bool GetFlagNuclear(){return FlagNuclear;}

};
#endif
