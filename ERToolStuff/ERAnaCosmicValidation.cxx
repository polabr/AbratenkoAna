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
{ return true; }

void ERAnaCosmicValidation::ProcessEnd(TFile* fout)
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
  _result_tree->Branch("_is_track", &_is_track,"_is_track/O");
  _result_tree->Branch("_true_primary", &_true_primary,"_true_primary/O");
  _result_tree->Branch("_true_secondary", &_true_secondary,"_true_secondary/O");
  _result_tree->Branch("_reco_primary", &_reco_primary,"_reco_primary/O");
  _result_tree->Branch("_reco_secondary", &_reco_secondary,"_reco_secondary/O");

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
