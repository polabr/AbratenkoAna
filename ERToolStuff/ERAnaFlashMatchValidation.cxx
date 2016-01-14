#ifndef ERTOOL_ERANAFLASHMATCHVALIDATION_CXX
#define ERTOOL_ERANAFLASHMATCHVALIDATION_CXX

#include "ERAnaFlashMatchValidation.h"

namespace ertool {

  ERAnaFlashMatchValidation::ERAnaFlashMatchValidation(const std::string& name) : AnaBase(name)
  {
    _time_diff = 0;
  }

  void ERAnaFlashMatchValidation::Reset()
  {}

  void ERAnaFlashMatchValidation::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAnaFlashMatchValidation::ProcessBegin()
  {

    _time_diff = new TH1D("time_diff", "Matched Flash vs. MCTrack", 100, 0, 500);

    n_matched_flashes = 0;
    n_qclusters = 0;

  }

  bool ERAnaFlashMatchValidation::Analyze(const EventData &data, const ParticleGraph &graph)
  {

    //Loop over all base particles
    //Since ERAlgoFlashMatch runs before this ana unit and matches a flash with a base particle,
    //Count the number of base particles with a flash associated, and call this "n_matched_flashes"
    //Also count the number of "base" particles in total, because each base particle
    //leads to one QCluster being reconstructed, we can call this "n_qclusters"
    //(ERAlgoFlashMatch takes each base particles and merges together qclusters from
    // all of the base particles' children)

    //We also need to fill the _time_diff histogram
    //which requires the mc time for each flash.
    // _time_diff->Fill(1000 * (_flash_time - _mc_time));
    //When a flash is found, get the root particle associated with it, grab the
    //mctrack associated with the root particle, and get the mc time from it
    //Since this code is designed to work for mctrack input, I will use the recoparticle
    //graph to get the _mc_time, but if someone wants to run this with reco tracks, they
    //will have to figure out how to find that track in the mcparticlegraph (nontrivial).


    for (auto const& base_node_id : graph.GetBaseNodes() ) {

      n_qclusters++;

      auto const& base_part = graph.GetParticle(base_node_id);

      auto flashID = base_part.FlashID();

      if (flashID != kINVALID_FLASH_ID) {
        n_matched_flashes++;
        float flash_time = data.Flash(flashID)._t;
        float mc_time = 1.e-3 * data.Track(base_part)._time;
        // std::cout << "Flash time is " << flash_time;
        // std::cout << "MC time is " << mc_time;
        _time_diff->Fill(1000 * (flash_time - mc_time));
      }

    }// End loop over BaseNodes

    return true;
  }

  void ERAnaFlashMatchValidation::ProcessEnd(TFile* fout)
  {

    std::cout << "\nEfficiency (#matches/#interactions)  : "
              << n_matched_flashes / n_qclusters * 100
              << "%, (" << n_matched_flashes << "/"
              << n_qclusters << ")" << std::endl;
    std::cout << "Correctness (#good matches/#matches) : "
              << float(_time_diff->Integral(1, 80)) / n_matched_flashes * 100
              << "%, (" << _time_diff->Integral(1, 80) << "/"
              << n_matched_flashes << ")" << std::endl;


  }

}

#endif
