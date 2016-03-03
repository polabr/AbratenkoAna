/**
 * \file CleanPi0Filter.h
 *
 * \ingroup EventFilters
 * 
 * \brief Class def header for a class CleanPi0Filter
 *
 * @author davidkaleko
 */

/** \addtogroup EventFilters

    @{*/

#ifndef LARLITE_CLEANPI0FILTER_H
#define LARLITE_CLEANPI0FILTER_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class CleanPi0Filter
     User custom analysis class made by SHELL_USER_NAME
   */
  class CleanPi0Filter : public ana_base{
  
  public:

    /// Default constructor
    CleanPi0Filter(){ _name="CleanPi0Filter"; _fout=0;}

    /// Default destructor
    virtual ~CleanPi0Filter(){}

    /** IMPLEMENT in CleanPi0Filter.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in CleanPi0Filter.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in CleanPi0Filter.cc! 
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
