#ifndef LARLITE_TRIGEFFSTUDY_CXX
#define LARLITE_TRIGEFFSTUDY_CXX

#include "TrigEffStudy.h"
#include "DataFormat/opdetwaveform.h"
#include "DataFormat/ophit.h"
#include "DataFormat/simphotons.h"
#include "DataFormat/mctruth.h"
#include "DataFormat/trigger.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/mcshower.h"

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
//
// 5) Note that in simulation, the trigger data product TriggerTime() is usually 3650 us.
// However, the actual particle interaction time is NOT exactly 3650, the particle can
// arrive any time within a 1.6us window after the trigger time. To get the actual particle
// arrival time, you do mcparticle.Trajectory().front().T(), which is a time in
// NANOSECONDS which is the number of ns after the trigger data product TriggerTime()
//
////////////////////////////////////////////////////////////////////////////////////////

namespace larlite {

  bool TrigEffStudy::initialize() {

    try { constructLLInterface(); }
    catch (fememu::EmulatorException& e) {
      std::cout << e.what() << std::endl;
      return false;
    }
    if (!_use_mc)
      // _debughist = new TH1F("debughist", "oph.PeakTime() - truth_part_arrival_time + trigger_time_us", 1000, -.5, 10);
      _debughist = new TH1F("debughist", "oph.PeakTime()", 1000, -.5, 10);
    else
      _debughist = new TH1F("debughist", "simphoton.Time/1000. - truth_part_arrival_time + 3650.", 1000, -.2, 2);

    initializeTTree();

    std::cout << "_window_us_after_truth_part_time = " << _window_us_after_truth_part_time << std::endl;
    std::cout << "_window_us_before_truth_part_time = " << _window_us_before_truth_part_time << std::endl;

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


    ///Read in mctrack (used to compute some ttree variables)
    auto ev_mctrack = storage->get_data<event_mctrack>("mcreco");

    ///Read in mcshower (used to compute some ttree variables)
    auto ev_mcshower = storage->get_data<event_mcshower>("mcreco");

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

    //Read in trigger data product
    auto trig = storage->get_data<trigger>("triggersim");
    //us with respect to RUN START
    float trigger_time_us;
    if (!trig) {
      //print(larlite::msg::kWARNING, __FUNCTION__, Form("Did not find specified data product, trigger! Using hard-coded 3650 us."));
      trigger_time_us = 3650.;
    }
    else trigger_time_us = trig->TriggerTime();

    //Reset ttree vars to initialization values
    resetTTreeVars();

    //compute nophit_over_nsimph
    float simch_sum = 0;
    for (int i = 0; i < ev_simph->size(); i++)
      simch_sum += ev_simph->at(i).size();
    float ophit_sum = 0;
    for (int i = 0; i < ev_oph->size(); i++) {
      auto oph = ev_oph->at(i);
      if (oph.OpChannel() > 31) continue;
      if (oph.PeakTime() < -_window_us_before_truth_part_time || oph.PeakTime() > _window_us_after_truth_part_time) continue;
      ophit_sum += oph.PE();
    }
    nophit_over_nsimph = simch_sum ? ophit_sum / simch_sum : -9999;

    //compute total energy deposited
    energy_deposited = 0.;
    if (ev_mcshower && ev_mctrack) {
      for (auto const& mcshower : *ev_mcshower)
        energy_deposited += mcshower.DetProfile().E();
      for (auto const& mctrack : *ev_mctrack)
        if (mctrack.size())
          energy_deposited += mctrack.front().E() - mctrack.back().E();
    }
    
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

    //Figure out the emulated trigger time in us with respect to RUN START
    //First find one of the opdetwaveforms that was used by the emulator to generate a trigger
    //(all wfs used by emulator are time-aligned, so just one will suffice)
    float odw_timestamp = -1;
    for (auto const &wf : *ev_odw)
      if ( wf.ChannelNumber() < 31 && wf.size() > 1000 ) {
        odw_timestamp = wf.TimeStamp();
        break;
      }

    if (odw_timestamp == -1) {
      print(larlite::msg::kERROR, __FUNCTION__, Form("Could not locate opdetwaveform used by trigger emulator!"));
      return false;
    }

    emulated_trigger_tick = _myoutput.fire_time_v.at(0) != -1 ?
                            _myoutput.fire_time_v.at(0) : -999999;

    float emulated_trigger_time_us = _myoutput.fire_time_v.at(0) != -1 ?
                                     _myoutput.fire_time_v.at(0) * 0.0156 + odw_timestamp :
                                     -9e9;

    ///Find the x-position of the first particle, and the arrival time of the first particle
    //For neutrino files, neutrino has        mother -1, status 0 (almost always), trackID 0
    //For single electron files, electron has mother -1, status 1,                 trackID -1
    float truth_part_arrival_time; // us with respect to RUN START
    for (auto const &part : ev_mct->front().GetParticles()) {
      if (part.Mother() == -1 && part.TrackId() <= 0) {
        x_pos = part.Trajectory().front().X();
        pdg = part.PdgCode();
        energy = part.Trajectory().front().E();
        truth_part_arrival_time = part.Trajectory().front().T() / 1000. + trigger_time_us;
        break;
      }
      print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find relevant particle in mctruth!"));
      return false;
    }


    //TTree variable is the trigger time difference
    em_trig_minus_truth_particle_time_ns = (emulated_trigger_time_us - truth_part_arrival_time) * 1000.;

    /// Now we compute the "reconstructed PE", in two different possible ways, depending on how
    //_use_mc is toggled.

    // If !_use_mc,
    //Loop over ophit and sum the total reconstructed PE within specified time window to save to the ttree
    if (!_use_mc) {



      n_reco_PE = 0;
      double seriouslyfuckyou = 0;
      size_t gofuckyourself = 0;
      for (auto const& oph : *ev_oph) {
        // Don't count PMTs numbered greater than 31
        if (oph.OpChannel() > 31) continue;

        ophit_peaktime = oph.PeakTime();
        ftruth_part_arrival_time = truth_part_arrival_time;
        ftrigger_time_us = trigger_time_us;
        femtrigger_time_us = emulated_trigger_time_us;
        ophit_tree->Fill();


        // Don't sum PEs from ophits that occur outside of specified time window around true particle arrival time
        // Note ophit time == 0 is the trigger data TriggerTime()
        // NOTE! for single particles, trigger data TriggerTime is always 3650.
        // This is NOT the case for neutrinos. If you have access to the trigger data product, use that value,
        // but if not, for neutrinos oph.PeakTime() is w.r.t. the particle interaction time,
        // so you can just use oph.PeakTime() below.

        // Use this if you have trigger data product, or you are using single particles
        // _debughist->Fill(oph.PeakTime() - truth_part_arrival_time + trigger_time_us);
        if (oph.PeakTime() - truth_part_arrival_time + trigger_time_us < -_window_us_before_truth_part_time ||
            oph.PeakTime() - truth_part_arrival_time + trigger_time_us > _window_us_after_truth_part_time )
          continue;

        //Use this for neutrino sample if you don't have trigger data product
        // _debughist->Fill(oph.PeakTime());
        // if (oph.PeakTime() < -_window_us_before_truth_part_time ||
        //     oph.PeakTime() > _window_us_after_truth_part_time )
        //   continue;
        // if (oph.PeakTime() < -_window_us_before_truth_part_time ||
        //     oph.PeakTime() > _window_us_after_truth_part_time )
        //   continue;

        n_reco_PE +=  oph.PE();//oph.Amplitude() / 20.; //oph.PE();

      }//End loop over ophits




    }





    // If _use_mc,
    //Loop over simphotons and sum the total PE in the entire event
    else {

      //Reset ttree vars to initialization values
      n_reco_PE = 0;
      for (auto const& simph_v : *ev_simph) {
        if (simph_v.OpChannel() > 31) continue;

        for (auto const& simph : simph_v) {

          _debughist->Fill(simph.Time / 1000. - truth_part_arrival_time + trigger_time_us);

          if (simph.Time / 1000. - truth_part_arrival_time + trigger_time_us < -_window_us_before_truth_part_time ||
              simph.Time / 1000. - truth_part_arrival_time + trigger_time_us > _window_us_after_truth_part_time )
            continue;

          n_reco_PE ++;
        }
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
      ophit_tree->Write();
      _debughist->Write();
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
    emulated_trigger_tick = -999999;
    n_reco_PE = -9.e9;
    x_pos = -9.e9;
    pdg = -999999;
    energy = -9.e9;
    energy_deposited = -9.e9;
    nophit_over_nsimph = -9.e9;
    em_trig_minus_truth_particle_time_ns = -9.e9;
  }

  void TrigEffStudy::initializeTTree() {
    if (!_ana_tree) {
      _ana_tree = new TTree("ana_tree", "ana_tree");
      _ana_tree->Branch("n_trigs", &n_trigs, "n_trigs/I");
      _ana_tree->Branch("emulated_trigger_tick", &emulated_trigger_tick, "emulated_trigger_tick/I");
      _ana_tree->Branch("n_reco_PE", &n_reco_PE, "n_reco_PE/F");
      _ana_tree->Branch("x_pos", &x_pos, "x_pos/F");
      _ana_tree->Branch("pdg", &pdg, "pdg/I");
      _ana_tree->Branch("energy", &energy, "energy/F");
      _ana_tree->Branch("em_trig_minus_truth_particle_time_ns", &em_trig_minus_truth_particle_time_ns, "em_trig_minus_truth_particle_time_ns/F");
      _ana_tree->Branch("nophit_over_nsimph", &nophit_over_nsimph, "nophit_over_nsimph/F");
      _ana_tree->Branch("energy_deposited", &energy_deposited, "energy_deposited/F");
    }

    if (!ophit_tree) {
      ophit_tree = new TTree("ophit_tree", "ophit_tree");
      ophit_tree->Branch("ophit_peaktime", &ophit_peaktime, "ophit_peaktime/F");
      ophit_tree->Branch("ftruth_part_arrival_time", &ftruth_part_arrival_time, "ftruth_part_arrival_time/F");
      ophit_tree->Branch("ftrigger_time_us", &ftrigger_time_us, "ftrigger_time_us/F");
      ophit_tree->Branch("femtrigger_time_us", &femtrigger_time_us, "femtrigger_time_us/F");
    }

  }

}
#endif
