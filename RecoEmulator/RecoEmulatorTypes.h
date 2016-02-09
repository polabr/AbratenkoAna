/**
 * \file RecoEmulatorTypes.h
 *
 * \ingroup RecoEmulator
 *
 * \brief Class def header for a class RecoEmulatorTypes
 *
 * @author davidkaleko
 */

/** \addtogroup RecoEmulator

    @{*/
#ifndef RECOEMULATORTYPES_H
#define RECOEMULATORTYPES_H

#include "GeoAlgo/GeoTrajectory.h"
#include <iostream>

struct RecoEmulatorTrack_t {

	::geoalgo::Trajectory _trajectory;
	double _energy;
	std::vector<double> _dedx;

};

#endif
/** @} */ // end of doxygen group

