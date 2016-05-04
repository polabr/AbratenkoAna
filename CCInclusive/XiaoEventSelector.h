/**
 * \file XiaoEventSelector.h
 *
 * \ingroup CCInclusive
 *
 * \brief Copying anatree stuff to larlite:
 *  http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=5724
 *     these are the producer names that xiao uses
 *     note you need to skim off calorimetry objects to get track dedx!
 *
 *     track: pandoraNuPMA
 *     vtx: pmtrack
 *     calorimetry: pandoraNuPMAcalo

 *
 * @author davidkaleko
 */

/** \addtogroup CCInclusive

    @{*/

#ifndef LARLITE_XIAOEVENTSELECTOR_H
#define LARLITE_XIAOEVENTSELECTOR_H

#include "Analysis/ana_base.h"
#include "DataFormat/track.h"
#include "DataFormat/vertex.h"
#include "DataFormat/calorimetry.h"
#include "GeoAlgo/GeoVector.h"
#include "GeoAlgo/GeoSphere.h"
#include "GeoAlgo/GeoAABox.h"
#include "LArUtil/Geometry.h"
#include "TH1.h"
#include "TH2.h"

namespace larlite {
  /**
     \class XiaoEventSelector
     User custom analysis class made by SHELL_USER_NAME
   */
  class XiaoEventSelector : public ana_base {

  public:

    /// Default constructor
    XiaoEventSelector() { 
      _name = "XiaoEventSelector"; 
    _fout = 0;
     _hmult = 0; 
     _hdedx = 0;
     _hcorrect_ID = 0;
     _running_on_data = false;
   }

    /// Default destructor
    virtual ~XiaoEventSelector() {}

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

    void setRunningOnData(bool flag) { _running_on_data = flag; }

  protected:


    double flashDistZ(const track &longest_track, const double flash_z);

    bool trackAssociatedWithVtx(const track &trk,
                                const ::geoalgo::Sphere &vtx_sphere);

    bool isMichelMID(const ::geoalgo::Sphere &vtx_sphere,
                     const std::vector<size_t> trk_idx_vec,
                     const event_track *ev_track,
                     const event_calorimetry *ev_calo,
                     const larlite::AssSet_t & ass_calo_v);

    ::geoalgo::Sphere getVertexSphere(const vertex &vtx);

    size_t total_events;
    size_t passed_events;
    double fidvol_dist;
    double fidvol_dist_y;

    // Fiducial volume box
    geoalgo::AABox _fidvolBox;

    TH1F *_hmult;
    TH2D *_hdedx;
    TH1F *_hcorrect_ID;

    bool _running_on_data;


  };
}
#endif

/** @} */ // end of doxygen group
