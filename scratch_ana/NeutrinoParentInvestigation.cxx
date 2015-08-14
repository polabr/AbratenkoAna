#ifndef LARLITE_NEUTRINOPARENTINVESIGATION_CXX
#define LARLITE_NEUTRINOPARENTINVESIGATION_CXX

#include "NeutrinoParentInvestigation.h"
#include "DataFormat/mcflux.h"

namespace larlite {

  bool NeutrinoParentInvestigation::initialize() {

    if (!_nu_decay_code_hist)
      _nu_decay_code_hist = new TH1I("nu_decay_modes","Neutrino Decay Modes",14,0.5,14.5);

    return true;
  }
  
  bool NeutrinoParentInvestigation::analyze(storage_manager* storage) {
  
  
    auto ev_mcflux = storage->get_data<event_mcflux>("generator");

    if(!ev_mcflux){
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, MCFlux!"));
      return false;
    }
   
    auto my_flux = ev_mcflux->at(0);

    _nu_decay_code_hist->Fill(my_flux.fndecay);

     
    return true;
  }

  bool NeutrinoParentInvestigation::finalize() {

    if(_fout){
      _fout->cd();
      if(_nu_decay_code_hist)
	_nu_decay_code_hist->Write();
    }
  
    return true;
  }

}
#endif
