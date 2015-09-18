/**
 * \file MBLEESelection.h
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

#ifndef LARLITE_MBLEESelection_H
#define LARLITE_MBLEESelection_H

#include "Analysis/ana_base.h"
#include <stdexcept>

namespace larlite {
/**
   \class MBLEESelection
   User custom analysis class made by davidkaleko
 */
class MBLEESelection : public ana_base {

public:

  /// Default constructor
  MBLEESelection() { _name = "MBLEESelection"; _fout = 0; };

  /// Default destructor
  virtual ~MBLEESelection() {};

  virtual bool initialize();

  virtual bool analyze(storage_manager* storage);

  virtual bool finalize();

protected:

  size_t total_events;
  size_t kept_events;

  /// Map going from PDG to cherenkov threshold energy in CH2
  std::map<size_t, float> _cherenk_thresholds = {{11, 1.234}, {22, 5.678}};

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
