/**
 * \file CosmicAna.h
 *
 * \ingroup scratch_ana
 * 
 * \brief Class def header for a class CosmicAna
 *
 * @author davidkaleko
 */

/** \addtogroup scratch_ana

    @{*/

#ifndef LARLITE_COSMICANA_H
#define LARLITE_COSMICANA_H

#include "Analysis/ana_base.h"
#include "TH1F.h"
namespace larlite {
  /**
     \class CosmicAna
     User custom analysis class made by davidkaleko
   */
  class CosmicAna : public ana_base{
  
  public:

    /// Default constructor
    CosmicAna(){ 
      _name="CosmicAna"; 
      _fout=0;
      _h_MID=0;
    };

    /// Default destructor
    virtual ~CosmicAna(){};

    /** IMPLEMENT in CosmicAna.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in CosmicAna.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in CosmicAna.cc! 
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
