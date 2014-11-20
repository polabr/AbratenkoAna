#ifndef TRACKENERGYPLOTS_CXX
#define TRACKENERGYPLOTS_CXX

#include "TrackEnergyPlots.h"

namespace larlite {

  bool TrackEnergyPlots::initialize() {
    
    _event_counter = 0;

    //Initialize _ana_tree
    if(!_ana_tree){
      _ana_tree = new TTree("ana_tree","");
      _ana_tree->Branch("ERecoTrack_max",&_E_recotrack_max,"ERecoTrack_max/D");
      _ana_tree->Branch("EMCTrack_max",&_E_mctrack_max,"EMCTrack_max/D");
      _ana_tree->Branch("ERecoTrack_sum",&_E_recotrack_sum,"ERecoTrack_sum/D");
      _ana_tree->Branch("EMCTrack_sum",&_E_mctrack_sum,"EMCTrack_sum/D");
    }

    return true;
  }
  
  bool TrackEnergyPlots::analyze(storage_manager* storage) {
  
    _event_counter++;

    //Grab the MCTracks
    auto ev_mctrack = storage->get_data<event_mctrack>("mcreco");    
    if(!ev_mctrack) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctrack!"));
      return false;
    }

    //Grab the RecoTracks calorimetry objects
    //beziertrackercalo
    //beziertrackercccalo
    //costrkcalo
    //costrkcccalo
    //stitchcalo
    //stitchcccalo
    //stitchkalmanhitcalo
    //trackkalmanhitcalo
    //trackkalmanhitcccalo
    //trackkalspscalo
    //trackkalspscccalo
    auto ev_trackcalo = storage->get_data<event_calorimetry>("stitchkalmanhitcalo");    
    if(!ev_trackcalo) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, track calorimetry!"));
      return false;
    }

    //Find the biggest kinetic energy MC track,
    //assume it matches with the biggest kinetic energy reco track calorimetry
    //From the MCTracksContained, we already know all tracks are fully contained


    //consider doing:
    //study how well energy reconstruction works vs. range (length)?




    double highest_track_MC_E = 0;
    double summed_track_MC_E = 0;
    size_t highest_MC_E_track_indx = 0;
    size_t mccounter = 0;
    
    for (auto &track : *ev_mctrack){      
      
      double track_kinetic_E = track.MotherStart().Momentum().E()-track.MotherStart().Momentum().M();
      
      if(track_kinetic_E > highest_track_MC_E){
	highest_track_MC_E = track_kinetic_E;
	highest_MC_E_track_indx = mccounter;
      }
      mccounter++;

      summed_track_MC_E += track_kinetic_E;
    }
    _E_mctrack_max = highest_track_MC_E;
    _E_mctrack_sum = summed_track_MC_E;

    //Now find biggest reco track calorimetry
    double highest_trackcalo_E = 0;
    double summed_trackcalo_E = 0;
    size_t highest_E_trackcalo_indx = 0;
    size_t calocounter = 0;
    for (auto &trackcalo : *ev_trackcalo){      

      double track_kinetic_E = trackcalo.KineticEnergy();
      
      if(track_kinetic_E > highest_trackcalo_E){
	highest_trackcalo_E = track_kinetic_E;
	highest_E_trackcalo_indx = calocounter;
      }

      calocounter++;
      if(track_kinetic_E>0)
	summed_trackcalo_E += track_kinetic_E;
    }
    _E_recotrack_max = highest_trackcalo_E;
    _E_recotrack_sum = summed_trackcalo_E;

    if (_ana_tree)
      _ana_tree->Fill();

    return true;
  }

  bool TrackEnergyPlots::finalize() {
  
    std::cout<<_event_counter<<" events counted!"<<std::endl;

    if(_fout) {
      _fout->cd();
      
      if(_ana_tree)
	_ana_tree->Write();
    }
    
    delete _ana_tree;
    
    return true;
  }

}
#endif
