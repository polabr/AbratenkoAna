/**
 * \file MCTrackStudy.h
 *
 * \ingroup scratch_ana
 * 
 * \brief Class def header for a class MCTrackStudy
 *
 * @author davidkaleko
 */

/** \addtogroup scratch_ana

    @{*/

#ifndef LARLITE_MCTRACKSTUDY_H
#define LARLITE_MCTRACKSTUDY_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class MCTrackStudy
     User custom analysis class made by SHELL_USER_NAME
   */
  class MCTrackStudy : public ana_base{
  
  public:

    /// Default constructor
    MCTrackStudy(){ _name="MCTrackStudy"; _fout=0; _ana_tree=0; _producername = "mcreco";}

    /// Default destructor
    virtual ~MCTrackStudy(){}

    /** IMPLEMENT in MCTrackStudy.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MCTrackStudy.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in MCTrackStudy.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void setMCShowerProducer(std::string producername) { _producername = producername; }

    
  protected:
    std::string _producername;

    TTree* _ana_tree;
    double _start_x_vtx;
    double _start_t;

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
