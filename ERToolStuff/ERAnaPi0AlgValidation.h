/**
 * \file ERAnaPi0AlgValidation.h
 *
 * \ingroup ERToolStuff
 *
 * \brief Class def header for a class ERAnaPi0AlgValidation
 *
 * @author davidkaleko
 */

/** \addtogroup ERToolStuff

    @{*/

#ifndef ERTOOL_ERANAPI0ALGVALIDATION_H
#define ERTOOL_ERANAPI0ALGVALIDATION_H

#include "ERTool/Base/AnaBase.h"
#include "TTree.h"

namespace ertool {

    /**
       \class ERAnaPi0AlgValidation
       User custom Analysis class made by kaleko
     */
    class ERAnaPi0AlgValidation : public AnaBase {

    public:

        /// Default constructor
        ERAnaPi0AlgValidation(const std::string& name = "ERAnaPi0AlgValidation");

        /// Default destructor
        virtual ~ERAnaPi0AlgValidation() {}

        /// Reset function
        virtual void Reset() {};

        /// Function to accept fclite::PSet
        void AcceptPSet(const ::fcllite::PSet& cfg) {};

        /// Called @ before processing the first event sample
        void ProcessBegin() {};

        /// Function to evaluate input showers and determine a score
        bool Analyze(const EventData &data, const ParticleGraph &ps);

        /// Called after processing the last event sample
        void ProcessEnd(TFile* fout = nullptr);

    private:

        float _eCut;

        // Result tree comparison for reconstructed events
        // Filled once per event
        TTree* _result_tree;
        size_t n_true_pi0;
        size_t n_reco_pi0;
        double true_pi0_momentum;
        double reco_pi0_momentum;


        /// prepare TTree with variables
        void PrepareTreeVariables();
        /// Function to re-set TTree variables
        void ResetTreeVariables();
    };
}
#endif

/** @} */ // end of doxygen group
