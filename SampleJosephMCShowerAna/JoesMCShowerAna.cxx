#ifndef JOESMCSHOWERANA_CXX
#define JOESMCSHOWERANA_CXX

#include "JoesMCShowerAna.h"

namespace larlite {

  bool JoesMCShowerAna::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //

    return true;
  }
  
  bool JoesMCShowerAna::analyze(storage_manager* storage) {
  
    //
    // Do your event-by-event analysis here. This function is called for 
    // each event in the loop. You have "storage" pointer which contains 
    // event-wise data. To see what is available, check the "Manual.pdf":
    //
    // http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
    // 
    // Or you can refer to Base/DataFormatConstants.hh for available data type
    // enum values. Here is one example of getting PMT waveform collection.
    //
    // event_fifo* my_pmtfifo_v = (event_fifo*)(storage->get_data(DATA::PMFIFO));
    //
    // if( event_fifo )
    //
    //   std::cout << "Event ID: " << my_pmtfifo_v->event_id() << std::endl;
    //


    // Get the vector of mcshower objects from the file, for this event
    const event_mcshower* my_mcs_v = storage->get_data<event_mcshower>("mcshower");
    if( !my_mcs_v ){
      print(msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mcshower!"));
      return false;
    }
    
    // Get event_id() from the event_mcshower object
    std::cout << "Event ID: " << my_mcs_v->event_id() << std::endl;

    // Each element in this vector is an individual MCShower. Loop over:
    for( size_t i = 0; i < my_mcs_v->size(); ++i){

      auto this_shower = my_mcs_v->at(i);

      //See $LARLITE_COREDIR/DataFormat/mcshower.h for accessing parameters

      std::cout<<Form("MCShower %zu has generation point: (%f,%f,%f)\n",
		      i,
		      this_shower.MotherPosition().at(0),
		      this_shower.MotherPosition().at(1),
		      this_shower.MotherPosition().at(2));

      std::cout<<Form("MCShower %zu its first energy deposition point at: (%f,%f,%f)\n",
		      i,
		      this_shower.DaughterPosition().at(0),
		      this_shower.DaughterPosition().at(1),
		      this_shower.DaughterPosition().at(2));
    
      std::cout<<Form("MCShower %zu has (mother) energy: %f\n",
		      i,
		      this_shower.MotherMomentum().at(3));

    }// End loop over mcshowers



    return true;
  }// End analyze function

  bool JoesMCShowerAna::finalize() {

    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    //
    // Say you made a histogram pointer h1 to store. You can do this:
    //
    // if(_fout) { _fout->cd(); h1->Write(); }
    //
    // else 
    //   print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    //
  
    return true;
  }

}
#endif
