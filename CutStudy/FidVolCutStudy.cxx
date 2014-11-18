#ifndef FIDVOLCUTSTUDY_CXX
#define FIDVOLCUTSTUDY_CXX

#include "FidVolCutStudy.h"

namespace larlite {

  bool FidVolCutStudy::initialize() {
    //Set DistToBoxWall's "box" to be TPC 
    _DistToBoxWall.SetXYZMin( 0,
			      -(::larutil::Geometry::GetME()->DetHalfHeight()),
			      0);
    
    _DistToBoxWall.SetXYZMax( 2*(::larutil::Geometry::GetME()->DetHalfWidth()),
			      ::larutil::Geometry::GetME()->DetHalfHeight(),
			      ::larutil::Geometry::GetME()->DetLength());

    //Initialize _ana_tree
    if(!_ana_tree){
      _ana_tree = new TTree("ana_tree","");
      _ana_tree->Branch("DistVtxToWall",&_vtx_to_wall_dist,"DistVtxToWall/D");
      _ana_tree->Branch("EContainment",&_E_containment,"EContainment/D");
      _ana_tree->Branch("FwdToWallDist",&_fwd_to_wall_dist,"FwdToWallDist/D");
      _ana_tree->Branch("EMother",&_E_mother,"EMother/D");
    }
    
    return true;
  }
  
  bool FidVolCutStudy::analyze(storage_manager* storage) {

    //Read in MCShowers
    auto ev_mcshower = storage->get_data<event_mcshower>("mcreco");    
    if(!ev_mcshower) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, MCShower!"));
      return false;
    }

    //Read in other shit



    //Loop over MCShowers
    for( auto const & mcs : *ev_mcshower) {
      
      auto total_shower_E = mcs.MotherStart().E();
      auto deposited_E = mcs.DetProfile().E();
      auto shower_start_vtx = mcs.DetProfile().Position();
      auto shower_direction = mcs.DetProfile().Momentum();
    
      _vtx_to_wall_dist = _DistToBoxWall.DistanceToWall(shower_start_vtx);
      _E_containment = deposited_E/total_shower_E;
      _fwd_to_wall_dist = _DistToBoxWall.DistanceToWall(shower_start_vtx,
							shower_direction,
							true);
      _E_mother = total_shower_E;

      if (_ana_tree)
	  _ana_tree->Fill();
    }

    return true;
  }

  bool FidVolCutStudy::finalize() {

    if(_fout) {
      _fout->cd();

      if(_ana_tree)
	_ana_tree->Write();
    }

    delete _ana_tree;

    return true;
  }

}
#endif
