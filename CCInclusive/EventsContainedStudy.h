/**
 * \file EventsContainedStudy.h
 *
 * \ingroup CCInclusive
 *
 * \brief Class def header for a class EventsContainedStudy
 *
 * @author davidkaleko
 */

/** \addtogroup CCInclusive

    @{*/

#ifndef LARLITE_EVENTSCONTAINEDSTUDY_H
#define LARLITE_EVENTSCONTAINEDSTUDY_H

#include "Analysis/ana_base.h"
#include "LArUtil/Geometry.h"
#include "TH1.h"
#include "GeoAlgo/GeoAABox.h"
 #include "GeoAlgo/GeoSphere.h"

namespace larlite {
  /**
     \class EventsContainedStudy
     User custom analysis class made by SHELL_USER_NAME
   */
  class EventsContainedStudy : public ana_base {

  public:

    /// Default constructor
    EventsContainedStudy() {
      _name = "EventsContainedStudy";
      _fout = 0;
      _tree = 0;
      _h_mu_E_contained = 0;
      _h_mu_E_all = 0;
      _h_mu_E_contained = 0;
      _h_mu_E_all = 0;
    }

    /// Default destructor
    virtual ~EventsContainedStudy() {}

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

  protected:

        // Sphere around neutrino vertex
    geoalgo::Sphere _nu_sphere;

    TTree *_tree;
    bool _mu_contained;
    bool _p_contained;
    size_t _n_reco_tracks_fromvtx;
    size_t _n_mc_tracks_fromvtx;

    geoalgo::AABox _fidvolBox;

    size_t _evts_pass_filter;
    size_t _evts_fully_contained;



    TH1F *_h_mu_E_contained;
    TH1F *_h_mu_E_all;
    TH1F *_h_nu_E_contained;
    TH1F *_h_nu_E_all;
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
