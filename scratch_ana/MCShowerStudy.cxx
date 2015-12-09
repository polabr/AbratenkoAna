#ifndef LARLITE_MCSHOWERSTUDyY_CXX
#define LARLITE_MCSHOWERSTUDyY_CXX

#include "MCShowerStudy.h"
#include "DataFormat/mcshower.h"

namespace larlite {

bool MCShowerStudy::initialize() {
    _start_x_vtx = -9e9;
    _detprof_x_vtx = -9e9;
    if (!_ana_tree) {
        _ana_tree = new TTree("mcs_tree", "mcs_tree");
        _ana_tree->Branch("start_x_vtx", &_start_x_vtx, "start_x_vtx/D");
        _ana_tree->Branch("detprof_x_vtx", &_detprof_x_vtx, "detprof_x_vtx/D");
        _ana_tree->Branch("detprof_E", &_detprof_E, "detprof_E/D");
    }
    return true;
}

bool MCShowerStudy::analyze(storage_manager* storage) {

    _start_x_vtx = -9e9;
    _detprof_x_vtx = -9e9;
    _detprof_E = -9e9;

    auto ev_mcs = storage->get_data<event_mcshower>(_producername);
    if (!ev_mcs) {
        print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, MCShower!"));
        return false;
    }
    if (!ev_mcs->size())
        return false;


    for (auto const& mcs : *ev_mcs) {
        _start_x_vtx = mcs.Start().X();
        _detprof_x_vtx = mcs.DetProfile().X();
        _detprof_E = mcs.DetProfile().E();
        if (!_ana_tree) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("what the FUCK why is there no ana tree?!"));
            return false;
        }
        _ana_tree->Fill();
    }

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
