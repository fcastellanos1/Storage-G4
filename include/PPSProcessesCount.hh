//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PPSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#ifndef PPSProcessesCount_HH
#define PPSProcessesCount_HH

#include "globals.hh"
#include <vector>


class OneProcessCount
{
public:
    OneProcessCount(G4String name) {Name=name; Counter=0;};
   ~OneProcessCount() {};
   
public:
    G4String  GetName()       {return Name;};
    G4int     GetCounter()    {return Counter;};        
    void      Count()         {Counter++;};
    
private:
    G4String Name;            // process name
    G4int    Counter;         // process counter
};


typedef std::vector<OneProcessCount*> ProcessesCount;

#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......





