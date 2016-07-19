#ifndef LARLITE_QUICKMIKEPLOT_CXX
#define LARLITE_QUICKMIKEPLOT_CXX

#include "QuickMikePlot.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/mctruth.h"
#include "DataFormat/mcflux.h"

namespace larlite {

    bool QuickMikePlot::initialize() {

        _fidvolBox = FidVolBox();

        _tree = new TTree("tree", "tree");
        _tree->Branch("true_nu_E", &_true_nu_E, "true_nu_E/D");
        _tree->Branch("true_mu_E", &_true_mu_E, "true_mu_E/D");
        _tree->Branch("true_mu_len", &_true_mu_len, "true_mu_len/D");
        _tree->Branch("fndecay", &_fndecay, "fndecay/I");
        _tree->Branch("CCNC", &_CCNC, "CCNC/O");
        _tree->Branch("nu_invidvol", &_nu_infidvol, "nu_infidvol/O");
        _tree->Branch("nupdg", &_nupdg, "nupdg/I");
        _tree->Branch("nu_vtx_x", &_nu_vtx_x, "nu_vtx_x/D");
        _tree->Branch("nu_vtx_y", &_nu_vtx_y, "nu_vtx_y/D");
        _tree->Branch("nu_vtx_z", &_nu_vtx_z, "nu_vtx_z/D");

        return true;
    }

    bool QuickMikePlot::analyze(storage_manager* storage) {

        _CCNC = false;
        _nupdg = -999;
        _fndecay = -999;
        _true_nu_E = -999.;
        _true_mu_E = -999.;
        _true_mu_len = -999.;
        _nu_infidvol = false;
        _nu_vtx_x = -999.;
        _nu_vtx_y = -999.;
        _nu_vtx_z = -999.;

        auto ev_mctruth = storage->get_data<event_mctruth>("generator");
        if (!ev_mctruth) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, mctruth!"));
            return false;
        }
        if (ev_mctruth->size() != 1) {
            // print(larlite::msg::kERROR, __FUNCTION__, Form("MCTruth size is not equal to 1... it equals %lu!", ev_mctruth->size()));
            return false;
        }
        auto const& nu_vtx = ev_mctruth->at(0).GetNeutrino().Nu().Trajectory().front().Position().Vect();
        _nu_sphere.Center(nu_vtx);
        _nu_sphere.Radius(0.5);

        _true_nu_E = ev_mctruth->at(0).GetNeutrino().Nu().Trajectory().front().E();
        _CCNC = ev_mctruth->at(0).GetNeutrino().CCNC();
        _nupdg = ev_mctruth->at(0).GetNeutrino().Nu().PdgCode();
        _nu_infidvol = _fidvolBox.Contain(nu_vtx);
        _nu_vtx_x = nu_vtx.X();
        _nu_vtx_y = nu_vtx.Y();
        _nu_vtx_z = nu_vtx.Z();


        auto ev_mctrack = storage->get_data<event_mctrack>("mcreco");
        if (!ev_mctrack) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, mctrack!"));
            return false;
        }
        auto ev_mcflux = storage->get_data<event_mcflux>("generator");
        if (!ev_mcflux) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, mcflux!"));
            return false;
        }
        // Require exactly one neutrino interaction
        if (ev_mcflux->size() != 1) {
            print(larlite::msg::kINFO, __FUNCTION__, Form("ev_mcflux size is not 1!"));
            return false;
        }
        _fndecay = ev_mcflux->at(0).fndecay;

        // bool _found_muon = false;
        // mctrack mu_mctrack;
        // for (auto const& mct : *ev_mctrack) {

        //     if (!mct.size()) continue;

        //     // Only count mctracks that stat within 1cm of the true neutrino vertex
        //     if ( !_nu_sphere.Contain(mct.front().Position().Vect()) )
        //         continue;

        //     if (mct.PdgCode() == 13 ) {
        //         //if you've already found a muon mctrack, return false
        //         if (_found_muon) return false;
        //         _found_muon = true;
        //         mu_mctrack = mct;
        //     }

        // }
        // if(!_found_muon) return false;
        // _true_mu_E = mu_mctrack.front().E();
        // _true_mu_len = (mu_mctrack.front().Position().Vect() - mu_mctrack.back().Position().Vect()).Mag();

        _tree->Fill();

        return true;
    }

    bool QuickMikePlot::finalize() {

        if (_fout) {
            _tree->Write();
        }
        return true;
    }

}
#endif
