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
# scratch_ana
rootlibmap libKalekoAna_scratch_ana.rootmap libKalekoAna_scratch_ana.so $LARLITE_USERDEVDIR/KalekoAna/scratch_ana/LinkDef.h \
    libKalekoAna_SignalSelection.so \
    libLArLite_Analysis.so \
    libLArLite_LArUtil.so \
    libBasicTool_GeoAlgo.so













