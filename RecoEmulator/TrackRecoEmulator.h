/**
 * \file TrackRecoEmulator.h
 *
 * \ingroup RecoEmulator
 * 
 * \brief Class def header for a class TrackRecoEmulator
 *
 * @author davidkaleko
 */

/** \addtogroup RecoEmulator

    @{*/

#ifndef LARLITE_TRACKRECOEMULATOR_H
#define LARLITE_TRACKRECOEMULATOR_H

#include "Analysis/ana_base.h"
#include "RecoEmulatorTypes.h"

namespace larlite {
  /**
     \class TrackRecoEmulator
     User custom analysis class made by SHELL_USER_NAME
   */
  class TrackRecoEmulator : public ana_base{
  
  public:

    /// Default constructor
    TrackRecoEmulator(){ _name="TrackRecoEmulator"; _fout=0;}

    /// Default destructor
    virtual ~TrackRecoEmulator(){}
 
    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

  protected:
    
    /// Acts directly w/ reference to struct object
    /// Returns FALSE if the track should be removed (IE not reconstructed)
     bool applySmearing(RecoEmulatorTrack_t &RET);


     /// Debugging: print info about the struct
     void printStructInfo(const RecoEmulatorTrack_t &ret);

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
