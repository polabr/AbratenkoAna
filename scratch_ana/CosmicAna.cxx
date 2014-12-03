#ifndef COSMICANA_CXX
#define COSMICANA_CXX

#include "CosmicAna.h"

namespace larlite {

  bool CosmicAna::initialize() {
    
    _h_MID = new TH1F("_h_MID","Cosmic Score",11,-0.05,1.05);

    return true;
  }
  
  bool CosmicAna::analyze(storage_manager* storage) {
    
    //Read in cosmic tag shit
    auto ev_ctag = storage->get_data<event_cosmictag>("stitchkalmanhittag");    
    if(!ev_ctag) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, CosmicTag!"));
      return false;
    }

    for (auto const & ctag : *ev_ctag){
      _h_MID->Fill(ctag.CosmicScore());
    }

    return true;
  }

  bool CosmicAna::finalize() {

    if(_fout) { _fout->cd(); _h_MID->Write(); }

    delete _h_MID;

    return true;
  }

}
#endif
