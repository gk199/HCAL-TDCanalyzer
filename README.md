# HCAL TDC and Extended Bits Analyzers
This code is used for looking at TDC data from 2018 HE runs, and 2021 MWGRs with packed TDC values and extended bits in HB. Details for each approach are below:

## 2018 HE: HcalDigiAnalyzer
Starting from 2018D Isolated Bunch files (`/store/data/Run2018D/IsolatedBunch/RAW`, [DAS link](https://cmsweb.cern.ch/das/request?instance=prod/global&input=file+dataset%3D%2FIsolatedBunch%2FRun2018D-v1%2FRAW)), the ones with a valid events are listed in `/MyAnalyzer/python/ConfFile_cfg.py`. To run over these, can do `cmsRun python/ConfFile_cfg.py`, or submit Condor jobs. This is done with
```
voms-proxy-init --rfc --voms cms --valid 48:00
cp /tmp/x509up_u101898 /afs/cern.ch/user/g/gkopp
chmod 777 /afs/cern.ch/user/g/gkopp/x509up_u101898
kinit
condor_submit condor_IsoBunch.sub
condor_q --nobatch
```
This will then run the hcalDigis task to get the TDC info from the RAW data. In `MyAnalyzer/plugins/MyAnalyzer.cc`, the QIE11 digis are accessed. The TDC value for each TS is reported, and the pulse shape is integrated to report the charge and ADC of the full digi pulse. An ntuple `IsoBunch_Run2018D.root` is created, with the branches holding the TDC and charge info, which can be changed by editing `MyAnalyzer.cc`.

To plot the TDC per depth and per ieta, use `MyAnalyzer/bin/2018_HE_TDC.cxx`, and run with `2018_HE_TDC.exe`. This outputs a pdf plot of the TDC distribution per depth. Some of this above code has been adapted from the analyzer by Long Wang. 

## MWGR Extended Bits
Using `Debug/HcalDebug/plugins/AnalyzeTP.cc`, this places the extended bits in the tps tree after accessing the TP digis. Check that in `python/customize.py`, AnalyzeTP.cc is listed in line 48, a few lines after `def analyze_tp(process, name, tag1):`.  Run this using `test/one_run.py`, and list the relevant runs to consider, along with the dataset information. This will output a python config file -- if only a portion of the run is needed, this can be edited to only use specific files.
```
voms-proxy-init --rfc --voms cms --valid 48:00
cp /tmp/x509up_u101898 /afs/cern.ch/user/g/gkopp
chmod 777 /afs/cern.ch/user/g/gkopp/x509up_u101898
kinit
./one_run.py -r 339479 -t Cosmics -p Commissioning2021
cmsRun analyze_339479.py
```
This is taken from the [HCAL debug framework](https://github.com/gk199/cms-hcal-debug) with more details on the github page. Running the python config will then output a Root file `analyze*.root`, with the extended bits stored. To understand the info in this file, use `HcalDigiAnalyzer-2018RAW/MyAnalyzer/bin/ExtendedBits.cxx`. This reads in the `analyze*.root` file, and running as an executable (`ExtendedBits.exe`) will print the extended bits for an event of interest.

Ideally we'd like to cross-correlate this with the fine-grain bits, but the current MWGR used don't have HB values processed. This approach uses MWGR cosmics (RAW data) [https://cmsweb.cern.ch/das/request?input=file+dataset%3D%2FCosmics%2FCommissioning2021-v1%2FRAW&instance=prod/global&amp;idx=50&&amp;limit=50].

## MWGR TDC -- fine grain bits
Starting from the MWGR Cosmics run (as above, but now using the (RAW-RECO data)[https://cmsweb.cern.ch/das/request?instance=prod/global&input=file+dataset%3D%2FCosmics%2FCommissioning2021-CosmicTP-PromptReco-v1%2FRAW-RECO]), the TDC fine grain bits can be looked at. This is done in `HcalDigiAnalyzer-2018RAW/MyAnalyzer/bin/RechHit.cxx` and `RechHit.exe`, where the TDC value (packed in HB, so is 0, 1, 2, or 3; or 0-63 in HE), time slice, event, and ieta are printed out. 

For simplicity, a single RAW-RECO file is copied locally to run over:
```
xrdcp root://cmsxrootd.fnal.gov//store/mc/*/*/*/*/*/*.root path/to/output
```

## Location on lxplus
```
/afs/cern.ch/work/g/gkopp/MWGR/CMSSW_11_2_2_patch1/src/HcalDigiAnalyzer-2018RAW
```

### Helpful references
https://cmssdt.cern.ch/lxr/source/DataFormats/HcalRecHit/test/HcalRecHitDump.cc

https://cmssdt.cern.ch/lxr/source/DataFormats/HcalRecHit/interface/HBHERecHit.h

https://cmssdt.cern.ch/lxr/source/DataFormats/HcalDigi/interface/QIE11DataFrame.h

For looking at RAW and RAW-RECO data, using the "parent-child" relationship tracing on DAS is helpful to see which files are processed to later data tiers.

Studies by others to reference: 2018 HE TDC studies from Long -- (first)[https://indico.cern.ch/event/944874/contributions/3970495/attachments/2094462/3524649/TDCdatavsmc.pdf], (second)[https://indico.cern.ch/event/961080/contributions/4042712/attachments/2114609/3557646/TDCdatavsmc2.pdf]. Michael Krohn's (extended bits)[https://indico.cern.ch/event/995767/contributions/4184636/attachments/2176494/3675363/HCAL_L1FeatureBits_MWGR3_RemoveMixedModules.pdf].
