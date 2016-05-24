/**
 * \file HitStudy.h
 *
 * \ingroup scratch_ana
 *
 * \brief Class def header for a class HitStudy
 *
 * @author davidkaleko
 */

/** \addtogroup scratch_ana

    @{*/

#ifndef LARLITE_HITSTUDY_H
#define LARLITE_HITSTUDY_H

#include "Analysis/ana_base.h"
#include "LArUtil/LArUtilManager.h"
#include <TH1F.h>

namespace larlite {
/**
   \class HitStudy
   User custom analysis class made by SHELL_USER_NAME
 */
class HitStudy : public ana_base {

public:

  /// Default constructor
  HitStudy() { _name = "HitStudy"; _fout = 0;}

  /// Default destructor
  virtual ~HitStudy() {}

  /** IMPLEMENT in HitStudy.cc!
      Initialization method to be called before the analysis event loop.
  */
  virtual bool initialize();

  /** IMPLEMENT in HitStudy.cc!
      Analyze a data event-by-event
  */
  virtual bool analyze(storage_manager* storage);

  /** IMPLEMENT in HitStudy.cc!
      Finalize method to be called after all events processed.
  */
  virtual bool finalize();

protected:
 
  //get the geo/detector singletons only once
  const larutil::Geometry* mygeo;
  const larutil::GeometryHelper* mygeoutil;
  const larutil::DetectorProperties* mydetprop;

  double fTimetoCm;
  double fWiretoCm;
  double fElectronsToADC;

  std::vector<TH1F*> _hit_wires;
  std::vector<TH1F*> _hit_times;
  
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
