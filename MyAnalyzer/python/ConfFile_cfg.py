import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

from Configuration.StandardSequences.Eras import eras
process = cms.Process('TDC',eras.Run3)

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.Services_cff')
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)

process.load('Configuration.EventContent.EventContent_cff')
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("RecoLocalCalo.Configuration.hcalLocalReco_cff")
process.load('Configuration.StandardSequences.EndOfProcess_cff')
#process.load("RecoLuminosity.LumiProducer.bunchSpacingProducer_cfi")

process.load("RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi")

process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring(
#		'file:../../out_pions.root'
#		'file:/afs/cern.ch/work/l/lowang/TDC_scan/CMSSW_11_2_X_2020-08-30-2300/src/HcalDPG/10824.0_TTbar_13+2018+TTbar_13TeV_TuneCUETP8M1_GenSim+Digi+Reco+HARVEST+ALCA+Nano/step2.root'
#		'/store/data/Run2018D/JetHT/RAW/v1/000/323/415/00000/87BF31D2-301F-EF47-9268-14B1B258F441.root'
#		'file:/afs/cern.ch/work/l/lowang/TDC_scan/CMSSW_11_2_X_2020-08-30-2300/src/HcalDPG/11024.0_TTbar_13+2018PU+TTbar_13TeV_TuneCUETP8M1_GenSim+DigiPU+RecoPU+HARVESTPU+Nano/step3.root'
#		'file:/afs/cern.ch/work/l/lowang/TDC_scan/CMSSW_11_2_X_2020-08-30-2300/src/step3.root'
		'/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/995/00000/5E24D816-829A-E811-AD7D-FA163E517117.root',
		'/store/data/Run2018D/IsolatedBunch/RAW/v1/000/324/729/00000/F4320861-24A3-C943-B04B-32241C65B880.root',
		'/store/data/Run2018D/IsolatedBunch/RAW/v1/000/324/729/00000/ED53AAF9-6F71-1945-AE36-FC40CA6E79D8.root',
		'/store/data/Run2018D/IsolatedBunch/RAW/v1/000/324/729/00000/D7EAC3B3-0B7F-104F-9701-2A155F8359CD.root',
		'/store/data/Run2018D/IsolatedBunch/RAW/v1/000/324/729/00000/3EBE1EE2-546F-9A43-A5D4-37852227D0BB.root',
		'/store/data/Run2018D/IsolatedBunch/RAW/v1/000/324/729/00000/2B4B4F1E-3FA9-9143-B3B3-A2AEE61BFE4B.root',
		'/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/EC3A47DE-9C9A-E811-A41A-FA163EF4E3E9.root',
		'/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/D4FFBD2E-A19A-E811-A2D9-FA163EF2167E.root',
		'/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/C097176A-979A-E811-98E7-02163E01A14D.root',
		'/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/3E34367C-A19A-E811-B221-FA163E2FE8B3.root',
		'/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/CAAE4264-929A-E811-A79B-FA163E60FDAB.root',
		'/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/C6C19C92-919A-E811-8086-FA163E1C5C83.root',
		'/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/B0E791BF-919A-E811-BC10-FA163E359971.root',
		'/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/AAD80669-929A-E811-9347-FA163EE52A49.root',
		'/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/A8705592-939A-E811-BDF8-FA163ED6D685.root',
		'/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/28006257-949A-E811-9E11-02163E013283.root',
		'/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/0409D776-939A-E811-930B-A4BF0127AF07.root',
		'/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/2C0093D6-949A-E811-8B6C-FA163E6D3900.root',
		'/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/995/00000/9800B67F-899A-E811-A174-02163E01A15C.root',
		'/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/995/00000/5A6BCF6D-8C9A-E811-B00B-FA163E21086B.root'

        )
)

process.options = cms.untracked.PSet(
#	SkipEvent = cms.untracked.vstring('ProductNotFound')
)

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.autoCond import autoCond
process.GlobalTag.globaltag = autoCond['phase1_2021_realistic']
#process.GlobalTag.globaltag = 'phase1_2021_realistic' #'112X_upgrade2018_realistic_v3' #'101X_dataRun2_HLT_v7' #'101X_dataRun2_v8' #'110X_mcRun3_2021_realistic_v6' #'106X_dataRun3_HLT_v3' #'101X_dataRun2_v8' #'101X_dataRun2_HLT_frozen_v6'


process.MyAnalyzer = cms.EDAnalyzer('MyAnalyzer',
#        tagRecHit = cms.untracked.InputTag("hbheprereco"),
#	tagQIE11 = cms.InputTag("simHcalDigis", "HBHEQIE11DigiCollection")
	tagQIE11 = cms.untracked.InputTag("hcalDigis")
)

process.TFileService = cms.Service("TFileService",
      fileName = cms.string("IsoBunch_Run2018D.root"),
      closeFileFast = cms.untracked.bool(True)
)


process.p = cms.Path(
	process.bunchSpacingProducer*
        process.hcalDigis*
#	process.hcalLocalRecoSequence*
	process.MyAnalyzer
)

#process.outpath = cms.EndPath(process.out)
