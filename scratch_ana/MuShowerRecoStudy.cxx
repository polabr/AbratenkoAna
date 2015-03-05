#ifndef MUSHOWERRECOSTUDY_CXX
#define MUSHOWERRECOSTUDY_CXX

#include "MuShowerRecoStudy.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/shower.h"

namespace larlite {

  bool MuShowerRecoStudy::initialize() {
    
    if(!_reco_minus_true_showers)
      _reco_minus_true_showers = new TH1F("reco_minus_true_showers","Single Muons: # Reco Showers - # True Showers",5,-2.5,2.5);
    if(!_nrecoshowers_whenNoMCShowers)
      _nrecoshowers_whenNoMCShowers = new TH1F("nrecoshowers_whenNoMCShowers","Single Muons w/ No MCShowers: # Reco Showers;# Reco Showers;Events",5,-2.5,2.5);
    if(!_LoverE_singleshowers)
      _LoverE_singleshowers = new TH1F("LoverE_singleshowers","Shower Length/Energy;Shower Length/Energy [cm/MeV];Counts",100,0,1);

    if(!_mu_nMIDs_vs_mu_E)
      _mu_nMIDs_vs_mu_E = new TH1F("mu_nMIDs_vs_mu_E","Number of Muon's as Showers Per Muon Energy;Muon Energy [MeV];Number of Muon's Recod as Showers",20,0,2000);
    if(!_ana_tree){

      _ana_tree = new TTree("ana_tree","ana_tree");
      _ana_tree->Branch("n_reco_showers",&_n_reco_showers,"n_reco_showers/I");
      _ana_tree->Branch("n_true_showers",&_n_true_showers,"n_true_showers/I");
      _ana_tree->Branch("single_shower_length",&_single_shower_length,"single_shower_length/D");
      _ana_tree->Branch("single_shower_energy",&_single_shower_energy,"single_shower_energy/D");
      _ana_tree->Branch("muon_mcenergy",&_muon_mcenergy,"muon_mcenergy/D");
      }
      

    return true;
  }
  
  bool MuShowerRecoStudy::analyze(storage_manager* storage) {
  
    ClearTTreeVars();

    auto ev_mcs = storage->get_data<event_mcshower>("mcreco");

    if(!ev_mcs){
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, MCShower!"));
      return false;
    }
   
    auto ev_mct = storage->get_data<event_mctrack>("mcreco");
    if(!ev_mct){
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, MCTrack!"));
      return false;
    }

    //auto ev_s = storage->get_data<event_shower>("kalekoreco");
    auto ev_s = storage->get_data<event_shower>("newdefaultreco");

    if(!ev_s){
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, Shower!"));
      return false;
    }

    //Fill ttree variables
    _n_reco_showers = ev_s->size();
    _n_true_showers = ev_mcs->size();
    //when there is one reconstructed shower in the event:
    if(_n_reco_showers == 1){
      auto &show = ev_s->at(0);
      _single_shower_length=show.Length();
      _single_shower_energy=show.MIPEnergy().at(show.best_plane());
      _LoverE_singleshowers->Fill(_single_shower_energy/_single_shower_length);
    }
    if(ev_mct->size() == 1)
      _muon_mcenergy = ev_mct->at(0).at(0).E();

    //Fill ana_tree
    _ana_tree->Fill();

    //Fill extra histograms
    _reco_minus_true_showers->Fill(_n_reco_showers - _n_true_showers);

    //If no mcshowers in the event
    if( !ev_mcs->size() ){
      _nrecoshowers_whenNoMCShowers->Fill(ev_s->size());
      if(_n_reco_showers == 1){
	if(ev_mct->size() != 1){
	  std::cout<<"wtf more than one mctrack here."<<std::endl;
	  return false;}
	_mu_nMIDs_vs_mu_E->Fill(ev_mct->at(0).at(0).E());
      }
    }

    
    return true;
  }

  bool MuShowerRecoStudy::finalize() {

    if(_fout){
      _fout->cd();
      if(_ana_tree)
	_ana_tree->Write();
      if(_reco_minus_true_showers)
	_reco_minus_true_showers->Write();
      if(_nrecoshowers_whenNoMCShowers)
	_nrecoshowers_whenNoMCShowers->Write();
      if(_LoverE_singleshowers)
	_LoverE_singleshowers->Write();
      if(_mu_nMIDs_vs_mu_E)
	_mu_nMIDs_vs_mu_E->Write();
    }
    return true;
  }

  void MuShowerRecoStudy::ClearTTreeVars(){

    _n_reco_showers = 0;
    _n_true_showers = 0;
    _single_shower_length = -999.;
    _single_shower_energy = -999.;
    _muon_mcenergy = -999.;
  }
}
#endif
