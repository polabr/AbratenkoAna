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

  for (size_t i = 0; i < 3; ++i)
    _total_particles.push_back(0);
}


bool ERAnaCosmicValidation::Analyze(const EventData &data, const ParticleGraph &ps)
{

  /// Grab the reconstructed particle graph
  auto const& particles = ps.GetParticleArray();
  /// Grab the mc particle graph
  auto const& mc_graph = MCParticleGraph();

  if (!mc_graph.GetParticleArray().size()) {
    std::cout << "WARNING: Size of mc particle graph is zero! Perhaps you forgot to include mctruth/mctrack/mcshower?" << std::endl;
    return false;
  }

  //Loop through reconstructed particle graph
  for ( auto const & p : particles ) {

    // Reset ttree variables (since we will be filling the TTree once per particle)
    ResetTreeVariables();

    _is_track = p.RecoType() == RecoType_t::kTrack;
    //Particle is recod "primary" if it is tagged as cosmic, and tagged as a primary
    _reco_primary = (p.ProcessType() == ProcessType_t::kCosmic) && p.Primary();
    //Particle is recod "secondary" if it is a descdendant, and its parent is a cosmic
    _reco_secondary = p.Descendant() && ps.GetParticle(p.Parent()).ProcessType() == ProcessType_t::kCosmic;

    /// Find the same particle in the mcparticle event data and fill some more ttree variables
    /// that have to come from the mcparticle event data.
    ertool::Particle found_mcparticle;
    try {
      found_mcparticle = FindMCParticleGraphParticle(data, p);
      // Check the generation of the identified mcparticlegraph node
      _true_primary = GetMCParticleGeneration(found_mcparticle) == MC_ParticleGeneration_t::kPrimary;
      _true_secondary = GetMCParticleGeneration(found_mcparticle) == MC_ParticleGeneration_t::kSecondary;
      // Increment counters
      if (_true_primary) _total_particles.at(0)++;
      else if (_true_secondary) _total_particles.at(1)++;
      else _total_particles.at(2)++;
      // Fill ttree
      _result_tree->Fill();
    }
    catch (ertool::ERException& e) {
      std::cout << e.msg() << std::endl;
      std::cout << "Skipping this particle." << std::endl;
      continue;
    }
  }// End loop over reco particle graph

  return true;

} // end analyze function

ertool::Particle ERAnaCosmicValidation::FindMCParticleGraphParticle(const EventData &data, const ertool::Particle &p) {

  /// Grab the mc particle graph
  auto const& mc_graph = MCParticleGraph();
  /// Grab the mc event data
  auto const& mc_data = MCEventData();

  /// Loop through the MCParticleGraph and find the node that corresponds to this particle
  bool found_match = false;

  /// Container to store the particle from mcparticlegraph that matches my reco particle
  ertool::Particle found_mcparticle;

  for ( auto const & mcp : mc_graph.GetParticleArray() ) {

    found_mcparticle = mcp;

    //If we're looking for a track
    if ( _is_track ) {
      //and this mcparticlegraph node doesn't correspond to a track... skip
      if (mcp.RecoType() != RecoType_t::kTrack) continue;
      //Check that the track we're looking for matches exactly the mcparticlegraph's track:
      //Check the start x, y, z coordinates all agree
      if (mc_data.Track(mcp).at(0).at(0) != data.Track(p).at(0).at(0) ||
          mc_data.Track(mcp).at(0).at(1) != data.Track(p).at(0).at(1) ||
          mc_data.Track(mcp).at(0).at(2) != data.Track(p).at(0).at(2)) continue;
      //Check energy agrees (note, mc_data.Track(mcp)._time is unfilled (1e308), but data.Track(p)._time IS filled)
      if (mc_data.Track(mcp)._energy != data.Track(p)._energy) continue;
      found_match = true;
    }
    else { //If we're looking for a shower
      //and this mcparticlegraph node corresponds to a track... skip
      if (mcp.RecoType() != RecoType_t::kShower) continue;
      //Check that the shower we're looking for matches exactly the mcparticlegraph's shower:
      //Check the start point coordinates all agree
      if (mc_data.Shower(mcp).Start() != data.Shower(p).Start()) continue;
      //Check energy, dedx agree
      //(note, mc_data.Shower(mcp)._time is unfilled (1e308), but data.Shower(p)._time IS filled)
      if (mc_data.Shower(mcp)._energy != data.Shower(p)._energy ||
          mc_data.Shower(mcp)._dedx != data.Shower(p)._dedx ) continue;
      found_match = true;
    }

    /// Note I have verified that only one match is ever found, so breaking here is fine to save computing time
    if (found_match) break;

  } // End loop over mcparticlegraph to find the mcparticlegraph node corresponding to this particle

  if (found_match) return found_mcparticle;
  else {
    throw ertool::ERException(Form("FindMCParticleGraphParticle failed to find a match! Particle info: %s.", p.Print().c_str()));
    return ertool::Particle();
  }

}

/// Simple function that takes in a particle from mcparticlegraph
/// and returns the MC generation of that particle (to tell if it's primary, secondary, etc)
ERAnaCosmicValidation::MC_ParticleGeneration_t ERAnaCosmicValidation::GetMCParticleGeneration(const ertool::Particle & myparticle) {

  if (myparticle.Generation() == 0)
    return MC_ParticleGeneration_t::kPrimary;
  else if (myparticle.Generation() > 0)
    return MC_ParticleGeneration_t::kSecondary;
  return MC_ParticleGeneration_t::kOther;

}

void ERAnaCosmicValidation::ProcessEnd(TFile * fout)
{
  std::cout << "ERAnaCosmicValidation: Total particles analyzed counter:" << std::endl;
  std::cout << "    # Primary   = " << _total_particles.at(0) << std::endl;
  std::cout << "    # Secondary = " << _total_particles.at(1) << std::endl;
  std::cout << "    # Other     = " << _total_particles.at(2) << std::endl;
  std::cout << std::endl;

  if (fout) {
    fout->cd();
    _result_tree->Write();
  }
  return;
}

void ERAnaCosmicValidation::PrepareTreeVariables() {

  if (_result_tree) { delete _result_tree; }

  _result_tree = new TTree("cos_tree", "cos_tree");
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
