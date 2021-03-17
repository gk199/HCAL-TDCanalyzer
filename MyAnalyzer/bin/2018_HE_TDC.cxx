#include "TMath.h"
#include "TFile.h"
#include "TH1F.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TColor.h"
#include "TLegend.h"
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
  TFile *f = new TFile("/afs/cern.ch/work/g/gkopp/MWGR/CMSSW_11_2_2_patch1/src/HcalDigiAnalyzer-2018RAW/MyAnalyzer/IsoBunch_Run2018D.root");

  TTreeReader myReader("MyAnalyzer/qiedigi",f);

  TTreeReaderValue<Float_t> Run(myReader, "RunNum");
  TTreeReaderValue<Float_t> Lumi(myReader, "LumiNum");
  TTreeReaderValue<Float_t> Event(myReader, "EvtNum");
  TTreeReaderValue<Float_t> ADC(myReader, "ADC");
  TTreeReaderValue<Float_t> Charge(myReader, "Charge");
  TTreeReaderValue<Float_t> TDC1(myReader, "TDC1");
  TTreeReaderValue<Float_t> iEta(myReader, "IEta");
  TTreeReaderValue<Float_t> iPhi(myReader, "IPhi");
  TTreeReaderValue<Float_t> Depth(myReader, "Depth");

  TCanvas* c1 = new TCanvas("c1","TDC Distribution, 2018D",0,0,400,300);
  gStyle->SetOptStat(0);
  TH1F* depth1 = new TH1F("depth1","HE Depth 1",50,0,49);
  TH1F* depth2 = new TH1F("depth2","HE Depth 2",50,0,49);
  TH1F* depth3 = new TH1F("depth3","HE Depth 3",50,0,49);
  TH1F* depth4 = new TH1F("depth4","HE Depth 4",50,0,49);
  TH1F* depth5 = new TH1F("depth5","HE Depth 5",50,0,49);
  TH1F* depth6 = new TH1F("depth6","HE Depth 6",50,0,49);
  TH1F* depth7 = new TH1F("depth7","HE Depth 7",50,0,49);

  TH1F* depth2_ieta = new TH1F("depth2_ieta","HE Depth 2 by ieta",16,15,30);

  gROOT->SetObjectStat(0);

  int evtCounter = 1;
  while (myReader.Next()) {
    if ( abs(*iEta) > 30 ) continue; // don't consider HF
    if ( *Charge > 7000 && *ADC > 50) { // about 3 GeV, this is the charge for the full digi pulse shape
      if (*Depth == 1) depth1->Fill(*TDC1);
      if (*Depth == 2) depth2->Fill(*TDC1);
      if (*Depth == 3) depth3->Fill(*TDC1);
      if (*Depth == 4) depth4->Fill(*TDC1);
      if (*Depth == 5) depth5->Fill(*TDC1);
      if (*Depth == 6) depth6->Fill(*TDC1);
      if (*Depth == 7) depth7->Fill(*TDC1);
      
      //      if (*Depth == 2) 
    }
    evtCounter++;
  }
  depth1->SetLineColor(kRed);
  depth2->SetLineColor(kGreen);
  depth3->SetLineColor(kTeal);
  depth4->SetLineColor(kBlue);
  depth5->SetLineColor(kGray);
  depth6->SetLineColor(kBlack);
  depth7->SetLineColor(kOrange);

  depth1->Draw();
  depth2->Draw("same");
  depth3->Draw("same");
  depth4->Draw("same");
  depth5->Draw("same");
  depth6->Draw("same");
  depth7->Draw("same");

  TLegend* leg = new TLegend(0.55,0.6,0.9,0.9);
  leg->AddEntry(depth1,Form("HE Depth 1, mean =%.2f",depth1->GetMean()));
  leg->AddEntry(depth2,Form("HE Depth 2, mean =%.2f",depth2->GetMean()));
  leg->AddEntry(depth3,Form("HE Depth 3, mean =%.2f",depth3->GetMean()));
  leg->AddEntry(depth4,Form("HE Depth 4, mean =%.2f",depth4->GetMean()));
  leg->AddEntry(depth5,Form("HE Depth 5, mean =%.2f",depth5->GetMean()));
  leg->AddEntry(depth6,Form("HE Depth 6, mean =%.2f",depth6->GetMean()));
  leg->AddEntry(depth7,Form("HE Depth 7, mean =%.2f",depth7->GetMean()));
  leg->Draw("same");
  //  gPad->BuildLegend();
  depth1->SetTitle("TDC Distribution, 2018D");
  depth1->GetXaxis()->SetTitle("TDC value in SOI, 1/2 ns steps");
  depth1->GetYaxis()->SetTitle("Entries");
  c1->SaveAs("TDC_by_depth.pdf");


}
