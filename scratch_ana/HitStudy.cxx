#ifndef LARLITE_HITSTUDY_CXX
#define LARLITE_HITSTUDY_CXX

#include "HitStudy.h"
#include "DataFormat/hit.h"

namespace larlite {

bool HitStudy::initialize() {

    _hit_wires = new TH1F("hit_wires", "Wire [cm] from each hit", 8000, 0, 1050);
    _hit_times = new TH1F("hit_times", "Time [cm] from each hit", 8000, 0, 1050);
    return true;
}

bool HitStudy::analyze(storage_manager* storage) {
    //Read in Hits
    auto ev_hit = storage->get_data<event_hit>("cchit");
    if (!ev_hit) {
        print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, hit!"));
        return false;
    }


    //Loop over hits
    for (auto &hit : *ev_hit) {
      //i personally only care about hits on the y plane
      if(hit.WireID().Plane != 2) continue;
      
        _hit_wires->Fill(hit.WireID().Wire * fWiretoCm);
        _hit_times->Fill(hit.PeakTime()    * fTimetoCm);
    }

    return true;

}

bool HitStudy::finalize() {

    if (_fout) { _fout->cd(); _hit_wires->Write(); _hit_times->Write(); }

    return true;
}

}
#endif
