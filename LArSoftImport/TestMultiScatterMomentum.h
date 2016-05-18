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

namespace larlite {
  /**
     \class TestMultiScatterMomentum
     User custom analysis class made by SHELL_USER_NAME
   */
  class TestMultiScatterMomentum : public ana_base {

  public:

    /// Default constructor
    TestMultiScatterMomentum() { _name = "TestMultiScatterMomentum"; _fout = 0; _ana_tree = 0;}

    /// Default destructor
    virtual ~TestMultiScatterMomentum() {}

    /** IMPLEMENT in TestMultiScatterMomentum.cc!
        Initialization method to be called before the analysis event loop.
    */
    virtual bool initialize();

    /** IMPLEMENT in TestMultiScatterMomentum.cc!
        Analyze a data event-by-event
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in TestMultiScatterMomentum.cc!
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:

    TrackMomentumCalculator _tmc;

    TTree* _ana_tree;
    double _true_mom;
    double _mcs_reco_mom;
    double _length;
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
