/**
 * \file PerfectReco_1eNp0else_Filter.h
 *
 * \ingroup EventFilters
 * 
 * \brief Class def header for a class PerfectReco_1eNp0else_Filter
 *
 * @author davidkaleko
 */

/** \addtogroup EventFilters

    @{*/

#ifndef LARLITE_PERFECTRECO_1ENP0ELSE_FILTER_H
#define LARLITE_PERFECTRECO_1ENP0ELSE_FILTER_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class PerfectReco_1eNp0else_Filter
     User custom analysis class made by SHELL_USER_NAME
   */
  class PerfectReco_1eNp0else_Filter : public ana_base{
  
  public:

    /// Default constructor
    PerfectReco_1eNp0else_Filter(){ _name="PerfectReco_1eNp0else_Filter"; _fout=0;}

    /// Default destructor
    virtual ~PerfectReco_1eNp0else_Filter(){}

    /** IMPLEMENT in PerfectReco_1eNp0else_Filter.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in PerfectReco_1eNp0else_Filter.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in PerfectReco_1eNp0else_Filter.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:
    
    size_t total_events;
    size_t kept_events;

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
