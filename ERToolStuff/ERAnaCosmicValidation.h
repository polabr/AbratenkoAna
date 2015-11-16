/**
 * \file ERAnaCosmicValidation.h
 *
 * \ingroup ERToolStuff
 * 
 * \brief Class def header for a class ERAnaCosmicValidation
 *
 * @author davidkaleko
 */

/** \addtogroup ERToolStuff

    @{*/

#ifndef ERTOOL_ERANACOSMICVALIDATION_H
#define ERTOOL_ERANACOSMICVALIDATION_H

#include "ERTool/Base/AnaBase.h"

namespace ertool {

  /**
     \class ERAnaCosmicValidation
     User custom Analysis class made by kazuhiro
   */
  class ERAnaCosmicValidation : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaCosmicValidation(const std::string& name="ERAnaCosmicValidation");

    /// Default destructor
    virtual ~ERAnaCosmicValidation(){}

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

  };
}
#endif

/** @} */ // end of doxygen group 
