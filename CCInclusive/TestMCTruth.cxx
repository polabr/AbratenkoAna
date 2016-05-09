#ifndef LARLITE_TESTMCTRUTH_CXX
#define LARLITE_TESTMCTRUTH_CXX

#include "TestMCTruth.h"
#include "DataFormat/mctruth.h"
#include "TLorentzVector.h"

namespace larlite {

    bool TestMCTruth::initialize() {

        fidvol_dist = 10.;
        fidvol_dist_y = 20.;

        //Box here is TPC
        _fidvolBox.Min( 0 + fidvol_dist,
                        -(::larutil::Geometry::GetME()->DetHalfHeight()) + fidvol_dist_y,
                        0 + fidvol_dist);

        _fidvolBox.Max( 2 * (::larutil::Geometry::GetME()->DetHalfWidth()) - fidvol_dist,
                        ::larutil::Geometry::GetME()->DetHalfHeight() - fidvol_dist_y,
                        ::larutil::Geometry::GetME()->DetLength() - fidvol_dist);

        evt_counter = 0;
        kept_events = 0;

        _h_mup = new TH2D("h_mup", "Reco vs True Mu Momentum (Reco From Proton Momentum & angles)", 100, 0, 3, 100, 0, 3);
        _h_mup_diff = new TH1F("h_mup_diff", "Reco - True / True Mu Momentum [GeV]", 100, -1, 1);

        _tree = new TTree("tree", "tree");
        _tree->Branch("absphidiff_minuspi", &absphidiff_minuspi, "absphidiff_minuspi/D");
        _tree->Branch("reco_mu_p", &reco_mu_p, "reco_mu_p/D");
        _tree->Branch("true_mu_p", &true_mu_p, "true_mu_p/D");
        _tree->Branch("premnant", &premnant, "premnant/D");
        _tree->Branch("ptremnant", &ptremnant, "ptremnant/D");
        _tree->Branch("plane_Z_anglediff", &plane_Z_anglediff, "plane_Z_anglediff/D");
_tree->Branch("true_nu_E", &true_nu_E, "true_nu_E/D");  
        return true;
    }

    bool TestMCTruth::analyze(storage_manager* storage) {
        absphidiff_minuspi = -999.;
        reco_mu_p = -999.;
        true_mu_p = -999.;
        premnant = -999.;
        ptremnant = -999.;
        plane_Z_anglediff = -999.;
        true_nu_E = -999.;

        //std::cout << " --- New event --- " << std::endl;
        evt_counter++;
        auto ev_mctruth = storage->get_data<event_mctruth>("generator");
        if (!ev_mctruth) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, mctruth!"));
            return false;
        }
        if (ev_mctruth->size() != 1) {
            // print(larlite::msg::kERROR, __FUNCTION__, Form("MCTruth size is not equal to 1... it equals %lu!", ev_mctruth->size()));
            return false;
        }


        /// First select a good event (this is basically a filter)

        // Require the neutrino interaction is inside the fiducial volume
        // if (!_fidvolBox.Contain(ev_mctruth->at(0).GetNeutrino().Nu().Trajectory().front().Position()))
        //     return false;

        // Require two particles in the final state
        TLorentzVector p_momentum;
        TLorentzVector mu_momentum;

        size_t n_muons = 0;
        size_t n_protons = 0;
        TVector3 zdir(0, 0, 1.);
        for (auto const& particle : ev_mctruth->at(0).GetParticles()) {

            if ( particle.StatusCode() == 15 ){
                premnant = particle.Trajectory().at(0).Momentum().Vect().Mag();
                ptremnant = premnant * sin(particle.Trajectory().at(0).Momentum().Vect().Angle(zdir));
            }
            // Only particles with status code 1 are relevant
            if ( particle.StatusCode() != 1) continue;

            // //Note: this KE is in units of GEV!
            double KE = particle.Trajectory().at(0).E() - particle.Mass();

            // //Don't care about any particles with less than 20 MeV KE
            // if ( KE < 0.02 ) continue;

            //Count up the number of muons
            if ( particle.PdgCode() == 13 ) {
                mu_momentum = particle.Trajectory().front().Momentum();
                n_muons++;
            }

            //Count up the number of protons
            if ( particle.PdgCode() == 2212 ) {
                //only consider protons above 100 MeV
                // if ( KE < 0.100 ) continue;
                p_momentum = particle.Trajectory().front().Momentum();
                n_protons++;
            }

            //Skip this event if any other particles exist... this filter
            //is to pick out 1mu1pNn events!
            // 22  => gammas
            // 211 => charged pions
            // 111 => pi0s
            // 11  => electrons
            // 321 => kaons
            if ( abs(particle.PdgCode()) == 22  ||
                    abs(particle.PdgCode()) == 211 ||
                    abs(particle.PdgCode()) == 111 ||
                    abs(particle.PdgCode()) == 11  ||
                    abs(particle.PdgCode()) == 321 ||
                    abs(particle.PdgCode()) == 2112 ||
                    //abs(particle.PdgCode()) == 2000000101 || //kbindino
                    abs(particle.PdgCode()) == 14 ||
                    abs(particle.PdgCode()) == 12 ||
                    particle.PdgCode() == -13 ||
                    //particle.PdgCode() == 1000180400 || //argon 40
                    abs(particle.PdgCode()) == 311 ||
                    abs(particle.PdgCode()) == 3222 || //strange baryons
                    abs(particle.PdgCode()) == 3122 ) //lambda baryon
                return false;
            // else {
            //     if (particle.PdgCode() != 13 && particle.PdgCode() != 2212)
            //         std::cout << "Found particle with PDG code " << particle.PdgCode() << std::endl;
            // }

        }

        //Skip this event if the number of muons is not 1 or # protons is not 1
        // std::cout<<"n_muons is "<<n_muons<<" and n_protons is "<<n_protons<<std::endl;
        if ( n_muons != 1 || n_protons != 1 )
            return false;

        kept_events++;
        /// OK now we have a good event
        // std::cout << "Found a good event!" << std::endl;
        // std::cout << "evt_counter is " << evt_counter << std::endl;
        true_nu_E = ev_mctruth->at(0).GetNeutrino().Nu().Trajectory().front().E();
        double mumass_MEV = 105.6583715;
        double pmass_MEV = 938.;
        // TVector3 zdir(0, 0, 1.);
        TVector3 muvec = mu_momentum.Vect();
        TVector3 pvec = p_momentum.Vect();
        double thetamu = muvec.Angle(zdir);
        double thetap  = pvec.Angle(zdir);
        // std::cout<<"thetamu is "<<thetamu<<", thetap is "<<thetap<<std::endl;
        // std::cout<<"proton TOTAL ENERGY is pvec mag squared plus proton mass squared sqrt is "
        // <<std::pow(std::pow(pvec.Mag(),2) + std::pow(pmass_MEV/1000.,2),0.5)<<std::endl;
        // std::cout<<"p energy from TLorentzVector is "<<p_momentum.E()<<std::endl;
        reco_mu_p = pvec.Mag() * sin(thetap) / sin(thetamu);
        true_mu_p = muvec.Mag();
        _reco_mu_E = pow(pow(reco_mu_p, 2) + pow(mumass_MEV / 1000., 2), 0.5);
        absphidiff_minuspi = std::abs(::geoalgo::Vector(pvec).Phi() - ::geoalgo::Vector(muvec).Phi())-3.14159265;
        // std::cout<<"Reco mu E is "<<_reco_mu_E<<", true mu E is "<<mu_momentum.E()<<std::endl;
        // std::cout << "Reco mu p is " << reco_mu_p << ", true mu p is " << true_mu_p << std::endl;
        _h_mup->Fill(reco_mu_p, muvec.Mag());
        _h_mup_diff->Fill((reco_mu_p - muvec.Mag()) / muvec.Mag());

        TVector3 cross = muvec.Cross(pvec);

        plane_Z_anglediff = cross.Angle(zdir);

        // TVector3 totalf = muvec + pvec;
        // TVector3 totali = ev_mctruth->at(0).GetNeutrino().Nu().Trajectory().front().Momentum().Vect();
        // std::cout<<"Final momentum (mu + p) minus initial (nu) is :"<<std::endl;
        // (totalf-totali).Print();
        // std::cout << " -- Muon angle is " << thetamu << std::endl;
        // std::cout << "True muon TOTAL ENERGY is " << mu_momentum.E() << std::endl;
        // std::cout << "Muon momentum magnitude is " << muvec.Mag() << std::endl;
        // std::cout << "MUON transverse momentum component is " << muvec.Mag()*sin(thetamu) << std::endl;
        // std::cout << "Parallel component is " << muvec.Mag()*cos(thetamu) << std::endl;
        //std::cout << "(total energy) magnitude squared plus mass square sqrt is "
        //           << std::pow(std::pow(muvec.Mag(), 2) + std::pow(mumass_MEV / 1000., 2), 0.5) << std::endl;
        // std::cout << "PROTON transverse momentum component is " << pvec.Mag()*sin(thetap) << std::endl;
        // printTruthStuff(*ev_mctruth);
        _tree->Fill();
        return true;
    }

    void TestMCTruth::printTruthStuff(const event_mctruth &mctruth) {
        // std::cout << "TRUE NEUTRINO ENERGY IS "
        //           << mctruth.at(0).GetNeutrino().Nu().Trajectory().front().E() << " GEV." << std::endl;
        std::cout << "----- LISTING ALL MCTRUTH PARTICLES IN EVENT, Skipping status != 0 or 1 or 15:" << std::endl;
        TVector3 numom(0., 0., 0.);
        TVector3 daughtmoms(0., 0., 0.);
        for (auto const& particle : mctruth.at(0).GetParticles()) {

            // Only particles with status code 1 are relevant
            if ( particle.StatusCode() != 1 && particle.StatusCode() != 0 && particle.StatusCode() != 15    ) continue;

            // // //Note: this KE is in units of GEV!
            // double KE = particle.Trajectory().at(0).E() - particle.Mass();

            // std::cout << " -- Particle with Status " << particle.StatusCode()
            //           << " and PDG " << particle.PdgCode()
            //           << " has TOTAL ENERGY " << particle.Trajectory().at(0).E() << " GEV."
            //           << " and MASS " << particle.Mass() << std::endl;
            // std::cout << " This particle's 3-Momentum magnitude is "
            // << particle.Trajectory().at(0).Momentum().Vect().Mag() << std::endl;

            if (particle.StatusCode() == 0 && particle.PdgCode() == 14)
                numom = particle.Trajectory().at(0).Momentum().Vect();
            if (particle.StatusCode() == 1 || particle.StatusCode() == 15)
                daughtmoms += particle.Trajectory().at(0).Momentum().Vect();
            // particle.Trajectory().at(0).Momentum().Vect().Print();
            // if(particle.StatusCode() == 15){
            //   std::cout<<"Particle with status code 15 has this momentum:"<<std::endl;
            //   particle.Trajectory().at(0).Momentum().Vect().Print();
            // }
        }
        std::cout << "Summary: neutrino momentum:" << std::endl;
        numom.Print();
        std::cout << "sum of status 1 and 15 momenta:" << std::endl;
        daughtmoms.Print();
    }

    bool TestMCTruth::finalize() {

        std::cout << "End of TestMCTruth. Total events is "
                  << evt_counter << " while # of events with 1 mu and 1 p is "
                  << kept_events << std::endl;

        if (_fout) {
            _fout->cd();
            _h_mup->Write();
            _h_mup_diff->Write();
            _tree->Write();
        }
        return true;
    }

}
#endif
