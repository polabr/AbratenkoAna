/**
 * \file CosmicTrackLister.h
 *
 * \ingroup scratch_ana
 * 
 * \brief Class def header for a class CosmicTrackLister
 *
 * @author davidkaleko
 */

/** \addtogroup scratch_ana

    @{*/

#ifndef LARLITE_COSMICTRACKLISTER_H
#define LARLITE_COSMICTRACKLISTER_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class CosmicTrackLister
     User custom analysis class made by davidkaleko
   */
  class CosmicTrackLister : public ana_base{
  
  public:

    /// Default constructor
    CosmicTrackLister(){ 
      _name="CosmicTrackLister"; 
      _fout=0;
      _track_producer_name = "trackkalmanhit";
      _min_cosmic_score = 0.1;
    };

    /// Default destructor
    virtual ~CosmicTrackLister(){};

    /** IMPLEMENT in CosmicTrackLister.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in CosmicTrackLister.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in CosmicTrackLister.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    /// Setter for reco track producer name
    void SetTrackProducerName(std::string name){ _track_producer_name = name; }

    /// Setter for minimum cutoff cosmic score
    void SetMinCosmicScore(double score){ _min_cosmic_score = score; }

  protected:
    
    std::string _track_producer_name;

    double _min_cosmic_score;

    std::vector<size_t> _track_indices;

  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
