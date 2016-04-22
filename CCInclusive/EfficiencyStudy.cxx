#ifndef LARLITE_EFFICIENCYSTUDY_CXX
#define LARLITE_EFFICIENCYSTUDY_CXX

#include "EfficiencyStudy.h"
#include "DataFormat/vertex.h"
#include "DataFormat/mctruth.h"
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


        if (!_tree) {

            _tree = new TTree("tree", "tree");
            _tree->Branch("dist_vtx_truth", &_dist_vtx_truth, "dist_vtx_truth/D");
            _tree->Branch("is_truth_fiducial", &_is_truth_fiducial, "is_truth_fiducial/O");
        }

        return true;
    }

    bool EfficiencyStudy::analyze(storage_manager* storage) {

        _dist_vtx_truth = -999;
        _is_truth_fiducial = false;

        auto ev_vtx = storage->get_data<event_vertex>("pandoraNu");
        if (!ev_vtx) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, vertex!"));
            return false;
        }
        if (!ev_vtx->size()) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Zero reconstructed vertices in this event!"));
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

        ::geoalgo::Vector nustart = ::geoalgo::Vector(ev_mctruth->at(0).GetNeutrino().Nu().Trajectory().at(0).X(),
                                     ev_mctruth->at(0).GetNeutrino().Nu().Trajectory().at(0).Y(),
                                     ev_mctruth->at(0).GetNeutrino().Nu().Trajectory().at(0).Z());

        for (auto const& vtx : *ev_vtx) {
            _dist_vtx_truth = ::geoalgo::Vector(vtx.X(), vtx.Y(), vtx.Z()).Dist(nustart);
            _tree->Fill();
        }


//_myGeoAABox.Contain(mytrack.Start().Position())


        return true;
    }

    bool EfficiencyStudy::finalize() {

        if (_fout) {
            _fout->cd();
            _tree->Write();
        }

        return true;
    }

}
#endif
