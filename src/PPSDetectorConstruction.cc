//local
#include "PPSDetectorConstruction.hh"
#include "PPSDetectorMessenger.hh"

// geant4
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4FieldManager.hh"
#include "G4PVReplica.hh"
#include "G4RunManager.hh"
#include "G4TransportationManager.hh"
#include "G4PropagatorInField.hh"
#include "G4UniformMagField.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4ChordFinder.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnitsTable.hh"
#include "G4PolarizationManager.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

G4double samplerLength = 1*mm;
G4double samplerSize = 20.*cm;

PPSDetectorConstruction::PPSDetectorConstruction():
	pWorld(0), 
	pSampler0(0),pSampler1(0),
	pTarget(0), TargetMaterial(0),
	slices_r(1),slices_z(1),n1(10),n2(9) {
  //Initial parameters 
  worldSize         = 40.*m;
  targetSize        = 2.5*cm;
  targetThickness   = 3.0*mm;
  
  SetTargetMaterial("G4_W"); 
  SetBinPedd(1);
  DefineMaterials();
  
  detectorMessenger = new PPSDetectorMessenger(this);
  
  dividedBoxPhys.resize(n1+n2);
  
}

PPSDetectorConstruction::~PPSDetectorConstruction() { }

void PPSDetectorConstruction::DefineMaterials()
{
  // Vacuum from example novice No3
  //G4double a, z, density;
  //Vacuum = new G4Material("Galactic",z=1.,a=1.01*g/mole,density=universe_mean_density,kStateGas,2.73*kelvin,3.e-18*pascal);
  Tungsten    =   G4NistManager::Instance()->FindOrBuildMaterial("G4_W");
  Copper    =   G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");
  Vacuum      =   G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

}

G4VPhysicalVolume* PPSDetectorConstruction::Construct()
{	
  
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  G4PolarizationManager::GetInstance()->Clean();
  
  // World
  G4Box* sWorld = new G4Box("World",worldSize/2.,worldSize/2.,worldSize/2.);
  G4LogicalVolume* lWorld = new G4LogicalVolume(sWorld,Vacuum,"World");
  pWorld = new G4PVPlacement(0, G4ThreeVector(),lWorld,"World",0,false,0);
  // Helper to store position in Z
  G4double current_position = 0.;

  //
  // Sampler0
  //
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  PPSSamplerSD* SamplerSensDet = new PPSSamplerSD("PPSSamplerSD");
  SDman->AddNewDetector(SamplerSensDet);
  //
  G4VisAttributes* LogVisAttSampler= new G4VisAttributes(G4Colour(1.0,1.0,0.));
  G4ThreeVector positionSampler0= G4ThreeVector(0,0,samplerLength/2.);
  G4Box* solidSampler0 = new G4Box("Sampler0",samplerSize/2.,samplerSize/2.,samplerLength/2.);
  G4LogicalVolume* logicSampler0 = new G4LogicalVolume(solidSampler0,Vacuum,"Sampler0");
  pSampler0 = new G4PVPlacement(0,positionSampler0,logicSampler0,"Sampler0",lWorld,false,0);
  logicSampler0->SetVisAttributes(LogVisAttSampler);
  logicSampler0->SetSensitiveDetector(SamplerSensDet);
  // Update position in z
  current_position += samplerLength;

  //
  // Iron pipe before the glass with vacuum inside
  //
  G4double pipeLength = 10*cm, pipeInnerRadius = 28*mm, pipeOuterRadius = 31*mm;
  G4Tubs* pipeTubs = new G4Tubs("beampipe", pipeInnerRadius, pipeOuterRadius, pipeLength/2., 0., 360.*degree);
  G4ThreeVector positionPipe = G4ThreeVector(0., 0., current_position + pipeLength/2.);
  G4Material* materialPipe = G4NistManager::Instance()->FindOrBuildMaterial((G4String)"G4_Fe");
  G4LogicalVolume* logicPipe = new G4LogicalVolume(pipeTubs, materialPipe, "logicPipe");
  G4VPhysicalVolume* pPipe = new G4PVPlacement(nullptr, positionPipe, logicPipe, "Pipe", lWorld, false, 0);
  // Visualization attributes
  G4VisAttributes * LogVisAttPipe = new G4VisAttributes ( G4Colour(202/255., 98/255., 18/255. ));
  LogVisAttPipe->SetForceSolid(true);
  logicPipe->SetVisAttributes(LogVisAttPipe);
  // Update position in z
  current_position += pipeLength;

  //
  // Glass after the iron pipe
  //
  G4double glassThickness = 1*mm;
  //G4Box* boxGlass = new G4Box("glass", samplerSize/2., samplerSize/2., glassThickness/2.);
  G4Tubs* glassTubs = new G4Tubs("glass", 0., pipeOuterRadius, glassThickness/2., 0., 360.*degree);
  G4ThreeVector positionGlass = G4ThreeVector(0,0, current_position + glassThickness/2.);
  G4Material* materialGlass = G4NistManager::Instance()->FindOrBuildMaterial((G4String)"G4_GLASS_PLATE");
  G4LogicalVolume* logicGlass = new G4LogicalVolume(glassTubs, materialGlass, "logicGlass");
  G4VPhysicalVolume* pGlass = new G4PVPlacement(nullptr, positionGlass, logicGlass, "Glass", lWorld, false, 0);
  // Visualization attributes
  G4VisAttributes * LogVisAttGlass = new G4VisAttributes ( G4Colour(255/255., 255/255., 255/255., 0.5) );
  LogVisAttGlass->SetForceSolid(true);
  logicGlass->SetVisAttributes(LogVisAttGlass);
  // Update position in z
  current_position += glassThickness;
  
  //
  // Medium (air) before converter
  //
  G4double mediumThickness = 4*cm;
  G4Box* boxMedium = new G4Box("medium", samplerSize/2., samplerSize/2., mediumThickness/2.);
  G4ThreeVector positionMedium = G4ThreeVector(0,0,current_position+mediumThickness/2.);
  G4Material* materialMedium = G4NistManager::Instance()->FindOrBuildMaterial((G4String)"G4_AIR");
  G4LogicalVolume* logicMedium = new G4LogicalVolume(boxMedium, materialMedium, "logicMedium");
  G4VPhysicalVolume* pMedium = new G4PVPlacement(nullptr, positionMedium, logicMedium, "Medium", lWorld, false, 0);
  // Update position in z
  current_position += mediumThickness;

  //
  // Target (converter)
  //
  G4ThreeVector positionTarget= G4ThreeVector(0,0,current_position+targetThickness/2);
  G4Box* solidTarget = new G4Box("Target",targetSize/2.,targetSize/2.,targetThickness/2.);
  G4LogicalVolume* logicTarget = new G4LogicalVolume(solidTarget, TargetMaterial,"Target");
  pTarget = new G4PVPlacement(0,positionTarget,logicTarget,"Target",lWorld,false,0);
  G4VisAttributes * LogVisAttTarget       = new G4VisAttributes ( G4Colour(204/255. ,204/255. ,255/255. )); //metal
  LogVisAttTarget->SetForceSolid(true);
  logicTarget->SetVisAttributes(LogVisAttTarget);
  // register logical Volume in PolarizationManager with zero polarization
  G4PolarizationManager * polMgr = G4PolarizationManager::GetInstance();
  polMgr->SetVolumePolarization(logicTarget,G4ThreeVector(0.,0.,0.));
  //Target Calo
  TargetCaloSD= new PPSTargetCaloSD("PPSTargetCaloSD");//,this);
  SDman->AddNewDetector(TargetCaloSD);
  logicTarget->SetSensitiveDetector(TargetCaloSD);
  // Update position in z
  current_position += targetThickness;
  
  //
  // Sampler 1
  //
  G4ThreeVector positionSampler1=G4ThreeVector(0.,0.,current_position+samplerLength/2);
  G4LogicalVolume* logicSampler1 = new G4LogicalVolume(solidSampler0,Vacuum,"Sampler1");
  pSampler1 = new G4PVPlacement(0,positionSampler1,logicSampler1,"Sampler1",lWorld,false,0);
  logicSampler1->SetVisAttributes(LogVisAttSampler);
  logicSampler1->SetSensitiveDetector(SamplerSensDet);
  // Update position in z
  current_position += samplerLength;
  
  //
  // Silicon Detector
  //
  PPSSiliconDetector* siliconDetector = new PPSSiliconDetector("PPSSiliconDetector");
  SDman->AddNewDetector(siliconDetector);
  //
  G4double siThick = 0.02*mm, siSize = targetSize;
  G4ThreeVector posSiDet= G4ThreeVector(0,0,current_position+2*mm+siThick/2.);
  G4Box* solidSiDet = new G4Box("SiDet", siSize/2.,siSize/2.,siThick/2.);
  G4Material* Silicon = G4NistManager::Instance()->FindOrBuildMaterial((G4String)"G4_Si");
  G4LogicalVolume* logicSiDet = new G4LogicalVolume(solidSiDet,Silicon,"SiDet");
  G4VPhysicalVolume* pSiDet = new G4PVPlacement(0,posSiDet,logicSiDet,"SiDet",lWorld,false,0);
  // Visual attributes
  G4VisAttributes* logVisAttSiDet = new G4VisAttributes(G4Colour(0.6,0.6,0.6, 1.0));
  logVisAttSiDet->SetForceSolid(true);
  logicSiDet->SetVisAttributes(logVisAttSiDet);
  logicSiDet->SetSensitiveDetector(siliconDetector);
  // Update position in z
  current_position += siThick;
  
  
  
  PrintParameters();
  //always return the root volume	
  
  return pWorld;
}

void PPSDetectorConstruction::PrintParameters()
{
	G4cout << "###############################################################" << G4endl;
	G4cout << " The Target size"           << G4BestUnit(targetSize,"Length")  << G4endl;
	G4cout << " Thickness "                       << G4BestUnit(targetThickness,"Length")   << G4endl;
	G4cout << " of "                      << TargetMaterial->GetName()          << G4endl;
	G4cout << " Nb of layers in R " << slices_r << G4endl;
	G4cout << " and in Z "                << slices_z                           << G4endl;
	G4cout<< "################################################################" << G4endl;
}

void PPSDetectorConstruction::SetTargetMaterial(G4String materialChoice)
{
	// search the material by its name
	G4Material* mat = G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
	if (mat != TargetMaterial) {
		if(mat) 
		{
			TargetMaterial = mat;
			UpdateGeometry();
		} 
		else 
		{
			G4cout << "### Warning!  Target material: <"<< materialChoice << "> not found" << G4endl;  
			G4cout << " Exit NOW ..." << G4endl;	  
			exit(1);
		}     
	}
}

void PPSDetectorConstruction::SetTargetSize(G4double value)
{
	targetSize = value;
	UpdateGeometry();
}

void PPSDetectorConstruction::SetTargetThickness(G4double value)
{
	targetThickness = value; 
	UpdateGeometry();
}

void PPSDetectorConstruction::SetCellR(G4int value)
{
	slices_r=value;
}

void PPSDetectorConstruction::SetCellZ(G4int value)
{
	slices_z=value;
}

void PPSDetectorConstruction::SetBinPedd(G4int value)
{
	slices_pedd=value;
}

void PPSDetectorConstruction::UpdateGeometry()
{
	if (pWorld) G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}	
