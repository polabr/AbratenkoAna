/**
 * \file TestCCQECalc.h
 *
 * \ingroup CCInclusive
 *
 * \brief Class def header for a class TestCCQECalc
 *
 * @author davidkaleko
 */

/** \addtogroup CCInclusive

    @{*/

#ifndef LARLITE_TESTCCQECALC_H
#define LARLITE_TESTCCQECALC_H

#include "Analysis/ana_base.h"
#include "TH2.h"
#include "CCQECalc.h"
#include "GeoAlgo/GeoAABox.h"
#include "GeoAlgo/GeoVector.h"
#include "LArUtil/Geometry.h"

namespace larlite {
  /**
     \class TestCCQECalc
     User custom analysis class made by SHELL_USER_NAME
   */
  class TestCCQECalc : public ana_base {

  public:

    /// Default constructor
    TestCCQECalc() { _name = "TestCCQECalc"; _fout = 0; _h = 0;}

    /// Default destructor
    virtual ~TestCCQECalc() {}

    /** IMPLEMENT in TestCCQECalc.cc!
        Initialization method to be called before the analysis event loop.
    */
    virtual bool initialize();

    /** IMPLEMENT in TestCCQECalc.cc!
        Analyze a data event-by-event
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in TestCCQECalc.cc!
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:

    TH2D *_h;

    CCQECalc *mycalc;
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
