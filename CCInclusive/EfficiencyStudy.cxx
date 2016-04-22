#ifndef LARLITE_EFFICIENCYSTUDY_CXX
#define LARLITE_EFFICIENCYSTUDY_CXX

#include "EfficiencyStudy.h"
#include "DataFormat/vertex.h"
#include "DataFormat/mctruth.h"
#include "DataFormat/opflash.h"
#include "DataFormat/track.h"
#include "DataFormat/mctrajectory.h"


namespace larlite {

    bool EfficiencyStudy::initialize() {


        //Box here is TPC
        _myGeoAABox.Min( 0,
                         -(::larutil::Geometry::GetME()->DetHalfHeight()),
                         0);

        _myGeoAABox.Max( 2 * (::larutil::Geometry::GetME()->DetHalfWidth()),
                         ::larutil::Geometry::GetME()->DetHalfHeight(),
                         ::larutil::Geometry::GetME()->DetLength());


        if (!_vtx_tree) {
            _vtx_tree = new TTree("vtxtree", "vtxtree");
            _vtx_tree->Branch("dist_vtx_truth", &_dist_vtx_truth, "dist_vtx_truth/D");
        }

        if (!_evt_tree) {
            _evt_tree = new TTree("evttree", "evttree");
            _evt_tree->Branch("is_truth_fiducial", &_is_truth_fiducial, "is_truth_fiducial/O");
            _evt_tree->Branch("is_numuCC", &_is_numuCC, "is_numuCC/O");
            _evt_tree->Branch("flash_in_bgw", &_flash_in_bgw, "flash_in_bgw/O");
            _evt_tree->Branch("is_areco_vtx_in_fidvol", &_is_areco_vtx_in_fidvol, "_is_areco_vtx_in_fidvol/O");
            _evt_tree->Branch("is_atrack_fromvtx", &_is_atrack_fromvtx, "_is_atrack_fromvtx/O");
            _evt_tree->Branch("longest_track_nearflash_z", &_longest_track_nearflash_z, "_longest_track_nearflash_z/O");
            _evt_tree->Branch("longest_trk_contained", &_longest_trk_contained, "_longest_trk_contained/O");
            _evt_tree->Branch("longest_trk_range_longenough", &_longest_trk_range_longenough, "_longest_trk_range_longenough/O");
        }

        if (!_flash_tree) {
            _flash_tree = new TTree("flashtree", "flashtree");
            _flash_tree->Branch("flash_time", &_flash_time, "flash_time/D");
        }

        return true;
    }

    bool EfficiencyStudy::analyze(storage_manager* storage) {

        _dist_vtx_truth = -999.;
        _is_truth_fiducial = false;
        _is_numuCC = false;
        _flash_in_bgw = false;
        _flash_time = -999.;
        _is_areco_vtx_in_fidvol = false;
        _is_atrack_fromvtx = false;
        _longest_track_nearflash_z = false;
        _longest_trk_contained = false;
        _longest_trk_range_longenough = false;

        auto ev_vtx = storage->get_data<event_vertex>("pandoraNu");
        if (!ev_vtx) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, vertex!"));
            return false;
        }
        if (!ev_vtx->size()) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Zero reconstructed vertices in this event!"));
            return false;
        }

        auto ev_track = storage->get_data<event_track>("pandoraNuKHit");
        if (!ev_track) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, track!"));
            return false;
        }
        if (!ev_track->size()) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Zero reconstructed tracks in this event!"));
            return false;
        }

        auto ev_mctruth = storage->get_data<event_mctruth>("generator");
        if (!ev_mctruth) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, mctruth!"));
            return false;
        }
        if (ev_mctruth->size() != 1) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("MCTruth size is not equal to 1... it equals %lu!", ev_mctruth->size()));
            return false;
        }
        auto ev_opflash = storage->get_data<event_opflash>("opflashSat");
        if (!ev_opflash) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, opflash!"));
            return false;
        }
        if (!ev_opflash->size()) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("opflash size is zero!"));
            return false;
        }

        const mcnu neutrino = ev_mctruth->at(0).GetNeutrino();
        _is_numuCC = !neutrino.CCNC() && abs(neutrino.Nu().PdgCode() == 14);

        const ::geoalgo::Vector nustart = ::geoalgo::Vector(neutrino.Nu().Trajectory().at(0).X(),
                                          neutrino.Nu().Trajectory().at(0).Y(),
                                          neutrino.Nu().Trajectory().at(0).Z());

        // 10cm fiducial volume
        _is_truth_fiducial = _myGeoAABox.Contain(nustart) && _geoAlgo.SqDist(_myGeoAABox, nustart) > 100.;

        // Loop over vertices.
        // For each vertex in fiducial volume, loop over reco tracks
        // If you find a reco track starting w/in 5cm from any vertex, store it
        // then if you find a longer reco track, store that one instead
        track longest_track;
        double longest_trk_len = 0;
        for (auto const& vtx : *ev_vtx) {
            ::geoalgo::Vector vertex = ::geoalgo::Vector(vtx.X(), vtx.Y(), vtx.Z());
            _dist_vtx_truth = vertex.Dist(nustart);
            _vtx_tree->Fill();

            if (_myGeoAABox.Contain(vertex) && _geoAlgo.SqDist(_myGeoAABox, vertex) > 100.)
                _is_areco_vtx_in_fidvol = true;

            for (auto const& trk : *ev_track) {
                if (!trk.NumberTrajectoryPoints()) continue;
                ::geoalgo::Vector trkstart = ::geoalgo::Vector(trk.Vertex());
                ::geoalgo::Vector trkend   = ::geoalgo::Vector(trk.End() );
                double dist_trk_start_vtx = vertex.Dist(trkstart);

                if (dist_trk_start_vtx > 5.) continue;

                _is_atrack_fromvtx = true;
                double trk_len = trkstart.Dist(trkend);
                if (trk_len > longest_trk_len) {
                    longest_track = trk;
                    longest_trk_len = trk_len;
                }
            }
        }

        if(longest_trk_len > 0)
        _longest_trk_contained = _myGeoAABox.Contain(::geoalgo::Vector(longest_track.Vertex())) &&
                                 _myGeoAABox.Contain(::geoalgo::Vector(longest_track.End()));

        _longest_trk_range_longenough = longest_trk_len > 75.;

        for (auto const& flash : *ev_opflash) {
            _flash_time = flash.Time();
            _flash_tree->Fill();

            if (flash.Time() > 3.5 && flash.Time() < 5.2 && flash.TotalPE() > 50.)
                _flash_in_bgw = true;
        }

        _evt_tree->Fill();

        return true;
    }

    bool EfficiencyStudy::finalize() {

        if (_fout) {
            _fout->cd();
            _vtx_tree->Write();
            _evt_tree->Write();
            _flash_tree->Write();
        }

        return true;
    }

}
#endif
