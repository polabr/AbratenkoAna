/**
 * \file ShowerProfileValidation.h
 *
 * \ingroup scratch_ana
 * 
 * \brief Class def header for a class ShowerProfileValidation
 *
 * @author davidkaleko
 */

/** \addtogroup scratch_ana

    @{*/

#ifndef LARLITE_SHOWERPROFILEVALIDATION_H
#define LARLITE_SHOWERPROFILEVALIDATION_H

#include "Analysis/ana_base.h"
 #include "TH2.h"
#include "EMShowerTools/EMShowerProfile.h"
 #include "GeoAlgo/GeoAlgo.h"
#include "GeoAlgo/GeoAABox.h"

namespace larlite {
  /**
     \class ShowerProfileValidation
     User custom analysis class made by SHELL_USER_NAME
   */
  class ShowerProfileValidation : public ana_base{
  
  public:

    /// Default constructor
    ShowerProfileValidation();

    /// Default destructor
    virtual ~ShowerProfileValidation(){}

    /** IMPLEMENT in ShowerProfileValidation.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ShowerProfileValidation.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ShowerProfileValidation.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:

     /// GeoAlgo instance for geometric computations
    geoalgo::GeoAlgo _geoAlgo;

    /// EMShowerProfiler instance
    ::EMShowerProfile _emsProf;

    /// TPC AABox
    ::geoalgo::AABox _fTPC;

    TTree* ana_tree;
    double E_contained;
    double prof_E_contained;

    TH2F* out_hist;
    
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
