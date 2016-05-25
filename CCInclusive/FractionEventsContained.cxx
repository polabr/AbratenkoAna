#ifndef LARLITE_FRACTIONEVENTSCONTAINED_CXX
#define LARLITE_FRACTIONEVENTSCONTAINED_CXX

#include "FractionEventsContained.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/mctruth.h"

namespace larlite {

	bool FractionEventsContained::initialize() {

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


		return true;
	}

	bool FractionEventsContained::analyze(storage_manager* storage) {

		_evts_pass_filter++;

		_mu_contained = false;
		_p_contained = false;

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

		double true_nu_E = ev_mctruth->at(0).GetNeutrino().Nu().Trajectory().front().E();

		// Store the muon and the proton mctracks for later investigation
		bool _found_muon = false;
		mctrack mu_mctrack;
		bool _found_proton = false;
		mctrack p_mctrack;

		// std::cout<<"Loop over mctracks"<<std::endl;
		for (auto const& mct : *ev_mctrack) {
			// std::cout<<" mct.size is "<<mct.size()<<std::endl;
			if (!mct.size()) continue;
			// std::cout<<" mct pdg is "<<mct.PdgCode()<<std::endl;
			// std::cout<<" mct deposited energy is "<<mct.front().E() - mct.back().E()<<std::endl;

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

				// print(larlite::msg::kWARNING, "FractionEventsContained",
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
			print(larlite::msg::kWARNING, "FractionEventsContained",
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

		return true;
	}

	bool FractionEventsContained::finalize() {

		std::cout << "Fraction Events Contained! Input events = "
		          << _evts_pass_filter << ", fully contained events = "
		          << _evts_fully_contained << ", so the fraction of events contained is "
		          << (double)_evts_fully_contained / (double)_evts_pass_filter << std::endl;

		if (_fout) {
			_fout->cd();
			_h_mu_E_contained->Write();
			_h_mu_E_all->Write();
			_h_nu_E_contained->Write();
			_h_nu_E_all->Write();
		}

		return true;
	}

}
#endif
