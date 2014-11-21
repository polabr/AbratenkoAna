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
# SignalSelection
rootlibmap libKalekoAna_SignalSelection.rootmap libKalekoAna_SignalSelection.so $LARLITE_USERDEVDIR/KalekoAna/SignalSelection/LinkDef.h \
    libLArLite_LArUtil.so \
    libBasicTool_GeoAlgo.so \

