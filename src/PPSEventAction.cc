#include "PPSEventAction.hh"
#include "PPSEventActionMessenger.hh"
#include "PPSSamplerHit.hh"
#include "PPSTargetCaloHit.hh"

#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4VVisManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "PPSRunAction.hh"
#include "G4SDManager.hh"

PPSEventAction::PPSEventAction(PPSRunAction * ra)
:printModulo(10000),eventMessenger(0),runAction(ra)
{
  eventMessenger = new PPSEventActionMessenger(this);
}

PPSEventAction::~PPSEventAction()
{
  delete eventMessenger;
}

void PPSEventAction::BeginOfEventAction(const G4Event* evt)
{
  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  SamplerCollID = SDman->GetCollectionID("SamplerCollection");

  G4int evtNb = evt->GetEventID();
  //printing survey
  if (evtNb%printModulo == 0)
    G4cout << "\n---> Begin of Event: " << evtNb << G4endl;
}


void PPSEventAction::EndOfEventAction(const G4Event* evt)
{
  G4int evtNb = evt->GetEventID();
  if (evtNb%printModulo == 0)
      G4cout << ">>> End Event " << evt->GetEventID() << G4endl;
  
  if (runAction) runAction->EventFinished();
    
  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
	PPSSamplerHitsCollection*  hc=NULL;
	PPSTargetCaloHitsCollection* TargetCaloHC=NULL;
  
  auto analysisManager = G4AnalysisManager::Instance();
    
  //G4cout << " SamplerCollID " << SamplerCollID << " for event " << evtNb << G4endl;
  
  // Remember where SamplerCollID comes from (BeginOfEventAction):
  //SamplerCollID = SDman->GetCollectionID("SamplerCollection");
  if(SamplerCollID>=0) {
    G4SDManager * SDman = G4SDManager::GetSDMpointer();
    G4int SiliconCollID = SDman->GetCollectionID("SiliconDetectorHC");
    G4int samplerIDs[2] = {SamplerCollID, SiliconCollID};
    
    for (int id = 0; id<2; id++){
      //hc = (PPSSamplerHitsCollection*)(HCE->GetHC(SamplerCollID));
      hc = (PPSSamplerHitsCollection*)(HCE->GetHC(samplerIDs[id]));
    
    //G4cout << " hc " << hc << " for event " << evtNb << G4endl;
    
    
    if(hc) {
      G4cout.precision(6);
      //G4cout << " hc entries " << hc->entries() << " for event " << evtNb << G4endl;
      for (G4int i=0; i<hc->entries(); i++) {
        
        analysisManager->FillNtupleIColumn(0, evtNb);
        
        if((*hc)[i]->GetName()=="Sampler0")
          analysisManager->FillNtupleIColumn(10, 0);
        else if((*hc)[i]->GetName()=="Sampler1")
          analysisManager->FillNtupleIColumn(10, 1);
	else if((*hc)[i]->GetName()=="SiDet1")
          analysisManager->FillNtupleIColumn(10, 2);
	else if((*hc)[i]->GetName()=="SiDet2")
          analysisManager->FillNtupleIColumn(10, 3);
	else if((*hc)[i]->GetName()=="SiDet3")
          analysisManager->FillNtupleIColumn(10, 4);
        else
          analysisManager->FillNtupleIColumn(10, 999);
            
        analysisManager->FillNtupleIColumn(1,(*hc)[i]->GetPDG());
        analysisManager->FillNtupleDColumn(2,(*hc)[i]->GetX()/mm);
        analysisManager->FillNtupleDColumn(3,(*hc)[i]->GetY()/mm);
        analysisManager->FillNtupleDColumn(4,(*hc)[i]->GetZ()/mm);
        analysisManager->FillNtupleDColumn(5,(*hc)[i]->GetPX());
        analysisManager->FillNtupleDColumn(6,(*hc)[i]->GetPY());
        analysisManager->FillNtupleDColumn(7,(*hc)[i]->GetPZ());
        analysisManager->FillNtupleDColumn(8,(*hc)[i]->GetEnergy()/MeV);
        analysisManager->FillNtupleIColumn(9,(*hc)[i]->GetTrackID());
        analysisManager->FillNtupleDColumn(11,(*hc)[i]->GetX0()/mm);
        analysisManager->FillNtupleDColumn(12,(*hc)[i]->GetY0()/mm);
        analysisManager->FillNtupleDColumn(13,(*hc)[i]->GetZ0()/mm);
        analysisManager->FillNtupleDColumn(14,(*hc)[i]->GetTime()/nanosecond);
        //sx=(*hc)[i]->GetSx();
        //sy=(*hc)[i]->GetSy();
        //sz=(*hc)[i]->GetSz();
            
        analysisManager->AddNtupleRow();
        //G4cout << " added row for event " << evtNb << G4endl;
      }
    }
    }
  }
    
	if(TargetCaloCollID>=0)
    TargetCaloHC = (PPSTargetCaloHitsCollection*)(HCE->GetHC(TargetCaloCollID));

  //if(TargetCaloHC)
  //    rootOutput.WriteDataTarget(TargetCaloHC);
  
  

}
