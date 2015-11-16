#ifndef PROTONSTUDY_CXX
#define PROTONSTUDY_CXX

#include "ProtonStudy.h"
#include "DataFormat/mctrack.h"
#include "GeoAlgo/GeoVector.h"

namespace larlite {

bool ProtonStudy::initialize() {
 
  if (!_ana_tree) {
    _ana_tree = new TTree("ana_tree", "ana_tree");
    _ana_tree->Branch("sum_trk_len", &_sum_trk_len, "sum_trk_len/D");
    _ana_tree->Branch("vect_trk_len", &_vect_trk_len, "vect_trk_len/D");
    _ana_tree->Branch("prot_KE", &_prot_KE, "prot_KE/D");
    _ana_tree->Branch("pdg", &_pdg, "pdg/I");
  }
 
  return true;
}

bool ProtonStudy::analyze(storage_manager* storage) {
 
  _sum_trk_len = 0;
  _vect_trk_len = 0;
  _prot_KE = 0;
  _pdg = 0;

  //Read in MCTracks
  auto ev_mct = storage->get_data<event_mctrack>(  "mcreco" );
  if (!ev_mct) {
    std::cout << "no mctracks" << std::endl;
    return false;
  }
 
  //Loop over MCTracks
  for (auto const& mct : *ev_mct) {
    //Avoid double counting
    if (mct.AncestorTrackID() != mct.TrackID()) continue;

    //      if(mct.PdgCode() != 2212) continue;
    if (!mct.size()) continue;

    //This energy includes mass. Not counting only deposited energy.
    _prot_KE = mct.Start().Momentum().E() - proton_mass;


    _vect_trk_len = (
                      geoalgo::Vector( (*mct.rbegin()).Position() ) -
                      geoalgo::Vector( (*mct.begin() ).Position() )
                    ).Length();
    _sum_trk_len = 0;

    if (mct.size() >= 3) {
      for (size_t istep = 0; istep < mct.size() - 2; istep++) {
        auto step = mct.at(istep);
        auto nextstep = mct.at(istep + 1);
        _sum_trk_len += (geoalgo::Vector(nextstep.Position()) - geoalgo::Vector(step.Position())).Length();
      }
    }
    else if (mct.size() == 2) {
      _sum_trk_len = (geoalgo::Vector(mct.at(1).Position()) - geoalgo::Vector(mct.at(0).Position())).Length();
    }
    else
      _sum_trk_len = 0;

    _pdg = mct.PdgCode();
    _ana_tree->Fill();

  }//end loop over mctracks

  return true;
}

bool ProtonStudy::finalize() {

  if (_fout) {
    _fout->cd();
    if (_ana_tree)
      _ana_tree->Write();
  }

  return true;
}

}
#endif
