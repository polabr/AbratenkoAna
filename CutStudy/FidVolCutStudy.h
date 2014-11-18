/**
 * \file FidVolCutStudy.h
 *
 * \ingroup CutStudy
 * 
 * \brief Class def header for a class FidVolCutStudy
 *
 * @author davidkaleko
 */

/** \addtogroup CutStudy

    @{*/

#ifndef LARLITE_FIDVOLCUTSTUDY_H
#define LARLITE_FIDVOLCUTSTUDY_H

#include "Analysis/ana_base.h"
#include "BasicTool/GeoAlgo/DistToBoxWall.h"
#include "LArUtil/Geometry.h"

namespace larlite {
  /**
     \class FidVolCutStudy
     User custom analysis class made by davidkaleko
   */
  class FidVolCutStudy : public ana_base{
  
  public:

    /// Default constructor
    FidVolCutStudy(){ _name="FidVolCutStudy"; _fout=0; _ana_tree=0;};

    /// Default destructor
    virtual ~FidVolCutStudy(){};

    /** IMPLEMENT in FidVolCutStudy.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in FidVolCutStudy.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in FidVolCutStudy.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:
   

  private:

    //TTree and variables contained therein
    TTree *_ana_tree;
    double _vtx_to_wall_dist;
    double _E_containment;
    double _fwd_to_wall_dist;
    double _E_mother;

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
