#include "MakeAlCaPhiSymSpectra.h"
#include "HLTStudies/AlCaPhiSymStudies/interface/TEndcapRings.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Common/interface/EventBase.h"
#include "DataFormats/Provenance/interface/EventAuxiliary.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"
#include "SimDataFormats/CrossingFrame/interface/MixCollection.h"
#include "SimDataFormats/CrossingFrame/interface/CrossingFramePlaybackInfoExtended.h"
#include "SimDataFormats/CrossingFrame/interface/CrossingFrame.h"
#include "SimDataFormats/CrossingFrame/interface/PCrossingFrame.h"

#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbService.h"
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbRecord.h"

#include "CondFormats/EcalObjects/interface/EcalIntercalibConstants.h"
#include "CondFormats/DataRecord/interface/EcalIntercalibConstantsRcd.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "Geometry/EcalAlgo/interface/EcalBarrelGeometry.h"
#include "Geometry/EcalAlgo/interface/EcalEndcapGeometry.h"

#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

#include <fstream>
#include <iostream>
#include <memory>

MakeAlCaPhiSymSpectra::MakeAlCaPhiSymSpectra(const edm::ParameterSet& ps){

  recHitCollection_EB_                   = ps.getParameter<edm::InputTag>("recHitCollection_EB");
  recHitCollection_EE_                   = ps.getParameter<edm::InputTag>("recHitCollection_EE");

  /*EB_rings = 85;
  EE_rings = 39;

  nBins = 1000;*/
  enMin = 0.;
  enMax = 8.;
  calMin = 0.;
  calMax = 20;
  
  naiveId_ = 0;

  edm::Service<TFileService> fs;
  h_nEvents = fs->make<TH1F>("h_nEvents","h_nEvents",3,-1,2);

  EBM_eSpectrum_histos.resize(EB_rings);
  EBP_eSpectrum_histos.resize(EB_rings);
  EEM_eSpectrum_histos.resize(EE_rings);
  EEP_eSpectrum_histos.resize(EE_rings);

  EBM_etSpectrum_histos.resize(EB_rings);
  EBP_etSpectrum_histos.resize(EB_rings);
  EEM_etSpectrum_histos.resize(EE_rings);
  EEP_etSpectrum_histos.resize(EE_rings);

  EBM_calibration_histos.resize(EB_rings);
  EBP_calibration_histos.resize(EB_rings);
  EEM_calibration_histos.resize(EE_rings);
  EEP_calibration_histos.resize(EE_rings);

  std::ostringstream t;
  for (int i=0; i<EB_rings; i++) { //EB

    t << "EBM_eSpectrum_" << i+1;
    EBM_eSpectrum_histos[i] = fs->make<TH1F>(t.str().c_str(),t.str().c_str(),nBins,enMin,enMax); 
    t.str("");
    t << "EBM_etSpectrum_" << i+1;
    EBM_etSpectrum_histos[i] = fs->make<TH1F>(t.str().c_str(),t.str().c_str(),nBins,enMin,enMax); 
    t.str("");
    t << "EBM_calibration_" << i+1;
    EBM_calibration_histos[i] = fs->make<TH1F>(t.str().c_str(),t.str().c_str(),nBins,calMin,calMax); 
    t.str("");

    t << "EBP_eSpectrum_" << i+1;
    EBP_eSpectrum_histos[i] = fs->make<TH1F>(t.str().c_str(),t.str().c_str(),nBins,enMin,enMax); 
    t.str("");
    t << "EBP_etSpectrum_" << i+1;
    EBP_etSpectrum_histos[i] = fs->make<TH1F>(t.str().c_str(),t.str().c_str(),nBins,enMin,enMax); 
    t.str("");
    t << "EBP_calibration_" << i+1;
    EBM_calibration_histos[i] = fs->make<TH1F>(t.str().c_str(),t.str().c_str(),nBins,enMin,enMax); 
    t.str("");

  }

  for (int i=0; i<EE_rings; i++) { //EE
    t << "EEM_eSpectrum_" << i+1;
    EEM_eSpectrum_histos[i] = fs->make<TH1F>(t.str().c_str(),t.str().c_str(),nBins,enMin,enMax); 
    t.str("");
    t << "EEM_etSpectrum_" << i+1;
    EEM_etSpectrum_histos[i] = fs->make<TH1F>(t.str().c_str(),t.str().c_str(),nBins,enMin,enMax); 
    t.str("");
    t << "EEM_calibration_" << i+1;
    EEM_calibration_histos[i] = fs->make<TH1F>(t.str().c_str(),t.str().c_str(),nBins,calMin,calMax); 
    t.str("");

    t << "EEP_eSpectrum_" << i+1;
    EEP_eSpectrum_histos[i] = fs->make<TH1F>(t.str().c_str(),t.str().c_str(),nBins,enMin,enMax); 
    t.str("");
    t << "EEP_etSpectrum_" << i+1;
    EEP_etSpectrum_histos[i] = fs->make<TH1F>(t.str().c_str(),t.str().c_str(),nBins,enMin,enMax); 
    t.str("");
    t << "EEP_calibration_" << i+1;
    EEP_calibration_histos[i] = fs->make<TH1F>(t.str().c_str(),t.str().c_str(),nBins,calMin,calMax); 
    t.str("");

  }
  
}
MakeAlCaPhiSymSpectra::~MakeAlCaPhiSymSpectra()
{
}

// ------------ method called once each job just before starting event loop  ------------

void MakeAlCaPhiSymSpectra::beginJob()
{
}

// ------------ method called to for each event  ------------
void MakeAlCaPhiSymSpectra::analyze(const edm::Event& ev, const edm::EventSetup& iSetup)
{
  naiveId_++;
  std::cout << "Event = " << naiveId_ << std::endl;
  
  using namespace edm;
  
  //LaserCorrections
  edm::ESHandle<EcalLaserDbService> theLaser;
  iSetup.get<EcalLaserDbRecord>().get(theLaser);
  
  //InterCalibration constants
  edm::ESHandle<EcalIntercalibConstants> pIcal;
  iSetup.get<EcalIntercalibConstantsRcd>().get(pIcal);
  //const EcalIntercalibConstants* Mcal = pIcal.product();

  const edm::Timestamp& evtTimeStamp = edm::Timestamp(0);

  TEndcapRings *eRings = new TEndcapRings(); 

  //rechitsEB
  edm::Handle<EcalRecHitCollection> recHitsEB;
  ev.getByLabel( recHitCollection_EB_, recHitsEB );
  const EcalRecHitCollection* theBarrelEcalRecHits = recHitsEB.product () ;
  if ( ! recHitsEB.isValid() ) {
    std::cerr << "EcalValidation::analyze --> recHitsEB not found" << std::endl; 
  }
  
  EBRecHitCollection::const_iterator itb;
  for (itb = theBarrelEcalRecHits->begin(); itb != theBarrelEcalRecHits->end(); ++itb)
    {
      EBDetId id_crystal(itb->id());

      float ieta = id_crystal.ieta();
      float eta = eRings->GetEtaFromIRing (ieta);
            
      float e  = itb->energy();
      float et = itb->energy()/cosh(eta);
      
      float LaserCorrection = theLaser->getLaserCorrection(id_crystal, evtTimeStamp);
      float InterCalibConst = 1.; // ??
      float ADCToGeV_EB = 1.; // ??
      float Calibration = LaserCorrection * InterCalibConst * ADCToGeV_EB;

      if (ieta < 0) { //EBM
	EBM_eSpectrum_histos[ieta+85]->Fill(e);
	EBM_etSpectrum_histos[ieta+85]->Fill(et);
	EBM_calibration_histos[ieta+85]->Fill(Calibration);
      }
      else if (ieta > 0) { //EBP
	EBP_eSpectrum_histos[ieta-1]->Fill(e);
	EBP_etSpectrum_histos[ieta-1]->Fill(et);
	EBP_calibration_histos[ieta-1]->Fill(Calibration);
      }
    }

  //rechitsEE
  edm::Handle<EcalRecHitCollection> recHitsEE;
  ev.getByLabel( recHitCollection_EE_, recHitsEE );
  const EcalRecHitCollection* theEndcapEcalRecHits = recHitsEE.product () ;
  if ( ! recHitsEE.isValid() ) {
    std::cerr << "EcalValidation::analyze --> recHitsEE not found" << std::endl; 
  }
 
  EERecHitCollection::const_iterator ite;
  for (ite = theEndcapEcalRecHits->begin(); ite != theEndcapEcalRecHits->end(); ++ite)
    {
      EEDetId id_crystal(ite->id());

      float LaserCorrection = theLaser->getLaserCorrection(id_crystal, evtTimeStamp);
      float InterCalibConst = 1.; // ??
      float ADCToGeV_EE = 1.; // ??
      float Calibration = LaserCorrection * InterCalibConst * ADCToGeV_EE;
       
      float iring = eRings->GetEndcapRing( id_crystal.ix(), id_crystal.iy(), id_crystal.zside() );
      float eta = eRings->GetEtaFromIRing (iring);
      
      float e  = ite->energy();
      float et = ite->energy()/cosh(eta);

      if (id_crystal.zside() < 0) { //EEM
	EEM_eSpectrum_histos[iring]->Fill(e);
	EEM_etSpectrum_histos[iring]->Fill(et);
	EEM_calibration_histos[iring]->Fill(Calibration);
      }
      else if (id_crystal.zside() > 0) { //EEP
	EEP_eSpectrum_histos[iring]->Fill(e);
	EEP_etSpectrum_histos[iring]->Fill(et);
	EEP_calibration_histos[iring]->Fill(Calibration);
      }
    }  

}
  

// ------------ method called once each job just after ending the event loop  ------------
void MakeAlCaPhiSymSpectra::endJob()
{
  h_nEvents->SetBinContent(h_nEvents->FindBin(0),naiveId_); 
}


//define this as a plug-in
DEFINE_FWK_MODULE(MakeAlCaPhiSymSpectra);
