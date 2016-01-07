#ifndef LARLITE_OPHITOFFSETSTUDY_CXX
#define LARLITE_OPHITOFFSETSTUDY_CXX

#include "OpHitOffsetStudy.h"
#include "DataFormat/mctruth.h"
#include "DataFormat/ophit.h"

namespace larlite {

    bool OpHitOffsetStudy::initialize() {
        if (!hist)
            hist = new TH1F("offsethist", "oph.PeakTime() - truth_part_arrival_time;Microseconds;# of ophits", 1000, -1, 7);
        return true;
    }

    bool OpHitOffsetStudy::analyze(storage_manager* storage) {
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


        float truth_part_arrival_time; // us with respect to simulated trigger
        for (auto const &part : ev_mct->front().GetParticles()) {
            if (part.Mother() == -1 && part.TrackId() <= 0) {
                truth_part_arrival_time = part.Trajectory().front().T() / 1000.;
                break;
            }
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find relevant particle in mctruth!"));
            return false;
        }

        for (auto const& oph : *ev_oph) {
            // Don't count PMTs numbered greater than 31
            if (oph.OpChannel() > 31) continue;
            hist->Fill(oph.PeakTime() - truth_part_arrival_time);
        }//End loop over ophits

        return true;
    }

    bool OpHitOffsetStudy::finalize() {

        if (_fout) {

            _fout->cd();
            hist->Write();

        }

        return true;
    }

}
#endif
