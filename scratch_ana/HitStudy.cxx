#ifndef LARLITE_HITSTUDY_CXX
#define LARLITE_HITSTUDY_CXX

#include "HitStudy.h"
#include "DataFormat/hit.h"

namespace larlite {

bool HitStudy::initialize() {

    mygeo = larutil::Geometry::GetME();
    mygeoutil = larutil::GeometryUtilities::GetME();
    mydetprop = larutil::DetectorProperties::GetME();

    fTimetoCm = mygeoutil->TimeToCm();
    fWiretoCm = mygeoutil->WireToCm();
    fElectronsToADC = mydetprop->ElectronsToADC();

    _hit_wires.push_back(new TH1F("hit_wires_plane0", "Reconstructed CC-Hits on a given Wire Number (Plane 0);Wire Number;Number of Hits", 2390, -0.5, 2390.5));
    _hit_wires.push_back(new TH1F("hit_wires_plane1", "Reconstructed CC-Hits on a given Wire Number (Plane 1);Wire Number;Number of Hits", 2390, -0.5, 2390.5));
    _hit_wires.push_back(new TH1F("hit_wires_plane2", "Reconstructed CC-Hits on a given Wire Number (Plane 2);Wire Number;Number of Hits", 3456, -0.5, 3455.5));
//    _hit_times = new TH1F("hit_times", "Time [cm] from each hit", 8000, 0, 1050);

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
        if (hit.WireID().Plane > 2) {
            std::cout << "I don't even know anymore." << std::endl;
            continue;
        }

        _hit_wires.at(hit.WireID().Plane)->Fill(hit.WireID().Wire);// * fWiretoCm);
        // _hit_times->Fill(hit.PeakTime()    * fTimetoCm);
    }

    return true;

}

bool HitStudy::finalize() {

    if (_fout) { 
    _fout->cd(); 
    _hit_wires.at(0)->Write(); 
    _hit_wires.at(1)->Write(); 
    _hit_wires.at(2)->Write(); 

    }//_hit_times->Write(); }

    return true;
}

}
#endif
