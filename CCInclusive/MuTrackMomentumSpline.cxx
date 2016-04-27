#ifndef MUTRACKMOMENTUMSPLINE_CXX
#define MUTRACKMOMENTUMSPLINE_CXX

#include "MuTrackMomentumSpline.h"

namespace larlite {

	MuTrackMomentumSpline::MuTrackMomentumSpline() {

		// Initialize the spline with hardcoded (fit) variables. 
		// See MuTrackMomentumSpline header file to see where these came from.
		Float_t Range_grampercm[29] = {9.833E-1, 1.786E0, 3.321E0, 6.598E0, 1.058E1, 3.084E1, 4.250E1,
		                               6.732E1, 1.063E2, 1.725E2, 2.385E2, 4.934E2,
		                               6.163E2, 8.552E2, 1.202E3, 1.758E3, 2.297E3,
		                               4.359E3, 5.354E3, 7.298E3, 1.013E4, 1.469E4,
		                               1.910E4, 3.558E4, 4.326E4, 5.768E4, 7.734E4, 1.060E5, 1.307E5
		                              };
		Float_t KE_MeV[29] = {10, 14, 20, 30, 40, 80, 100, 140, 200, 300, 400, 800, 1000,
		                      1400, 2000, 3000, 4000, 8000, 10000, 14000, 20000, 30000,
		                      40000, 80000, 100000, 140000, 200000, 300000, 400000
		                     };

		KEvsR = new TGraph(29, Range_grampercm, KE_MeV);
		KEvsR_spline3 = new TSpline3("KEvsRS", KEvsR);



	}

	double MuTrackMomentumSpline::GetMuMomentum(const double muon_range_cm) {
		double argon_density = 1.396; //g/cm^3
		// input to spline should be grams per cm^2
		// take range in cm and multiply by argon_density
		return KEvsR_spline3->Eval(muon_range_cm*argon_density);
	}


}
#endif
