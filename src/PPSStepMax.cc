#include "PPSStepMax.hh"
#include "PPSStepMaxMessenger.hh"

PPSStepMax::PPSStepMax(const G4String& processName)
 : G4VDiscreteProcess(processName),MaxChargedStep(DBL_MAX)
{
  pMess = new PPSStepMaxMessenger(this);
}
 
PPSStepMax::~PPSStepMax() { delete pMess; }

G4bool PPSStepMax::IsApplicable(const G4ParticleDefinition& particle) 
{ 
  return (particle.GetPDGCharge() != 0.);
}

void PPSStepMax::SetMaxStep(G4double step) {MaxChargedStep = step;}

G4double PPSStepMax::PostStepGetPhysicalInteractionLength(const G4Track& aTrack,
                                                  G4double,
                                                  G4ForceCondition* condition )
{
  // condition is set to "Not Forced"
  *condition = NotForced;
  
  G4double ProposedStep = DBL_MAX;

  if((MaxChargedStep > 0.) &&
     (aTrack.GetVolume() != 0) &&
     (aTrack.GetVolume()->GetName() != "World"))
     ProposedStep = MaxChargedStep;

  return ProposedStep;
}

G4VParticleChange* PPSStepMax::PostStepDoIt(const G4Track& aTrack, const G4Step&)
{
   // do nothing
   aParticleChange.Initialize(aTrack);
   return &aParticleChange;
}

