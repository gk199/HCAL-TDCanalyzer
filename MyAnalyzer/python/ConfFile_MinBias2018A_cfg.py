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
            '/store/data/Run2018A/MinimumBias/RAW/v1/000/315/252/00000/1A8E4C88-9F49-E811-8015-FA163E0EA436.root',
            '/store/data/Run2018A/MinimumBias/RAW/v1/000/315/255/00000/104B0D0E-9C49-E811-9EF7-FA163E6DCA6C.root',
            '/store/data/Run2018A/MinimumBias/RAW/v1/000/315/257/00000/72DDEE65-A549-E811-89DF-FA163E0C6510.root',
            '/store/data/Run2018A/MinimumBias/RAW/v1/000/315/257/00000/B8B1CB5A-A549-E811-B353-FA163EFAEDB1.root'
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
      fileName = cms.string("MinBias_Run2018A.root"),
      closeFileFast = cms.untracked.bool(True)
)


process.p = cms.Path(
	process.bunchSpacingProducer*
        process.hcalDigis*
#	process.hcalLocalRecoSequence*
	process.MyAnalyzer
)

#process.outpath = cms.EndPath(process.out)
