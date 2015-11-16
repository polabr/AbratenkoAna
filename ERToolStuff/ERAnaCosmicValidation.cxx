#ifndef ERTOOL_ERANACOSMICVALIDATION_CXX
#define ERTOOL_ERANACOSMICVALIDATION_CXX

#include "ERAnaCosmicValidation.h"

namespace ertool {

ERAnaCosmicValidation::ERAnaCosmicValidation(const std::string& name)
  : AnaBase(name)
  , _result_tree(nullptr)
{

  PrepareTreeVariables();

  // set default energy cut (for counting) to 0
  _eCut = 0;
}


bool ERAnaCosmicValidation::Analyze(const EventData &data, const ParticleGraph &ps)
{

  ResetTreeVariables();

  // //Loop through particle graph
  // auto const& particles = ps.GetParticleArray();
  // for ( auto const & p : particles ) {
  //   //If this particle was tagged as "cosmic"
  //   if ( p.ProcessType() == ProcessType_t::kCosmic )
  //     std::cout << "This particle was tagged as kCosmic!" << std::endl;
  //   else
  //     std::cout << "This particle was NOT tagged as cosmic." << std::endl;
  //   if ( p.RecoType() == RecoType_t::kTrack ) {
  //     std::cout << "This particle comes from a track!" << std::endl;
  //     auto mytrack = data.Track(p.RecoID());
  //     std::cout << Form("Start point of track is = (%f,%f,%f).\n", mytrack.at(0).at(0),
  //                       mytrack.at(0).at(1),
  //                       mytrack.at(0).at(2));
  //   }
  //   else if ( p.RecoType() == RecoType_t::kShower )
  //     std::cout << "This particle comes from a shower!" << std::endl;
  //   else
  //     std::cout << "This particle comes from neither a track, nor a shower!" << std::endl;
  // }

  // Get MC particle set
  auto const& mc_graph = MCParticleGraph();

  if (!mc_graph.GetParticleArray().size()) {
    std::cout << "WARNING: Size of mc particle graph is zero! Perhaps you forgot to include mctruth/mctrack/mcshower?" << std::endl;
    return false;
  }

  for ( auto const & mc : mc_graph.GetParticleArray() ) {
    // Find the primary cosmics from the mc graph
    if (GetParticleGeneration(mc) != MC_ParticleGeneration_t::kPrimary)
      continue;
  }
  return true;

}

ERAnaCosmicValidation::MC_ParticleGeneration_t ERAnaCosmicValidation::GetParticleGeneration(const ertool::Particle & myparticle) {

  if (myparticle.Generation() == 0)
    return MC_ParticleGeneration_t::kPrimary;
  else if (myparticle.Generation() > 0)
    return MC_ParticleGeneration_t::kSecondary;

  return MC_ParticleGeneration_t::kOther;
  
}

void ERAnaCosmicValidation::ProcessEnd(TFile * fout)
{

  if (fout) {
    fout->cd();
    _result_tree->Write();
  }

  return;

}

void ERAnaCosmicValidation::PrepareTreeVariables() {

  if (_result_tree) { delete _result_tree; }

  _result_tree = new TTree("CosmicValidationTree", "CosmicValidationTree");
  _result_tree->Branch("_is_track", &_is_track, "_is_track/O");
  _result_tree->Branch("_true_primary", &_true_primary, "_true_primary/O");
  _result_tree->Branch("_true_secondary", &_true_secondary, "_true_secondary/O");
  _result_tree->Branch("_reco_primary", &_reco_primary, "_reco_primary/O");
  _result_tree->Branch("_reco_secondary", &_reco_secondary, "_reco_secondary/O");

  return;
}

void ERAnaCosmicValidation::ResetTreeVariables() {

  _is_track = false;
  _true_primary = false;
  _true_secondary = false;
  _reco_primary = false;
  _reco_secondary = false;

  return;
}

}

#endif
