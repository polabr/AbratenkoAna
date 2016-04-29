/**
 * \file MCShowersContainedFilter.h
 *
 * \ingroup EventFilters
 * 
 * \brief Class def header for a class MCShowersContainedFilter
 *
 * @author davidkaleko
 */

/** \addtogroup EventFilters

    @{*/

#ifndef LARLITE_MCSHOWERSCONTAINEDFILTER_H
#define LARLITE_MCSHOWERSCONTAINEDFILTER_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class MCShowersContainedFilter
     User custom analysis class made by SHELL_USER_NAME
   */
  class MCShowersContainedFilter : public ana_base{
  
  public:

    /// Default constructor
    MCShowersContainedFilter(){ _name="MCShowersContainedFilter"; _fout=0;}

    /// Default destructor
    virtual ~MCShowersContainedFilter(){}

    /** IMPLEMENT in MCShowersContainedFilter.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MCShowersContainedFilter.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in MCShowersContainedFilter.cc! 
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
