// Code for creating a TTree of BNB + cosmic data
// Filters out cosmics; matches 1 mctrack to 1 track 
//
// Author: Polina Abratenko

#ifndef LARLITE_TESTMULTISCATTERMOMENTUM_CXX
#define LARLITE_TESTMULTISCATTERMOMENTUM_CXX

#include "TestMultiScatterMomentum.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/mctruth.h"

namespace larlite {
  
  bool TestMultiScatterMomentum::initialize() {

    _tmc = TrackMomentumCalculator();

    // Create TTree variables
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
    
    // Set initial values for all vars
    _true_mom = -999.;
    _mcs_reco_mom = -999.;
    _true_length = -999.;
    _reco_length = -999.;
    _mu_contained = false;

    std::cout << "Starting mc truth now." << "\n";
    
    // Get neutrino interaction vertex
    auto ev_mctruth = storage->get_data<event_mctruth>("generator");

    // Neutrino vertex has to be at a single point
    if(ev_mctruth->size() != 1) 
      return false;
    
    auto mctruth = ev_mctruth->at(0);
    
    auto const& nu_vtx = mctruth.GetNeutrino().Nu().Trajectory().front().Position().Vect();
    
    if (_fidvolBox.Contain(nu_vtx)) {
      
      std::cout << "Neutrino vertex contained!" << "\n";
      
      std::cout << "The x-coordinate of the nu vertex is located at: " << mctruth.GetNeutrino().Nu().Trajectory().front().X() << ".\n";
      std::cout << "The y-coordinate of the nu vertex is located at: " << mctruth.GetNeutrino().Nu().Trajectory().front().Y() << ".\n";
      std::cout << "The z-coordinate of the nu vertex is located at: " << mctruth.GetNeutrino().Nu().Trajectory().front().Z() << ".\n";	

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
    for (size_t i = 0; i < ev_mctrack->size(); i++) {
      
      auto mctrack = ev_mctrack->at(i);
      
      if(mctrack.PdgCode() == 13 && mctrack.Origin() == 1) {
	
	std::cout << "On mctrack " << i << ", and the particle's mother PDG is " << mctrack.MotherPdgCode() << ", the PDG code is " << mctrack.PdgCode() << ", and the ancestor PDG is " << mctrack.AncestorPdgCode() << "\n";
	std::cout << "The origin is: " << mctrack.Origin() << ".\n";
	
	if (mctrack.size() != 0) {
	  
	  std::cout << "The x coordinate of the starting point of the muon is: " << mctrack.front().X() << "\n"; 
	  std::cout << "The y coordinate of the starting point of the muon is: " << mctrack.front().Y() << "\n"; 
	  std::cout << "The z coordinate of the starting point of the muon is: " << mctrack.front().Z() << "\n"; 
	  
	} else {
	  print(larlite::msg::kERROR, __FUNCTION__, Form("mctrack.size() was 0 here!"));
	  continue;
	}
	
	auto const& mct_start = mctrack.front().Position().Vect();
	
	double dist = (nu_vtx - mct_start).Mag();
	
	std::cout << "This is the difference between coordinates: " << dist << "\n";
	
	if (dist > 0.001) {
	  print(larlite::msg::kERROR, __FUNCTION__, Form("Distance was too large, skipping this mctrack..."));
	  return false;
	}
	
	// Extract MC TTree info from the one MCTrack  
	_true_mom = mctrack.front().Momentum().Vect().Mag() / 1000.;
	std::cout << "True mom for this mctrack: " << _true_mom << " GeV \n";
	
	_true_length = (mctrack.End().Position().Vect() - mctrack.Start().Position().Vect()).Mag(); 
	std::cout << "True length for this mctrack: " << _true_length << " cm \n";	
	
	if (_using_mctracks) {
	  _mcs_reco_mom = _tmc.GetMomentumMultiScatterLLHD(mctrack);
       
	std::cout << "Printing mcs reco mom: " << _mcs_reco_mom << " GeV \n";    
	
	}

	std::cout << "Everything went ok so far. Now onto the Reco Tracks." << "\n";    
	
	if (!_using_mctracks) {
	  
	  //Get the Reco Tracks
	  auto ev_track = storage->get_data<event_track>("pandoraNuKHit");
	  
	  if (!ev_track) {	    
	    print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, track!"));
	    return false;
	  } 
	  
	  std::cout << "I see " << ev_track->size() << " tracks in this event\n";
	  
	  if (ev_track->size() < 1) {
	    print(larlite::msg::kERROR, __FUNCTION__, Form("No tracks in this event! Skipping..."));
	    return false;
	  }

	  // Setting the final chosen track's distance to some absurdly high number to start with
	  double minDist = 10000;

	  // Just assigning a dummy value to chosenTrack here
	  auto chosenTrack = ev_track->at(0);

	  // This will be the index of the final chosen track
	  int index = 0;

	  // Sorting through the tracks
	  for (size_t i = 0; i < ev_track->size(); i++) {
	    
	    auto track = ev_track->at(i);
	    
	    auto const& trk_start = track.LocationAtPoint(0);  
	    
	    std::cout << "These are TRACK " << i << "'s starting X, Y, Z coordinates: " << trk_start.X() << ", " << trk_start.Y() << ", " << trk_start.Z() << "\n";
	    
	    auto  distBetweenStarts = sqrt(pow(trk_start.X() - mct_start.X(), 2) + pow(trk_start.Y() - mct_start.Y(), 2) + pow(trk_start.Z() - mct_start.Z(), 2));
	    
	    std::cout << "Dist between starts: " << distBetweenStarts << "\n";
	    
	    if (distBetweenStarts < minDist) {
	      
	      minDist = distBetweenStarts;

	      chosenTrack = track;

	      index = i;
	    
	    }
	    
	  }
	  
	  std::cout << "This is the resulting minDist: " << minDist << "\n";
	  
	  auto const& cho_trk_start = chosenTrack.LocationAtPoint(0);
	  
	  std::cout << "These are the CHOSEN TRACK's starting X, Y, Z coordinates: " << cho_trk_start.X() << ", " << cho_trk_start.Y() << ", " << cho_trk_start.Z() << "\n";

	  std::cout << "It is track number " << index << "\n";
	 
	  // Extract Reco TTree info from the one track  
	  _mcs_reco_mom = _tmc.GetMomentumMultiScatterLLHD(chosenTrack);
	  
	  std::cout << "This is the track's mcs reco: " << _mcs_reco_mom << " GeV \n";
	  
	  _reco_length = chosenTrack.Length();
	  
	  std::cout << "This is the track's reco length: " << _reco_length << " cm \n";

	}
	
      } else {
	
	print(larlite::msg::kERROR, __FUNCTION__, Form("These mctracks did not have the PDG=13 and Origin=1"));
	continue;
	
      }
      
      _mu_contained = _fidvolBox.Contain(mctrack.front().Position().Vect()) &&  _fidvolBox.Contain(mctrack.back().Position().Vect());                                                                                    
      
      _ana_tree->Fill();
      
    }
    
    return true;
    
  }
  
  bool TestMultiScatterMomentum::finalize() {
    
    std::cout << "Writing ana_tree" << "\n";
    if (_fout) { _fout->cd(); _ana_tree->Write(); }
    
    else
      print(larlite::msg::kERROR, __FUNCTION__, "Did not find an output file pointer!!! File not opened?");
    
    if (_ana_tree)
      delete _ana_tree;
    
    return true;
    
  }
  
}
#endif

