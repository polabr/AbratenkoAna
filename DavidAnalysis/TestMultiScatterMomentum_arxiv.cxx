#ifndef LARLITE_TESTMULTISCATTERMOMENTUM_ARXIV_CXX
#define LARLITE_TESTMULTISCATTERMOMENTUM_ARXIV_CXX

#include "TestMultiScatterMomentum_arxiv.h"
#include "DataFormat/mctrack.h"

namespace larlite {

    bool TestMultiScatterMomentum_arxiv::initialize() {

        _tmc = TrackMomentumCalculator_arxiv();

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

    bool TestMultiScatterMomentum_arxiv::analyze(storage_manager* storage) {

        _true_mom = -999.;
        _mcs_reco_mom = -999.;
        _true_length = -999.;
        _reco_length = -999.;
        _mu_contained = false;

        //Get the MCTracks
        auto ev_mctrack = storage->get_data<event_mctrack>("mcreco");
        if (!ev_mctrack) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, mctrack!"));
            return false;
        }
        if (ev_mctrack->size() != 1)
            return false;

        /// Extract MC TTree info from the one MCTrack
        auto const& mct = ev_mctrack->at(0);
        _true_mom = mct.front().Momentum().Vect().Mag() / 1000.;

        _true_length = (mct.End().Position().Vect() - mct.Start().Position().Vect()).Mag();

        if (_using_mctracks)
            _mcs_reco_mom = _tmc.GetMomentumMultiScatterLLHD(mct);


        if (!_using_mctracks) {
            //Get the Reco Tracks
            auto ev_track = storage->get_data<event_track>("pandoraNuKHit");
            if (!ev_track) {
                print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, track!"));
                return false;
            }
            if (ev_track->size() != 1)
                return false;

            /// Extract Reco TTree info from the one track
            auto const& trk = ev_track->at(0);
            _mcs_reco_mom = _tmc.GetMomentumMultiScatterLLHD(trk);
            _reco_length = trk.Length();
        }

        _mu_contained = _fidvolBox.Contain(mct.front().Position().Vect()) &&
                        _fidvolBox.Contain(mct.back().Position().Vect());

        _ana_tree->Fill();
        return true;
    }

    bool TestMultiScatterMomentum_arxiv::finalize() {

        if (_fout) { _fout->cd(); _ana_tree->Write(); }

        else
            print(larlite::msg::kERROR, __FUNCTION__, "Did not find an output file pointer!!! File not opened?");

        if (_ana_tree)
            delete _ana_tree;

        return true;
    }

}
#endif
