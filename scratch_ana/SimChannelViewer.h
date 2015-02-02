/**
 * \file SimChannelViewer.h
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class SimChannelViewer
 *
 * @author davidkaleko
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef LARLITE_SIMCHANNELVIEWER_HH
#define LARLITE_SIMCHANNELVIEWER_HH


#include <iostream>
#include "Analysis/ana_base.h"
#include "LArUtil/LArUtilManager.h"
#include <TH2D.h>
#include "DataFormat/simch.h"

namespace cluster{

  /**
     \class SimChannelViewer
     User defined class SimChannelViewer ... these comments are used to generate
     doxygen documentation!
  */
  
  class SimChannelViewer{
    
  public:
    
    /// Default constructor
    SimChannelViewer();
    
    /// Default destructor
    virtual ~SimChannelViewer(){};

    void GenerateHisto(const larlite::event_simch *esch, int plane);


    TH2D* GetSimChView() { return hSimChView; }


  protected:

    TH2D* hSimChView;

    std::map<UShort_t, std::vector< larlite::ide > > fTDCIDEs;
};

  
}
#endif
/** @} */ // end of doxygen group 

