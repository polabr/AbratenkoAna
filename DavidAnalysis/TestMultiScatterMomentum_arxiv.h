/**
 * \file TestMultiScatterMomentum_arxiv.h
 *
 * \ingroup LArSoftImport
 *
 * \brief Class def header for a class TestMultiScatterMomentum_arxiv
 *
 * @author davidkaleko
 */

/** \addtogroup LArSoftImport

    @{*/

#ifndef LARLITE_TESTMULTISCATTERMOMENTUM_ARXIV_H
#define LARLITE_TESTMULTISCATTERMOMENTUM_ARXIV_H

#include "Analysis/ana_base.h"
#include "TrackMomentumCalculator_arxiv.h"
#include "LArUtil/Geometry.h"
#include "GeoAlgo/GeoAABox.h"

namespace larlite {
  /**
     \class TestMultiScatterMomentum_arxiv
     User custom analysis class made by SHELL_USER_NAME
   */
  class TestMultiScatterMomentum_arxiv : public ana_base {

  public:

    /// Default constructor
    TestMultiScatterMomentum_arxiv() { _name = "TestMultiScatterMomentum_arxiv"; _fout = 0; _ana_tree = 0; _using_mctracks = true;}

    /// Default destructor
    virtual ~TestMultiScatterMomentum_arxiv() {}

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

    void SetUseMCTracks(bool flag) { _using_mctracks = flag; }

  protected:

    bool _using_mctracks;

    TrackMomentumCalculator_arxiv _tmc;

    TTree* _ana_tree;
    double _true_mom;
    double _mcs_reco_mom;
    double _true_length;
    double _reco_length;
    bool _mu_contained;

    geoalgo::AABox _fidvolBox;

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
