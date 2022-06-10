// -*- C++ -*-
//
// Package:    HcalDPG/MyAnalyzer
// Class:      MyAnalyzer
//
/**\class MyAnalyzer MyAnalyzer.cc HcalDPG/MyAnalyzer/plugins/MyAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Long Wang
//         Created:  Wed, 05 Aug 2020 08:12:27 GMT
//
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalDetId/interface/HcalGenericDetId.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalRecHit/interface/CaloRecHitAuxSetter.h"
#include "DataFormats/HcalRecHit/test/HcalRecHitDump.cc"

#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"

#include "TH1.h"
#include <TNtuple.h>
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.

class MyAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  explicit MyAnalyzer(const edm::ParameterSet&);
  ~MyAnalyzer();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void beginJob() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override;

  // ----------member data ---------------------------
  edm::EDGetTokenT<HBHERecHitCollection> hcalRecHitsToken_;
  edm::EDGetTokenT<QIE11DigiCollection> qie11digisToken_;
  edm::ESGetToken<HcalDbService, HcalDbRecord> hcalDbServiceToken_;

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  edm::ESGetToken<SetupData, SetupRecord> setupToken_;
#endif
  TNtuple* tup_rh;
  TNtuple* tup_qie;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
MyAnalyzer::MyAnalyzer(const edm::ParameterSet& iConfig)
  //:  hcalRecHitsToken_ (consumes<HBHERecHitCollection>(iConfig.getUntrackedParameter<edm::InputTag>("tagRecHit"))),
  :   qie11digisToken_ (consumes<QIE11DigiCollection>(iConfig.getUntrackedParameter<edm::InputTag>("tagQIE11", edm::InputTag("hcalDigis")))), 
      hcalDbServiceToken_ (esConsumes<HcalDbService, HcalDbRecord>()) {// edm::Transition::BeginRun>()) {
  
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  setupDataToken_ = esConsumes<SetupData, SetupRecord>();
#endif

  //now do what ever initialization is needed
  usesResource("TFileService");
  edm::Service<TFileService> fs;
  tup_rh = fs->make<TNtuple>("rechit", "rechit", "RunNum:LumiNum:EvtNum:Energy:Time:TDC0:TDC1:TDC2:TDC3:TDC4:IEta:IPhi:Depth");
  tup_qie= fs->make<TNtuple>("qiedigi", "qiedigi", "RunNum:LumiNum:EvtNum:BunchCrossing:ADC:Charge:TDC0:TDC1:TDC2:TDC3:IEta:IPhi:Depth:TCharge:TADC");
}

MyAnalyzer::~MyAnalyzer() {
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  //
  // please remove this method altogether if it would be left empty
}

//
// member functions
//

// ------------ method called for each event  ------------
void MyAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;

  long runid   = iEvent.id().run();
  long eventid = iEvent.id().event();
  long lumiid  = iEvent.id().luminosityBlock();
  long bunchx  = iEvent.bunchCrossing();

  //  edm::ESHandle<HcalDbService> conditions;
  //  iSetup.get<HcalDbRecord>().get(conditions); // this is obsolete, replaced by below line and hcalDbServiceToken initialized above
  edm::ESHandle<HcalDbService> conditions = iSetup.getHandle(hcalDbServiceToken_);

  //edm::Handle<HBHERecHitCollection> hcalRecHits;
  edm::Handle<QIE11DigiCollection> qie11Digis;

  //bool gotHcalRecHits = iEvent.getByToken(hcalRecHitsToken_, hcalRecHits);
  bool gotQIE11Digis = iEvent.getByToken(qie11digisToken_, qie11Digis);

  //if (!gotHcalRecHits)
    //std::cout << "Could not find HCAL RecHits with tag: hbhereco " << std::endl;
  if (!gotQIE11Digis)
    std::cout << "Could not find HCAL QIE11Digis with tag: qie11Digis" << std::endl;

/*
  for (HBHERecHitCollection::const_iterator it = hcalRecHits->begin(); it != hcalRecHits->end(); ++it) {
    //	Explicit check on the DetIds present in the Collection
    HcalDetId did = it->id();
    if(did.subdet() != HcalEndcap) continue;

    unsigned rawTDCValues[5] = {0,};
    bool unpackedTDCData = false;

    const uint32_t auxTDC = it->auxTDC();
    if (auxTDC) {
      const unsigned six_bits_mask = 0x3f;
      for (unsigned ts = 0; ts < 5; ++ts)
        rawTDCValues[ts] = CaloRecHitAuxSetter::getField(auxTDC, six_bits_mask, ts * 6);
      unpackedTDCData = true;
    }

    if(!unpackedTDCData) std::cout << "Found rechit with unpackedTDCData" << std::endl;

    tup_rh->Fill(runid, lumiid, eventid, it->energy(), it->time(), rawTDCValues[0], rawTDCValues[1], rawTDCValues[2], rawTDCValues[3], rawTDCValues[4], did.ieta(), did.iphi(), did.depth());

  }*/

  for (QIE11DigiCollection::const_iterator it = qie11Digis->begin(); it != qie11Digis->end(); ++it) {
    const QIE11DataFrame digi = static_cast<const QIE11DataFrame>(*it);

    //	Explicit check on the DetIds present in the Collection
    HcalDetId const& did = digi.detid();
    //    if(did.subdet() != HcalEndcap || did.subdet() != HcalBarrel) continue; // this causes HB HE TDC error code to not write
    if(did.subdet() != HcalEndcap) continue;

    const HcalQIECoder* channelCoder = conditions -> getHcalCoder(did);
    const HcalQIEShape* shape = conditions -> getHcalShape(channelCoder);
    HcalCoderDb coder(*channelCoder,*shape);
    CaloSamples cs; coder.adc2fC(digi,cs);

    int nTS = digi.samples(); 

    float charge_ = 0;
    int ADC_=0;
    for(int i=0; i<nTS; i++){
      charge_ += cs[i];
      ADC_ += digi[i].adc();
    }

    double TCharge = charge_; int TADC = ADC_;
    for (QIE11DigiCollection::const_iterator it2 = qie11Digis->begin(); it2 != qie11Digis->end(); ++it2) {
      if(it==it2) continue;
      const QIE11DataFrame digi2 = static_cast<const QIE11DataFrame>(*it2);
      HcalDetId const& did2 = digi2.detid();
      //      if(did2.subdet() != HcalEndcap || did2.subdet() != HcalBarrel) continue;
      if(did2.subdet() != HcalEndcap) continue;

      const HcalQIECoder* channelCoder2 = conditions -> getHcalCoder(did2);
      const HcalQIEShape* shape2 = conditions -> getHcalShape(channelCoder2);
      HcalCoderDb coder2(*channelCoder2,*shape2);
      CaloSamples cs2; coder2.adc2fC(digi2,cs2);

      if((did.ieta()==did2.ieta()) && (did.iphi()==did2.iphi()) && (did.depth()!=did2.depth())){
        for(int i=0; i<nTS; i++){
          TCharge += cs2[i];
          TADC += digi2[i].adc();
        }
      }
    }

    tup_qie->Fill(runid, lumiid, eventid, bunchx, ADC_, charge_, digi[2].tdc(), digi[3].tdc(), digi[4].tdc(), digi[5].tdc(), did.ieta(), did.iphi(), did.depth(), TCharge, TADC);

  }


#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  // if the SetupData is always needed
  auto setup = iSetup.getData(setupToken_);
  // if need the ESHandle to check if the SetupData was there or not
  auto pSetup = iSetup.getHandle(setupToken_);
#endif
}

// ------------ method called once each job just before starting event loop  ------------
void MyAnalyzer::beginJob() {
  // please remove this method if not needed
}

// ------------ method called once each job just after ending the event loop  ------------
void MyAnalyzer::endJob() {
  // please remove this method if not needed
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void MyAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

  //Specify that only 'tracks' is allowed
  //To use, remove the default given above and uncomment below
  //ParameterSetDescription desc;
  //desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
  //descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MyAnalyzer);
