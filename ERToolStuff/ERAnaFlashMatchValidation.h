/**
 * \file ERAnaFlashMatchValidation.h
 *
 * \ingroup ERToolStuff
 * 
 * \brief This code is designed for mctracks input on single muons. It assumes you have run ERAlgoFlashMatch
 *        before this. It computes figures of merit intended to exactly copy the couts at the end of
 *        UBT0Finder.
 *
 * @author davidkaleko
 */

/** \addtogroup ERToolStuff

    @{*/

#ifndef ERTOOL_ERANAFLASHMATCHVALIDATION_H
#define ERTOOL_ERANAFLASHMATCHVALIDATION_H

#include "ERTool/Base/AnaBase.h"
// #include "TTree.h"
#include "TH1.h"

namespace ertool {

  /**
     \class ERAnaFlashMatchValidation
     User custom Analysis class made by kazuhiro
   */
  class ERAnaFlashMatchValidation : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaFlashMatchValidation(const std::string& name="ERAnaFlashMatchValidation");

    /// Default destructor
    virtual ~ERAnaFlashMatchValidation(){}

    /// Reset function
    virtual void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Analyze(const EventData &data, const ParticleGraph &ps);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);


  private:

    size_t n_matched_flashes;
    size_t n_qclusters;


   // TTree* _flashmatch_tree;
   //  double _npe;
   //  double _npts;
   //  double _flash_x;
   //  double _flash_y;
   //  double _flash_z;
   //  double _tpc_x;
   //  double _tpc_y;
   //  double _tpc_z;
   //  double _score;
   //  double _flash_time;
   //  double _mc_time;
   //  double _mc_x;
   //  double _mc_y;
   //  double _mc_z;
   //  double _mc_dx;
   //  double _mc_edep;
   //  double _trk_shift;

   //   TTree* _int_tree;
   //  double _t0 ;
   //  double _n_pe ;
   //  double _int_e ;
   //  double _n_flash ;

    TH1D * _time_diff ;


  };
}
#endif

/** @} */ // end of doxygen group 
