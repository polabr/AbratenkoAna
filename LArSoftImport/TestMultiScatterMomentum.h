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
#include "LArUtil/Geometry.h"
#include "GeoAlgo/GeoAABox.h"

// #include "../UserDev/BasicTool/GeoAlgo/GeoAABox.h"

namespace larlite {
  /**
     \class TestMultiScatterMomentum
     User custom analysis class made by SHELL_USER_NAME
  */
  class TestMultiScatterMomentum : public ana_base {
    
  public:
    
    /// Default constructor
    TestMultiScatterMomentum() { _name = "TestMultiScatterMomentum"; _fout = 0; _ana_tree = 0; _trackmatch_tree = 0; _using_mctracks = true;}
    
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
    
    const larlite::mctrack getChosenMCTrack(storage_manager* storage, const TVector3 nu_vtx);
    
    const larlite::track getMatchedTrack(storage_manager* storage, const larlite::mctrack chosen_mctrack);
    
  protected:
    
    bool _using_mctracks;
    
    TrackMomentumCalculator _tmc;
    
    // This ttree is filled ONCE PER EVENT (or not at all for an event if something fails)
    TTree* _ana_tree;
    double _true_mom;
    double _mcs_reco_mom;
    double _true_length;
    double _reco_length;
    bool _mu_contained;
    double _distances;
    double _angles;
    TVector3 _startPoints;
    TVector3 _endPoints;
    
    // This ttree is filled once per RECO TRACK inside of the getMatchedTrack function
    // this ttree is mostly for debugging to determine where your track-match cut values
    // should be placed
    TTree* _trackmatch_tree;
    double _match_dist;
    double _match_dotprod;
    
    geoalgo::AABox _fidvolBox;
    
    // This is the value the dot product will be compared to
    double unitConstant;
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
