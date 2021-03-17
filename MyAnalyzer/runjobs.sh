#!/bin/bash

export X509_USER_PROXY=$1
voms-proxy-info -all
voms-proxy-info -all -file $1

condor_argu=$2

cd /afs/cern.ch/work/g/gkopp/MWGR/CMSSW_11_2_2_patch1/src/HcalDigiAnalyzer-2018RAW/MyAnalyzer
eval `scramv1 runtime -sh`
cmsRun $condor_argu
