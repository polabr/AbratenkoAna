#ifndef ECCQECALCULATOR_CXX
#define ECCQECALCULATOR_CXX

#include "ECCQECalculator.h"
namespace larlite{
  namespace util{

    double ECCQECalculator::ComputeECCQE(const std::vector<double> &lepton_momentum){

      double l_energy = -999.;
      try{l_energy = lepton_momentum.at(3);}
      catch (...) { 
	std::cout<<"Exception within ECCQECalculator. "
		 <<"Did you feed in the right size vector? "
		 <<"Returning -999. CCQE energy to punish your mistakes."
		 << std::endl;
      }

      return ComputeECCQE(l_energy,lepton_momentum);

    }

    double ECCQECalculator::ComputeECCQE(double energy, const std::vector<double> &lepton_momentum){
      
      if ( lepton_momentum.size() != 4 ){
	std::cerr << "From ComputeECCQE: input 4-momentum doesn't have size 4! Quitting..."<<std::endl;
	return -99.;
      }

      ///.at(0) is x momentum in MeV/C
      
      double M_n = 939.565;    // MeV/c2
      double M_p = 938.272;    // MeV/c2
      double M_e = 0.511;      // MeV/c2
      double bindingE = 30.0;  // MeV

      double l_energy = energy;
      double l_mom = pow(pow(l_energy,2) - pow(M_e,2), 0.5);

      // Only truth info goes into theta calculation
      double l_theta = 
	TMath::ACos(lepton_momentum.at(2)/
		    pow(
		      (
			pow(lepton_momentum.at(0),2)+
			pow(lepton_momentum.at(1),2)+
			pow(lepton_momentum.at(2),2)
		      ), 0.5
		    )
	);

      double nu_energy_num = pow(M_p,2) - pow(M_n - bindingE,2)
        - pow(M_e,2) + 2.0*(M_n - bindingE)*l_energy;
      double nu_energy_den = 2.0*(M_n - bindingE - l_energy + l_mom*TMath::Cos(l_theta));
      
      // Return energy in GeV, so divide by 1000
      return (nu_energy_num/nu_energy_den)/1000.;

    }

  }// end namespace util
}// end namespace ubsens  
#endif
