#ifndef TRACKASSESSMENT_CXX
#define TRACKASSESSMENT_CXX

#include "TrackAssessment.h"


bool TrackAssessment::Configure(){
  
  //Set DistToBoxWall's "box" to be TPC 
  _myGeoAABox.Min( 0,
		   -(::larutil::Geometry::GetME()->DetHalfHeight()),
		   0);
  
  _myGeoAABox.Max( 2*(::larutil::Geometry::GetME()->DetHalfWidth()),
		   ::larutil::Geometry::GetME()->DetHalfHeight(),
		   ::larutil::Geometry::GetME()->DetLength());
  
  
  _isConfigured = true;
  
  return true;
  
}


bool TrackAssessment::AreTracksOk(const std::vector< std::pair<larlite::track,double> > &tracklist){

  //First make sure the TrackAssessment module is configured
  if(!_isConfigured) Configure();
  
  //Loop over tracks, make sure each is contained, and each has energy > 0
  for (auto const & mypair : tracklist){

    // Make sure track's energy is positive
    if(mypair.second < 0){
      std::cerr<<"This track has negative energy. Tracks are not OK."<<std::endl;
      return false;
    }

    // Make sure track is contained in active volume
    if( !isFullyContained(mypair.first) ) return false;

  }
  
  return true;
}


bool TrackAssessment::isFullyContained(const larlite::track &thistrack){
   
  auto start = thistrack.vertex_at(0);
  auto end = thistrack.vertex_at(thistrack.n_point());

  //Make sure track start point and end point are in active volume
  if( _myGeoAABox.Contain(start) && _myGeoAABox.Contain(end) )
    return true;
  
  return false;
  
}








#endif


