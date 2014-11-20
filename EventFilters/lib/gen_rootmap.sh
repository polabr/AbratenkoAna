#!/bin/bash

temp=make_rootmap.$$.temp

rootlibmap() {
 ROOTMAP=$1
 SOFILE=$2
 LINKDEF=$3
 shift 3
 DEPS=$*
 if [[ -e $SOFILE && -e $LINKDEF ]]; then
     rlibmap -f -o $ROOTMAP -l $SOFILE -d $DEPS -c $LINKDEF 2>> $temp
     rm -f $temp
 fi
}

######################################################
# EventFilters
rootlibmap libKalekoAna_EventFilters.rootmap libKalekoAna_EventFilters.so $LARLITE_USERDEVDIR/KalekoAna/EventFilters/LinkDef.h \
    libLArLite_Analysis.so \
    libLArLite_LArUtil.so \
    libBasicTool_GeoAlgo.so \
    libSelectionTool_ShowerStudy.so













