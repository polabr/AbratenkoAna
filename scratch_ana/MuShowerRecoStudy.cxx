#ifndef MUSHOWERRECOSTUDY_CXX
#define MUSHOWERRECOSTUDY_CXX

#include "MuShowerRecoStudy.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/shower.h"

namespace larlite {

  bool MuShowerRecoStudy::initialize() {
    
    if(!_reco_minus_true_showers)
      _reco_minus_true_showers = new TH1F("reco_minus_true_showers","Single Muons: # Reco Showers - # True Showers",5,-2.5,2.5);
 if(!_nrecoshowers_whenNoMCShowers)
      _nrecoshowers_whenNoMCShowers = new TH1F("nrecoshowers_whenNoMCShowers","Single Muons w/ No MCShowers: # Reco Showers;# Reco Showers;Events",5,-2.5,2.5);

    return true;
  }
  
  bool MuShowerRecoStudy::analyze(storage_manager* storage) {
  
    auto ev_mcs = storage->get_data<event_mcshower>("mcreco");

    if(!ev_mcs){
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, MCShower!"));
      return false;
    }

    auto ev_s = storage->get_data<event_shower>("showerreco");

    if(!ev_s){
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, Shower!"));
      return false;
    }

    _reco_minus_true_showers->Fill(ev_s->size() - ev_mcs->size());
    if( !ev_mcs->size() )
      _nrecoshowers_whenNoMCShowers->Fill(ev_s->size());

    return true;
  }

  bool MuShowerRecoStudy::finalize() {

    if(_fout){
      _fout->cd();
      if(_reco_minus_true_showers)
	_reco_minus_true_showers->Write();
      if(_nrecoshowers_whenNoMCShowers)
	_nrecoshowers_whenNoMCShowers->Write();
    }
    return true;
  }

}
#endif
