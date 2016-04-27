/**
 * \file MuTrackMomentumSpline.h
 *
 * \ingroup CCInclusive
 *
 * \brief This is kaleko copying some stuff to compute a muon momentum from a range table. Copied from here:
 * https://cdcvs.fnal.gov/redmine/projects/larreco/repository/revisions/develop/entry/larreco/RecoAlg/TrackMomentumCalculator.cxx
 *
 * @author davidkaleko
 */

/** \addtogroup CCInclusive

    @{*/
#ifndef MUTRACKMOMENTUMSPLINE_H
#define MUTRACKMOMENTUMSPLINE_H

#include <iostream>
 #include "TGraph.h"
 #include "TSpline.h"

/**
   \class MuTrackMomentumSpline
   User defined class MuTrackMomentumSpline ... these comments are used to generate
   doxygen documentation!
 */
namespace larlite {

	class MuTrackMomentumSpline {

	public:

		/// Default constructor
		MuTrackMomentumSpline();

		/// Default destructor
		~MuTrackMomentumSpline() {}

		double GetMuMomentum(const double muon_range);

	private:

		TGraph *KEvsR;
		TSpline3 *KEvsR_spline3;
	};

}
#endif
/** @} */ // end of doxygen group

