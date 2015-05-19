/**
 * \file ProtonStudy.h
 *
 * \ingroup scratch_ana
 * 
 * \brief Quick ana to look at how low energy we can reconstruct protons
 *
 * @author davidkaleko
 */

/** \addtogroup scratch_ana

    @{*/

#ifndef LARLITE_PROTONSTUDY_H
#define LARLITE_PROTONSTUDY_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class ProtonStudy
     User custom analysis class made by davidkaleko
   */
  class ProtonStudy : public ana_base{
  
  public:

    /// Default constructor
    ProtonStudy(){ _name="ProtonStudy"; _fout=0; _ana_tree=0;};

    /// Default destructor
    virtual ~ProtonStudy(){};

    /** IMPLEMENT in ProtonStudy.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ProtonStudy.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ProtonStudy.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    protected:

    TTree* _ana_tree;
    double _sum_trk_len;
    double _vect_trk_len;
    double _prot_KE;



    double proton_mass = 938.272;


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
