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
#include "TH1.h"

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
      _window_us_after_truth_part_time = 0.1;
      _window_us_before_truth_part_time = 0.01;
      _use_mc = true;
      _debughist = 0;
      ophit_tree = 0;
    }

    /// Default destructor
    virtual ~TrigEffStudy() {}

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

    // Setter for config object of LL interface
    void setConfig(fememu::FEMBeamTriggerConfig* dummy) { _myconfig = dummy; }

    // Setter for how wide of a time-window one wants to integrate PEs over PMTs
    // Time window starts _window_us_before_truth_part_time before trigger, extends _window_us_after_truth_part_time past trigger
    void setWindowPreTruthPartTime(float dummy) { _window_us_before_truth_part_time = dummy; }
    void setWindowPostTruthPartTime(float dummy) { _window_us_after_truth_part_time = dummy; }
    void setUseMC(bool dummy) { _use_mc = dummy; }

  protected:

    // Bool to toggle whether to use simphotons (if true), or ophits (if false)
    bool _use_mc;

    fememu::LLInterface _my_LLint;
    fememu::FEMBeamTriggerConfig *_myconfig = 0;
    fememu::FEMBeamTriggerOutput _myoutput;

    // Construct the LL interface. Throws exception if config not set
    void constructLLInterface();

    // Time window to integrate PE over PMT channels 0-31
    // This much time is searched after true mcparticle arrival time
    float _window_us_after_truth_part_time;
    // This much time is searched before true mcparticle arrival time time
    float _window_us_before_truth_part_time;

    // Analysis ttree for efficiency plotting later
    TTree *_ana_tree;
    int n_trigs;
    int emulated_trigger_tick;
    float n_reco_PE; //or, number of simphotons if _use_mc is true
    float x_pos;
    int pdg; //PDG of particle whose x_pos and energy are saved
    float energy; //Truth energy of particle whose x_pos is saved
    float em_trig_minus_truth_particle_time_ns; //Time difference between emulated trigger and actual particle arrival time
    float nophit_over_nsimph;
    float energy_deposited;

    //debugging ophit tree
    TTree *ophit_tree;
    float ophit_peaktime;
    float ftruth_part_arrival_time;
    float ftrigger_time_us;
    float femtrigger_time_us;

    // TTree functionality
    void resetTTreeVars();
    void initializeTTree();

    TH1F *_debughist;

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
