#ifndef NUENERGYRECO_CXX
#define NUENERGYRECO_CXX

#include "NuEnergyReco.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/mctruth.h"

namespace larlite {

  bool NuEnergyReco::initialize() {

    if(tree) delete tree;
    tree = new TTree("tree","tree");
    tree->Branch("nu_E_from_mcnu",&nu_E_from_mcnu,"nu_E_from_mcnu/D");
    tree->Branch("nu_vtx_x_from_mcnu",&nu_vtx_x_from_mcnu,"nu_vtx_x_from_mcnu/D");
    tree->Branch("nu_vtx_y_from_mcnu",&nu_vtx_y_from_mcnu,"nu_vtx_y_from_mcnu/D");
    tree->Branch("nu_vtx_z_from_mcnu",&nu_vtx_z_from_mcnu,"nu_vtx_z_from_mcnu/D");
    tree->Branch("nu_E_from_mcpart",&nu_E_from_mcpart,"nu_E_from_mcpart/D");
    tree->Branch("nu_vtx_x_from_mcpart",&nu_vtx_x_from_mcpart,"nu_vtx_x_from_mcpart/D");
    tree->Branch("nu_vtx_y_from_mcpart",&nu_vtx_y_from_mcpart,"nu_vtx_y_from_mcpart/D");
    tree->Branch("nu_vtx_z_from_mcpart",&nu_vtx_z_from_mcpart,"nu_vtx_z_from_mcpart/D");
    tree->Branch("remnant_E",&remnant_E,"remnant_E/D");
    tree->Branch("tot_mcs_E",&tot_mcs_E,"tot_mcs_E/D");
    tree->Branch("tot_mct_E",&tot_mct_E,"tot_mct_E/D");
    tree->Branch("n_mcs",&n_mcs,"n_mcs/I");
    tree->Branch("n_mct",&n_mct,"n_mct/I");
    tree->Branch("tot_initial_E",&tot_initial_E,"tot_initial_E/D");
    tree->Branch("tot_final_E",&tot_final_E,"tot_final_E/D");
    tree->Branch("non_mcs_mct_E",&non_mcs_mct_E,"non_mcs_mct_E/D");
    tree->Branch("n_non_mcs_mct",&n_non_mcs_mct,"n_non_mcs_mct/I");
    tree->Branch("nuance_intxn",&nuance_intxn,"nuance_intxn/I");
    tree->Branch("hit_target",&hit_target,"hit_target/I");
    tree->Branch("hit_nuc",&hit_nuc,"hit_nuc/I");
    tree->Branch("nu_pdg",&nu_pdg,"nu_pdg/I");
    
    return true;
  }
  
  bool NuEnergyReco::analyze(storage_manager* storage) {

    ResetTTreeVars();
    
    auto ev_mcs = storage->get_data<event_mcshower>(  "mcreco" );
    auto ev_mct = storage->get_data<event_mctrack >(  "mcreco" );
    auto ev_tru = storage->get_data<event_mctruth >("generator");

    if(!ev_mcs || !ev_mct || !ev_tru){
      std::cout<<"wtf"<<std::endl;
      return false;
    }
    
    auto mcnu_traj = ev_tru->at(0).GetNeutrino().Nu().Trajectory();
    nu_E_from_mcnu = 1000.*mcnu_traj.at(0).E();
    nu_vtx_x_from_mcnu = mcnu_traj.at(0).X();
    nu_vtx_y_from_mcnu = mcnu_traj.at(0).Y();
    nu_vtx_z_from_mcnu = mcnu_traj.at(0).Z();

    nuance_intxn = ev_tru->at(0).GetNeutrino().InteractionType();

    nu_pdg = ev_tru->at(0).GetNeutrino().Nu().PdgCode();

    hit_target = ev_tru->at(0).GetNeutrino().Target();
    hit_nuc = ev_tru->at(0).GetNeutrino().HitNuc();

    non_mcs_mct_E = 0;
    n_non_mcs_mct = 0;
    auto mcpart_list = ev_tru->at(0).GetParticles();
    for (auto const mcp : mcpart_list){

      if (mcp.StatusCode() == 15)
	remnant_E = 1000*(mcp.Trajectory().at(0).E());

      if (abs(mcp.PdgCode()) == 12){
	nu_E_from_mcpart = 1000.*mcp.Trajectory().at(0).E();
	nu_vtx_x_from_mcpart = mcp.Trajectory().at(0).X();
	nu_vtx_y_from_mcpart = mcp.Trajectory().at(0).Y();
	nu_vtx_z_from_mcpart = mcp.Trajectory().at(0).Z();
      }

      if (mcp.StatusCode() == 1){
	//These are all the PDGs that are made into mcshowers or mctracks (by definition)
	//pi0 removed because its two gammas (22) are what matter. these are removed
	if( mcp.PdgCode() != 22 &&
	    mcp.PdgCode() != 11 &&
	    mcp.PdgCode() != -11 &&
	    mcp.PdgCode() != 211 &&
	    mcp.PdgCode() != -211 &&
	    //mcp.PdgCode() != 111 &&
	    mcp.PdgCode() != 13 &&
	    mcp.PdgCode() != -13 &&
	    mcp.PdgCode() != 2212 &&
	    mcp.PdgCode() != 321 &&
	    mcp.PdgCode() != -321  ){
	  non_mcs_mct_E += 1000*mcp.Trajectory().at(0).E();
	  n_non_mcs_mct++;
	}
      }
    }

    tot_initial_E = ComputeTotalInitialE(mcpart_list);
    tot_final_E   = ComputeTotalFinalE(mcpart_list);
    

    for (auto const& mcs : *ev_mcs){
      //Avoid double counting
      if(mcs.AncestorTrackID() != mcs.TrackID()) continue;
      //Not using detprofile here. This energy includes mass.
      tot_mcs_E += mcs.Start().E();
    }

    for (auto const& mct : *ev_mct){
      //Avoid double counting
      if(mct.AncestorTrackID() != mct.TrackID()) continue;
      //This energy includes mass. Not counting only deposited energy.
      tot_mct_E += mct.Start().Momentum().E();
    }

    n_mcs = ev_mcs->size();
    n_mct = ev_mct->size();
      
    tree->Fill();
  
    return true;
  }

  bool NuEnergyReco::finalize() {

    if(_fout) { _fout->cd(); tree->Write(); }
  
    return true;
  }

  double NuEnergyReco::ComputeTotalInitialE(const std::vector<larlite::mcpart> & myparts){

    double tmp_E = 0.;
    for (auto const mcp : myparts){
      if (mcp.StatusCode() == 0)
	tmp_E += 1000.*mcp.Trajectory().at(0).E();
    }

    return tmp_E;
  }

  double NuEnergyReco::ComputeTotalFinalE(const std::vector<larlite::mcpart> & myparts){

    double tmp_E = 0.;
    for (auto const mcp : myparts){
      if (mcp.StatusCode() == 1 || mcp.StatusCode() == 15)
	tmp_E += 1000.*mcp.Trajectory().at(0).E();
    }
    return tmp_E;
  }

  void NuEnergyReco::ResetTTreeVars(){

    nu_E_from_mcnu = 0;
    nu_vtx_x_from_mcnu = 0;
    nu_vtx_y_from_mcnu = 0;
    nu_vtx_z_from_mcnu = 0;

    nu_E_from_mcpart = 0;
    nu_vtx_x_from_mcpart = 0;
    nu_vtx_y_from_mcpart = 0;
    nu_vtx_z_from_mcpart = 0;

    remnant_E = 0;

    tot_mcs_E = 0;
    tot_mct_E = 0;
    n_mcs = 0;
    n_mct = 0;

    tot_initial_E = 0;
    tot_final_E = 0;
    
    non_mcs_mct_E = 0;
    n_non_mcs_mct = 0;
    
    nuance_intxn = -1;
    hit_target = -1;
    hit_nuc = -1;

    nu_pdg = -1;

  }
}
#endif
