/**
 * \file TrackAssessment.h
 *
 * \ingroup SignalSelection
 * 
 * \brief Class def header for a class TrackAssessment
 *
 * @author davidkaleko
 */

/** \addtogroup SignalSelection

    @{*/
#ifndef TRACKASSESSMENT_H
#define TRACKASSESSMENT_H

#include <iostream>
#include "DataFormat/track.h"
#include "BasicTool/GeoAlgo/GeoAlgo.h"
#include "LArUtil/Geometry.h"
#include "TVector3.h"

/**
   \class TrackAssessment
   This class is meant to determine whether a set of tracks are well defined. 
   It takes in a list of pairs of <track (data product), Energy>
   (which may correspond to tracks in one event, or tracks from a longer continuous data stream), 
   and it returns whether these tracks are good.
 */
class TrackAssessment{

public:

  /// Default constructor
  TrackAssessment(){ _isConfigured = false; };

  /// Default destructor
  virtual ~TrackAssessment(){};

  bool Configure();

  bool AreTracksOk(const std::vector< std::pair<larlite::track,double> > &tracklist);
  
protected:

  bool isFullyContained(const larlite::track &thistrack);
  
  bool _isConfigured;
  
  //Geometry algo instance
  geoalgo::AABox _myGeoAABox;

};

#endif
/** @} */ // end of doxygen group 

