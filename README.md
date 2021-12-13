# HCAL TDC and Extended Bits Analyzers
This code is used for looking at TDC data from 2018 HE runs, and 2021 MWGRs with packed TDC values and extended bits in HB. Details for each approach are below:

## Setup
Setup a CMSSW environment, an example below:
```
cmsrel CMSSW_11_2_0
cd CMSSW_11_2_0/src
cmsenv

git clone git@github.com:gk199/HCAL-TDCanalyzer.git
cd MyAnalyzer
cmsenv
scram b -j 8
```

## 2018 HE: HcalDigiAnalyzer
Starting from 2018 Isolated Bunch files (`/store/data/Run2018D/IsolatedBunch/RAW`, [2018D DAS link](https://cmsweb.cern.ch/das/request?instance=prod/global&input=file+dataset%3D%2FIsolatedBunch%2FRun2018D-v1%2FRAW); or `/store/data/Run2018A/IsolatedBunch/RAW`, [2018A DAS link](https://cmsweb.cern.ch/das/request?instance=prod/global&input=file+dataset%3D%2FIsolatedBunch%2FRun2018A-v1%2FRAW)), the ones with a valid events are listed in `MyAnalyzer/python/ConfFile_*_cfg.py`. Single muon 2018A events are also used to understand the PU contributions to TDC=62 rate, from `/store/data/Run2018D/SingleMuon/RAW` [2018A single muon DAS link](https://cmsweb.cern.ch/das/request?view=list&limit=50&instance=prod%2Fglobal&input=file+dataset%3D%2FSingleMuon%2FRun2018A-v1%2FRAW). Using minimum bias files was also investigated, but all were moved to TAPE, while sigle muon was on disk and expected to not bias the TDC error code rates significantly.

To run over these, do `cmsRun python/ConfFile_*_cfg.py`, or submit Condor jobs. This is done with
```
voms-proxy-init --rfc --voms cms --valid 48:00
cp /tmp/x509up_u101898 /afs/cern.ch/user/g/gkopp
chmod 777 /afs/cern.ch/user/g/gkopp/x509up_u101898
kinit
condor_submit condor_IsoBunch_*.sub
condor_q --nobatch
```
This will then run the hcalDigis task to get the TDC info from the RAW data. In `MyAnalyzer/plugins/MyAnalyzer.cc`, the QIE11 digis are accessed. The TDC value for each TS is reported (with digi[3].tdc() from the SOI, and saved as TDC1), and the pulse shape is integrated to report the charge and ADC of the full digi pulse. An ntuple `IsoBunch_Run2018*.root` is created, with the branches holding the TDC and charge info, which can be changed by editing `MyAnalyzer.cc`. Bunch crossing number is also included.

To plot the TDC per depth and per ieta, edit `MyAnalyzer/bin/2018_HE_TDC.cxx`, and run with 
```
cmsenv
scram b -j 8
2018_HE_TDC.exe
```
Some of this above code has been adapted from the analyzer by Long Wang. This plots the TDC distribution (for pulses above about 3GeV) by ieta and depth, and also does the background 90% calculation (for pulses above 2GeV).

### Run 3 MC
Similar plots were made in Run 3 MC on QCD samples, which is done here: `/afs/cern.ch/work/g/gkopp/HCAL_Trigger/L1Ntuples/HCAL_TP_TimingBitEmulator/CMSSW_11_2_0/src/HcalTrigger/Validation` with the script `bin/rates_FineGrain.cxx`. This is run with `rates_bkgTDCdist.exe new QCD_PU_18pt7 3 3` where the arguments are HB and HE energy requirements to plot. This script is also used to determine 90\% values for background samples.

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

Ideally we'd like to cross-correlate this with the fine-grain bits, but the current MWGR used don't have HB values processed. This approach uses MWGR cosmics [RAW data](https://cmsweb.cern.ch/das/request?input=file+dataset%3D%2FCosmics%2FCommissioning2021-v1%2FRAW&instance=prod/global&amp;idx=50&&amp;limit=50).

## MWGR TDC -- fine grain bits
Starting from the MWGR Cosmics run (as above, but now using the [RAW-RECO data](https://cmsweb.cern.ch/das/request?instance=prod/global&input=file+dataset%3D%2FCosmics%2FCommissioning2021-CosmicTP-PromptReco-v1%2FRAW-RECO)), the TDC fine grain bits can be looked at. This is done in `HcalDigiAnalyzer-2018RAW/MyAnalyzer/bin/RechHit.cxx` and `RechHit.exe`, where the TDC value (packed in HB, so is 0, 1, 2, or 3; or 0-63 in HE), time slice, event, and ieta are printed out. 

For simplicity, a single RAW-RECO file is copied locally to run over:
```
xrdcp root://cmsxrootd.fnal.gov//store/mc/*/*/*/*/*/*.root path/to/output
```

## MWGR / CRUZET 2021 processing
Looking at CRUZET 2021 data to run the [HcalUnpacker.cc](https://cmssdt.cern.ch/lxr/source/EventFilter/HcalRawToDigi/src/HcalUnpacker.cc) (specifically function unpackUTCA) in the Raw2Digi step. This was done on run 342797, and HcalTrigTowDetId id.version() and id.setVersion() (from [HcalTrigTowerDetId.cc](https://cmssdt.cern.ch/lxr/source/DataFormats/HcalDetId/src/HcalTrigTowerDetId.cc)) were used here. From [HcalUHTRData.h](https://cmssdt.cern.ch/lxr/source/EventFilter/HcalRawToDigi/interface/HcalUHTRData.h), can getFirmwareFlavor() and getFirmwareRevision() using MASK and OFFSET defined in [PackerHelper.h](https://cmssdt.cern.ch/lxr/source/EventFilter/HcalRawToDigi/plugins/PackerHelp.h). This was to investigate possible ideas about how to pass the uHTR firmware version further along in the processing chain, but it seems likely that a more reasonable approach would be to put this in the HcalTriggerPrimitiveDigi data format, with the integer field "presamples", of which only four bits are currently used. This is already a mixed use field, and could handle RAW and onwards in data. 

To look at a specific run and determine details, reference [OMS](https://cmsoms.cern.ch/cms/fills/bunch_info?cms_fill=7256). Find the "Fill" for that run, and pick "bunch info" from the drop down menu to show a table of which BX were filled or not. This tool also shows which subsystems were running at the time of data collection.

For the CRUZET data, use the following cmsDriver command (recommended from Hypernews):

```
cmsDriver.py CRUZET_hcal -s RAW2DIGI:RawToDigi_hcalOnly --data --scenario cosmics --era Run3 --conditions auto:run3_data_prompt --filein /store/data/Commissioning2021/Cosmics/RAW/v1/000/342/797/00000/4c3681af-72eb-42dc-9279-62e9e56c8547.root --fileout 2021Cosmic.root -n 100
```

This work was done in `/afs/cern.ch/work/g/gkopp/L1emulator/CMSSW_12_0_0_pre5/src/` as I was already working here to develop the full [L1 emulator](https://github.com/gk199/cmssw/tree/LLPtriggerHCAL_L1emulator) with the Layer-1, Layer-2, and jet algorithm modifications. 

## Location on lxplus
```
/afs/cern.ch/work/g/gkopp/MWGR/CMSSW_11_2_2_patch1/src/HcalDigiAnalyzer-2018RAW
```

### Helpful references
https://cmssdt.cern.ch/lxr/source/DataFormats/HcalRecHit/test/HcalRecHitDump.cc

https://cmssdt.cern.ch/lxr/source/DataFormats/HcalRecHit/interface/HBHERecHit.h

https://cmssdt.cern.ch/lxr/source/DataFormats/HcalDigi/interface/QIE11DataFrame.h

For looking at RAW and RAW-RECO data, using the "parent-child" relationship tracing on DAS is helpful to see which files are processed to later data tiers.

Studies by others to reference: 2018 HE TDC studies from Long -- [first](https://indico.cern.ch/event/944874/contributions/3970495/attachments/2094462/3524649/TDCdatavsmc.pdf), [second](https://indico.cern.ch/event/961080/contributions/4042712/attachments/2114609/3557646/TDCdatavsmc2.pdf). Michael Krohn's [extended bits](https://indico.cern.ch/event/995767/contributions/4184636/attachments/2176494/3675363/HCAL_L1FeatureBits_MWGR3_RemoveMixedModules.pdf).
