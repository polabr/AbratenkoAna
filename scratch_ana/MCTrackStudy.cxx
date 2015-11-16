#ifndef LARLITE_MCTRACKSTUDY_CXX
#define LARLITE_MCTRACKSTUDY_CXX

#include "MCTrackStudy.h"
#include "DataFormat/mctrack.h"

namespace larlite {

bool MCTrackStudy::initialize() {
    _start_x_vtx = -9e9;

    if (!_ana_tree) {
        _ana_tree = new TTree("ana_tree", "ana_tree");
        _ana_tree->Branch("start_x_vtx", &_start_x_vtx, "start_x_vtx/D");
    }
    return true;
}

bool MCTrackStudy::analyze(storage_manager* storage) {

    _start_x_vtx = -9e9;

    auto ev_mct = storage->get_data<event_mctrack>("mcreco");
    if (!ev_mct) {
        print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, mctrack!"));
        return false;
    }
    if (!ev_mct->size()) {
        print(larlite::msg::kERROR, __FUNCTION__, Form("Zero mctracks in this event!"));
        return false;
    }

    for (auto const& mct : *ev_mct) {
        //If mctrack has no size, it never enters detector
        if (!mct.size()) continue;
        _start_x_vtx = mct.at(0).X();
        _ana_tree->Fill();
    }



    return true;
}

bool MCTrackStudy::finalize() {

    if (_fout) {
        _fout->cd();
        if (_ana_tree)
            _ana_tree->Write();

    }
    return true;
}

}
#endif
