#ifndef SHOWERASSESSMENT_CXX
#define SHOWERASSESSMENT_CXX

#include "ShowerAssessment.h"

bool ShowerAssessment::Configure(){

  //Set DistToBoxWall's "box" to be TPC 
  _DistToBoxWall.SetXYZMin( 0,
			    -(::larutil::Geometry::GetME()->DetHalfHeight()),
			    0);
  
  _DistToBoxWall.SetXYZMax( 2*(::larutil::Geometry::GetME()->DetHalfWidth()),
			    ::larutil::Geometry::GetME()->DetHalfHeight(),
			    ::larutil::Geometry::GetME()->DetLength());
  
  _isConfigured = true;

  return true;

}


bool ShowerAssessment::AreShowersOk(const std::vector< larlite::shower > &showerlist){

  //First make sure the ShowerAssessment module is configured
  if(!_isConfigured) Configure();
  
  //Loop over showers, make sure each is contained, and each has energy > 0
  for (auto const & myshower : showerlist){

    // Make sure shower is contained in active volume
    if( !isFullyContained(myshower) ) return false;

  }
  
  return true;
}


bool ShowerAssessment::isFullyContained(const larlite::shower &thisshower){


  ////////////////////////////////////////////
  // IMPLEMENT THIS FN
  ////////////////////////////////////////////

  //If the shower start point is not in active volume, return false
  if( _DistToBoxWall.DistanceToWall(thisshower.ShowerStart()) < 0 )
    return false;

  return true;
  
}

double ShowerAssessment::EstimateShowerLengthCM(double shower_energy_mev){

  /// This formula taken from Andrzej
  double rad_length_cm = 48.; //Assumed energy independent (roughly correct)
  //this should be 14cm, but studying mcshowers tells me g4 is using 
  //47cm (fit)

  double epsilon_mev = 5715.; //Parameter fit from geant4 output apparently
  
  double shower_length = rad_length_cm * ( log( shower_energy_mev/epsilon_mev ) - 1 + (0.08*18) + 9.6 );

  return shower_length;
}

double ShowerAssessment::EstimateShowerEContainment(double dist_to_wall_cm){

  // This formula taken from my understanding of electromagnetic showers
  // They deposit (1- 1/e) of their energy in each radiation length, X
  // So E(length of shower, d) = E_o * (1-e^(-d/X))
  // This is assuming radiation length is energy independent.
  // Ex: plug in d = 0: No energy deposited
  // Ex: plug in d = inf: All of initial energy E_o is deposited
  // Ex: plug in d = X: All but 1/e of energy E_o is deposited

  double rad_length_cm = 48.; //Assumed energy independent (roughly correct)
  //this should be 14cm, but studying mcshowers tells me g4 is using 
  //47cm (fit)
  
  // If you're outside of the TPC, no energy is contained
  if(dist_to_wall_cm < 0) return 0;

  return (1 - pow(2.71828,(dist_to_wall_cm*(-1)/rad_length_cm)));
}

double ShowerAssessment::EstimateShowerEContainment(const larlite::shower &shower){

  double dist_to_wall = _DistToBoxWall.DistanceToWall(shower.ShowerStart(),shower.Direction(),1);
  
  return EstimateShowerEContainment(dist_to_wall);

}
#endif
