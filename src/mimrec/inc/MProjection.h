/*
 * MProjection.h
 *
 * Copyright (C) by Andreas Zoglauer.
 * All rights reserved.
 *
 * Please see the source-file for the copyright-notice.
 *
 */


#ifndef __MProjection__
#define __MProjection__


////////////////////////////////////////////////////////////////////////////////


// ROOT libs:

// MEGAlib libs:
#include "MGlobal.h"
#include "MPhysicalEvent.h"
#include "MComptonEvent.h"
#include "MPairEvent.h"
#include "MPhotoEvent.h"

// Forward declarations:


////////////////////////////////////////////////////////////////////////////////


class MProjection
{
  // Public Interface:
 public:
  //! Default constructor
  MProjection(int CoordinateSystem);
  //! Default destructor
  virtual ~MProjection();

  //! Choose if you want to use maths approximations
  virtual void SetApproximatedMaths(bool Approximated = true) { m_ApproximatedMaths = Approximated; }

  //! Assimilate the data of this event
  virtual bool Assimilate(MPhysicalEvent* Event);


  // Projection coordinate systems:
  //! ID of galactic coordinates
  static const int c_Galactic;
  //! ID of spheric coordinates
  static const int c_Spheric;
  //! ID of Cartesian coordinates 2D
  static const int c_Cartesian2D;
  //! ID of Cartesian coordinates 3D
  static const int c_Cartesian3D;

  // protected methods:
 protected:


  // private methods:
 private:


  // protected members:
 protected:
  //! The actual used coordinate system
  int m_CoordinateSystem;
   
   //! The current event
  MPhysicalEvent* m_Event;
  //! For faster access: Compton event
  MComptonEvent* m_C;
  //! For faster access: Pair event
  MPairEvent* m_P;
  //! For faster access: Photo event
  MPhotoEvent* m_Photo;

  //! True if we should used approxiamted maths
  bool m_ApproximatedMaths;

  
  
  // private members:
 private:



#ifdef ___CINT___
 public:
  ClassDef(MProjection, 0) // no description
#endif

};

#endif


////////////////////////////////////////////////////////////////////////////////
