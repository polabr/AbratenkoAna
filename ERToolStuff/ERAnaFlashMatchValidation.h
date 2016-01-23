/**
 * \file ERAnaFlashMatchValidation.h
 *
 * \ingroup ERToolStuff
 * 
 * \brief This code is designed for mctracks input on single muons. It assumes you have run ERAlgoFlashMatch
 *        before this. It computes figures of merit intended to exactly copy the couts at the end of
 *        UBT0Finder.
 *
 * @author davidkaleko
 */

/** \addtogroup ERToolStuff

    @{*/

#ifndef ERTOOL_ERANAFLASHMATCHVALIDATION_H
#define ERTOOL_ERANAFLASHMATCHVALIDATION_H

#include "ERTool/Base/AnaBase.h"
#include "TH1.h"

namespace ertool {

  /**
     \class ERAnaFlashMatchValidation
     User custom Analysis class made by kaleko
   */
  class ERAnaFlashMatchValidation : public AnaBase {
  
  public:

    /// What sample you are validating flashmatch algo performance on
    enum RunMode_t {
      kSingleMuons,
      kSingleElectrons,
      kCosmics,
      kRUN_MODE_MAX
    };

    /// Default constructor
    ERAnaFlashMatchValidation(const std::string& name="ERAnaFlashMatchValidation");

    /// Default destructor
    virtual ~ERAnaFlashMatchValidation(){}

    /// Reset function
    virtual void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Analyze(const EventData &data, const ParticleGraph &ps);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);

    /// Set whether you are running on single mu, single e, or cosmics
    void SetRunMode(RunMode_t runmode) { _run_mode = runmode; }

  private:

    size_t n_matched_flashes;
    size_t n_qclusters;

    RunMode_t _run_mode;

    TH1D * _time_diff ;


  };
}
#endif

/** @} */ // end of doxygen group 
