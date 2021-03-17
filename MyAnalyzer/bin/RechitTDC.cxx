#include "TMath.h"
#include "TFile.h"
#include "TH1F.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TChain.h"
#include "TROOT.h"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

#include "DataFormats/HcalRecHit/interface/CaloRecHitAuxSetter.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"

int main(int argc, char *argv[]) {
  TFile *f = new TFile("/afs/cern.ch/work/g/gkopp/MWGR/Cosmics_Commissioning2021-CosmicTP-PromptReco-v1_RAW-RECO_run339479_40800997-26af-4c3d-9738-e61184d1e4c5.root");

  TTreeReader myReader("Events",f);

  TTreeReaderValue<std::vector<HBHERecHit>> FullrawTDC(myReader, "HBHERecHitsSorted_hbhereco__RECO.obj.obj"); // rawTDC is the variable name
  TTreeReaderValue<UInt_t> Run(myReader, "EventAuxiliary.id_.run_");
  TTreeReaderValue<UInt_t> Lumi(myReader, "EventAuxiliary.id_.luminosityBlock_");
  TTreeReaderValue<ULong64_t> Event(myReader, "EventAuxiliary.id_.event_");

  unsigned rawTDCValues[5] = {0,};
  const unsigned six_bits_mask = 0x3f;

  int evtCounter = 1;
  while (myReader.Next()) {
    for (HBHERecHit frame:*FullrawTDC) {
      HcalDetId DetectorID = HcalDetId(frame.id());
      const uint32_t auxTDC = frame.auxTDC();
      for (unsigned ts = 0; ts < 5; ts++) {
	rawTDCValues[ts] = CaloRecHitAuxSetter::getField(auxTDC,six_bits_mask,ts*6);
	if (abs(DetectorID.ieta()) > 16 && rawTDCValues[ts] != 63) std::cout << rawTDCValues[ts] << " raw TDC value for TS = " << ts << " in event = " << evtCounter << " at HE ieta = " << DetectorID.ieta() << std::endl;
	if (abs(DetectorID.ieta()) < 16 && rawTDCValues[ts] != 0) {
	  std::cout << rawTDCValues[ts] << " raw TDC value for TS = " << ts << " in event = " << evtCounter << " at HB ieta = " << DetectorID.ieta() << " and iphi = " << DetectorID.iphi() << " at depth " << DetectorID.depth() << std::endl;
	  std::cout << *Run << " = run, " << *Lumi << " = lumi, " << *Event << " = event" << std::endl;
	}
      }
    }
    evtCounter++;
  }
}
