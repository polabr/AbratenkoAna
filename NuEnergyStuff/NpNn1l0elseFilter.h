/**
 * \file NpNn1l0elseFilter.h
 *
 * \ingroup scratch_ana
 * 
 * \brief Class def header for a class NpNn1l0elseFilter
 *
 * @author davidkaleko
 */

/** \addtogroup scratch_ana

    @{*/

#ifndef LARLITE_NPNN1L0ELSEFILTER_H
#define LARLITE_NPNN1L0ELSEFILTER_H

#include "Analysis/ana_base.h"
#include "DataFormat/mcpart.h"
#include "DataFormat/mctruth.h"

namespace larlite {
  /**
     \class NpNn1l0elseFilter
     User custom analysis class made by SHELL_USER_NAME
   */
  class NpNn1l0elseFilter : public ana_base{
  
  public:

    /// Default constructor
    NpNn1l0elseFilter(){ _name="NpNn1l0elseFilter"; _fout=0;}

    /// Default destructor
    virtual ~NpNn1l0elseFilter(){}

    /** IMPLEMENT in NpNn1l0elseFilter.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in NpNn1l0elseFilter.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in NpNn1l0elseFilter.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:

    size_t evt_counter;
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
