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
            '/store/data/Run2018A/IsolatedBunch/RAW/v1/000/315/252/00000/027CAE0B-9449-E811-9255-FA163E5AFFF7.root',
            '/store/data/Run2018A/IsolatedBunch/RAW/v1/000/315/252/00000/06A89AC7-9349-E811-851E-FA163E28A50A.root',
            '/store/data/Run2018A/IsolatedBunch/RAW/v1/000/315/252/00000/12143947-9449-E811-8534-FA163E546B17.root',
            '/store/data/Run2018A/IsolatedBunch/RAW/v1/000/315/252/00000/1863A3FB-9349-E811-9998-FA163E83FD8A.root',
            '/store/data/Run2018A/IsolatedBunch/RAW/v1/000/315/252/00000/1A9E2D16-9449-E811-9F44-02163E00B0CB.root',
            '/store/data/Run2018A/IsolatedBunch/RAW/v1/000/315/252/00000/1CC80BC6-9349-E811-A5EA-02163E01A05F.root',
            '/store/data/Run2018A/IsolatedBunch/RAW/v1/000/315/252/00000/2E340AEA-9349-E811-9733-FA163EB94ED3.root',
            '/store/data/Run2018A/IsolatedBunch/RAW/v1/000/315/252/00000/349B6F2B-9449-E811-9B4B-FA163E688D1A.root',
            '/store/data/Run2018A/IsolatedBunch/RAW/v1/000/315/252/00000/4688DAD0-9349-E811-91F1-02163E017FC4.root',
            '/store/data/Run2018A/IsolatedBunch/RAW/v1/000/315/252/00000/58AC03E5-9349-E811-8F2C-FA163E3B27AF.root'
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
      fileName = cms.string("IsoBunch_Run2018A_bx.root"),
      closeFileFast = cms.untracked.bool(True)
)


process.p = cms.Path(
	process.bunchSpacingProducer*
        process.hcalDigis*
#	process.hcalLocalRecoSequence*
	process.MyAnalyzer
)

#process.outpath = cms.EndPath(process.out)
