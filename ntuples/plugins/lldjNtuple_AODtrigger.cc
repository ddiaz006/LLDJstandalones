// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"

#include "LLDJstandalones/ntuples/interface/lldjNtuple.h"

// (local) variables associated with tree branches

ULong64_t   AOD_HLT_Ele23Loose_;
ULong64_t   AOD_HLT_Ele27Tight_;
ULong64_t   AOD_HLT_Ele17Ele12_;
ULong64_t   AOD_HLT_Ele23Ele12_;

ULong64_t   AOD_HLT_IsoMu22_;
ULong64_t   AOD_HLT_IsoTkMu22_;
ULong64_t   AOD_HLT_Mu17Mu8_;
ULong64_t   AOD_HLT_Mu17TkMu8_;

// // triggers used in VHbb
//  // https://indico.cern.ch/event/655639/contributions/2670493/attachments/1497203/2337123/VHbb_Approval_28_07_17_v10.pdf#page=8
// ULong64_t   AOD_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ
// 
// ULong64_t   AOD_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v* 
// ULong64_t   AOD_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v*  
// ULong64_t   AOD_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v* 
// ULong64_t   AOD_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v* 
// 
// ULong64_t   AOD_HLT_Ele27_WPTight_Gsf
// 
// ULong64_t   AOD_HLT_IsoMu24
// ULong64_t   AOD_HLT_IsoTkMu24


 // Full trigger list 
// HLT_AK8PFJet360_TrimMass30_v7
// HLT_AK8PFJet400_TrimMass30_v1
// HLT_AK8PFHT750_TrimMass50_v1
// HLT_AK8PFHT800_TrimMass50_v1
// HLT_AK8DiPFJet300_200_TrimMass30_BTagCSV_p20_v1
// HLT_AK8DiPFJet280_200_TrimMass30_BTagCSV_p087_v1
// HLT_AK8DiPFJet300_200_TrimMass30_BTagCSV_p087_v1
// HLT_AK8DiPFJet300_200_TrimMass30_v1
// HLT_AK8PFHT700_TrimR0p1PT0p03Mass50_v8
// HLT_AK8PFHT650_TrimR0p1PT0p03Mass50_v7
// HLT_AK8PFHT600_TrimR0p1PT0p03Mass50_BTagCSV_p20_v6
// HLT_AK8DiPFJet280_200_TrimMass30_v5
// HLT_AK8DiPFJet250_200_TrimMass30_v5
// HLT_AK8DiPFJet280_200_TrimMass30_BTagCSV_p20_v5
// HLT_AK8DiPFJet250_200_TrimMass30_BTagCSV_p20_v5
// HLT_CaloJet260_v4
// HLT_CaloJet500_NoJetID_v5
// HLT_Dimuon13_PsiPrime_v6
// HLT_Dimuon13_Upsilon_v6
// HLT_Dimuon20_Jpsi_v6
// HLT_DoubleEle24_22_eta2p1_WPLoose_Gsf_v8
// HLT_DoubleEle25_CaloIdL_GsfTrkIdVL_v7
// HLT_DoubleEle33_CaloIdL_v7
// HLT_DoubleEle33_CaloIdL_MW_v8
// HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW_v10
// HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_v9
// HLT_DoubleMediumCombinedIsoPFTau35_Trk1_eta2p1_Reg_v2
// HLT_DoubleTightCombinedIsoPFTau35_Trk1_eta2p1_Reg_v2
// HLT_DoubleMediumCombinedIsoPFTau40_Trk1_eta2p1_Reg_v1
// HLT_DoubleTightCombinedIsoPFTau40_Trk1_eta2p1_Reg_v1
// HLT_DoubleMediumCombinedIsoPFTau40_Trk1_eta2p1_v1
// HLT_DoubleTightCombinedIsoPFTau40_Trk1_eta2p1_v1
// HLT_DoubleMediumIsoPFTau35_Trk1_eta2p1_Reg_v5
// HLT_DoubleMediumIsoPFTau40_Trk1_eta2p1_Reg_v7
// HLT_DoubleMediumIsoPFTau40_Trk1_eta2p1_v7
// HLT_DoubleEle37_Ele27_CaloIdL_GsfTrkIdVL_v7
// HLT_DoubleMu33NoFiltersNoVtx_v5
// HLT_DoubleMu38NoFiltersNoVtx_v5
// HLT_DoubleMu23NoFiltersNoVtxDisplaced_v5
// HLT_DoubleMu28NoFiltersNoVtxDisplaced_v5
// HLT_DoubleMu0_v2
// HLT_DoubleMu4_3_Bs_v7
// HLT_DoubleMu4_3_Jpsi_Displaced_v7
// HLT_DoubleMu4_JpsiTrk_Displaced_v7
// HLT_DoubleMu4_LowMassNonResonantTrk_Displaced_v7
// HLT_DoubleMu3_Trk_Tau3mu_v4
// HLT_DoubleMu4_PsiPrimeTrk_Displaced_v7
// HLT_Mu7p5_L2Mu2_Jpsi_v4
// HLT_Mu7p5_L2Mu2_Upsilon_v4
// HLT_Mu7p5_Track2_Jpsi_v4
// HLT_Mu7p5_Track3p5_Jpsi_v4
// HLT_Mu7p5_Track7_Jpsi_v4
// HLT_Mu7p5_Track2_Upsilon_v4
// HLT_Mu7p5_Track3p5_Upsilon_v4
// HLT_Mu7p5_Track7_Upsilon_v4
// HLT_Dimuon0er16_Jpsi_NoOS_NoVertexing_v4
// HLT_Dimuon0er16_Jpsi_NoVertexing_v4
// HLT_Dimuon6_Jpsi_NoVertexing_v4
// HLT_Photon150_v5
// HLT_Photon90_CaloIdL_HT300_v5
// HLT_HT250_CaloMET70_v3
// HLT_DoublePhoton60_v7
// HLT_DoublePhoton85_v8
// HLT_Ele17_Ele8_Gsf_v8
// HLT_Ele20_eta2p1_WPLoose_Gsf_LooseIsoPFTau28_v3
// HLT_Ele22_eta2p1_WPLoose_Gsf_LooseIsoPFTau29_v3
// HLT_Ele22_eta2p1_WPLoose_Gsf_v9
// HLT_Ele22_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v9
// HLT_Ele23_WPLoose_Gsf_v9
// HLT_Ele23_WPLoose_Gsf_WHbbBoost_v9
// HLT_Ele24_eta2p1_WPLoose_Gsf_v7
// HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_v8
// HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v8
// HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau30_v4
// HLT_Ele25_WPTight_Gsf_v7
// HLT_Ele25_eta2p1_WPLoose_Gsf_v7
// HLT_Ele25_eta2p1_WPTight_Gsf_v7
// HLT_Ele27_WPLoose_Gsf_v7
// HLT_Ele27_WPLoose_Gsf_WHbbBoost_v9
// HLT_Ele27_WPTight_Gsf_v7
// HLT_Ele27_WPTight_Gsf_L1JetTauSeeded_v4
// HLT_Ele27_eta2p1_WPLoose_Gsf_v8
// HLT_Ele27_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v8
// HLT_Ele27_eta2p1_WPTight_Gsf_v8
// HLT_Ele30_WPTight_Gsf_v1
// HLT_Ele30_eta2p1_WPLoose_Gsf_v1
// HLT_Ele30_eta2p1_WPTight_Gsf_v1
// HLT_Ele32_WPTight_Gsf_v1
// HLT_Ele32_eta2p1_WPLoose_Gsf_v1
// HLT_Ele32_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v8
// HLT_Ele32_eta2p1_WPTight_Gsf_v8
// HLT_Ele35_WPLoose_Gsf_v7
// HLT_Ele35_CaloIdVT_GsfTrkIdT_PFJet150_PFJet50_v7
// HLT_Ele36_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v3
// HLT_Ele45_WPLoose_Gsf_v7
// HLT_Ele45_WPLoose_Gsf_L1JetTauSeeded_v4
// HLT_Ele45_CaloIdVT_GsfTrkIdT_PFJet200_PFJet50_v9
// HLT_Ele105_CaloIdVT_GsfTrkIdT_v8
// HLT_Ele30WP60_SC4_Mass55_v9
// HLT_Ele30WP60_Ele8_Mass55_v8
// HLT_HT200_v4
// HLT_HT275_v4
// HLT_HT325_v4
// HLT_HT425_v4
// HLT_HT575_v4
// HLT_HT410to430_v4
// HLT_HT430to450_v4
// HLT_HT450to470_v4
// HLT_HT470to500_v4
// HLT_HT500to550_v4
// HLT_HT550to650_v4
// HLT_HT650_v5
// HLT_Mu16_eta2p1_MET30_v4
// HLT_IsoMu16_eta2p1_MET30_v4
// HLT_IsoMu16_eta2p1_MET30_LooseIsoPFTau50_Trk30_eta2p1_v5
// HLT_IsoMu17_eta2p1_v6
// HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v8
// HLT_IsoMu17_eta2p1_LooseIsoPFTau20_SingleL1_v8
// HLT_DoubleIsoMu17_eta2p1_v7
// HLT_DoubleIsoMu17_eta2p1_noDzCut_v5
// HLT_IsoMu18_v5
// HLT_IsoMu19_eta2p1_LooseIsoPFTau20_v5
// HLT_IsoMu19_eta2p1_LooseIsoPFTau20_SingleL1_v5
// HLT_IsoMu19_eta2p1_MediumIsoPFTau32_Trk1_eta2p1_Reg_v5
// HLT_IsoMu19_eta2p1_LooseCombinedIsoPFTau20_v1
// HLT_IsoMu19_eta2p1_MediumCombinedIsoPFTau32_Trk1_eta2p1_Reg_v1
// HLT_IsoMu19_eta2p1_TightCombinedIsoPFTau32_Trk1_eta2p1_Reg_v1
// HLT_IsoMu21_eta2p1_MediumCombinedIsoPFTau32_Trk1_eta2p1_Reg_v1
// HLT_IsoMu21_eta2p1_TightCombinedIsoPFTau32_Trk1_eta2p1_Reg_v1
// HLT_IsoMu20_v6
// HLT_IsoMu21_eta2p1_LooseIsoPFTau20_SingleL1_v5
// HLT_IsoMu21_eta2p1_LooseIsoPFTau50_Trk30_eta2p1_SingleL1_v4
// HLT_IsoMu21_eta2p1_MediumIsoPFTau32_Trk1_eta2p1_Reg_v5
// HLT_IsoMu22_v5
// HLT_IsoMu22_eta2p1_v4
// HLT_IsoMu24_v4
// HLT_IsoMu27_v7
// HLT_IsoTkMu18_v5
// HLT_IsoTkMu20_v7
// HLT_IsoTkMu22_v5
// HLT_IsoTkMu22_eta2p1_v4
// HLT_IsoTkMu24_v4
// HLT_IsoTkMu27_v7
// HLT_JetE30_NoBPTX3BX_v4
// HLT_JetE30_NoBPTX_v4
// HLT_JetE50_NoBPTX3BX_v3
// HLT_JetE70_NoBPTX3BX_v3
// HLT_L1SingleMu18_v1
// HLT_L2Mu10_v3
// HLT_L1SingleMuOpen_v3
// HLT_L1SingleMuOpen_DT_v3
// HLT_L2DoubleMu23_NoVertex_v6
// HLT_L2DoubleMu28_NoVertex_2Cha_Angle2p5_Mass10_v6
// HLT_L2DoubleMu38_NoVertex_2Cha_Angle2p5_Mass10_v6
// HLT_L2Mu10_NoVertex_NoBPTX3BX_v2
// HLT_L2Mu10_NoVertex_NoBPTX_v3
// HLT_L2Mu45_NoVertex_3Sta_NoBPTX3BX_v1
// HLT_L2Mu40_NoVertex_3Sta_NoBPTX3BX_v2
// HLT_LooseIsoPFTau50_Trk30_eta2p1_v7
// HLT_LooseIsoPFTau50_Trk30_eta2p1_MET80_v5
// HLT_LooseIsoPFTau50_Trk30_eta2p1_MET90_v5
// HLT_LooseIsoPFTau50_Trk30_eta2p1_MET110_v5
// HLT_LooseIsoPFTau50_Trk30_eta2p1_MET120_v5
// HLT_PFTau120_eta2p1_v5
// HLT_PFTau140_eta2p1_v5
// HLT_VLooseIsoPFTau120_Trk50_eta2p1_v5
// HLT_VLooseIsoPFTau140_Trk50_eta2p1_v5
// HLT_Mu17_Mu8_v5
// HLT_Mu17_Mu8_DZ_v7
// HLT_Mu17_Mu8_SameSign_v5
// HLT_Mu17_Mu8_SameSign_DZ_v6
// HLT_Mu20_Mu10_v5
// HLT_Mu20_Mu10_DZ_v6
// HLT_Mu20_Mu10_SameSign_v4
// HLT_Mu20_Mu10_SameSign_DZ_v6
// HLT_Mu17_TkMu8_DZ_v6
// HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v6
// HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v7
// HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v5
// HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v6
// HLT_Mu25_TkMu0_dEta18_Onia_v6
// HLT_Mu27_TkMu8_v5
// HLT_Mu30_TkMu11_v5
// HLT_Mu30_eta2p1_PFJet150_PFJet50_v5
// HLT_Mu40_TkMu11_v5
// HLT_Mu40_eta2p1_PFJet200_PFJet50_v7
// HLT_Mu20_v4
// HLT_TkMu17_v1
// HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v3
// HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v3
// HLT_TkMu20_v4
// HLT_Mu24_eta2p1_v5
// HLT_TkMu24_eta2p1_v5
// HLT_Mu27_v5
// HLT_TkMu27_v5
// HLT_Mu45_eta2p1_v5
// HLT_Mu50_v5
// HLT_TkMu50_v3
// HLT_Mu38NoFiltersNoVtx_Photon38_CaloIdL_v7
// HLT_Mu42NoFiltersNoVtx_Photon42_CaloIdL_v7
// HLT_Mu28NoFiltersNoVtxDisplaced_Photon28_CaloIdL_v7
// HLT_Mu33NoFiltersNoVtxDisplaced_Photon33_CaloIdL_v7
// HLT_Mu23NoFiltersNoVtx_Photon23_CaloIdL_v7
// HLT_DoubleMu18NoFiltersNoVtx_v5
// HLT_Mu33NoFiltersNoVtxDisplaced_DisplacedJet50_Tight_v5
// HLT_Mu33NoFiltersNoVtxDisplaced_DisplacedJet50_Loose_v5
// HLT_Mu28NoFiltersNoVtx_DisplacedJet40_Loose_v5
// HLT_Mu38NoFiltersNoVtxDisplaced_DisplacedJet60_Tight_v5
// HLT_Mu38NoFiltersNoVtxDisplaced_DisplacedJet60_Loose_v5
// HLT_Mu38NoFiltersNoVtx_DisplacedJet60_Loose_v5
// HLT_Mu28NoFiltersNoVtx_CentralCaloJet40_v5
// HLT_PFHT300_PFMET100_v6
// HLT_PFHT300_PFMET110_v6
// HLT_PFHT550_4JetPt50_v6
// HLT_PFHT650_4JetPt50_v6
// HLT_PFHT750_4JetPt50_v8
// HLT_PFHT750_4JetPt70_v2
// HLT_PFHT750_4JetPt80_v2
// HLT_PFHT800_4JetPt50_v2
// HLT_PFHT850_4JetPt50_v2
// HLT_PFJet15_NoCaloMatched_v7
// HLT_PFJet25_NoCaloMatched_v5
// HLT_DiPFJet15_NoCaloMatched_v5
// HLT_DiPFJet25_NoCaloMatched_v5
// HLT_DiPFJet15_FBEta3_NoCaloMatched_v6
// HLT_DiPFJet25_FBEta3_NoCaloMatched_v6
// HLT_DiPFJetAve15_HFJEC_v6
// HLT_DiPFJetAve25_HFJEC_v6
// HLT_DiPFJetAve35_HFJEC_v6
// HLT_AK8PFJet40_v5
// HLT_AK8PFJet60_v4
// HLT_AK8PFJet80_v4
// HLT_AK8PFJet140_v4
// HLT_AK8PFJet200_v4
// HLT_AK8PFJet260_v5
// HLT_AK8PFJet320_v5
// HLT_AK8PFJet400_v5
// HLT_AK8PFJet450_v5
// HLT_AK8PFJet500_v5
// HLT_PFJet40_v9
// HLT_PFJet60_v9
// HLT_PFJet80_v8
// HLT_PFJet140_v8
// HLT_PFJet200_v8
// HLT_PFJet260_v9
// HLT_PFJet320_v9
// HLT_PFJet400_v9
// HLT_PFJet450_v9
// HLT_PFJet500_v9
// HLT_DiPFJetAve40_v8
// HLT_DiPFJetAve60_v8
// HLT_DiPFJetAve80_v7
// HLT_DiPFJetAve140_v7
// HLT_DiPFJetAve200_v7
// HLT_DiPFJetAve260_v8
// HLT_DiPFJetAve320_v8
// HLT_DiPFJetAve400_v8
// HLT_DiPFJetAve500_v8
// HLT_DiPFJetAve60_HFJEC_v8
// HLT_DiPFJetAve80_HFJEC_v8
// HLT_DiPFJetAve100_HFJEC_v8
// HLT_DiPFJetAve160_HFJEC_v8
// HLT_DiPFJetAve220_HFJEC_v9
// HLT_DiPFJetAve300_HFJEC_v9
// HLT_DiPFJet40_DEta3p5_MJJ600_PFMETNoMu140_v5
// HLT_DiPFJet40_DEta3p5_MJJ600_PFMETNoMu80_v5
// HLT_DiCentralPFJet170_v5
// HLT_SingleCentralPFJet170_CFMax0p1_v5
// HLT_DiCentralPFJet170_CFMax0p1_v5
// HLT_DiCentralPFJet220_CFMax0p3_v5
// HLT_DiCentralPFJet330_CFMax0p5_v5
// HLT_DiCentralPFJet430_v5
// HLT_PFHT125_v5
// HLT_PFHT200_v6
// HLT_PFHT250_v6
// HLT_PFHT300_v7
// HLT_PFHT350_v8
// HLT_PFHT400_v7
// HLT_PFHT475_v7
// HLT_PFHT600_v8
// HLT_PFHT650_v8
// HLT_PFHT800_v7
// HLT_PFHT900_v6
// HLT_PFHT200_PFAlphaT0p51_v7
// HLT_PFHT200_DiPFJetAve90_PFAlphaT0p57_v7
// HLT_PFHT200_DiPFJetAve90_PFAlphaT0p63_v7
// HLT_PFHT250_DiPFJetAve90_PFAlphaT0p55_v7
// HLT_PFHT250_DiPFJetAve90_PFAlphaT0p58_v7
// HLT_PFHT300_DiPFJetAve90_PFAlphaT0p53_v7
// HLT_PFHT300_DiPFJetAve90_PFAlphaT0p54_v7
// HLT_PFHT350_DiPFJetAve90_PFAlphaT0p52_v7
// HLT_PFHT350_DiPFJetAve90_PFAlphaT0p53_v7
// HLT_PFHT400_DiPFJetAve90_PFAlphaT0p51_v7
// HLT_PFHT400_DiPFJetAve90_PFAlphaT0p52_v7
// HLT_MET60_IsoTrk35_Loose_v3
// HLT_MET75_IsoTrk50_v5
// HLT_MET90_IsoTrk50_v5
// HLT_PFMET120_BTagCSV_p067_v6
// HLT_PFMET120_Mu5_v6
// HLT_PFMET170_NotCleaned_v7
// HLT_PFMET170_NoiseCleaned_v8
// HLT_PFMET170_HBHECleaned_v8
// HLT_PFMET170_JetIdCleaned_v7
// HLT_PFMET170_BeamHaloCleaned_v6
// HLT_PFMET170_HBHE_BeamHaloCleaned_v4
// HLT_PFMETTypeOne190_HBHE_BeamHaloCleaned_v4
// HLT_PFMET90_PFMHT90_IDTight_v7
// HLT_PFMET100_PFMHT100_IDTight_v7
// HLT_PFMET100_PFMHT100_IDTight_BeamHaloCleaned_v4
// HLT_PFMET110_PFMHT110_IDTight_v7
// HLT_PFMET120_PFMHT120_IDTight_v7
// HLT_CaloMHTNoPU90_PFMET90_PFMHT90_IDTight_BTagCSV_p067_v6
// HLT_CaloMHTNoPU90_PFMET90_PFMHT90_IDTight_v7
// HLT_QuadPFJet_BTagCSV_p016_p11_VBF_Mqq200_v5
// HLT_QuadPFJet_BTagCSV_p016_VBF_Mqq460_v5
// HLT_QuadPFJet_BTagCSV_p016_p11_VBF_Mqq240_v5
// HLT_QuadPFJet_BTagCSV_p016_VBF_Mqq500_v5
// HLT_QuadPFJet_VBF_v8
// HLT_L1_TripleJet_VBF_v5
// HLT_QuadJet45_TripleBTagCSV_p087_v6
// HLT_QuadJet45_DoubleBTagCSV_p087_v6
// HLT_DoubleJet90_Double30_TripleBTagCSV_p087_v5
// HLT_DoubleJet90_Double30_DoubleBTagCSV_p087_v5
// HLT_DoubleJetsC100_DoubleBTagCSV_p026_DoublePFJetsC160_v5
// HLT_DoubleJetsC100_DoubleBTagCSV_p014_DoublePFJetsC100MaxDeta1p6_v5
// HLT_DoubleJetsC112_DoubleBTagCSV_p026_DoublePFJetsC172_v5
// HLT_DoubleJetsC112_DoubleBTagCSV_p014_DoublePFJetsC112MaxDeta1p6_v5
// HLT_DoubleJetsC100_SingleBTagCSV_p026_v3
// HLT_DoubleJetsC100_SingleBTagCSV_p014_v3
// HLT_DoubleJetsC100_SingleBTagCSV_p026_SinglePFJetC350_v5
// HLT_DoubleJetsC100_SingleBTagCSV_p014_SinglePFJetC350_v5
// HLT_Photon135_PFMET100_v8
// HLT_Photon20_CaloIdVL_IsoL_v7
// HLT_Photon22_R9Id90_HE10_Iso40_EBOnly_PFMET40_v9
// HLT_Photon22_R9Id90_HE10_Iso40_EBOnly_VBF_v7
// HLT_Photon250_NoHE_v7
// HLT_Photon300_NoHE_v7
// HLT_Photon26_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon16_AND_HE10_R9Id65_Eta2_Mass60_v8
// HLT_Photon36_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon22_AND_HE10_R9Id65_Eta2_Mass15_v8
// HLT_Photon36_R9Id90_HE10_Iso40_EBOnly_PFMET40_v9
// HLT_Photon36_R9Id90_HE10_Iso40_EBOnly_VBF_v8
// HLT_Photon50_R9Id90_HE10_Iso40_EBOnly_PFMET40_v9
// HLT_Photon50_R9Id90_HE10_Iso40_EBOnly_VBF_v7
// HLT_Photon75_R9Id90_HE10_Iso40_EBOnly_PFMET40_v9
// HLT_Photon75_R9Id90_HE10_Iso40_EBOnly_VBF_v7
// HLT_Photon90_R9Id90_HE10_Iso40_EBOnly_PFMET40_v9
// HLT_Photon90_R9Id90_HE10_Iso40_EBOnly_VBF_v7
// HLT_Photon120_R9Id90_HE10_Iso40_EBOnly_PFMET40_v9
// HLT_Photon120_R9Id90_HE10_Iso40_EBOnly_VBF_v7
// HLT_Mu8_TrkIsoVVL_v5
// HLT_Mu17_TrkIsoVVL_v4
// HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v7
// HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v9
// HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_v7
// HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v9
// HLT_BTagMu_DiJet20_Mu5_v5
// HLT_BTagMu_DiJet40_Mu5_v5
// HLT_BTagMu_DiJet70_Mu5_v5
// HLT_BTagMu_DiJet110_Mu5_v5
// HLT_BTagMu_DiJet170_Mu5_v4
// HLT_BTagMu_Jet300_Mu5_v5
// HLT_BTagMu_AK8Jet300_Mu5_v4
// HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v9
// HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_L1JetTauSeeded_v3
// HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v9
// HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL_v8
// HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v9
// HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v9
// HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v4
// HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v3
// HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v4
// HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v9
// HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_v7
// HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_DZ_v3
// HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v9
// HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v4
// HLT_Mu30_Ele30_CaloIdL_GsfTrkIdVL_v8
// HLT_Mu33_Ele33_CaloIdL_GsfTrkIdVL_v3
// HLT_Mu37_Ele27_CaloIdL_GsfTrkIdVL_v6
// HLT_Mu27_Ele37_CaloIdL_GsfTrkIdVL_v6
// HLT_Mu8_DiEle12_CaloIdL_TrackIdL_v8
// HLT_Mu12_Photon25_CaloIdL_v8
// HLT_Mu12_Photon25_CaloIdL_L1ISO_v8
// HLT_Mu12_Photon25_CaloIdL_L1OR_v8
// HLT_Mu17_Photon22_CaloIdL_L1ISO_v6
// HLT_Mu17_Photon30_CaloIdL_L1ISO_v9
// HLT_Mu17_Photon35_CaloIdL_L1ISO_v9
// HLT_DiMu9_Ele9_CaloIdL_TrackIdL_v8
// HLT_TripleMu_5_3_3_v3
// HLT_TripleMu_12_10_5_v4
// HLT_Mu3er_PFHT140_PFMET125_v6
// HLT_Mu6_PFHT200_PFMET80_BTagCSV_p067_v5
// HLT_Mu6_PFHT200_PFMET100_v5
// HLT_Mu14er_PFMET100_v5
// HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_v9
// HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v9
// HLT_Ele12_CaloIdL_TrackIdL_IsoVL_v8
// HLT_Ele17_CaloIdL_GsfTrkIdVL_v6
// HLT_Ele17_CaloIdL_TrackIdL_IsoVL_v7
// HLT_Ele23_CaloIdL_TrackIdL_IsoVL_v8
// HLT_PFHT650_WideJetMJJ900DEtaJJ1p5_v8
// HLT_PFHT650_WideJetMJJ950DEtaJJ1p5_v8
// HLT_Photon22_v6
// HLT_Photon30_v7
// HLT_Photon36_v7
// HLT_Photon50_v7
// HLT_Photon75_v7
// HLT_Photon90_v7
// HLT_Photon120_v7
// HLT_Photon175_v8
// HLT_Photon165_HE10_v8
// HLT_Photon22_R9Id90_HE10_IsoM_v7
// HLT_Photon30_R9Id90_HE10_IsoM_v8
// HLT_Photon36_R9Id90_HE10_IsoM_v8
// HLT_Photon50_R9Id90_HE10_IsoM_v8
// HLT_Photon75_R9Id90_HE10_IsoM_v8
// HLT_Photon90_R9Id90_HE10_IsoM_v8
// HLT_Photon120_R9Id90_HE10_IsoM_v8
// HLT_Photon165_R9Id90_HE10_IsoM_v9
// HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass90_v7
// HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelSeedMatch_Mass70_v7
// HLT_Diphoton30PV_18PV_R9Id_AND_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v7
// HLT_Diphoton30_18_Solid_R9Id_AND_IsoCaloId_AND_HE_R9Id_Mass55_v7
// HLT_Diphoton30EB_18EB_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v7
// HLT_Dimuon0_Jpsi_Muon_v5
// HLT_Dimuon0_Upsilon_Muon_v5
// HLT_QuadMuon0_Dimuon0_Jpsi_v4
// HLT_QuadMuon0_Dimuon0_Upsilon_v4
// HLT_Rsq0p25_Calo_v3
// HLT_RsqMR240_Rsq0p09_MR200_4jet_Calo_v3
// HLT_RsqMR240_Rsq0p09_MR200_Calo_v3
// HLT_Rsq0p25_v6
// HLT_Rsq0p30_v6
// HLT_RsqMR240_Rsq0p09_MR200_v6
// HLT_RsqMR240_Rsq0p09_MR200_4jet_v6
// HLT_RsqMR270_Rsq0p09_MR200_v6
// HLT_RsqMR270_Rsq0p09_MR200_4jet_v6
// HLT_Rsq0p02_MR300_TriPFJet80_60_40_BTagCSV_p063_p20_Mbb60_200_v5
// HLT_Rsq0p02_MR400_TriPFJet80_60_40_DoubleBTagCSV_p063_Mbb60_200_v3
// HLT_Rsq0p02_MR450_TriPFJet80_60_40_DoubleBTagCSV_p063_Mbb60_200_v3
// HLT_Rsq0p02_MR500_TriPFJet80_60_40_DoubleBTagCSV_p063_Mbb60_200_v2
// HLT_Rsq0p02_MR550_TriPFJet80_60_40_DoubleBTagCSV_p063_Mbb60_200_v2
// HLT_HT200_DisplacedDijet40_DisplacedTrack_v4
// HLT_HT250_DisplacedDijet40_DisplacedTrack_v5
// HLT_HT350_DisplacedDijet40_DisplacedTrack_v5
// HLT_HT350_DisplacedDijet80_DisplacedTrack_v5
// HLT_HT350_DisplacedDijet80_Tight_DisplacedTrack_v5
// HLT_HT350_DisplacedDijet40_Inclusive_v4
// HLT_HT400_DisplacedDijet40_Inclusive_v5
// HLT_HT500_DisplacedDijet40_Inclusive_v5
// HLT_HT550_DisplacedDijet40_Inclusive_v5
// HLT_HT550_DisplacedDijet80_Inclusive_v2
// HLT_HT650_DisplacedDijet80_Inclusive_v5
// HLT_HT750_DisplacedDijet80_Inclusive_v5
// HLT_VBF_DisplacedJet40_DisplacedTrack_v4
// HLT_VBF_DisplacedJet40_DisplacedTrack_2TrackIP2DSig5_v4
// HLT_VBF_DisplacedJet40_TightID_DisplacedTrack_v4
// HLT_VBF_DisplacedJet40_Hadronic_v4
// HLT_VBF_DisplacedJet40_Hadronic_2PromptTrack_v4
// HLT_VBF_DisplacedJet40_TightID_Hadronic_v4
// HLT_VBF_DisplacedJet40_VTightID_Hadronic_v4
// HLT_VBF_DisplacedJet40_VVTightID_Hadronic_v4
// HLT_VBF_DisplacedJet40_VTightID_DisplacedTrack_v4
// HLT_VBF_DisplacedJet40_VVTightID_DisplacedTrack_v4
// HLT_PFMETNoMu90_PFMHTNoMu90_IDTight_v7
// HLT_PFMETNoMu100_PFMHTNoMu100_IDTight_v7
// HLT_PFMETNoMu110_PFMHTNoMu110_IDTight_v7
// HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_v7
// HLT_MonoCentralPFJet80_PFMETNoMu90_PFMHTNoMu90_IDTight_v7
// HLT_MonoCentralPFJet80_PFMETNoMu100_PFMHTNoMu100_IDTight_v7
// HLT_MonoCentralPFJet80_PFMETNoMu110_PFMHTNoMu110_IDTight_v7
// HLT_MonoCentralPFJet80_PFMETNoMu120_PFMHTNoMu120_IDTight_v7
// HLT_Ele27_eta2p1_WPLoose_Gsf_HT200_v9
// HLT_Photon90_CaloIdL_PFHT500_v9
// HLT_DoubleMu8_Mass8_PFHT250_v6
// HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT250_v7
// HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT250_v7
// HLT_DoubleMu8_Mass8_PFHT300_v9
// HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_v10
// HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_v10
// HLT_Mu10_CentralPFJet30_BTagCSV_p13_v5
// HLT_DoubleMu3_PFMET50_v5
// HLT_Ele10_CaloIdM_TrackIdM_CentralPFJet30_BTagCSV_p13_v7
// HLT_Ele15_IsoVVVL_BTagCSV_p067_PFHT400_v7
// HLT_Ele15_IsoVVVL_PFHT350_PFMET50_v8
// HLT_Ele15_IsoVVVL_PFHT600_v9
// HLT_Ele15_IsoVVVL_PFHT350_v8
// HLT_Ele15_IsoVVVL_PFHT400_PFMET50_v6
// HLT_Ele15_IsoVVVL_PFHT400_v6
// HLT_Ele50_IsoVVVL_PFHT400_v6
// HLT_Mu8_TrkIsoVVL_DiPFJet40_DEta3p5_MJJ750_HTT300_PFMETNoMu60_v4
// HLT_Mu10_TrkIsoVVL_DiPFJet40_DEta3p5_MJJ750_HTT350_PFMETNoMu60_v5
// HLT_Mu15_IsoVVVL_BTagCSV_p067_PFHT400_v6
// HLT_Mu15_IsoVVVL_PFHT350_PFMET50_v7
// HLT_Mu15_IsoVVVL_PFHT600_v8
// HLT_Mu15_IsoVVVL_PFHT350_v7
// HLT_Mu15_IsoVVVL_PFHT400_PFMET50_v5
// HLT_Mu15_IsoVVVL_PFHT400_v5
// HLT_Mu50_IsoVVVL_PFHT400_v5
// HLT_Dimuon16_Jpsi_v6
// HLT_Dimuon10_Jpsi_Barrel_v6
// HLT_Dimuon8_PsiPrime_Barrel_v6
// HLT_Dimuon8_Upsilon_Barrel_v6
// HLT_Dimuon0_Phi_Barrel_v6
// HLT_Mu16_TkMu0_dEta18_Onia_v5
// HLT_Mu16_TkMu0_dEta18_Phi_v5
// HLT_TrkMu15_DoubleTrkMu5NoFiltersNoVtx_v6
// HLT_TrkMu17_DoubleTrkMu8NoFiltersNoVtx_v6
// HLT_Mu8_v5
// HLT_Mu17_v4
// HLT_Mu3_PFJet40_v6
// HLT_Ele8_CaloIdM_TrackIdM_PFJet30_v9
// HLT_Ele12_CaloIdM_TrackIdM_PFJet30_v9
// HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v7
// HLT_Ele23_CaloIdM_TrackIdM_PFJet30_v9
// HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet140_v7
// HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165_v7
// HLT_PFHT400_SixJet30_DoubleBTagCSV_p056_v5
// HLT_PFHT450_SixJet40_BTagCSV_p056_v5
// HLT_PFHT400_SixJet30_v7
// HLT_PFHT450_SixJet40_v7
// HLT_Ele115_CaloIdVT_GsfTrkIdT_v7
// HLT_Mu55_v4
// HLT_Photon42_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon25_AND_HE10_R9Id65_Eta2_Mass15_v8
// HLT_Photon90_CaloIdL_PFHT600_v8
// HLT_PixelTracks_Multiplicity60ForEndOfFill_v3
// HLT_PixelTracks_Multiplicity85ForEndOfFill_v3
// HLT_PixelTracks_Multiplicity110ForEndOfFill_v3
// HLT_PixelTracks_Multiplicity135ForEndOfFill_v3
// HLT_PixelTracks_Multiplicity160ForEndOfFill_v3
// HLT_FullTracks_Multiplicity80_v4
// HLT_FullTracks_Multiplicity100_v4
// HLT_FullTracks_Multiplicity130_v4
// HLT_FullTracks_Multiplicity150_v4
// HLT_ECALHT800_v5
// HLT_DiSC30_18_EIso_AND_HE_Mass70_v6
// HLT_Photon125_v5
// HLT_MET100_v3
// HLT_MET150_v3
// HLT_MET200_v4
// HLT_Ele27_HighEta_Ele20_Mass55_v7
// HLT_L1FatEvents_v1
// HLT_Physics_v4
// HLT_L1FatEvents_part0_v1
// HLT_L1FatEvents_part1_v1
// HLT_L1FatEvents_part2_v1
// HLT_L1FatEvents_part3_v1
// DST_Physics_v2
// DST_CaloJet40_BTagScouting_v7
// DST_CaloJet40_CaloScouting_PFScouting_v7
// DST_CaloJet40_CaloBTagScouting_v6
// DST_L1HTT_BTagScouting_v7
// DST_L1HTT_CaloScouting_PFScouting_v7
// DST_L1HTT_CaloBTagScouting_v6
// DST_HT250_CaloScouting_v5
// DST_HT250_CaloBTagScouting_v3
// DST_HT410_BTagScouting_v7
// DST_HT410_PFScouting_v7
// DST_HT450_BTagScouting_v7
// DST_HT450_PFScouting_v7
// DST_ZeroBias_CaloScouting_PFScouting_v6
// DST_ZeroBias_BTagScouting_v7
// DST_L1DoubleMu_BTagScouting_v8
// DST_L1DoubleMu_CaloScouting_PFScouting_v7
// DST_DoubleMu3_Mass10_BTagScouting_v8
// DST_DoubleMu3_Mass10_CaloScouting_PFScouting_v7
// HLT_Random_v2
// HLT_ZeroBias_v4
// HLT_AK4CaloJet30_v5
// HLT_AK4CaloJet40_v4
// HLT_AK4CaloJet50_v4
// HLT_AK4CaloJet80_v4
// HLT_AK4CaloJet100_v4
// HLT_AK4PFJet30_v7
// HLT_AK4PFJet50_v7
// HLT_AK4PFJet80_v7
// HLT_AK4PFJet100_v7
// HLT_HISinglePhoton10_v4
// HLT_HISinglePhoton15_v4
// HLT_HISinglePhoton20_v4
// HLT_HISinglePhoton40_v4
// HLT_HISinglePhoton60_v4
// HLT_EcalCalibration_v3
// HLT_HcalCalibration_v2
// AlCa_EcalPhiSym_v6
// AlCa_SingleEle_WPVeryLoose_Gsf_v7
// AlCa_DoubleEle_CaloIdL_TrackIdL_IsoVL_DZ_v6
// AlCa_DoubleEle_CaloIdL_TrackIdL_IsoVL_v6
// HLT_GlobalRunHPDNoise_v4
// HLT_L1BptxMinus_v2
// HLT_L1BptxPlus_v2
// HLT_L1NotBptxOR_v2
// HLT_L1BeamGasMinus_v2
// HLT_L1BeamGasPlus_v2
// HLT_L1BptxXOR_v2
// HLT_L1MinimumBiasHF_OR_v2
// HLT_L1MinimumBiasHF_AND_v2
// HLT_HcalNZS_v3
// HLT_HcalPhiSym_v3
// HLT_HcalIsolatedbunch_v2
// HLT_ZeroBias_FirstCollisionAfterAbortGap_v3
// HLT_ZeroBias_FirstCollisionAfterAbortGap_copy_v1
// HLT_ZeroBias_FirstCollisionAfterAbortGap_TCDS_v1
// HLT_ZeroBias_IsolatedBunches_v3
// HLT_ZeroBias_FirstCollisionInTrain_v1
// HLT_ZeroBias_FirstBXAfterTrain_v1
// AlCa_RPCMuonNoTriggers_v3
// AlCa_RPCMuonNoHits_v3
// AlCa_RPCMuonNormalisation_v3
// AlCa_LumiPixels_Random_v2
// AlCa_LumiPixels_ZeroBias_v4
// MC_ReducedIterativeTracking_v3
// MC_PFMET_v6
// MC_AK4PFJets_v6
// MC_PFHT_v6
// MC_PFMHT_v6
// MC_CaloMET_v3
// MC_CaloMET_JetIdCleaned_v3
// MC_AK4CaloJets_v3
// MC_CaloHT_v3
// MC_CaloMHT_v3
// MC_AK8PFJets_v6
// MC_AK8TrimPFJets_v6
// MC_AK8PFHT_v6
// MC_AK8CaloHT_v3
// MC_Diphoton10_10_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass10_v6
// MC_DoubleEle5_CaloIdL_GsfTrkIdVL_MW_v6
// MC_Ele5_WPLoose_Gsf_v8
// MC_Ele15_Ele10_CaloIdL_TrackIdL_IsoVL_DZ_v7
// MC_IsoMu_v7
// MC_IsoTkMu15_v6
// MC_DoubleMu_TrkIsoVVL_DZ_v4
// MC_DoubleGlbTrkMu_TrkIsoVVL_DZ_v4
// MC_DoubleMuNoFiltersNoVtx_v3
// MC_DoubleL1Tau_MediumIsoPFTau32_Trk1_eta2p1_Reg_v6
// MC_LooseIsoPFTau20_v5
// MC_LooseIsoPFTau50_Trk30_eta2p1_v4
// HLT_Photon500_v6
// HLT_Photon600_v6
// HLT_Mu300_v3
// HLT_Mu350_v3
// HLT_MET250_v4
// HLT_MET300_v4
// HLT_MET600_v4
// HLT_MET700_v4
// HLT_PFMET300_v6
// HLT_PFMET400_v6
// HLT_PFMET500_v6
// HLT_PFMET600_v6
// HLT_Ele250_CaloIdVT_GsfTrkIdT_v6
// HLT_Ele300_CaloIdVT_GsfTrkIdT_v6
// HLT_HT2000_v4
// HLT_HT2500_v4
// HLT_IsoTrackHE_v3
// HLT_IsoTrackHB_v3
// AlCa_EcalPi0EBonly_v8
// AlCa_EcalPi0EEonly_v8
// AlCa_EcalEtaEBonly_v8
// AlCa_EcalEtaEEonly_v8



ULong64_t   AOD_HLT_PFHT350PFMET100_isPS_;

ULong64_t   AOD_HLT_Ele23Loose_isPS_;
ULong64_t   AOD_HLT_Ele27Tight_isPS_;
ULong64_t   AOD_HLT_Ele17Ele12_isPS_;
ULong64_t   AOD_HLT_Ele23Ele12_isPS_;

ULong64_t   AOD_HLT_IsoMu22_isPS_;
ULong64_t   AOD_HLT_IsoTkMu22_isPS_;
ULong64_t   AOD_HLT_Mu17Mu8_isPS_;
ULong64_t   AOD_HLT_Mu17TkMu8_isPS_;


const int bitsize=8;


void lldjNtuple::branchesAODTrigger(TTree* tree){
  tree->Branch("AOD_HLT_Ele23Loose",       &AOD_HLT_Ele23Loose_) ;
  tree->Branch("AOD_HLT_Ele27Tight",       &AOD_HLT_Ele27Tight_) ;
  tree->Branch("AOD_HLT_Ele17Ele12",       &AOD_HLT_Ele17Ele12_) ;
  tree->Branch("AOD_HLT_Ele23Ele12",       &AOD_HLT_Ele23Ele12_) ;
  
  tree->Branch("AOD_HLT_IsoMu22"  ,        &AOD_HLT_IsoMu22_)   ;
  tree->Branch("AOD_HLT_IsoTkMu22",        &AOD_HLT_IsoTkMu22_) ;
  tree->Branch("AOD_HLT_Mu17Mu8"  ,        &AOD_HLT_Mu17Mu8_)   ;
  tree->Branch("AOD_HLT_Mu17TkMu8",        &AOD_HLT_Mu17TkMu8_) ;
}


void lldjNtuple::fillAODTrigger(const edm::Event &e, const edm::EventSetup &es){
  
  AOD_HLT_Ele23Loose_       = 0;
  AOD_HLT_Ele27Tight_       = 0;
  AOD_HLT_Ele17Ele12_       = 0;
  AOD_HLT_Ele23Ele12_       = 0;
  
  AOD_HLT_IsoMu22_          = 0;
  AOD_HLT_IsoTkMu22_        = 0;
  AOD_HLT_Mu17Mu8_          = 0;
  AOD_HLT_Mu17TkMu8_        = 0;
  
  if( !( e.getByToken(AODTriggerToken_, AODTriggerHandle_) && e.getByToken(AODTriggerEventToken_, AODTriggerEventHandle_) ) ){ return; }
  
  for(int i = 0; i < (int)hltConfig_.size(); i++){
    string name = hltConfig_.triggerName(i);
    // std::cout<<name<<"\n";
    
    int bitEle23Loose = -1;
    if      (name.find("HLT_Ele23_WPLoose_Gsf_v1")  != string::npos) bitEle23Loose = 0  ;
    else if (name.find("HLT_Ele23_WPLoose_Gsf_v2")  != string::npos) bitEle23Loose = 1  ;
    else if (name.find("HLT_Ele23_WPLoose_Gsf_v3")  != string::npos) bitEle23Loose = 2  ;
    else if (name.find("HLT_Ele23_WPLoose_Gsf_v4")  != string::npos) bitEle23Loose = 3  ;
    else if (name.find("HLT_Ele23_WPLoose_Gsf_v5")  != string::npos) bitEle23Loose = 4  ;
    else if (name.find("HLT_Ele23_WPLoose_Gsf_v6")  != string::npos) bitEle23Loose = 5  ;
    else if (name.find("HLT_Ele23_WPLoose_Gsf_v7")  != string::npos) bitEle23Loose = 6  ;
    else if (name.find("HLT_Ele23_WPLoose_Gsf_v8")  != string::npos) bitEle23Loose = 7  ;
    else if (name.find("HLT_Ele23_WPLoose_Gsf_v9")  != string::npos) bitEle23Loose = 8  ;
    else if (name.find("HLT_Ele23_WPLoose_Gsf_v10") != string::npos) bitEle23Loose = 9  ;
    else if (name.find("HLT_Ele23_WPLoose_Gsf_v11") != string::npos) bitEle23Loose = 10 ;
    else if (name.find("HLT_Ele23_WPLoose_Gsf_v12") != string::npos) bitEle23Loose = 11 ;
    
    int bitEle27Tight = -1;
    if      (name.find("HLT_Ele27_WPTight_Gsf_v1")  != string::npos) bitEle27Tight = 0 ;
    else if (name.find("HLT_Ele27_WPTight_Gsf_v2")  != string::npos) bitEle27Tight = 1 ;
    else if (name.find("HLT_Ele27_WPTight_Gsf_v3")  != string::npos) bitEle27Tight = 2 ;
    else if (name.find("HLT_Ele27_WPTight_Gsf_v4")  != string::npos) bitEle27Tight = 3 ;
    else if (name.find("HLT_Ele27_WPTight_Gsf_v5")  != string::npos) bitEle27Tight = 4 ;
    else if (name.find("HLT_Ele27_WPTight_Gsf_v6")  != string::npos) bitEle27Tight = 5 ;
    else if (name.find("HLT_Ele27_WPTight_Gsf_v7")  != string::npos) bitEle27Tight = 6 ;
    else if (name.find("HLT_Ele27_WPTight_Gsf_v8")  != string::npos) bitEle27Tight = 7 ;
    else if (name.find("HLT_Ele27_WPTight_Gsf_v9")  != string::npos) bitEle27Tight = 8 ;
    
    // Double Electron
    int bitEle17Ele12 = -1;
    if      (name.find("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v1")  != string::npos) bitEle17Ele12 = 0 ;
    else if (name.find("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v2")  != string::npos) bitEle17Ele12 = 1 ;
    else if (name.find("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v3")  != string::npos) bitEle17Ele12 = 2 ;
    else if (name.find("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v4")  != string::npos) bitEle17Ele12 = 3 ;
    else if (name.find("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v5")  != string::npos) bitEle17Ele12 = 4 ;
    else if (name.find("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v6")  != string::npos) bitEle17Ele12 = 5 ;
    else if (name.find("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v7")  != string::npos) bitEle17Ele12 = 6 ;
    else if (name.find("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v8")  != string::npos) bitEle17Ele12 = 7 ;
    else if (name.find("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v9")  != string::npos) bitEle17Ele12 = 8 ;
    else if (name.find("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v10") != string::npos) bitEle17Ele12 = 9 ;
    
    int bitEle23Ele12 = -1;
    if      (name.find("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v1")  != string::npos) bitEle23Ele12 = 0 ;
    else if (name.find("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v2")  != string::npos) bitEle23Ele12 = 1 ;
    else if (name.find("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v3")  != string::npos) bitEle23Ele12 = 2 ;
    else if (name.find("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v4")  != string::npos) bitEle23Ele12 = 3 ;
    else if (name.find("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v5")  != string::npos) bitEle23Ele12 = 4 ;
    else if (name.find("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v6")  != string::npos) bitEle23Ele12 = 5 ;
    else if (name.find("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v7")  != string::npos) bitEle23Ele12 = 6 ;
    else if (name.find("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v8")  != string::npos) bitEle23Ele12 = 7 ;
    else if (name.find("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v9")  != string::npos) bitEle23Ele12 = 8 ;
    else if (name.find("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v10") != string::npos) bitEle23Ele12 = 9 ;
    
    // Single Iso Mu
    int bitIsoMu22   = -1;
    if      (name.find("HLT_IsoMu22_v1") != string::npos) bitIsoMu22 = 0 ;
    else if (name.find("HLT_IsoMu22_v2") != string::npos) bitIsoMu22 = 1 ;
    else if (name.find("HLT_IsoMu22_v3") != string::npos) bitIsoMu22 = 2 ;
    else if (name.find("HLT_IsoMu22_v4") != string::npos) bitIsoMu22 = 3 ;
    else if (name.find("HLT_IsoMu22_v5") != string::npos) bitIsoMu22 = 4 ;
    else if (name.find("HLT_IsoMu22_v6") != string::npos) bitIsoMu22 = 5 ;
    else if (name.find("HLT_IsoMu22_v7") != string::npos) bitIsoMu22 = 6 ;
    
    int bitIsoTkMu22 = -1;
    if      (name.find("HLT_IsoTkMu22_v1")  != string::npos) bitIsoTkMu22 = 0 ;
    else if (name.find("HLT_IsoTkMu22_v2")  != string::npos) bitIsoTkMu22 = 1 ;
    else if (name.find("HLT_IsoTkMu22_v3")  != string::npos) bitIsoTkMu22 = 2 ;
    else if (name.find("HLT_IsoTkMu22_v4")  != string::npos) bitIsoTkMu22 = 3 ;
    else if (name.find("HLT_IsoTkMu22_v5")  != string::npos) bitIsoTkMu22 = 4 ;
    else if (name.find("HLT_IsoTkMu22_v6")  != string::npos) bitIsoTkMu22 = 5 ;
    else if (name.find("HLT_IsoTkMu22_v7")  != string::npos) bitIsoTkMu22 = 6 ;
    else if (name.find("HLT_IsoTkMu22_v8")  != string::npos) bitIsoTkMu22 = 7 ;
    else if (name.find("HLT_IsoTkMu22_v9")  != string::npos) bitIsoTkMu22 = 8 ;
    else if (name.find("HLT_IsoTkMu22_v10") != string::npos) bitIsoTkMu22 = 9 ;
    
    // Double Iso Mu
    int bitMu17Mu8   = -1;
    if      (name.find("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v1") != string::npos) bitMu17Mu8 = 0 ;
    else if (name.find("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v2") != string::npos) bitMu17Mu8 = 1 ;
    else if (name.find("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v3") != string::npos) bitMu17Mu8 = 2 ;
    else if (name.find("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v4") != string::npos) bitMu17Mu8 = 3 ;
    else if (name.find("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v5") != string::npos) bitMu17Mu8 = 4 ;
    else if (name.find("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v6") != string::npos) bitMu17Mu8 = 5 ;
    
    int bitMu17TkMu8 = -1;
    if      (name.find("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v1") != string::npos) bitMu17TkMu8 = 0 ;                   
    else if (name.find("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v2") != string::npos) bitMu17TkMu8 = 1 ;                   
    else if (name.find("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v3") != string::npos) bitMu17TkMu8 = 2 ;                   
    else if (name.find("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v4") != string::npos) bitMu17TkMu8 = 3 ;                   
    else if (name.find("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v5") != string::npos) bitMu17TkMu8 = 4 ;                   
    else if (name.find("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v6") != string::npos) bitMu17TkMu8 = 5 ;                   
    
    //printf(" Reading trigger: %s\n" , name.c_str()  );
    //printf(" bitPFHT350PFMET100 %d \n", bitPFHT350PFMET100 ); 
    //printf(" bitEle23Loose      %d \n", bitEle23Loose      ); 
    //printf(" bitEle27Tight      %d \n", bitEle27Tight      ); 
    //printf(" bitEle17Ele12      %d \n", bitEle17Ele12      ); 
    //printf(" bitEle23Ele12      %d \n", bitEle23Ele12      ); 
    //printf(" bitIsoMu22         %d \n", bitIsoMu22         ); 
    //printf(" bitIsoTkMu22       %d \n", bitIsoTkMu22       ); 
    //printf(" bitMu17Mu8         %d \n", bitMu17Mu8         ); 
    //printf(" bitMu17TkMu8       %d \n", bitMu17TkMu8       ); 
    
    // indicates prescaling and whether trigger was fired or not
    unsigned int isPrescaled = hltPrescale_.prescaleValue(e,es,name);
    const unsigned int triggerIndex = hltConfig_.triggerIndex(name);
    unsigned int isFired     = AODTriggerHandle_->accept(triggerIndex);
    //cout << "was run " << AODTriggerHandle_->wasrun(triggerIndex) << endl;
    
    if ( bitEle23Loose      >= 0 ){
      AOD_HLT_Ele23Loose_            |= (isFired     << bitEle23Loose      );
      AOD_HLT_Ele23Loose_isPS_       |= (isPrescaled << bitEle23Loose      );
    }
    if ( bitEle27Tight      >= 0 ){     
      AOD_HLT_Ele27Tight_            |= (isFired     << bitEle27Tight      );
      AOD_HLT_Ele27Tight_isPS_       |= (isPrescaled << bitEle27Tight      );
    }
    if ( bitEle17Ele12      >= 0 ){     
      AOD_HLT_Ele17Ele12_            |= (isFired     << bitEle17Ele12      );
      AOD_HLT_Ele17Ele12_isPS_       |= (isPrescaled << bitEle17Ele12      );
    }
    if ( bitEle23Ele12      >= 0 ){     
      AOD_HLT_Ele23Ele12_            |= (isFired     << bitEle23Ele12      );
      AOD_HLT_Ele23Ele12_isPS_       |= (isPrescaled << bitEle23Ele12      );
    }
    
    if ( bitIsoMu22         >= 0 ){   
      AOD_HLT_IsoMu22_               |= (isFired     << bitIsoMu22         );
      AOD_HLT_IsoMu22_isPS_          |= (isPrescaled << bitIsoMu22         );
    }
    if ( bitIsoTkMu22       >= 0 ){     
      AOD_HLT_IsoTkMu22_             |= (isFired     << bitIsoTkMu22       );
      AOD_HLT_IsoTkMu22_isPS_        |= (isPrescaled << bitIsoTkMu22       );
    }
    if ( bitMu17Mu8         >= 0 ){   
      AOD_HLT_Mu17Mu8_               |= (isFired     << bitMu17Mu8         );
      AOD_HLT_Mu17Mu8_isPS_          |= (isPrescaled << bitMu17Mu8         );
    }
    if ( bitMu17TkMu8       >= 0 ){     
      AOD_HLT_Mu17TkMu8_             |= (isFired     << bitMu17TkMu8       );
      AOD_HLT_Mu17TkMu8_isPS_        |= (isPrescaled << bitMu17TkMu8       );
    }
    
    //std::bitset<bitsize> AOD_HLT_PFHT350PFMET100_b(bitPFHT350PFMET100);
    std::bitset<bitsize> AOD_HLT_Ele23Loose_b(bitEle23Loose);
    std::bitset<bitsize> AOD_HLT_Ele27Tight_b(bitEle27Tight);
    std::bitset<bitsize> AOD_HLT_Ele17Ele12_b(bitEle17Ele12);
    std::bitset<bitsize> AOD_HLT_Ele23Ele12_b(bitEle23Ele12);
    std::bitset<bitsize> AOD_HLT_IsoMu22_b(bitIsoMu22);
    std::bitset<bitsize> AOD_HLT_IsoTkMu22_b(bitIsoTkMu22);
    std::bitset<bitsize> AOD_HLT_Mu17Mu8_b(bitMu17Mu8);
    std::bitset<bitsize> AOD_HLT_Mu17TkMu8_b(bitMu17TkMu8);
    
  }//hltConfig loop
  
}//fillAODTrigger
