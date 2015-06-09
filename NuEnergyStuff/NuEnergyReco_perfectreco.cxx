#ifndef LARLITE_NUENERGYRECO_PERFECTRECO_CXX
#define LARLITE_NUENERGYRECO_PERFECTRECO_CXX

#include "NuEnergyReco_perfectreco.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/mctruth.h"
#include <algorithm> //for std::sort

namespace larlite {
  
  bool NuEnergyReco_perfectreco::initialize() {
    
    if(tree) delete tree;

    tree = new TTree("tree","tree");
    tree->Branch("true_nu_E",&true_nu_E,"true_nu_E/D");
    tree->Branch("nu_pdg",&nu_pdg,"nu_pdg/I");
    tree->Branch("nuance_intxn",&nuance_intxn,"nuance_intxn/I");
    tree->Branch("isCC",&isCC,"isCC/O");
    tree->Branch("mode",&_mode,"mode/I");
    tree->Branch("lep_E",&lep_E,"lep_E/D");
    tree->Branch("n_lep",&n_lep,"n_lep/I");
    tree->Branch("n_protons_mctruth",&n_protons_mctruth,"n_protons_mctruth/I");
    tree->Branch("tot_primaryprot_KE",&tot_primaryprot_KE,"tot_primaryprot_KE/D");
    tree->Branch("n_primary_protons",&n_primary_protons,"n_primary_protons/I");
    tree->Branch("tot_secondaryprot_KE",&tot_secondaryprot_KE,"tot_secondaryprot_KE/D");
    tree->Branch("n_secondary_protons",&n_secondary_protons,"n_secondary_protons/I");
    tree->Branch("avg_dist_to_secondary_protons",&avg_dist_to_secondary_protons,"avg_dist_to_secondary_protons/D");
    tree->Branch("n_neutrons",&n_neutrons,"n_neutrons/I");
    tree->Branch("tot_neutron_KE",&tot_neutron_KE,"tot_neutron_KE/D");
    tree->Branch("tot_pt",&tot_pt,"tot_pt/D");

    return true;
  }
  
  bool NuEnergyReco_perfectreco::analyze(storage_manager* storage) {

    ResetTTreeVars();
    
    auto ev_tru = storage->get_data<event_mctruth >("generator");
    auto ev_mcs = storage->get_data<event_mcshower>(  "mcreco" );
    auto ev_mct = storage->get_data<event_mctrack >(  "mcreco" );
    
    if( !ev_tru || !ev_mcs || !ev_mct ){
      std::cout<<"wtf"<<std::endl;
      return false;
    }
    
    
    //////////////////////////////////////
    ///Stuff we can only get from mctruth
    //////////////////////////////////////
    
    auto mcnu_traj = ev_tru->at(0).GetNeutrino().Nu().Trajectory();
    auto event_vtx = mcnu_traj.at(0);
    
    true_nu_E = 1000.*mcnu_traj.at(0).E();
    nu_pdg = ev_tru->at(0).GetNeutrino().Nu().PdgCode();
    nuance_intxn = ev_tru->at(0).GetNeutrino().InteractionType();
    //CCNC returns 0 if CC, 1 if NC
    isCC = ev_tru->at(0).GetNeutrino().CCNC() == 0 ? true : false;
    _mode = ev_tru->at(0).GetNeutrino().Mode();
    
    
    n_lep = 0;
    //these come from perfect reconstruction
    std::vector<float> primary_prot_energies;
    primary_prot_energies.clear();
    std::vector<float> secondary_prot_energies;
    secondary_prot_energies.clear();

    //these come from mctruth
    std::vector<float> neut_energies;
    neut_energies.clear();
    
    auto mcpart_list = ev_tru->at(0).GetParticles();
    n_protons_mctruth = 0;
    for (auto const mcp : mcpart_list){
      
      if (mcp.StatusCode() == 1){
	
	double part_KE_MEV = 1000.*(mcp.Trajectory().at(0).E()-mcp.Trajectory().at(0).Momentum().M());
	if(abs(mcp.PdgCode()) == 2112 && part_KE_MEV > _min_n_E_MEV) 
	  neut_energies.push_back(part_KE_MEV);
	
	if(abs(mcp.PdgCode()) == 2212 && part_KE_MEV > _min_p_E_MEV){
	  n_protons_mctruth++;
	}
      }
    }
    
    //////////////////////////////////////
    ///END stuff we can only get from mctruth
    //////////////////////////////////////



    TLorentzVector total_momentum(0.,0.,0.,0.);

    //Loop over mctracks
    avg_dist_to_secondary_protons=0;
    for (auto const& mct : *ev_mct){

      double KE_MEV = mct.Start().E() - mct.Start().Momentum().M();
      //Protons
      // I'm finding a lot of mctrack protons that don't match up with mctruth protons
      // I've realized it is because I'll occasionally find another proton in the event
      // (sometimes with quite high energy) that is several cm away from the neutrino
      // interaction point. These protons are generally made by "NeutronInelastic"
      // or "ProtonInelastic" processes.
      // We should be able to distinguish them as separate from the vertex, in the data.
      // Protons from the vertex have process == "primary".

      if(abs(mct.PdgCode()) == 2212 && KE_MEV > _min_p_E_MEV){
	if(mct.Process() == "primary"){
	  total_momentum += mct.Start().Momentum();
	  primary_prot_energies.push_back(KE_MEV);
	}
	else{
	  float dist = pow(pow(mct.Start().X()-event_vtx.X(),2) + pow(mct.Start().Y()-event_vtx.Y(),2) + pow(mct.Start().Z()-event_vtx.Z(),2),0.5);
	  avg_dist_to_secondary_protons += dist; 
	  secondary_prot_energies.push_back(KE_MEV);
	}
      }

      else {
	// sometimes there are protons with small kinetic energy here
	if(abs(mct.PdgCode()) == 2212) continue;
	
	//	if (abs(mct.PdgCode()) == 13)
	//	  std::cout<<"wtf why is there a muon in this event? it probably had energy below whatever cutoff you used in your filter."<<std::endl;
	//	else
	//	  std::cout<<"uhhh why is there a mctrack with pdg "<<mct.PdgCode()<<" in this event?? it probably had energy below whatever cutoff you used in your filter"<<std::endl;
      }
    }

    //Loop over mcshowers
    for (auto const& mcs : *ev_mcs){
      //Electrons
      if(abs(mcs.PdgCode()) == 11){
	lep_E = mcs.Start().E();
	n_lep++;
	total_momentum += mcs.Start().Momentum();
      }
      //      else
      //	std::cout<<"wtf why is there a mcshower with pdg "<<mcs.PdgCode()<<" in this event?! It probably had energy below cutoff in whatever filter you used."<<std::endl;

    }
    
    //this equals x component^2 + y component ^2, sqrt
    tot_pt = total_momentum.Vect().Perp();

    n_primary_protons = primary_prot_energies.size();
    n_secondary_protons = secondary_prot_energies.size();
    n_neutrons = neut_energies.size();
    avg_dist_to_secondary_protons /= n_secondary_protons;

    //sort energies vector from lowest to highest
    std::sort(primary_prot_energies.begin(),primary_prot_energies.end());
    std::sort(secondary_prot_energies.begin(),secondary_prot_energies.end());
    std::sort(neut_energies.begin(),neut_energies.end());

    tot_primaryprot_KE = 0.;
    tot_secondaryprot_KE = 0.;
    tot_neutron_KE = 0.;

    for (double E : primary_prot_energies)
      tot_primaryprot_KE += E;

    for (double E : secondary_prot_energies)
      tot_secondaryprot_KE += E;

    for (double E : neut_energies)
      tot_neutron_KE += E;

    tree->Fill();
  
    return true;
  }

  bool NuEnergyReco_perfectreco::finalize() {


    if(_fout) { _fout->cd(); tree->Write(); }
  
    return true;
  }

  void NuEnergyReco_perfectreco::ResetTTreeVars(){
    true_nu_E = 0;
    nu_pdg = -1;    
    nuance_intxn = -1;
    isCC = false;
    _mode = -1;
    lep_E = 0;
    tot_pt = -1.;
  }

}
#endif
