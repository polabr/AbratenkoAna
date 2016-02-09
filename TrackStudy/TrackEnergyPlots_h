/**
 * \file TrackEnergyPlots.h
 *
 * \ingroup TrackStudy
 * 
 * \brief Class def header for a class TrackEnergyPlots
 *
 * @author davidkaleko
 */

/** \addtogroup TrackStudy

    @{*/

#ifndef LARLITE_TRACKENERGYPLOTS_H
#define LARLITE_TRACKENERGYPLOTS_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class TrackEnergyPlots
     User custom analysis class made by davidkaleko
   */
  class TrackEnergyPlots : public ana_base{
  
  public:

    /// Default constructor
    TrackEnergyPlots(){ _name="TrackEnergyPlots"; _fout=0; _ana_tree=0;};

    /// Default destructor
    virtual ~TrackEnergyPlots(){};

    /** IMPLEMENT in TrackEnergyPlots.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in TrackEnergyPlots.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in TrackEnergyPlots.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:

    size_t _event_counter;

  private:
    TTree *_ana_tree;
    double _E_recotrack_max;
    double _E_mctrack_max;
    double _E_recotrack_sum;
    double _E_mctrack_sum;

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
