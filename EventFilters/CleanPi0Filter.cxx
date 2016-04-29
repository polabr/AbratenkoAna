#ifndef LARLITE_CLEANPI0FILTER_CXX
#define LARLITE_CLEANPI0FILTER_CXX

#include "CleanPi0Filter.h"
#include "DataFormat/mcshower.h"

namespace larlite {

    bool CleanPi0Filter::initialize() {
        n_total_evts = 0;
        n_kept_events = 0;
        return true;
    }

    bool CleanPi0Filter::analyze(storage_manager* storage) {
        n_total_evts++;
        //Grab the MCShowers
        auto ev_mcshower = storage->get_data<event_mcshower>("mcreco");
        if (!ev_mcshower) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, mcshower!"));
            return false;
        }
        if (ev_mcshower->size() != 2)
            return false;


        //Make sure all MC showers are fully contained
        for (auto &mcs : *ev_mcshower) {
            if (mcs.DetProfile().E() / mcs.Start().E() < 0.90 ) return false;
            if (mcs.DetProfile().E() < 50.) return false;
        }

        n_kept_events++;
        return true;
    }

    bool CleanPi0Filter::finalize() {
        std::cout << "n_total_evts = " << n_total_evts << std::endl;
        std::cout << "n_kept_events = " << n_kept_events << std::endl;

        return true;
    }

}
#endif
