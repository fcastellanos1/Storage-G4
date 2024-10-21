//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PPSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#ifndef PPSInput_h
#define PPSInput_h 1
#include "G4ios.hh"
#include <fstream>
#include "globals.hh"
using namespace std;

class PPSInputMessenger;

class PPSInput
{
public:
  PPSInput();
  ~PPSInput();
  void GetNextParticle(G4double& x0 , G4double& y0 , G4double& z0 ,
		       G4double& px0, G4double& py0, G4double& zp0,
		       G4double& Sx0, G4double& Sy0, G4double& Sz0);
private:
  ifstream input_file;
  G4String itsName;
  G4bool pola_file;
  G4double Eg,xpos,ypos,zpos,xp,yp;
  G4int ntupleId;
  G4bool is_txt = 0;
  G4bool is_root = 0;
public:
  void SetInputBunch(G4String NameFile);
  void SetInputTxt(G4String NameFile);
  void SetInputRoot(G4String NameFile);
  G4bool IsTxt();
  G4bool IsRoot();
  PPSInputMessenger* inputMessenger;
  G4String GetInputName(){return itsName;}
  
};


#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

