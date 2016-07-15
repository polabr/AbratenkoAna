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

		unitConstant = 1.;

		_tmc = TrackMomentumCalculator();

		// Create TTree variables
		if (!_ana_tree) {
			_ana_tree = new TTree("ana_tree", "ana_tree");
			_ana_tree->Branch("true_mom", &_true_mom, "true_mom/D");
			_ana_tree->Branch("mcs_reco_mom", &_mcs_reco_mom, "mcs_reco_mom/D");
			_ana_tree->Branch("true_len", &_true_length, "true_len/D");
			_ana_tree->Branch("reco_len", &_reco_length, "reco_len/D");
			_ana_tree->Branch("mu_contained", &_mu_contained, "mu_contained/O");
			_ana_tree->Branch("distances", &_distances, "distances/D");
			_ana_tree->Branch("angles", &_angles, "angles/D");
			_ana_tree->Branch("startPoints", &_startPoints, "startPoints/D");
			_ana_tree->Branch("endPoints", &_endPoints, "endPoints/D");
		}

		if (!_trackmatch_tree) {
			_trackmatch_tree = new TTree("trackmatch_tree", "trackmatch_tree");
			_trackmatch_tree->Branch("match_dist", &_match_dist, "match_dist/D");
			_trackmatch_tree->Branch("match_dotprod", &_match_dotprod, "match_dotprod/D");
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


	const larlite::mctrack TestMultiScatterMomentum::getChosenMCTrack(storage_manager* storage, const TVector3 nu_vtx) {

		// This function loops over mctracks and finds the one that you are interested in
		// EG the muon mctrack from a neutrino interaction, or
		// the single muon mctrack from single muon samples, etc.
		// This function actually RETURNS a reference to the mctrack.

		//Get the MCTracks
		auto ev_mctrack = storage->get_data<event_mctrack>("mcreco");

		if (!ev_mctrack) {
			print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, mctrack!"));
			throw std::exception();
		}
		if (!ev_mctrack->size()) {
			print(larlite::msg::kERROR, __FUNCTION__, Form("Zero mctracks in this event!"));
			throw std::exception();
		}

		// Count the number of mctracks that we find (hopefully we find exactly one)
		// Also store the found mctrack to return later
		larlite::mctrack found_mctrack;
		size_t n_found_mctracks = 0;
		// Loop over mctracks
		for (auto const& mctrack : *ev_mctrack) {
			if ( mctrack.size() == 0 ) continue;
			if ( mctrack.Origin() != 1 || mctrack.PdgCode() != 13 ) continue;
			// At this point in the loop we only have mctracks that are
			// muons that came from origin == 1 (not cosmic)
			// and the mctracks have size greater than or equal to 1

			// Compute distance between mctrack start and neutrino vertex
			// Require this distance is less than 0.001 cm
			double dist = (nu_vtx - mctrack.front().Position().Vect()).Mag();
			if ( dist > 0.001 ) continue;

			// At this point we have found the mctrack we want
			found_mctrack = mctrack;
			n_found_mctracks++;
		}

		// Require exactly one good mctrack, otherwise throw exception
		if (n_found_mctracks != 1) {
			print(larlite::msg::kWARNING, __FUNCTION__, Form("n_found_mctracks not equal to 1! It is %zu.", n_found_mctracks));
			throw std::exception();
		}

		// Here we are sure we have found exactly one MCTrack, and we are sure
		// it is the MCTrack in which we are interested
		// Return this mctrack
		return found_mctrack;

	}

	const larlite::track TestMultiScatterMomentum::getMatchedTrack(storage_manager* storage,
	        const larlite::mctrack chosen_mctrack) {

		// This function loops over mctracks and finds the one track that MATCHES the mctrack you've already found
		// This function actually RETURNS a reference to the track.

		// Initialize the debugging track_match tree variables:
		_match_dist = -999.;
		_match_dotprod = -999.;

		//Get the Tracks
		auto ev_track = storage->get_data<event_track>("pandoraNuPMA");

		if (!ev_track) {
			print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, track!"));
			throw std::exception();
		}
		if (!ev_track->size()) {
			print(larlite::msg::kERROR, __FUNCTION__, Form("Zero tracks in this event!"));
			throw std::exception();
		}

		// Obtain vector approximating direction of mctrack
		TVector3 mctrackDir = chosen_mctrack.back().Position().Vect() - chosen_mctrack.front().Position().Vect();
		TVector3 unitMctrackDir = mctrackDir.Unit();

		/////////////////////////////////
		// THESE CUT VALUES ARE TUNED AFTER RUNNING AND LOOKING AT TRACK_MATCH_TREE
		/////////////////////////////////

		// Max allowed (unitConstant - abs(dot product)) b/t reco track and mctrack
		// for the track to be considered "matched" to the mctrack
		double maxDir = 0.0002;

		// Setting the maximum allowed distance threshold
		double maxDist = 3;

		// Count the number of tracks that we find (hopefully we find exactly one)
		// Also store the found track to return later
		larlite::track chosenTrack;
		size_t n_found_tracks = 0;
		// Loop over tracks
		for (auto const& track : *ev_track) {

			// Want a track with at least 2 points in it
			if (track.NumberTrajectoryPoints() < 2) continue;

			double distBetweenStarts = (track.Vertex() - chosen_mctrack.front().Position().Vect()).Mag();
			double distBetweenStartEnd =  (track.End() - chosen_mctrack.front().Position().Vect()).Mag();

			// Assigns minimum difference value to distBetweenStarts
			if (distBetweenStarts > distBetweenStartEnd) distBetweenStarts = distBetweenStartEnd;

			_match_dist = distBetweenStarts;

			auto const& trk_start = track.Vertex();
			auto const& trk_end = track.End();

			TVector3 trackDir = trk_end - trk_start;
			TVector3 unitTrackDir = trackDir.Unit();

			double dotProduct = unitTrackDir.Dot(unitMctrackDir);
			double angleDiff = unitConstant - fabs(dotProduct);

			_match_dotprod = angleDiff;

			// This is a debugging ttree
			_trackmatch_tree->Fill();

			// If the start of the reco track is greater than "maxDist" cm from the start of the mctrack,
			// we know we don't want this track, so continue
			if (fabs(distBetweenStarts) > maxDist) continue;

			// If the reco track doesn't have close enough direction to the mctrack, skip this reco track
			if (angleDiff > maxDir) continue;

			// Here we have a "matched" track!
			chosenTrack = track;
			n_found_tracks++;
		}

		// Require exactly one matched track, otherwise throw exception
		if (n_found_tracks != 1) {
			print(larlite::msg::kWARNING, __FUNCTION__, Form("n_found_tracks not equal to 1! It is %zu.", n_found_tracks));
			throw std::exception();
		}

		return chosenTrack;
	}

	bool TestMultiScatterMomentum::analyze(storage_manager * storage) {

		// Set initial values for all vars
		_true_mom = -999.;
		_mcs_reco_mom = -999.;
		_true_length = -999.;
		_reco_length = -999.;
		_mu_contained = false;
		_distances = -999.;
		_angles = -999.;
		_startPoints.SetXYZ(-999., -999., -999.);
		_endPoints.SetXYZ(-999., -999., -999.);

		// Get neutrino interaction vertex
		auto ev_mctruth = storage->get_data<event_mctruth>("generator");
		if (!ev_mctruth) {
			print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, mctruth!"));
			return false;
		}
		// There should be exactly one neutrino in this event
		if (ev_mctruth->size() != 1) {
			print(larlite::msg::kERROR, __FUNCTION__, Form("MCTruth size was not 1! It was %zu.", ev_mctruth->size()));
			return false;
		}

		auto mctruth = ev_mctruth->at(0);
		auto const& nu_vtx = mctruth.GetNeutrino().Nu().Trajectory().front().Position().Vect();

		// Require the true neutrino vertex is contained in our TPC box, otherwise skip the entire event
		if ( !_fidvolBox.Contain(nu_vtx) ) {
			// print(larlite::msg::kERROR, __FUNCTION__, Form("NEUTRINO VERTEX NOT CONTAINED"));
			return false;
		}

		// Find the mctrack you are interested in with the getChosenMCTrack function
		larlite::mctrack chosen_mctrack;
		try {
			chosen_mctrack = getChosenMCTrack(storage, nu_vtx);
		}
		catch (const std::exception& e) {
			// If fails (it threw an exception), skip the event
			return false;
		}

		// At this point in the code, we have "chosen_mctrack" and we are sure
		// it is the mctrack we want.

		// Fill TTree variables that only have to do with the MCTrack
		// Extract MC TTree info from the one MCTrack
		_true_mom = chosen_mctrack.front().Momentum().Vect().Mag() / 1000.;

		// This is true length in the detector, so front() and back() are used rather than Start() and End()
		_true_length = (chosen_mctrack.back().Position().Vect() - chosen_mctrack.front().Position().Vect()).Mag();

		// Is the mctrack contained in fiducial volume?
		_mu_contained = _fidvolBox.Contain(chosen_mctrack.front().Position().Vect()) &&
		                _fidvolBox.Contain(chosen_mctrack.back().Position().Vect());


		// If the user is running only on MCTracks:
		// fill the ttree with the found MCTrack info then
		// FINISH the event (return true)
		// If the user is NOT running only on MCTracks:
		// continue directly past this if-statement and start dealing with reco tracks
		if (_using_mctracks) {
			_mcs_reco_mom = _tmc.GetMomentumMultiScatterLLHD(chosen_mctrack);
			_ana_tree->Fill();
			return true;
		}

		// Now on to reconstructed tracks (if the user has _using_mctracks set to false)

		// Find the reco track that matches the chosen mctrack
		larlite::track chosen_track;
		try {
			chosen_track = getMatchedTrack(storage, chosen_mctrack);
		}
		catch (const std::exception& e) {
			// If fails (it threw an exception), skip the event
			return false;
		}

		// At this point in the code, we have "chosen_track" and we are sure
		// it is the track we want (we have decided it matches the mctrack we previously found)

		// Let's store some TTree variables about the chosen track
		_reco_length = chosen_track.Length();

		// Start and end point of matched reco track (TVector3's)
		_startPoints = chosen_track.Vertex();
		_endPoints = chosen_track.End();

		// Distance between track start and MCTrack start, or track end and MCTrack start
		// (whichever is smaller)
		double distBetweenStarts = (_startPoints - chosen_mctrack.front().Position().Vect()).Mag();
		double distBetweenStartEnd =  (_endPoints - chosen_mctrack.front().Position().Vect()).Mag();
		if (distBetweenStarts > distBetweenStartEnd) _distances = distBetweenStartEnd;
		else _distances = distBetweenStarts;

		// Angle difference between chosen MCTrack and matched reco track
		TVector3 mctrackDir = chosen_mctrack.back().Position().Vect() - chosen_mctrack.front().Position().Vect();
		TVector3 unitMctrackDir = mctrackDir.Unit();
		TVector3 trackDir = _endPoints - _startPoints;
		TVector3 unitTrackDir = trackDir.Unit();
		double dotProduct = unitTrackDir.Dot(unitMctrackDir);
		_angles = fabs(dotProduct) - unitConstant;

		// MCS momentum of matched reco track:
		_mcs_reco_mom = _tmc.GetMomentumMultiScatterLLHD(chosen_track);

		// Length of matched reco track:
		_reco_length = chosen_track.Length();

		_ana_tree->Fill();

		return true;

	}

	bool TestMultiScatterMomentum::finalize() {

		std::cout << "Writing ttrees..." << "\n";
		if (_fout) { _fout->cd(); _ana_tree->Write(); _trackmatch_tree->Write(); }

		else
			print(larlite::msg::kERROR, __FUNCTION__, "Did not find an output file pointer!!! File not opened?");

		if (_ana_tree)
			delete _ana_tree;
		if (_trackmatch_tree)
			delete _trackmatch_tree;

		return true;

	}

}
#endif
