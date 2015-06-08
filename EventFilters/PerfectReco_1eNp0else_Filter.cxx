#ifndef LARLITE_PERFECTRECO_1ENP0ELSE_FILTER_CXX
#define LARLITE_PERFECTRECO_1ENP0ELSE_FILTER_CXX

#include "PerfectReco_1eNp0else_Filter.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/mcshower.h"

namespace larlite {

  bool PerfectReco_1eNp0else_Filter::initialize() {

    total_events = 0;
    kept_events = 0;

    return true;
  }
  
  bool PerfectReco_1eNp0else_Filter::analyze(storage_manager* storage) {
  
    total_events++;

    //Grab the MCTracks
    auto ev_mct = storage->get_data<event_mctrack>("mcreco");
    if(!ev_mct) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctrack!"));
      return false;
    }
    //Grab the MCShowers
    auto ev_mcs = storage->get_data<event_mcshower>("mcreco");
    if(!ev_mcs) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mcshower!"));
      return false;
    }

    size_t n_electrons = 0;
    //Loop over MCShowers
    for(auto const& mcs : *ev_mcs){

      double KE = mcs.Start().E() - mcs.Start().Momentum().M();

      //If there are any mcshowers that aren't electrons
      //that have > 20MeV KE, skip this event
      if ( abs(mcs.PdgCode()) != 11 && KE > 0.02 )
	return false;

      //Enforce that there is exactly 1 electron, above 20MeV kinetic energy
      if ( abs(mcs.PdgCode()) == 11 && KE > 0.02 )
	n_electrons++;
    }


    //Loop over MCTracks
    for(auto const& mcs : *ev_mct){
      double KE = mcs.Start().E() - mcs.Start().Momentum().M();
      //If there are any mctracks that have >20 MEV kinetic energy
      //and are not protons, skip this event
      if( abs(mcs.PdgCode()) != 2212 && KE > 0.02 )
	return false;
    }


    //Skip this event if the number of electrons is not 1
    if ( n_electrons != 1 )
      return false;

    kept_events++;
    return true;
  }

  bool PerfectReco_1eNp0else_Filter::finalize() {
  
    std::cout<<"PerfectReco_1eNp0else_Filter: Total events = "<<total_events<<std::endl;
    std::cout<<"PerfectReco_1eNp0else_Filter: Kept events = "<<kept_events<<std::endl;
    return true;
  }

}
#endif
