#ifndef LARLITE_FINDEVENTBYRUNSUBRUNEVT_CXX
#define LARLITE_FINDEVENTBYRUNSUBRUNEVT_CXX

#include "FindEventByRunSubrunEvt.h"

namespace larlite {

    bool FindEventByRunSubrunEvt::initialize() {

        if (!runno || !subrunno || !evtno) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Didn't set run number, subrun number, or event number!"));
            return false;
        }

        return true;
    }

    bool FindEventByRunSubrunEvt::analyze(storage_manager* storage) {

        if ( storage->run_id() == runno && storage->subrun_id() == subrunno && storage->event_id() == evtno ) {
            std::cout << "w00t w00t found event with run " << runno
                      << ", subrun " << subrunno
                      << ", event " << evtno
                      << "! Saving to output file ... " << std::endl;
            return true;
        }
        else return false;

    }

    bool FindEventByRunSubrunEvt::finalize() {

        return true;
    }

}
#endif
