/*
 * MERNoising.cxx
 *
 *
 * Copyright (C) by Andreas Zoglauer.
 * All rights reserved.
 *
 *
 * This code implementation is the intellectual property of
 * Andreas Zoglauer.
 *
 * By copying, distributing or modifying the Program (or any work
 * based on the Program) you indicate your acceptance of this statement,
 * and all its terms.
 *
 */


////////////////////////////////////////////////////////////////////////////////
//
// MERNoising
//
// Find all clusters...
//
////////////////////////////////////////////////////////////////////////////////


// Include the header:
#include "MERNoising.h"

// Standard libs:
#include <cstdlib>
#include <iomanip>
using namespace std;

// ROOT libs:

// MEGAlib libs:
#include "MStreams.h"
#include "MAssert.h"
#include "MREAM.h"
#include "MREAMDriftChamberEnergy.h"
#include "MREAMGuardRingHit.h"
#include "MREAMDirectional.h"
#include "MDDriftChamber.h"
#include "MDStrip2D.h"
#include "MDStrip3D.h"
#include "MDVoxel3D.h"
#include "MDStrip3DDirectional.h"
#include "MDGuardRing.h"


////////////////////////////////////////////////////////////////////////////////


#ifdef ___CLING___
ClassImp(MERNoising)
#endif


////////////////////////////////////////////////////////////////////////////////


MERNoising::MERNoising()
{
  // Construct an instance of MERNoising
}


////////////////////////////////////////////////////////////////////////////////


MERNoising::~MERNoising()
{
  // Delete this instance of MERNoising
}


////////////////////////////////////////////////////////////////////////////////


bool MERNoising::PreAnalysis()
{
  m_NTriggeredEvents = 0;
  m_NVetoedEvents = 0;
  m_NNotTriggeredOrVetoedEvents = 0;
  
  m_TriggerMapTriggerNames.clear();
  m_VetoMapTriggerNames.clear();
  m_VetoMapVetoNames.clear();
  
  return true;
}


////////////////////////////////////////////////////////////////////////////////


bool MERNoising::Analyze(MRERawEvent* Event)
{
  // Do all the things required for simulated events such as noising, triggering, etc.

  MRESE* RESE = 0;
  MString TriggerName;
  MDTriggerUnit* Trigger = m_Geometry->GetTriggerUnit();

  // Step 1: Noise

  // Noise the system time
  MDSystem* System = m_Geometry->GetSystem();
  if (System != 0) {
    MTime Time = Event->GetEventTime();
    System->NoiseTime(Time);
    Event->SetEventTime(Time);
  }
  
  // Step 1.1: Noise hits:
  int h_max = Event->GetNRESEs();
  for (int h = 0; h < h_max; ++h) {
    if (Event->GetRESEAt(h)->GetType() == MRESE::c_Hit) {
      //if (((MREHit *) (Event->GetRESEAt(h)))->GetDetector() != MDDetector::c_Scintillator) {
      mdebug<<"TG - Event: "<<Event->GetEventID()<<": Noising..."<<Event->GetRESEAt(h)->GetEnergy()<<endl;
      if (((MREHit *) (Event->GetRESEAt(h)))->Noise(m_Geometry) == false) {
        // No energy: voxel bad, not in detector volume, etc:
        mdebug<<"TG - Event: "<<Event->GetEventID()<<": Rejecting hit (noise)!"<<endl;
        RESE = Event->GetRESEAt(h);
        Event->RemoveRESE(RESE);
        delete RESE;
      }
    } else {
      mout<<"MRERawEvent* MFileEventsEvta::GetNextEvent(): Cannot noise a hit which is no single hit!"<<endl;
      mout<<Event->GetRESEAt(h)->ToString()<<endl;
    }
  }
  Event->CompressRESEs();

  // Step 1.2: Noise additional measurements:
  for (vector<MREAM*>::iterator Iter = Event->GetREAMBegin();
       Iter != Event->GetREAMEnd(); /* handled externally due to delete */) {
    if ((*Iter)->GetType() == MREAM::c_DriftChamberEnergy) {
      MREAMDriftChamberEnergy* DCE = dynamic_cast<MREAMDriftChamberEnergy*>(*Iter);
      double Energy = DCE->GetEnergy();
      MDVolumeSequence* V = DCE->GetVolumeSequence();
      dynamic_cast<MDDriftChamber*>(V->GetDetector())->NoiseLightEnergy(Energy);
      DCE->SetEnergy(Energy);
      DCE->SetEnergyResolution(dynamic_cast<MDDriftChamber*>(V->GetDetector())->GetLightEnergyResolution(Energy));
      ++Iter;
    } else if ((*Iter)->GetType() == MREAM::c_GuardRingHit) {
      MREAMGuardRingHit* GR = dynamic_cast<MREAMGuardRingHit*>(*Iter);
      double Energy = GR->GetEnergy();
      MDVolumeSequence* V = GR->GetVolumeSequence();
      if (V->GetDetector()->HasGuardRing() == false || V->GetDetector()->GetGuardRing()->IsActive() == false) {
        Event->DeleteREAM(Iter);
      } else {
        MVector DummyPosition;
        double DummyTime = 0.0;
        V->GetDetector()->GetGuardRing()->Noise(DummyPosition, Energy, DummyTime, nullptr);
        GR->SetEnergy(Energy);
        GR->SetEnergyResolution(V->GetDetector()->GetGuardRing()->GetEnergyResolution(Energy));
        if (Energy == 0) {
          Event->DeleteREAM(Iter);
        } else {
          ++Iter;
        }
      }
    } else if ((*Iter)->GetType() == MREAM::c_Directional) {
      MREAMDirectional* DR = dynamic_cast<MREAMDirectional*>(*Iter);
      MVector Direction = DR->GetDirection();
      MDVolumeSequence* V = DR->GetVolumeSequence();
      MVector Center(0.0, 0.0, 0.0);
      Center = V->GetPositionInVolume(Center, V->GetDetectorVolume());
      Direction = (V->GetPositionInVolume(Direction, V->GetDetectorVolume()) - Center).Unit();
      dynamic_cast<MDStrip3DDirectional*>(V->GetDetector())->NoiseDirection(Direction, DR->GetEnergy());
      Center = MVector(0.0, 0.0, 0.0);
      Center = V->GetPositionInFirstVolume(Center, V->GetDetectorVolume());
      Direction = (V->GetPositionInFirstVolume(Direction, V->GetDetectorVolume()) - Center).Unit();
      DR->SetDirection(Direction);
      if (Direction == g_VectorNotDefined) {
        Event->DeleteREAM(Iter);
      } else {
        ++Iter;
      }
    } else {
      ++Iter;
    }
  }

  // Step b: Test trigger
  Trigger->Reset();
  h_max = Event->GetNRESEs();
  for (int h = 0; h < h_max; ++h) {
    RESE = Event->GetRESEAt(h);
    if (RESE->GetType() == MRESE::c_Hit) {
      if (Trigger->AddHit(RESE->GetEnergy(), *(RESE->GetVolumeSequence())) == false) {
        /*
        if (RESE->GetDetector() == MDDetector::c_Scintillator) {
          mimp<<"Removing scintillator hits which have not triggered..."<<show;
          RESE = Event->GetRESEAt(h);
          Event->RemoveRESE(RESE);
          mdebug<<"TG - Event: "<<Event->GetEventID()<<": Rejecting hit (trigger not added)!"<<endl;
          delete RESE;
        }
        */
      }
    } else {
      merr<<"We shouldn't have anything else but hits at this point!"<<endl;
    }
  }
  Event->CompressRESEs();
  for (unsigned int a = 0; a < Event->GetNREAMs(); ++a) {
    if (Event->GetREAMAt(a)->GetType() == MREAM::c_GuardRingHit) {
      MREAMGuardRingHit* GR = dynamic_cast<MREAMGuardRingHit*>(Event->GetREAMAt(a));
      Trigger->AddGuardRingHit(GR->GetEnergy(), *(GR->GetVolumeSequence()));
    }
  }


  if (Trigger->HasVetoed() == true) {
    ++m_NVetoedEvents;

    vector<MString> TriggerNames = Trigger->GetTriggerNameList();
    for (MString S: TriggerNames) {
      m_VetoMapTriggerNames[S]++; 
    }
    vector<MString> VetoNames = Trigger->GetVetoNameList();
    for (MString S: VetoNames) {
      m_VetoMapVetoNames[S]++; 
    }
    
    h_max = Event->GetNRESEs();
    for (int h = 0; h < h_max; ++h) {
      RESE = Event->GetRESEAt(h);
      Event->RemoveRESE(RESE);
      mdebug<<"TG - Event: "<<Event->GetEventID()<<": Rejecting hit (event not triggered)!"<<endl;
      delete RESE;
    }
    Event->CompressRESEs();
    mdebug<<"TG - Event: "<<Event->GetEventID()<<" vetoed by: "<<TriggerName<<endl;
  } else if (Trigger->HasTriggered()) {
    ++m_NTriggeredEvents;
    
    vector<MString> TriggerNames = Trigger->GetTriggerNameList();
    for (MString S: TriggerNames) {
      m_TriggerMapTriggerNames[S]++; 
    }
    mdebug<<"TG - Event: "<<Event->GetEventID()<<" triggered with: "<<TriggerName<<endl;
  } else {
    ++m_NNotTriggeredOrVetoedEvents;
    
    h_max = Event->GetNRESEs();
    for (int h = 0; h < h_max; ++h) {
      RESE = Event->GetRESEAt(h);
      Event->RemoveRESE(RESE);
      mdebug<<"TG - Event: "<<Event->GetEventID()<<": Rejecting hit (event not triggered)!"<<endl;
      delete RESE;
    }
    Event->CompressRESEs();
    mdebug<<"TG - Event: "<<Event->GetEventID()<<" not triggered or vetoed!"<<endl;
  }
  

  // Step c: Link the MREAMDirectional to hits:
  for (vector<MREAM*>::iterator Iter = Event->GetREAMBegin();
       Iter != Event->GetREAMEnd(); ++Iter) {
    if ((*Iter)->GetType() == MREAM::c_Directional) {
      MREAMDirectional* DR = dynamic_cast<MREAMDirectional*>(*Iter);
      // Find a belonging hit:
      h_max = Event->GetNRESEs();
      for (int h = 0; h < h_max; ++h) {
        RESE = Event->GetRESEAt(h);
        if (RESE->GetType() == MRESE::c_Hit) {
          if (RESE->GetVolumeSequence()->HasSameDetector(DR->GetVolumeSequence()) == true) {
            if (RESE->GetVolumeSequence()->GetGridPoint() == DR->GetVolumeSequence()->GetGridPoint()) {
              DR->SetRESE(RESE);
            }
          }
        } else {
          merr<<"We shouldn't have anything else but hits at this point!"<<endl;
        }
      }
    }
  }
  // If a linked RESE appears twice then remove the info of the one with the lower energy:
  for (vector<MREAM*>::iterator Iter = Event->GetREAMBegin();
       Iter != Event->GetREAMEnd(); ++Iter) {
    if ((*Iter)->GetType() == MREAM::c_Directional) {
      MREAMDirectional* DR1 = dynamic_cast<MREAMDirectional*>(*Iter);
      for (vector<MREAM*>::iterator Iter2 = Iter+1;
           Iter2 != Event->GetREAMEnd(); ++Iter2) {
        if ((*Iter2)->GetType() == MREAM::c_Directional) {
          MREAMDirectional* DR2 = dynamic_cast<MREAMDirectional*>(*Iter2);
          if (DR1->GetRESE() != 0 && DR2->GetRESE() != 0) {
            if (DR1->GetRESE()->GetID() == DR2->GetRESE()->GetID()) {
              if (DR1->GetEnergy() < DR2->GetEnergy()) {
                DR1->SetRESE(0);
              } else {
                DR2->SetRESE(0);
              }
            }
          }
        }
      }
    }
  }
  // Remove all REAMs with no RESE:
  for (vector<MREAM*>::iterator Iter = Event->GetREAMBegin();
       Iter != Event->GetREAMEnd(); /* handled externally due to delete ++Iter */) {
    if ((*Iter)->GetType() == MREAM::c_Directional) {
      MREAMDirectional* DR = dynamic_cast<MREAMDirectional*>(*Iter);
      // If there is none remove the info:
      if (DR->GetRESE() == 0) {
        Event->DeleteREAM(Iter);
      } else {
        ++Iter;
      }
    } else {
      ++Iter;
    }
  }

  return true;
}


////////////////////////////////////////////////////////////////////////////////


void MERNoising::AddStatistics(MERNoising* Noising)
{
  // Add the statistics
  
  for (map<MString, long>::const_iterator Iter = Noising->m_TriggerMapTriggerNames.begin(); Iter != Noising->m_TriggerMapTriggerNames.end(); ++Iter) {
    m_TriggerMapTriggerNames[(*Iter).first] += (*Iter).second;
  }

  for (map<MString, long>::const_iterator Iter = Noising->m_VetoMapTriggerNames.begin(); Iter != Noising->m_VetoMapTriggerNames.end(); ++Iter) {
    m_VetoMapTriggerNames[(*Iter).first] += (*Iter).second;
  }
  for (map<MString, long>::const_iterator Iter = Noising->m_VetoMapVetoNames.begin(); Iter != Noising->m_VetoMapVetoNames.end(); ++Iter) {
    m_VetoMapVetoNames[(*Iter).first] += (*Iter).second;
  }
  
  m_NTriggeredEvents += Noising->m_NTriggeredEvents;
  m_NVetoedEvents += Noising->m_NVetoedEvents;
  m_NNotTriggeredOrVetoedEvents += Noising->m_NNotTriggeredOrVetoedEvents;
}


////////////////////////////////////////////////////////////////////////////////


MString MERNoising::ToString(bool) const
{
  ostringstream out;

  int Width = 6;
  size_t Length = 50;

  if (m_NNotTriggeredOrVetoedEvents > 0 || m_NTriggeredEvents > 0 || m_NVetoedEvents > 0) {
    out<<endl;
    out<<"Trigger statistics:"<<endl;
    out<<"  Not triggered events: .................................. "<<setw(Width)<<m_NNotTriggeredOrVetoedEvents<<endl;

    out<<"  Number of vetoed events: ............................... "<<setw(Width)<<m_NVetoedEvents<<endl;
    if (m_NVetoedEvents > 0) {
      out<<"    Raised vetoes (multiples possible)"<<endl;
      for (auto Iter = m_VetoMapVetoNames.begin(); Iter != m_VetoMapVetoNames.end(); ++Iter) {
        size_t L = (*Iter).first.Length();
        out<<"      "<<(*Iter).first<<": ";
        if (L < Length) for (size_t l = 0; l < Length-L; ++l) out<<".";
        out<<" "<<setw(Width)<<(*Iter).second<<endl;
      }
      out<<"    Raised triggers canceled by a veto (multiples possible)"<<endl;
      for (auto Iter = m_VetoMapTriggerNames.begin(); Iter != m_VetoMapTriggerNames.end(); ++Iter) {
        size_t L = (*Iter).first.Length();
        out<<"      "<<(*Iter).first<<": ";
        if (L < Length) for (size_t l = 0; l < Length-L; ++l) out<<".";
        out<<" "<<setw(Width)<<(*Iter).second<<endl;
      }
    }
    
    out<<"  Number of triggered events: ............................ "<<setw(Width)<<m_NTriggeredEvents<<endl;
    if (m_NTriggeredEvents > 0) {
      out<<"    Raised triggers (multiples possible)"<<endl;
      for (auto Iter = m_TriggerMapTriggerNames.begin(); Iter != m_TriggerMapTriggerNames.end(); ++Iter) {
        size_t L = (*Iter).first.Length();
        out<<"      "<<(*Iter).first<<": ";
        if (L < Length) for (size_t l = 0; l < Length-L; ++l) out<<".";
        out<<" "<<setw(Width)<<(*Iter).second<<endl;
      }
    }
    out<<endl;
  }

  return out.str().c_str();
}


// MERNoising.cxx: the end...
////////////////////////////////////////////////////////////////////////////////
