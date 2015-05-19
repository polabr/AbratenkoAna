#ifndef NUENERGYRECO_CXX
#define NUENERGYRECO_CXX

#include "NuEnergyReco.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/mctruth.h"

#include <algorithm> //for std::sort

namespace larlite {

  bool NuEnergyReco::initialize() {

    if(tree) delete tree;
    tree = new TTree("tree","tree");
    tree->Branch("true_nu_E",&true_nu_E,"true_nu_E/D");
    /*
    tree->Branch("nu_vtx_x_from_mcnu",&nu_vtx_x_from_mcnu,"nu_vtx_x_from_mcnu/D");
    tree->Branch("nu_vtx_y_from_mcnu",&nu_vtx_y_from_mcnu,"nu_vtx_y_from_mcnu/D");
    tree->Branch("nu_vtx_z_from_mcnu",&nu_vtx_z_from_mcnu,"nu_vtx_z_from_mcnu/D");
    */
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
    tree->Branch("tot_binding_E",&tot_binding_E,"tot_binding_E/D");
    tree->Branch("E_ccqe",&E_ccqe,"E_ccqe/D");
    tree->Branch("lep_E",&lep_E,"lep_E/D");
    tree->Branch("n_lep",&n_lep,"n_lep/I");
    tree->Branch("n_protons",&n_protons,"n_protons/I");
    tree->Branch("tot_prot_E",&tot_prot_E,"tot_prot_E/D");
    tree->Branch("n_neutrons",&n_neutrons,"n_neutrons/I");
    tree->Branch("tot_neutron_E",&tot_prot_E,"tot_neutron_E/D");
    tree->Branch("max_prot_E",&max_prot_E,"max_prot_E/D");
    tree->Branch("second_prot_E",&second_prot_E,"second_prot_E/D");

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
    true_nu_E = 1000.*mcnu_traj.at(0).E();
    nu_vtx_x_from_mcnu = mcnu_traj.at(0).X();
    nu_vtx_y_from_mcnu = mcnu_traj.at(0).Y();
    nu_vtx_z_from_mcnu = mcnu_traj.at(0).Z();

    nuance_intxn = ev_tru->at(0).GetNeutrino().InteractionType();

    nu_pdg = ev_tru->at(0).GetNeutrino().Nu().PdgCode();

    hit_target = ev_tru->at(0).GetNeutrino().Target();
    hit_nuc = ev_tru->at(0).GetNeutrino().HitNuc();

    non_mcs_mct_E = 0;
    n_non_mcs_mct = 0;
    tot_binding_E = 0;
    n_lep = 0;
    std::vector<float> prot_energies;
    prot_energies.clear();
    std::vector<float> neut_energies;
    neut_energies.clear();

    auto mcpart_list = ev_tru->at(0).GetParticles();
    for (auto const mcp : mcpart_list){
      
      if (mcp.StatusCode() == 15)
	remnant_E = 1000*(mcp.Trajectory().at(0).E());
      
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
	  
	  if(mcp.PdgCode() == 2000000101){
	    tot_binding_E += 1000*mcp.Trajectory().at(0).E();
	  }
	}
	
	//Store the mcparticle lepton energy while we're at it
	if(abs(mcp.PdgCode()) == 13 || abs(mcp.PdgCode()) == 11){
	  lep_E = 1000.*mcp.Trajectory().at(0).E();
	  n_lep++;
	  auto lep_mom = 1000.*mcp.Trajectory().at(0).Momentum().Vect();
	  std::vector<double> tmp = {lep_mom.X(),lep_mom.Y(),lep_mom.Z(),lep_E};
	  E_ccqe = 1000.*_calc.ComputeECCQE(tmp);
	}
	if(abs(mcp.PdgCode()) == 2212){
	  prot_energies.push_back(1000.*mcp.Trajectory().at(0).E() - _p_mass);
	}
	if(abs(mcp.PdgCode()) == 2112){
	  neut_energies.push_back(1000.*mcp.Trajectory().at(0).E() - _n_mass);
	}
      }
    }
    
    //    std::cout<<"tot_binding_E = "<<tot_binding_E<<std::endl;
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

    //sort energies vector from lowest to highest
    std::sort(prot_energies.begin(),prot_energies.end());
    std::sort(neut_energies.begin(),neut_energies.end());

    //n_protons = prot_energies.size();
    //n_neutrons = neut_energies.size();
    

    max_prot_E = prot_energies.size() > 0 ? prot_energies.back() : -999.;
    second_prot_E = prot_energies.size() > 1 ? prot_energies.end()[-2] : -999.;

    tot_prot_E = 0.;
    tot_neutron_E = 0.;
    n_protons = 0;
    n_neutrons = 0;

    std::cout<<"fix this! in a more organized manner! you are only counting protons with above 30 mev!"<<std::endl;
    for (double E : prot_energies){
      tot_prot_E += E;
      if ( E > 30.) n_protons++;
    }
    for (double E : neut_energies){
      tot_neutron_E += E;
      if ( E > 30.) n_neutrons++;
    }
    //    tree->Fill();
  
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

    true_nu_E = 0;
    nu_vtx_x_from_mcnu = 0;
    nu_vtx_y_from_mcnu = 0;
    nu_vtx_z_from_mcnu = 0;

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

    E_ccqe = 0;
    lep_E = 0;

  }
}
#endif
