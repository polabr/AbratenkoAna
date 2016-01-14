#ifndef ERTOOL_ERANAPI0ALGVALIDATION_CXX
#define ERTOOL_ERANAPI0ALGVALIDATION_CXX

#include "ERAnaPi0AlgValidation.h"

namespace ertool {

  ERAnaPi0AlgValidation::ERAnaPi0AlgValidation(const std::string& name)
    : AnaBase(name)
    , _result_tree(nullptr)
  {

    PrepareTreeVariables();

    // set default energy cut (for counting) to 0
    _eCut = 0;

  }


  bool ERAnaPi0AlgValidation::Analyze(const EventData &data, const ParticleGraph &ps)
  {
    ResetTreeVariables();

    /// Grab the mc particle graph
    auto const& mc_graph = MCParticleGraph();

    if (!mc_graph.GetParticleArray().size()) {
      std::cout << "WARNING: Size of mc particle graph is zero! Perhaps you forgot to include mctruth/mctrack/mcshower?" << std::endl;
      return false;
    }

    // Find number of true pi0s in the event
    for (auto const& p : mc_graph.GetParticleArray()) {
      if (p.PdgCode() == 111) {
        n_true_pi0++;
        true_pi0_momentum = p.Momentum().Length();
      }
    }

    // Find number of reco pi0s in the event
    for (auto const& p : ps.GetParticleArray()) {
      if (p.PdgCode() == 111) {
        n_reco_pi0++;
        reco_pi0_momentum = p.Momentum().Length();
      }
    }

    // Fill ttree
    _result_tree->Fill();

    return true;

  } // end analyze function


  void ERAnaPi0AlgValidation::ProcessEnd(TFile * fout)
  {

    if (fout) {
      fout->cd();
      _result_tree->Write();
    }
    return;
  }

  void ERAnaPi0AlgValidation::PrepareTreeVariables() {

    if (_result_tree) { delete _result_tree; }

    _result_tree = new TTree("anatree", "anatree");
    _result_tree->Branch("n_true_pi0", &n_true_pi0, "n_true_pi0/I");
    _result_tree->Branch("n_reco_pi0", &n_reco_pi0, "n_reco_pi0/I");
    _result_tree->Branch("true_pi0_momentum", &true_pi0_momentum, "true_pi0_momentum/F");
    _result_tree->Branch("reco_pi0_momentum", &reco_pi0_momentum, "reco_pi0_momentum/F");

    return;
  }

  void ERAnaPi0AlgValidation::ResetTreeVariables() {

    n_true_pi0 = 0;
    n_reco_pi0 = 0;
    true_pi0_momentum = -999.;
    reco_pi0_momentum = -999.;

    return;
  }
}

#endif
