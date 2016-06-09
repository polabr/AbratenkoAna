/**
 * \file XiaoNuFinder.h
 *
 * \ingroup CCInclusive
 *
 * \brief Class def header for a class XiaoNuFinder
 *
 * @author davidkaleko
 */

/** \addtogroup CCInclusive

    @{*/
#ifndef LARLITE_XIAONUFINDER_H
#define LARLITE_XIAONUFINDER_H

#include <iostream>
#include "Analysis/ana_base.h"
#include "DataFormat/track.h"
#include "DataFormat/vertex.h"
#include "DataFormat/calorimetry.h"
#include "GeoAlgo/GeoVector.h"
#include "GeoAlgo/GeoSphere.h"
#include "GeoAlgo/GeoAABox.h"
#include "LArUtil/Geometry.h"
#include "CCInclusiveConstants.h"
#include "TrackMomentumSplines.h"

/**
   \class XiaoNuFinder
   User defined class XiaoNuFinder ... these comments are used to generate
   doxygen documentation!
 */

namespace larlite {

	class XiaoNuFinder : public larlite_base {

	public:
	
		void setInputType(InputFileType_t filetype) { _filetype = filetype; }

		/// Default constructor
		XiaoNuFinder();

		/// Default destructor
		~XiaoNuFinder() {}

		std::pair<larlite::vertex, std::vector<larlite::track> > findNeutrino(const event_track *ev_track,
		        const event_calorimetry *ev_calo,
		        const larlite::AssSet_t & ass_calo_v,
		        const event_vertex *ev_vtx,
		        const event_opflash *ev_flash);

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

		bool setBGWTimes();

		InputFileType_t _filetype;

		double fidvol_dist_x;
		double fidvol_dist_y;
		double fidvol_dist_z;
		double BGW_mintime;
		double BGW_maxtime;

		// Fiducial volume box
		geoalgo::AABox _fidvolBox;

		// Track momentum splines
		TrackMomentumSplines *myspline;
	};

}
#endif
/** @} */ // end of doxygen group

