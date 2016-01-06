/**
 * \file TrigEffStudy.h
 *
 * \ingroup FEMEmulator
 *
 * \brief Ana unit that owns LLInterface. It configures the LLInterface instance.
 *        Each event it asks LLInterface to "Emulate" and it checks whether a software trigger is fired.
 *        It then stores relevant information to a ttree for later efficiency plotting, etc.
 *
 * @author davidkaleko
 */

/** \addtogroup FEMEmulator

    @{*/

#ifndef LARLITE_TRIGEFFSTUDY_H
#define LARLITE_TRIGEFFSTUDY_H

#include "Analysis/ana_base.h"
#include "LArLiteInterface.h"
#include "TTree.h"

namespace larlite {
  /**
     \class TrigEffStudy
     User custom analysis class made by kaleko
   */
  class TrigEffStudy : public ana_base {

  public:

    /// Default constructor
    TrigEffStudy() {
      _name = "TrigEffStudy";
      _fout = 0;
      _ana_tree = 0;
      _window_max_us = 0.1;
      _window_min_us = -0.01;
    }

    /// Default destructor
    virtual ~TrigEffStudy() {}

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

    // Setter for config object of LL interface
    void setConfig(fememu::FEMBeamTriggerConfig* dummy) { _myconfig = dummy; }

    // Setter for how wide of a time-window one wants to integrate PEs over PMTs
    // Time window starts _window_min_us before trigger, extends _window_max_us past trigger
    void setWindowMin(float dummy) { _window_min_us = dummy; }
    void setWindowMax(float dummy) { _window_max_us = dummy; }

  protected:

    // Bool to toggle whether to use simphotons (if true), or ophits (if false)
    bool _use_mc;

    fememu::LLInterface _my_LLint;
    fememu::FEMBeamTriggerConfig *_myconfig = 0;
    fememu::FEMBeamTriggerOutput _myoutput;

    // Construct the LL interface. Throws exception if config not set
    void constructLLInterface();

    // Time window to integrate PE over PMT channels 0-31
    // This much time is searched after trigger time
    float _window_max_us;
    // This much time is searched before trigger time
    float _window_min_us;

    // Analysis ttree for efficiency plotting later
    TTree *_ana_tree;
    int n_trigs;
    int n_reco_PE; //or, number of simphotons if _use_mc is true
    float x_pos;
    int pdg; //PDG of particle whose x_pos and energy are saved
    float energy; //Truth energy of particle whose x_pos is saved

    // TTree functionality
    void resetTTreeVars();
    void initializeTTree();

  };
}
#endif

//**************************************************************************
//
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group
