#ifndef CCQEMCFILTER_CXX
#define CCQEMCFILTER_CXX

#include "CCQEMCFilter.h"

namespace larlite {

  size_t total_events = 0;
  size_t ccqe_events = 0;
  size_t kept_events = 0;

  bool CCQEMCFilter::initialize() {

    return true;
  }
  
  bool CCQEMCFilter::analyze(storage_manager* storage) {
  
    //Grab the MCTruth
    auto ev_mctruth = storage->get_data<event_mctruth>("generator");
    if(!ev_mctruth) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctruth!"));
      return false;
    }

    //There should only be one element in ev_mctruth
    if(ev_mctruth->size() != 1){
      print(larlite::msg::kERROR,__FUNCTION__,Form("More than one mctruth in this event??"));
      return false;
    }

    total_events++;
    
    //Enforce that the interaction number is 1001, this is CCQE
    size_t int_type = ev_mctruth->at(0).GetNeutrino().InteractionType();
    if(int_type != 1001) return false;

    ccqe_events++;

    //Enforce that there is exactly 1 electron and 1 proton, each above 20MeV kinetic energy
    //I don't care about neutrons, weird quarks, the neutrino, etc.
    size_t n_electrons = 0;
    size_t n_protons = 0;
    
    auto particles = ev_mctruth->at(0).GetParticles();
    //std::cout<<"Particle PDGs in this event (CCQE): ";
    for(auto const& particle : particles){
      double KE = particle.Trajectory().at(0).E()-particle.Mass();
      if(abs(particle.PdgCode()) == 11 && KE > 0.02)
	n_electrons++;
      if(abs(particle.PdgCode()) == 2212 && KE > 0.02)
	n_protons++;
      //	std::cout<<particle.PdgCode()<<" (Kinetic energy = "<< particle.Trajectory().at(0).E()-particle.Mass() << "), ";
    }
    // std::cout<<std::endl;

    if(n_electrons != 1 || n_protons != 1)
      return false;

    
    //If you get here, it is CCQE with 1 good electron and 1 good proton.
    kept_events++;
    return true;
  }

  bool CCQEMCFilter::finalize() {

    std::cout<<"Total events = "<<total_events<<std::endl;
    std::cout<<"Ccqe events = "<<ccqe_events<<std::endl;
    std::cout<<"Final kept events = "<<kept_events<<std::endl;

    return true;
  }

}
#endif
