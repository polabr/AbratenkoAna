#ifndef LARLITE_TRIGEFFSTUDY_CXX
#define LARLITE_TRIGEFFSTUDY_CXX

#include "TrigEffStudy.h"
#include "DataFormat/opdetwaveform.h"
#include "DataFormat/ophit.h"

namespace larlite {

    bool TrigEffStudy::initialize() {

        try { constructLLInterface(); }
        catch (fememu::EmulatorException& e) {
            std::cout << e.what() << std::endl;
            return false;
        }

        initializeTTree();

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

        ///Read in ophit (used to compute some TTree variables)
        auto ev_oph = storage->get_data<event_ophit>("ophitCFD"); 
        if (!ev_oph) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, ophit!"));
            return false;
        }
        if (!ev_oph->size()) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("ophit exists but has zero size!"));
            return false;
        }

        //Reset ttree vars to initialization values
        resetTTreeVars();

        //Run the trigger emulation and store the result
        _myoutput = _my_LLint.Emulate(*ev_odw);

        //Save relevant portions of the result to the ttree variables
        if(!_myoutput.fire_time_v.size()){
            std::cout<<"wtf fire_time_v has zero size? skipping this event."<<std::endl;
            return false;
        }

        // To count number of triggers, loop over fire_time_v and look for entries != -1
        n_trigs = 0;
        for (int iwindow = 0; iwindow < _myoutput.fire_time_v.size(); iwindow++)
            n_trigs += _myoutput.fire_time_v.at(iwindow) == -1 ? 0 : 1;

        if (n_trigs > 1)
            std::cout << "WARNING LOOKING ONLY @ FIRST TRIGGER! THERE WERE >1 TRIGGERS!" << std::endl;

        //Loop over ophit and sum the total reconstructed PE within specified time window to save to the ttree
        //Only do this if a trigger fires
        if (n_trigs) {
            
            float trig_time = _myoutput.fire_time_v.at(0);
            std::cout<<"trig_time = "<<trig_time<<std::endl;
            n_reco_PE = 0;
            for (auto const& oph : *ev_oph) {
                // Don't count PMTs numbered greater than 31
                if (oph.OpChannel() > 31) continue;
                // Don't sum PEs from ophits that occur outside of specified time window around trigger fire time
                if (oph.PeakTime() < _window_min_us || oph.PeakTime() > _window_max_us )
                    continue;
                n_reco_PE += oph.PE();
            }//End loop over ophits

        }//End if a trigger was fired

        //Fill the ttree
        _ana_tree->Fill();

        return true;
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
    }

    void TrigEffStudy::initializeTTree() {
        if (!_ana_tree) {
            _ana_tree = new TTree("ana_tree", "ana_tree");
            _ana_tree->Branch("n_trigs", &n_trigs, "n_trigs/I");
            _ana_tree->Branch("n_reco_PE", &n_reco_PE, "n_reco_PE/I");
        }

    }

}
#endif
