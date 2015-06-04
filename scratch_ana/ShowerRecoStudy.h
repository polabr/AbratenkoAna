/**
 * \file ShowerRecoStudy.h
 *
 * \ingroup scratch_ana
 * 
 * \brief Class def header for a class ShowerRecoStudy
 *
 * @author davidkaleko
 */

/** \addtogroup scratch_ana

    @{*/

#ifndef LARLITE_SHOWERRECOSTUDY_H
#define LARLITE_SHOWERRECOSTUDY_H

#include "Analysis/ana_base.h"
#include "TTree.h"
#include "EMShowerTools/EMShowerProfile.h"

namespace larlite {
  /**
     \class ShowerRecoStudy
     User custom analysis class made by davidkaleko
   */
  class ShowerRecoStudy : public ana_base{
  
  public:

    /// Default constructor
    ShowerRecoStudy(){ 
      _name="ShowerRecoStudy"; 
      _fout=0;
      _ana_tree = 0;
    };

    /// Default destructor
    virtual ~ShowerRecoStudy(){};

    /** IMPLEMENT in ShowerRecoStudy.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ShowerRecoStudy.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ShowerRecoStudy.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    protected:

    TTree* _ana_tree;
    size_t _n_reco_showers;
    size_t _n_true_showers;
    double _reco_show_length;
    double _true_show_length;
    double _prof_show_length;
    double _reco_show_E;
    double _true_show_E;
    double _st_pt_diff;
    bool _E_contained;
    double _reco_show_OA;
    double _prof_show_OA;
    double _reco_true_3d_anglediff;
    double _reco_show_direrr;

    EMShowerProfile _shrProfiler;
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
