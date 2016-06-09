#ifndef LARLITE_TESTPPICALO_CXX
#define LARLITE_TESTPPICALO_CXX

#include "TestPPiCalo.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/track.h"
#include "DataFormat/calorimetry.h"

namespace larlite {

    bool TestPPiCalo::initialize() {

        _h_avgdedx_proton = new TH1F("h_avgdedx_proton", "Average dEdx In Most-Hits Plane: Single Protons", 1000, 0, 10);
        _h_avgdedx_pion = new TH1F("h_avgdedx_pion", "Average dEdx In Most-Hits Plane: Single Pions", 1000, 0, 10);
        _h_avgdedx_muon = new TH1F("h_avgdedx_muon", "Average dEdx In Most-Hits Plane: Single Muons", 1000, 0, 10);

        return true;
    }

    bool TestPPiCalo::analyze(storage_manager* storage) {

        //Grab the MCTrack
        auto ev_mctrack = storage->get_data<event_mctrack>("mcreco");
        if (!ev_mctrack) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, mctrack!"));
            return false;
        }
        if (ev_mctrack->size() != 1) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("More than 1 MCTrack in this event!"));
            return false;
        }

        int mctrack_pdg = ev_mctrack->at(0).PdgCode();
        int proton_1_pion_2_muon_3 = 0;
        if ( abs(mctrack_pdg) == 2212 ) proton_1_pion_2_muon_3 = 1;
        else if (abs(mctrack_pdg) == 211) proton_1_pion_2_muon_3 = 2;
        else if ( abs(mctrack_pdg) == 13) proton_1_pion_2_muon_3 = 3;
        else {
            print(larlite::msg::kERROR, __FUNCTION__, Form("The MCTrack was not proton or pion or muon. What the heck?"));
            return false;
        }

        auto ev_track = storage->get_data<event_track>("pandoraNuPMA");
        if (!ev_track) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, track!"));
            return false;
        }
        if (ev_track->size() != 1 ) {
            print(larlite::msg::kWARNING, __FUNCTION__, Form("# of reconstructed tracks in this event is not == 1"));
            return false;
        }

        event_calorimetry* ev_calo = nullptr;
        auto const& ass_calo_v = storage->find_one_ass(ev_track->id(),
                                 ev_calo,
                                 Form("%scalo", ev_track->name().c_str()));

        if (!ev_calo) {
            std::cerr << "no event_calorimetry!" << std::endl;
            return false;
        }

        if (ev_calo->empty()) {
            std::cout << "ev_calo empty" << std::endl;
            return false;
        }

        //size of ev calo is 3 (one per plane)... let's use the plane with the most hits
        // Choose the calo object for this track by the one
        // with the most number of hits in the dEdx vector
        // (this is how analysis tree does it)
        calorimetry thecalo;
        size_t tmp_nhits = 0;
        for (size_t i = 0; i < 3; ++i)
            if (ev_calo->at(i).dEdx().size() > tmp_nhits) {
                thecalo = ev_calo->at(i);
                tmp_nhits = thecalo.dEdx().size();
            }

        // Parameter of interest is average dedx over the whole track
        double dedx_avg = 0;
        for (size_t i = 0; i < thecalo.dEdx().size(); ++i)
            dedx_avg += thecalo.dEdx().at(i);
        dedx_avg /= thecalo.dEdx().size();

        if (proton_1_pion_2_muon_3 == 1) _h_avgdedx_proton->Fill(dedx_avg);
        if (proton_1_pion_2_muon_3 == 2) _h_avgdedx_pion->Fill(dedx_avg);
        if (proton_1_pion_2_muon_3 == 3) _h_avgdedx_muon->Fill(dedx_avg);
        
        return true;
    }

    bool TestPPiCalo::finalize() {

        if (_fout) {
            _h_avgdedx_proton->Write();
            _h_avgdedx_pion->Write();
            _h_avgdedx_muon->Write();
        }

        return true;
    }

}
#endif
