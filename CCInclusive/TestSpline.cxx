#ifndef LARLITE_TESTSPLINE_CXX
#define LARLITE_TESTSPLINE_CXX

#include "TestSpline.h"
#include "DataFormat/mctruth.h"
#include "DataFormat/mctrack.h"

namespace larlite {

    bool TestSpline::initialize() {

        _h = new TH2D("h", "Spline Computed E vs. Perfect Reco Muon Deposited E", 100, 0, 3, 100, 0, 3);

        myspline = new MuTrackMomentumSpline();


        double fidvol_dist = 10.;
        double fidvol_dist_y = 20.;

        //Box here is TPC
        _myGeoAABox.Min( 0 + fidvol_dist,
                         -(::larutil::Geometry::GetME()->DetHalfHeight()) + fidvol_dist_y,
                         0 + fidvol_dist);

        _myGeoAABox.Max( 2 * (::larutil::Geometry::GetME()->DetHalfWidth()) - fidvol_dist,
                         ::larutil::Geometry::GetME()->DetHalfHeight() - fidvol_dist_y,
                         ::larutil::Geometry::GetME()->DetLength() - fidvol_dist);


        return true;
    }

    bool TestSpline::analyze(storage_manager* storage) {


        auto ev_mctruth = storage->get_data<event_mctruth>("generator");
        if (!ev_mctruth) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, mctruth!"));
            return false;
        }
        if (ev_mctruth->size() != 1) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("MCTruth size is not equal to 1... it equals %lu!", ev_mctruth->size()));
            return false;
        }


        auto ev_mctrack = storage->get_data<event_mctrack>("mcreco");
        if (!ev_mctrack) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, mctrack!"));
            return false;
        }


        const mcnu neutrino = ev_mctruth->at(0).GetNeutrino();
        bool _is_numuCC = !neutrino.CCNC() && neutrino.Nu().PdgCode() == 14 && neutrino.Lepton().PdgCode() == 13;
        if (!_is_numuCC) return false;

        size_t n_muon_mctracks = 0;
        mctrack mutrack;
        for (auto const& mct : *ev_mctrack) {
            if (mct.PdgCode() == 13 && mct.size() >= 2) {
                mutrack = mct;
                n_muon_mctracks++;
            }
        }

        if (mutrack.size() < 2) return false;

        if ( !_myGeoAABox.Contain(::geoalgo::Vector(mutrack.front().Position().Vect())) ||
                !_myGeoAABox.Contain(::geoalgo::Vector(mutrack.back().Position().Vect()))) return false;
        if (n_muon_mctracks > 1) return false;

        double tracklen = ::geoalgo::Vector(mutrack.front().Position().Vect()).Dist(
                              ::geoalgo::Vector(mutrack.back().Position().Vect()));

        double mu_depE = (mutrack.front().E() - mutrack.back().E()) / 1000.;
        double mu_splineE = myspline->GetMuMomentum(tracklen) / 1000.;

        _h->Fill(mu_depE, mu_splineE);


        return true;
    }

    bool TestSpline::finalize() {

        if (_fout) {
            _h->Write();
        }
        return true;
    }

}
#endif
