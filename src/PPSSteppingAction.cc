
// local
#include "PPSSteppingAction.hh"
#include "PPSDetectorConstruction.hh"
#include "PPSPrimaryGeneratorAction.hh"
#include "PPSRunAction.hh"
#include "PPSEventAction.hh"

// geant4
#include "G4RunManager.hh"
#include "G4PolarizationHelper.hh"
#include "G4UnitsTable.hh"
#include "G4StokesVector.hh"
#include "G4Step.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//#include "PPSOutput.hh"
//extern PPSOutput rootOutput;

PPSSteppingAction::PPSSteppingAction(PPSDetectorConstruction* det,
				     PPSPrimaryGeneratorAction* prim, 
				     PPSRunAction* RuAct, 
				     PPSEventAction* EveAct)
  :detector(det), 
   primary(prim), 
   runAction(RuAct),
   eventAction(EveAct),
   tot_target(0.),
   tot_sampler(0.),
   MaxStepNumber(1000000) { 
}

PPSSteppingAction::~PPSSteppingAction() { 
}

void PPSSteppingAction::UserSteppingAction(const G4Step* aStep) {
  G4StepPoint* prePoint = aStep->GetPreStepPoint();
  G4StepPoint* endPoint = aStep->GetPostStepPoint();
  
  G4String procName = endPoint->GetProcessDefinedStep()->GetProcessName();
  runAction->CountProcesses(procName);
  
  G4Track* aTrack = aStep->GetTrack();
  G4ThreeVector direction = endPoint->GetMomentumDirection();
  
  G4String particleName = aTrack->GetDynamicParticle()->GetDefinition()->GetParticleName();
  //G4StokesVector pol = aTrack->GetPolarization();
  G4double TotalEnergy = endPoint->GetTotalEnergy();
  
  //G4cout << aTrack->GetTrackID() << " " << particleName << " " << aTrack->GetVolume()->GetName() << G4endl;
  
#if 0
  if(aTrack->GetTrackID() != 1 && aTrack->GetDefinition()->GetPDGEncoding() == -11) 
    {
      if(store_start_true == false && tID == -123454321)
        {
	  t0=theTrack->GetGlobalTime()/nanosecond;
	  tID=aTrack->GetTrackID();
	  store_start_true = true;
        }
      theTrack->GetVolume()->GetName();
    }
#endif    
  
#if 0
  if(particleName=="gamma")
    {  
      G4double photonCut = 2.*MeV;//1.*MeV;//2.*MeV;
      
      if(TotalEnergy < photonCut)
	{
	  aTrack->SetTrackStatus(fStopAndKill);
	  //          G4cout << "Photon KILLED "<< G4BestUnit(TotalEnergy,"Energy") << G4endl;
	}
    }
  if(particleName=="e-"||particleName=="e+")
    {
      G4double QEDchargedCut = 1.3*MeV;//1.*MeV;//1.3*MeV;
      if(TotalEnergy < QEDchargedCut)
	{
	  aTrack->SetTrackStatus(fStopAndKill);
	  //          G4cout << "e+/e- KILLED "<< G4BestUnit(TotalEnergy,"Energy") << G4endl;
	}
    }
#endif
  
  if(aTrack->GetProperTime() > 1e-4*second)
    {
      G4cout << "WARNING: ProperTime > 1.e-4 second!" << G4endl;
      G4cout<<" Killing the particle"<<G4endl;
      aTrack->SetTrackStatus(fStopAndKill);
      runAction->CountKilled++; 
    }
  
  if ((prePoint->GetTouchableHandle()->GetVolume()==detector->GetSampler1())
      && endPoint->GetTouchableHandle()->GetVolume()!=detector->GetSampler1())
    {
      G4ThreeVector position  = endPoint->GetPosition();
      G4double kinEnergy = endPoint->GetKineticEnergy();//GetTotalEnergy();
      G4ThreeVector beamDirection = primary->GetParticleGun()->GetParticleMomentumDirection();
      G4double polZ = endPoint->GetPolarization().z();
      
      G4double costheta=direction*beamDirection;
      G4double  xdir=direction*G4PolarizationHelper::GetParticleFrameX(beamDirection);
      G4double  ydir=direction*G4PolarizationHelper::GetParticleFrameY(beamDirection);
      G4double phi=std::atan2(ydir,xdir);
      
      tot_sampler+=TotalEnergy;
      runAction->FillData(particleName,0,0.,kinEnergy,costheta,phi,polZ);
      runAction->SetSumESampler1(tot_sampler);
    }   
  G4double edep=aStep->GetTotalEnergyDeposit();
  if ((prePoint->GetTouchableHandle()->GetVolume()==detector->GetTarget()) && edep>0.)
    {
      //tot_target+=edep; 
      runAction->FillData(particleName,1,edep,0.,0.,0.,0.);
      //		runAction->SetEDep(tot_target);	
    }
  
  
  if(TotalEnergy >0.)
    {
      G4TouchableHandle theTouchable = prePoint->GetTouchableHandle();
      G4int copyNo = theTouchable->GetCopyNumber();
      //G4int motherCopyNo = theTouchable->GetCopyNumber(1);
      std::string name  = (theTouchable->GetVolume()->GetName());
      
      
      int iz =  detector->GetMapBullet()[theTouchable->GetVolume()->GetName()].x();
      int iy =  detector->GetMapBullet()[theTouchable->GetVolume()->GetName()].y();
      int ix = copyNo;
      
     // rootOutput.EnergyDepoSpheres[iz][iy][ix] += aStep->GetTotalEnergyDeposit();
      tot_target+=aStep->GetTotalEnergyDeposit(); 
      runAction->SetEDep(tot_target);	
      
      //   G4cout << " Copy Nb " << iz << "  " << iy << " " << ix << detector->GetMapBullet()[theTouchable->GetVolume()->GetName()]
      // << "  " << theTouchable->GetVolume()->GetName() << "  "  << rootOutput.EnergyDepoSpheres[iz][iy][ix] << G4endl;
      
      
    }
  
  
#if 0  
  std::size_t found = (prePoint->GetTouchableHandle()->GetVolume()->GetName()).find("devidedBoxPhysZ1_Y_");
  if(TotalEnergy >0.)
    {
      if(found!=std::string::npos)
	{
	  int iy,ix;
	
	  G4TouchableHandle theTouchable = prePoint->GetTouchableHandle();
	  G4int copyNo = theTouchable->GetCopyNumber();
	  G4int motherCopyNo = theTouchable->GetCopyNumber(1);
	  std::string name  = (theTouchable->GetVolume()->GetName());
	  char tmp[256];
	  sprintf(tmp,"%s",(name.erase(0,19)).c_str());
      
	  G4int position_y = atoi(tmp);
	  //      G4int position_y = G4int((theTouchable->GetVolume()->GetName()).remove(0,19));
      
	  G4cout << " Copy Nb " << copyNo << "  " << motherCopyNo << "  " << theTouchable->GetVolume()->GetName() << G4endl;
	  iy = position_y;
	  ix = copyNo;
	  //rootOutput.EnergyDepoSpheresFirst[iy][ix] += aStep->GetTotalEnergyDeposit();
	}
    
      found = (prePoint->GetTouchableHandle()->GetVolume()->GetName()).find("devidedBoxPhysZ2_Y_");
      if(found!=std::string::npos)
	{
	  int iy,ix;
        
	  G4TouchableHandle theTouchable = prePoint->GetTouchableHandle();
	  G4int copyNo = theTouchable->GetCopyNumber();
	  G4int motherCopyNo = theTouchable->GetCopyNumber(1);
	  std::string name  = (theTouchable->GetVolume()->GetName());
	  char tmp[256];
	  sprintf(tmp,"%s",(name.erase(0,19)).c_str());
        
	  G4int position_y = atoi(tmp);
	  //      G4int position_y = G4int((theTouchable->GetVolume()->GetName()).remove(0,19));
        
	  // G4cout << " Copy Nb " << copyNo << "  " << position_y << "  " << theTouchable->GetVolume()->GetName() << G4endl;
	  iy = position_y;
	  ix = copyNo;
	  //rootOutput.EnergyDepoSpheresSecond[iy][ix] += aStep->GetTotalEnergyDeposit();
        
	  // G4cout << endPoint->GetTotalEnergy() << "  " << prePoint->GetTotalEnergy() <<  "  " << aStep->GetTotalEnergyDeposit() << G4endl;
	}

    }
#endif

}   


