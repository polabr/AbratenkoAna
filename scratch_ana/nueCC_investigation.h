/**
 * \file nueCC_investigation.h
 *
 * \ingroup scratch_ana
 * 
 * \brief Class def header for a class nueCC_investigation
 *
 * @author davidkaleko
 */

/** \addtogroup scratch_ana

    @{*/

#ifndef LARLITE_NUECC_INVESTIGATION_H
#define LARLITE_NUECC_INVESTIGATION_H

#include "Analysis/ana_base.h"


/*
the purpose of this code is to investigate variables that might be used
to help pick out nueCC events from a big BNB sample (there are like 7
in 1000 BNB events). this code will loop over BNB events, save to a tree
whether each event was nueCC, and save lots of variables to Draw() and see
*/

namespace larlite {
  /**
     \class nueCC_investigation
     User custom analysis class made by davidkaleko
   */
  class nueCC_investigation : public ana_base{
  
  public:

    /// Default constructor
    nueCC_investigation(){ 
      _ana_tree=0;
      _name="nueCC_investigation"; 
      _fout=0;
    };

    /// Default destructor
    virtual ~nueCC_investigation(){};

    /** IMPLEMENT in nueCC_investigation.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in nueCC_investigation.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in nueCC_investigation.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:


    //analysis TTree
    TTree *_ana_tree;
    bool _is_nueCC;
    int _n_mcshowers_over_50MEV;
    //is the highest energy MCShower from an electron?
    //this means assuming we can distinguish e/g 100% perfectly
    bool _is_electron_shower;
    double _max_mcs_energy;

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
