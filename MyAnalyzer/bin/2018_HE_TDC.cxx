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

  std::map<int, TH1F*> depth_all_ieta; // histogram for TDC at each depth
  std::map<int, std::map<int, TH1F*>> depth_by_ieta; // histogram for TDC at each depth and ieta
  for (int depth = 1; depth <= 7; depth ++) {
    depth_all_ieta[depth] = new TH1F(Form("depth_all_ieta_%d",depth),Form("HE Depth %d",depth), 50,0,49);
    for (int ieta = 16; ieta < 30; ieta ++) depth_by_ieta[depth][ieta] = new TH1F(Form("depth%d_ieta%d",depth, ieta), Form("HE Depth %d, ieta %d", depth, ieta), 50,0,49);
  }

  gROOT->SetObjectStat(0);

  int evtCounter = 1;
  while (myReader.Next()) {
    if ( abs(*iEta) >= 30 ) continue; // don't consider HF
    if ( *Charge > 7000 && *ADC > 50) { // about 3 GeV, this is the charge for the full digi pulse shape
      depth_all_ieta[*Depth]->Fill(*TDC1);
      depth_by_ieta[*Depth][abs(*iEta)]->Fill(*TDC1);
    }
    evtCounter++;
  }

  TCanvas* c1 = new TCanvas("c1","TDC Distribution, 2018D",0,0,400,300);
  gStyle->SetOptStat(0);
  TLegend* leg = new TLegend(0.5,0.6,0.9,0.9);
  depth_all_ieta[1]->Draw();
  for (int depth = 1; depth <= 7; depth++) 
    {
      depth_all_ieta[depth]->SetLineColor(kRainBow+depth*6);
      //      depth_all_ieta[depth]->Scale(1./depth_all_ieta[depth]->Integral()); // normalize histograms
      depth_all_ieta[depth]->Draw("same");
      leg->AddEntry(depth_all_ieta[depth],Form("HE Depth %d, mean = %.2f",depth,depth_all_ieta[depth]->GetMean()));
    }
  leg->Draw("same");
  depth_all_ieta[1]->SetTitle("TDC Distribution, 2018D");   
  depth_all_ieta[1]->GetXaxis()->SetTitle("TDC value in SOI, 1/2 ns steps");
  depth_all_ieta[1]->GetYaxis()->SetTitle("Entries");

  c1->SaveAs("TDC_by_depth.pdf");
  c1->SetLogy();
  c1->SaveAs("TDC_by_depth_log.pdf");

  TCanvas* c2 = new TCanvas("c2","TDC Distribution, 2018D",0,0,400,300);
  gStyle->SetOptStat(0);
  TLegend* leg2 = new TLegend(0.5,0.6,0.9,0.9);
  for (int ieta = 16; ieta < 30; ieta++) {
    if (ieta == 16) depth_by_ieta[4][ieta]->Draw();
    if (ieta == 17 || ieta == 18) depth_by_ieta[2][ieta]->Draw();
    if (ieta > 18) depth_by_ieta[1][ieta]->Draw();
    for (int depth = 1; depth <= 7; depth++)
      {
	depth_by_ieta[depth][ieta]->SetLineColor(kRainBow+depth*6);
	depth_by_ieta[depth][ieta]->Draw("same");
	leg2->AddEntry(depth_by_ieta[depth][ieta],Form("HE Depth %d, mean = %.2f",depth,depth_by_ieta[depth][ieta]->GetMean()));
      }
    leg2->Draw("same");
    depth_by_ieta[1][ieta]->SetTitle(Form("TDC Distribution at ieta = %d, 2018D",ieta));
    depth_by_ieta[1][ieta]->GetXaxis()->SetTitle("TDC value in SOI, 1/2 ns steps");
    depth_by_ieta[1][ieta]->GetYaxis()->SetTitle("Entries");
    if (ieta ==16) {
      depth_by_ieta[4][ieta]->SetTitle(Form("TDC Distribution at ieta = %d, 2018D",ieta));
      depth_by_ieta[4][ieta]->GetXaxis()->SetTitle("TDC value in SOI, 1/2 ns steps");
      depth_by_ieta[4][ieta]->GetYaxis()->SetTitle("Entries");
    }
    if (ieta == 17 || ieta == 18) {
      depth_by_ieta[2][ieta]->SetTitle(Form("TDC Distribution at ieta = %d, 2018D",ieta));
      depth_by_ieta[2][ieta]->GetXaxis()->SetTitle("TDC value in SOI, 1/2 ns steps");
      depth_by_ieta[2][ieta]->GetYaxis()->SetTitle("Entries");    
    }
    c2->SaveAs(Form("TDC_by_depth_ieta%d.pdf",ieta));
    c2->SetLogy();
    c2->SaveAs(Form("TDC_by_depth_ieta%d_log.pdf",ieta));
    c2->Clear();
    c2->SetLogy(0);
    leg2->Clear();
  }
}
