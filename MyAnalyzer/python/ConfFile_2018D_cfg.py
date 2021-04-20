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
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/323/416/00000/DD79DB8A-6E4F-594F-B679-941C4D198E2D.root',
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/324/729/00000/2B4B4F1E-3FA9-9143-B3B3-A2AEE61BFE4B.root',
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/324/729/00000/3EBE1EE2-546F-9A43-A5D4-37852227D0BB.root',
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/324/729/00000/D7EAC3B3-0B7F-104F-9701-2A155F8359CD.root',
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/324/729/00000/ED53AAF9-6F71-1945-AE36-FC40CA6E79D8.root',
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/324/729/00000/F4320861-24A3-C943-B04B-32241C65B880.root',
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/995/00000/00F17AB2-899A-E811-9111-FA163E370FDB.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/995/00000/16419319-8B9A-E811-8F80-FA163EE5B1AD.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/995/00000/38E30EE2-829A-E811-8FFF-FA163E756520.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/995/00000/4ADEEFD5-829A-E811-811D-FA163EE50654.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/995/00000/5842D7D1-829A-E811-ADBF-FA163E4055E5.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/995/00000/5A6BCF6D-8C9A-E811-B00B-FA163E21086B.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/995/00000/5E24D816-829A-E811-AD7D-FA163E517117.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/995/00000/7010AD44-8B9A-E811-A6BF-FA163E975D95.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/995/00000/7829A370-899A-E811-AB5C-FA163E04A606.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/995/00000/9800B67F-899A-E811-A174-02163E01A15C.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/995/00000/AAC1488F-8C9A-E811-86FA-FA163EB1AF7C.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/995/00000/C2ECD974-829A-E811-8518-FA163E1C1225.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/995/00000/C6F7C565-8C9A-E811-80DE-A4BF0127B2B3.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/995/00000/CA632822-8A9A-E811-BC00-02163E00BA13.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/0409D776-939A-E811-930B-A4BF0127AF07.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/28006257-949A-E811-9E11-02163E013283.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/2C0093D6-949A-E811-8B6C-FA163E6D3900.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/36572C99-919A-E811-8CA4-A4BF0112BD52.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/3E34367C-A19A-E811-B221-FA163E2FE8B3.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/5E6F8B72-969A-E811-A145-02163E00BBAF.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/6EA750A6-919A-E811-9308-FA163EA4290B.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/904FE6E1-919A-E811-897A-FA163E09ACB2.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/9073FC08-9F9A-E811-90FD-FA163E39C36B.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/A07D277C-919A-E811-BE32-FA163EACCC10.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/A8705592-939A-E811-BDF8-FA163ED6D685.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/AAD80669-929A-E811-9347-FA163EE52A49.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/B0E791BF-919A-E811-BC10-FA163E359971.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/C097176A-979A-E811-98E7-02163E01A14D.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/C6C19C92-919A-E811-8086-FA163E1C5C83.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/CAAE4264-929A-E811-A79B-FA163E60FDAB.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/D47BA881-9C9A-E811-BEEE-FA163E47A7A4.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/D4FFBD2E-A19A-E811-A2D9-FA163EF2167E.root', 
            '/store/data/Run2018D/IsolatedBunch/RAW/v1/000/320/996/00000/EC3A47DE-9C9A-E811-A41A-FA163EF4E3E9.root' 
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
      fileName = cms.string("IsoBunch_Run2018D_bx.root"),
      closeFileFast = cms.untracked.bool(True)
)


process.p = cms.Path(
	process.bunchSpacingProducer*
        process.hcalDigis*
#	process.hcalLocalRecoSequence*
	process.MyAnalyzer
)

#process.outpath = cms.EndPath(process.out)
