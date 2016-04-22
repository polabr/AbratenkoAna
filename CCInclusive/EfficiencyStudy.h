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
    EfficiencyStudy() { _name = "EfficiencyStudy"; _fout = 0; _tree = 0; }

    /// Default destructor
    virtual ~EfficiencyStudy() {}

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

  protected:

    TTree *_tree;
    double _dist_vtx_truth;
    bool _is_truth_fiducial;

    //Geometry algo instance
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
