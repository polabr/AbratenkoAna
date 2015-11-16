/**
 * \file MCShowerStudy.h
 *
 * \ingroup scratch_ana
 * 
 * \brief Class def header for a class MCShowerStudy
 *
 * @author davidkaleko
 */

/** \addtogroup scratch_ana

    @{*/

#ifndef LARLITE_MCSHOWERSTUDY_H
#define LARLITE_MCSHOWERSTUDY_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class MCShowerStudy
     User custom analysis class made by SHELL_USER_NAME
   */
  class MCShowerStudy : public ana_base{
  
  public:

    /// Default constructor WITHOUT _ana_tree=0 IT RANDOMLY SEGFAULTS HALF THE GODDAMN TIME ON MY LAPTOP
    MCShowerStudy(){ _name="MCShowerStudy"; _fout=0; _ana_tree = 0;}

    /// Default destructor
    virtual ~MCShowerStudy(){}

    /** IMPLEMENT in MCShowerStudy.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MCShowerStudy.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in MCShowerStudy.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:
    

    TTree* _ana_tree;
    double _start_x_vtx;
    double _detprof_x_vtx;
    double _detprof_E;

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
