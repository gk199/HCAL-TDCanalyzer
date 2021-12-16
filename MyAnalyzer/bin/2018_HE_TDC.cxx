#include "TMath.h"
#include "TFile.h"
#include "TH1F.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TGraph.h"
#include "TEfficiency.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TColor.h"
#include "TLegend.h"
#include "TChain.h"
#include "TROOT.h"
#include "TGraphAsymmErrors.h"

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
  TFile *f = new TFile("/afs/cern.ch/work/g/gkopp/MWGR/CMSSW_11_2_2_patch1/src/HcalDigiAnalyzer-2018RAW/MyAnalyzer/IsoBunch_Run2018A.root");
  //  TFile *f = new TFile("/afs/cern.ch/work/g/gkopp/MWGR/CMSSW_11_2_2_patch1/src/HcalDigiAnalyzer-2018RAW/MyAnalyzer/SingleMuon_Run2018A.root");

  TTreeReader myReader("MyAnalyzer/qiedigi",f);

  TTreeReaderValue<Float_t> Run(myReader, "RunNum");
  TTreeReaderValue<Float_t> Lumi(myReader, "LumiNum");
  TTreeReaderValue<Float_t> Event(myReader, "EvtNum");
  //  TTreeReaderValue<Float_t> BunchX(myReader, "BunchCrossing");
  TTreeReaderValue<Float_t> ADC(myReader, "ADC");
  TTreeReaderValue<Float_t> Charge(myReader, "Charge");
  TTreeReaderValue<Float_t> TDC0(myReader, "TDC0"); // SOI-1 
  TTreeReaderValue<Float_t> TDC1(myReader, "TDC1"); // SOI
  TTreeReaderValue<Float_t> iEta(myReader, "IEta");
  TTreeReaderValue<Float_t> iPhi(myReader, "IPhi");
  TTreeReaderValue<Float_t> Depth(myReader, "Depth");

  TH1F *HE62 = new TH1F("HE62","Error Code 62 Rate in HE, 2018A;Depth;Rate of TDC=62",8,0,8);
  TH1F *HE = new TH1F("HE","Error Code 62 Rate in HE, 2018A;Depth;Rate of TDC=62",8,0,8);

  std::map<int, TH1F*> depth_all_ieta; // histogram for TDC at each depth
  std::map<int, TH1F*> depth_all_ieta_tdc0;
  std::map<int, TH1F*> depth_all_ieta_tdc01;
  std::map<int, std::map<int, TH1F*>> depth_by_ieta; // histogram for TDC at each depth and ieta
  std::map<int, std::map<int, TH1F*>> depth_by_ieta_tdc0;
  for (int depth = 1; depth <= 7; depth ++) {
    depth_all_ieta[depth] = new TH1F(Form("depth_all_ieta_%d",depth),Form("HE Depth %d",depth), 50,0,49);
    depth_all_ieta_tdc0[depth] = new TH1F(Form("depth_all_ieta_tdc0_%d",depth),Form("SOI-1 HE Depth %d",depth), 50,0,49);
    depth_all_ieta_tdc01[depth] = new TH1F(Form("depth_all_ieta_tdc01_%d",depth),Form("SOI-1 and SOI HE Depth %d",depth), 100,0,99);
    for (int ieta = 16; ieta < 30; ieta ++) depth_by_ieta[depth][ieta] = new TH1F(Form("depth%d_ieta%d",depth, ieta), Form("HE Depth %d, ieta %d", depth, ieta), 50,0,49);
    for (int ieta = 16; ieta < 30; ieta ++) depth_by_ieta_tdc0[depth][ieta] = new TH1F(Form("depth%d_ieta%d_tdc0",depth, ieta), Form("SOI-1 HE Depth %d, ieta %d", depth, ieta), 50,0,49);
  }

  gROOT->SetObjectStat(0);
  gStyle->SetOptStat(0);

  int eta_depth_tdc[30][8][51] = {{{0}}};
  int TDC_graph[100000] = {0};
  int ADC_graph[100000] = {0};

  int evtCounter = 1;
  while (myReader.Next()) {
    if ( abs(*iEta) >= 30 ) continue; // don't consider HF
    if ( *Charge > 7000 && *ADC > 50) { // about 3 GeV, this is the charge for the full digi pulse shape
      depth_all_ieta[*Depth]->Fill(*TDC1);
      depth_all_ieta_tdc0[*Depth]->Fill(*TDC0);
      if (*TDC0 < 50) depth_all_ieta_tdc01[*Depth]->Fill(*TDC0);
      depth_all_ieta_tdc01[*Depth]->Fill(*TDC1+50);
      depth_by_ieta[*Depth][abs(*iEta)]->Fill(*TDC1);
      depth_by_ieta_tdc0[*Depth][abs(*iEta)]->Fill(*TDC0);
    }
    if ( *ADC > 33) eta_depth_tdc[static_cast<int>(abs(*iEta))][static_cast<int>(*Depth)][static_cast<int>(*TDC1)] += 1; // at each ieta, depth, count how many cells with TDC of each value
    if (abs(*iEta) >= 16) {
      if (*TDC1 == 62) HE62->Fill(*Depth);
      HE->Fill(*Depth);
    }
    if (evtCounter < 100000) {
      TDC_graph[evtCounter] = static_cast<int>(*TDC1);
      ADC_graph[evtCounter] = static_cast<int>(*Charge);
    }

    evtCounter++;
  }

  // HE TDC = 62 rate plots
  TCanvas *cHE = new TCanvas();
  if (TEfficiency::CheckConsistency(*HE62,*HE)) {
    TEfficiency *effHE = new TEfficiency(*HE62,*HE);
    effHE->SetTitle("Error Code TDC=62 Rates in HE, 2018A data");
    effHE->SetLineWidth(3.);
    effHE->SetLineColor(kBlack);
    effHE->Draw();
    gPad->Update();
    effHE->GetPaintedGraph()->SetMaximum(0.0005);
    effHE->GetPaintedGraph()->SetMinimum(0.);
    gPad->Update();
    cHE->SaveAs("2018A_plots/HE_TDCerror62_zoom2_2018A.png");
    effHE->GetPaintedGraph()->SetMaximum(0.005);
    cHE->SaveAs("2018A_plots/HE_TDCerror62_zoom_2018A.png");
    gPad->Update();
    effHE->GetPaintedGraph()->SetMaximum(0.05);
    gPad->Update();
  }
  cHE->SaveAs("2018A_plots/HE_TDCerror62_2018A.png");

  // ADC vs TDC plot
  TGraph *gr_adc = new TGraph(100000, ADC_graph, TDC_graph);
  TCanvas *c1_adc = new TCanvas();
  gr_adc->SetTitle(Form("Charge (fC) vs TDC, 2018A"));
  gr_adc->GetYaxis()->SetTitle("TDC value, 0.5 ns steps");
  gr_adc->GetXaxis()->SetTitle("Charge in fC, 7k fC = 3 GeV");
  gr_adc->Draw("AP");
  gr_adc->SetMarkerStyle(20);
  gr_adc->SetMarkerSize(0.5);
  c1_adc->SaveAs(Form("2018A_plots/TDC_vs_ADC_full_2018A.png")); 
  gr_adc->SetMaximum(30);
  gr_adc->SetMinimum(0);
  gr_adc->GetXaxis()->SetLimits(0,200);
  gr_adc->Draw("P");
  c1_adc->SaveAs(Form("2018A_plots/TDC_vs_ADC_2018A.png")); 

  TCanvas* c2 = new TCanvas("c2","TDC Distribution, 2018A",0,0,400,300);
  //  gStyle->SetOptStat(0);
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
  depth_all_ieta[1]->SetTitle("TDC Distribution, 2018A");   
  depth_all_ieta[1]->GetXaxis()->SetTitle("TDC value in SOI, 1/2 ns steps");
  depth_all_ieta[1]->GetYaxis()->SetTitle("Entries");

  c2->SaveAs("2018A_plots/TDC_by_depth.pdf");
  c2->SetLogy();
  c2->SaveAs("2018A_plots/TDC_by_depth_log.pdf");

  // soi-1 
  TCanvas* c2_tdc0 = new TCanvas("c2_tdc0","SOI-1 TDC Distribution, 2018A",0,0,400,300);
  TLegend* leg_tdc0 = new TLegend(0.1,0.6,0.5,0.9);
  depth_all_ieta_tdc0[1]->Draw();
  for (int depth = 1; depth <= 7; depth++)
    {
      depth_all_ieta_tdc0[depth]->SetLineColor(kRainBow+depth*6);
      depth_all_ieta_tdc0[depth]->Draw("same");
      leg_tdc0->AddEntry(depth_all_ieta_tdc0[depth],Form("HE Depth %d, mean = %.2f",depth,depth_all_ieta_tdc0[depth]->GetMean()));
    }
  leg_tdc0->Draw("same");
  depth_all_ieta_tdc0[1]->SetTitle("SOI-1 TDC Distribution, 2018A");
  depth_all_ieta_tdc0[1]->GetXaxis()->SetTitle("TDC value in SOI-1, 1/2 ns steps");
  depth_all_ieta_tdc0[1]->GetYaxis()->SetTitle("Entries");
  c2_tdc0->SaveAs("2018A_plots/TDC_by_depth_tdc0.pdf");
  c2_tdc0->SetLogy();
  c2_tdc0->SaveAs("2018A_plots/TDC_by_depth_log_tdc0.pdf");

  // soi-1 and soi
  TCanvas* c2_tdc01 = new TCanvas("c2_tdc01","SOI-1 TDC Distribution, 2018A",0,0,400,300);
  TLegend* leg_tdc01 = new TLegend(0.1,0.6,0.48,0.9);
  depth_all_ieta_tdc01[1]->Draw();
  for (int depth = 1; depth <= 7; depth++)
    {
      depth_all_ieta_tdc01[depth]->SetLineColor(kRainBow+depth*6);
      depth_all_ieta_tdc01[depth]->Draw("same");
      leg_tdc01->AddEntry(depth_all_ieta_tdc01[depth],Form("HE Depth %d, mean = %.2f",depth,depth_all_ieta_tdc01[depth]->GetMean()));
    }
  leg_tdc01->Draw("same");
  depth_all_ieta_tdc01[1]->SetTitle("SOI-1 and SOI TDC Distribution, 2018A");
  depth_all_ieta_tdc01[1]->GetXaxis()->SetTitle("TDC value in SOI-1 and SOI, 1/2 ns steps");
  depth_all_ieta_tdc01[1]->GetYaxis()->SetTitle("Entries");
  c2_tdc01->SaveAs("2018A_plots/TDC_by_depth_tdc01.pdf");
  c2_tdc01->SetLogy();
  c2_tdc01->SaveAs("2018A_plots/TDC_by_depth_log_tdc01.pdf");

  TCanvas* c3 = new TCanvas("c3","TDC Distribution, 2018A",0,0,400,300);
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
    depth_by_ieta[1][ieta]->SetTitle(Form("TDC Distribution at ieta = %d, 2018A (7k+ fC)",ieta));
    depth_by_ieta[1][ieta]->GetXaxis()->SetTitle("TDC value in SOI, 1/2 ns steps");
    depth_by_ieta[1][ieta]->GetYaxis()->SetTitle("Entries");
    if (ieta == 16) {
      depth_by_ieta[4][ieta]->SetTitle(Form("TDC Distribution at ieta = %d, 2018 (7k+ fC)",ieta));
      depth_by_ieta[4][ieta]->GetXaxis()->SetTitle("TDC value in SOI, 1/2 ns steps");
      depth_by_ieta[4][ieta]->GetYaxis()->SetTitle("Entries");
    }
    if (ieta == 17 || ieta == 18) {
      depth_by_ieta[2][ieta]->SetTitle(Form("TDC Distribution at ieta = %d, 2018 (7k+ fC)",ieta));
      depth_by_ieta[2][ieta]->GetXaxis()->SetTitle("TDC value in SOI, 1/2 ns steps");
      depth_by_ieta[2][ieta]->GetYaxis()->SetTitle("Entries");    
    }
    c3->SaveAs(Form("2018A_plots/TDC_by_depth_ieta%d.pdf",ieta));
    c3->SetLogy();
    c3->SaveAs(Form("2018A_plots/TDC_by_depth_ieta%d_log.pdf",ieta));
    c3->Clear();
    c3->SetLogy(0);
    leg2->Clear();
  }


  TCanvas* c3_tdc0 = new TCanvas("c3_tdc0","TDC Distribution, 2018A",0,0,400,300);
  gStyle->SetOptStat(0);
  TLegend* leg2_tdc0 = new TLegend(0.1,0.6,0.5,0.9);
  for (int ieta = 16; ieta < 30; ieta++) {
    if (ieta == 16) depth_by_ieta_tdc0[4][ieta]->Draw();
    if (ieta == 17 || ieta == 18) depth_by_ieta_tdc0[2][ieta]->Draw();
    if (ieta > 18) depth_by_ieta_tdc0[1][ieta]->Draw();
    for (int depth = 1; depth <= 7; depth++)
      {
        depth_by_ieta_tdc0[depth][ieta]->SetLineColor(kRainBow+depth*6);
        depth_by_ieta_tdc0[depth][ieta]->Draw("same");
        leg2_tdc0->AddEntry(depth_by_ieta_tdc0[depth][ieta],Form("HE Depth %d, mean = %.2f",depth,depth_by_ieta_tdc0[depth][ieta]->GetMean()));
      }
    leg2_tdc0->Draw("same");
    depth_by_ieta_tdc0[1][ieta]->SetTitle(Form("SOI-1 TDC Distribution at ieta = %d, 2018A (7k+ fC)",ieta));
    depth_by_ieta_tdc0[1][ieta]->GetXaxis()->SetTitle("TDC value in SOI-1, 1/2 ns steps");
    depth_by_ieta_tdc0[1][ieta]->GetYaxis()->SetTitle("Entries");
    if (ieta == 16) {
      depth_by_ieta_tdc0[4][ieta]->SetTitle(Form("SOI-1 TDC Distribution at ieta = %d, 2018 (7k+ fC)",ieta));
      depth_by_ieta_tdc0[4][ieta]->GetXaxis()->SetTitle("TDC value in SOI-1, 1/2 ns steps");
      depth_by_ieta_tdc0[4][ieta]->GetYaxis()->SetTitle("Entries");
    }
    if (ieta == 17 || ieta == 18) {
      depth_by_ieta_tdc0[2][ieta]->SetTitle(Form("SOI-1 TDC Distribution at ieta = %d, 2018 (7k+ fC)",ieta));
      depth_by_ieta_tdc0[2][ieta]->GetXaxis()->SetTitle("TDC value in SOI-1, 1/2 ns steps");
      depth_by_ieta_tdc0[2][ieta]->GetYaxis()->SetTitle("Entries");
    }
    c3_tdc0->SaveAs(Form("2018A_plots/TDC_by_depth_ieta%d_tdc0.pdf",ieta));
    c3_tdc0->SetLogy();
    c3_tdc0->SaveAs(Form("2018A_plots/TDC_by_depth_ieta%d_log_tdc0.pdf",ieta));
    c3_tdc0->Clear();
    c3_tdc0->SetLogy(0);
    leg2_tdc0->Clear();
  }

  // calculate partial sum / cumulative sum from the TDC distributions
  double sum[30][8] = {{0}}; // indexed by eta, depth
  double partial_sum[30][8][51] = {{{0}}}; // counts partial sum up to each tdc value
  double cumulative_frac[30][8][51] = {{{0}}};

  for (int eta = 0; eta < 30; eta++) {
    for (int depth = 1; depth < 8; depth++) {
      for (int tdc = 0; tdc<=50; tdc++) {
	sum[eta][depth] += eta_depth_tdc[eta][depth][tdc];
	partial_sum[eta][depth][tdc] = sum[eta][depth];
      }
    }
  }
  //  std::cout << partial_sum[0][1][50] << std::endl;

  int bkg90[30][8] = {{0}};
  for (int eta = 0; eta < 30; eta++) {
    for (int depth = 1; depth < 8; depth++) {
      for (int tdc = 0; tdc<=50; tdc++) {
	cumulative_frac[eta][depth][tdc] = (double) ( partial_sum[eta][depth][tdc] / partial_sum[eta][depth][50] );
	if (cumulative_frac[eta][depth][tdc] > 0.9 && bkg90[eta][depth] == 0 ) bkg90[eta][depth] = tdc;
	//	if (eta == 16 && depth == 5) std::cout << cumulative_frac[eta][depth][tdc] << " depth 2 at TDC = " << tdc << " with ps = " << partial_sum[eta][depth][tdc] << std::endl;
      }
    }
  }
  std::ofstream TDCdistribution_Background90;
  TDCdistribution_Background90.open("TDCdistribution_Background90.txt");
  for (int eta = 0; eta < 30; eta++) TDCdistribution_Background90 << eta << ", " << bkg90[eta][1] << ", " << bkg90[eta][2] << ", " << bkg90[eta][3] << ", " << bkg90[eta][4] << ", " << bkg90[eta][5] << ", " << bkg90[eta][6] << ", " << bkg90[eta][7] << std::endl;
  TDCdistribution_Background90.close();



}
