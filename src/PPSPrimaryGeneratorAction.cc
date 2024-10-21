#include "PPSPrimaryGeneratorAction.hh"
#include "PPSPrimaryGeneratorMessenger.hh"
#include "PPSDetectorConstruction.hh"
//#include "PPSOutput.hh"
#include "PPSInput.hh"

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"

extern PPSInput input;
//extern PPSOutput rootOutput;
class PPSPrimaryGeneratorMessenger;
// Keep initial point in phase space for diagnostics
G4double initial_E,sum_initial_E;

PPSPrimaryGeneratorAction::PPSPrimaryGeneratorAction(PPSDetectorConstruction* det)
:detector(det), itsRMS(0.),itsbatch(true)
{
	particleGun  = new G4ParticleGun(1);
	primaryMessenger = new PPSPrimaryGeneratorMessenger(this);
	G4ParticleDefinition* particle
		= G4ParticleTable::GetParticleTable()->FindParticle("gamma");
	particleGun->SetParticleDefinition(particle);
	particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
	particleGun->SetParticlePosition(G4ThreeVector(0.*m, 0.*m, 0.*m));

	particleGun->SetParticleEnergy(10.*MeV);    

}

PPSPrimaryGeneratorAction::~PPSPrimaryGeneratorAction()
{
	delete particleGun;
}

void PPSPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if(GetBatchMode()==true)
    {
      //this function is called at the begining of event
      G4double x0=0., y0=0., z0=0., xp0=0., yp0=0., zp0=0.,px0=0.,py0=0.,pz0=0.;
      G4double Sx0=0.,Sy0=0.,Sz0=0.;
      G4ThreeVector pol;
      G4ThreeVector PartMomDir;
      
      // Case 1: input file in PPsim.in
      if(input.GetInputName()!="")
	{
	  
	  // Case 1.1: ROOT input file (Nikolai's generator)
	  if(input.IsRoot()){
	    input.GetNextParticle(x0,y0,z0,px0,py0,pz0,Sx0,Sy0,Sz0);
	    px0=px0*eV;
	    py0=py0*eV;
	    pz0=pz0*eV;
	    x0=x0*meter;
	    y0=y0*meter;
	    z0=GetZposition() + z0*meter;
	    if(GetPrimaryEnergy())
	      {
	      G4cerr << " Error input file set and also energy inside .in , not compatible I exited" << G4endl;
	      exit(1);
	      }
	    
	    particleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
	    
	    G4double P=sqrt(px0*px0+py0*py0+pz0*pz0);
	    xp0=px0/P;
	    yp0=py0/P;
	    zp0=pz0/P;
	    PartMomDir=G4ThreeVector(xp0,yp0,zp0);
	    particleGun->SetParticleMomentumDirection(PartMomDir);
	    particleGun->SetParticleEnergy(P);
	    
	    G4ThreeVector polari =G4ThreeVector(Sx0,Sy0,Sz0);
	  
	    particleGun->SetParticlePolarization(polari);
	    
	    sum_initial_E += P;
	    initial_E=P;
	  
	  }

	  // Case 1.2: txt input (Muchnoi's generator: Guangyi's case)
	  if(input.IsTxt()){
	    
	    input.GetNextParticle(x0,y0,z0,px0,py0,pz0,Sx0,Sy0,Sz0);
	    if(GetPrimaryEnergy())
	      {
		G4cerr << " Error input file set and also energy inside .in , not compatible I exited" << G4endl;
		exit(1);
	      }
	    particleGun->SetParticlePosition(G4ThreeVector(x0*mm, y0*mm, 0*m)); // For now: not using Z initial position from PPsim.in. Muchnoi's file already have it simulated
	    particleGun->SetParticleMomentumDirection(G4ThreeVector(px0,py0,std::sqrt(1-px0*px0-py0*py0)));
	    particleGun->SetParticleEnergy(pz0*GeV);
	    particleGun->SetParticlePolarization( G4ThreeVector(Sx0,Sy0,Sz0) );
	    sum_initial_E += pz0*GeV;
	    initial_E = pz0*GeV;
	  }
	
	} // End case 1
	  
      // Case 2: no input filename in PPsim.in
      else
	{
	  if(!GetPrimaryEnergy())
	    {
	      G4cerr << " Error from PPSPrimaryGeneratorAction\n Input file not specified nor incident energy checked you ppsim input file" << G4endl;
	      exit(1);
	    }
      
	  // Gaussian radius distribution
	  if(GetRMS()){
	    //rootOutput.rms=GetRMS();//!! in mm
	    G4double sigmaR = GetRMS()*1.e-1;//in cm !!!
	    G4double x0_new = G4RandGauss::shoot(0., sigmaR );
	    G4double y0_new = G4RandGauss::shoot(0., sigmaR );
	    x0 += x0_new;
	    y0 += y0_new;
	  }
	  
	  x0 *= centimeter;
	  y0 *= centimeter;
	  
	  G4double P=sqrt(px0*px0+py0*py0+pz0*pz0);
	  xp0=px0/P;
	  yp0=py0/P;
	  zp0=pz0/P;
	  
	  G4double M=particleGun->GetParticleDefinition()->GetPDGMass()/MeV;
	  G4double E=sqrt(P*P+M*M);
	  
	  E*=MeV;
	  
	  //If we forced the incident energy
	  // we must also forced the momentum direction
	  if(GetPrimaryEnergy()){
	    E=GetPrimaryEnergy();
	    xp0=0.;
	    yp0=0.;
	    zp0=1.;
	  }
	  z0=GetZposition();
	  
	  particleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
	  
	  PartMomDir=G4ThreeVector(xp0,yp0,zp0);
	  particleGun->SetParticleMomentumDirection(PartMomDir);
	  
	  particleGun->SetParticleEnergy(E);
	  
	  if(GetPrimaryPolarization().x() || GetPrimaryPolarization().y() || GetPrimaryPolarization().z())
	    pol=GetPrimaryPolarization();
	  else
	    pol=G4ThreeVector(Sx0,Sy0,Sz0);

	  particleGun->SetParticlePolarization(pol);
	  sum_initial_E += E;
	  initial_E=E;
	  
	  //std::cout <<particleGun->GetParticlePosition()/mm << std::endl;
	  //std::cout <<particleGun->GetParticleMomentumDirection() << std::endl;
	  //std::cout <<particleGun->GetParticleTime()/second << std::endl;
	  
	} // End case 2
	  
        
  
      particleGun->GeneratePrimaryVertex(anEvent);
    }
}
