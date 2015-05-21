#ifndef LARLITE_NPNN1L0ELSEFILTER_CXX
#define LARLITE_NPNN1L0ELSEFILTER_CXX

#include "NpNn1l0elseFilter.h"

namespace larlite {

  bool NpNn1l0elseFilter::initialize() {
    evt_counter = 0;
    return true;
  }
  
  bool NpNn1l0elseFilter::analyze(storage_manager* storage) {
  
    auto ev_tru = storage->get_data<event_mctruth >("generator");
    size_t int_type = ev_tru->at(0).GetNeutrino().InteractionType();
    if(int_type == 1001) return false;

    auto mcnu_traj = ev_tru->at(0).GetNeutrino().Nu().Trajectory();

    auto mcpart_list = ev_tru->at(0).GetParticles();
    size_t n_lep = 0;

    for (auto const mcp : mcpart_list){
      
      if (mcp.StatusCode() == 1){  
	
	if(abs(mcp.PdgCode()) == 13 || abs(mcp.PdgCode()) == 11){
	  n_lep++;  
	}
	
	//if any of these particles are in the final state, quit
	if( mcp.PdgCode() == 22 || //gamma
	    abs(mcp.PdgCode()) == 211 || //pion
	    mcp.PdgCode() == 111 || //pizero
	    abs(mcp.PdgCode()) == 321 ){ //kaon
	  return false;
	}
	
      }
    }

    if(n_lep != 1) return false;
    
    evt_counter++;

    //print out event info
    /*
    std::cout<<"-------------- printing particles ----------------"<<std::endl;
    std::cout<<"Note: intxn type is "<<int_type<<std::endl;
    for (auto const mcp : mcpart_list){
      std::cout<<" PDGS: ";
      if (mcp.StatusCode() == 1)
	std::cout<<mcp.PdgCode()<<", ";
      std::cout<<"."<<std::endl;
    }

    std::cout<<"-------------- end printing particles ----------------"<<std::endl;
    */

    return true;
  }

  bool NpNn1l0elseFilter::finalize() {
    std::cout<<"NpNn1l0elseFilter kept this many events: "<<evt_counter<<std::endl;
      
    return true;
  }

}
#endif
