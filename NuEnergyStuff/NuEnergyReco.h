/**
 * \file NuEnergyReco.h
 *
 * \ingroup scratch_ana
 * 
 * \brief Class def header for a class NuEnergyReco
 *
 * @author davidkaleko
 */

/** \addtogroup scratch_ana

    @{*/

#ifndef LARLITE_NUENERGYRECO_H
#define LARLITE_NUENERGYRECO_H

#include "Analysis/ana_base.h"
#include "DataFormat/mcpart.h"
#include "TTree.h"
#include "ECCQECalculator.h"

namespace larlite {
  /**
     \class NuEnergyReco
     User custom analysis class made by davidkaleko
   */
  class NuEnergyReco : public ana_base{
  
  public:

    /// Default constructor
    NuEnergyReco(){ 
      _name="NuEnergyReco"; 
      _min_p_E = 0.;
      _min_n_E = 0.;
      tree=0; 
      _fout=0;
    };
    
    /// Default destructor
    virtual ~NuEnergyReco(){};

    /** IMPLEMENT in NuEnergyReco.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in NuEnergyReco.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in NuEnergyReco.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    
    void SetMinPE(double kaleko){ _min_p_E = kaleko; }
    void SetMinNE(double kaleko){ _min_n_E = kaleko; }

    protected:

    
    double _min_p_E;
    double _min_n_E;
    
    double _p_mass = 938.27;
    double _n_mass = 939.57;

    void ResetTTreeVars();

    TTree* tree;
    double true_nu_E;
    int nu_pdg;
    int nuance_intxn;
    double E_ccqe;
    double lep_E;
    int n_lep;
    double tot_prot_E;
    int n_protons;
    double tot_neutron_E;
    int n_neutrons;
    double max_prot_E;
    double second_prot_E;
    double tot_pt;

    larlite::util::ECCQECalculator _calc;


  };
}
#endif
