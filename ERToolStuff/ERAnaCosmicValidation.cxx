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

  _potential_secondary_nodeIDs.clear();

}


bool ERAnaCosmicValidation::Analyze(const EventData &data, const ParticleGraph &ps)
{

  // First loop through the reconstructed cosmic primary particles just to get a list of all
  // MC secondaries that could potentially be associated with that particle
  // (this will be the denominator in our secondary tagger efficiency plot)
  _potential_secondary_nodeIDs.clear();
  ertool::Particle found_mcparticle;
  for ( auto const &pnodeid : ps.GetPrimaryNodes() ) {

    ertool::Particle primary = ps.GetParticle(pnodeid);

    if (primary.ProcessType() != ProcessType_t::kCosmic) continue;

    try { found_mcparticle = FindMCParticleGraphParticle(data, primary); }
    catch (ertool::ERException& e) { 
      std::cout << "Skipping particle (in building secondary particle list)" << std::endl; 
      continue; 
    }

    /// This function grabs the mc secondaries and adds them to a std::set to be used later
    TrackAllSecondaries(found_mcparticle);
  }



  /// Grab the reconstructed particle graph
  auto const& particles = ps.GetParticleArray();
  /// Grab the mc particle graph
  auto const& mc_graph = MCParticleGraph();

  if (!mc_graph.GetParticleArray().size()) {
    std::cout << "WARNING: Size of mc particle graph is zero! Perhaps you forgot to include mctruth/mctrack/mcshower?" << std::endl;
    return false;
  }

  //Loop through ALL particles in the reconstructed particle graph, fill ttree for each.
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

      // Check the generation of the identified mcparticlegraph node to see if it's truly primary
      _true_primary = IsMCParticlePrimary(found_mcparticle);// == MC_ParticleGeneration_t::kPrimary;

      // To check if this mcparticle is a "secondary", I need to define "secondary"
      // Secondary means this mcparticle is a child of the mcparticle tagged as a primary cosmic.
      // That way, even if the "primary" tagged particle wasn't actually primary, my ERAlgoCRSecondary has still
      // done what it's supposed to if it finds a particle that is a secondary of that incorrectly tagged "primary"
      _true_secondary = IsMCParticleSecondary(found_mcparticle);// == MC_ParticleGeneration_t::kSecondary;

      // Increment counters
      if (_true_primary) _total_particles.at(0)++;
      else if (_true_secondary) _total_particles.at(1)++;
      else _total_particles.at(2)++;
      // Fill ttree
      _result_tree->Fill();
    }

    catch (ertool::ERException& e) {
      // std::cout << e.msg() << std::endl;
      std::cout << "Skipping this particle (in main particle loop)." << std::endl;
      continue;
    }
  }// End loop over reco particle graph

  return true;

} // end analyze function

/// Simple function that takes in a particle from mcparticlegraph
/// and returns whether that particle is "primary"
// (if it doesn't have a parent that deposits energy in the detector)
bool ERAnaCosmicValidation::IsMCParticlePrimary(const ertool::Particle & myparticle) {

  // If the particle is MC primary (currently MCParticles are never set as primary):
  if (myparticle.Primary()) return true;

  // If the particle doesn't descend from another, it's primary
  if (!myparticle.Descendant()) return true;

  // If the particle descends from another (IE it has a parent), do some more checks
  // std::cout << "mcparticle is a descendant... " << std::endl;
  /// Grab the mc event data
  auto const& mc_data = MCEventData();
  /// Grab the mc particle graph
  auto const& mc_graph = MCParticleGraph();
  /// Grab the particle's parent
  auto const& parent = mc_graph.GetParticle(myparticle.Parent());
  /// If the parent doesn't deposit energy in the detector (or is invisible), this particle is "primary"
  if (parent.RecoType() == RecoType_t::kTrack) {

    if (mc_data.Track(parent)._energy > 0) {
      // std::cout << "but its parent is a track that deposits energy " << mc_data.Track(parent)._energy
      //           << " so it is NOT primary" << std::endl;
      return false;
    }
    else {
      // std::cout << " But parent doesn't deposit energy so it IS primary." << std::endl;
      return true;
    }
  }
  else if (parent.RecoType() == RecoType_t::kShower) {

    if (mc_data.Shower(parent)._energy > 1) {
      // std::cout << "but its parent is a shower that deposits energy " << mc_data.Shower(parent)._energy
      //           << " so it is NOT primary" << std::endl;
      return false;
    }
    else {
      // std::cout << " But parent doesn't deposit energy so it IS primary." << std::endl;
      return true;
    }
  }
  else if (parent.RecoType() == RecoType_t::kInvisible) {
    // std::cout << "but its parent is invisible, so it IS primary" << std::endl;
    return true;
  }
  else std::cout << " ---- wtf parent isn't track, shower, or invisible... ---- " << std::endl;

  // If we haven't figured out the particle is primary yet, then it isn't.
  // Only caste left is if parent isn't track, shower, or invisible
  return false;

}

/// Simple function that takes in a particle from mcparticlegraph
/// and returns whether that particle is "secondary"
/// (meaning, that particle has a parent in the mcparticlegraph that deposits energy in the detector)
bool ERAnaCosmicValidation::IsMCParticleSecondary(const ertool::Particle & myparticle) {

  // I have a list of total possible secondaries (mcparticlegraph nodes that are children of the
  // mcparticlegraph nodes corresponding to reconstructed primary particles)
  // So to tell if an mcparticle is a secondary, I just ask if it's in the list.
  if ( _potential_secondary_nodeIDs.find(myparticle.ID()) != _potential_secondary_nodeIDs.end())
    return true;
  return false;

  // /// If the mc particle isn't the child of another mc particle, it can't be a secondary
  // if (!myparticle.Descendant()) return false;

  // /// Grab the mc event data
  // auto const& mc_data = MCEventData();
  // /// Grab the mc particle graph
  // auto const& mc_graph = MCParticleGraph();

  // // If the mc particle doesn't have a mc parent that deposits energy in the detector, it isn't a secondary
  // // (at least, not in my definition of secondary)
  // // Two cases: parent is a track, or parent is a shower
  // auto const& parent = mc_graph.GetParticle(myparticle.Parent());
  // // std::cout << "parent node ID is " << myparticle.Parent() << std::endl;
  // // std::cout << "particle reco type is " << myparticle.RecoType() << std::endl;
  // // std::cout << "parent reco type is " << parent.RecoType() << std::endl;

  // // Parent is a track:
  // // If parent track deposits less than 1 MeV in the detector
  // // this particle is not considered a secondary
  // if (parent.RecoType() == RecoType_t::kTrack && mc_data.Track(parent)._energy < 1) {
  //   // std::cout << "parent was a track, with energy " << mc_data.Track(parent)._energy << std::endl;
  //   return false;
  // }

  // // Parent is a shower:
  // // If parent shower deposits less than 1 MeV in the detector
  // // this particle is not considered a secondary
  // else if (parent.RecoType() == RecoType_t::kShower && mc_data.Shower(parent)._energy < 1) {
  //   // std::cout << "parent was a shower, with energy " << mc_data.Shower(parent)._energy << std::endl;
  //   return false;
  // }
  // // Parent has no track or shower associated with it... this particle cannot be a secondary
  // else if (parent.RecoType() == RecoType_t::kInvisible)
  //   return false;

  return true;

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


ertool::Particle ERAnaCosmicValidation::FindMCParticleGraphParticle(const EventData &data, const ertool::Particle &p) {

  /// Grab the mc particle graph
  auto const& mc_graph = MCParticleGraph();
  /// Grab the mc event data
  auto const& mc_data = MCEventData();

  /// Loop through the MCParticleGraph and find the node that corresponds to this particle
  bool found_match = false;

  /// Container to store the particle from mcparticlegraph that matches my reco particle
  ertool::Particle found_mcparticle;
  ertool::Particle candidate_mcparticle;
  // Looping over the mcgraph subset with only this reco type is much faster
  // as most of the mcparticlegraph particles aren't associated with tracks or showers (if they don't enter the detector)
  for ( auto const & mcp : mc_graph.GetParticleNodes(p.RecoType()) ) {

    candidate_mcparticle = mc_graph.GetParticle(mcp);

    //If we're looking for a track
    if ( p.RecoType() == RecoType_t::kTrack ) {
      //Check that the track we're looking for matches exactly the mcparticlegraph's track:
      //Check the start x, y, z coordinates all agree

      if (mc_data.Track(candidate_mcparticle).at(0).at(0) != data.Track(p).at(0).at(0) ||
          mc_data.Track(candidate_mcparticle).at(0).at(1) != data.Track(p).at(0).at(1) ||
          mc_data.Track(candidate_mcparticle).at(0).at(2) != data.Track(p).at(0).at(2)) continue;
      //Check energy agrees (note, mc_data.Track(candidate_mcparticle)._time is unfilled (1e308), but data.Track(p)._time IS filled)
      if (mc_data.Track(candidate_mcparticle)._energy != data.Track(p)._energy) continue;
      found_match = true;
    }
    else if ( p.RecoType() == RecoType_t::kShower ) { //If we're looking for a shower
      //Check that the shower we're looking for matches exactly the mcparticlegraph's shower:
      //Check the start point coordinates all agree
      if (mc_data.Shower(candidate_mcparticle).Start() != data.Shower(p).Start()) continue;
      //Check energy, dedx agree
      //(note, mc_data.Shower(candidate_mcparticle)._time is unfilled (1e308), but data.Shower(p)._time IS filled)
      if (mc_data.Shower(candidate_mcparticle)._energy != data.Shower(p)._energy ||
          mc_data.Shower(candidate_mcparticle)._dedx != data.Shower(p)._dedx ) continue;
      found_match = true;
    }
    else std::cout << " wtf you want to look for an mc particle matching a reco particle that is neither track nor shower?" << std::endl;

    /// Note I have verified that only one match is ever found, so breaking here is fine to save computing time
    if (found_match) {
      found_mcparticle = candidate_mcparticle;
      break;
    }
  } // End loop over mcparticlegraph to find the mcparticlegraph node corresponding to this particle

  if (found_match) return found_mcparticle;
  else {
    throw ertool::ERException(Form("FindMCParticleGraphParticle failed to find a match! Particle info: %s.", p.Print().c_str()));
    return ertool::Particle();
  }

}

/// Function to take in a particle from the mcparticle graph, and find all the mc secondaries associated with
/// that particle, and add them to a std::set keeping track of them all.
void ERAnaCosmicValidation::TrackAllSecondaries(const ertool::Particle & p) {

  //If you want just one layer deep of children, use p.Children
  //If you want all the layers below this particle, you need to use ParticleGraph::GetAllDescendantNodes
  // (meaning you need access to the mcparticlegraph here)
  for (auto const& childnodeID : p.Children())
    _potential_secondary_nodeIDs.insert( childnodeID );

}


}

#endif
