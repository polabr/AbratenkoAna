/**
 * \file JoesMCShowerAna.h
 *
 * \ingroup SampleJosephMCShowerAna
 * 
 * \brief Class def header for a class JoesMCShowerAna
 *
 * @author davidkaleko
 */

/** \addtogroup SampleJosephMCShowerAna

    @{*/

#ifndef LARLITE_JOESMCSHOWERANA_H
#define LARLITE_JOESMCSHOWERANA_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class JoesMCShowerAna
     User custom analysis class made by davidkaleko
   */
  class JoesMCShowerAna : public ana_base{
  
  public:

    /// Default constructor
    JoesMCShowerAna(){ _name="JoesMCShowerAna"; _fout=0;};

    /// Default destructor
    virtual ~JoesMCShowerAna(){};

    /** IMPLEMENT in JoesMCShowerAna.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in JoesMCShowerAna.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in JoesMCShowerAna.cc! 
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
