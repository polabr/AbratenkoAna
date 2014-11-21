/**
 * \file ShowerAssessment.h
 *
 * \ingroup SignalSelection
 * 
 * \brief Class def header for a class ShowerAssessment
 *
 * @author davidkaleko
 */

/** \addtogroup SignalSelection

    @{*/
#ifndef SHOWERASSESSMENT_H
#define SHOWERASSESSMENT_H

#include <iostream>
#include "DataFormat/shower.h"
#include "BasicTool/GeoAlgo/DistToBoxWall.h"
#include "LArUtil/Geometry.h"

/**
   \class ShowerAssessment
   This class is meant to determine whether a set of showers are well defined. 
   It takes in a list of showers (data product)
   (which may correspond to showers in one event, or showers from a longer continuous data stream), 
   and it returns whether these showers are good.
 */
class ShowerAssessment{

public:

  /// Default constructor
  ShowerAssessment(){};

  /// Default destructor
  virtual ~ShowerAssessment(){};

  bool Configure();

  bool AreShowersOk(const std::vector< larlite::shower > &showerlist);
  
  double EstimateShowerLengthCM(double shower_energy_MEV);

  double EstimateShowerEContainment(double dist_to_wall_cm);

  double EstimateShowerEContainment(const larlite::shower &thisshower);

protected:

  bool isFullyContained(const larlite::shower &thisshower);

  bool _isConfigured;
  
  //Geometry algo instance
  ::geoalgo::DistToBoxWall _DistToBoxWall;

};

#endif
/** @} */ // end of doxygen group 

