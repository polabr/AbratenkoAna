/**
 * \file ERAnaCosmicValidation.h
 *
 * \ingroup ERToolStuff
 *
 * \brief Class def header for a class ERAnaCosmicValidation
 *
 * @author davidkaleko
 */

/** \addtogroup ERToolStuff

    @{*/

#ifndef ERTOOL_ERANACOSMICVALIDATION_H
#define ERTOOL_ERANACOSMICVALIDATION_H

#include "ERTool/Base/AnaBase.h"
#include "TTree.h"
#include "GeoAlgo/GeoAlgo.h"
#include "ERTool/Algo/AlgoFindRelationship.h"

namespace ertool {

    /**
       \class ERAnaCosmicValidation
       User custom Analysis class made by kazuhiro
     */
    class ERAnaCosmicValidation : public AnaBase {

    public:

        // enum MC_ParticleGeneration_t {
        //   kPrimary,      ///< Primary cosmic
        //   kSecondary,    ///< Secondary cosmic
        //   kOther,        ///< Neither primary or secondary
        //   kINVALID_RECO_TYPE
        // };

        /// Default constructor
        ERAnaCosmicValidation(const std::string& name = "ERAnaCosmicValidation");

        /// Default destructor
        virtual ~ERAnaCosmicValidation() {}

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

        /// set the energy cut to be used when counting particles
        void SetECut(double c) { _eCut = c; }

    private:

        float _eCut;

        // Result tree comparison for reconstructed events
        // Filled once per ERTool Track or ERTool Shower!
        TTree* _result_tree;
        bool _is_track; // Whether this ttree entry is from an ertool shower, or an ertool track
        bool _true_primary; // Whether this ertool object is truly a cosmic primary
        bool _true_secondary; // Whether this ertool object is truly a cosmic secondary
        bool _true_orphan; // Whether this ertool object is truly a cosmic orphan
        bool _reco_primary; // Whether this ertool object was tagged a cosmic primary
        bool _reco_secondary; // Whether this ertool object was tagged a cosmic secondary
        bool _reco_orphan; // Whether this ertool object was tagged a cosmic orphan
        int _pdg_parent; //PDG of parent of particle (-1 if no parent)
        int _pdg_self; //PDG of particle
        float _perp_dist_top_wall; // Dist from particle start point to top wall of TPC (-1 if not in TPC)
        float _orphan_dist; // Dist CROrphan uses to determine if something is orphan
        float _energy; // Energy of track or shower associated with the particle.
        float _trk_length; // If particle is a track, this is track.Length()
        float _start_x_vtx;
        float _start_y_vtx;
        float _start_z_vtx;
        bool _start_contained_in_TPC; // Whether start point of particle is contained in TPC box
        bool _trackend_contained_in_TPC; // Whether end point of particle is contained in TPC box
        float _perp_dist_any_wall; // Min [perpendicular distance b/t start point or end point] and any wall of TPC box
        bool _going_upwards; // Whether particle has positive y-component of direction
        float _dist_to_closest_particle; // Shortest distance between this particle and the nearest other particle
        bool _tagged_as_neutron; // Whether CROrphan tagged this particle as a neutron

        /// prepare TTree with variables
        void PrepareTreeVariables();
        /// Function to re-set TTree variables
        void ResetTreeVariables();

        /// Simple function that takes in a particle from mcparticlegraph
        /// and returns the MC generation of that particle (to tell if it's primary, secondary, etc)
        bool IsMCParticlePrimary(const ertool::Particle& myparticle);

        /// Simple function that takes in a particle from mcparticlegraph
        /// and returns whether that particle is "secondary"
        /// (meaning, that particle has a parent in the mcparticlegraph that deposits energy in the detector)
        bool IsMCParticleSecondary(const ertool::Particle & myparticle);

        /// Simple function that takes in a particle from mcparticlegraph
        /// and returns whether that particle is "orphan"
        bool IsMCParticleOrphan(const ertool::Particle & myparticle);

        /// Function that takes a particle from the reco particle graph and returns
        /// the matching particle from the MCParticleGraph
        ertool::Particle FindMCParticleGraphParticle(const EventData & data, const ertool::Particle & p);

        /// Function to take in a particle from the mcparticle graph, and find all the mc secondaries associated with
        /// that particle, and add them to a std::set keeping track of them all.
        void TrackAllSecondaries(const ertool::Particle &p);

        /// Basic counter: # of primary, secondary, other, skipped particles
        std::vector< size_t > _total_particles;

        /// List of all potential nodeID of MC secondaries associated with a tagged primary particle
        std::set< size_t > _potential_secondary_nodeIDs;

        int evtno;

        ::geoalgo::GeoAlgo _geoalg;
        ::geoalgo::AABox _tpc_box;
        ertool::AlgoFindRelationship _findRel;

    };
}
#endif

/** @} */ // end of doxygen group
