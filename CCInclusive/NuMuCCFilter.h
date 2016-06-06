/**
 * \file NuMuCCFilter.h
 *
 * \ingroup CCInclusive
 * 
 * \brief Class def header for a class NuMuCCFilter
 *
 * @author davidkaleko
 */

/** \addtogroup CCInclusive

    @{*/

#ifndef LARLITE_NUMUCCFILTER_H
#define LARLITE_NUMUCCFILTER_H

#include "Analysis/ana_base.h"
#include "GeoAlgo/GeoAABox.h"
#include "LArUtil/Geometry.h"

namespace larlite {
  /**
     \class NuMuCCFilter
     User custom analysis class made by SHELL_USER_NAME
   */
  class NuMuCCFilter : public ana_base{
  
  public:

    /// Default constructor
    NuMuCCFilter(){ _name="NuMuCCFilter"; _fout=0;}

    /// Default destructor
    virtual ~NuMuCCFilter(){}

    /** IMPLEMENT in NuMuCCFilter.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in NuMuCCFilter.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in NuMuCCFilter.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:
    
    // Fiducial volume box
    geoalgo::AABox _fidvolBox;
    double fidvol_dist;
    double fidvol_dist_y;
    double fidvol_dist_x;
    double fidvol_dist_z;
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
