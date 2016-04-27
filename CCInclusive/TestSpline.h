/**
 * \file TestSpline.h
 *
 * \ingroup CCInclusive
 *
 * \brief Class def header for a class TestSpline
 *
 * @author davidkaleko
 */

/** \addtogroup CCInclusive

    @{*/

#ifndef LARLITE_TESTSPLINE_H
#define LARLITE_TESTSPLINE_H

#include "Analysis/ana_base.h"
#include "TH2.h"
#include "MuTrackMomentumSpline.h"
#include "GeoAlgo/GeoAABox.h"
#include "GeoAlgo/GeoVector.h"
#include "LArUtil/Geometry.h"

namespace larlite {
  /**
     \class TestSpline
     User custom analysis class made by SHELL_USER_NAME
   */
  class TestSpline : public ana_base {

  public:

    /// Default constructor
    TestSpline() { _name = "TestSpline"; _fout = 0; _h=0; myspline=0;}

    /// Default destructor
    virtual ~TestSpline() {}


    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

  protected:
    TH2D *_h;

    MuTrackMomentumSpline *myspline;
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
