#include "PPSTargetCaloSD.hh"
#include "PPSTargetCaloHit.hh"
//#include "PPSOutput.hh"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "Randomize.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"


#define max(A,B) ((A>B)? A:B)
//extern PPSOutput rootOutput;

PPSTargetCaloSD::PPSTargetCaloSD(G4String name)
:G4VSensitiveDetector(name),edep(-1.)
{
	G4String HCname;
	collectionName.insert(HCname="TargetCaloCollection");
}

PPSTargetCaloSD::~PPSTargetCaloSD()
{
	//G4cout << " PPSTargetSD ended  dim target (r,z)[mm]=(" << rootOutput.dim_target[0] << "," << rootOutput.dim_target[1] <<")" << G4endl;
}

void PPSTargetCaloSD::Initialize(G4HCofThisEvent*HCE)
{
	PPSTargetCaloCollection = new PPSTargetCaloHitsCollection
    (SensitiveDetectorName,collectionName[0]);
}

G4bool PPSTargetCaloSD::ProcessHits(G4Step*aStep,G4TouchableHistory* /*ROhist*/)
{
	G4Track* theTrack=aStep->GetTrack();
	//G4double 
	edep = aStep->GetTotalEnergyDeposit();
    
	if ( edep == 0. ) return false;
	if(theTrack->GetDefinition()->GetParticleName() != "geantino" && edep==0.) return true;
    
	G4ThreeVector prePoint  = aStep->GetPreStepPoint() ->GetPosition();
	G4ThreeVector postPoint = aStep->GetPostStepPoint()->GetPosition();
	G4ThreeVector pos = prePoint + G4UniformRand()*(postPoint - prePoint);
    
    
	G4double xpos=pos.x()/mm;
	G4double ypos=pos.y()/mm;
	G4double rpos = sqrt(xpos*xpos+ypos*ypos);
	G4double zpos=pos.z()/mm;
	edep=edep/MeV;
    
	PPSTargetCaloHit* newHit = new PPSTargetCaloHit(xpos,ypos,zpos,edep,zpos*edep,rpos*edep);
	PPSTargetCaloCollection->insert( newHit );
    
  G4double rmin =   0.;
  //G4double rmax =   rootOutput.dim_target[0]/mm;
	G4double zmin =   0.;
  //G4double zmax =   rootOutput.dim_target[1]/mm;
    
 /* G4int Nr = rootOutput.nb_slices[0];
  G4int Nz = rootOutput.nb_slices[1];
  G4int Nx = rootOutput.nb_slices_pedd;
    
    // Trick taken from examples/advanced/purging_magnet/include/PurgMagTabulatedField3D.hh
    double d_bin_r, d_bin_z, d_bin_x;
    double r_dummy = ( std::modf(Nr*rpos/rmax, &d_bin_r));
    double z_dummy = ( std::modf(Nz*zpos/zmax, &d_bin_z));
    double x_dummy = ( std::modf((xpos+rmax)*(Nx)/(2*rmax), &d_bin_x));

    int bin_r = static_cast<int>(d_bin_r);
    int bin_z = static_cast<int>(d_bin_z);
    int bin_x = static_cast<int>(d_bin_x);
    //int bin_event = rootOutput.eventNb;
	
    
    if(edep>0.)
	{
            
        if(bin_r >= rootOutput.nb_slices[0])
        {
            G4cerr << " Oups Binning problem on r (see PPSTargetSD) ... but I will continue " << G4endl;
            bin_r = rootOutput.nb_slices[0]-1;
        }   
        
        if(bin_z >= rootOutput.nb_slices[1])
        {
            G4cerr << " Oups Binning problem on z (see PPSTargetSD) ... but I will continue " << G4endl;
            bin_z = rootOutput.nb_slices[1]-1;
        } 
        
        if(bin_x >= rootOutput.nb_slices_pedd)
        {
            G4cerr << " Oups Binning problem on x (see PPSTargetSD) ... but I will continue " << G4endl;
            bin_x = rootOutput.nb_slices_pedd-1;
        } 
        
	rootOutput.SetVectorEnergy(bin_z,bin_r,edep);

    } 
    */
	return true;
}

void PPSTargetCaloSD::EndOfEvent(G4HCofThisEvent* HCE)
{       
	G4SDManager * SDman = G4SDManager::GetSDMpointer();
	HCID = SDman->GetCollectionID(collectionName[0]);
	HCE->AddHitsCollection( HCID, PPSTargetCaloCollection );
}

