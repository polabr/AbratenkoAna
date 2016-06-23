#ifndef LARLITE_TESTMULTISCATTERMOMENTUM_CXX
#define LARLITE_TESTMULTISCATTERMOMENTUM_CXX

#include "TestMultiScatterMomentum.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/mctruth.h"

namespace larlite {

  bool TestMultiScatterMomentum::initialize() {

    _tmc = TrackMomentumCalculator();

    if (!_ana_tree) {
      _ana_tree = new TTree("ana_tree", "ana_tree");
      _ana_tree->Branch("true_mom", &_true_mom, "true_mom/D");
      _ana_tree->Branch("mcs_reco_mom", &_mcs_reco_mom, "mcs_reco_mom/D");
      _ana_tree->Branch("true_len", &_true_length, "true_len/D");
      _ana_tree->Branch("reco_len", &_reco_length, "reco_len/D");
      _ana_tree->Branch("mu_contained", &_mu_contained, "mu_contained/O");
    }

    double fidvol_dist = 5.;
    double fidvol_dist_y = 5.;
    
    //Box here is TPC
    _fidvolBox.Min( 0 + fidvol_dist,
		    -(::larutil::Geometry::GetME()->DetHalfHeight()) + fidvol_dist_y,
		    0 + fidvol_dist);

    _fidvolBox.Max( 2 * (::larutil::Geometry::GetME()->DetHalfWidth()) - fidvol_dist,
		    ::larutil::Geometry::GetME()->DetHalfHeight() - fidvol_dist_y,
		    ::larutil::Geometry::GetME()->DetLength() - fidvol_dist);
    
    return true;
  }

  bool TestMultiScatterMomentum::analyze(storage_manager* storage) {
    
    _true_mom = -999.;
    _mcs_reco_mom = -999.;
    _true_length = -999.;
    _reco_length = -999.;
    _mu_contained = false;

    std::cout << "Starting mc truth now" << "\n";

    // Get neutrino interaction vertex
    auto ev_mctruth=storage->get_data<event_mctruth>("generator");

    if(ev_mctruth->size() != 1) 
      return false;
    
    auto mctruth=ev_mctruth->at(0);
    
    auto const& nu_vtx=mctruth.GetNeutrino().Nu().Trajectory().front().Position().Vect();
    
    if (_fidvolBox.Contain(nu_vtx)){
      
      std::cout << "Neutrino vertex contained!" << "\n";
      
      std::cout << "The x coordinate of the nu vertex is located at: " << mctruth.GetNeutrino().Nu().Trajectory().front().X() << ".\n";
      std::cout << "The y coordinate of the nu vertex is located at: " << mctruth.GetNeutrino().Nu().Trajectory().front().Y() << ".\n";
      std::cout << "The z coordinate of the nu vertex is located at: " << mctruth.GetNeutrino().Nu().Trajectory().front().Z() << ".\n";	
      
    } else {
      print(larlite::msg::kERROR, __FUNCTION__, Form("NEUTRINO VERTEX NOT CONTAINED"));
      return false;
    }

    //Get the MCTracks
    auto ev_mctrack = storage->get_data<event_mctrack>("mcreco");
    
    if (!ev_mctrack) {
      print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, mctrack!"));
      return false;
    }

    std::cout << "I see " << ev_mctrack->size() << " mctracks in this event\n";

    // Narrowing down the number of mctracks
    for (size_t i=0; i<ev_mctrack->size(); i++) {
      
      auto mctrack=ev_mctrack->at(i);
      
      if(mctrack.PdgCode()==13 && mctrack.Origin()==1) {
	
	std::cout<<"On mctrack " << i << ", and the particle's mother PDG is "<<mctrack.MotherPdgCode()<< ", the PDG code is " << mctrack.PdgCode() << ", and the ancestor PDG is "<<mctrack.AncestorPdgCode() <<"\n";
	std::cout << "The origin is: " << mctrack.Origin() << ".\n";
	
	if (mctrack.size()!=0) {
	  
	  std::cout << "The x coordinate of the starting point of the muon is: " << mctrack.front().X() << ".\n"; 
	  std::cout << "The y coordinate of the starting point of the muon is: " << mctrack.front().Y() << ".\n"; 
	  std::cout << "The z coordinate of the starting point of the muon is: " << mctrack.front().Z() << ".\n"; 

	} else {
	  print(larlite::msg::kERROR, __FUNCTION__, Form("mctrack.size() was 0 here!"));
	  return false;
	}
	
	auto const& mct_start = mctrack.front().Position().Vect();
	
	double dist=(nu_vtx-mct_start).Mag();
	
	std::cout << "This is the difference between coordinates: " << dist << "\n";

	if (dist>0.001) {
	  
	  print(larlite::msg::kERROR, __FUNCTION__, Form("Distance was too large, skipping this mctrack..."));
	  return false;
	  
	}

	// Put here?
	//	  W00T, next step! This is where another if statement for sorting the mctracks would go, 
	//        after sifting through only PGD=13 and Origin=1
	
	// Extract MC TTree info from the one MCTrack  

	_true_mom=mctrack.front().Momentum().Vect().Mag()/1000.;

	_true_length=(mctrack.End().Position().Vect()-mctrack.Start().Position().Vect()).Mag(); 

	if (_using_mctracks)                                                                                    
	  _mcs_reco_mom = _tmc.GetMomentumMultiScatterLLHD(mctrack);                                                
	std::cout << "Everything went ok so far. Now onto the Reco Tracks." << "\n";    

	if (!_using_mctracks) {
	  
	  //Get the Reco Tracks
	  auto ev_track=storage->get_data<event_track>("pandoraNuKHit");
	  
	  if (!ev_track) {
	    print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, track!"));
	    return false;
	  }

	  std::cout << "I see " << ev_track->size() << " tracks in this event\n";
	  
	  // Sorting through the tracks
	  for (size_t i=0; i<ev_track->size(); i++) {
	    
	    auto track=ev_track->at(i);
	    
	    std::cout << i << "\n";

	    auto const& trk_start = track.front().Position().Vect();
	    auto const& trk_end = track.back().Position().Vect();
	    
	    std::cout << "This is this track's x coordinate: " << track.front().X() << "\n";
	    std::cout << "This is this track's y coordinate: " << track.front().Y() << "\n";
	    std::cout << "This is this track's z coordinate: " << track.front().Z() << "\n";

	  }
	  
	  // Note: mct_start variable aleady initialized above
	  auto const& mct_end = mctrack.back().Position().Vect();

	  if (ev_track->size() != 1) {
	    print(larlite::msg::kERROR, __FUNCTION__, Form("NOTE: ev_track->size()!=1"));
	    return false;
	    
	  }                    
	  
	} else {
	  
	  print(larlite::msg::kERROR, __FUNCTION__, Form("These mctracks did not have the PDG=13 and Origin=1"));
	  
	  continue;
	  
	}
	
      }
      
    } // Ends the for loop above.
    
      // MC Reco stuff should be in here

    /*

    // Extract MC TTree info from the one MCTrack
    auto const& mct = ev_mctrack->at(0);
    
        _true_mom=mct.front().Momentum().Vect().Mag()/1000.;
    
     _true_length=(mct.End().Position().Vect()-mct.Start().Position().Vect()).Mag();
    
    if (_using_mctracks)
      _mcs_reco_mom = _tmc.GetMomentumMultiScatterLLHD(mct);
  
    std::cout << "Everything went ok so far. Now onto the Reco Tracks." << "\n";
  
    if (!_using_mctracks) {

      //Get the Reco Tracks
      auto ev_track = storage->get_data<event_track>("pandoraNuKHit");
          
      if (!ev_track) {

	print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, track!"));
	return false;

      }

      std::cout << "I see " << ev_track->size() << " tracks in this event\n";

      if (ev_track->size() != 1) {
	print(larlite::msg::kERROR, __FUNCTION__, Form("NOTE: ev_track->size()!=1"));
	return false;
    
      }
    
      // Extract Reco TTree info from the one track
      auto const& trk = ev_track->at(0);
      _mcs_reco_mom = _tmc.GetMomentumMultiScatterLLHD(trk);
      _reco_length = trk.Length();
  
    }
  
    _mu_contained = _fidvolBox.Contain(mct.front().Position().Vect()) &&
      _fidvolBox.Contain(mct.back().Position().Vect());
  
    _ana_tree->Fill();
    
    */

    return true;
}

  bool TestMultiScatterMomentum::finalize() {
  
    if (_fout) { _fout->cd(); _ana_tree->Write(); }
  
    else
      print(larlite::msg::kERROR, __FUNCTION__, "Did not find an output file pointer!!! File not opened?");
  
    if (_ana_tree)
      delete _ana_tree;
  
    return true;
  }

}
#endif

