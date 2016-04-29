/**
 * \file CCQECalc.h
 *
 * \ingroup CCInclusive
 * 
 * \brief Class def header for a class CCQECalc
 *
 * @author davidkaleko
 */

/** \addtogroup CCInclusive

    @{*/
#ifndef CCQECALC_H
#define CCQECALC_H


#include <vector>
#include <iostream>
#include <math.h> //pow
#include "TMath.h"
#include "TVector3.h"

/**
   \class CCQECalc
   A static function to calculate CCQE energy. Returns in units of GEV.
 */
namespace larlite{

    class CCQECalc {

    public:

      /// Default constructor
      CCQECalc() {};

      /// Default destructor
      virtual ~CCQECalc() {};

      /// Method using manually-input energy (IE if you smear energy first)
      /// Energy should be in MeV, direction can be (doesn't have to be) unit-normalized
      double ComputeECCQE(double energy, const std::vector<double> &lepton_dir, bool is_electron = true);

      double ComputeECCQE(double energy, const TVector3 &lepton_dir, bool is_electron = true);


    };

}// end namespace larlite
#endif
/** @} */ // end of doxygen group

