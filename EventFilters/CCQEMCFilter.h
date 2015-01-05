/**
 * \file CCQEMCFilter.h
 *
 * \ingroup EventFilters
 * 
 * \brief Class def header for a class CCQEMCFilter
 *
 * @author davidkaleko
 */

/** \addtogroup EventFilters

    @{*/

#ifndef LARLITE_CCQEMCFILTER_H
#define LARLITE_CCQEMCFILTER_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class CCQEMCFilter
     User custom analysis class made by davidkaleko
   */
  class CCQEMCFilter : public ana_base{
  
  public:

    /// Default constructor
    CCQEMCFilter(){ _name="CCQEMCFilter"; _fout=0;};

    /// Default destructor
    virtual ~CCQEMCFilter(){};

    /** IMPLEMENT in CCQEMCFilter.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in CCQEMCFilter.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in CCQEMCFilter.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    protected:

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
