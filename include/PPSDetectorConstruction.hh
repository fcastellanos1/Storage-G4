#ifndef PPSDetectorConstruction_h
#define PPSDetectorConstruction_h 1

//local includes
#include "PPSSamplerSD.hh"
#include "PPSSiliconDetector.hh"
#include "PPSTargetCaloSD.hh"

// G4 includes
#include "G4VUserDetectorConstruction.hh"
#include "G4UserLimits.hh"
#include "globals.hh"
#include "G4EqMagElectricField.hh"
#include "G4PropagatorInField.hh"
#include "G4TransportationManager.hh"
#include "G4ChordFinder.hh"
#include "G4ClassicalRK4.hh"

class G4LogicalVolume;
class G4Material;
class G4UserLimits;
class PPSDetectorMessenger;
class PPSCapture;
class PPSSamplerSD;
class PPSSiliconDetector;
class PPSTargetCaloSD;

class PPSDetectorConstruction : public G4VUserDetectorConstruction
{

	public:
		PPSDetectorConstruction();
		~PPSDetectorConstruction();
		G4VPhysicalVolume* Construct();
		void DefineMaterials(); 	  
		void UpdateGeometry();
		void PrintParameters();

	private:
		PPSDetectorMessenger*   detectorMessenger;
  
		//World 
	public: 
		const G4VPhysicalVolume*    GetWorld()      			{return pWorld;}
		G4double                    GetWorldSize()  			{return worldSize;}  
		G4double                    GetTargetSize()  			{return targetSize;}
		G4double                    GetTargetThickness()  {return targetThickness;}
		G4Material*                 GetTargetMaterial()   {return TargetMaterial;}


	private:
		G4VPhysicalVolume*      pWorld;  
		G4double                worldSize;
		
		//Sampler  
	public:
		const G4VPhysicalVolume* GetSampler1()    	{return pSampler1;}
		const G4VPhysicalVolume* GetSampler0()    	{return pSampler0;}

	private:
  		//PPSSamplerSD*           SamplerSensDet;
		//PPSSiliconDetector*   siliconDetector;
		G4VPhysicalVolume*      pSampler0;
		G4VPhysicalVolume*      pSampler1;

		//Target
	public: 
		void SetTargetSize(G4double);
		void SetTargetThickness(G4double);              
		void SetCellR(G4int);
		void SetCellZ(G4int);
		void SetBinPedd(G4int);
		void SetTargetMaterial(G4String);            
		const G4VPhysicalVolume*  GetTarget()     			{return pTarget;}
		G4int                       GetSlicesR()     		    	{return slices_r;}
		G4int                       GetSlicesZ()            		{return slices_z;}
		G4int                       GetBinPedd()            		{return slices_pedd;}

	private:
		PPSTargetCaloSD* 	TargetCaloSD;
		G4VPhysicalVolume*      pTarget;
		G4Material*             TargetMaterial;
		G4Material*             Vacuum;
		G4Material*             Tungsten;
		G4Material*             Copper;
		G4double                targetSize;
		G4double                targetThickness;
		G4int                   slices_r;
		G4int                   slices_z;
		G4int                   slices_pedd;

		typedef std::vector<G4VPhysicalVolume*> VectorMotherSphere;
		VectorMotherSphere dividedBoxPhys;
		typedef std::map<G4String, G4ThreeVector> MapBullet;
		MapBullet bullet;	
		G4int n1;
		G4int n2;
	public:    
		G4double length_radius[2];
		
		MapBullet GetMapBullet() {return bullet;}

};
#endif
