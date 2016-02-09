#ifndef LARLITE_TRACKRECOEMULATOR_CXX
#define LARLITE_TRACKRECOEMULATOR_CXX

#include "TrackRecoEmulator.h"
#include "DataFormat/mctrack.h"

namespace larlite {

    bool TrackRecoEmulator::initialize() {

        // _rand_seed = 123.456;
        // std::cout<<"RANDOM SEED USED IS "<<_rand_seed<<std::endl;
        return true;
    }

    bool TrackRecoEmulator::analyze(storage_manager* storage) {

        ///Read in mctracks
        auto ev_mct = storage->get_data<event_mctrack>("mcreco");
        if (!ev_mct) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, mctrack!"));
            return false;
        }
        if (!ev_mct->size()) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Zero mctracks in event!"));
            return false;
        }


        // For each MCTrack make a RecoEmulatorTrack struct
        std::vector<RecoEmulatorTrack_t> RET_v;
        RET_v.resize(ev_mct->size());
        // Fill each RecoEmulatorTrack struct with info from mctrack
        for ( size_t i = 0; i < ev_mct->size(); ++i) {

            auto const& mct = ev_mct->at(i);
            auto &RET = RET_v[i];

            // Loop over mctrack and fill RET trajectory
            RET._trajectory.reserve(mct.size());
            for ( size_t j = 0; j < mct.size(); ++j)
                RET._trajectory.push_back( ::geoalgo::Vector(mct[j].Position()) );

            RET._energy = mct.Start().E() - mct.End().E();

            RET._dedx = mct.dEdx();
        }


        std::cout << "Just made my RET_v! Length of RET_v is " << RET_v.size() << std::endl;
        // std::cout << "Just made my RET_v! Here's the info about each:" << std::endl;
        // for (auto const& RET : RET_v)
        //     printStructInfo(RET);

        // Apply smearing w/ a reference to the struct
        for ( size_t iret = 0; iret < RET_v.size(); ++iret) {
            auto &RET = RET_v[iret];

            // If the applySmearing returns false, that means "delete this track"
            // so we remove it (by index) from the std::vector
            if (!applySmearing(RET)) {
                RET_v.erase(RET_v.begin() + iret);
                iret--;
            }

        }

        /// Now we have a modified RET_v
        std::cout << "Finished modifying my RET_v! Length of RET_v is " << RET_v.size() << std::endl;
        // Here's the info about each:" << std::endl;
        // for (auto const& RET : RET_v)
        //     printStructInfo(RET);


        return true;
    }


    bool TrackRecoEmulator::applySmearing(RecoEmulatorTrack_t &RET) {


        /// If the track doesn't have a trajectory (doesn't go through TPC) delete it.
        if (!RET._trajectory.size()) return false;

        /// Delete the track if it is too short!
        double len = RET._trajectory.front().Dist(RET._trajectory.back());

        /// My shitty by-hand efficiency-as-a-function-of-length is:
        ///  .25           len is 0 to 5cm
        ///  .05*len       len is 5 to 15cm
        ///  .75           len is >15cm
        /// This absolutely needs to be changed and is only a place holder!
        double eff = 0.;
        if (len < 5.)
            eff = 0.25;
        else if (len >= 5. && len < 15.)
            eff = 0.05 * len;
        else
            eff = 0.75;

        /// Pick a random number 0 to 1. If the number is > the efficiency, delete the track.
        double rand_draw = rand() / (double)RAND_MAX;
        if ( rand_draw > eff ) {
            std::cout << "Deleting track! eff is " << eff << " and my random draw was " << rand_draw << std::endl;
            return false;
        }


        ///Now here I need to think about how to smear start point, start angle, flipping
        ///to "break tracks" into two, need to rethink how to do this at a higher level
        /// (acting on a reference to a struct won't work, need to divide the struct into two)
        return true;
    }

    void TrackRecoEmulator::printStructInfo(const RecoEmulatorTrack_t &ret) {
        std::cout << "Struct info: _energy = " << ret._energy << std::endl;
        if (ret._trajectory.size()) std::cout << "Trajectory exists! Start = " << ret._trajectory.at(0) << std::endl;
    }

    bool TrackRecoEmulator::finalize() {

        return true;
    }

}
#endif
