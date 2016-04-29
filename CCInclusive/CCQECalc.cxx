#ifndef CCQECALC_CXX
#define CCQECALC_CXX

#include "CCQECalc.h"
namespace larlite {


  double CCQECalc::ComputeECCQE(double totalenergy, const std::vector<double> &lepton_dir, bool is_electron) {

    if ( lepton_dir.size() != 3 ) {
      std::cerr << "From ComputeECCQE: input direction vector doesn't have size 3! Quitting..." << std::endl;
      return -99.;
    }

    double M_n = 939.565;    // MeV/c2
    double M_p = 938.272;    // MeV/c2
    double leptonmass = 0.511;      // MeV/c2
    if (!is_electron) leptonmass = 105.6583;

    double bindingE = 30.0;  // MeV

    double l_energy = totalenergy;
    double l_mom = pow(pow(l_energy, 2) - pow(leptonmass, 2), 0.5);

    // Only truth info goes into theta calculation
    double l_theta =
      TMath::ACos(lepton_dir.at(2) /
                  pow(
                    (
                      pow(lepton_dir.at(0), 2) +
                      pow(lepton_dir.at(1), 2) +
                      pow(lepton_dir.at(2), 2)
                    ), 0.5
                  )
                 );

    double nu_energy_num = pow(M_p, 2) - pow(M_n - bindingE, 2)
                           - pow(leptonmass, 2) + 2.0 * (M_n - bindingE) * l_energy;
    double nu_energy_den = 2.0 * (M_n - bindingE - l_energy + l_mom * TMath::Cos(l_theta));

    // For a result in GEV, divide by 1000.
    return (nu_energy_num / nu_energy_den) / 1000.;

  }

  double CCQECalc::ComputeECCQE(double totalenergy, const TVector3 &lepton_dir, bool is_electron) {

    std::vector<double> temp;
    temp.clear();
    temp.push_back(lepton_dir.X());
    temp.push_back(lepton_dir.Y());
    temp.push_back(lepton_dir.Z());

    return ComputeECCQE(totalenergy, temp, is_electron);

  }
}
#endif
