#ifndef ONLYONEHIGHEMCTRACKFILTER_CXX
#define ONLYONEHIGHEMCTRACKFILTER_CXX

#include "OnlyOneHighEMCTrackFilter.h"
#include "DataFormat/mctrack.h"

namespace larlite {

  bool OnlyOneHighEMCTrackFilter::initialize() {
    
    if(_track_cutoff_energy_MEV < 0){
      print(larlite::msg::kERROR,__FUNCTION__,Form("You need to set the cutoff energy for the OnlyOneHighEMCTrackFilter!"));
      return false;
    }

    return true;
  }
  
  bool OnlyOneHighEMCTrackFilter::analyze(storage_manager* storage) {
 
    //Grab the MCTracks
    auto ev_mctrack = storage->get_data<event_mctrack>("mcreco");    
    if(!ev_mctrack) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctrack!"));
      return false;
    }

    //Make sure there is only one mctrack above some cutoff threshold
    size_t n_high_E_tracks = 0;
    for (auto &track : *ev_mctrack){
      
      double track_kinetic_E = track.MotherStart().Momentum().E()-track.MotherStart().Momentum().M();

      if(track_kinetic_E > _track_cutoff_energy_MEV)
	n_high_E_tracks++;
    }
    
    if(n_high_E_tracks > 1) return false;

    return true;

  }

  bool OnlyOneHighEMCTrackFilter::finalize() {

  
    return true;
  }

}
#endif
