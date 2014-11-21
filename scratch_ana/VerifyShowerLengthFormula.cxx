#ifndef VERIFYSHOWERLENGTHFORMULA_CXX
#define VERIFYSHOWERLENGTHFORMULA_CXX

#include "VerifyShowerLengthFormula.h"

namespace larlite {

  bool VerifyShowerLengthFormula::initialize() {
    
    _sa = new ShowerAssessment();
    _sa->Configure();


    //Set DistToBoxWall's "box" to be TPC 
    _DistToBoxWall.SetXYZMin( 0,
			      -(::larutil::Geometry::GetME()->DetHalfHeight()),
			      0);
    
    _DistToBoxWall.SetXYZMax( 2*(::larutil::Geometry::GetME()->DetHalfWidth()),
			      ::larutil::Geometry::GetME()->DetHalfHeight(),
			      ::larutil::Geometry::GetME()->DetLength());


    if(!_ana_tree){
      _ana_tree = new TTree("ana_tree","");
      _ana_tree->Branch("MCS_disttowall",&_mcs_dist_to_wall,"MCS_disttowall/D");
      _ana_tree->Branch("MCS_Econtainment",&_mcs_E_containment,"MCS_Econtainment/D");
      _ana_tree->Branch("est_Econtainment",&_est_E_containment,"est_Econtainment/D");
    }

    return true;
  }
  
  bool VerifyShowerLengthFormula::analyze(storage_manager* storage) {
    
    //Read in MCShowers
    auto ev_mcshower = storage->get_data<event_mcshower>("mcreco");    
    if(!ev_mcshower) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, MCShower!"));
      return false;
    }


    //Loop over MCShowers
    for (auto &mcs : *ev_mcshower){

      //_mcs_dist_to_wall = _DistToBoxWall.DistanceToWall(mcs.DetProfile().Position(),mcs.DetProfile().Momentum(),1);
      _mcs_dist_to_wall = _DistToBoxWall.DistanceToWall(mcs.Start().Position(),mcs.Start().Momentum(),1);
      
      //If MCShower originated outside of the TPC, skip this event
      //if(_DistToBoxWall.DistanceToWall(mcs.DetProfile().Position()) < 0) continue;
      if(_DistToBoxWall.DistanceToWall(mcs.Start().Position()) < 0) continue;
      
      _mcs_E_containment = mcs.DetProfile().E()/mcs.Start().E();
      
      //Use _sa to estimate energy containment of MCShower, given it's distance to wall
      _est_E_containment = _sa->EstimateShowerEContainment(_mcs_dist_to_wall);
    
      //Fill ana tree
      _ana_tree->Fill();
    }
    
  
    return true;
  }

  bool VerifyShowerLengthFormula::finalize() {

    if(_fout){
      if(_ana_tree){
	_fout->cd();
	_ana_tree->Write();
	delete _ana_tree;
      }
    }

    delete _sa;

    return true;
  }

}
#endif
