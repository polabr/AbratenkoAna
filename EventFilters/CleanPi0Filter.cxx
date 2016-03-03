#ifndef LARLITE_CLEANPI0FILTER_CXX
#define LARLITE_CLEANPI0FILTER_CXX

#include "CleanPi0Filter.h"
#include "DataFormat/mcshower.h"

namespace larlite {

  bool CleanPi0Filter::initialize() {

    return true;
  }
  
  bool CleanPi0Filter::analyze(storage_manager* storage) {
   //Grab the MCShowers
    auto ev_mcshower = storage->get_data<event_mcshower>("mcreco");    
    if(!ev_mcshower) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mcshower!"));
      return false;
    }
    if(ev_mcshower->size() != 2)
        return false;


    //Make sure all MC showers are fully contained
    for (auto &mcs : *ev_mcshower){
        if (mcs.DetProfile().E()/mcs.Start().E() < 0.90 ) return false;
        if (mcs.DetProfile().E() < 10.) return false;
    }
  
    return true;
  }

  bool CleanPi0Filter::finalize() {

  
    return true;
  }

}
#endif
