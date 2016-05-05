#ifndef LARLITE_XIAOEVENTSELECTOR_CXX
#define LARLITE_XIAOEVENTSELECTOR_CXX

#include "XiaoEventSelector.h"
#include "DataFormat/opflash.h"
#include "DataFormat/mctruth.h"


namespace larlite {

    bool XiaoEventSelector::initialize() {

        total_events = 0;
        passed_events = 0;

        fidvol_dist = 10.;
        fidvol_dist_y = 20.;

        //Box here is TPC
        _fidvolBox.Min( 0 + fidvol_dist,
                        -(::larutil::Geometry::GetME()->DetHalfHeight()) + fidvol_dist_y,
                        0 + fidvol_dist);

        _fidvolBox.Max( 2 * (::larutil::Geometry::GetME()->DetHalfWidth()) - fidvol_dist,
                        ::larutil::Geometry::GetME()->DetHalfHeight() - fidvol_dist_y,
                        ::larutil::Geometry::GetME()->DetLength() - fidvol_dist);

        _hmult = new TH1F("hmult", "Track Multiplicity", 10, -0.5, 9.5);
        _hdedx = new TH2D("hdedx", "End dEdx vs Start dEdx;End dEdx;Start dEdx", 50, 0, 20, 50, 0, 20);
        _hcorrect_ID = new TH1F("hcorrectID", "Was Neutrino Vtx Correctly Identified?", 2, -0.5, 1.5);


        if (!_tree) {
            _tree = new TTree("tree", "tree");
            _tree->Branch("true_nu_pdg", &_true_nu_pdg, "true_nu_pdg/I");
            _tree->Branch("true_nu_E", &_true_nu_E, "true_nu_E/D");
            _tree->Branch("mu_contained", &_mu_contained, "mu_contained/O");
            _tree->Branch("p_phi", &_p_phi, "p_phi/D");
            _tree->Branch("mu_phi", &_mu_phi, "mu_phi/D");
            _tree->Branch("correct_ID", &_correct_ID, "correct_ID/O");
            _tree->Branch("mu_end_dedx", &_mu_end_dedx, "mu_end_dedx/D");
            _tree->Branch("mu_start_dedx", &_mu_start_dedx, "mu_start_dedx/D");
        }
        return true;
    }

    void XiaoEventSelector::resetTTreeVars() {
        _mu_start_dedx = -999.;
        _mu_end_dedx = -999.;
        _correct_ID = false;
        _mu_phi = -999.;
        _p_phi = -999.;
        _mu_contained = false;
        _true_nu_E = -999.;
        _true_nu_pdg = -999;
    }

    bool XiaoEventSelector::analyze(storage_manager* storage) {

        total_events++;

        resetTTreeVars();

        auto ev_vtx = storage->get_data<event_vertex>("pmtrack");
        if (!ev_vtx) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, vertex!"));
            return false;
        }
        if (!ev_vtx->size()) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Zero reconstructed vertices in this event!"));
            return false;
        }

        auto ev_track = storage->get_data<event_track>("pandoraNuPMA");
        if (!ev_track) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, track!"));
            return false;
        }
        if (!ev_track->size()) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Zero reconstructed tracks in this event!"));
            return false;
        }
        auto ev_opflash = storage->get_data<event_opflash>("opflashSat");
        if (!ev_opflash) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, opflash!"));
            return false;
        }
        if (!ev_opflash->size()) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("opflash size is zero!"));
            return false;
        }
        event_calorimetry* ev_calo = nullptr;
        auto const& ass_calo_v = storage->find_one_ass(ev_track->id(),
                                 ev_calo,
                                 Form("%scalo", ev_track->name().c_str()));

        if (!ev_calo) {
            std::cerr << "no event_calorimetry!" << std::endl;
            return false;
        }

        if (ev_calo->empty()) {
            std::cout << "ev_calo empty" << std::endl;
            return false;
        }


        // Try to find a neutrino vertex in this event... return a reco vertex,
        // and a std::vector (length two) of tracks that are associated with that vertex
        std::pair<larlite::vertex, std::vector<larlite::track> > reco_neutrino;
        try {
            reco_neutrino = findNeutrino(ev_track, ev_calo, ass_calo_v, ev_vtx, ev_opflash);
        }
        catch (...) {
            return false;
        }

        if (reco_neutrino.second.size() != 2) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("uhhh why does reco neutrino ahve more than 2 tracks associated with it"));
            return false;
        }

        // Now that we found a neutrino interaction, let's pick which track is the muon and which is the proton
        // then store some ttree variables about each
        auto const &mutrack = reco_neutrino.second.at(0).Length() > reco_neutrino.second.at(1).Length() ?
                              reco_neutrino.second.at(0)          : reco_neutrino.second.at(1);
        auto const &ptrack  = reco_neutrino.second.at(0).Length() > reco_neutrino.second.at(1).Length() ?
                              reco_neutrino.second.at(1)          : reco_neutrino.second.at(0);

        _mu_contained = _fidvolBox.Contain(::geoalgo::Vector(mutrack.Vertex())) &&
                        _fidvolBox.Contain(::geoalgo::Vector(mutrack.End()));
        _mu_phi = ::geoalgo::Vector(mutrack.VertexDirection()).Phi();
        _p_phi  = ::geoalgo::Vector( ptrack.VertexDirection()).Phi();

        // If we found a vertex and we are running over MC, let's check if it is accurate
        if (!_running_on_data) {
            auto ev_mctruth = storage->get_data<event_mctruth>("generator");
            if (!ev_mctruth) {
                print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, mctruth!"));
                return false;
            }
            if (ev_mctruth->size() != 1) {
                print(larlite::msg::kERROR, __FUNCTION__, Form("MCTruth size doesn't equal one!"));
                return false;
            }
            // std::cout << "The reconstructed vertex is at : " << thevertexsphere.Center() << std::endl;
            // std::cout << "The true vertex is at : "
            //           <<::geoalgo::Vector(ev_mctruth->at(0).GetNeutrino().Nu().Trajectory().front().Position()) << std::endl;
            _true_nu_E = ev_mctruth->at(0).GetNeutrino().Nu().Trajectory().front().E();
            _true_nu_pdg = ev_mctruth->at(0).GetNeutrino().Nu().PdgCode();
            ::geoalgo::Sphere thevertexsphere(reco_neutrino.first.X(),
                                              reco_neutrino.first.Y(),
                                              reco_neutrino.first.Z(),
                                              3.0);
            _correct_ID = thevertexsphere.Contain(ev_mctruth->at(0).GetNeutrino().Nu().Trajectory().front().Position());
            _hcorrect_ID->Fill(_correct_ID);
        }

        _tree->Fill();
        passed_events++;
        return true;
    }


    double XiaoEventSelector::flashDistZ(const track & longest_track, const double flash_z) {

        // If flash is between start/end of track in Z (including flipping track direction)
        // return true

        /// I'm jsut going to copy christoph's exactly to try to duplicate
        // double FlashTrackDist(double flash, double start, double end) {
        double end = longest_track.End().Z();
        double start = longest_track.Vertex().Z();
        double flash = flash_z;
        if (end >= start) {
            if (flash < end && flash > start) return 0;
            else return TMath::Min(fabs(flash - start), fabs(flash - end));
        }
        else {
            if (flash > end && flash < start) return 0;
            else return TMath::Min(fabs(flash - start), fabs(flash - end));
        }
    }

    bool XiaoEventSelector::trackAssociatedWithVtx(const larlite::track &trk,
            const ::geoalgo::Sphere &vtx_sphere) {

        if (!trk.NumberTrajectoryPoints()) return false;

        ::geoalgo::Vector trkstart = ::geoalgo::Vector( trk.Vertex() );
        ::geoalgo::Vector trkend   = ::geoalgo::Vector( trk.End()    );

        // If this track doesn't start/end near the reco vertex, ignore it
        if ( !vtx_sphere.Contain(trkstart) && !vtx_sphere.Contain(trkend) )
            return false;

        return true;
    }

    bool XiaoEventSelector::isMichelMID(const ::geoalgo::Sphere &vtx_sphere,
                                        const std::vector<size_t> associated_track_idx_vec,
                                        const larlite::event_track * ev_track,
                                        const larlite::event_calorimetry * ev_calo,
                                        const larlite::AssSet_t & ass_calo_v) {

        if (associated_track_idx_vec.size() != 2) {
            std::string errMsg(">2 tracks asstd with vtx in Michel MID check");
            throw std::runtime_error(errMsg);
        }

        // Loop over the two tracks that are associated with the vertex
        // Figure out which is the long one, which is the short one
        // store their indices as "long_track_idx", "short_track_idx"
        auto const& trk0 = ev_track->at( associated_track_idx_vec.at(0) );
        auto const& trk1 = ev_track->at( associated_track_idx_vec.at(1) );
        size_t long_track_idx = 0;
        size_t short_track_idx = 0;
        if (trk0.Length() > trk1.Length()) {
            long_track_idx = associated_track_idx_vec.at(0);
            short_track_idx = associated_track_idx_vec.at(1);
        }
        else {
            long_track_idx = associated_track_idx_vec.at(1);
            short_track_idx = associated_track_idx_vec.at(0);
        }

        // idea: maybe cosmic MIDs are at gaps... try loweirng 3cm

        // Make a cut on dE/dx of longer track
        double vertex_dedx = 0.;
        double far_dedx = 0.;
        double far_max_y = -999.;
        calorimetry thecalo;
        auto const& long_trk = ev_track->at(long_track_idx);
        auto const& short_trk = ev_track->at(short_track_idx);

        // Choose the calo object for this track by the one
        // with the most number of hits in the dEdx vector
        // (this is how analysis tree does it)
        size_t tmp_nhits = 0;
        for (size_t i = 0; i < 3; ++i)
            if (ev_calo->at(ass_calo_v[long_track_idx][i]).dEdx().size() > tmp_nhits) {
                thecalo = ev_calo->at(ass_calo_v[long_track_idx][i]);
                tmp_nhits = thecalo.dEdx().size();
            }

        // now we have a calorimetry object for this track
        // compute the dedx of the track for the portion closest to the vertex
        // and the dedx for the portion far from the vertex
        ::geoalgo::Vector trkstart = ::geoalgo::Vector( long_trk.Vertex() );
        ::geoalgo::Vector trkend   = ::geoalgo::Vector( long_trk.End()    );


        // xiao says:
        // use first ten hits and last ten hits if track has more than 50 hits
        // otherwise use first 50% of hits and last 50% of hits
        double avg_front_dedx = 0.;
        double avg_back_dedx = 0.;
        if (tmp_nhits > 50) {
            for (size_t i = 0; i < 20; ++i) {
                avg_front_dedx += thecalo.dEdx()[i];
                avg_back_dedx += thecalo.dEdx()[tmp_nhits - 1 - i];
            }
            avg_front_dedx /= 20.;
            avg_back_dedx /= 20.;
        }
        else {
            size_t nhits_to_use = (size_t)tmp_nhits / 5.;
            for (size_t i = 0; i < nhits_to_use; ++i) {
                avg_front_dedx += thecalo.dEdx()[i];
                avg_back_dedx += thecalo.dEdx()[tmp_nhits - 1 - i];
            }
            avg_front_dedx /= (double)nhits_to_use;
            avg_back_dedx /= (double)nhits_to_use;
        }


        // If track is fully contained in the 3cm sphere then throw it out it
        if ( vtx_sphere.Contain(trkstart) && vtx_sphere.Contain(trkend) )
            return true;
        if (vtx_sphere.Contain(trkstart)) {
            vertex_dedx = avg_front_dedx;
            far_dedx    = avg_back_dedx;
            far_max_y   = trkend.at(1);
        }
        else if (vtx_sphere.Contain(trkend)) {
            vertex_dedx = avg_back_dedx;
            far_dedx    = avg_front_dedx;
            far_max_y   = trkstart.at(1);
        }

        _hdedx->Fill(vertex_dedx, far_dedx);

        // Final tagging (docdb 5724 slide 7):
        if ( ( (vertex_dedx > far_dedx && vertex_dedx > 2.5 && far_dedx < 4.) ||
                far_max_y > ::larutil::Geometry::GetME()->DetHalfHeight() - fidvol_dist_y ) &&
                short_trk.Length() < 30. )
            return true;

        _mu_start_dedx = vertex_dedx;
        _mu_end_dedx = far_dedx;

        return false;
    }

    ::geoalgo::Sphere XiaoEventSelector::getVertexSphere(const vertex & vtx) {

        ::geoalgo::Sphere vtx_sphere;
        ::geoalgo::Vector vertex = ::geoalgo::Vector(vtx.X(), vtx.Y(), vtx.Z());
        vtx_sphere.Radius(3.); //3cm sphere around each vertex
        vtx_sphere.Center(vertex);

        return vtx_sphere;

    }

    std::pair<larlite::vertex, std::vector<larlite::track> > XiaoEventSelector::findNeutrino(const event_track *ev_track,
            const event_calorimetry *ev_calo,
            const larlite::AssSet_t & ass_calo_v,
            const event_vertex *ev_vtx,
            const event_opflash *ev_opflash) {

        std::pair<larlite::vertex, std::vector<larlite::track> > result;

        // Loop over flashes, store the brightest flash in the BGW
        opflash theflash;
        bool _flash_in_bgw = false;
        for (auto const& flash : *ev_opflash) {

            if (flash.Time() > 3.55 && flash.Time() < 5.15 && flash.TotalPE() > 50.) {
                _flash_in_bgw = true;
                // Keep track of the brightest flash in the BGW
                if (flash.TotalPE() > theflash.TotalPE())
                    theflash = flash;
            }
        }
        // Require there is at least one >50 PE flash inside of BGW
        if (!_flash_in_bgw) throw std::exception();


        // Loop over vertices.
        // For each vertex in fiducial volume, loop over reco tracks
        // If you find a reco track starting or ending w/in 3cm from the vertex
        // Also keep track of multiplicities
        // Keep track of number of vertices that pass all cuts
        size_t n_viable_vertices = 0;
        // ::geoalgo::Sphere thevertexsphere(0, 0, 0, 3.);
        for (auto const& vtx : *ev_vtx) {

            // Make a 3cm geosphere around the vertex
            ::geoalgo::Sphere vtx_sphere = getVertexSphere(vtx);

            // Make sure vertex is in fiducial volume
            if (!_fidvolBox.Contain(vtx_sphere.Center())) continue;

            // Loop over tracks, store index of the ones associated with this vertex
            std::vector<size_t> associated_track_idx_vec;
            associated_track_idx_vec.clear();
            bool _at_least_one_track_matches_flash = false;

            for (size_t i = 0; i < ev_track->size(); ++i) {
                auto const trk = ev_track->at(i);

                if ( trackAssociatedWithVtx(trk, vtx_sphere) ) {
                    associated_track_idx_vec.push_back(i);

                    if ( flashDistZ(trk, theflash.ZCenter()) < 70. )
                        _at_least_one_track_matches_flash = true;
                }

            } //done looping over tracks

            // Require at least one vertex-associated track matches the BGW flash
            // (and, in there too is that there exists at least 1 such track)
            // KALEKO: requiring == 2 tracks coming from vertex to explore
            // possible CCQE phase space. This is different from Xiao!

            if (_at_least_one_track_matches_flash &&
                    associated_track_idx_vec.size() == 2) {

                // Now we have a vertex (vtx) that has exactly two tracks associated with it
                // Let's do some michel removal!
                if (!isMichelMID(vtx_sphere,
                                 associated_track_idx_vec,
                                 ev_track,
                                 ev_calo,
                                 ass_calo_v)) {
                    n_viable_vertices++;
                    result.first = vtx;
                    std::vector<track> temp;
                    temp.clear();
                    temp.push_back(ev_track->at(associated_track_idx_vec[0]));
                    temp.push_back(ev_track->at(associated_track_idx_vec[1]));
                    result.second = temp;
                }

            }// End mult == 2

            _hmult->Fill(associated_track_idx_vec.size());

        } //done looping over vertices

        // If this event doesn't contain any possible neutrino vertices that pass cuts
        // Or if it contains several, throw that out too! (KALEKO)
        if (n_viable_vertices != 1) throw std::exception();

        return result;
    }

    bool XiaoEventSelector::finalize() {

        std::cout << "EventSelector finalize! "
                  << total_events << " analyzed in total, "
                  << passed_events << " passed the filter." << std::endl;


        if (_fout) {
            _fout->cd();
            _hmult->Write();
            _hdedx->Write();
            _hcorrect_ID->Write();
            _tree->Write();
        }

        return true;
    }

}

#endif
