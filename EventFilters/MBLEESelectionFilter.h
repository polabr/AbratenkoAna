/**
 * \file MBLEESelectionFilter.h
 *
 * \ingroup EventFilters
 *
 * \brief Filter to pick out events that MiniBooNE would have accepted
 *        (for example, 1 subevent, 1 electron, etc)
 *
 * @author davidkaleko
 */

/** \addtogroup EventFilters

    @{*/

#ifndef LARLITE_MBLEESELECTIONFILTER_H
#define LARLITE_MBLEESELECTIONFILTER_H

#include "Analysis/ana_base.h"
#include <stdexcept>

namespace larlite {
/**
   \class MBLEESelectionFilter
   User custom analysis class made by davidkaleko
 */
class MBLEESelectionFilter : public ana_base {

public:

  /// Default constructor
  MBLEESelectionFilter() { _name = "MBLEESelectionFilter"; _fout = 0; };

  /// Default destructor
  virtual ~MBLEESelectionFilter() {};

  virtual bool initialize();

  virtual bool analyze(storage_manager* storage);

  virtual bool finalize();

protected:

  size_t total_events;
  size_t kept_events;

  /// Map going from PDG to cherenkov threshold energy in CH2
  /// These values taken from Georgia's thesis (they include mass, presumably)




  std::map<size_t, float> _cherenk_thresholds_MEV =
  { {11, 0.7},      /// e  ==> 0.7 MeV
    {211, 190.},    /// pi ==> 190 MeV
    {13, 144.},     /// mu ==> 144 MeV
    {2212, 1280.},  /// p  ==> 1280 MeV
    {2112, -1.}     /// n ==> don't care
  };

  /// Georgia's thesis says:
  /// "A minimum visible energy cut, Evis = Ee − me > 140 MeV, is required to reject NC events"
  float _min_vis_energy_MEV = 140.;

  /// Georgia's thesis says:
  /// "An additional kinematics-based cut is used to reject mis-identified events due to interactions
  /// that take place outside the detector, or close to the fiducial volume boundary."
  /// ********* implement this fiducial volume cut! *********
  float _dist_to_wall = 99999999.;

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
