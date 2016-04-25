/**
 * \file EfficiencyStudy.h
 *
 * \ingroup CCInclusive
 *
 * \brief Class def header for a class EfficiencyStudy
 *
 * @author davidkaleko
 */

/** \addtogroup CCInclusive

    @{*/

#ifndef LARLITE_EFFICIENCYSTUDY_H
#define LARLITE_EFFICIENCYSTUDY_H

#include "Analysis/ana_base.h"
#include "TTree.h"
#include "GeoAlgo/GeoAlgo.h"
#include "GeoAlgo/GeoVector.h"
#include "GeoAlgo/GeoAABox.h"
#include "LArUtil/Geometry.h"

namespace larlite {
  /**
     \class EfficiencyStudy
     User custom analysis class made by SHELL_USER_NAME
   */
  class EfficiencyStudy : public ana_base {

  public:

    /// Default constructor
    EfficiencyStudy() { 
      _name = "EfficiencyStudy"; 
      _fout = 0; 
      _vtx_tree = 0; 
      _evt_tree = 0;
      _flash_tree = 0;
    }

    /// Default destructor
    virtual ~EfficiencyStudy() {}

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

  protected:

    // This tree is filled once per reconstructed neutrino vertex
    TTree *_vtx_tree;
    double _dist_vtx_truth;

    // This tree is filled once per art event
    TTree *_evt_tree;
    bool _is_truth_fiducial;
    bool _is_numuCC;
    bool _flash_in_bgw;
    bool _is_areco_vtx_in_fidvol;
    bool _is_atrack_fromvtx;
    bool _longest_track_nearflash_z;
    bool _longest_trk_contained;
    bool _longest_trk_range_longenough;


    // This tree is to debug flash timing and is filled once per flash
    TTree *_flash_tree;
    double _flash_time; //in us

    //Geometry algo instances
    geoalgo::GeoAlgo _geoAlgo;
    // Fiducial volume box
    geoalgo::AABox _myGeoAABox;
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
