/**
 * \file NuEnergyCalc.h
 *
 * \ingroup CCInclusive
 * 
 * \brief Class def header for a class NuEnergyCalc
 *
 * @author davidkaleko
 */

/** \addtogroup CCInclusive

    @{*/
#ifndef NUENERGYCALC_H
#define NUENERGYCALC_H


#include <vector>
#include <iostream>
#include <math.h> //pow
#include "TMath.h"
#include "TVector3.h"

/**
   \class NuEnergyCalc
   A class holding functions to calculate neutrino energy. Returns in units of GEV.
 */
namespace larlite{

    class NuEnergyCalc {

    public:

      /// Default constructor
      NuEnergyCalc() {};

      /// Default destructor
      virtual ~NuEnergyCalc() {};

      /// Method using manually-input energy (IE if you smear energy first)
      /// Energy should be in MeV, direction can be (doesn't have to be) unit-normalized
      /// Energy is TOTAL ENERGY (including MASS)
      /// Return value is GEV even though input is MEV
      double ComputeECCQE(double energy, const std::vector<double> &lepton_dir, bool is_electron = true);

      double ComputeECCQE(double energy, const TVector3 &lepton_dir, bool is_electron = true);

      /// Method using 4-momentum conservation for numu interacting on neutron (with unknown magnitude
      /// of fermi momentum), with exiting proton and muon.
      /// Input are muon and proton momentum magnitude and direction
      /// The direction can be unit vector, the momentum magnitude should match
      /// E_tot^2 = sqrt( p^2 + m^2 )
      /// and should be in units of MEV
      /// Return value is GEV even though input is MEV
      double ComputeEnu1mu1p(const TVector3 &mu_dir, double mu_mom_mag_MEV, const TVector3 &p_dir, double p_mom_mag_MEV);


    };

}// end namespace larlite
#endif
/** @} */ // end of doxygen group

