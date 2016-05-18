#ifndef LARLITE_TESTMULTISCATTERMOMENTUM_CXX
#define LARLITE_TESTMULTISCATTERMOMENTUM_CXX

#include "TestMultiScatterMomentum.h"
#include "DataFormat/mctrack.h"

namespace larlite {

    bool TestMultiScatterMomentum::initialize() {

        _tmc = TrackMomentumCalculator();

        if (!_ana_tree) {
            _ana_tree = new TTree("ana_tree", "ana_tree");
            _ana_tree->Branch("true_mom", &_true_mom, "true_mom/D");
            _ana_tree->Branch("mcs_reco_mom", &_mcs_reco_mom, "mcs_reco_mom/D");
            _ana_tree->Branch("len", &_length, "len/D");
        }

        return true;
    }

    bool TestMultiScatterMomentum::analyze(storage_manager* storage) {

        //Get the MCTracks
        auto ev_mctrack = storage->get_data<event_mctrack>("mcreco");
        if (!ev_mctrack) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, mctrack!"));
            return false;
        }
        if (ev_mctrack->size() != 1)
            return false;


        //Loop over the (one) MCTrack
        for (auto const& mct : *ev_mctrack) {
            _true_mom = mct.front().Momentum().Vect().Mag() / 1000.;
            _mcs_reco_mom = _tmc.GetMomentumMultiScatterLLHD(mct);
            _length = (mct.End().Position().Vect() - mct.Start().Position().Vect()).Mag();
            _ana_tree->Fill();
        }

        return true;
    }

    bool TestMultiScatterMomentum::finalize() {

        if (_fout) { _fout->cd(); _ana_tree->Write(); }

        else
            print(larlite::msg::kERROR, __FUNCTION__, "Did not find an output file pointer!!! File not opened?");


        if (_ana_tree)
            delete _ana_tree;

        return true;
    }

}
#endif
