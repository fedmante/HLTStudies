/*
C++ code to draw occupancy plots.
This program read outputTotal.root, that is the union of the .root files created by bin/MakeAlCaPhiSymOccupancyPlot.cpp

To compile: c++ -o finalPlots `root-config --cflags --glibs` finalPlots.cpp TEndcapRings.cc
To run: ./finalPlots.cpp
*/

using namespace std;

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <map>
#include <vector>
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TMath.h"
#include "TH2F.h"
#include "TROOT.h"
#include "TTree.h"
#include "TStyle.h"
#include "TMultiGraph.h"
#include "TPaveStats.h"

#include "TEndcapRings.h"

void drawGraphs(TGraph* g1,TGraph* g2, std::string Title, std::string g1_Title, std::string g2_Title, float xmin, float xmax, float ymin, float ymax);

int main (int argc, char** argv) {

  TFile* f = new TFile("file:outputOccupancy_total.root"); 
  
  TH1F* h_events = (TH1F*)f->Get("h_nEvents");
  TH2F* h_EB = (TH2F*)f->Get("h2_hitOccupancy_EB"); //360 x 170 bins
  TH2F* h_EEM = (TH2F*)f->Get("h2_hitOccupancy_EEM"); //100 x 100 bins
  TH2F* h_EEP = (TH2F*)f->Get("h2_hitOccupancy_EEP"); //100 x 100 bins

  Double_t num_events = h_events->GetBinContent(h_events->FindBin(0));
  cout << "Total number of events = " << num_events << endl;

  h_EB->Scale(1./num_events);
  h_EEM->Scale(1./num_events);
  h_EEP->Scale(1./num_events);
  cout << "Scaling for " << 1./num_events << endl;
  

  //--- draw graphs occupancy per event
  /*
  std::string PU;
  std::string bx;
  std::string yesno;
  std::string multifit;
  cout << "PU?" << endl;
  cin >> PU;
  cout << "bx?" << endl;
  cin >> bx;
  cout << "multifit? (yes/no)" << endl;
  cin >> yesno;
  if (yesno == "yes")
     multifit = "multifit";
  else if (yesno == "no")
    multifit = "no_multifit";
    
  h_EB -> SetTitle(("EB_PU" + PU + "_" + bx + "ns_" + multifit).c_str());
  h_EB -> GetXaxis() -> SetTitle("iphi");
  h_EB -> GetYaxis() -> SetTitle("ieta");
  h_EB -> GetZaxis() -> SetTitle("hits/event");

  h_EB -> GetXaxis() -> SetLabelSize(0.04);
  h_EB -> GetXaxis() -> SetTitleSize(0.05);
  h_EB -> GetXaxis() -> SetTitleOffset(0.9);
  h_EB -> GetYaxis() -> SetLabelSize(0.04);
  h_EB -> GetYaxis() -> SetTitleSize(0.05);
  h_EB -> GetYaxis() -> SetTitleOffset(0.9);
  h_EB -> GetZaxis() -> SetLabelSize(0.04);
  h_EB -> GetZaxis() -> SetTitleSize(0.05);
  h_EB -> GetZaxis() -> SetTitleOffset(1.2);
  h_EB -> GetZaxis() -> SetRangeUser(0,0.001);
  h_EB -> SetStats(0);

  h_EEM -> SetTitle(("EEM_PU" + PU + "_" + bx + "ns_" + multifit).c_str());
  h_EEM -> GetXaxis() -> SetTitle("ix");
  h_EEM -> GetYaxis() -> SetTitle("iy");
  h_EEM -> GetZaxis() -> SetTitle("hits/event");

  h_EEM -> GetXaxis() -> SetLabelSize(0.04);
  h_EEM -> GetXaxis() -> SetTitleSize(0.05);
  h_EEM -> GetXaxis() -> SetTitleOffset(0.9);
  h_EEM -> GetYaxis() -> SetLabelSize(0.04);
  h_EEM -> GetYaxis() -> SetTitleSize(0.05);
  h_EEM -> GetYaxis() -> SetTitleOffset(0.9);
  h_EEM -> GetZaxis() -> SetLabelSize(0.04);
  h_EEM -> GetZaxis() -> SetTitleSize(0.05);
  h_EEM -> GetZaxis() -> SetTitleOffset(1.);
  h_EEM -> GetZaxis() -> SetRangeUser(0,0.0002);
  h_EEM -> SetStats(0);

  h_EEP -> SetTitle(("EEP_PU" + PU + "_" + bx + "ns_" + multifit).c_str());
  h_EEP -> GetXaxis() -> SetTitle("ix");
  h_EEP -> GetYaxis() -> SetTitle("iy");
  h_EEP -> GetZaxis() -> SetTitle("hits/event");
  h_EEP -> GetXaxis() -> SetLabelSize(0.04);
  h_EEP -> GetXaxis() -> SetTitleSize(0.05);
  h_EEP -> GetXaxis() -> SetTitleOffset(0.9);
  h_EEP -> GetYaxis() -> SetLabelSize(0.04);
  h_EEP -> GetYaxis() -> SetTitleSize(0.05);
  h_EEP -> GetYaxis() -> SetTitleOffset(0.9);
  h_EEP -> GetZaxis() -> SetLabelSize(0.04);
  h_EEP -> GetZaxis() -> SetTitleSize(0.05);
  h_EEP -> GetZaxis() -> SetTitleOffset(1.);
  h_EEP -> GetZaxis() -> SetRangeUser(0,0.0002);
  h_EEP -> SetStats(0);

  TCanvas* c1 = new TCanvas("c1","c1");
  c1->SetRightMargin(0.2);
  c1 -> cd();
  h_EB -> Draw("colz");
  c1 -> Print(("EB_PU" + PU + "_" + bx + "ns_" + multifit + ".png").c_str(), "png");
  c1 -> Print(("EB_PU" + PU + "_" + bx + "ns_" + multifit + ".pdf").c_str(), "pdf"); 
  
  TCanvas* c2 = new TCanvas("c2","c2");
  c2->SetRightMargin(0.2);
  c2 -> cd();
  h_EEM -> Draw("colz");
  c2 -> Print(("EEM_PU" + PU + "_" + bx + "ns_" + multifit + ".png").c_str(), "png");
  c2 -> Print(("EEM_PU" + PU + "_" + bx + "ns_" + multifit + ".pdf").c_str(), "pdf"); 
  
  TCanvas* c3 = new TCanvas("c3","c3");
  c3->SetRightMargin(0.2);
  c3 -> cd();
  h_EEP -> Draw("colz");
  c3 -> Print(("EEP_PU" + PU + "_" + bx + "ns_" + multifit + ".png").c_str(), "png");
  c3 -> Print(("EEP_PU" + PU + "_" + bx + "ns_" + multifit + ".pdf").c_str(), "pdf"); 
  */
  //---computation of mean hits per event and of lower occupancy for EB, EEM, EEP
 
  Double_t EB_mean = 0.;
  Double_t EEM_mean= 0.;
  Double_t EEP_mean = 0.;
  Double_t EB_lowerOccupancy = 10.;
  Double_t EEM_lowerOccupancy = 10.;
  Double_t EEP_lowerOccupancy = 10.;

  int EB_channels = 0;
  for (int i=1; i <= h_EB->GetNbinsX(); i++) {
    for (int j=1; j <= h_EB->GetNbinsY(); j++) {
      if (h_EB->GetBinContent(h_EB->FindBin(i,j)) > 0) { //find mean occupancy
	EB_channels = EB_channels + 1;
	EB_mean = EB_mean + h_EB->GetBinContent(h_EB->FindBin(i,j));
      }
      
      if (h_EB->GetBinContent(h_EB->FindBin(i,j)) < EB_lowerOccupancy && h_EB->GetBinContent(h_EB->FindBin(i,j)) != 0) {  //Find lower occupancy
	EB_lowerOccupancy = h_EB->GetBinContent(h_EB->FindBin(i,j));
      }
    }
  }
  EB_mean = EB_mean/EB_channels;

 int EEM_channels = 0;
  for (int i=1; i <= h_EEM->GetNbinsX(); i++) {
    for (int j=1; j <= h_EEM->GetNbinsY(); j++) {
      if (h_EEM->GetBinContent(h_EEM->FindBin(i,j)) > 0) { //find lower occupancy
	EEM_channels = EEM_channels + 1;
	EEM_mean = EEM_mean + h_EEM->GetBinContent(h_EEM->FindBin(i,j));
      }
      if (h_EEM->GetBinContent(h_EEM->FindBin(i,j)) < EEM_lowerOccupancy && h_EEM->GetBinContent(h_EEM->FindBin(i,j)) != 0) //find lower occupancy
	EEM_lowerOccupancy = h_EEM->GetBinContent(h_EEM->FindBin(i,j));
    }
  }
  EEM_mean = EEM_mean/EEM_channels;

 int EEP_channels = 0;
  for (int i=1; i <= h_EEP->GetNbinsX(); i++) {
    for (int j=1; j <= h_EEP->GetNbinsY(); j++) {
      if (h_EEP->GetBinContent(h_EEP->FindBin(i,j)) > 0) { //find mean occupancy
	EEP_channels = EEP_channels + 1;
	EEP_mean = EEP_mean + h_EEP->GetBinContent(h_EEP->FindBin(i,j));
      }
      if (h_EEP->GetBinContent(h_EEP->FindBin(i,j)) < EEP_lowerOccupancy && h_EEP->GetBinContent(h_EEP->FindBin(i,j)) != 0) //find lower occupancy
	EEP_lowerOccupancy = h_EEP->GetBinContent(h_EEP->FindBin(i,j));
    }
  }
  EEP_mean = EEP_mean/EEP_channels;

  cout << "EB mean occupancy = " << EB_mean << endl;
  cout << "EEM mean occupancy = " << EEM_mean << endl;
  cout << "EEP mean occupancy = " << EEP_mean << endl;
  
  cout << "EB lower occupancy = " << EB_lowerOccupancy << endl;
  cout << "EEM lower occupancy = " << EEM_lowerOccupancy << endl;
  cout << "EEP lower occupancy = " << EEP_lowerOccupancy << endl;
  
  //--- draw graphs mean occupancy vs iRing

  TFile *outputFile = new TFile ("occupancy_rings.root","RECREATE");
  std::cout << "New file created: occupancy_rings.root" << std::endl; 

  static const int EB_rings = 85;
  static const int EE_rings = 39;

  TEndcapRings *eRings = new TEndcapRings(); 

  Int_t nBins = 100;   
  Double_t occupancyMin = 0.;
  Double_t occupancyMax = 0.1;
  Double_t meanOccupancy = 0.;

  std::vector<TH1F*> EBM_occupancy_histos;
  std::vector<TH1F*> EBP_occupancy_histos;
  std::vector<TH1F*> EEM_occupancy_histos;
  std::vector<TH1F*> EEP_occupancy_histos;

  EBM_occupancy_histos.resize(EB_rings);
  EBP_occupancy_histos.resize(EB_rings);
  EEM_occupancy_histos.resize(EE_rings);
  EEP_occupancy_histos.resize(EE_rings);
  
  ostringstream t;

  for (int i=0; i<EB_rings; i++) { //EB-
    t << "EBM_occupancy_" << i+1;
    EBM_occupancy_histos[i]=new TH1F(t.str().c_str(),";mean occupancy",nBins,occupancyMin,occupancyMax); 
    t.str("");
  }

  for (int i=0; i<EB_rings; i++) { //EB+
    t << "EBP_occupancy_" << i+1;
    EBP_occupancy_histos[i]=new TH1F(t.str().c_str(),";mean occupancy",nBins,occupancyMin,occupancyMax); 
    t.str("");
  }

  for (int i=0; i<EE_rings; i++) { //EE-
    t << "EEM_occupancy_" << i+1;
    EEM_occupancy_histos[i]=new TH1F(t.str().c_str(),";mean occupancy",nBins,occupancyMin,occupancyMax); 
    t.str("");
  }

  for (int i=0; i<EE_rings; i++) { //EE+
    t << "EEP_occupancy_" << i+1;
    EEP_occupancy_histos[i]=new TH1F(t.str().c_str(),";mean occupancy",nBins,occupancyMin,occupancyMax); 
    t.str("");
  }
  
  //fill histos
  for (int iphi=0; iphi<361; iphi++) { //EB-
    for (int ieta=-85; ieta<0; ieta++) {
      if ( h_EB->GetBinContent(h_EB->FindBin(iphi,ieta)) != 0 )
	EBM_occupancy_histos[ieta+85]->Fill( h_EB->GetBinContent(h_EB->FindBin(iphi,ieta)) ); 
    }
  }
  
  for (int iphi=0; iphi<361; iphi++) { //EB+
    for (int ieta=1; ieta<86; ieta++) {
      if ( h_EB->GetBinContent(h_EB->FindBin(iphi,ieta)) != 0 )
	EBP_occupancy_histos[ieta-1]->Fill( h_EB->GetBinContent(h_EB->FindBin(iphi,ieta)) ); 
    }
  }

  for (int ix=1; ix<101; ix++) { //EE-
    for (int iy=1; iy<101; iy++) {
      int iring = eRings->GetEndcapRing(ix,iy,-1);
      if ( h_EEM->GetBinContent(h_EEM->FindBin(ix,iy)) != 0 )
	EEM_occupancy_histos[iring]->Fill( h_EEM->GetBinContent(h_EEM->FindBin(ix,iy)) ); 
    }
  }

  for (int ix=1; ix<101; ix++) { //EE+
    for (int iy=1; iy<101; iy++) {
      int iring = eRings->GetEndcapRing(ix,iy,-1);
      if ( h_EEP->GetBinContent(h_EEP->FindBin(ix,iy)) != 0 )
	EEP_occupancy_histos[iring]->Fill( h_EEP->GetBinContent(h_EEP->FindBin(ix,iy)) ); 
    }
  }

  //write histo
  outputFile->cd();

  for(int i=0;i<EB_rings;i++){
    EBM_occupancy_histos[i]->Write();
    EBP_occupancy_histos[i]->Write();
  }

  for(int i=0;i<EE_rings;i++){
    EEM_occupancy_histos[i]->Write();
    EEP_occupancy_histos[i]->Write();
  }

  //graphs
  TGraph *EBM_meanOccupancy = new TGraph();
  TGraph *EBP_meanOccupancy = new TGraph(); 
  TGraph *EEM_meanOccupancy = new TGraph(); 
  TGraph *EEP_meanOccupancy = new TGraph(); 
        
  for(int i = 0; i < EB_rings; i++) { //EB-
    meanOccupancy = EBM_occupancy_histos[i]->GetMean();      
    EBM_meanOccupancy->SetPoint(i,i+1,meanOccupancy);
  }

  for(int i = 0; i < EB_rings; i++) { //EB+
    meanOccupancy = EBP_occupancy_histos[i]->GetMean();      
    EBP_meanOccupancy->SetPoint(i,i+1,meanOccupancy);
  }

  for(int i = 0; i < EE_rings; i++) { //EE-
    meanOccupancy = EEM_occupancy_histos[i]->GetMean();      
    EEM_meanOccupancy->SetPoint(i,i+1,meanOccupancy);
  }

  for(int i = 0; i < EE_rings; i++) { //EE+
    meanOccupancy = EEP_occupancy_histos[i]->GetMean();      
    EEP_meanOccupancy->SetPoint(i,i+1,meanOccupancy);
  }

  outputFile->Close();

  //draw graphs   
  drawGraphs(EBM_meanOccupancy,EBP_meanOccupancy,std::string("EB_mean_occupancy"),std::string("EBM"),std::string("EBP"),0,87,0,0.002); 
  drawGraphs(EEM_meanOccupancy,EEP_meanOccupancy,std::string("EE_mean_occupancy"),std::string("EEM"),std::string("EEP"),0,40,0,0.002);


} //main


void drawGraphs(TGraph* g1,TGraph* g2, std::string Title, std::string g1_Title, std::string g2_Title, float xmin, float xmax, float ymin, float ymax) {
  
  gStyle -> SetOptFit (00111);
  gStyle -> SetOptStat ("");
  gStyle -> SetStatX (.90);
  gStyle -> SetStatY (.90);
  gStyle -> SetStatW (.15);
     
  g1 -> SetTitle(Title.c_str());
  g1 -> GetXaxis() -> SetLabelSize(0.04);
  g1 -> GetYaxis() -> SetLabelSize(0.04);
  g1 -> GetXaxis() -> SetTitleSize(0.05);
  g1 -> GetYaxis() -> SetTitleSize(0.05);
  g1 -> GetYaxis() -> SetTitleOffset(1.2);
  g1 -> GetYaxis() -> SetRangeUser(ymin,ymax);
  g1 -> GetXaxis() -> SetRangeUser(xmin,xmax);
 
  g1 -> GetXaxis() -> SetTitle("iRing");
  g1 -> GetYaxis() -> SetTitle("mean occupancy");
   
  g1 -> SetMarkerStyle(20);
  g1 -> SetMarkerSize(0.6);
  g1 -> SetMarkerColor(kBlack);
  g1 -> SetLineColor(kBlack);
  g1 -> SetLineWidth(1.8);

  g2 -> SetMarkerStyle(20);
  g2 -> SetMarkerSize(0.6);
  g2 -> SetMarkerColor(kBlue);
  g2 -> SetLineColor(kBlue);
  g2 -> SetLineWidth(1.8);

  TLegend* legend = new TLegend(.75, 0.84, 1., 0.96);
  legend -> SetFillColor(kWhite);
  legend -> SetFillStyle(1000);
  legend -> SetLineWidth(0); 
  legend -> SetLineColor(kWhite);
  legend -> SetTextFont(42);
  legend -> SetTextSize(0.04);
  legend -> AddEntry(g1,g1_Title.c_str(),"L");
  legend -> AddEntry(g2,g2_Title.c_str(),"L");
 
  TCanvas* c1 = new TCanvas("c1","c1");
  c1 -> cd();

  g1 -> Draw("APL");
  g2 -> Draw("PL");
  legend -> Draw("same");
  
  c1 -> Print((Title+".png").c_str(),"png");
  c1 -> Print((Title+".pdf").c_str(),"pdf");
    
  delete c1;

}