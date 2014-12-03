/**
 * \file CosmicMIDEstimator.h
 *
 * \ingroup scratch_ana
 * 
 * \brief Class def header for a class CosmicMIDEstimator
 *
 * @author davidkaleko
 */

/** \addtogroup scratch_ana

    @{*/

#ifndef LARLITE_COSMICMIDESTIMATOR_H
#define LARLITE_COSMICMIDESTIMATOR_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class CosmicMIDEstimator
     User custom analysis class made by davidkaleko
   */
  class CosmicMIDEstimator : public ana_base{
  
  public:

    /// Default constructor
    CosmicMIDEstimator(){ 
      _name="CosmicMIDEstimator"; 
      _fout=0;
      _h_MID=0;
    };

    /// Default destructor
    virtual ~CosmicMIDEstimator(){};

    /** IMPLEMENT in CosmicMIDEstimator.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in CosmicMIDEstimator.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in CosmicMIDEstimator.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:

    TH1F* _h_MID;

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
