#ifndef MBLEESELECTIONFILTER_CXX
#define MBLEESELECTIONFILTER_CXX

#include "MBLEESelectionFilter.h"
#include "DataFormat/mctruth.h"

namespace larlite {


bool MBLEESelectionFilter::initialize() {

  total_events = 0;
  kept_events = 0;

  std::cout << _cherenk_thresholds_MEV[11];
  return true;
}

bool MBLEESelectionFilter::analyze(storage_manager* storage) {

  //Grab the MCTruth
  auto ev_mctruth = storage->get_data<event_mctruth>("generator");
  if (!ev_mctruth) {
    print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, mctruth!"));
    return false;
  }

  //There should only be one element in ev_mctruth
  if (ev_mctruth->size() != 1) {
    print(larlite::msg::kERROR, __FUNCTION__, Form("More than one mctruth in this event??"));
    return false;
  }

  total_events++;

  ////////////////////////////////////////////////////////////////////////
  //Enforce that there is exactly 1 electron above cherenkov threshold
  //If there isn't, skip this event (return false)
  //Also count other particles above cherenkov threshold (extra 'subevents'),
  //and enforce there are none of these
  size_t n_electrons = 0;
  size_t n_additional_subevents = 0;

  auto &particles = ev_mctruth->at(0).GetParticles();

  for (auto const& particle : particles) {
    //Note, when counting final state particles, ignore any particles that don't have status code == 1
    if (particle.StatusCode() != 1) continue;

    double E_MEV  = particle.Trajectory().at(0).E() * 1000.;
    //double KE_MEV = E_MEV - (particle.Mass() * 1000.);
    size_t PDG = abs(particle.PdgCode());
    //Make sure I have a recorded cherenkov threshold for this type of particle
    if (!_cherenk_thresholds_MEV.count(PDG))
      throw std::invalid_argument( Form("PDG %zu not in std::map of cherenkov thresholds!", PDG) );

    //If this particle is below cherenkov threshold, ignore it
    if ( E_MEV < _cherenk_thresholds_MEV[PDG]) continue;

    //If this particle is an electron above minimum energy cut, count it
    if (PDG == 11 || E_MEV > _min_vis_energy_MEV) n_electrons++;
    else n_additional_subevents++;
  }

  if (n_electrons != 1 || n_additional_subevents)
    return false;
  //
  ////////////////////////////////////////////////////////////////////////

  //If you get here, the event has 1 good electron and only 1 subevent (the electron)
  kept_events++;
  return true;
}

bool MBLEESelectionFilter::finalize() {

  std::cout << "Total events = " << total_events << std::endl;
  std::cout << "Final kept events = " << kept_events << std::endl;

  return true;
}

}
#endif
