/**
 * \file OnlyOneHighEMCTrackFilter.h
 *
 * \ingroup TrackStudy
 * 
 * \brief Class def header for a class OnlyOneHighEMCTrackFilter
 *
 * @author davidkaleko
 */

/** \addtogroup TrackStudy

    @{*/

#ifndef LARLITE_ONLYONEHIGHEMCTRACKFILTER_H
#define LARLITE_ONLYONEHIGHEMCTRACKFILTER_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class OnlyOneHighEMCTrackFilter
     User custom analysis class made by davidkaleko
   */
  class OnlyOneHighEMCTrackFilter : public ana_base{
  
  public:

    /// Default constructor
    OnlyOneHighEMCTrackFilter(){ 
      _name="OnlyOneHighEMCTrackFilter";
      _fout=0;
      _track_cutoff_energy_MEV = -1.;
    };

    /// Default destructor
    virtual ~OnlyOneHighEMCTrackFilter(){};

    /** IMPLEMENT in OnlyOneHighEMCTrackFilter.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in OnlyOneHighEMCTrackFilter.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in OnlyOneHighEMCTrackFilter.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetTrackCutoffEnergyMEV(double david) { _track_cutoff_energy_MEV = david; };

  protected:

  private:

    double _track_cutoff_energy_MEV;

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
