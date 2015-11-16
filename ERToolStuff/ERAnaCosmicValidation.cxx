#ifndef ERTOOL_ERANACOSMICVALIDATION_CXX
#define ERTOOL_ERANACOSMICVALIDATION_CXX

#include "ERAnaCosmicValidation.h"

namespace ertool {

  ERAnaCosmicValidation::ERAnaCosmicValidation(const std::string& name) : AnaBase(name)
  {}

  void ERAnaCosmicValidation::Reset()
  {}

  void ERAnaCosmicValidation::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAnaCosmicValidation::ProcessBegin()
  {}

  bool ERAnaCosmicValidation::Analyze(const EventData &data, const ParticleGraph &ps)
  { return true; }

  void ERAnaCosmicValidation::ProcessEnd(TFile* fout)
  {}

}

#endif
