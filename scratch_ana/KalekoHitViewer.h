/**
 * \file KalekoHitViewer.h
 *
 * \ingroup scratch_ana
 * 
 * \brief Class to read in reconstructed hits and make a TH2 histogram of them
 *
 * @author davidkaleko
 */

/** \addtogroup scratch_ana

    @{*/
#ifndef LARLITE_KALEKOHITVIEWER_H
#define LARLITE_KALEKOHITVIEWER_H

#include <iostream>
#include "Analysis/ana_base.h"
#include "LArUtil/LArUtilManager.h"
#include "DataFormat/hit.h"
#include "TH2D.h"


/* NOTE: this is _not_ an ana_processor module like you are used to.
This started as an empty class (llgen_class_empty).
The way this one works is that the python run script will get the data
from the data file, and hand it to this class for each event.
That's why there are no "initialize", "analyze", "finalize" functions here.
Instead, you should initialize histograms and things in the constructor.
*/

namespace larlite{				
  /**
     \class KalekoHitViewer
     User defined class KalekoHitViewer ... these comments are used to generate
     doxygen documentation!
  */
  class KalekoHitViewer{
    
  public:
    
    /// Default constructor
    KalekoHitViewer();
    
    /// Default destructor
    ~KalekoHitViewer();

    void GenerateHisto(const larlite::event_hit *ev_hit, int plane);

    TH2D* GetHitHisto() { return hHitHisto; }

  protected:

    TH2D* hHitHisto;
    
    const larutil::GeometryUtilities* mygeoutil;
    double fTimetoCm;
    double fWiretoCm;

  };

}//end namespace larlite  
#endif
  /** @} */ // end of doxygen group 
  
