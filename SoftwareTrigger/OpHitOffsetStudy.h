/**
 * \file OpHitOffsetStudy.h
 *
 * \ingroup SoftwareTrigger
 * 
 * \brief Class def header for a class OpHitOffsetStudy
 *
 * @author davidkaleko
 */

/** \addtogroup SoftwareTrigger

    @{*/

#ifndef LARLITE_OPHITOFFSETSTUDY_H
#define LARLITE_OPHITOFFSETSTUDY_H

#include "Analysis/ana_base.h"
#include "TH1.h"

namespace larlite {
  /**
     \class OpHitOffsetStudy
     User custom analysis class made by SHELL_USER_NAME
   */
  class OpHitOffsetStudy : public ana_base{
  
  public:

    /// Default constructor
    OpHitOffsetStudy(){ _name="OpHitOffsetStudy"; _fout=0; hist = 0;}

    /// Default destructor
    virtual ~OpHitOffsetStudy(){}

    /** IMPLEMENT in OpHitOffsetStudy.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in OpHitOffsetStudy.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in OpHitOffsetStudy.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:
    
    TH1F *hist;
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
