#ifndef SHOWERRECOSTUDY_CXX
#define SHOWERRECOSTUDY_CXX

#include "ShowerRecoStudy.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/shower.h"
#include "GeoAlgo/GeoVector.h"

namespace larlite {

  bool ShowerRecoStudy::initialize() {

   if(!_ana_tree){

     _ana_tree = new TTree("ana_tree","ana_tree");
     _ana_tree->Branch("n_reco_showers",&_n_reco_showers,"n_reco_showers/I");
     _ana_tree->Branch("n_true_showers",&_n_true_showers,"n_true_showers/I");
     _ana_tree->Branch("reco_show_length",&_reco_show_length,"reco_show_length/D");
     _ana_tree->Branch("true_show_length",&_true_show_length,"true_show_length/D");
     _ana_tree->Branch("prof_show_length",&_prof_show_length,"prof_show_length/D");
     _ana_tree->Branch("reco_show_E",&_reco_show_E,"reco_show_E/D");
     _ana_tree->Branch("true_show_E",&_true_show_E,"true_show_E/D");
     _ana_tree->Branch("st_pt_diff",&_st_pt_diff,"st_pt_diff/D");
     _ana_tree->Branch("E_contained",&_E_contained,"E_contained/O");
     _ana_tree->Branch("reco_show_OA",&_reco_show_OA,"reco_show_OA/D");
     _ana_tree->Branch("prof_show_OA",&_prof_show_OA,"prof_show_OA/D");
     _ana_tree->Branch("3d_anglediff",&_reco_true_3d_anglediff,"3d_anglediff/D");
     _ana_tree->Branch("reco_show_direrr",&_reco_show_direrr,"reco_show_direrr/D");

   }

    return true;
  }
  
  bool ShowerRecoStudy::analyze(storage_manager* storage) {
  
    _n_reco_showers = 0;
    _n_true_showers = 0;
    _reco_show_length = -999.;
    _true_show_length = -999.;
    _prof_show_length = -999.;
    _reco_show_E = -999.;
    _true_show_E = -999.;
    _st_pt_diff = -1.;
    _E_contained = false;
    _reco_show_OA = -999.;
    _prof_show_OA = -999.;
    _reco_true_3d_anglediff = -999.;
    _reco_show_direrr = -999.;


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

    //if(ev_s->size() != 1 || ev_mcs->size() != 1)
    //return false;

    _n_reco_showers = ev_s->size();
    _n_true_showers = ev_mcs->size();

    if(_n_reco_showers == 1){
      _reco_show_length = ev_s->at(0).Length();
      // temporary fix after bug in pitch units was found
      _reco_show_length /= 0.3;
      _reco_show_E = ev_s->at(0).Energy().at(2);
      _reco_show_OA = ev_s->at(0).OpeningAngle();
    }
    if(_n_true_showers == 1){
      _true_show_length = geoalgo::Vector(ev_mcs->at(0).End().Position()).Dist(geoalgo::Vector(ev_mcs->at(0).Start().Position()) );
      _true_show_E = ev_mcs->at(0).DetProfile().E();
      _prof_show_length = _shrProfiler.Length(_true_show_E);
      _prof_show_OA = _shrProfiler.OpeningAngle(_true_show_E);
      _E_contained = ( ( ev_mcs->at(0).DetProfile().E() / ev_mcs->at(0).Start().E() ) > 0.95 ) ? true : false;
    }
    if(_n_reco_showers == 1 && _n_true_showers == 1 ){
      _st_pt_diff = geoalgo::Vector(ev_mcs->at(0).Start().Position()).Dist(geoalgo::Vector(ev_s->at(0).ShowerStart()));
      _reco_true_3d_anglediff = geoalgo::Vector(ev_mcs->at(0).Start().Momentum().Vect()).Angle(geoalgo::Vector(ev_s->at(0).Direction()));
      _reco_show_direrr = ev_s->at(0).DirectionErr().X();
    }
    
    _ana_tree->Fill();

    return true;
  }

  bool ShowerRecoStudy::finalize() {

    if(_fout){
      _fout->cd();
      if(_ana_tree)
	_ana_tree->Write();

    }  
    return true;
  }

}
#endif
