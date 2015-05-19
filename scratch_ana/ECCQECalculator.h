/**
 * \file ECCQECalculator.h
 *
 * \ingroup Utilities
 * 
 * \brief A static function to calculate CCQE energy.
 *
 * @author davidkaleko
 */

/** \addtogroup Utilities

    @{*/
#ifndef LARLITE_ECCQECALCULATOR_H
#define LARLITE_ECCQECALCULATOR_H

#include <vector>
#include <iostream>
#include <math.h> //pow
#include "TMath.h"

/**
   \class ECCQECalculator
   A static function to calculate CCQE energy.
 */
namespace larlite{
  namespace util{

    class ECCQECalculator{
      
    public:
      
      /// Default constructor
      ECCQECalculator(){};
      
      /// Default destructor
      virtual ~ECCQECalculator(){};
      
      /// Method using only truth momentum 4-vector (wrapper)
      /// Momentum vector should be in MeV
      static double ComputeECCQE(const std::vector<double> &lepton_momentum);
      
      /// Method using manually-input energy (IE if you smear energy first)
      /// Energy should be in MeV, momentum in MeV
      static double ComputeECCQE(double energy, const std::vector<double> &lepton_momentum);

    };
  }// end namespace util
}// end namespace ubsens
#endif
/** @} */ // end of doxygen group 

