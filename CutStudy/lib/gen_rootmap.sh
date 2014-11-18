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
# CutStudy
rootlibmap libKalekoAna_CutStudy.rootmap libKalekoAna_CutStudy.so $LARLITE_USERDEVDIR/KalekoAna/CutStudy/LinkDef.h \
    libLArLite_Analysis.so \
    libLArLite_LArUtil.so \
    libBasicTool_GeoAlgo.so













