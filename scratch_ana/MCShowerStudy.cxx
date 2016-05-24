#ifndef LARLITE_MCSHOWERSTUDyY_CXX
#define LARLITE_MCSHOWERSTUDyY_CXX

#include "MCShowerStudy.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/shower.h"

namespace larlite {

    bool MCShowerStudy::initialize() {

        if (!_ana_tree) {
            _ana_tree = new TTree("mcs_tree", "mcs_tree");
            _ana_tree->Branch("reco_dedx", &_reco_dedx, "reco_dedx/D");
            _ana_tree->Branch("reco_z_vtx", &_reco_z_vtx, "reco_z_vtx/D");
            _ana_tree->Branch("reco_y_vtx", &_reco_y_vtx, "reco_y_vtx/D");
            _ana_tree->Branch("reco_x_vtx", &_reco_x_vtx, "reco_x_vtx/D");
            _ana_tree->Branch("reco_E", &_reco_E, "reco_E/D");
            _ana_tree->Branch("n_reco_showers", &_n_reco_showers, "n_reco_showers/I");
            _ana_tree->Branch("mc_reco_angle", &_mc_reco_angle, "mc_reco_angle/D");
            _ana_tree->Branch("mc_dedx", &_mc_dedx, "mc_dedx/D");
            _ana_tree->Branch("detprof_E", &_detprof_E, "detprof_E/D");
            _ana_tree->Branch("detprof_z_vtx", &_detprof_z_vtx, "detprof_z_vtx/D");
            _ana_tree->Branch("detprof_y_vtx", &_detprof_y_vtx, "detprof_y_vtx/D");
            _ana_tree->Branch("detprof_x_vtx", &_detprof_x_vtx, "detprof_x_vtx/D");
            _ana_tree->Branch("start_x_vtx", &_start_x_vtx, "start_x_vtx/D");
        }
        return true;
    }

    bool MCShowerStudy::analyze(storage_manager* storage) {

        _start_x_vtx = -999;
        _detprof_x_vtx = -999;
        _detprof_y_vtx = -999;
        _detprof_z_vtx = -999;
        _detprof_E = -999;
        _mc_dedx = -999;
        _mc_reco_angle = -999;
        _n_reco_showers = -999;
        _reco_E = -999;
        _reco_x_vtx = -999;
        _reco_y_vtx = -999;
        _reco_z_vtx = -999;
        _reco_dedx = -999;

        auto ev_mcs = storage->get_data<event_mcshower>(_producername);
        if (!ev_mcs) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, MCShower!"));
            return false;
        }
        if (ev_mcs->size() != 1)
            return false;

        auto ev_s = storage->get_data<event_shower>("recoemu");
        if (!ev_s) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, Shower!"));
            return false;
        }

        auto mcs = ev_mcs->at(0);
        _start_x_vtx = mcs.Start().X();
        _detprof_x_vtx = mcs.DetProfile().X();
        _detprof_y_vtx = mcs.DetProfile().Y();
        _detprof_z_vtx = mcs.DetProfile().Z();
        _detprof_E = mcs.DetProfile().E();
        _mc_dedx = mcs.dEdx();

        _n_reco_showers = ev_s->size();
        if (!_n_reco_showers) {
            _ana_tree->Fill();
            return false;
        }

        auto shower = ev_s->at(0);
        _mc_reco_angle = shower.Direction().Angle(mcs.DetProfile().Momentum().Vect()) * (180./3.14159);
        _reco_E = shower.Energy();
        _reco_x_vtx = shower.ShowerStart().X();
        _reco_y_vtx = shower.ShowerStart().Y();
        _reco_z_vtx = shower.ShowerStart().Z();
        _reco_dedx = shower.dEdx();

        _ana_tree->Fill();

        return true;
    }

    bool MCShowerStudy::finalize() {

        if (_fout) {
            _fout->cd();
            if (_ana_tree)
                _ana_tree->Write();

        }
        return true;
    }

}
#endif
