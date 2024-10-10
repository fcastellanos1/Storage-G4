#include "PPSInput.hh"
#include "PPSInputMessenger.hh"
#include "globals.hh"

#include "G4RootAnalysisReader.hh"

using G4AnalysisReader = G4RootAnalysisReader;



PPSInput::PPSInput():itsName("")
		    ,pola_file(false)
{
  inputMessenger = new PPSInputMessenger(this);
}

PPSInput::~PPSInput()
{
  input_file.close();
}

void PPSInput::SetInputBunch(G4String NameFile)
{
  
  // Create (or get) analysis reader
  auto analysisReader = G4AnalysisReader::Instance();
  analysisReader->SetVerboseLevel(1);

  // Define a base file name
  analysisReader->SetFileName(NameFile);

  // Read ntuple
  ntupleId = analysisReader->GetNtuple("mc");;
  if ( ntupleId < 0 ) {
    G4cerr << "ntuple not found" << G4endl;
  }
  else {
    itsName=NameFile;
    analysisReader->SetNtupleDColumn("Eg", Eg);
    analysisReader->SetNtupleDColumn("xpos", xpos);
    analysisReader->SetNtupleDColumn("ypos", ypos);
    analysisReader->SetNtupleDColumn("zpos", zpos);
    analysisReader->SetNtupleDColumn("xgp", xp);
    analysisReader->SetNtupleDColumn("ygp", yp);
  }
  
  /*itsName = NameFile;
  G4String real_name_file;
  //Due to "/" pb POLA_ can not be found so I made this trick
  size_t found=itsName.find_last_of("/\\");
  real_name_file=itsName.substr(found+1);
  G4cout << itsName << " " << real_name_file << G4endl;
  input_file.open(itsName);
  if(!real_name_file.find("POLA_"))
    { 
      G4cout << "Input file polarized" << G4endl;
      pola_file = true;
    }
  else G4cout <<  " Input file not polarized"<< G4endl; 
  
  if(!input_file.good()){ G4cerr<<"Cannot open bunch file "<< itsName <<G4endl; exit(1);}*/
  
  
}

void PPSInput::GetNextParticle(G4double& x0 , G4double& y0 ,  G4double& z0 ,
			       G4double& px0, G4double& py0, G4double& pz0,
			       G4double& Sx0, G4double& Sy0, G4double& Sz0)
{
  
  auto analysisReader = G4AnalysisReader::Instance();
  
  if ( analysisReader->GetNtupleRow(ntupleId) ) {
     // G4cout << "th entry: "
     //        << "  Eg: " << Eg << G4endl;
    
    x0=xpos;
    y0=ypos;
    z0=zpos;
    pz0=Eg;
    px0=xp*pz0;
    py0=yp*pz0;
    Sx0 = Sy0 = Sz0 = 0.;
  }
  
  
/*#define _READ(value) input_file>>value
  
	if(_READ(x0))
	{
		_READ(y0);
		_READ(px0);
		_READ(py0);
		_READ(pz0);
		if(pola_file==true)
		{
			if(_READ(Sx0))
			{
				_READ(Sy0);
				_READ(Sz0);
			}
		}
		else
		{
			Sx0=0.;
			Sy0=0.;
			Sz0=0.;	  
		}
	}*/
}		  
