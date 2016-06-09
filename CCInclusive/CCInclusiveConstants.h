/**
 * \file CCInclusiveConstants.h
 *
 * \ingroup CCInclusive
 * 
 * \brief Class def header for a class CCInclusiveConstants
 *
 * @author davidkaleko
 */

/** \addtogroup CCInclusive

    @{*/
#ifndef CCINCLUSIVECONSTANTS_H
#define CCINCLUSIVECONSTANTS_H

#include <iostream>

/**
   \class CCInclusiveConstants
   User defined class CCInclusiveConstants ... these comments are used to generate
   doxygen documentation!
 */
   namespace larlite{
   	
  /// Category of input file (for different BGW times)
    enum InputFileType_t {
      kOnBeam,
      kOffBeam,
      kCorsikaInTime,
      kBNBOnly,
      kBNBCosmic,
      kINPUT_FILE_TYPE_MAX
    };

}

#endif
/** @} */ // end of doxygen group 

