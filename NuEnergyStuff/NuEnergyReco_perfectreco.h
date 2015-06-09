/**
 * \file NuEnergyReco_perfectreco.h
 *
 * \ingroup NuEnergyStuff
 * 
 * \brief Class def header for a class NuEnergyReco_perfectreco
 *
 * @author davidkaleko
 */

/** \addtogroup NuEnergyStuff

    @{*/

#ifndef LARLITE_NUENERGYRECO_PERFECTRECO_H
#define LARLITE_NUENERGYRECO_PERFECTRECO_H

#include "Analysis/ana_base.h"
#include "TTree.h"

namespace larlite {
  /**
     \class NuEnergyReco_perfectreco
     User custom analysis class made by davidkaleko
     This uses on mctruth to study the best way to compute the true
     nu energy from "perfect reconstructed" (mctrack/mcshower) quantities.
     It basically makes a ttree and stores it for TTree->Draw stuff.
   */
  class NuEnergyReco_perfectreco : public ana_base{
  
  public:

    /// Default constructor
    NuEnergyReco_perfectreco(){ 
      _name="NuEnergyReco_perfectreco"; 
      _min_p_E_MEV = 30.;
      _min_n_E_MEV = -1.;
      tree=0; 
      _fout=0;
    }

    /// Default destructor
    virtual ~NuEnergyReco_perfectreco(){}

    /** IMPLEMENT in NuEnergyReco_perfectreco.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in NuEnergyReco_perfectreco.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in NuEnergyReco_perfectreco.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetMinPE_MEV(double kaleko){ _min_p_E_MEV = kaleko; }
    void SetMinNE_MEV(double kaleko){ _min_n_E_MEV = kaleko; }


  protected:

    double _min_p_E_MEV;
    double _min_n_E_MEV;

    void ResetTTreeVars();
    

    TTree* tree;
    double true_nu_E; //
    int nu_pdg; //
    int nuance_intxn; //
    bool isCC; //
    int _mode; //
    double lep_E;
    int n_lep;
    double tot_primaryprot_KE;
    int n_primary_protons;
    double tot_secondaryprot_KE;
    int n_secondary_protons;
    double avg_dist_to_secondary_protons;
    int n_protons_mctruth;
    double tot_neutron_KE;
    int n_neutrons;
    double tot_pt;

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
