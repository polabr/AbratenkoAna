#ifndef ERTOOL_ERANAFLASHMATCHVALIDATION_CXX
#define ERTOOL_ERANAFLASHMATCHVALIDATION_CXX

#include "ERAnaFlashMatchValidation.h"

namespace ertool {

  ERAnaFlashMatchValidation::ERAnaFlashMatchValidation(const std::string& name) : AnaBase(name), _time_diff(nullptr)
  {
//    _time_diff = 0;
  }

  void ERAnaFlashMatchValidation::Reset()
  {}

  void ERAnaFlashMatchValidation::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAnaFlashMatchValidation::ProcessBegin()
  {

    _time_diff = new TH1D("time_diff", "Matched Flash vs. MCTrack", 100, 0, 500);
    // _time_diff = new TH1D("time_diff", "Matched Flash vs. MCTrack", 100, -5000000, 5000000);

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
    // _time_diff->Fill(1000 * (flash_time - mc_time));
    //When a flash is found, get the root particle associated with it, grab the
    //mctrack associated with the root particle, and get the mc time from it
    //Since this code is designed to work for mctrack input, I will use the recoparticle
    //graph to get the mc_time, but if someone wants to run this with reco tracks, they
    //will have to figure out how to find that track in the mcparticlegraph (nontrivial).


    // Let's try using MCParticleGraph to see the output
    // Get MC particle set
    auto const& mc_graph = MCParticleGraph();
    // Get the MC data
    auto const& mc_data = MCEventData();



    for (auto const& base_node_id : graph.GetBaseNodes() ) {
      // for (auto const& base_node_id : mc_graph.GetBaseNodes() ) {

      auto const& base_part = graph.GetParticle(base_node_id);
      // auto const& base_part = mc_graph.GetParticle(base_node_id);

      //To copy UBT0Finder, which only looks for matches to mctracks, ignore
      //base particles that aren't kTrack
      if (base_part.RecoType() != kTrack) continue;

      // Not exactly sure what this means but I am copying what is done in MCQCluster
      if (base_part.RecoType() == kTrack &&
          (data.Track(base_part)._time < -2050000 || data.Track(base_part)._time > 2750000 ))
        continue;

      n_qclusters++;

      auto flashID = base_part.FlashID();

      if (flashID != kINVALID_FLASH_ID) {
        n_matched_flashes++;
        double flash_time = data.Flash(flashID)._t;
        double mc_time = 1.e-3 * data.Track(base_part)._time;
        // std::cout << "Flash found .Track first point is " << data.Track(base_part).front() << std::endl;
        // std::cout << "Flash found .Track end point is is " << data.Track(base_part).back() << std::endl;
        // std::cout<<"mc and flash time for match : "<<mc_time<<", "<<flash_time<<std::endl;
        // std::cout<<"  -- mct start x is "<<data.Track(base_part).front().at(0)<<std::endl;
        // double mc_time = 1.e-3 * mc_data.Track(base_part)._time;

        if (mc_time < -2050 || mc_time > 2750)
          continue;

        _time_diff->Fill(1000 * (flash_time - mc_time));
      }

    }// End loop over BaseNodes

    return true;
  }

  void ERAnaFlashMatchValidation::ProcessEnd(TFile* fout)
  {

    std::cout << "\nEfficiency (#matches/#interactions)  : "
              << (float)n_matched_flashes / n_qclusters * 100
              << "%, (" << n_matched_flashes << "/"
              << n_qclusters << ")" << std::endl;
    std::cout << "Correctness (#good matches/#matches) : "
              << float(_time_diff->Integral(1, 80)) / n_matched_flashes * 100
              << "%, (" << _time_diff->Integral(1, 80) << "/"
              << n_matched_flashes << ")" << std::endl;

    fout->cd();
    _time_diff->Write();

  }

}

#endif
