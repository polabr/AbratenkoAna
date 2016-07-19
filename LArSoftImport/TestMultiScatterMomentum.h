/**
 * \file TestMultiScatterMomentum.h
 *
 * \ingroup LArSoftImport
 *
 * \brief Class def header for a class TestMultiScatterMomentum
 *
 * @author davidkaleko
 */

/** \addtogroup LArSoftImport
    
    @{*/

#ifndef LARLITE_TESTMULTISCATTERMOMENTUM_H
#define LARLITE_TESTMULTISCATTERMOMENTUM_H

#include "Analysis/ana_base.h"
#include "TrackMomentumCalculator.h"
#include "TrackMomentumSplines.h"
#include "LArUtil/Geometry.h"
#include "GeoAlgo/GeoAABox.h"
#include "TH1D.h"
#include "FidVolBox.h"

// This is the code that selects events on actual data
#include "XiaoNuFinder.h"

// #include "../UserDev/BasicTool/GeoAlgo/GeoAABox.h"

namespace larlite {
  /**
     \class TestMultiScatterMomentum
     User custom analysis class made by SHELL_USER_NAME
  */
  class TestMultiScatterMomentum : public ana_base {
    
  public:
    
    /// Default constructor
    TestMultiScatterMomentum() { 
      _name = "TestMultiScatterMomentum"; 
      _fout = 0; 
      _ana_tree = 0; 
      _trackmatch_tree = 0; 
      _using_mctracks = true;
      _running_on_data = false;
    }
    
    /// Default destructor
    virtual ~TestMultiScatterMomentum() {}
    
    virtual bool initialize();
    
    virtual bool analyze(storage_manager* storage);
    
    virtual bool finalize();
    
    void SetUseMCTracks(bool flag) { _using_mctracks = flag; }
    
    /* general code structure:
       initialize function defines ttree, etc
       analyze function once per event
       1) read in data products (tracks, mctracks)
       2) function called getChosenMCTrack() that returns an actual mctrack
       -- if it cannot find a good mctrack, throw exception
       2a) fill ttree variables that only have to do w/ the mctrack (true momentum, etc)
       3) function called getMatchedTrack() that returns an actual track
       -- if it cannot find a good track, throw exception
       3a) fill ttree variables that only have to do w/ the track (length, etc)
       4) fill ttree variables that have to do with comparisons b/t track and mctrack
       finalize function saves stuff
    */
    
    // This function is used on MC simulation to find the mctrack of interest
    const larlite::mctrack getChosenMCTrack(storage_manager* storage, const TVector3 nu_vtx);
    
    // This function is used on MC simulation to get the reco track closely matching the given mctrack
    const larlite::track getMatchedTrack(storage_manager* storage, const larlite::mctrack chosen_mctrack);
    
    // This function is used when running on actual data to find the reco track of interest
    const larlite::track getMuonTrackData(storage_manager* storage);

    // Set this to true in your python run script if you are running on real data
    // instead of simulated cosmics. The default is FALSE so if you don't set it,
    // it will assuming you are running on simulated data.
    void SetRunningOnData(bool flag) { _running_on_data = flag; }

  protected:

    TH1D* th;
    
    bool _using_mctracks;
    
    // Instance of TrackMomentumCalculator (MCS based energy)
    TrackMomentumCalculator _tmc;
    
    // This ttree is filled ONCE PER EVENT (or not at all for an event if something fails)
    TTree* _ana_tree;
    double _true_mom;
    double _mcs_reco_mom;
    double _true_length;
    double _reco_length;
    bool _mu_contained;
    bool _mu_contained_reco;
    double _distances;
    double _angles;
    TVector3 _startPoints;
    TVector3 _endPoints;
    double _range_recotrack_mom;
    double _range_MCTrack_mom;
    
    // This ttree is filled once per RECO TRACK inside of the getMatchedTrack function
    // this ttree is mostly for debugging to determine where your track-match cut values
    // should be placed
    TTree* _trackmatch_tree;
    double _match_dist;
    double _match_dotprod;
    
    geoalgo::AABox _fidvolBox;
    
    // This is the value the dot product will be compared to
    double unitConstant;

    // Instance of TrackMomentumSplines (range based energy)
    TrackMomentumSplines _range_calc;

    // Instance of XiaoNuFinder (for finding neutrino events in actual data)
    XiaoNuFinder _nu_finder;

    // Whether you are running on actual data, or simulated BNB+Cosmics
    bool _running_on_data;
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
