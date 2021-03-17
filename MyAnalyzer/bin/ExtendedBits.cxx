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
  TFile *f = new TFile("/afs/cern.ch/work/g/gkopp/MWGR/CMSSW_11_2_2_patch1/src/Debug/HcalDebug/test/analyze_339479_RAWfull.root");

  TTreeReader myReader("analyzeRaw/tps",f);

  TTreeReaderValue<Int_t> Run(myReader, "run");
  TTreeReaderValue<Int_t> Lumi(myReader, "lumi");
  TTreeReaderValue<Int_t> Event(myReader, "event");
  TTreeReaderValue<Int_t> iEta(myReader, "ieta");
  TTreeReaderValue<Int_t> iPhi(myReader, "iphi");
  TTreeReaderValue<Int_t> Depth(myReader, "depth");
  TTreeReaderValue<Int_t> SOI(myReader, "soi");
  TTreeReaderValue<Int_t> FG0(myReader, "fg0");
  TTreeReaderValue<Int_t> FG1(myReader, "fg1");

  int run_of_interest = 339479;
  int lumi_of_interest = 1382;
  int event_of_interest = 22539389;
  int soi_of_interest = 3;
  int depth_of_interest = 4;
  int ieta_of_interest = -2;
  int iphi_of_interest = 16;

  int evtCounter = 1;
  while (myReader.Next()) {
    //    if (*Run == run_of_interest && *Lumi == lumi_of_interest) { 
    if (*Event == event_of_interest) {
      std::cout << *FG0 << " extended bits 0 and 1 = " << *FG1 << std::endl;
    }
    evtCounter++;
  }
}
