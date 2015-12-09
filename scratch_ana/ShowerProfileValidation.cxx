#ifndef LARLITE_SHOWERPROFILEVALIDATION_CXX
#define LARLITE_SHOWERPROFILEVALIDATION_CXX

#include "ShowerProfileValidation.h"
#include "DataFormat/mcshower.h"

namespace larlite {


    /// Default constructor
    ShowerProfileValidation::ShowerProfileValidation() : _fTPC(0, -116.5, 0, 256.35, 116.5, 1036.8)
    {
        _name = "ShowerProfileValidation";
        _fout = 0;
        ana_tree = 0;
        out_hist = 0;

    }


    bool ShowerProfileValidation::initialize() {

        if (!ana_tree) ana_tree = new TTree("ana_tree", "ana_tree");
        ana_tree->Branch("E_contained", &E_contained, "E_contained/D");
        ana_tree->Branch("prof_E_contained", &prof_E_contained, "prof_E_contained/D");

        if (!out_hist) 
            out_hist = new TH2F("profPerformanceHist",
                "EMShowerProfiler Energy Contained on Single Electrons;True Energy Fraction Contained;Profiler's Calculated Fraction Energy Contained",
                100,-0.05,1.05,100,-0.05,1.05);

        return true;
    }

    bool ShowerProfileValidation::analyze(storage_manager* storage) {

        E_contained = -999.;
        prof_E_contained = -999.;

        auto ev_mcs = storage->get_data<event_mcshower>("mcreco");
        if (!ev_mcs) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, MCShower!"));
            return false;
        }

        if (!ev_mcs->size()) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Zero MCShowers in event!"));
            return false;
        }

        if (ev_mcs->size() != 1) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("More than 1 MCShower in event!"));
            return false;
        }

        auto shr = ev_mcs->at(0);
        E_contained = shr.DetProfile().E() / shr.Start().E();
        try{
        auto xs_a = _geoAlgo.Intersection(_fTPC,
                                          ::geoalgo::HalfLine_t(
                                              shr.DetProfile().Position(),
                                              shr.DetProfile().Momentum()
                                          )
                                         );

        prof_E_contained = _emsProf.EnergyContainment( ::geoalgo::Vector(shr.Start().Position()).Dist(xs_a[0]) );

        if(xs_a.size() != 1)
            std::cout<<"hrmmmm two intersections with the AABox. figure out how to handle this"<<std::endl;
        else if (!xs_a.size())
            std::cout<<"no intersections with aabox!! dunno what to do"<<std::endl;

        // std::cout << "xs_a is : (";
        // for (size_t penis = 0; penis < xs_a.size(); penis++)
        //     std::cout << xs_a.at(penis) << ",";
        // std::cout << ")" << std::endl;

        ana_tree->Fill();
        out_hist->Fill(E_contained,prof_E_contained);
    }
    catch(const ::geoalgo::GeoAlgoException &e){
        std::cout<<e.what()<<std::endl;
    }
        return true;
    }

    bool ShowerProfileValidation::finalize() {

        if (_fout) { _fout->cd(); ana_tree->Write(); out_hist->Write(); }


        return true;
    }

}
#endif
