/**
 * \file VerifyShowerLengthFormula.h
 *
 * \ingroup scratch_ana
 * 
 * \brief Class def header for a class VerifyShowerLengthFormula
 *
 * @author davidkaleko
 */

/** \addtogroup scratch_ana

    @{*/

#ifndef LARLITE_VERIFYSHOWERLENGTHFORMULA_H
#define LARLITE_VERIFYSHOWERLENGTHFORMULA_H

#include "Analysis/ana_base.h"
#include "KalekoAna/SignalSelection/ShowerAssessment.h"
#include "BasicTool/GeoAlgo/DistToBoxWall.h"
#include "LArUtil/Geometry.h"

namespace larlite {
  /**
     \class VerifyShowerLengthFormula
     User custom analysis class made by davidkaleko
   */
  class VerifyShowerLengthFormula : public ana_base{
  
  public:

    /// Default constructor
    VerifyShowerLengthFormula(){ 
      _name="VerifyShowerLengthFormula";
      _fout=0;
      _sa = 0;
      _ana_tree = 0;
    };

    /// Default destructor
    virtual ~VerifyShowerLengthFormula(){};

    /** IMPLEMENT in VerifyShowerLengthFormula.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in VerifyShowerLengthFormula.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in VerifyShowerLengthFormula.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    protected:

    //Instance of ShowerAssessment
    ShowerAssessment *_sa;

    TTree* _ana_tree;
    double _mcs_dist_to_wall;
    double _mcs_E_containment;
    double _est_E_containment;

    //Geometry algo instance
    ::geoalgo::DistToBoxWall _DistToBoxWall;
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
