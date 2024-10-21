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
  // Three cases of input: txt, root, anything else
  G4String extension;
  G4bool match_txt = 0, match_root = 0;

  // Txt input
  extension = ".txt";
  // Verifies size is enough
  if (NameFile.size() >= extension.size()) {
    // Compares last characters with extension
    match_txt = (NameFile.compare(NameFile.size() - extension.size(), extension.size(), extension) == 0);
    if (match_txt){ SetInputTxt(NameFile);  }
  }
  
  // Root input
  extension = ".root";
  // Verifies size is enough
  if (NameFile.size() >= extension.size()) {
    // Compares last characters with extension
    match_root = (NameFile.compare(NameFile.size() - extension.size(), extension.size(), extension) == 0);
    if (match_root){ SetInputRoot(NameFile); }
  }
 
  // No input
  if (!match_root && !match_txt) {
    G4cerr << "Error from PPSInput: Extension not recognizable. Exiting\n";
    exit(1);
  }
}

void PPSInput::SetInputTxt(G4String NameFile)
{
  itsName = NameFile;
  input_file.open(NameFile);
  
  if(!input_file.good()){ G4cerr<<"Error from PPSInput: Cannot open input txt file "<< itsName <<G4endl; exit(1);}

  is_txt = 1;
}

// this was the original function before
void PPSInput::SetInputRoot(G4String NameFile)
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
    is_root = 1;
    analysisReader->SetNtupleDColumn("Eg", Eg);
    analysisReader->SetNtupleDColumn("xpos", xpos);
    analysisReader->SetNtupleDColumn("ypos", ypos);
    analysisReader->SetNtupleDColumn("zpos", zpos);
    analysisReader->SetNtupleDColumn("xgp", xp);
    analysisReader->SetNtupleDColumn("ygp", yp);
  }  
  
}


void PPSInput::GetNextParticle(G4double& x0 , G4double& y0 ,  G4double& z0 ,
			       G4double& px0, G4double& py0, G4double& pz0,
			       G4double& Sx0, G4double& Sy0, G4double& Sz0)
{
  if (IsRoot()){
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
  }

  if (IsTxt()){
    input_file >> x0 >> y0 >> px0>> py0 >> pz0; //[mm], [N/A]
    z0 = 0.;
    Sx0 = Sy0 = Sz0 = 0.;
    // Units are in Primary Generator
    // Keep in mind txt has 18m already traveled: CHANGE PPsim.in !!!
    
    //particleGun->SetParticlePosition(G4ThreeVector(srxx*mm,sryy*mm,0*m));
    //particleGun->SetParticleMomentumDirection(G4ThreeVector(srpxx,srpyy,std::sqrt(1-srpxx*srpxx-srpyy*srpyy)));
    //particleGun->SetParticleEnergy(srpzz*GeV);
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

 G4bool PPSInput::IsTxt()
 {return is_txt;}

 G4bool PPSInput::IsRoot()
 {return is_root;}
