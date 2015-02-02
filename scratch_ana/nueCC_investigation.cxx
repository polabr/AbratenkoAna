#ifndef NUECC_INVESTIGATION_CXX
#define NUECC_INVESTIGATION_CXX

#include "nueCC_investigation.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/mctruth.h"

namespace larlite {

  bool nueCC_investigation::initialize() {

    _ana_tree = new TTree("ana_tree","ana_tree");
    _ana_tree->Branch("is_nueCC",&_is_nueCC,"is_nueCC/O");
    _ana_tree->Branch("n_mcshower_over50MEV",&_n_mcshowers_over_50MEV,"n_mcshower_over50MEV/I");
    _ana_tree->Branch("is_biggest_mcs_e_shower",&_is_electron_shower,"is_biggest_mcs_e_shower/O");
    _ana_tree->Branch("max_mcs_energy",&_max_mcs_energy,"max_mcs_energy/D");

    return true;
  }
  
  bool nueCC_investigation::analyze(storage_manager* storage) {
  
    //Grab the MCTruth
    auto ev_mctruth = storage->get_data<event_mctruth>("generator");
    if(!ev_mctruth) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctruth!"));
      return false;
    }

    //Figure out whether the interaction is nueCC
    _is_nueCC = 
      (
	ev_mctruth->at(0).GetNeutrino().CCNC()==0 &&
	abs(ev_mctruth->at(0).GetNeutrino().Nu().PdgCode())==12 
      ) 
      ? true : false;
   
   
    //Grab the MCShowers
    auto ev_mcshower = storage->get_data<event_mcshower>("mcreco");    
    if(!ev_mcshower) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, MCShower!"));
      return false;
    }


    //Initialize some variables before looping over MCShowers
    _max_mcs_energy = 0;
    _n_mcshowers_over_50MEV = 0;
    _is_electron_shower = false;

    //Loop over MCShowers
    for (auto const& mcs : *ev_mcshower){

      //Count MCShowers with energy over 50 MEV
      if(mcs.DetProfile().E() > 50.)
	_n_mcshowers_over_50MEV++;

      //Find the biggest MCShower energy
      if(mcs.DetProfile().E() > _max_mcs_energy){
	_max_mcs_energy = mcs.DetProfile().E();

	//Ask if biggest mcshower is created by an electron
	_is_electron_shower = (mcs.PdgCode() == 11) ? true : false;
      }      
     
    }






    //Fill the ana tree
    if(_ana_tree) _ana_tree->Fill();
    

    return true;
  }

  bool nueCC_investigation::finalize() {

    if(_fout) { 
      _fout->cd(); 
      _ana_tree->Write(); 
    }
    
    if(_ana_tree)
      delete _ana_tree;
  
    return true;
  }

}
#endif
