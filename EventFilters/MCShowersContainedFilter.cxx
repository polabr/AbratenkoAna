#ifndef LARLITE_MCSHOWERSCONTAINEDFILTER_CXX
#define LARLITE_MCSHOWERSCONTAINEDFILTER_CXX

#include "MCShowersContainedFilter.h"
#include "DataFormat/mcshower.h"

namespace larlite {

  bool MCShowersContainedFilter::initialize() {

    return true;
  }
  
  bool MCShowersContainedFilter::analyze(storage_manager* storage) {
    //Grab the MCShowers
        auto ev_mcshower = storage->get_data<event_mcshower>("mcreco");
        if (!ev_mcshower) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, mcshower!"));
            return false;
        }
        
        if (!ev_mcshower->size()) return false;

        //Make sure all MC showers are fully contained
        for (auto &mcs : *ev_mcshower) {
            if (mcs.DetProfile().E() / mcs.Start().E() < 0.90 ) return false;
        }

    return true;
  }

  bool MCShowersContainedFilter::finalize() {

  
    return true;
  }

}
#endif
