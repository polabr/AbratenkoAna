/**
 * \file CheckGenieOutput.h
 *
 * \ingroup NuEnergyStuff
 * 
 * \brief Class def header for a class CheckGenieOutput
 *
 * @author davidkaleko
 */

/** \addtogroup NuEnergyStuff

    @{*/

#ifndef LARLITE_CHECKGENIEOUTPUT_H
#define LARLITE_CHECKGENIEOUTPUT_H

#include "Analysis/ana_base.h"
#include "DataFormat/mcpart.h"
#include "TTree.h"

namespace larlite {
  /**
     \class CheckGenieOutput
     User custom analysis class made by SHELL_USER_NAME
   */
  class CheckGenieOutput : public ana_base{
  
  public:

    /// Default constructor
    CheckGenieOutput(){ _name="CheckGenieOutput"; tree=0; _fout=0;}

    /// Default destructor
    virtual ~CheckGenieOutput(){}

    /** IMPLEMENT in CheckGenieOutput.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in CheckGenieOutput.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in CheckGenieOutput.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:
        
    double _p_mass = 938.27;
    double _n_mass = 939.57;
    
    void ResetTTreeVars();

    double ComputeNuEFromMCPart(const std::vector<larlite::mcpart> &myparts);
    double ComputeTotalInitialE(const std::vector<larlite::mcpart> &myparts);
    double ComputeTotalFinalE(const std::vector<larlite::mcpart> &myparts);
    
    TTree* tree;
    double true_nu_E;
    double nu_vtx_x_from_mcnu;
    double nu_vtx_y_from_mcnu;
    double nu_vtx_z_from_mcnu;
    double nu_E_from_mcpart;
    double nu_vtx_x_from_mcpart;
    double nu_vtx_y_from_mcpart;
    double nu_vtx_z_from_mcpart;
    double remnant_E;
    double tot_initial_E;
    double tot_final_E;
    double non_mcs_mct_E;
    double tot_mcs_E;
    double tot_mct_E;
    int nuance_intxn;
    int hit_target;
    int hit_nuc;
    int nu_pdg;
    double tot_binding_E;


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
