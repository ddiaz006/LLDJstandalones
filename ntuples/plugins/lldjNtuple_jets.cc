#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "LLDJstandalones/ntuples/interface/lldjNtuple.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "CLHEP/Random/RandomEngine.h"
#include "CLHEP/Random/RandGauss.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "RecoTracker/DebugTools/interface/GetTrackTrajInfo.h"

#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "TrackingTools/GeomPropagators/interface/Propagator.h"
#include "TrackingTools/GeomPropagators/interface/StateOnTrackerBound.h"
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"

#include "PhysicsTools/RecoUtils/interface/CheckHitPattern.h"
#include "RecoVertex/ConfigurableVertexReco/interface/ConfigurableVertexReconstructor.h"
#include "RecoVertex/VertexTools/interface/VertexCompatibleWithBeam.h"
#include "RecoVertex/VertexTools/interface/VertexDistanceXY.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/KinematicFit/interface/KinematicParticleVertexFitter.h"
#include "RecoVertex/KinematicFit/interface/KinematicParticleFitter.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicParticle.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicParticleFactoryFromTransientTrack.h"
#include "RecoVertex/KinematicFitPrimitives/interface/RefCountedKinematicParticle.h"
#include "RecoVertex/VertexPrimitives/interface/ConvertToFromReco.h"
#include "RecoVertex/VertexPrimitives/interface/ConvertError.h"


using namespace std;
typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;

// miniAOD collection handles
edm::Handle<edm::View<pat::Jet> >   slimmedJetHandle;
edm::Handle<double>                 rhoHandle;
edm::Handle<reco::VertexCollection> vtxHandle;
// AOD Collection Handles
edm::Handle<edm::View<reco::CaloJet> >  AODak4CaloJetsHandle;   
edm::Handle<edm::View<reco::PFJet>   >  AODak4PFJetsHandle;     
edm::Handle<edm::View<reco::PFJet>   >  AODak4PFJetsCHSHandle;  
edm::Handle<edm::View<reco::Vertex>  >  AODVertexHandle;
edm::Handle<edm::View<reco::Track>   >  AODTrackHandle;
edm::Handle<reco::BeamSpot> beamspotHandle_;
edm::ESHandle<MagneticField> magneticField;
// transient tracks
map<reco::TransientTrack,reco::TrackBaseRef> refMap;
vector<TrajectoryStateOnSurface> tsosList;

// Global stuff for displaced vertices
ConfigurableVertexReconstructor* vtxfitter_; 
VertexDistanceXY vertexDistanceXY_;
VertexCompatibleWithBeam* vertexBeam_ = new VertexCompatibleWithBeam(vertexDistanceXY_,100);

// ak4 slimmedJets
Int_t          nSlimmedJets_;
vector<float>  slimmedJetPt_;
vector<float>  slimmedJetEn_;
vector<float>  slimmedJetEta_;
vector<float>  slimmedJetPhi_;
vector<float>  slimmedJetRawPt_;
vector<float>  slimmedJetRawEn_;
vector<float>  slimmedJetMt_;
vector<float>  slimmedJetArea_;
vector<float>  slimmedJetLeadTrackPID_;
vector<float>  slimmedJetLeadTrackPt_;
vector<float>  slimmedJetLeadTrackEta_;
vector<float>  slimmedJetLeadTrackPhi_;
vector<int>    slimmedJetLepTrackPID_;
vector<float>  slimmedJetLepTrackPt_;
vector<float>  slimmedJetLepTrackEta_;
vector<float>  slimmedJetLepTrackPhi_;
vector<float>  slimmedJetCHF_;
vector<float>  slimmedJetNHF_;
vector<float>  slimmedJetCEF_;
vector<float>  slimmedJetNEF_;
vector<int>    slimmedJetNCH_;
vector<int>    slimmedJetNNP_;
vector<float>  slimmedJetMUF_;
vector<float>  slimmedJetHFHAE_;
vector<float>  slimmedJetHFEME_;
vector<int>    slimmedJetNConstituents_;
vector<float>  slimmedJetVtxPt_;
vector<float>  slimmedJetVtxMass_;
vector<float>  slimmedJetVtxNtrks_;
vector<float>  slimmedJetVtx3DVal_;
vector<float>  slimmedJetVtx3DSig_;
vector<float>  slimmedJetCSV2BJetTags_; // recommended
vector<float>  slimmedJetJetProbabilityBJetTags_;
vector<float>  slimmedJetpfCombinedMVAV2BJetTags_;
vector<int>    slimmedJetPartonID_;
vector<int>    slimmedJetHadFlvr_;
vector<bool>   slimmedJetPFLooseId_;
vector<int>    slimmedJetID_; 
vector<float>  slimmedJetPUID_;
vector<int>    slimmedJetPUFullID_;
vector<float>  slimmedJetJECUnc_;
vector<float>  slimmedJetP4Smear_;
vector<float>  slimmedJetP4SmearUp_;
vector<float>  slimmedJetP4SmearDo_;
vector<UInt_t> slimmedJetFiredTrgs_;

// Displaced Jet Variables
vector<float>  slimmedJetAlphaD_;
vector<float>  slimmedJetAlphaMaxD_;
vector<float>  slimmedJetSumIP_;
vector<float>  slimmedJetSumIPSig_;
vector<float>  slimmedJetLog10IPSig_;
vector<float>  slimmedJetMedianLog10IPSig_;
vector<float>  slimmedJetTrackAngle_;
vector<float>  slimmedJetLogTrackAngle_;
vector<float>  slimmedJetMedianLog10TrackAngle_;
vector<float>  slimmedJetTotalTrackAngle_;
vector<float>  slimmedJetDauVertex_x_;
vector<float>  slimmedJetDauVertex_y_;
vector<float>  slimmedJetDauVertex_z_;
vector<float>  slimmedJetDauVertex_r_;

// Alpha Maxs
vector<float>  slimmedJetAlphaMax_PV3onPV2_ ; 
vector<float>  slimmedJetAlphaMax_PV3onChg_ ; 
vector<float>  slimmedJetAlphaMax_PV3onAll_ ; 
vector<float>  slimmedJetAlphaMax_PV2onChg_ ; 
vector<float>  slimmedJetAlphaMax_PV2onAll_ ; 

vector<float>  slimmedJetAlpha2Max_PV3onPV2_ ; 
vector<float>  slimmedJetAlpha2Max_PV3onChg_ ; 
vector<float>  slimmedJetAlpha2Max_PV3onAll_ ; 
vector<float>  slimmedJetAlpha2Max_PV2onChg_ ; 
vector<float>  slimmedJetAlpha2Max_PV2onAll_ ; 

// Track Info
vector<vector<float>> slimmedJetTrackPt_;
vector<vector<float>> slimmedJetTrackEta_;
vector<vector<float>> slimmedJetTrackPhi_;
vector<vector<int>>   slimmedJetTrackPDGID_;
vector<vector<int>>   slimmedJetTrackMomPDGID_;

//gen-info for ak4
vector<float>  slimmedJetGenJetEn_;
vector<float>  slimmedJetGenJetPt_;
vector<float>  slimmedJetGenJetEta_;
vector<float>  slimmedJetGenJetPhi_;
vector<int>    slimmedJetGenPartonID_;
vector<float>  slimmedJetGenEn_;
vector<float>  slimmedJetGenPt_;
vector<float>  slimmedJetGenEta_;
vector<float>  slimmedJetGenPhi_;
vector<int>    slimmedJetGenPartonMomID_;

// AOD ---------------------------------------------
// AOD variables
// Calo Jets
Int_t          AODnCaloJet_;
vector<float>  AODCaloJetPt_;
vector<float>  AODCaloJetEta_;
vector<float>  AODCaloJetPhi_;

vector<float>  AODCaloJetAlphaMax_;
vector<float>  AODCaloJetAlphaMax2_;
vector<float>  AODCaloJetAlphaMaxPrime_;
vector<float>  AODCaloJetAlphaMaxPrime2_;
vector<float>  AODCaloJetBeta_;
vector<float>  AODCaloJetBeta2_;

vector<float>  AODCaloJetSumIP_;
vector<float>  AODCaloJetSumIPSig_;
//vector<float>  AODCaloJetLog10IPSig_;
vector<float>  AODCaloJetMedianIP_;
vector<float>  AODCaloJetMedianLog10IPSig_;
vector<float>  AODCaloJetTrackAngle_;
vector<float>  AODCaloJetLogTrackAngle_;
vector<float>  AODCaloJetMedianLog10TrackAngle_;
vector<float>  AODCaloJetTotalTrackAngle_;

vector<float>  AODCaloJetAvfVx_;
vector<float>  AODCaloJetAvfVy_;
vector<float>  AODCaloJetAvfVz_;
vector<float>  AODCaloJetAvfVertexTotalChiSquared_;
vector<float>  AODCaloJetAvfVertexDegreesOfFreedom_;
vector<float>  AODCaloJetAvfVertexChi2NDoF_;
vector<float>  AODCaloJetAvfVertexDistanceToBeam_;
vector<float>  AODCaloJetAvfVertexTransverseError_;
vector<float>  AODCaloJetAvfVertexTransverseSig_;
vector<float>  AODCaloJetAvfVertexDeltaEta_;
vector<float>  AODCaloJetAvfVertexDeltaPhi_;
vector<float>  AODCaloJetAvfVertexRecoilPt_;
vector<float>  AODCaloJetAvfVertexTrackMass_;
vector<float>  AODCaloJetAvfVertexTrackEnergy_;
vector<float>  AODCaloJetAvfBeamSpotDeltaPhi_;
vector<float>  AODCaloJetAvfBeamSpotRecoilPt_;
vector<float>  AODCaloJetAvfBeamSpotMedianDeltaPhi_;
vector<float>  AODCaloJetAvfBeamSpotLog10MedianDeltaPhi_;
vector<int>    AODCaloJetNCleanMatchedTracks_;
vector<int>    AODCaloJetSumHitsInFrontOfVert_;
vector<int>    AODCaloJetSumMissHitsAfterVert_;
vector<int>    AODCaloJetHitsInFrontOfVertPerTrack_;
vector<int>    AODCaloJetMissHitsAfterVertPerTrack_;
vector<float>  AODCaloJetAvfDistToPV_;
vector<float>  AODCaloJetAvfVertexDeltaZtoPV_;
vector<float>  AODCaloJetAvfVertexDeltaZtoPV2_;

// PF Jets
Int_t          AODnPFJet_;
vector<float>  AODPFJetPt_;
vector<float>  AODPFJetEta_;
vector<float>  AODPFJetPhi_;
vector<float>  AODPFJetAlphaMax_;
vector<float>  AODPFJetAlphaMax2_;
vector<float>  AODPFJetAlphaMaxPrime_;
vector<float>  AODPFJetAlphaMaxPrime2_;
vector<float>  AODPFJetBeta_;
vector<float>  AODPFJetBeta2_;
vector<float>  AODPFJetSumIP_;
vector<float>  AODPFJetSumIPSig_;
vector<float>  AODPFJetMedianIP_;
vector<float>  AODPFJetMedianLog10IPSig_;
vector<float>  AODPFJetTrackAngle_;
vector<float>  AODPFJetLogTrackAngle_;
vector<float>  AODPFJetMedianLog10TrackAngle_;
vector<float>  AODPFJetTotalTrackAngle_;

// PFchs Jets
Int_t          AODnPFchsJet_;
vector<float>  AODPFchsJetPt_;
vector<float>  AODPFchsJetEta_;
vector<float>  AODPFchsJetPhi_;
vector<float>  AODPFchsJetAlphaMax_;
vector<float>  AODPFchsJetAlphaMax2_;
vector<float>  AODPFchsJetAlphaMaxPrime_;
vector<float>  AODPFchsJetAlphaMaxPrime2_;
vector<float>  AODPFchsJetBeta_;
vector<float>  AODPFchsJetBeta2_;
vector<float>  AODPFchsJetSumIP_;
vector<float>  AODPFchsJetSumIPSig_;
vector<float>  AODPFchsJetMedianIP_;
vector<float>  AODPFchsJetMedianLog10IPSig_;
vector<float>  AODPFchsJetTrackAngle_;
vector<float>  AODPFchsJetLogTrackAngle_;
vector<float>  AODPFchsJetMedianLog10TrackAngle_;
vector<float>  AODPFchsJetTotalTrackAngle_;

// temporary variables

vector<TVector3> AODallTrackPositions; // x,y,z 

vector<float>    AODallTrackPt;
vector<float>    AODallTrackEta;
vector<float>    AODallTrackPhi;
vector<float>    AODallTrackIFSPt;

vector<int>      AODallTracknMissingInner;
vector<int>      AODallTracknMissingOuter;
vector<float>    AODallTrackAngle;

vector<int>      AODwhichVertexByTrack;

vector<float>    AODallTrackdxy;
vector<float>    AODallTrackdxyerr;

// set parameters for tracks to be accepted
const float minTrackPt_ = 1.0;
const float maxDRtrackJet_ = 0.4;


// start functions ----------------------------------------

// initialize branches
void lldjNtuple::branchesJets(TTree* tree) {

  //link the variable in c++ code to variable in branch
  tree->Branch("nSlimmedJets",                      &nSlimmedJets_);          
  tree->Branch("slimmedJetPt",                      &slimmedJetPt_);           
  tree->Branch("slimmedJetEn",                      &slimmedJetEn_);           
  tree->Branch("slimmedJetEta",                     &slimmedJetEta_);          
  tree->Branch("slimmedJetPhi",                     &slimmedJetPhi_);          
  tree->Branch("slimmedJetRawPt",                   &slimmedJetRawPt_);        
  tree->Branch("slimmedJetRawEn",                   &slimmedJetRawEn_);        
  tree->Branch("slimmedJetMt",                      &slimmedJetMt_);           
  tree->Branch("slimmedJetArea",                    &slimmedJetArea_);         
  tree->Branch("slimmedJetLeadTrackPID",            &slimmedJetLeadTrackPID_); 
  tree->Branch("slimmedJetLeadTrackPt",             &slimmedJetLeadTrackPt_);  
  tree->Branch("slimmedJetLeadTrackEta",            &slimmedJetLeadTrackEta_); 
  tree->Branch("slimmedJetLeadTrackPhi",            &slimmedJetLeadTrackPhi_); 
  tree->Branch("slimmedJetLepTrackPID",             &slimmedJetLepTrackPID_);  
  tree->Branch("slimmedJetLepTrackPt",              &slimmedJetLepTrackPt_);   
  tree->Branch("slimmedJetLepTrackEta",             &slimmedJetLepTrackEta_);  
  tree->Branch("slimmedJetLepTrackPhi",             &slimmedJetLepTrackPhi_);  
  tree->Branch("slimmedJetCHF",                     &slimmedJetCHF_);          
  tree->Branch("slimmedJetNHF",                     &slimmedJetNHF_);          
  tree->Branch("slimmedJetCEF",                     &slimmedJetCEF_);          
  tree->Branch("slimmedJetNEF",                     &slimmedJetNEF_);          
  tree->Branch("slimmedJetNCH",                     &slimmedJetNCH_);          
  tree->Branch("slimmedJetNNP",                     &slimmedJetNNP_);          
  tree->Branch("slimmedJetMUF",                     &slimmedJetMUF_);          
  tree->Branch("slimmedJetHFHAE",                   &slimmedJetHFHAE_);        
  tree->Branch("slimmedJetHFEME",                   &slimmedJetHFEME_);        
  tree->Branch("slimmedJetNConstituents",           &slimmedJetNConstituents_);
  tree->Branch("slimmedJetVtxPt",                   &slimmedJetVtxPt_);        
  tree->Branch("slimmedJetVtxMass",                 &slimmedJetVtxMass_);      
  tree->Branch("slimmedJetVtxNtrks",                &slimmedJetVtxNtrks_);     
  tree->Branch("slimmedJetVtx3DVal",                &slimmedJetVtx3DVal_);     
  tree->Branch("slimmedJetVtx3DSig",                &slimmedJetVtx3DSig_);     
  tree->Branch("slimmedJetCSV2BJetTags",            &slimmedJetCSV2BJetTags_); 
  tree->Branch("slimmedJetJetProbabilityBJetTags",  &slimmedJetJetProbabilityBJetTags_);  
  tree->Branch("slimmedJetpfCombinedMVAV2BJetTags", &slimmedJetpfCombinedMVAV2BJetTags_); 
  tree->Branch("slimmedJetPartonID",                &slimmedJetPartonID_);   
  tree->Branch("slimmedJetHadFlvr",                 &slimmedJetHadFlvr_);    
  tree->Branch("slimmedJetPFLooseId",               &slimmedJetPFLooseId_);  
  tree->Branch("slimmedJetID",                      &slimmedJetID_);         
  tree->Branch("slimmedJetPUID",                    &slimmedJetPUID_);       
  tree->Branch("slimmedJetPUFullID",                &slimmedJetPUFullID_);   
  tree->Branch("slimmedJetJECUnc",                  &slimmedJetJECUnc_);     
  tree->Branch("slimmedJetP4Smear",                 &slimmedJetP4Smear_);    
  tree->Branch("slimmedJetP4SmearUp",               &slimmedJetP4SmearUp_);  
  tree->Branch("slimmedJetP4SmearDo",               &slimmedJetP4SmearDo_);  
  tree->Branch("slimmedJetFiredTrgs",               &slimmedJetFiredTrgs_);  
 
  tree->Branch("slimmedJetAlphaD",                  &slimmedJetAlphaD_);     
  tree->Branch("slimmedJetAlphaMaxD",               &slimmedJetAlphaMaxD_);  
  tree->Branch("slimmedJetSumIP",                   &slimmedJetSumIP_);      
  tree->Branch("slimmedJetSumIPSig",                &slimmedJetSumIPSig_);   
  //tree->Branch("slimmedJetLog10IPSig",              &slimmedJetLog10IPSig_);              
  tree->Branch("slimmedJetMedianLog10IPSig",        &slimmedJetMedianLog10IPSig_);        
  tree->Branch("slimmedJetTrackAngle",              &slimmedJetTrackAngle_);              
  tree->Branch("slimmedJetLogTrackAngle",           &slimmedJetLogTrackAngle_);           
  tree->Branch("slimmedJetMedianLog10TrackAngle",     &slimmedJetMedianLog10TrackAngle_);     
  tree->Branch("slimmedJetTotalTrackAngle",         &slimmedJetTotalTrackAngle_);         
  tree->Branch("slimmedJetDauVertex_x",             &slimmedJetDauVertex_x_);             
  tree->Branch("slimmedJetDauVertex_y",             &slimmedJetDauVertex_y_);             
  tree->Branch("slimmedJetDauVertex_z",             &slimmedJetDauVertex_z_);             
  tree->Branch("slimmedJetDauVertex_r",             &slimmedJetDauVertex_r_);             
 
  tree->Branch("slimmedJetAlphaMax_PV3onPV2",       &slimmedJetAlphaMax_PV3onPV2_);       
  tree->Branch("slimmedJetAlphaMax_PV3onChg",       &slimmedJetAlphaMax_PV3onChg_);       
  tree->Branch("slimmedJetAlphaMax_PV3onAll",       &slimmedJetAlphaMax_PV3onAll_);       
  tree->Branch("slimmedJetAlphaMax_PV2onChg",       &slimmedJetAlphaMax_PV2onChg_);       
  tree->Branch("slimmedJetAlphaMax_PV2onAll",       &slimmedJetAlphaMax_PV2onAll_);       
 
  tree->Branch("slimmedJetAlpha2Max_PV3onPV2",      &slimmedJetAlpha2Max_PV3onPV2_);      
  tree->Branch("slimmedJetAlpha2Max_PV3onChg",      &slimmedJetAlpha2Max_PV3onChg_);      
  tree->Branch("slimmedJetAlpha2Max_PV3onAll",      &slimmedJetAlpha2Max_PV3onAll_);      
  tree->Branch("slimmedJetAlpha2Max_PV2onChg",      &slimmedJetAlpha2Max_PV2onChg_);      
  tree->Branch("slimmedJetAlpha2Max_PV2onAll",      &slimmedJetAlpha2Max_PV2onAll_);      
 
  tree->Branch("slimmedJetTrackPt",                 &slimmedJetTrackPt_);                 
  tree->Branch("slimmedJetTrackEta",                &slimmedJetTrackEta_);                
  tree->Branch("slimmedJetTrackPhi",                &slimmedJetTrackPhi_);                
  tree->Branch("slimmedJetTrackPDGID",              &slimmedJetTrackPDGID_);              
  tree->Branch("slimmedJetTrackMomPDGID",           &slimmedJetTrackMomPDGID_);           
 
  tree->Branch("slimmedJetGenJetEn",                &slimmedJetGenJetEn_);                
  tree->Branch("slimmedJetGenJetPt",                &slimmedJetGenJetPt_);                
  tree->Branch("slimmedJetGenJetEta",               &slimmedJetGenJetEta_);               
  tree->Branch("slimmedJetGenJetPhi",               &slimmedJetGenJetPhi_);               
  tree->Branch("slimmedJetGenPartonID",             &slimmedJetGenPartonID_);             
  tree->Branch("slimmedJetGenEn",                   &slimmedJetGenEn_);                   
  tree->Branch("slimmedJetGenPt",                   &slimmedJetGenPt_);                   
  tree->Branch("slimmedJetGenEta",                  &slimmedJetGenEta_);                  
  tree->Branch("slimmedJetGenPhi",                  &slimmedJetGenPhi_);                  
  tree->Branch("slimmedJetGenPartonMomID",          &slimmedJetGenPartonMomID_);          


  tree->Branch("AODnCaloJet"                   , &AODnCaloJet_);
  tree->Branch("AODCaloJetPt"                  , &AODCaloJetPt_);
  tree->Branch("AODCaloJetEta"                 , &AODCaloJetEta_);
  tree->Branch("AODCaloJetPhi"                 , &AODCaloJetPhi_);

  tree->Branch("AODCaloJetAlphaMax"            , &AODCaloJetAlphaMax_);                               
  tree->Branch("AODCaloJetAlphaMax2"           , &AODCaloJetAlphaMax2_);                               
  tree->Branch("AODCaloJetAlphaMaxPrime"       , &AODCaloJetAlphaMaxPrime_);                               
  tree->Branch("AODCaloJetAlphaMaxPrime2"      , &AODCaloJetAlphaMaxPrime2_);                               
  tree->Branch("AODCaloJetBeta"                , &AODCaloJetBeta_);                               
  tree->Branch("AODCaloJetBeta2"               , &AODCaloJetBeta2_);                               

  tree->Branch("AODCaloJetSumIP"               , &AODCaloJetSumIP_);
  tree->Branch("AODCaloJetSumIPSig"            , &AODCaloJetSumIPSig_);
  tree->Branch("AODCaloJetMedianIP"            , &AODCaloJetMedianIP_);
  tree->Branch("AODCaloJetMedianLog10IPSig"    , &AODCaloJetMedianLog10IPSig_);
  tree->Branch("AODCaloJetTrackAngle"          , &AODCaloJetTrackAngle_);
  tree->Branch("AODCaloJetLogTrackAngle"       , &AODCaloJetLogTrackAngle_);
  tree->Branch("AODCaloJetMedianLog10TrackAngle" , &AODCaloJetMedianLog10TrackAngle_);
  tree->Branch("AODCaloJetTotalTrackAngle"     , &AODCaloJetTotalTrackAngle_);
                       
  tree->Branch("AODCaloJetAvfVx", &AODCaloJetAvfVx_);
  tree->Branch("AODCaloJetAvfVy", &AODCaloJetAvfVy_);
  tree->Branch("AODCaloJetAvfVz", &AODCaloJetAvfVz_);
  tree->Branch("AODCaloJetAvfVertexTotalChiSquared", &AODCaloJetAvfVertexTotalChiSquared_);
  tree->Branch("AODCaloJetAvfVertexDegreesOfFreedom", &AODCaloJetAvfVertexDegreesOfFreedom_);
  tree->Branch("AODCaloJetAvfVertexChi2NDoF", &AODCaloJetAvfVertexChi2NDoF_);
  tree->Branch("AODCaloJetAvfVertexDistanceToBeam", &AODCaloJetAvfVertexDistanceToBeam_);
  tree->Branch("AODCaloJetAvfVertexTransverseError", &AODCaloJetAvfVertexTransverseError_);
  tree->Branch("AODCaloJetAvfVertexTransverseSig", &AODCaloJetAvfVertexTransverseSig_);
  tree->Branch("AODCaloJetAvfVertexDeltaEta", &AODCaloJetAvfVertexDeltaEta_);
  tree->Branch("AODCaloJetAvfVertexDeltaPhi", &AODCaloJetAvfVertexDeltaPhi_);
  tree->Branch("AODCaloJetAvfVertexRecoilPt", &AODCaloJetAvfVertexRecoilPt_);
  tree->Branch("AODCaloJetAvfVertexTrackMass", &AODCaloJetAvfVertexTrackMass_);
  tree->Branch("AODCaloJetAvfVertexTrackEnergy", &AODCaloJetAvfVertexTrackEnergy_);
  tree->Branch("AODCaloJetAvfBeamSpotDeltaPhi", &AODCaloJetAvfBeamSpotDeltaPhi_);
  tree->Branch("AODCaloJetAvfBeamSpotRecoilPt", &AODCaloJetAvfBeamSpotRecoilPt_);
  tree->Branch("AODCaloJetAvfBeamSpotMedianDeltaPhi", &AODCaloJetAvfBeamSpotMedianDeltaPhi_);
  tree->Branch("AODCaloJetAvfBeamSpotLog10MedianDeltaPhi", &AODCaloJetAvfBeamSpotLog10MedianDeltaPhi_);
  tree->Branch("AODCaloJetNCleanMatchedTracks", &AODCaloJetNCleanMatchedTracks_);
  tree->Branch("AODCaloJetSumHitsInFrontOfVert", &AODCaloJetSumHitsInFrontOfVert_);
  tree->Branch("AODCaloJetSumMissHitsAfterVert", &AODCaloJetSumMissHitsAfterVert_);
  tree->Branch("AODCaloJetHitsInFrontOfVertPerTrack", &AODCaloJetHitsInFrontOfVertPerTrack_);
  tree->Branch("AODCaloJetMissHitsAfterVertPerTrack", &AODCaloJetMissHitsAfterVertPerTrack_);
  tree->Branch("AODCaloJetAvfDistToPV", &AODCaloJetAvfDistToPV_);
  tree->Branch("AODCaloJetAvfVertexDeltaZtoPV", &AODCaloJetAvfVertexDeltaZtoPV_);
  tree->Branch("AODCaloJetAvfVertexDeltaZtoPV2", &AODCaloJetAvfVertexDeltaZtoPV2_);
                          
  // PF Jets
  tree->Branch("AODnPFJet"                        , &AODnPFJet_);                                 
  tree->Branch("AODPFJetPt"                       , &AODPFJetPt_);                                 
  tree->Branch("AODPFJetEta"                      , &AODPFJetEta_);                                 
  tree->Branch("AODPFJetPhi"                      , &AODPFJetPhi_);                                 
  tree->Branch("AODPFJetAlphaMax"                 , &AODPFJetAlphaMax_);                                 
  tree->Branch("AODPFJetAlphaMax2"                , &AODPFJetAlphaMax2_);                                 
  tree->Branch("AODPFJetAlphaMaxPrime"            , &AODPFJetAlphaMaxPrime_);                                 
  tree->Branch("AODPFJetAlphaMaxPrime2"           , &AODPFJetAlphaMaxPrime2_);                                 
  tree->Branch("AODPFJetBeta"                     , &AODPFJetBeta_);                                 
  tree->Branch("AODPFJetBeta2"                    , &AODPFJetBeta2_);                                 
  tree->Branch("AODPFJetSumIP"                    , &AODPFJetSumIP_);                                 
  tree->Branch("AODPFJetSumIPSig"                 , &AODPFJetSumIPSig_);                                 
  tree->Branch("AODPFJetMedianIP"                 , &AODPFJetMedianIP_);                                                                           
  tree->Branch("AODPFJetMedianLog10IPSig"         , &AODPFJetMedianLog10IPSig_);                           
  tree->Branch("AODPFJetTrackAngle"               , &AODPFJetTrackAngle_);                                    
  tree->Branch("AODPFJetLogTrackAngle"            , &AODPFJetLogTrackAngle_);                                         
  tree->Branch("AODPFJetMedianLog10TrackAngle"    , &AODPFJetMedianLog10TrackAngle_);                           
  tree->Branch("AODPFJetTotalTrackAngle"          , &AODPFJetTotalTrackAngle_);
  
  // PFcms Jets 
  tree->Branch("AODnPFchsJet"                     , &AODnPFchsJet_);                                  
  tree->Branch("AODPFchsJetPt"                    , &AODPFchsJetPt_);                                  
  tree->Branch("AODPFchsJetEta"                   , &AODPFchsJetEta_);                                 
  tree->Branch("AODPFchsJetPhi"                   , &AODPFchsJetPhi_);                                      
  tree->Branch("AODPFchsJetAlphaMax"              , &AODPFchsJetAlphaMax_);                                  
  tree->Branch("AODPFchsJetAlphaMax2"             , &AODPFchsJetAlphaMax2_);                                     
  tree->Branch("AODPFchsJetAlphaMaxPrime"         , &AODPFchsJetAlphaMaxPrime_);                                  
  tree->Branch("AODPFchsJetAlphaMaxPrime2"        , &AODPFchsJetAlphaMaxPrime2_);                       
  tree->Branch("AODPFchsJetBeta"                  , &AODPFchsJetBeta_);                                  
  tree->Branch("AODPFchsJetBeta2"                 , &AODPFchsJetBeta2_);                                 
  tree->Branch("AODPFchsJetSumIP"                 , &AODPFchsJetSumIP_);                                    
  tree->Branch("AODPFchsJetSumIPSig"              , &AODPFchsJetSumIPSig_);                                 
  tree->Branch("AODPFchsJetMedianIP"              , &AODPFchsJetMedianIP_);                                   
  tree->Branch("AODPFchsJetMedianLog10IPSig"      , &AODPFchsJetMedianLog10IPSig_);                              
  tree->Branch("AODPFchsJetTrackAngle"            , &AODPFchsJetTrackAngle_);                                            
  tree->Branch("AODPFchsJetLogTrackAngle"         , &AODPFchsJetLogTrackAngle_);                                   
  tree->Branch("AODPFchsJetMedianLog10TrackAngle" , &AODPFchsJetMedianLog10TrackAngle_);
  tree->Branch("AODPFchsJetTotalTrackAngle"       , &AODPFchsJetTotalTrackAngle_);

}

//fills slimmedJets .clear() to empty vector of old data
void lldjNtuple::fillJets(const edm::Event& e, const edm::EventSetup& es) {

 bool dodebug = false;
 // cleanup from previous execution
 nSlimmedJets_=0;
 slimmedJetPt_.clear();
 slimmedJetEn_.clear();
 slimmedJetEta_.clear();
 slimmedJetPhi_.clear();
 slimmedJetRawPt_.clear();
 slimmedJetRawEn_.clear();
 slimmedJetMt_.clear();
 slimmedJetArea_.clear();
 slimmedJetLeadTrackPID_.clear();
 slimmedJetLeadTrackPt_.clear();
 slimmedJetLeadTrackEta_.clear();
 slimmedJetLeadTrackPhi_.clear();
 slimmedJetLepTrackPID_.clear();
 slimmedJetLepTrackPt_.clear();
 slimmedJetLepTrackEta_.clear();
 slimmedJetLepTrackPhi_.clear();
 slimmedJetCHF_.clear();
 slimmedJetNHF_.clear();
 slimmedJetCEF_.clear();
 slimmedJetNEF_.clear();
 slimmedJetNCH_.clear();
 slimmedJetNNP_.clear();
 slimmedJetMUF_.clear();
 slimmedJetHFHAE_.clear();
 slimmedJetHFEME_.clear();
 slimmedJetNConstituents_.clear();
 slimmedJetVtxPt_.clear();
 slimmedJetVtxMass_.clear();
 slimmedJetVtxNtrks_.clear();
 slimmedJetVtx3DVal_.clear();
 slimmedJetVtx3DSig_.clear();
 slimmedJetCSV2BJetTags_.clear();
 slimmedJetJetProbabilityBJetTags_.clear();
 slimmedJetpfCombinedMVAV2BJetTags_.clear();
 slimmedJetPartonID_.clear();
 slimmedJetHadFlvr_.clear();
 slimmedJetPFLooseId_.clear();
 slimmedJetID_.clear(); 
 slimmedJetPUID_.clear();
 slimmedJetPUFullID_.clear();
 slimmedJetJECUnc_.clear();
 slimmedJetP4Smear_.clear();
 slimmedJetP4SmearUp_.clear();
 slimmedJetP4SmearDo_.clear();
 slimmedJetFiredTrgs_.clear();
 
 slimmedJetAlphaD_.clear();
 slimmedJetAlphaMaxD_.clear();
 slimmedJetSumIP_.clear();
 slimmedJetSumIPSig_.clear();
 //slimmedJetLog10IPSig_.clear();
 slimmedJetMedianLog10IPSig_.clear();
 slimmedJetTrackAngle_.clear();
 slimmedJetLogTrackAngle_.clear();
 slimmedJetMedianLog10TrackAngle_.clear();
 slimmedJetTotalTrackAngle_.clear();
 slimmedJetDauVertex_x_.clear();
 slimmedJetDauVertex_y_.clear();
 slimmedJetDauVertex_z_.clear();
 slimmedJetDauVertex_r_.clear();
 
 slimmedJetAlphaMax_PV3onPV2_.clear(); 
 slimmedJetAlphaMax_PV3onChg_.clear(); 
 slimmedJetAlphaMax_PV3onAll_.clear(); 
 slimmedJetAlphaMax_PV2onChg_.clear(); 
 slimmedJetAlphaMax_PV2onAll_.clear(); 
 
 slimmedJetAlpha2Max_PV3onPV2_.clear(); 
 slimmedJetAlpha2Max_PV3onChg_.clear(); 
 slimmedJetAlpha2Max_PV3onAll_.clear(); 
 slimmedJetAlpha2Max_PV2onChg_.clear(); 
 slimmedJetAlpha2Max_PV2onAll_.clear(); 
 
 slimmedJetTrackPt_.clear();
 slimmedJetTrackEta_.clear();
 slimmedJetTrackPhi_.clear();
 slimmedJetTrackPDGID_.clear();
 slimmedJetTrackMomPDGID_.clear();
 
 slimmedJetGenJetEn_.clear();
 slimmedJetGenJetPt_.clear();
 slimmedJetGenJetEta_.clear();
 slimmedJetGenJetPhi_.clear();
 slimmedJetGenPartonID_.clear();
 slimmedJetGenEn_.clear();
 slimmedJetGenPt_.clear();
 slimmedJetGenEta_.clear();
 slimmedJetGenPhi_.clear();
 slimmedJetGenPartonMomID_.clear();

 AODnCaloJet_=0;
 AODCaloJetPt_.clear();
 AODCaloJetEta_.clear();
 AODCaloJetPhi_.clear();

 AODCaloJetAlphaMax_.clear();                               
 AODCaloJetAlphaMax2_.clear();                               
 AODCaloJetAlphaMaxPrime_.clear();                               
 AODCaloJetAlphaMaxPrime2_.clear();                               
 AODCaloJetBeta_.clear();                               
 AODCaloJetBeta2_.clear();                               

 AODCaloJetSumIP_.clear();
 AODCaloJetSumIPSig_.clear();
 AODCaloJetMedianIP_.clear();
 AODCaloJetMedianLog10IPSig_.clear();
 AODCaloJetTrackAngle_.clear();
 AODCaloJetLogTrackAngle_.clear();
 AODCaloJetMedianLog10TrackAngle_.clear();
 AODCaloJetTotalTrackAngle_.clear();

 AODCaloJetAvfVx_.clear();
 AODCaloJetAvfVy_.clear();
 AODCaloJetAvfVz_.clear();
 AODCaloJetAvfVertexTotalChiSquared_.clear();
 AODCaloJetAvfVertexDegreesOfFreedom_.clear();
 AODCaloJetAvfVertexChi2NDoF_.clear();
 AODCaloJetAvfVertexDistanceToBeam_.clear();
 AODCaloJetAvfVertexTransverseError_.clear();
 AODCaloJetAvfVertexTransverseSig_.clear();
 AODCaloJetAvfVertexDeltaEta_.clear();
 AODCaloJetAvfVertexDeltaPhi_.clear();
 AODCaloJetAvfVertexRecoilPt_.clear();
 AODCaloJetAvfVertexTrackMass_.clear();
 AODCaloJetAvfVertexTrackEnergy_.clear();
 AODCaloJetAvfBeamSpotDeltaPhi_.clear();
 AODCaloJetAvfBeamSpotRecoilPt_.clear();
 AODCaloJetAvfBeamSpotMedianDeltaPhi_.clear();
 AODCaloJetAvfBeamSpotLog10MedianDeltaPhi_.clear();
 AODCaloJetNCleanMatchedTracks_.clear();
 AODCaloJetSumHitsInFrontOfVert_.clear();
 AODCaloJetSumMissHitsAfterVert_.clear();
 AODCaloJetHitsInFrontOfVertPerTrack_.clear();
 AODCaloJetMissHitsAfterVertPerTrack_.clear();
 AODCaloJetAvfDistToPV_.clear();
 AODCaloJetAvfVertexDeltaZtoPV_.clear();
 AODCaloJetAvfVertexDeltaZtoPV2_.clear();

 // PF Jets
 AODnPFJet_=0;
 AODPFJetPt_.clear();
 AODPFJetEta_.clear();
 AODPFJetPhi_.clear();
 AODPFJetAlphaMax_.clear();
 AODPFJetAlphaMax2_.clear();
 AODPFJetAlphaMaxPrime_.clear();
 AODPFJetAlphaMaxPrime2_.clear();
 AODPFJetBeta_.clear();
 AODPFJetBeta2_.clear();
 AODPFJetSumIP_.clear();
 AODPFJetSumIPSig_.clear();
 AODPFJetMedianIP_.clear();
 AODPFJetMedianLog10IPSig_.clear();
 AODPFJetTrackAngle_.clear();
 AODPFJetLogTrackAngle_.clear();
 AODPFJetMedianLog10TrackAngle_.clear();
 AODPFJetTotalTrackAngle_.clear();
 
 // PFchs Jets
 AODnPFchsJet_=0;
 AODPFchsJetPt_.clear();
 AODPFchsJetEta_.clear();
 AODPFchsJetPhi_.clear();
 AODPFchsJetAlphaMax_.clear();
 AODPFchsJetAlphaMax2_.clear();
 AODPFchsJetAlphaMaxPrime_.clear();
 AODPFchsJetAlphaMaxPrime2_.clear();
 AODPFchsJetBeta_.clear();
 AODPFchsJetBeta2_.clear();
 AODPFchsJetSumIP_.clear();
 AODPFchsJetSumIPSig_.clear();
 AODPFchsJetMedianIP_.clear();
 AODPFchsJetMedianLog10IPSig_.clear();
 AODPFchsJetTrackAngle_.clear();
 AODPFchsJetLogTrackAngle_.clear();
 AODPFchsJetMedianLog10TrackAngle_.clear();
 AODPFchsJetTotalTrackAngle_.clear();

 // get slimmedJets
 e.getByToken(jetsAK4Label_, slimmedJetHandle);

 if (!slimmedJetHandle.isValid()) {
   edm::LogWarning("lldjNtuple") << "no miniAOD pat::Jets (AK4) in event";
   return;
 }

 //edm::Handle<vector<reco::GenParticle> > genParticlesHandle;
 //if(doGenParticles_)e.getByToken(genParticlesCollection_, genParticlesHandle);
 
 e.getByToken(rhoLabel_, rhoHandle);
 float rho = *(rhoHandle.product());
 
 e.getByToken(vtxLabel_, vtxHandle);
 if (!vtxHandle.isValid()) edm::LogWarning("lldjNtuple") << "Primary vertices info not unavailable";

 // Accessing the JEC uncertainties 
 //ak4  
 edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
 es.get<JetCorrectionsRecord>().get("AK4PFchs",JetCorParColl); 
 JetCorrectorParameters const & JetCorPar = (*JetCorParColl)["Uncertainty"];
 JetCorrectionUncertainty *jecUnc=0;
 jecUnc = new JetCorrectionUncertainty(JetCorPar);
 // //ak8
 // edm::ESHandle<JetCorrectorParametersCollection> AK8JetCorParColl;
 // es.get<JetCorrectionsRecord>().get("AK8PFchs",AK8JetCorParColl);
 // JetCorrectorParameters const & AK8JetCorPar = (*AK8JetCorParColl)["Uncertainty"];
 // JetCorrectionUncertainty *AK8jecUnc=0;
 // AK8jecUnc = new JetCorrectionUncertainty(AK8JetCorPar);
 
 //start slimmedJets Lvdp
 int nrslimmedJet = 0;
 int IP0Sum = 0;
 int TA0Sum = 0;

 if(dodebug){ printf("Starting loop over slimmedJets\n"); }
 unsigned int tmp_slimmedJetnr = 0; 

 // miniAOD Jets -------------------------------------------
 for (edm::View<pat::Jet>::const_iterator iJet = slimmedJetHandle->begin(); iJet != slimmedJetHandle->end(); ++iJet) {
   tmp_slimmedJetnr++;
   if(dodebug){ printf(" Jet Nr: %u \n",tmp_slimmedJetnr); }

   // start with ID so we skip events early
   float NHF      = iJet->neutralHadronEnergyFraction();
   float NEMF     = iJet->neutralEmEnergyFraction();
   float NumConst = iJet->chargedMultiplicity()+iJet->neutralMultiplicity();
   float CHF      = iJet->chargedHadronEnergyFraction();
   float CHM      = iJet->chargedMultiplicity();
   float CEMF     = iJet->chargedEmEnergyFraction();
   float NNP      = iJet->neutralMultiplicity();

   // (eta-dependent) jet ID from <TWIKI REFERENCE>
   bool looseJetID        = false;    
   bool tightJetID        = false;
   if (fabs(iJet->eta()) <= 2.7) {
     looseJetID = (NHF<0.99 && NEMF<0.99 && NumConst>1) && ((fabs(iJet->eta())<=2.4 && CHF>0 && CHM>0 && CEMF<0.99) || fabs(iJet->eta())>2.4);
     tightJetID = (NHF<0.90 && NEMF<0.90 && NumConst>1) && ((fabs(iJet->eta())<=2.4 && CHF>0 && CHM>0 && CEMF<0.99) || fabs(iJet->eta())>2.4);
   } else if (fabs(iJet->eta()) <= 3.0) {
     looseJetID = (NEMF>0.01 && NHF<0.98 && NNP>2);
     tightJetID = (NEMF>0.01 && NHF<0.98 && NNP>2);
   } else {
     looseJetID = (NEMF<0.90 && NNP>10); 
     tightJetID = (NEMF<0.90 && NNP>10);
   }
   Int_t slimmedJetIDdecision = 0;
   if (looseJetID) slimmedJetIDdecision += pow(2, 1);
   if (tightJetID) slimmedJetIDdecision += pow(2, 2);

   // place selection on slimmedJets making it into collection
   // define cuts earlier? thresholds set in cfg?
   if (iJet->pt() < 20 || fabs(iJet->eta()) > 2.4 || slimmedJetIDdecision==0 ) continue; 

   nrslimmedJet++;

   slimmedJetID_.push_back(slimmedJetIDdecision);    

   // PUJet ID from slimmedJets
   slimmedJetPUID_.push_back(iJet->userFloat("pileupJetId:fullDiscriminant"));
   slimmedJetPUFullID_.push_back(iJet->userInt("pileupJetId:fullId"));

   // Set basic jet varibales
   if(dodebug){printf(" pt: %f\n",iJet->pt()); }
   slimmedJetPt_.push_back(    iJet->pt());
   //cout << endl<< iJet->pt()<<" *****slimmedJet constituents: "<<iJet->getJetConstituents().size() <<endl;
   //cout<<endl<<"JetPT: "<<iJet->pt() <<"Event "<<e.id().event()<<endl;
   slimmedJetEn_.push_back(    iJet->energy());
   slimmedJetEta_.push_back(   iJet->eta());
   slimmedJetPhi_.push_back(   iJet->phi());
   slimmedJetRawPt_.push_back( (*iJet).correctedJet("Uncorrected").pt());
   slimmedJetRawEn_.push_back( (*iJet).correctedJet("Uncorrected").energy());
   slimmedJetMt_.push_back(    iJet->mt());
   slimmedJetArea_.push_back(  iJet->jetArea());
   slimmedJetCEF_.push_back(   iJet->chargedEmEnergyFraction());
   slimmedJetNEF_.push_back(   iJet->neutralEmEnergyFraction());
   slimmedJetCHF_.push_back(   iJet->chargedHadronEnergyFraction());
   slimmedJetNHF_.push_back(   iJet->neutralHadronEnergyFraction());
   slimmedJetNCH_.push_back(   iJet->chargedMultiplicity());
   slimmedJetNNP_.push_back(   iJet->neutralMultiplicity());
   slimmedJetMUF_.push_back(   iJet->muonEnergyFraction());
   //slimmedJetNConstituents_.push_back(iJet->numberOfDaughters());
   slimmedJetNConstituents_.push_back(iJet->getJetConstituents().size());

   // Jet Energy Correction uncertainty
   if (fabs(iJet->eta()) < 5.2) {
     jecUnc->setJetEta(iJet->eta());
     jecUnc->setJetPt(iJet->pt()); // here you must use the CORRECTED jet pt
     slimmedJetJECUnc_.push_back(jecUnc->getUncertainty(true));
   } else {
     slimmedJetJECUnc_.push_back(-1.);
   }

   // miniAOD jet tracks 
   // initialize leading track and lepton
   int   leadTrkPID = -99;
   float leadTrkPt  = -99;
   float leadTrkEta = -99;
   float leadTrkPhi = -99;
   int   lepTrkPID  = -99;
   float lepTrkPt   = -99;
   float lepTrkEta  = -99;
   float lepTrkPhi  = -99;

   // Tracks from Primay Vertex for different PV identification schemes
   // used for alphaMax
   vector<float> sumtracksfPV2(vtxHandle->size(),  0.);
   vector<float> sum2tracksfPV2(vtxHandle->size(), 0.);
   vector<float> sumtracksfPV3(vtxHandle->size(),  0.);
   vector<float> sum2tracksfPV3(vtxHandle->size(), 0.);
   vector<float> sumtracksMinDzVtx(vtxHandle->size() ,  0.);
   vector<float> sum2tracksMinDzVtx(vtxHandle->size() , 0.);
   float sumneutraltracks   = 0. ;
   float sum2neutraltracks  = 0. ;
   float sumalltracks       = 0. ;
   float sum2alltracks      = 0. ;

   // Interaction Point
   float dxy;
   float dxyerr;
   float SumIP = 0.0;
   float SumIPSig = 0.0;
   float MedianLog10IPSig = 0.0;
   float TotalTrackAngle = 0.0;
   float MedianLogTrackAngle = 0.0;
   bool  IPtest = false;
   float dxySig = -1.;

   float daughterPt = -1.;
   float daughterEta = -99.;
   float daughterPhi = -99.;
   int   daughterPDGID = 0;

   // increment only for non-null daughters
   if(dodebug){ printf("  Looping over Tracks\n"); }
   unsigned int tmp_tracknr = 0; 

   vector<float> tmpTrackPt;
   vector<float> tmpTrackEta;
   vector<float> tmpTrackPhi;
   vector<int>   tmpTrackPDGID;
   vector<int>   tmpTrackMomPDGID;

   // looping over tracks
   for (unsigned id = 0; id < iJet->getJetConstituents().size(); id++) {

     // daughter of iJet is reco::Candidate
     const edm::Ptr<reco::Candidate> daughterRECO = iJet->getJetConstituents().at(id);

     //cast daughter as (pat::PackedCandidate)daughterPAT to access member functions
     const pat::PackedCandidate &daughterPAT = dynamic_cast<const pat::PackedCandidate &>(*iJet->getJetConstituents().at(id));

     // Vertex is already ID'd for miniAOD through jet->daughter
     const reco::VertexRef vref = daughterPAT.vertexRef();

     if (daughterRECO.isNonnull() && daughterRECO.isAvailable()) 
     {
       tmp_tracknr++;
       if(dodebug){ printf("   Track Nr: %u \n",tmp_tracknr); }

       // set common variables
       daughterPt = daughterRECO->pt(); 
       daughterEta = daughterRECO->eta();
       daughterPhi = daughterRECO->phi();
       daughterPDGID = daughterPAT.pdgId();
       if(dodebug){ 
        printf("  Jet Daughter\n");
        printf("   pt %f, eta %f, phi %f PDGID %i\n",
         daughterPt,daughterEta,daughterPhi,daughterPDGID); 
         cout <<"VertexRefIndex: "<<vref.index()
              <<" Charge: "<<daughterRECO->charge()
              <<" fromPv(): "<<daughterPAT.fromPV()
              << " pvAssociationQuality(): " <<daughterPAT.pvAssociationQuality()
       	 << endl;
       }

       tmpTrackPt    .push_back( daughterPt    );
       tmpTrackEta   .push_back( daughterEta   );
       tmpTrackPhi   .push_back( daughterPhi   );
       tmpTrackPDGID .push_back( daughterPDGID );

       //slimmedJetTrackPDGID_.push_back(daughterPAT.pdgId());

       // r = transverse vertex distance
       math::XYZPoint slimmedJetDauVertex = daughterPAT.vertex();
       float slimmedJetDauVertex_r = sqrt(pow(slimmedJetDauVertex.x(),2)+pow(slimmedJetDauVertex.y(),2));
       slimmedJetDauVertex_x_.push_back(slimmedJetDauVertex.x());
       slimmedJetDauVertex_y_.push_back(slimmedJetDauVertex.y());
       slimmedJetDauVertex_z_.push_back(slimmedJetDauVertex.z());
       slimmedJetDauVertex_r_.push_back(slimmedJetDauVertex_r);

       // set lead soft lepton (track) variables
       if (abs(daughterRECO->pdgId()) == 11 || abs(daughterRECO->pdgId()) == 13) {
         if (daughterPt > lepTrkPt) {
           lepTrkPID = daughterRECO->pdgId();
           lepTrkPt  = daughterPt;
           lepTrkEta = daughterEta;
           lepTrkPhi = daughterPhi;
         }
       }  

       // set lead (neutral) track variables
       if (daughterRECO->charge() != 0 && daughterPt > leadTrkPt) {
         leadTrkPID = daughterRECO->pdgId();
         leadTrkPt  = daughterPt;
         leadTrkEta = daughterEta;
         leadTrkPhi = daughterPhi;
       }

       // Displaced Jet Variables
       // restrict to neutral tracks 
       // should we include cut : daughterPAT.fromPV() > 1
       if (daughterRECO->charge() != 0)
       {

         // Impact Parameter
         dxy = fabs(daughterPAT.dxy()); 
         dxyerr = daughterPAT.dxyError();
         if(dxyerr>0){ dxySig = dxy/dxyerr; IPtest = true; }
         if (IPtest ==true){
           SumIP += dxy;
           SumIPSig +=dxySig;
           //cout <<"************dxy: " <<dxy<< " dxyerr: "<<dxyerr<< " dxySig: "<<dxySig<<" log10dxySig: "<< log10(dxySig) <<endl; 
           slimmedJetLog10IPSig_.push_back( log10(dxySig) );
         }     

         // Alpha Calculation----------------------

         // loop over vertices (already looping over tracks and slimmedJets)
         if(dodebug){ printf("   Looping over vertices\n"); }
         int vtxIDminTrackDz=-1;
         float minPvDz = 10000.0; 
         for(unsigned int k = 0; k < vtxHandle->size(); ++k){

           // variation with fromPV > 1
           if(daughterPAT.fromPV(k) >1){
             if(dodebug){ printf("      (2+) associated with vtx %u\n",k); }

             sumtracksfPV2[k]  += daughterPt;
             sum2tracksfPV2[k] += daughterPt*daughterPt;
           }

           // variation with fromPV > 2
           if(daughterPAT.fromPV(k) >2){
             if(dodebug){ printf("      -(3+) associated with vtx %u\n",k); }

             sumtracksfPV3[k]  += daughterPt;
             sum2tracksfPV3[k] += daughterPt*daughterPt;
           }

           // vtxIDminTrackDz is the ID of the vtx with smallest dZ to this specific jet daughter
           // will need two loops
           //  first find the vtx id for min dz with track (must go through all vtxs)
           //  then (still looping over tracks) add track pt to sumtracksMinDZ[ vtx id w/ min dz to track ]
                //std::cout<<"  vtx trk DZ = "<<daughterPAT.dz(vtxHandle->at(k).position())<<std::endl;
           if(fabs(daughterPAT.dz(vtxHandle->at(k).position())) < minPvDz) {
            minPvDz = fabs(daughterPAT.dz(vtxHandle->at(k).position()));
            vtxIDminTrackDz = k;
           }
         }  // end looping over vertices
         if(dodebug){printf(" end of vertices, minPVDz was %f at id %i \n", minPvDz,vtxIDminTrackDz); }

         // vtxIDminTrackDz is ID of the best vtx for this track, add track pt to alpha numerator for this vtx
         sumtracksMinDzVtx[vtxIDminTrackDz]  += daughterPt;
         sum2tracksMinDzVtx[vtxIDminTrackDz] += daughterPt*daughterPt;

         sumneutraltracks += daughterPt;
         sum2neutraltracks += daughterPt*daughterPt;

         //track angle stuff  ----- FIXME
         //get jet axis 
         TVector3 JetAxis(iJet->px(),iJet->py(),iJet->pz());
         //get point of closest approach
         TVector3 Tang(slimmedJetDauVertex.x(),slimmedJetDauVertex.y(),slimmedJetDauVertex.z());
         //slimmedJetTrackPhi_.push_back(daughterPAT.phiAtVtx());
         slimmedJetTrackAngle_.push_back(Tang.Angle(JetAxis)); //acos(Tang*JetAxis/(MagTang*MagJetAxis)));
         slimmedJetLogTrackAngle_.push_back( log(fabs(Tang.Angle(JetAxis))) );//not sure if log or log10
         TotalTrackAngle += Tang.Angle(JetAxis);
         // reco::TransientTrack tt(trackHandle->at(id),magneticField_); 

       //printf(" end of track, minPVDz was at id %i \n", vtxIDminTrackDz); 
       } // if (daughterRECO->charge!=0)

       sumalltracks += daughterPt;
       sum2alltracks += daughterPt*daughterPt;

     } // if daughter exists, nonnull
   } // loop over tracks

   // if you want to save this to the ntuple, it should be vector< vector< float > > 
   //  don't forget that we have multiple jets
   slimmedJetAlphaD_.clear();

   //save alpha = sum over tracks associated with vtx / all tracks
   if(sumalltracks>0){
    for(unsigned z =0; z <sumtracksMinDzVtx.size(); z++){
     //std::cout<<"sum["<<z<<"] = "<<sumtracksMinDzVtx[z]<<" sumalltracks = "<<sumalltracks<<" div = "<<sumtracksMinDzVtx[z]/sumalltracks<<std::endl;
     slimmedJetAlphaD_.push_back(sumtracksMinDzVtx[z]/sumalltracks);
    }
   }
   else { slimmedJetAlphaD_.push_back(-99.9); }

   //set alphaMax variable out of range of true alpha max
   float alphaM = -99.9;
   //find the maximum 
   for (unsigned y = 0; y<slimmedJetAlphaD_.size(); y++){
    if(slimmedJetAlphaD_[y]>alphaM){ alphaM = slimmedJetAlphaD_[y];}
   }	
   // std::cout<<"  max = "<<alphaM<<std::endl;
   slimmedJetAlphaMaxD_.push_back(alphaM);



   //if(alphaM<0){
   // for(unsigned z =0; z <sumtracksMinDzVtx.size(); z++)
   // {
   //  std::cout<<sumtracksMinDzVtx[z]/sumalltracks<<" "<<sumtracksMinDzVtx[z]<<" "<<sumalltracks<<std::endl;
   // }
   //}

   //      vector<float> alphaNum_ = vector<float>(vtxHandle->size(),0.0);
   //alphaNum_.clear();
   // push back vector of track variables for this specific jet
   slimmedJetTrackPt_    .push_back( tmpTrackPt    );
   slimmedJetTrackEta_   .push_back( tmpTrackEta   );
   slimmedJetTrackPhi_   .push_back( tmpTrackPhi   );
   slimmedJetTrackPDGID_ .push_back( tmpTrackPDGID );

   if(dodebug){ 
    printf("   Done Loopong over tracks\n"); 
    //for(unsigned int k = 0; k < sumtracksfPV2->size(); ++k){
    for(unsigned int k = 0; k < sumtracksfPV2.size(); ++k){
       printf("    sum trackpt vtx %u  pt %f / %f \n", k,  sumtracksfPV2[k], sumalltracks );
     }
    }


   float slimmedJetAlphaMax_PV3onPV2 = -1;
   float slimmedJetAlphaMax_PV3onChg = -1;
   float slimmedJetAlphaMax_PV3onAll = -1;
   float slimmedJetAlphaMax_PV2onChg = -1;
   float slimmedJetAlphaMax_PV2onAll = -1;

   float slimmedJetAlpha_PV3onPV2 = -1;
   float slimmedJetAlpha_PV3onChg = -1;
   float slimmedJetAlpha_PV3onAll = -1;
   float slimmedJetAlpha_PV2onChg = -1;
   float slimmedJetAlpha_PV2onAll = -1;

   float slimmedJetAlpha2Max_PV3onPV2 = -1;
   float slimmedJetAlpha2Max_PV3onChg = -1;
   float slimmedJetAlpha2Max_PV3onAll = -1;
   float slimmedJetAlpha2Max_PV2onChg = -1;
   float slimmedJetAlpha2Max_PV2onAll = -1;

   float slimmedJetAlpha2_PV3onPV2 = -1;
   float slimmedJetAlpha2_PV3onChg = -1;
   float slimmedJetAlpha2_PV3onAll = -1;
   float slimmedJetAlpha2_PV2onChg = -1;
   float slimmedJetAlpha2_PV2onAll = -1;

   for(unsigned int q = 0; q < sumtracksfPV3.size(); q++){
     slimmedJetAlpha_PV3onPV2  = sumtracksfPV3[q]  / sumtracksfPV2[q] ;
     slimmedJetAlpha2_PV3onPV2 = sum2tracksfPV3[q] / sum2tracksfPV2[q];
     if(slimmedJetAlpha_PV3onPV2  > slimmedJetAlphaMax_PV3onPV2 ){ slimmedJetAlphaMax_PV3onPV2  = slimmedJetAlpha_PV3onPV2; }
     if(slimmedJetAlpha2_PV3onPV2 > slimmedJetAlpha2Max_PV3onPV2){ slimmedJetAlpha2Max_PV3onPV2 = slimmedJetAlpha2_PV3onPV2; }
   }

   for(unsigned int q = 0; q < sumtracksfPV3.size(); q++){
     slimmedJetAlpha_PV3onChg  = sumtracksfPV3[q]  / sumneutraltracks ;
     slimmedJetAlpha2_PV3onChg = sum2tracksfPV3[q] / sum2neutraltracks;
     if(slimmedJetAlpha_PV3onChg  > slimmedJetAlphaMax_PV3onChg ){ slimmedJetAlphaMax_PV3onChg  = slimmedJetAlpha_PV3onChg; }
     if(slimmedJetAlpha2_PV3onChg > slimmedJetAlpha2Max_PV3onChg){ slimmedJetAlpha2Max_PV3onChg = slimmedJetAlpha2_PV3onChg; }
   }

   for(unsigned int q = 0; q < sumtracksfPV3.size(); q++){
     slimmedJetAlpha_PV3onAll  = sumtracksfPV3[q]  / sumalltracks ;
     slimmedJetAlpha2_PV3onAll = sum2tracksfPV3[q] / sum2alltracks;
     if(slimmedJetAlpha_PV3onAll  > slimmedJetAlphaMax_PV3onAll ){ slimmedJetAlphaMax_PV3onAll  = slimmedJetAlpha_PV3onAll; }
     if(slimmedJetAlpha2_PV3onAll > slimmedJetAlpha2Max_PV3onAll){ slimmedJetAlpha2Max_PV3onAll = slimmedJetAlpha2_PV3onAll; }
   }

   for(unsigned int q = 0; q < sumtracksfPV2.size(); q++){
     slimmedJetAlpha_PV2onChg  = sumtracksfPV2[q]  / sumneutraltracks ;
     slimmedJetAlpha2_PV2onChg = sum2tracksfPV2[q] / sum2neutraltracks;
     if(slimmedJetAlpha_PV2onChg  > slimmedJetAlphaMax_PV2onChg ){ slimmedJetAlphaMax_PV2onChg  = slimmedJetAlpha_PV2onChg; }
     if(slimmedJetAlpha2_PV2onChg > slimmedJetAlpha2Max_PV2onChg){ slimmedJetAlpha2Max_PV2onChg = slimmedJetAlpha2_PV2onChg; }
   }

   for(unsigned int q = 0; q < sumtracksfPV2.size(); q++){
     slimmedJetAlpha_PV2onAll  = sumtracksfPV2[q]  / sumalltracks ;
     slimmedJetAlpha2_PV2onAll = sum2tracksfPV2[q] / sum2alltracks;
     if(slimmedJetAlpha_PV2onAll  > slimmedJetAlphaMax_PV2onAll ){ slimmedJetAlphaMax_PV2onAll  = slimmedJetAlpha_PV2onAll; }
     if(slimmedJetAlpha2_PV2onAll > slimmedJetAlpha2Max_PV2onAll){ slimmedJetAlpha2Max_PV2onAll = slimmedJetAlpha2_PV2onAll; }
   }


   slimmedJetAlphaMax_PV3onPV2_ .push_back(slimmedJetAlphaMax_PV3onPV2); 
   slimmedJetAlphaMax_PV3onChg_ .push_back(slimmedJetAlphaMax_PV3onChg); 
   slimmedJetAlphaMax_PV3onAll_ .push_back(slimmedJetAlphaMax_PV3onAll); 
   slimmedJetAlphaMax_PV2onChg_ .push_back(slimmedJetAlphaMax_PV2onChg); 
   slimmedJetAlphaMax_PV2onAll_ .push_back(slimmedJetAlphaMax_PV2onAll); 

   slimmedJetAlpha2Max_PV3onPV2_ .push_back(slimmedJetAlpha2Max_PV3onPV2);
   slimmedJetAlpha2Max_PV3onChg_ .push_back(slimmedJetAlpha2Max_PV3onChg);
   slimmedJetAlpha2Max_PV3onAll_ .push_back(slimmedJetAlpha2Max_PV3onAll);
   slimmedJetAlpha2Max_PV2onChg_ .push_back(slimmedJetAlpha2Max_PV2onChg);
   slimmedJetAlpha2Max_PV2onAll_ .push_back(slimmedJetAlpha2Max_PV2onAll);

   slimmedJetLeadTrackPID_.push_back(leadTrkPID);
   slimmedJetLeadTrackPt_ .push_back(leadTrkPt);
   slimmedJetLeadTrackEta_.push_back(leadTrkEta);
   slimmedJetLeadTrackPhi_.push_back(leadTrkPhi);
   slimmedJetLepTrackPID_ .push_back(lepTrkPID);
   slimmedJetLepTrackPt_  .push_back(lepTrkPt);
   slimmedJetLepTrackEta_ .push_back(lepTrkEta);
   slimmedJetLepTrackPhi_ .push_back(lepTrkPhi);    
   slimmedJetVtxPt_       .push_back(sqrt(pow(iJet->userFloat("vtxPx"),2)+pow(iJet->userFloat("vtxPy"),2)));
   slimmedJetVtxMass_     .push_back(iJet->userFloat("vtxMass"));
   slimmedJetVtxNtrks_    .push_back(iJet->userFloat("vtxNtracks"));
   slimmedJetVtx3DVal_    .push_back(iJet->userFloat("vtx3DVal"));
   slimmedJetVtx3DSig_    .push_back(iJet->userFloat("vtx3DSig"));
   
   //b-tagging
   slimmedJetCSV2BJetTags_           .push_back(iJet->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));
   slimmedJetJetProbabilityBJetTags_ .push_back(iJet->bDiscriminator("pfJetProbabilityBJetTags"));
   slimmedJetpfCombinedMVAV2BJetTags_.push_back(iJet->bDiscriminator("pfCombinedMVAV2BJetTags"));
 
   //parton id
   slimmedJetPartonID_.push_back(iJet->partonFlavour());
   slimmedJetHadFlvr_.push_back(iJet->hadronFlavour());

   // Displaced Jet Variable : median jet log IP significance
   // must sort IPsig to be able to find median
   sort(slimmedJetLog10IPSig_.begin(), slimmedJetLog10IPSig_.end());
   sort(slimmedJetLogTrackAngle_.begin(), slimmedJetLogTrackAngle_.end());

   //Now find median
   if(slimmedJetLog10IPSig_.size() == 0){
     IP0Sum+=1;
     slimmedJetMedianLog10IPSig_.push_back(-99.9); 
   }
   else if(slimmedJetLog10IPSig_.size()%2 ==0){
     MedianLog10IPSig = 0.5*( slimmedJetLog10IPSig_.at( (slimmedJetLog10IPSig_.size()/2) -1 ) 
                            + slimmedJetLog10IPSig_.at(  slimmedJetLog10IPSig_.size()/2     ));
     slimmedJetMedianLog10IPSig_.push_back(MedianLog10IPSig);
   }
   else{
     MedianLog10IPSig =       slimmedJetLog10IPSig_.at( (slimmedJetLog10IPSig_.size()-1)/2 );
     slimmedJetMedianLog10IPSig_.push_back(MedianLog10IPSig);
   }
   
   // Track Angle still broken FIXME
   if(slimmedJetLogTrackAngle_.size() == 0)
   {
    TA0Sum+=1;
   }
   else if(slimmedJetLogTrackAngle_.size()%2 ==0)
   {
    MedianLogTrackAngle = 0.5*( slimmedJetLogTrackAngle_[(slimmedJetLogTrackAngle_.size()/2) -1]
                              + slimmedJetLogTrackAngle_[ slimmedJetLogTrackAngle_.size()/2    ]);
    slimmedJetMedianLog10TrackAngle_.push_back(MedianLogTrackAngle);
   }
   else
   {
    MedianLogTrackAngle =       slimmedJetLogTrackAngle_[(slimmedJetLogTrackAngle_.size()-1)/2];
    slimmedJetMedianLog10TrackAngle_.push_back(MedianLogTrackAngle);
   }    

   //slimmedJetMedianLog10IPSig_.push_back(MedianLog10IPSig);
   slimmedJetSumIP_.push_back(SumIP);
   slimmedJetSumIPSig_.push_back(SumIPSig);

   if(slimmedJetLogTrackAngle_.size()>0)slimmedJetTotalTrackAngle_.push_back(TotalTrackAngle);
   else slimmedJetTotalTrackAngle_.push_back(0); // or -999?


   // gen jet and parton
   int slimmedJetGenPartonID    = -99;
   int slimmedJetGenPartonMomID = -99;
   float slimmedJetGenEn        = -999.;
   float slimmedJetGenPt        = -999.;
   float slimmedJetGenEta       = -999.;
   float slimmedJetGenPhi       = -999.;      
    
   float slimmedJetGenJetEn  = -999.;
   float slimmedJetGenJetPt  = -999.;
   float slimmedJetGenJetEta = -999.;
   float slimmedJetGenJetPhi = -999.;
   if( ! e.isRealData() ){
    if ((*iJet).genParton()) {
     slimmedJetGenPartonID = (*iJet).genParton()->pdgId();
     slimmedJetGenEn = (*iJet).genParton()->energy();
     slimmedJetGenPt = (*iJet).genParton()->pt();
     slimmedJetGenEta = (*iJet).genParton()->eta();
     slimmedJetGenPhi = (*iJet).genParton()->phi();
     if ((*iJet).genParton()->mother()) {
      slimmedJetGenPartonMomID = (*iJet).genParton()->mother()->pdgId();
     }
    }
     
   slimmedJetGenPartonID_.push_back(slimmedJetGenPartonID);
   slimmedJetGenPartonMomID_.push_back(slimmedJetGenPartonMomID);
   slimmedJetGenEn_ .push_back(slimmedJetGenEn);
   slimmedJetGenPt_ .push_back(slimmedJetGenPt);
   slimmedJetGenEta_ .push_back(slimmedJetGenEta);
   slimmedJetGenPhi_ .push_back(slimmedJetGenPhi);
   if ((*iJet).genJet()) {
    slimmedJetGenJetEn = (*iJet).genJet()->energy();
    slimmedJetGenJetPt = (*iJet).genJet()->pt();
    slimmedJetGenJetEta = (*iJet).genJet()->eta();
    slimmedJetGenJetPhi = (*iJet).genJet()->phi();
   }
   slimmedJetGenJetEn_.push_back(slimmedJetGenJetEn);
   slimmedJetGenJetPt_.push_back(slimmedJetGenJetPt);
   slimmedJetGenJetEta_.push_back(slimmedJetGenJetEta);
   slimmedJetGenJetPhi_.push_back(slimmedJetGenJetPhi);
   
   // access jet resolution       
   JME::JetParameters parameters;
   parameters.setJetPt(iJet->pt()).setJetEta(iJet->eta()).setRho(rho);
   float slimmedJetResolution = slimmedJetResolution_.getResolution(parameters);

    edm::Service<edm::RandomNumberGenerator> rng;
    if (!rng.isAvailable()) edm::LogError("JET : random number generator is missing !");
    CLHEP::HepRandomEngine & engine = rng->getEngine( e.streamID() );
    float rnd = CLHEP::RandGauss::shoot(&engine, 0., slimmedJetResolution);

    float slimmedJetResolutionSF   = slimmedJetResolutionSF_.getScaleFactor(parameters);
    float slimmedJetResolutionSFUp = slimmedJetResolutionSF_.getScaleFactor(parameters, Variation::UP);
    float slimmedJetResolutionSFDo = slimmedJetResolutionSF_.getScaleFactor(parameters, Variation::DOWN);

    float slimmedJetP4Smear   = -1.;
    float slimmedJetP4SmearUp = -1.;
    float slimmedJetP4SmearDo = -1.;
    if (slimmedJetGenJetPt > 0 && reco::deltaR(iJet->eta(), iJet->phi(), slimmedJetGenJetEta, slimmedJetGenJetPhi) < 0.2 && fabs(iJet->pt()-slimmedJetGenJetPt) < 3*slimmedJetResolution*iJet->pt()) {
      slimmedJetP4Smear   = 1. + (slimmedJetResolutionSF   - 1.)*(iJet->pt() - slimmedJetGenJetPt)/iJet->pt();
      slimmedJetP4SmearUp = 1. + (slimmedJetResolutionSFUp - 1.)*(iJet->pt() - slimmedJetGenJetPt)/iJet->pt();
      slimmedJetP4SmearDo = 1. + (slimmedJetResolutionSFDo - 1.)*(iJet->pt() - slimmedJetGenJetPt)/iJet->pt();
    } else {
      slimmedJetP4Smear   = 1. + rnd*sqrt(max(pow(slimmedJetResolutionSF,   2)-1, 0.));
      slimmedJetP4SmearUp = 1. + rnd*sqrt(max(pow(slimmedJetResolutionSFUp, 2)-1, 0.));
      slimmedJetP4SmearDo = 1. + rnd*sqrt(max(pow(slimmedJetResolutionSFDo, 2)-1, 0.));
    }
    slimmedJetP4Smear_  .push_back(slimmedJetP4Smear);
    slimmedJetP4SmearUp_.push_back(slimmedJetP4SmearUp);
    slimmedJetP4SmearDo_.push_back(slimmedJetP4SmearDo);
   }
   
   nSlimmedJets_++;
 }/// End Jets Loop
 
 if(dodebug){
   printf("    After jet loop, check all track vars\n");
   for(unsigned int k = 0; k < slimmedJetTrackPt_.size(); ++k){
     for(unsigned int l = 0; l < slimmedJetTrackPt_.at(k).size(); ++l){
       printf("    pt %f, eta %f, phi %f PDGID %i\n",
              slimmedJetTrackPt_.at(k).at(l), slimmedJetTrackEta_.at(k).at(l),
              slimmedJetTrackPhi_.at(k).at(l), slimmedJetTrackPDGID_.at(k).at(l) );
     }
   }
 }
 delete jecUnc;
 

 // ----------------------------------------------------------
 // AOD Section ----------------------------------------------
 
 bool verbose_AOD = false;
 
 // AOD Jet Handles
 e.getByToken( AODak4CaloJetsLabel_ ,  AODak4CaloJetsHandle  );   
 e.getByToken( AODak4PFJetsLabel_   ,  AODak4PFJetsHandle    );     
 e.getByToken( AODak4PFJetsCHSLabel_,  AODak4PFJetsCHSHandle );  
 e.getByToken( AODVertexLabel_      ,  AODVertexHandle );
 e.getByToken( AODTrackLabel_       ,  AODTrackHandle );

 // Magnetic field
 es.get<IdealMagneticFieldRecord>().get(magneticField);
 magneticField_ = &*magneticField;

 // beamspot
 e.getByToken(beamspotLabel_, beamspotHandle_);

 // clear values
 int nMissingInner = 0;
 int nMissingOuter = 0; 

 // propagator
 std::string thePropagatorName_ = "PropagatorWithMaterial";
 es.get<TrackingComponentsRecord>().get(thePropagatorName_,thePropagator_);
 StateOnTrackerBound stateOnTracker(thePropagator_.product());

 // Vertex
 vector<int> whichVertex_;
 whichVertex_.clear();
 whichVertex_ = vector<int>(AODTrackHandle->size(),-1);

 vtxfitter_ = new ConfigurableVertexReconstructor(lldj_pset_.getParameter<edm::ParameterSet>("vertexFitterConfig"));

 // clear master track vectors before starting track loop
 AODallTrackPositions       .clear(); 
 AODallTrackPt              .clear(); 
 AODallTrackEta             .clear(); 
 AODallTrackPhi             .clear(); 
 AODallTrackIFSPt           .clear(); 
 AODallTracknMissingInner   .clear(); 
 AODallTracknMissingOuter   .clear(); 
 AODallTrackAngle           .clear(); 
 AODwhichVertexByTrack      .clear(); 
 AODallTrackdxy             .clear(); 
 AODallTrackdxyerr          .clear(); 

 for(int j = 0; j < (int)AODTrackHandle->size(); j++){

  // get track j using the AOD track handle 
  reco::TrackBaseRef tref(AODTrackHandle,j);
  // make transient track (unfolding effects of B field ?)
  reco::TransientTrack tt(AODTrackHandle->at(j),magneticField_);

  if(!tt.isValid())continue;

  // track pt first
  float trackpt  = tref->pt();

  // make selections on track
  // for alphaMax, track angle we use ttIFSpt, not tref->pt()
  //   ---------!!!!--------------
  if (trackpt < minTrackPt_) continue;  // minimum pT for track
  if (!tref->quality(reco::TrackBase::highPurity)) continue; // track must be highPurity

  // find where track (no B field) would hit outer tracker
  FreeTrajectoryState fts = trajectoryStateTransform::initialFreeState(AODTrackHandle->at(j),magneticField_);
  TrajectoryStateOnSurface outer = stateOnTracker(fts);
  if(!outer.isValid()) continue;
  GlobalPoint outerPos = outer.globalPosition();
  TVector3 trackPos(outerPos.x(),outerPos.y(),outerPos.z());

  // push back track position to save in master vector
  AODallTrackPositions.push_back(trackPos);

  // track basics (trackpt above)
  float tracketa = tref->eta();
  float trackphi = tref->phi();
  float ttIFSpt  = tt.initialFreeState().momentum().transverse();
  AODallTrackPt .push_back(trackpt );  
  AODallTrackEta.push_back(tracketa);
  AODallTrackPhi.push_back(trackphi);
  AODallTrackIFSPt.push_back(ttIFSpt);

  /// Find best vertex associated with this track
  // we are on track j
  // loop over vertices
  // reassign index bestk if trackWeight is new max
  float maxWeight = 0; 
  int bestk = -1;
  for(int k = 0; k < (int)AODVertexHandle->size();k++){        
   if(AODVertexHandle->at(k).trackWeight(tref) > maxWeight){  
    maxWeight = AODVertexHandle->at(k).trackWeight(tref);    
    bestk = k;                                               
   }                                                          
  }                                                            
  AODwhichVertexByTrack.push_back(bestk); 
  
  // Number of missing tracker hits 
  nMissingInner = tref->hitPattern().numberOfLostTrackerHits(reco::HitPattern::MISSING_INNER_HITS);
  nMissingOuter = tref->hitPattern().numberOfLostTrackerHits(reco::HitPattern::MISSING_OUTER_HITS);
  AODallTracknMissingInner.push_back(nMissingInner) ;
  AODallTracknMissingOuter.push_back(nMissingOuter) ;

  /// For track angle
  // get track trajectory info
  static GetTrackTrajInfo getTrackTrajInfo; 
  vector<GetTrackTrajInfo::Result> trajInfo = getTrackTrajInfo.analyze(es, (*tref));
  if ( trajInfo.size() > 0 && trajInfo[0].valid) {
   // get inner tracker hit from trajectory state 
   const TrajectoryStateOnSurface& tsosInnerHit = trajInfo[0].detTSOS;

   //  here's the track angle
   // find beamspot x,y coordinates
   const reco::BeamSpot& pat_beamspot = (*beamspotHandle_);
   TVector2 bmspot(pat_beamspot.x0(),pat_beamspot.y0());
   // find track trajectory state on surface inner hit
   GlobalPoint  innerPos = tsosInnerHit.globalPosition();
   GlobalVector innerMom = tsosInnerHit.globalMomentum();
   
   // calculate the difference between inner hit and beamspot
   TVector2 sv(innerPos.x(),innerPos.y());
   TVector2 diff = (sv-bmspot);
   //cout<<"bs x: "<<bmspot.X()<<" y: "<<bmspot.Y()<<endl;
   //cout<<" sv x: "<<sv.X()<<" y: "<<sv.Y()<<endl;
   //cout<<" diff phi: "<<diff.Phi()<<endl;
   TVector2 momentum(innerMom.x(),innerMom.y());
   //cout<<" p x: "<<momentum.X()<<" y: "<<momentum.Y()<<endl;
   //cout<<" p phi: "<<momentum.Phi()<<endl;
   //cout<<" dPhi: "<<diff.DeltaPhi(momentum)<<endl;
   float ta = fabs( diff.DeltaPhi(momentum) ) ;

   AODallTrackAngle.push_back(ta);
  }
  else{ AODallTrackAngle.push_back(0); }

  // beamspot info, track impact parameter
  float dxy = fabs(tref->dxy(*beamspotHandle_));
  float dxyerr = tref->dxyError();
  if(verbose_AOD) printf(" dxy dxyerr: %0.4f %0.4f\n", dxy, dxyerr);
  AODallTrackdxy   .push_back(dxy   ) ;
  AODallTrackdxyerr.push_back(dxyerr) ;
   
 }//end track loop

 //Debug printing
 if(verbose_AOD){
  for(int j = 0; j < (int)AODTrackHandle->size(); j++){
   reco::TrackBaseRef tref(AODTrackHandle,j);
   printf("AOD track pt eta phi: %f %f %f\n",tref->pt(),tref->eta(),tref->phi());
  }
 
  printf("  AODallTrackPositions      %i \n",  (int)AODallTrackPositions    .size() ); 
  printf("  AODallTrackPt             %i \n",  (int)AODallTrackPt           .size() ); 
  printf("  AODallTrackEta            %i \n",  (int)AODallTrackEta          .size() ); 
  printf("  AODallTrackPhi            %i \n",  (int)AODallTrackPhi          .size() ); 
  printf("  AODallTrackIFSPt          %i \n",  (int)AODallTrackIFSPt        .size() ); 
  printf("  AODallTracknMissingInner  %i \n",  (int)AODallTracknMissingInner.size() ); 
  printf("  AODallTracknMissingOuter  %i \n",  (int)AODallTracknMissingOuter.size() ); 
  printf("  AODallTrackAngle          %i \n",  (int)AODallTrackAngle        .size() ); 
  printf("  AODwhichVertexByTrack     %i \n",  (int)AODwhichVertexByTrack   .size() ); 
  printf("  AODallTrackdxy            %i \n",  (int)AODallTrackdxy          .size() ); 
  printf("  AODallTrackdxyerr         %i \n",  (int)AODallTrackdxyerr       .size() ); 
 }


 // AOD Calo Jets -------------------------------------------
 for (edm::View<reco::CaloJet>::const_iterator iJet = AODak4CaloJetsHandle->begin(); iJet != AODak4CaloJetsHandle->end(); ++iJet) {

  if(verbose_AOD) printf("Calo %f \n",iJet->pt());
  
  float jetpt  = iJet->pt();
  float jeteta = iJet->eta();
  float jetphi = iJet->phi();

  // ID and jet selections
  bool passID = false;
  if( iJet->emEnergyFraction()>=0.0
   && iJet->emEnergyFraction()<=0.9
   && iJet->energyFractionHadronic()>=0.0
   && iJet->energyFractionHadronic()<=0.9)  passID = true; 

  if(iJet->pt()<20.0 || fabs(iJet->eta())>2.4 || !passID) continue;

  // caloJetTrackIDs is a vector of ints where each int is the 
  // index of a track passing deltaR requirement to this jet
  // out of the master track record of tracks passing basic selections
  vector<int>   caloJetTrackIDs = getJetTrackIndexs( jeteta, jetphi );
  if(caloJetTrackIDs.size()<1) continue;

  if(verbose_AOD){
   printf(" AOD Jet pt eta phi: %0.1f %0.1f %0.1f\n",jetpt,jeteta,jetphi);
   for( int i=0; i<(int)AODallTrackPositions.size(); i++){
    printf("  allTrack %i %0.1f %0.1f %0.1f \n",i,
     AODallTrackPt [i] ,
     AODallTrackEta[i] ,
     AODallTrackPhi[i] );

   }
   for( int i=0; i<(int)caloJetTrackIDs.size(); i++){
    printf(" Track %i at %i \n",i,caloJetTrackIDs[i]);
    printf("  caloTrack %i=%i %0.1f %0.1f %0.1f \n",i,
     caloJetTrackIDs[i],
     AODallTrackPt [caloJetTrackIDs[i]] ,
     AODallTrackEta[caloJetTrackIDs[i]] ,
     AODallTrackPhi[caloJetTrackIDs[i]] );
   }
  }

  // initialize variables
  float alphaMax,alphaMaxPrime,beta,alphaMax2,alphaMaxPrime2,beta2 = -1.;
  float totalTrackAngle, totalTrackAnglePt = 0.;
  float sumIP, sumIPSig = 0.;
  vector<float> caloJetTrackAngles; 
  caloJetTrackAngles.clear();
  vector<float> caloJetIPs; 
  caloJetIPs.clear();
  vector<float> caloJetIPSigs; 
  caloJetIPSigs.clear();

  // do calculations
  calculateAlphaMax(caloJetTrackIDs,alphaMax,alphaMaxPrime,beta,alphaMax2,alphaMaxPrime2,beta2);
  calculateTrackAngle(caloJetTrackIDs, caloJetTrackAngles, totalTrackAngle, totalTrackAnglePt);
  calculateIP(caloJetTrackIDs, caloJetIPs, caloJetIPSigs, sumIP, sumIPSig);
  calculateDisplacedVertices(es, caloJetTrackIDs);

  // find medians
  float medianTrackAngle;
  medianTrackAngle = findMedian(caloJetTrackAngles);
  float medianIP;
  medianIP = findMedian(caloJetIPs);
  float medianIPSig;
  medianIPSig = findMedian(caloJetIPSigs);

  ////////////////////////
  // Fill tree
  /////////////////////////
  AODnCaloJet_++;
  
  //Pt, Eta, Phi
  AODCaloJetPt_.push_back(jetpt);
  AODCaloJetEta_.push_back(jeteta);
  AODCaloJetPhi_.push_back(jetphi);
  
  //AlphaMax-type variables
  AODCaloJetAlphaMax_       .push_back(alphaMax      ) ; 
  AODCaloJetAlphaMax2_      .push_back(alphaMax2     ) ; 
  AODCaloJetAlphaMaxPrime_  .push_back(alphaMaxPrime ) ; 
  AODCaloJetAlphaMaxPrime2_ .push_back(alphaMaxPrime2) ; 
  AODCaloJetBeta_           .push_back(beta          ) ; 
  AODCaloJetBeta2_          .push_back(beta2         ) ; 

  //Totals
  AODCaloJetSumIP_.push_back(sumIP);
  AODCaloJetSumIPSig_.push_back(sumIPSig);

  AODCaloJetTotalTrackAngle_.push_back(totalTrackAngle);    

  /////Medians
  AODCaloJetMedianIP_             .push_back(medianIP);
  AODCaloJetMedianLog10IPSig_     .push_back(log10(medianIPSig));
  AODCaloJetMedianLog10TrackAngle_.push_back(log10(medianTrackAngle));

 }

 // AOD PF Jets -------------------------------------------
 for (edm::View<reco::PFJet>::const_iterator iJet = AODak4PFJetsHandle->begin(); iJet != AODak4PFJetsHandle->end(); ++iJet) {

  if(verbose_AOD) printf("PF %f \n",iJet->pt());
  
  float jetpt  = iJet->pt();
  float jeteta = iJet->eta();
  float jetphi = iJet->phi();

  // ID and jet selections
  bool pfjetPassLooseID = false;
  bool pfjetPassTightID = false;

  // from https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetID#Recommendations_for_13_TeV_2016
  float pfjetNHF                 = iJet->neutralHadronEnergyFraction();
  float pfjetNEMF                = iJet->neutralEmEnergyFraction();
  float pfjetCHF                 = iJet->chargedHadronEnergyFraction();
  float pfjetMUF                 = iJet->muonEnergyFraction();
  float pfjetCEMF                = iJet->chargedEmEnergyFraction();
  float pfjetNumConst            = iJet->chargedMultiplicity()+iJet->neutralMultiplicity();
  float pfjetNumNeutralParticle  = iJet->neutralMultiplicity();
  float pfjetCHM                 = iJet->chargedMultiplicity(); 

  if ( fabs(jeteta) <= 2.7 ){
   pfjetPassLooseID = (pfjetNHF<0.99 && pfjetNEMF<0.99 && pfjetNumConst>1) && ((abs(jeteta)<=2.4 && pfjetCHF>0 && pfjetCHM>0 && pfjetCEMF<0.99) || abs(jeteta)>2.4) && abs(jeteta)<=2.7 ;
   pfjetPassTightID = (pfjetNHF<0.90 && pfjetNEMF<0.90 && pfjetNumConst>1) && ((abs(jeteta)<=2.4 && pfjetCHF>0 && pfjetCHM>0 && pfjetCEMF<0.99) || abs(jeteta)>2.4) && abs(jeteta)<=2.7 ;
  }
  if ( fabs(jeteta) > 2.7 && fabs(jeteta) <= 3.0 ){
   pfjetPassLooseID = (pfjetNEMF<0.90 && pfjetNumNeutralParticle>2 ) ;
   pfjetPassTightID = (pfjetNEMF<0.90 && pfjetNumNeutralParticle>2 ) ;
  }
  if ( fabs(jeteta) > 3.0 ){
   pfjetPassLooseID = (pfjetNEMF<0.90 && pfjetNumNeutralParticle>10 ) ;
   pfjetPassTightID = (pfjetNEMF<0.90 && pfjetNumNeutralParticle>10 ) ; 
  }

  if(iJet->pt()<20.0 || fabs(iJet->eta())>2.4 || !pfjetPassLooseID) continue;

  // pfJetTrackIDs is a vector of ints where each int is the 
  // index of a track passing deltaR requirement to this jet
  // out of the master track record of tracks passing basic selections
  vector<int>   pfJetTrackIDs = getJetTrackIndexs( jeteta, jetphi );
  if(pfJetTrackIDs.size()<1) continue;

  if(verbose_AOD){
   printf(" AOD Jet pt eta phi: %0.1f %0.1f %0.1f\n",jetpt,jeteta,jetphi);
   for( int i=0; i<(int)AODallTrackPositions.size(); i++){
    printf("  allTrack %i %0.1f %0.1f %0.1f \n",i,
     AODallTrackPt [i] ,
     AODallTrackEta[i] ,
     AODallTrackPhi[i] );

   }
   for( int i=0; i<(int)pfJetTrackIDs.size(); i++){
    printf(" Track %i at %i \n",i,pfJetTrackIDs[i]);
    printf("  pfTrack %i=%i %0.1f %0.1f %0.1f \n",i,
     pfJetTrackIDs[i],
     AODallTrackPt [pfJetTrackIDs[i]] ,
     AODallTrackEta[pfJetTrackIDs[i]] ,
     AODallTrackPhi[pfJetTrackIDs[i]] );
   }
  }

  // initialize variables
  float alphaMax,alphaMaxPrime,beta,alphaMax2,alphaMaxPrime2,beta2 = -1.;
  float totalTrackAngle, totalTrackAnglePt = 0.;
  float sumIP, sumIPSig = 0.;
  vector<float> pfJetTrackAngles; 
  pfJetTrackAngles.clear();
  vector<float> pfJetIPs; 
  pfJetIPs.clear();
  vector<float> pfJetIPSigs; 
  pfJetIPSigs.clear();

  // do calculations
  calculateAlphaMax(pfJetTrackIDs,alphaMax,alphaMaxPrime,beta,alphaMax2,alphaMaxPrime2,beta2);
  calculateTrackAngle(pfJetTrackIDs, pfJetTrackAngles, totalTrackAngle, totalTrackAnglePt);
  calculateIP(pfJetTrackIDs, pfJetIPs, pfJetIPSigs, sumIP, sumIPSig);
  //calculateDisplacedVertices(es, pfJetTrackIDs);

  // find medians
  float medianTrackAngle;
  medianTrackAngle = findMedian(pfJetTrackAngles);
  float medianIP;
  medianIP = findMedian(pfJetIPs);
  float medianIPSig;
  medianIPSig = findMedian(pfJetIPSigs);

  ////////////////////////
  // Fill tree
  /////////////////////////
  AODnPFJet_++;
  
  //Pt, Eta, Phi
  AODPFJetPt_.push_back(jetpt);
  AODPFJetEta_.push_back(jeteta);
  AODPFJetPhi_.push_back(jetphi);
  
  //AlphaMax-type variables
  AODPFJetAlphaMax_       .push_back(alphaMax      ) ; 
  AODPFJetAlphaMax2_      .push_back(alphaMax2     ) ; 
  AODPFJetAlphaMaxPrime_  .push_back(alphaMaxPrime ) ; 
  AODPFJetAlphaMaxPrime2_ .push_back(alphaMaxPrime2) ; 
  AODPFJetBeta_           .push_back(beta          ) ; 
  AODPFJetBeta2_          .push_back(beta2         ) ; 

  //Totals
  AODPFJetSumIP_.push_back(sumIP);
  AODPFJetSumIPSig_.push_back(sumIPSig);

  AODPFJetTotalTrackAngle_.push_back(totalTrackAngle);    

  /////Medians
  AODPFJetMedianIP_             .push_back(medianIP);
  AODPFJetMedianLog10IPSig_     .push_back(log10(medianIPSig));
  AODPFJetMedianLog10TrackAngle_.push_back(log10(medianTrackAngle));

 }//end pf loop

 // AOD PFchs Jets -------------------------------------------
 for (edm::View<reco::PFJet>::const_iterator iJet = AODak4PFJetsCHSHandle->begin(); iJet != AODak4PFJetsCHSHandle->end(); ++iJet) {

  if(verbose_AOD) printf("PFchs %f \n",iJet->pt());
  
  float jetpt  = iJet->pt();
  float jeteta = iJet->eta();
  float jetphi = iJet->phi();

  // ID and jet selections
  bool pfchsjetPassLooseID = false;
  bool pfchsjetPassTightID = false;

  // from https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetID#Recommendations_for_13_TeV_2016
  float pfchsjetNHF                 = iJet->neutralHadronEnergyFraction();
  float pfchsjetNEMF                = iJet->neutralEmEnergyFraction();
  float pfchsjetCHF                 = iJet->chargedHadronEnergyFraction();
  float pfchsjetMUF                 = iJet->muonEnergyFraction();
  float pfchsjetCEMF                = iJet->chargedEmEnergyFraction();
  float pfchsjetNumConst            = iJet->chargedMultiplicity()+iJet->neutralMultiplicity();
  float pfchsjetNumNeutralParticle  = iJet->neutralMultiplicity();
  float pfchsjetCHM                 = iJet->chargedMultiplicity(); 

  if ( fabs(jeteta) <= 2.7 ){
   pfchsjetPassLooseID = (pfchsjetNHF<0.99 && pfchsjetNEMF<0.99 && pfchsjetNumConst>1) && ((abs(jeteta)<=2.4 && pfchsjetCHF>0 && pfchsjetCHM>0 && pfchsjetCEMF<0.99) || abs(jeteta)>2.4) && abs(jeteta)<=2.7 ;
   pfchsjetPassTightID = (pfchsjetNHF<0.90 && pfchsjetNEMF<0.90 && pfchsjetNumConst>1) && ((abs(jeteta)<=2.4 && pfchsjetCHF>0 && pfchsjetCHM>0 && pfchsjetCEMF<0.99) || abs(jeteta)>2.4) && abs(jeteta)<=2.7 ;
  }
  if ( fabs(jeteta) > 2.7 && fabs(jeteta) <= 3.0 ){
   pfchsjetPassLooseID = (pfchsjetNEMF<0.90 && pfchsjetNumNeutralParticle>2 ) ;
   pfchsjetPassTightID = (pfchsjetNEMF<0.90 && pfchsjetNumNeutralParticle>2 ) ;
  }
  if ( fabs(jeteta) > 3.0 ){
   pfchsjetPassLooseID = (pfchsjetNEMF<0.90 && pfchsjetNumNeutralParticle>10 ) ;
   pfchsjetPassTightID = (pfchsjetNEMF<0.90 && pfchsjetNumNeutralParticle>10 ) ; 
  }

  if(iJet->pt()<20.0 || fabs(iJet->eta())>2.4 || !pfchsjetPassLooseID) continue;

  // pfchsJetTrackIDs is a vector of ints where each int is the 
  // index of a track passing deltaR requirement to this jet
  // out of the master track record of tracks passing basic selections
  vector<int>   pfchsJetTrackIDs = getJetTrackIndexs( jeteta, jetphi );
  if(pfchsJetTrackIDs.size()<1) continue;

  if(verbose_AOD){
   printf(" AOD Jet pt eta phi: %0.1f %0.1f %0.1f\n",jetpt,jeteta,jetphi);
   for( int i=0; i<(int)AODallTrackPositions.size(); i++){
    printf("  allTrack %i %0.1f %0.1f %0.1f \n",i,
     AODallTrackPt [i] ,
     AODallTrackEta[i] ,
     AODallTrackPhi[i] );

   }
   for( int i=0; i<(int)pfchsJetTrackIDs.size(); i++){
    printf(" Track %i at %i \n",i,pfchsJetTrackIDs[i]);
    printf("  pfchsTrack %i=%i %0.1f %0.1f %0.1f \n",i,
     pfchsJetTrackIDs[i],
     AODallTrackPt [pfchsJetTrackIDs[i]] ,
     AODallTrackEta[pfchsJetTrackIDs[i]] ,
     AODallTrackPhi[pfchsJetTrackIDs[i]] );
   }
  }

  // initialize variables
  float alphaMax,alphaMaxPrime,beta,alphaMax2,alphaMaxPrime2,beta2 = -1.;
  float totalTrackAngle, totalTrackAnglePt = 0.;
  float sumIP, sumIPSig = 0.;
  vector<float> pfchsJetTrackAngles; 
  pfchsJetTrackAngles.clear();
  vector<float> pfchsJetIPs; 
  pfchsJetIPs.clear();
  vector<float> pfchsJetIPSigs; 
  pfchsJetIPSigs.clear();

  // do calculations
  calculateAlphaMax(pfchsJetTrackIDs,alphaMax,alphaMaxPrime,beta,alphaMax2,alphaMaxPrime2,beta2);
  calculateTrackAngle(pfchsJetTrackIDs, pfchsJetTrackAngles, totalTrackAngle, totalTrackAnglePt);
  calculateIP(pfchsJetTrackIDs, pfchsJetIPs, pfchsJetIPSigs, sumIP, sumIPSig);
  //calculateDisplacedVertices(es, pfchsJetTrackIDs);

  // find medians
  float medianTrackAngle;
  medianTrackAngle = findMedian(pfchsJetTrackAngles);
  float medianIP;
  medianIP = findMedian(pfchsJetIPs);
  float medianIPSig;
  medianIPSig = findMedian(pfchsJetIPSigs);

  ////////////////////////
  // Fill tree
  /////////////////////////
  AODnPFchsJet_++;
  
  //Pt, Eta, Phi
  AODPFchsJetPt_.push_back(jetpt);
  AODPFchsJetEta_.push_back(jeteta);
  AODPFchsJetPhi_.push_back(jetphi);
  
  //AlphaMax-type variables
  AODPFchsJetAlphaMax_       .push_back(alphaMax      ) ; 
  AODPFchsJetAlphaMax2_      .push_back(alphaMax2     ) ; 
  AODPFchsJetAlphaMaxPrime_  .push_back(alphaMaxPrime ) ; 
  AODPFchsJetAlphaMaxPrime2_ .push_back(alphaMaxPrime2) ; 
  AODPFchsJetBeta_           .push_back(beta          ) ; 
  AODPFchsJetBeta2_          .push_back(beta2         ) ; 

  //Totals
  AODPFchsJetSumIP_.push_back(sumIP);
  AODPFchsJetSumIPSig_.push_back(sumIPSig);

  AODPFchsJetTotalTrackAngle_.push_back(totalTrackAngle);    

  /////Medians
  AODPFchsJetMedianIP_             .push_back(medianIP);
  AODPFchsJetMedianLog10IPSig_     .push_back(log10(medianIPSig));
  AODPFchsJetMedianLog10TrackAngle_.push_back(log10(medianTrackAngle));

 }//end pf loop
 
}//end fill jets


vector<int> lldjNtuple::getJetTrackIndexs( float jeteta, float jetphi )
{
 vector<int> idvector;
 // loop over all selected tracks, dR match to jet
 for( int i=0; i<(int)AODallTrackPositions.size(); i++){
  float tracketa = AODallTrackPositions[i].Eta(); 
  float trackphi = AODallTrackPositions[i].Phi(); 
  float drt = deltaR( jeteta, jetphi, tracketa, trackphi );
  if(drt < maxDRtrackJet_){ idvector.push_back(i); }
 }
 return idvector;
}


void lldjNtuple::calculateAlphaMax(vector<int> jetTrackIDs, float& aMax, float& aMaxP, float& beta, float& aMax2, float& aMaxP2, float& beta2)
{

  float trackSumPt = 0; 
  float trackSumPt2 = 0; 
  float trackSumPtVtxMatched = 0; 
  float trackSumPtVtxMatched2 = 0; 

  int nrvtxs = AODVertexHandle->size();
  vector<float> trackSumPtByVtx( nrvtxs,0);
  vector<float> trackSumPtByVtx2(nrvtxs,0);

  // printf(" jetTracksIDs size: %i \n",(int)jetTrackIDs.size() );
  // printf(" AODwhichVertexByTrack size: %i \n",(int)AODwhichVertexByTrack.size() );

  for(int t=0; t< (int)jetTrackIDs.size(); t++){
   int trackID = jetTrackIDs[t];
  
   // sum pt of all tracks passing dR cut
   float trackpt = AODallTrackIFSPt[trackID];
   trackSumPt += trackpt;
   trackSumPt2 += (trackpt*trackpt);
  
   // now only for tracks associated with a vertex
   // the index of best vertex for track j is AODwhichVertexByTrack[j]
   if(AODwhichVertexByTrack[trackID] < 0)continue;

   // technically we could get this by summing over trackSumPtByVtx later
   trackSumPtVtxMatched += trackpt;
   trackSumPtVtxMatched2 += trackpt*trackpt;

   //// trackSumPtByVtx are sorted by vertex 
   //printf("  track %i to vtx %i TS %i\n",
   // trackID, AODwhichVertexByTrack[trackID],
   // (int)trackSumPtByVtx.size());
   trackSumPtByVtx[AODwhichVertexByTrack[trackID]] += trackpt;
   trackSumPtByVtx2[AODwhichVertexByTrack[trackID]] += (trackpt*trackpt);
  }
  
  // clear variables from previous execution
  double apMax =0;
  double apMax2 = 0;
  
  // calculate beta
  // beta = ratio of sum of track pt matched to any vertex / sum on all tracks
  beta = 1.0 - trackSumPtVtxMatched/trackSumPt;
  beta2 = 1.0 - trackSumPtVtxMatched2 / trackSumPt2;
  
  // calculate alpha
  // alpha[a] = sum pt tracks matched to vtx [a] / total track sum pt
  // ap[a] = sum pt tracks matched to vtx [a] / ( same sum + beta ) 
  float tmpMaxSumPt = -1.;
  for(int i = 0; i < nrvtxs; i++){
    // find vertex number of max pt sum
    if(trackSumPtByVtx[i] > tmpMaxSumPt){
     tmpMaxSumPt = trackSumPtByVtx[i];
    }
    // calculate and fill apMax 
    double ap = trackSumPtByVtx[i] / (trackSumPtByVtx[i] + beta);
    double ap2 = trackSumPtByVtx2[i] / (trackSumPtByVtx2[i] + beta2);
    if(ap > apMax) apMax = ap;
    if(ap2 > apMax2) apMax2 = ap2;
  }
  aMax   = tmpMaxSumPt  / trackSumPt;
  aMax2  = ( tmpMaxSumPt * tmpMaxSumPt ) / trackSumPt2;
  aMaxP  = apMax;
  aMaxP2 = apMax2;
  return;

}

void lldjNtuple::calculateTrackAngle(vector<int> jetTrackIDs, vector<float> &allTrackAngles,
 float &totalTrackAngle, float &totalTrackAnglePt)
{
  
  for(int t=0; t< (int)jetTrackIDs.size(); t++){
    int trackID = jetTrackIDs[t];
    // sum pt of all tracks passing dR cut
    float trackpt    = AODallTrackIFSPt[trackID];
    float trackangle = AODallTrackAngle[trackID];

    totalTrackAngle   += trackangle;
    totalTrackAnglePt += trackangle*trackpt;

    allTrackAngles.push_back(trackangle);
  }

  return;
  
}

void lldjNtuple::calculateIP(vector<int> jetTrackIDs, vector<float> &jetIPs, vector<float> &jetIPSigs, float &tsumIP, float &tsumIPSig)
{
  
  for(int t=0; t< (int)jetTrackIDs.size(); t++){

    int trackID = jetTrackIDs[t];
    // sum pt of all tracks passing dR cut
    float trackdxy    = AODallTrackdxy   [trackID];
    float trackdxyerr = AODallTrackdxyerr[trackID];
    float trackIPSig  = 0;
    if(trackdxyerr>0.) trackIPSig = trackdxy/trackdxyerr;

    tsumIP    += trackdxy;
    tsumIPSig += trackIPSig;

    //printf(" aa trackdyx: %0.4f %0.4f \n", trackdxy, trackIPSig);

    jetIPs.push_back(trackdxy);
    jetIPSigs.push_back(trackIPSig);

  }

  return;
  
}


float lldjNtuple::findMedian( vector<float> thevector){

 float themedian = -999. ;

 //printf(" thevector: ");
 //for ( int i=0; i<(int)thevector.size(); i++ ){
 // printf(" %0.4f ",thevector.at(i));
 //}
 //printf("\n");
 std::sort (thevector.begin(), thevector.end());
 //printf(" sorted thevector: ");
 //for ( int i=0; i<(int)thevector.size(); i++ ){
 // printf(" %0.4f ",thevector.at(i));
 //}
 //printf("\n");

 if(thevector.size() > 0){
  if((thevector.size()%2 == 0)){
   themedian = (
    thevector.at( (thevector.size() / 2) - 1 )
  + thevector.at( (thevector.size() / 2)     ) 
   ) / 2 ;
  } else {
   themedian = thevector.at( ( thevector.size() - 1 ) / 2 ) ;
  }
 }

 return themedian;

}

void lldjNtuple::deltaVertex3D(GlobalPoint secVert, std::vector<reco::TransientTrack> tracks, double& dEta, double& dPhi, double& pt, double& m, double& energy)
{
  TVector3 pv(AODVertexHandle->at(0).x(),AODVertexHandle->at(0).y(),AODVertexHandle->at(0).z());
  TVector3 sv(secVert.x(),secVert.y(),secVert.z());
  TVector3 diff = (sv-pv);
  TVector3 trackPt(0,0,0);
  TLorentzVector trackP4(0,0,0,0);
  for(int i = 0; i < (int)tracks.size(); i++){
    TVector3 tt;
    tt.SetPtEtaPhi(tracks[i].trajectoryStateClosestToPoint(secVert).momentum().transverse(),tracks[i].trajectoryStateClosestToPoint(secVert).momentum().eta(),tracks[i].trajectoryStateClosestToPoint(secVert).momentum().phi());
    trackPt += tt;
    trackP4 += TLorentzVector(tt,tracks[i].trajectoryStateClosestToPoint(secVert).momentum().mag());
  }
  dEta = diff.Eta()-trackPt.Eta();
  dPhi = diff.DeltaPhi(trackPt);
  pt = (trackPt - ((trackPt * diff)/(diff * diff) * diff)).Mag();
  m = trackP4.M();
  energy = trackP4.E();
}

void lldjNtuple::deltaVertex2D(GlobalPoint secVert, std::vector<reco::TransientTrack> tracks, double& dPhi, double& pt, double& mediandPhi)
{

  //edm::Handle<reco::BeamSpot> beamspotHandle_;//make this global??? BENTODO
  //e.getByToken(beamspotLabel_, beamspotHandle_);

  const reco::BeamSpot& pat_beamspot = (*beamspotHandle_);
  TVector2 bmspot(pat_beamspot.x0(),pat_beamspot.y0());
  TVector2 sv(secVert.x(),secVert.y());
  TVector2 diff = (sv-bmspot);
  TVector2 trackPt(0,0);
  vector<double> trackAngles;
  for(int i = 0; i < (int)tracks.size(); i++){
    TVector2 tt;
    tt.SetMagPhi(tracks[i].trajectoryStateClosestToPoint(secVert).momentum().transverse(),tracks[i].trajectoryStateClosestToPoint(secVert).momentum().phi());
    trackPt += tt;
    trackAngles.push_back(fabs(diff.DeltaPhi(tt)));
  }
  sort(trackAngles.begin(), trackAngles.end());

  if(trackAngles.size() == 0){
    //do nothing
  }else if((trackAngles.size()%2 == 0)){
    mediandPhi = trackAngles.at(trackAngles.size()/2-1);
  }else{
    mediandPhi = trackAngles.at((trackAngles.size()-1)/2);
  }

  dPhi = diff.DeltaPhi(trackPt);
  pt = (trackPt - ((trackPt * diff)/(diff * diff) * diff)).Mod();

}


vector<reco::TransientTrack> lldjNtuple::cleanTracks(vector<reco::TransientTrack> tracks, GlobalPoint vertPos)
{
  vector<reco::TransientTrack> cleanTracks;
  for(int i = 0; i < (int)tracks.size(); i++){
    if(tracks[i].trajectoryStateClosestToPoint(vertPos).perigeeError().transverseImpactParameterError() > 0 && tracks[i].trajectoryStateClosestToPoint(vertPos).perigeeParameters().transverseImpactParameter() / tracks[i].trajectoryStateClosestToPoint(vertPos).perigeeError().transverseImpactParameterError() > 3.0)continue;
    cleanTracks.push_back(tracks[i]);
  }
  return cleanTracks;
}



void lldjNtuple::calculateDisplacedVertices(const edm::EventSetup& es, vector<int> jetTrackIDs){
  
  //Select transient tracks for this jet
  vector<reco::TransientTrack> transientTracks;
  for(unsigned int j = 0; j < jetTrackIDs.size(); j++){
    reco::TransientTrack tt(AODTrackHandle->at( jetTrackIDs.at(j) ),magneticField_);
    transientTracks.push_back(tt);
  }
  
  if(transientTracks.size() > 1){
    vector<TransientVertex> avfVerts = vtxfitter_->vertices(transientTracks);
    if(avfVerts.size() > 0 && avfVerts[0].isValid()){
      GlobalPoint vertPos = avfVerts[0].position();
      GlobalError vertErr = avfVerts[0].positionError();
      
      AODCaloJetAvfVx_.push_back( vertPos.x() );
      AODCaloJetAvfVy_.push_back( vertPos.y() );
      AODCaloJetAvfVz_.push_back( vertPos.z() );
      
      float vertBeamXY = vertexBeam_->distanceToBeam(reco::Vertex(RecoVertex::convertPos(vertPos),RecoVertex::convertError(vertErr)));
      
      AODCaloJetAvfVertexTotalChiSquared_.push_back( avfVerts[0].totalChiSquared() );
      AODCaloJetAvfVertexDegreesOfFreedom_.push_back( avfVerts[0].degreesOfFreedom() );
      if(avfVerts[0].degreesOfFreedom() > 0) AODCaloJetAvfVertexChi2NDoF_.push_back( avfVerts[0].totalChiSquared()/avfVerts[0].degreesOfFreedom() );
       else AODCaloJetAvfVertexChi2NDoF_.push_back( 0. );
      AODCaloJetAvfVertexDistanceToBeam_.push_back( vertBeamXY );
      double rerr = vertErr.rerr(vertPos);
      AODCaloJetAvfVertexTransverseError_.push_back( rerr );
      if(rerr > 0) AODCaloJetAvfVertexTransverseSig_.push_back( vertBeamXY/rerr );
       else AODCaloJetAvfVertexTransverseSig_.push_back( 0. );
      
      vector<reco::TransientTrack> cleanTrackColl = cleanTracks(avfVerts[0].refittedTracks(),vertPos);
      
      double d3deta = 0, d3dphi = 0, d3dpt = 0, d3m = 0, d3en;
      deltaVertex3D(vertPos, cleanTrackColl,d3deta,d3dphi,d3dpt,d3m,d3en);
      AODCaloJetAvfVertexDeltaEta_.push_back( d3deta );
      AODCaloJetAvfVertexDeltaPhi_.push_back( d3dphi );
      AODCaloJetAvfVertexRecoilPt_.push_back( d3dpt );
      AODCaloJetAvfVertexTrackMass_.push_back( d3m );
      AODCaloJetAvfVertexTrackEnergy_.push_back( d3en );
      double d2dphi = 0,d2dpt = 0,d2dphiMed=1e-6;
      deltaVertex2D(vertPos,cleanTrackColl,d2dphi,d2dpt,d2dphiMed);
      AODCaloJetAvfBeamSpotDeltaPhi_.push_back( d2dphi );
      AODCaloJetAvfBeamSpotRecoilPt_.push_back( d2dpt );
      AODCaloJetAvfBeamSpotMedianDeltaPhi_.push_back( d2dphiMed );
      AODCaloJetAvfBeamSpotLog10MedianDeltaPhi_.push_back( log10(d2dphiMed) );
      AODCaloJetNCleanMatchedTracks_.push_back( (int)cleanTrackColl.size() );
      
      int nHitsInFront = 0;
      int nMissingAfter = 0;
      CheckHitPattern checkHitPattern_;
      for(int j = 0; j < (int)cleanTrackColl.size(); j++){
	CheckHitPattern::Result res = checkHitPattern_.analyze(es,cleanTrackColl[j].track(),avfVerts[0].vertexState(),false);
	nHitsInFront += res.hitsInFrontOfVert;
	nMissingAfter += res.missHitsAfterVert;
      }
      AODCaloJetSumHitsInFrontOfVert_.push_back( nHitsInFront );
      AODCaloJetSumMissHitsAfterVert_.push_back( nMissingAfter );
      AODCaloJetHitsInFrontOfVertPerTrack_.push_back( double(nHitsInFront)/double(transientTracks.size()) );
      AODCaloJetMissHitsAfterVertPerTrack_.push_back( double(nMissingAfter)/double(transientTracks.size()) );
      
      AODCaloJetAvfDistToPV_.push_back( 
				       sqrt(pow((AODVertexHandle->at(0).x() - avfVerts[0].position().x())/AODVertexHandle->at(0).x(),2)
					    +pow((AODVertexHandle->at(0).y() - avfVerts[0].position().y())/AODVertexHandle->at(0).y(),2)
					    +pow((AODVertexHandle->at(0).z() - avfVerts[0].position().z())/AODVertexHandle->at(0).z(),2)) );
      if(AODVertexHandle->size() > 0)AODCaloJetAvfVertexDeltaZtoPV_.push_back( AODVertexHandle->at(0).z() - avfVerts[0].position().z() );
       else AODCaloJetAvfVertexDeltaZtoPV_.push_back( 0. );
      if(AODVertexHandle->size() > 1)AODCaloJetAvfVertexDeltaZtoPV2_.push_back( AODVertexHandle->at(1).z() - avfVerts[0].position().z() );
       else AODCaloJetAvfVertexDeltaZtoPV2_.push_back( 0. );
      
      
    }//end valid avf vertex
    else{
     AODCaloJetAvfVx_.push_back( 0. );
     AODCaloJetAvfVy_.push_back( 0. );
     AODCaloJetAvfVz_.push_back( 0. );
     AODCaloJetAvfVertexTotalChiSquared_.push_back( 0. );
     AODCaloJetAvfVertexDegreesOfFreedom_.push_back( 0. );
     AODCaloJetAvfVertexChi2NDoF_.push_back( 0. );
     AODCaloJetAvfVertexDistanceToBeam_.push_back( 0. );
     AODCaloJetAvfVertexTransverseError_.push_back( 0. );
     AODCaloJetAvfVertexTransverseSig_.push_back( 0. );
     AODCaloJetAvfVertexDeltaEta_.push_back( 0. );
     AODCaloJetAvfVertexDeltaPhi_.push_back( 0. );
     AODCaloJetAvfVertexRecoilPt_.push_back( 0. );
     AODCaloJetAvfVertexTrackMass_.push_back( 0. );
     AODCaloJetAvfVertexTrackEnergy_.push_back( 0. );
     AODCaloJetAvfBeamSpotDeltaPhi_.push_back( 0. );
     AODCaloJetAvfBeamSpotRecoilPt_.push_back( 0. );
     AODCaloJetAvfBeamSpotMedianDeltaPhi_.push_back( 0. );
     AODCaloJetAvfBeamSpotLog10MedianDeltaPhi_.push_back( 0. );
     AODCaloJetNCleanMatchedTracks_.push_back( 0. );
     AODCaloJetSumHitsInFrontOfVert_.push_back( 0. );
     AODCaloJetSumMissHitsAfterVert_.push_back( 0. );
     AODCaloJetHitsInFrontOfVertPerTrack_.push_back( 0. );
     AODCaloJetMissHitsAfterVertPerTrack_.push_back( 0. );
     AODCaloJetAvfDistToPV_.push_back( 0. );
     AODCaloJetAvfVertexDeltaZtoPV_.push_back( 0. );
     AODCaloJetAvfVertexDeltaZtoPV2_.push_back( 0. );
    }

  }//end if transientTracks
  else{
   AODCaloJetAvfVx_.push_back( 0. );
   AODCaloJetAvfVy_.push_back( 0. );
   AODCaloJetAvfVz_.push_back( 0. );
   AODCaloJetAvfVertexTotalChiSquared_.push_back( 0. );
   AODCaloJetAvfVertexDegreesOfFreedom_.push_back( 0. );
   AODCaloJetAvfVertexChi2NDoF_.push_back( 0. );
   AODCaloJetAvfVertexDistanceToBeam_.push_back( 0. );
   AODCaloJetAvfVertexTransverseError_.push_back( 0. );
   AODCaloJetAvfVertexTransverseSig_.push_back( 0. );
   AODCaloJetAvfVertexDeltaEta_.push_back( 0. );
   AODCaloJetAvfVertexDeltaPhi_.push_back( 0. );
   AODCaloJetAvfVertexRecoilPt_.push_back( 0. );
   AODCaloJetAvfVertexTrackMass_.push_back( 0. );
   AODCaloJetAvfVertexTrackEnergy_.push_back( 0. );
   AODCaloJetAvfBeamSpotDeltaPhi_.push_back( 0. );
   AODCaloJetAvfBeamSpotRecoilPt_.push_back( 0. );
   AODCaloJetAvfBeamSpotMedianDeltaPhi_.push_back( 0. );
   AODCaloJetAvfBeamSpotLog10MedianDeltaPhi_.push_back( 0. );
   AODCaloJetNCleanMatchedTracks_.push_back( 0. );
   AODCaloJetSumHitsInFrontOfVert_.push_back( 0. );
   AODCaloJetSumMissHitsAfterVert_.push_back( 0. );
   AODCaloJetHitsInFrontOfVertPerTrack_.push_back( 0. );
   AODCaloJetMissHitsAfterVertPerTrack_.push_back( 0. );
   AODCaloJetAvfDistToPV_.push_back( 0. );
   AODCaloJetAvfVertexDeltaZtoPV_.push_back( 0. );
   AODCaloJetAvfVertexDeltaZtoPV2_.push_back( 0. );
  }

}//end calculateDisplacedVertices
