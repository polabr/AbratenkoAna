#ifndef LARLITE_EVENTSCONTAINEDSTUDY_CXX
#define LARLITE_EVENTSCONTAINEDSTUDY_CXX

#include "EventsContainedStudy.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/mctruth.h"
#include "DataFormat/track.h"

namespace larlite {

	bool EventsContainedStudy::initialize() {

		double fidvol_dist = 5.;
		double fidvol_dist_y = 5.;

		//Box here is TPC
		_fidvolBox.Min( 0 + fidvol_dist,
		                -(::larutil::Geometry::GetME()->DetHalfHeight()) + fidvol_dist_y,
		                0 + fidvol_dist);

		_fidvolBox.Max( 2 * (::larutil::Geometry::GetME()->DetHalfWidth()) - fidvol_dist,
		                ::larutil::Geometry::GetME()->DetHalfHeight() - fidvol_dist_y,
		                ::larutil::Geometry::GetME()->DetLength() - fidvol_dist);

		_evts_pass_filter = 0;
		_evts_fully_contained = 0;


		_h_mu_E_contained = new TH1F("_h_mu_E_contained", "Muon True Start Energy: Contained Events [GeV]", 100, 0, 3.);
		_h_mu_E_all = new TH1F("_h_mu_E_all", "Muon True Start Energy: All Events [GeV]", 100, 0, 3.);
		_h_nu_E_contained = new TH1F("_h_nu_E_contained", "Neutrino True Energy: Contained Events [GeV]", 100, 0, 3.);
		_h_nu_E_all = new TH1F("_h_nu_E_all", "Neutrino True Energy: All Events [GeV]", 100, 0, 3.);

		_tree = new TTree("tree", "tree");
		_tree->Branch("mu_contained", &_mu_contained, "mu_contained/O");
		_tree->Branch("p_contained", &_p_contained, "p_contained/O");
		_tree->Branch("n_reco_tracks_fromvtx", &_n_reco_tracks_fromvtx, "n_reco_tracks_fromvtx/I");
		_tree->Branch("n_mc_tracks_fromvtx", &_n_mc_tracks_fromvtx, "n_mc_tracks_fromvtx/I");
		return true;
	}

	bool EventsContainedStudy::analyze(storage_manager* storage) {

		_evts_pass_filter++;

		_mu_contained = false;
		_p_contained = false;
		_n_mc_tracks_fromvtx = 0;
		_n_reco_tracks_fromvtx = 0;

		auto ev_mctrack = storage->get_data<event_mctrack>("mcreco");
		if (!ev_mctrack) {
			print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, mctrack!"));
			return false;
		}
		// if (ev_mctrack->size() != 2) {
		// 	print(larlite::msg::kWARNING, __FUNCTION__, Form("MCTrack size is not equal to 2... it equals %lu!", ev_mctrack->size()));
		// 	return false;
		// }
//Grab the MCTruth
		auto ev_mctruth = storage->get_data<event_mctruth>("generator");
		if (!ev_mctruth) {
			print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, mctruth!"));
			return false;
		}
		// Require exactly one neutrino interaction
		if (ev_mctruth->size() != 1) {
			print(larlite::msg::kINFO, __FUNCTION__, Form("ev_mctruth size is not 1!"));
			return false;
		}

		auto ev_track = storage->get_data<event_track>("pandoraNuPMA");
		if (!ev_track) {
			print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, track!"));
			return false;
		}

		double true_nu_E = ev_mctruth->at(0).GetNeutrino().Nu().Trajectory().front().E();
		auto const& nu_vtx = ev_mctruth->at(0).GetNeutrino().Nu().Trajectory().front().Position().Vect();

		// Make a small 3cm geosphere around neutrino vertex
		_nu_sphere.Center(nu_vtx);
		_nu_sphere.Radius(3.);

		// Store the muon and the proton mctracks for later investigation
		bool _found_muon = false;
		mctrack mu_mctrack;
		bool _found_proton = false;
		mctrack p_mctrack;

		// std::cout<<"Loop over mctracks"<<std::endl;
		for (auto const& mct : *ev_mctrack) {

			if (!mct.size()) continue;

			// Only count mctracks that stat within 1cm of the true neutrino vertex
			if ( !_nu_sphere.Contain(mct.front().Position().Vect()) )
				continue;

			_n_mc_tracks_fromvtx++;

			if (mct.PdgCode() == 13 ) {
				//if you've already found a muon mctrack, return false
				if (_found_muon) return false;
				_found_muon = true;
				mu_mctrack = mct;
				// std::cout<<"FRACITON: found muon. start total energy is "<<mu_mctrack.front().E()<<std::endl;
			}
			else if (mct.PdgCode() == 2212) {
				// if you've already found a proton mctrack, return false;
				if (_found_proton) return false;
				_found_proton = true;
				p_mctrack = mct;
				// std::cout<<"FRACTION: found proton. start total energy is "<<p_mctrack.front().E()<<std::endl;
			}
			else {

				// If other events exist in this event ... if you are using
				// MC_1mu_1pNn0else_Filter_MCTracks, this might happen for example when you have a high
				// energy proton from the neutrino that interacts to create a charged pion.
				// For now just throw this event out. I'm looking for CLEAN 1mu+1p events
				// with no crazy shit happening downstream.
				// Maybe 10% of the 1mu+1p events have this extra crazy shit

				// print(larlite::msg::kWARNING, "EventsContainedStudy",
				//       Form("Found a mctrack that wasn't p or mu! Its PDG is %d and its energy deposited is %0.1f MEV.",
				//            mct.PdgCode(), mct.front().E() - mct.back().E()));
				// if (abs(mct.PdgCode()) == 211) {
				// std::cout << std::endl; std::cout << std::endl; std::cout << std::endl;
				// std::cout << "FRACTION: Here is the pion start point: ";
				// mct.front().Position().Vect().Print();
				// std::cout<<"FRACTION: Here is its deposited energy: "<<mct.front().E()-mct.back().E()<<std::endl;
				// throw std::exception();
				// std::cout<<"FRACTION: Here is its process string: "<<mct.Process()<<std::endl;
				// }


				return false;
			}
		}


		// If you haven't found a proton and a muon, something has gone wrong.
		if (!_found_proton || !_found_muon) {
			print(larlite::msg::kWARNING, "EventsContainedStudy",
			      Form("Either didn't find a proton, or didn't find a muon!"));
			return false;
		}

		_mu_contained = _fidvolBox.Contain(mu_mctrack.front().Position().Vect()) &&
		                _fidvolBox.Contain(mu_mctrack.back().Position().Vect());

		_p_contained = _fidvolBox.Contain(p_mctrack.front().Position().Vect()) &&
		               _fidvolBox.Contain(p_mctrack.back().Position().Vect());

		_h_mu_E_all->Fill(mu_mctrack.front().E() / 1000.);
		_h_nu_E_all->Fill(true_nu_E);

		if (_mu_contained && _p_contained) {
			_h_mu_E_contained->Fill(mu_mctrack.front().E() / 1000.);
			_h_nu_E_contained->Fill(true_nu_E);
			_evts_fully_contained++;
		}


		/// Loop over reco tracks
		// std::cout<<"Loop over mctracks"<<std::endl;
		for (auto const& trk : *ev_track) {
			if (_nu_sphere.Contain(trk.Vertex()))
				_n_reco_tracks_fromvtx++;
		}
		_tree->Fill();


		return true;
	}

	bool EventsContainedStudy::finalize() {

		std::cout << "Events Contained Study! Input events = "
		          << _evts_pass_filter << ", fully contained events = "
		          << _evts_fully_contained << ", so the fraction of events contained is "
		          << (double)_evts_fully_contained / (double)_evts_pass_filter << std::endl;

		if (_fout) {
			_fout->cd();
			_h_mu_E_contained->Write();
			_h_mu_E_all->Write();
			_h_nu_E_contained->Write();
			_h_nu_E_all->Write();
			_tree->Write();
		}

		return true;
	}

}
#endif
