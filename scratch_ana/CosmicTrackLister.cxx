#ifndef COSMICTRACKLISTER_CXX
#define COSMICTRACKLISTER_CXX

#include "CosmicTrackLister.h"
#include "DataFormat/track.h"
#include "DataFormat/cosmictag.h"
namespace larlite {

  bool CosmicTrackLister::initialize() {

    return true;
  }
  
  bool CosmicTrackLister::analyze(storage_manager* storage) {

    _track_indices.clear();

    //Read in cosmic tag info
    auto ev_ctag = storage->get_data<event_cosmictag>(Form("%stag",_track_producer_name.c_str()));    
    if(!ev_ctag) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, CosmicTag made by %stag!",_track_producer_name.c_str()));
      return false;
    }
    
    //Read in the reco track info
    auto ev_track = storage->get_data<event_track>(_track_producer_name.c_str());
    if(!ev_track) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, Track made by %s!",_track_producer_name.c_str()));
      return false;
    }

    //If no tags or tracks, an exception is thrown when you grab association
    //So, let's just quit here if that's the case
    if( !ev_ctag->size() || !ev_track->size() )
      return false;

    //Grab the associations to tracks 
    //(this is a list of track indices that belong to each cosmictag)
    auto my_ass = ev_ctag->association(ev_track->id());

    //Loop over cosmic tag info
    for (size_t cos_idx=0; cos_idx<ev_ctag->size(); ++cos_idx){
      
      auto const& this_ctag = ev_ctag->at(cos_idx);

      //If this track has score below cutoff value, skip it
      if ( this_ctag.CosmicScore() < _min_cosmic_score )
	continue;

      for(auto const& trk_idx : my_ass.at(cos_idx))
	_track_indices.push_back( trk_idx );

    }
  
    return true;
  }

  bool CosmicTrackLister::finalize() {

    return true;
  }

}
#endif
