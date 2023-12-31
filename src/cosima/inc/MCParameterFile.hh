/*
 * MCParameterFile.hh
 *
 * Copyright (C) by Andreas Zoglauer.
 * All rights reserved.
 *
 * Please see the source-file for the copyright-notice.
 *
 */



/******************************************************************************
 *
 * Parses and stores all the data from the parameter file *.source
 *
 */

#ifndef ___MCParameterFile___
#define ___MCParameterFile___

// Geant4:
#include "globals.hh"

// Cosima:
#include "MCRun.hh"
#include "MCActivator.hh"
#include "MCRegion.hh"

// MEGAlib:
#include "MParser.h"

// Standard libs:
#include <vector>
#include <string>
using namespace std;

// Forward declarations:

/******************************************************************************/

class MCParameterFile : public MParser
{
  // public interface:
public:
  /// Default constructor
  MCParameterFile();
  /// Default destructor
  virtual ~MCParameterFile();

  /// Return the run list
  vector<MCRun>& GetRunList();

  /// Return the currently active run - in the moment only one run is allowed
  MCRun& GetCurrentRun() { return m_RunList[m_ActiveRun]; }

  /// Return the number of runs
  unsigned int GetNRuns() { return m_RunList.size(); }

  /// Remove all but the given run from the list
  void RestrictToRun(int Run);

  /// Return the number of activators
  unsigned int GetNActivators() { return m_ActivatorList.size(); }

  /// Return the number of activators
  MCActivator& GetActivator(unsigned int a) { return m_ActivatorList[a]; }

  /// Return the number of runs
  void NextRun() { m_ActiveRun++; }

  /// Return the name of the geometry file
  string GetGeometryFileName() { return m_GeometryFileName.ToString(); }

  /// Set the default range cut for all particles
  void SetDefaultRangeCut(const double DefaultRangeCut) { m_DefaultRangeCut = DefaultRangeCut; }
  /// Return the default range cut for all particles
  double GetDefaultRangeCut() const { return m_DefaultRangeCut; }

  /// Set the detector time constant
  void SetDetectorTimeConstant(const double DetectorTimeConstant) { m_DetectorTimeConstant = DetectorTimeConstant; }
  /// Return the detector time constant
  double GetDetectorTimeConstant() const { return m_DetectorTimeConstant; }

  /// Return the region list
  const vector<MCRegion>& GetRegionList() { return m_RegionList; }

  /// Set the EM simulation package (high/low energy, doppler, polarized, etc.)
  void SetPhysicsListEM(const int PhysicsListEM) { m_PhysicsListEM = PhysicsListEM; }
  /// Return the EM simulation package (high/low energy, doppler, polarized, etc.)
  int GetPhysicsListEM() const { return m_PhysicsListEM; }

  /// Set the decay mode (normal, ignore, build-up activation)
  void SetDecayMode(const int DecayMode) { m_DecayMode = DecayMode; }
  /// Return the decay mode (normal, ignore, build-up activation)
  int GetDecayMode() const { return m_DecayMode; }

  /// Set the hadron simulation package 
  void SetPhysicsListHD(const int PhysicsListHD) { m_PhysicsListHD = PhysicsListHD; }
  /// Return the hadron simulation package 
  int GetPhysicsListHD() const { return m_PhysicsListHD; }

  /// Return if the data shall be stored in binary format
  bool StoreBinary() const { return m_StoreBinary; }
  /// Return the simulation info storage flag
  int StoreSimulationInfo() const { return m_StoreSimulationInfo; }
  /// Return the simulation info version flag
  int StoreSimulationInfoVersion() const { return m_StoreSimulationInfoVersion; }
  /// Return true, if simulation info specific to secondaries generated by ionization should be stored
  bool StoreSimulationInfoIonization() const { return m_StoreSimulationInfoIonization; }
  /// Return true, if data shall be store in scientific format x.yzE-uv or in fixed format x.yz
  bool StoreScientific() const { return m_StoreScientific; }
  /// Return the precision in which the data shall be stored 
  int StoreScientificPrecision() const { return m_StoreScientificPrecision; }
  /// Return true, if calibrated hits should be stored
  bool StoreCalibrated() const { return m_StoreCalibrated; }
  /// Return true, if each hit is stored in an own event
  bool StoreOneHitPerEvent() const { return m_StoreOneHitPerEvent; }
  /// Return the minimum amount of energy an event must deposit before it is stored (default: -1E+40*keV)
  double StoreMinimumEnergy() const { return m_StoreMinimumEnergy; }
  /// Return the maximum energy loss an event can have before it is discarded (default: 1E+40*keV)
  double StoreMaximumEnergyLoss() const { return m_StoreMaximumEnergyLoss; }
  /// Return true, if the hits should be discretized
  bool DiscretizeHits() const { return m_DiscretizeHits; }
  /// Return the watched volumes list
  vector<MString> GetStoreSimulationInfoWatchedVolumes() const { return m_StoreSimulationInfoWatchedVolumes; }
  
  /// Set the pre-trigger mode (either store everything, only events with hits, or fully pretriggered events)
  void SetPreTriggerMode(const int PreTriggerMode) { m_PreTriggerMode = PreTriggerMode; }
  /// Return true, if calibrated hits should be stored
  int GetPreTriggerMode() const { return m_PreTriggerMode; }
  
  /// Return the black absorber list
  vector<MString> GetBlackAbsorbers() const { return m_BlackAbsorbers; }

  /// Return true, if overlapchecks have to be performed
  bool PerformOverlapCheck() const { return m_CheckForOverlaps; }
  /// Return the numbere of random points of the overlap check
  int ResolutionOverlapCheck() const { return m_OverlapCheckResolution; }
  /// Return the tolerance of the overlap check
  double ToleranceOverlapCheck() const { return m_OverlapCheckTolerance; }

  /// True, if we have to dump cross sections
  bool CreateCrossSectionFiles() const { return m_CreateCrossSectionFiles; }
  /// The directory where to store the cross section files
  MString GetCrossSectionFileDirectory() const { return m_CrossSectionFileDirectory; }

  
  /// The decay mode normal
  static const int c_DecayModeNormal;
  /// The decay mode ignore
  static const int c_DecayModeIgnore;
  /// The decay mode build-up
  static const int c_DecayModeBuildUp;
  /// The decay mode build-up for 2-stage activation simulations
  static const int c_DecayModeActivationBuildUp;
  /// The decay mode delayed decay for 2-stage activation simulations
  static const int c_DecayModeActivationDelayedDecay;

  /// Pretriggering is off
  static const int c_PreTriggerEverything;
  /// Pretriggering is set to store any hit irrespectable if it is in normal or veto detector
  static const int c_PreTriggerEveryEventWithHits;
  /// Pretriggering is on
  static const int c_PreTriggerFull;
  
  
  
  // protected methods:
protected:
  /// Parse the geometry file
  virtual bool Parse();

  /// Return a run per name
  MCRun* GetRun(MString Name);
  /// Return an activator per name
  MCActivator* GetActivator(MString Name);
  /// Return a source per name
  MCSource* GetSource(MString Name);
  /// Return a region per name
  MCRegion* GetRegion(MString Name);

  // protected members:
protected:


  // private members:
private:
  /// List of all (parsed) runs of the parameter file 
  vector<MCRun> m_RunList;

  /// List of all (parsed) activators of the parameter file 
  vector<MCActivator> m_ActivatorList;

  /// Name of the geometry file
  MString m_GeometryFileName;

  /// A default range cut 
  double m_DefaultRangeCut;

  /// List of all regions for which special range cuts are defined
  vector<MCRegion> m_RegionList;

  /// Electromagnetic physics list (high/low energy, doppler, polarized, etc.)
  int m_PhysicsListEM;
  /// Hadron physics list
  int m_PhysicsListHD;

  /// The decay mode
  int m_DecayMode;

  /// The pre-trigger mode (either store everything, only events with hits, or fully pretriggered events)
  int m_PreTriggerMode;
  
  /// True, if the output should be in calibrated format (positions, keV)
  bool m_StoreCalibrated;
  /// True, if the output should be stored in binary format
  bool m_StoreBinary;
  /// True, if simulation info shall be store in scientific format x.yzE-uv or in fixed format x.yz
  bool m_StoreScientific;
  /// Return the precision (number of digits after '.') in which the data shall be stored 
  int m_StoreScientificPrecision;
  /// The simulation information storage level flag
  int m_StoreSimulationInfo;
  /// The simulation information storage version flag
  int m_StoreSimulationInfoVersion;
  /// True, if simulation info specific to secondaries generated by ionization should be stored
  bool m_StoreSimulationInfoIonization;

  /// True if each hit is stored in its own event
  bool m_StoreOneHitPerEvent;
  /// An event must at least deposit this amount of energy to be stored (default: -1E+40*keV)
  double m_StoreMinimumEnergy;
  /// The maximum energy loss an event can have before it is discarded (default: 1E+40*keV)
  double m_StoreMaximumEnergyLoss;
  /// True, if hits should be discretized in the volume voxels
  bool m_DiscretizeHits;
  /// List of watched volumes to store enter & exit information
  vector<MString> m_StoreSimulationInfoWatchedVolumes;

  /// List of black absorber volumes which kill each entered particle
  vector<MString> m_BlackAbsorbers;


  /// True, if we should check for overlaps during reconstruction
  bool m_CheckForOverlaps;
  /// Number of random points on volume surface for overlap check (around 1000)
  int m_OverlapCheckResolution;
  /// Tolerance for the overlap check (around 1 um) 
  double m_OverlapCheckTolerance;

  /// True, if we have to dump cross sections
  bool m_CreateCrossSectionFiles;
  /// The directory where to store the cross section files
  MString m_CrossSectionFileDirectory;

  /// ID of active run
  unsigned int m_ActiveRun;

  /// The activation time constant: The time during which decays are consindered coincident
  double m_DetectorTimeConstant;
};

#endif


/*
 * MCParameterFile.hh: the end...
 ******************************************************************************/
