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

    //Grab all track associations
    auto my_ass = storage->get_data<event_ass>(ev_track->name());

    //From track associations, grab the associations to cosmic tags
    //track_to_ctag_ass is a vector of vector of indicies of cosmic tags
    auto const& track_to_ctag_ass = my_ass->association(ev_track->id(),ev_ctag->id());

    //Loop over tracks this event
    for (size_t cos_idx=0; cos_idx<ev_ctag->size(); ++cos_idx){
      
      auto const& this_ctag = ev_ctag->at(cos_idx);

      //If this track has score below cutoff value, skip it
      if ( this_ctag.CosmicScore() < _min_cosmic_score )
	continue;

      //For this ctag, get all track indices that are associated w/ it

      ///////////////////////////////////////
      //
      //this shit doesn't work because i was fixing changes to associations and i forgot
      //what this code was actually doing. -dk, 051815
      //
      //////////////////////////////////////
      //for(auto const& trk_idx : track_to_ctag_ass)
	//	_track_indices.push_back( trk_idx );

    }
  
    return true;
  }

  bool CosmicTrackLister::finalize() {

    return true;
  }

}
#endif
