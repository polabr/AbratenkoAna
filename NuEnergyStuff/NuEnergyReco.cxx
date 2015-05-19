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
    tree->Branch("nu_pdg",&nu_pdg,"nu_pdg/I");
    tree->Branch("nuance_intxn",&nuance_intxn,"nuance_intxn/I");

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
    nu_pdg = ev_tru->at(0).GetNeutrino().Nu().PdgCode();
    nuance_intxn = ev_tru->at(0).GetNeutrino().InteractionType();

    n_lep = 0;
    std::vector<float> prot_energies;
    prot_energies.clear();
    std::vector<float> neut_energies;
    neut_energies.clear();

    auto mcpart_list = ev_tru->at(0).GetParticles();
    for (auto const mcp : mcpart_list){
      if (mcp.StatusCode() == 1){
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
    
    //sort energies vector from lowest to highest
    std::sort(prot_energies.begin(),prot_energies.end());
    std::sort(neut_energies.begin(),neut_energies.end());

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
    tree->Fill();
  
    return true;
  }

  bool NuEnergyReco::finalize() {

    if(_fout) { _fout->cd(); tree->Write(); }
  
    return true;
  }

  void NuEnergyReco::ResetTTreeVars(){
    true_nu_E = 0;
    nu_pdg = -1;    
    nuance_intxn = -1;
    E_ccqe = 0;
    lep_E = 0;
  }
}
#endif
