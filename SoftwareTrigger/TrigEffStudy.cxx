#ifndef LARLITE_TRIGEFFSTUDY_CXX
#define LARLITE_TRIGEFFSTUDY_CXX

#include "TrigEffStudy.h"
#include "DataFormat/opdetwaveform.h"
#include "DataFormat/ophit.h"
#include "DataFormat/simphotons.h"
#include "DataFormat/mctruth.h"
#include "DataFormat/trigger.h"

////////////////////////////////////////////////////////////////////////////////////////
// Please, allow me to summarize all of the different timings that are going on.
//
// 0) All time begins at the time of RUN START.
//
// 1) If you have an opdetwaveform and you ask for its TimeStamp(), that is
// the number of microseconds between the RUN START and the beginning of that waveform.
//
// 2) If you have a trigger data product and you ask for its TriggerTime(), that is
// the number of microseconds between the RUN START and the time of that trigger firing.
//
// 3) If you have an ophit and you ask for its PeakTime(), that is the number of
// microseconds between the trigger data product TriggerTime() and the peak of that ophit.
//
// 4) If you ask Taritree's FEM trigger emulator for a fire_time, that is in
// number of SAMPLES (15.6ns each) between the TimeStamp() of the opdetwaveform
// and the emulated trigger firing (since the emulator takes as input opdetwaveforms)
////////////////////////////////////////////////////////////////////////////////////////

namespace larlite {

  bool TrigEffStudy::initialize() {

    try { constructLLInterface(); }
    catch (fememu::EmulatorException& e) {
      std::cout << e.what() << std::endl;
      return false;
    }

    initializeTTree();

    _use_mc = true;

    return true;
  }

  bool TrigEffStudy::analyze(storage_manager* storage) {

    ///Read in opdet waveform (handed to trigger decision making object)
    auto ev_odw = storage->get_data<event_opdetwaveform>("pmtreadout");
    if (!ev_odw) {
      print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, opdetwaveform!"));
      return false;
    }
    if (!ev_odw->size()) {
      print(larlite::msg::kERROR, __FUNCTION__, Form("opdetwaveform exists but has zero size!"));
      return false;
    }

    ///Read in mctruth (used to compute some ttree variables)
    auto ev_mct = storage->get_data<event_mctruth>("generator");
    if (!ev_mct) {
      print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, mctruth!"));
      return false;
    }
    if (ev_mct->size() != 1) {
      print(larlite::msg::kERROR, __FUNCTION__, Form("mctruth exists but has size != 1!"));
      return false;
    }

    //Read in trigger data product
    auto trig = storage->get_data<trigger>("triggersim");
    //us with respect to RUN START
    float trigger_time_us;
    if (!trig) {
      print(larlite::msg::kWARNING, __FUNCTION__, Form("Did not find specified data product, trigger! Using hard-coded 3650 us."));
      trigger_time_us = 3650.;
    }
    else trigger_time_us = trig->TriggerTime();

    //Reset ttree vars to initialization values
    resetTTreeVars();

    //Run the trigger emulation and store the result
    _myoutput = _my_LLint.Emulate(*ev_odw);

    //Sanity check on the result of trigger emulation
    if (!_myoutput.fire_time_v.size()) {
      std::cout << "wtf fire_time_v has zero size? skipping this event." << std::endl;
      return false;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////
    //// Save relevant portions of the result to the ttree variables
    //////////////////////////////////////////////////////////////////////////////////////////////////

    // To count number of triggers, loop over fire_time_v and look for entries != -1
    n_trigs = 0;
    for (unsigned int iwindow = 0; iwindow < _myoutput.fire_time_v.size(); iwindow++)
      n_trigs += _myoutput.fire_time_v.at(iwindow) == -1 ? 0 : 1;

    if (n_trigs > 1)
      std::cout << "WARNING LOOKING ONLY @ FIRST TRIGGER! THERE WERE >1 TRIGGERS!" << std::endl;

    //us with respect to RUN START
    float emulated_trigger_time_us = _myoutput.fire_time_v.at(0) != -1 ?
                                     _myoutput.fire_time_v.at(0) * 0.0156 + ev_odw->at(0).TimeStamp() :
                                     -9e9;
    //TTree variable is the trigger time difference
    em_trig_minus_dp_trig_us = emulated_trigger_time_us - trigger_time_us;

    ///Find the x-position of the first particle
    //For neutrino files, neutrino has        mother -1, status 0 (almost always), trackID 0
    //For single electron files, electron has mother -1, status 1,                 trackID -1
    for (auto const &part : ev_mct->front().GetParticles()) {
      if (part.Mother() == -1 && part.TrackId() <= 0) {
        x_pos = part.Trajectory().front().X();
        pdg = part.PdgCode();
        energy = part.Trajectory().front().E();
        break;
      }
      print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find relevant particle in mctruth. Can't save x-position!"));
      return false;
    }

    /// Now we compute the "reconstructed PE", in two different possible ways, depending on how
    //_use_mc is toggled.

    // If !_use_mc,
    //Loop over ophit and sum the total reconstructed PE within specified time window to save to the ttree
    if (!_use_mc) {

      ///Read in ophit (used to compute some TTree variables)
      auto ev_oph = storage->get_data<event_ophit>("opflash");
      if (!ev_oph) {
        print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, ophit!"));
        return false;
      }
      if (!ev_oph->size()) {
        print(larlite::msg::kERROR, __FUNCTION__, Form("ophit exists but has zero size!"));
        return false;
      }

      n_reco_PE = 0;
      for (auto const& oph : *ev_oph) {
        // Don't count PMTs numbered greater than 31
        if (oph.OpChannel() > 31) continue;
        // Don't sum PEs from ophits that occur outside of specified time window around trigger data product fire time
        // Note ophit time == 0 is the trigger data TriggerTime()
        if (oph.PeakTime() < _window_us_before_trig || oph.PeakTime() > _window_us_after_trig )
          continue;

        n_reco_PE +=  oph.Amplitude() / 20.; //oph.PE();

      }//End loop over ophits
    }





    // If _use_mc,
    //Loop over simphotons and sum the total PE in the entire event
    else {
      ///Read in simphotons (used as an alternative to compute some TTree variables)
      auto ev_simph = storage->get_data<event_simphotons>("largeant");
      if (!ev_simph) {
        print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product simphotons!"));
        return false;
      }
      if (ev_simph->empty()) {
        print(larlite::msg::kERROR, __FUNCTION__, Form("simphotons exist but has zero size!"));
        return false;
      }

      //Reset ttree vars to initialization values
      n_reco_PE = 0;
      for (auto const& simph_v : *ev_simph) {
        if (simph_v.OpChannel() > 31) continue;
        n_reco_PE += simph_v.size();

      }
    }

    //Fill the ttree
    _ana_tree->Fill();

    return true;





    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Here are code snippets Kazu wrote that I feel like might be useful in the future, so I leave it here commented
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Figure out MC interaction time
    //double interaction_g4time = 0;
    //interaction_g4time = (*ev_mct).front().GetParticles().front().Trajectory().front().T() / 1.e3;
    // Figure out beam-window start time
    //double beam_window_start_g4time = 0;
    //for (auto const& wf : *ev_odw)
    //  if (wf.size() > 1000) { beam_window_start_g4time = wf.TimeStamp() - 3650.; break;}
    //std::cout<<"trig_time = "<<trig_time<<" ... G4 time "<<beam_window_start + trig_time * 0.0156 << std::endl;
    //auto const ev_mct = storage->get_data<event_mctruth>("generator");
    //std::cout<<beam_window_start + trig_time * 0.0156 - 3650. - (*ev_mct).front().GetParticles().front().Trajectory().front().T()/1.e3<<std::endl;
    /*
    for(auto const& simph : simph_v) {
      if(simph.Time/1000. < (beam_window_start_g4time + trig_time * 0.0156 + _window_min_us) ||
         simph.Time/1000. > (beam_window_start_g4time + trig_time * 0.0156 + _window_us_after_trig) )
        continue;
      n_reco_PE++;
    }
    */
  }

  bool TrigEffStudy::finalize() {

    if (_fout) {
      if (_ana_tree) {
        _fout->cd();
        _ana_tree->Write();
      }
    }

    return true;
  }

  void TrigEffStudy::constructLLInterface() {

    if (!_myconfig)
      throw fememu::EmulatorException("Config not set to configure LLInterface instance from within TrigEffStudy!");
    _my_LLint = fememu::LLInterface(*_myconfig);

  }

  void TrigEffStudy::resetTTreeVars() {
    n_trigs = -1;
    n_reco_PE = -1;
    x_pos = -9.e9;
    pdg = -999999;
    energy = -9.e9;
    em_trig_minus_dp_trig_us = -9.e9;
  }

  void TrigEffStudy::initializeTTree() {
    if (!_ana_tree) {
      _ana_tree = new TTree("ana_tree", "ana_tree");
      _ana_tree->Branch("n_trigs", &n_trigs, "n_trigs/I");
      _ana_tree->Branch("n_reco_PE", &n_reco_PE, "n_reco_PE/I");
      _ana_tree->Branch("x_pos", &x_pos, "x_pos/F");
      _ana_tree->Branch("pdg", &pdg, "pdg/I");
      _ana_tree->Branch("energy", &energy, "energy/F");
      _ana_tree->Branch("em_trig_minus_dp_trig_us", &em_trig_minus_dp_trig_us, "em_trig_minus_dp_trig_us/F");
    }

  }

}
#endif
