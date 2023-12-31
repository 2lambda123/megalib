/*
 * MResponseMatrixO14.cxx
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
// MResponseMatrixO14
//
////////////////////////////////////////////////////////////////////////////////


// Include the header:
#include "MResponseMatrixO14.h"

// Standard libs:
#include <iostream>
#include <functional>
#include <algorithm>
#include <limits>
using namespace std;

// ROOT libs:
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TCanvas.h"

// MEGAlib libs:
#include "MAssert.h"
#include "MStreams.h"
#include "MFileResponse.h"
#include "MTimer.h"

////////////////////////////////////////////////////////////////////////////////


#ifdef ___CLING___
ClassImp(MResponseMatrixO14)
#endif


////////////////////////////////////////////////////////////////////////////////


MResponseMatrixO14::MResponseMatrixO14() :
  MResponseMatrixOx()
{
  // default constructor

  Init();
}


////////////////////////////////////////////////////////////////////////////////


MResponseMatrixO14::MResponseMatrixO14(vector<float> x1Axis, vector<float> x2Axis, 
                                       vector<float> x3Axis, vector<float> x4Axis, 
                                       vector<float> x5Axis, vector<float> x6Axis, 
                                       vector<float> x7Axis, vector<float> x8Axis, 
                                       vector<float> x9Axis, vector<float> x10Axis, 
                                       vector<float> x11Axis, vector<float> x12Axis, 
                                       vector<float> x13Axis, vector<float> x14Axis) :
  MResponseMatrixOx()
{
  // constructor

  Init();
  SetAxis(x1Axis, x2Axis, x3Axis, x4Axis, x5Axis, x6Axis, x7Axis, x8Axis, 
          x9Axis, x10Axis, x11Axis, x12Axis, x13Axis, x14Axis);
}


////////////////////////////////////////////////////////////////////////////////


MResponseMatrixO14::MResponseMatrixO14(MString Name, 
                                       vector<float> x1Axis, vector<float> x2Axis, 
                                       vector<float> x3Axis, vector<float> x4Axis, 
                                       vector<float> x5Axis, vector<float> x6Axis, 
                                       vector<float> x7Axis, vector<float> x8Axis,
                                       vector<float> x9Axis, vector<float> x10Axis, 
                                       vector<float> x11Axis, vector<float> x12Axis, 
                                       vector<float> x13Axis, vector<float> x14Axis) :
  MResponseMatrixOx(Name)
{
  // constructor

  Init();
  SetAxis(x1Axis, x2Axis, x3Axis, x4Axis, x5Axis, x6Axis, x7Axis, x8Axis, 
          x9Axis, x10Axis, x11Axis, x12Axis, x13Axis, x14Axis);
}


////////////////////////////////////////////////////////////////////////////////


MResponseMatrixO14::~MResponseMatrixO14()
{
  // default destructor
}


////////////////////////////////////////////////////////////////////////////////


void MResponseMatrixO14::Init()
{
  // Initializations common to all constructors

  m_Order = 14;
  m_AxisO14.clear();
  m_AxesO13.clear();
}


////////////////////////////////////////////////////////////////////////////////


bool MResponseMatrixO14::operator==(const MResponseMatrixO14& R)
{
  // Two matrixes are identical if they have the same axis:

  if (m_AxisO14 != R.m_AxisO14) {
    return false;
  }

  for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
    if (!(m_AxesO13[i] == R.m_AxesO13[i])) {
      return false;
    }
  }

  return true;
}


////////////////////////////////////////////////////////////////////////////////


MResponseMatrixO14& MResponseMatrixO14::operator+=(const MResponseMatrixO14& R)
{
  // Append a matrix to this one

  if (*this == R) {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      m_AxesO13[i] += R.m_AxesO13[i];
    }
  } else {
    merr<<"Matrices have different axes!"<<endl;
  }

  return *this;
}


////////////////////////////////////////////////////////////////////////////////


MResponseMatrixO14& MResponseMatrixO14::operator-=(const MResponseMatrixO14& R)
{
  // Append a matrix to this one

  if (*this == R) {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      m_AxesO13[i] -= R.m_AxesO13[i];
    }
  } else {
    merr<<"Matrices have different axes!"<<endl;
  }

  return *this;
}


////////////////////////////////////////////////////////////////////////////////


MResponseMatrixO14& MResponseMatrixO14::operator/=(const MResponseMatrixO14& R)
{
  // Append a matrix to this one

  if (*this == R) {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      m_AxesO13[i] /= R.m_AxesO13[i];
    }
  } else {
    merr<<"Matrices have different axes!"<<endl;
  }

  return *this;
}


////////////////////////////////////////////////////////////////////////////////


MResponseMatrixO14& MResponseMatrixO14::operator+=(const float& Value)
{
  // Append a matrix to this one

  for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
    m_AxesO13[i] += Value;
  }

  return *this;
}


////////////////////////////////////////////////////////////////////////////////


MResponseMatrixO14& MResponseMatrixO14::operator*=(const float& Value)
{
  // Append a matrix to this one

  for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
    m_AxesO13[i] *= Value;
  }

  return *this;
}


////////////////////////////////////////////////////////////////////////////////


void MResponseMatrixO14::SetAxis(vector<float> x1Axis, vector<float> x2Axis, 
                                 vector<float> x3Axis, vector<float> x4Axis, 
                                 vector<float> x5Axis, vector<float> x6Axis, 
                                 vector<float> x7Axis, vector<float> x8Axis, 
                                 vector<float> x9Axis, vector<float> x10Axis, 
                                 vector<float> x11Axis, vector<float> x12Axis, 
                                 vector<float> x13Axis, vector<float> x14Axis)
{
  // Set the axes

  if (x14Axis.size() == 0) {
    merr<<"Size of axis 14 is zero!"<<endl;
    massert(false);
    return;    
  }

  if (IsIncreasing(x14Axis) == false) {
    merr<<"Axes 14 is not in increasing order!"<<endl;
    massert(false);
    return;
  }

  if ((x1Axis.size()-1)*(x2Axis.size()-1)*(x3Axis.size()-1)*
      (x4Axis.size()-1)*(x5Axis.size()-1)*(x6Axis.size()-1)*
      (x7Axis.size()-1)*(x8Axis.size()-1)*(x9Axis.size()-1)*
      (x10Axis.size()-1)*(x11Axis.size()-1)*(x12Axis.size()-1)*
      (x13Axis.size()-1)*(x14Axis.size()-1) > c_SizeLimit) {
    merr<<"The total number of bins is larger than the limit of "
        <<c_SizeLimit<<endl;
    massert(false);
    return;
  }

  m_AxisO14 = x14Axis;

  m_AxesO13.resize(m_AxisO14.size()-1);
  for (unsigned int b = 0; b < m_AxisO14.size()-1; ++b) {
    m_AxesO13[b].SetAxis(x1Axis, x2Axis, x3Axis, x4Axis, x5Axis, x6Axis, 
                         x7Axis, x8Axis, x9Axis, x10Axis, x11Axis, x12Axis, x13Axis);
    m_AxesO13[b].SetName(m_Name);
  }
}


////////////////////////////////////////////////////////////////////////////////


void MResponseMatrixO14::SetAxisNames(MString x1Name, MString x2Name, 
                                      MString x3Name, MString x4Name, 
                                      MString x5Name, MString x6Name, 
                                      MString x7Name, MString x8Name, 
                                      MString x9Name, MString x10Name, 
                                      MString x11Name, MString x12Name, 
                                      MString x13Name, MString x14Name)
{
  // Set the names of the axes:

  m_NameAxisO14 = x14Name;
  for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
    m_AxesO13[i].SetAxisNames(x1Name, x2Name, x3Name, x4Name, x5Name, x6Name, 
                              x7Name, x8Name, x9Name, x10Name, x11Name, x12Name, 
                              x13Name);
  }
}


////////////////////////////////////////////////////////////////////////////////


MString MResponseMatrixO14::GetAxisName(unsigned int order) const
{
  // Return the name of the axis

  massert(order >= 1 && order <= 14);

  if (order == 14) {
    return m_NameAxisO14;
  } else {
    massert(m_AxisO14.size() > 0);
    return m_AxesO13[0].GetAxisName(order);
  }
}


////////////////////////////////////////////////////////////////////////////////


void MResponseMatrixO14::SetBinContent(unsigned int x1, unsigned int x2, 
                                       unsigned int x3, unsigned int x4, 
                                       unsigned int x5, unsigned int x6, 
                                       unsigned int x7, unsigned int x8, 
                                       unsigned int x9, unsigned int x10, 
                                       unsigned int x11, unsigned int x12, 
                                       unsigned int x13, unsigned int x14, 
                                       float Value)
{
  // Set the content of the bin

  massert(x14 < m_AxisO14.size()-1);
  
  m_AxesO13[x14].SetBinContent(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, 
                               x12, x13, Value);
}


////////////////////////////////////////////////////////////////////////////////


void MResponseMatrixO14::Add(float x1, float x2, float x3, float x4, 
                             float x5, float x6, float x7, float x8, 
                             float x9, float x10, float x11, float x12, 
                             float x13, float x14, float Value)
{
  /// Add a value to the bin closest to x, y, z

  // Get Position:
  int Position = -1; 
  for (unsigned int i = 0; i < m_AxisO14.size(); ++i) {
    if (m_AxisO14[i] > x14) {
      break;
    } 
    Position = (int) i;
  }
  
  if (Position <= -1) {
    mdebug<<"Underflow in "<<m_Name
          <<": ("<<x1<<", "<<x2<<", "<<x3<<", "<<x4<<", "<<x5<<", "<<x6
          <<", "<<x7<<", "<<x8<<", "<<x9<<", "<<x10<<", "<<x11<<", "<<x12
          <<", "<<x13<<", "<<x14<<") = "<<Value<<endl;
    return;
  }
  if (Position >= (int) m_AxisO14.size()-1) {
    mdebug<<"Overflow in "
          <<m_Name<<": ("<<x1<<", "<<x2<<", "<<x3<<", "<<x4<<", "<<x5<<", "
          <<x6<<", "<<x7<<", "<<x8<<", "<<x9<<", "<<x10<<", "<<x11<<", "
          <<x12<<", "<<x13<<", "<<x14<<") = "<<Value<<endl;
    return;
  }

  m_AxesO13[Position].Add(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, Value);
}


////////////////////////////////////////////////////////////////////////////////


void MResponseMatrixO14::SetMatrix(unsigned int b, MResponseMatrixO13 R)
{
  // Set a whole sub matrix

  massert(b < m_AxisO14.size());
  m_AxesO13[b] = R;
}


////////////////////////////////////////////////////////////////////////////////


unsigned long MResponseMatrixO14::GetNBins() const
{
  // Return the number of bins

  if (m_AxisO14.size() < 2) return 0;
  unsigned long Bins = m_AxisO14.size()-1;
  return Bins*m_AxesO13[0].GetNBins();
}


////////////////////////////////////////////////////////////////////////////////


unsigned int MResponseMatrixO14::GetAxisBins(unsigned int order) const
{
  // Return the number of bins for the main axis O14

  massert(order >= 1 && order <= 14);

  if (order == 14) {
    return m_AxisO14.size()-1;
  } else {
    massert(m_AxisO14.size() > 0);
    return m_AxesO13[0].GetAxisBins(order);
  }
}


////////////////////////////////////////////////////////////////////////////////


vector<float> MResponseMatrixO14::GetAxis(unsigned int order) const
{
  // Return the axis content at bin b

  massert(order >= 1 && order <= 14);

  if (order == 14) {
    return m_AxisO14;
  } else {
    massert(GetNBins() > 0);
    return m_AxesO13[0].GetAxis(order);
  }
}

////////////////////////////////////////////////////////////////////////////////


float MResponseMatrixO14::GetAxisContent(unsigned int b, unsigned int order) const
{
  // Return the axis content at bin b

  massert(order >= 1 && order <= 14);

  if (order == 14) {
    massert(b < m_AxisO14.size());
    return m_AxisO14[b];
  } else {
    massert(GetNBins() > 0);
    return m_AxesO13[0].GetAxisContent(b, order);
  }
}


////////////////////////////////////////////////////////////////////////////////


float MResponseMatrixO14::GetAxisBinCenter(unsigned int b, unsigned int order) const
{
  // Return the center of axis bin b

  massert(order >= 1 && order <= 14);

  if (order == 14) {
    massert(b < m_AxisO14.size()-1);
    return 0.5*(m_AxisO14[b]+m_AxisO14[b+1]);
  } else {
    massert(GetNBins() > 0);
    return m_AxesO13[0].GetAxisBinCenter(b, order);
  }
}


////////////////////////////////////////////////////////////////////////////////


unsigned int MResponseMatrixO14::GetAxisBin(float x, unsigned int order) const
{
  // Return the center of axis bin b

  massert(order >= 1 && order <= 14);

  // Get Position:
  if (order == 14) {
    unsigned int Position = c_Outside; 
    for (unsigned int i = 0; i < m_AxisO14.size(); ++i) {
      if (m_AxisO14[i] > x) {
        break;
      } 
      Position = i;
    }
    
    if (Position >= m_AxisO14.size()-1) {
      Position = c_Outside;
    }
    return Position;
  } else {
    massert(GetNBins() > 0);
    return m_AxesO13[0].GetAxisBin(x, order);
  }
}


////////////////////////////////////////////////////////////////////////////////


float MResponseMatrixO14::GetAxisMinimum(unsigned int order) const
{
   // Return the lower edge of the lowest bin of the axis 

  massert(order >= 1 && order <= 14);

  if (order == 14) {
    massert(m_AxisO14.size() > 0);
    return m_AxisO14[0];
  } else {
    massert(GetNBins() > 0);
    return m_AxesO13[0].GetAxisMinimum(order);
  }
}


////////////////////////////////////////////////////////////////////////////////


float MResponseMatrixO14::GetAxisMaximum(unsigned int order) const
{
   // Return the upper edge of the highest bin of the axis 

  massert(order >= 1 && order <= 14);

  if (order == 14) {
    massert(m_AxisO14.size() > 0);
    return m_AxisO14[m_AxisO14.size()-1];
  } else {
    massert(GetNBins() > 0);
    return m_AxesO13[0].GetAxisMaximum(order);
  }
}


////////////////////////////////////////////////////////////////////////////////


float MResponseMatrixO14::GetAxisLowEdge(unsigned int b, unsigned int order) const
{
   // Return the lower edge of bin b 

  massert(order >= 1 && order <= 14);

  if (order == 14) {
    massert(b < m_AxisO14.size()-1);
    return m_AxisO14[b];
  } else {
    massert(GetNBins() > 0);
    return m_AxesO13[0].GetAxisLowEdge(b, order);
  }
}


////////////////////////////////////////////////////////////////////////////////


float MResponseMatrixO14::GetAxisHighEdge(unsigned int b, unsigned int order) const
{
   // Return the high edge of bin b 

  massert(order >= 1 && order <= 14);

  if (order == 14) {
    massert(b < m_AxisO14.size()-1);
    return m_AxisO14[b+1];
  } else {
    massert(GetNBins() > 0);
    return m_AxesO13[0].GetAxisHighEdge(b, order);
  } 
}


////////////////////////////////////////////////////////////////////////////////


float MResponseMatrixO14::GetBinContent(unsigned int x1, unsigned int x2, 
                                        unsigned int x3, unsigned int x4, 
                                        unsigned int x5, unsigned int x6, 
                                        unsigned int x7, unsigned int x8, 
                                        unsigned int x9, unsigned int x10, 
                                        unsigned int x11, unsigned int x12, 
                                        unsigned int x13, unsigned int x14) const
{
  // Return the content of bin x, y

  massert(GetNBins() > 0);
  massert(x1 < GetAxisBins(1));
  massert(x2 < GetAxisBins(2));
  massert(x3 < GetAxisBins(3));
  massert(x4 < GetAxisBins(4));
  massert(x5 < GetAxisBins(5));
  massert(x6 < GetAxisBins(6));
  massert(x7 < GetAxisBins(7));
  massert(x8 < GetAxisBins(8));
  massert(x9 < GetAxisBins(9));
  massert(x10 < GetAxisBins(10));
  massert(x11 < GetAxisBins(11));
  massert(x12 < GetAxisBins(12));
  massert(x13 < GetAxisBins(13));
  massert(x14 < GetAxisBins(14));

  return m_AxesO13[x14].GetBinContent(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13);
}


////////////////////////////////////////////////////////////////////////////////


float MResponseMatrixO14::GetBinArea(unsigned int x1, unsigned int x2, 
                                     unsigned int x3, unsigned int x4, 
                                     unsigned int x5, unsigned int x6, 
                                     unsigned int x7, unsigned int x8, 
                                     unsigned int x9, unsigned int x10, 
                                     unsigned int x11, unsigned int x12, 
                                     unsigned int x13, unsigned int x14) const
{
  // Return the area of this bin

  massert(x14 < m_AxisO14.size()-1);

  return (m_AxisO14[x14+1]-m_AxisO14[x14])*m_AxesO13[x14].GetBinArea(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13);
}


////////////////////////////////////////////////////////////////////////////////


float MResponseMatrixO14::GetBinContent(unsigned int x1, unsigned int x1axis, 
                                        unsigned int x2, unsigned int x2axis,
                                        unsigned int x3, unsigned int x3axis,
                                        unsigned int x4, unsigned int x4axis,
                                        unsigned int x5, unsigned int x5axis,
                                        unsigned int x6, unsigned int x6axis,
                                        unsigned int x7, unsigned int x7axis,
                                        unsigned int x8, unsigned int x8axis,
                                        unsigned int x9, unsigned int x9axis,
                                        unsigned int x10, unsigned int x10axis,
                                        unsigned int x11, unsigned int x11axis,
                                        unsigned int x12, unsigned int x12axis,
                                        unsigned int x13, unsigned int x13axis,
                                        unsigned int x14, unsigned int x14axis) const
{
  // 

  massert(GetNBins() > 0);
  massert(x1 < GetAxisBins(x1axis));
  massert(x2 < GetAxisBins(x2axis));
  massert(x3 < GetAxisBins(x3axis));
  massert(x4 < GetAxisBins(x4axis));
  massert(x5 < GetAxisBins(x5axis));
  massert(x6 < GetAxisBins(x6axis));
  massert(x7 < GetAxisBins(x7axis));
  massert(x8 < GetAxisBins(x8axis));
  massert(x9 < GetAxisBins(x9axis));
  massert(x10 < GetAxisBins(x10axis));
  massert(x11 < GetAxisBins(x11axis));
  massert(x12 < GetAxisBins(x12axis));
  massert(x13 < GetAxisBins(x13axis));
  massert(x14 < GetAxisBins(x14axis));
  
  if (x1axis == 14) {
    return m_AxesO13[x1].GetBinContent(x2, x2axis, x3, x3axis, x4, x4axis, x5, x5axis, x6, x6axis, x7, x7axis, x8, x8axis, 
                                       x9, x9axis, x10, x10axis, x11, x11axis, x12, x12axis, x13, x13axis, x14, x14axis);
  } else if (x2axis == 14) {
    return m_AxesO13[x2].GetBinContent(x1, x1axis, x3, x3axis, x4, x4axis, x5, x5axis, x6, x6axis, x7, x7axis, x8, x8axis, 
                                       x9, x9axis, x10, x10axis, x11, x11axis, x12, x12axis, x13, x13axis, x14, x14axis);
  } else if (x3axis == 14) {
    return m_AxesO13[x3].GetBinContent(x1, x1axis, x2, x2axis, x4, x4axis, x5, x5axis, x6, x6axis, x7, x7axis, x8, x8axis, 
                                       x9, x9axis, x10, x10axis, x11, x11axis, x12, x12axis, x13, x13axis, x14, x14axis);
  } else if (x4axis == 14) {
    return m_AxesO13[x4].GetBinContent(x1, x1axis, x2, x2axis, x3, x3axis, x5, x5axis, x6, x6axis, x7, x7axis, x8, x8axis, 
                                       x9, x9axis, x10, x10axis, x11, x11axis, x12, x12axis, x13, x13axis, x14, x14axis);
  } else if (x5axis == 14) {
    return m_AxesO13[x5].GetBinContent(x1, x1axis, x2, x2axis, x3, x3axis, x4, x4axis, x6, x6axis, x7, x7axis, x8, x8axis, 
                                       x9, x9axis, x10, x10axis, x11, x11axis, x12, x12axis, x13, x13axis, x14, x14axis);
  } else if (x6axis == 14) {
    return m_AxesO13[x6].GetBinContent(x1, x1axis, x2, x2axis, x3, x3axis, x4, x4axis, x5, x5axis, x7, x7axis, x8, x8axis, 
                                       x9, x9axis, x10, x10axis, x11, x11axis, x12, x12axis, x13, x13axis, x14, x14axis);
  } else if (x7axis == 14) {
    return m_AxesO13[x7].GetBinContent(x1, x1axis, x2, x2axis, x3, x3axis, x4, x4axis, x5, x5axis, x6, x6axis, x8, x8axis, 
                                       x9, x9axis, x10, x10axis, x11, x11axis, x12, x12axis, x13, x13axis, x14, x14axis);
  } else if (x8axis == 14) {
    return m_AxesO13[x8].GetBinContent(x1, x1axis, x2, x2axis, x3, x3axis, x4, x4axis, x5, x5axis, x6, x6axis, x7, x7axis, 
                                       x9, x9axis, x10, x10axis, x11, x11axis, x12, x12axis, x13, x13axis, x14, x14axis);
  } else if (x9axis == 14) {
    return m_AxesO13[x9].GetBinContent(x1, x1axis, x2, x2axis, x3, x3axis, x4, x4axis, x5, x5axis, x6, x6axis, x7, x7axis, 
                                       x8, x8axis, x10, x10axis, x11, x11axis, x12, x12axis, x13, x13axis, x14, x14axis);
  } else if (x10axis == 14) {
    return m_AxesO13[x10].GetBinContent(x1, x1axis, x2, x2axis, x3, x3axis, x4, x4axis, x5, x5axis, x6, x6axis, x7, x7axis, 
                                        x8, x8axis, x9, x9axis, x11, x11axis, x12, x12axis, x13, x13axis, x14, x14axis);
  } else if (x11axis == 14) {
    return m_AxesO13[x11].GetBinContent(x1, x1axis, x2, x2axis, x3, x3axis, x4, x4axis, x5, x5axis, x6, x6axis, x7, x7axis, 
                                        x8, x8axis, x9, x9axis, x10, x10axis, x12, x12axis, x13, x13axis, x14, x14axis);
  } else if (x12axis == 14) {
    return m_AxesO13[x12].GetBinContent(x1, x1axis, x2, x2axis, x3, x3axis, x4, x4axis, x5, x5axis, x6, x6axis, x7, x7axis, 
                                        x8, x8axis, x9, x9axis, x10, x10axis, x11, x11axis, x13, x13axis, x14, x14axis);
  } else if (x13axis == 14) {
    return m_AxesO13[x13].GetBinContent(x1, x1axis, x2, x2axis, x3, x3axis, x4, x4axis, x5, x5axis, x6, x6axis, x7, x7axis, 
                                        x8, x8axis, x9, x9axis, x10, x10axis, x11, x11axis, x12, x12axis, x14, x14axis);
  } else if (x14axis == 14) {
    return m_AxesO13[x14].GetBinContent(x1, x1axis, x2, x2axis, x3, x3axis, x4, x4axis, x5, x5axis, x6, x6axis, x7, x7axis, 
                                        x8, x8axis, x9, x9axis, x10, x10axis, x11, x11axis, x12, x12axis, x13, x13axis);
  } else {
    merr<<"Axes are not correctly defined: x1-axis="<<x1axis
        <<" x2-axis="<<x2axis<<" x3-axis="<<x3axis
        <<" x4-axis="<<x4axis<<" x5-axis="<<x5axis
        <<" x6-axis="<<x6axis<<" x7-axis="<<x7axis
        <<" x8-axis="<<x8axis<<" x9-axis="<<x9axis
        <<" x10-axis="<<x10axis<<" x11-axis="<<x11axis
        <<" x12-axis="<<x12axis<<" x13-axis="<<x13axis
        <<" x14-axis="<<x14axis<<endl;
    return 0;
  }
}


////////////////////////////////////////////////////////////////////////////////


float MResponseMatrixO14::GetInterpolated(float x1, float x2, float x3, float x4, 
                                          float x5, float x6, float x7, float x8, 
                                          float x9, float x10, float x11, float x12, 
                                          float x13, float x14, bool DoExtrapolate) const
{
  // Return the array-data according to value x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14

  // We have to distinguish to different cases:
  // (1) the values are at the center of the bin (e.g. used by response files)
  // (2) the values are the edge (axis point) (e.g. used by MEGAlib absorption files) 

  // We can assume that the bin size is larger equal 2

  // The linear interpolation goes as follows:
  // y = m*x + t
  // m = (y2-y1)/(x2-x1)
  // t = y1 - m*x1
  // y = y1 + (x-x1)*m

  if (m_ValuesCentered == true) { 
    if (m_AxisO14.size() == 2) {
      return m_AxesO13.front().GetInterpolated(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, DoExtrapolate);
    } else {
      // Get Position:
      int Position = FindBinCentered(m_AxisO14, x14);

      // Take care of boundaries:
      if (Position < 0) {
        if (DoExtrapolate == true) {
          Position = 0; // extrapolate below lower edge
        } else {
          return m_AxesO13.front().GetInterpolated(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, DoExtrapolate);
        }
      } else if (Position >= int(m_AxesO13.size()-1)) {
        if (DoExtrapolate == true) {
          Position = int(m_AxesO13.size()-2); // extrapolate above higher edge
          // limits of highest CENTERED bin are m_AxesO13.size()-2 and m_AxesO13.size()-1 !!
        } else {
          return m_AxesO13.back().GetInterpolated(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, DoExtrapolate);
        }
      }

      // Interpolate:
      return m_AxesO13[Position].GetInterpolated(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, DoExtrapolate) + (x14 - GetAxisBinCenter(Position))/
        (GetAxisBinCenter(Position+1) - GetAxisBinCenter(Position))*
        (m_AxesO13[Position+1].GetInterpolated(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, DoExtrapolate) - 
         m_AxesO13[Position].GetInterpolated(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, DoExtrapolate));
    }
  } else {
    // Get Position:
    int Position = FindBin(m_AxisO14, x14);

    // Take care of boundaries:
    if (Position < 0) {
      if (DoExtrapolate == true) {
        Position = 0; // extrapolate below lower edge
      } else {
        return m_AxesO13.front().GetInterpolated(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, DoExtrapolate);
      }
    } else if (Position >= int(m_AxisO14.size()-1)) {
      if (DoExtrapolate == true) {
        Position = int(m_AxisO14.size()-2); // extrapolate above higher edge
        // limits of highest bin are m_AxisO14.size()-2 and  m_AxisO14.size()-1 !!
      } else {
        return m_AxesO13.back().GetInterpolated(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, DoExtrapolate);
      }
    }
    
    // Interpolate:
    return m_AxesO13[Position].GetInterpolated(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, DoExtrapolate) + (x14 - m_AxisO14[Position])/
      (m_AxisO14[Position+1] - m_AxisO14[Position])*
      (m_AxesO13[Position+1].GetInterpolated(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, DoExtrapolate) - 
       m_AxesO13[Position].GetInterpolated(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, DoExtrapolate));
  }
}


////////////////////////////////////////////////////////////////////////////////


float MResponseMatrixO14::Get(float x1, float x2, float x3, float x4, float x5, 
                              float x6, float x7, float x8, float x9, float x10, 
                              float x11, float x12, float x13, float x14) const
{
  // Return the array-data according to value x 

  if (m_AxisO14.size() == 0) {
    return 0;
  } 

  int Position = FindBin(m_AxisO14, x14);

  if (Position < 0) {
    Position = 0;
  } else if (Position >= int(GetAxisBins())) {
    Position = GetAxisBins()-1;
  }

  return m_AxesO13[Position].Get(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13);
}


////////////////////////////////////////////////////////////////////////////////


float MResponseMatrixO14::GetMaximum() const
{
  // Return the maximum

  float LocMax = -numeric_limits<float>::max();
  float Max = -numeric_limits<float>::max();
  for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
    LocMax = m_AxesO13[i].GetMaximum();
    if (LocMax > Max) {
      Max = LocMax;
    }
  }  

  return Max;
}


////////////////////////////////////////////////////////////////////////////////


float MResponseMatrixO14::GetMinimum() const
{
  // Return the minimum

  float LocMin = numeric_limits<float>::max();
  float Min = numeric_limits<float>::max();
  for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
    LocMin = m_AxesO13[i].GetMinimum();
    if (LocMin < Min) {
      Min = LocMin;
    }
  }  

  return Min;
}


////////////////////////////////////////////////////////////////////////////////


double MResponseMatrixO14::GetSum() const
{
  // Return the sum of all bins:

  double Sum = 0;
  for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
    Sum += m_AxesO13[i].GetSum();
  }  

  return Sum;
}


////////////////////////////////////////////////////////////////////////////////


MResponseMatrixO1 MResponseMatrixO14::GetSumMatrixO1(unsigned int a1) const
{
  // Return the sum of the matrix as function of dimension order
  // Attention: Very inefficient algorithm!

  massert(AreIncreasing(14, a1));

  MResponseMatrixO1 R;
  R.SetAxis(GetAxis(a1));

  if (a1 == 14) {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      R.SetBinContent(i, m_AxesO13[i].GetSum());
    }
  } else {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      R += m_AxesO13[i].GetSumMatrixO1(a1);
    }
  }
  
  return R;
}


////////////////////////////////////////////////////////////////////////////////


MResponseMatrixO2 MResponseMatrixO14::GetSumMatrixO2(unsigned int a1, 
                                                    unsigned int a2) const
{
  // Return the sum of the matrix as function of dimension order
  // Attention: Simple, but very inefficient algorithm!

  massert(AreIncreasing(14, a1, a2));

  MResponseMatrixO2 R;
  R.SetAxis(GetAxis(a1), GetAxis(a2));
  
  if (a2 == 14) {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      R.SetMatrix(i, m_AxesO13[i].GetSumMatrixO1(a1));
    }
  } else {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      R += m_AxesO13[i].GetSumMatrixO2(a1, a2);
    }
  }

  return R;
}


////////////////////////////////////////////////////////////////////////////////


MResponseMatrixO3 MResponseMatrixO14::GetSumMatrixO3(unsigned int a1, 
                                                    unsigned int a2,
                                                    unsigned int a3) const
{
  // Return the sum of the matrix as function of dimension order
  // Attention: Simple, but very inefficient algorithm!

  massert(AreIncreasing(14, a1, a2, a3));

  MResponseMatrixO3 R;
  R.SetAxis(GetAxis(a1), GetAxis(a2), GetAxis(a3));
  
  if (a3 == 14) {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      R.SetMatrix(i, m_AxesO13[i].GetSumMatrixO2(a1, a2));
    }
  } else {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      R += m_AxesO13[i].GetSumMatrixO3(a1, a2, a3);
    }
  }

  return R;
}


////////////////////////////////////////////////////////////////////////////////


MResponseMatrixO4 MResponseMatrixO14::GetSumMatrixO4(unsigned int a1, 
                                                    unsigned int a2,
                                                    unsigned int a3,
                                                    unsigned int a4) const
{
  // Return the sum of the matrix as function of dimension order
  // Attention: Simple, but very inefficient algorithm!

  massert(AreIncreasing(14, a1, a2, a3, a4));

  MResponseMatrixO4 R;
  R.SetAxis(GetAxis(a1), GetAxis(a2), GetAxis(a3), GetAxis(a4));
  
  if (a4 == 14) {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      R.SetMatrix(i, m_AxesO13[i].GetSumMatrixO3(a1, a2, a3));
    }
  } else {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      R += m_AxesO13[i].GetSumMatrixO4(a1, a2, a3, a4);
    }
  }

  return R;
}


////////////////////////////////////////////////////////////////////////////////


MResponseMatrixO5 MResponseMatrixO14::GetSumMatrixO5(unsigned int a1, 
                                                    unsigned int a2,
                                                    unsigned int a3,
                                                    unsigned int a4,
                                                    unsigned int a5) const
{
  // Return the sum of the matrix as function of dimension order
  // Attention: Simple, but very inefficient algorithm!

  massert(AreIncreasing(14, a1, a2, a3, a4, a5));

  MResponseMatrixO5 R;
  R.SetAxis(GetAxis(a1), GetAxis(a2), GetAxis(a3), GetAxis(a4), GetAxis(a5));
  
  if (a5 == 14) {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      R.SetMatrix(i, m_AxesO13[i].GetSumMatrixO4(a1, a2, a3, a4));
    }
  } else {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      R += m_AxesO13[i].GetSumMatrixO5(a1, a2, a3, a4, a5);
    }
  }

  return R;
}


////////////////////////////////////////////////////////////////////////////////


MResponseMatrixO6 MResponseMatrixO14::GetSumMatrixO6(unsigned int a1, 
                                                    unsigned int a2,
                                                    unsigned int a3,
                                                    unsigned int a4,
                                                    unsigned int a5,
                                                    unsigned int a6) const
{
  // Return the sum of the matrix as function of dimension order
  // Attention: Simple, but very inefficient algorithm!

  massert(AreIncreasing(14, a1, a2, a3, a4, a5, a6));

  MResponseMatrixO6 R;
  R.SetAxis(GetAxis(a1), GetAxis(a2), GetAxis(a3), GetAxis(a4), GetAxis(a5), GetAxis(a6));
  
  if (a6 == 14) {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      R.SetMatrix(i, m_AxesO13[i].GetSumMatrixO5(a1, a2, a3, a4, a5));
    }
  } else {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      R += m_AxesO13[i].GetSumMatrixO6(a1, a2, a3, a4, a5, a6);
    }
  }

  return R;
}


////////////////////////////////////////////////////////////////////////////////


MResponseMatrixO7 MResponseMatrixO14::GetSumMatrixO7(unsigned int a1, 
                                                    unsigned int a2,
                                                    unsigned int a3,
                                                    unsigned int a4,
                                                    unsigned int a5,
                                                    unsigned int a6,
                                                    unsigned int a7) const
{
  // Return the sum of the matrix as function of dimension order
  // Attention: Simple, but very inefficient algorithm!

  massert(AreIncreasing(14, a1, a2, a3, a4, a5, a6, a7));

  MResponseMatrixO7 R;
  R.SetAxis(GetAxis(a1), GetAxis(a2), GetAxis(a3), GetAxis(a4), GetAxis(a5), GetAxis(a6), GetAxis(a7));
  
  if (a7 == 14) {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      R.SetMatrix(i, m_AxesO13[i].GetSumMatrixO6(a1, a2, a3, a4, a5, a6));
    }
  } else {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      R += m_AxesO13[i].GetSumMatrixO7(a1, a2, a3, a4, a5, a6, a7);
    }
  }

  return R;
}


////////////////////////////////////////////////////////////////////////////////


MResponseMatrixO8 MResponseMatrixO14::GetSumMatrixO8(unsigned int a1, 
                                                     unsigned int a2,
                                                     unsigned int a3,
                                                     unsigned int a4,
                                                     unsigned int a5,
                                                     unsigned int a6,
                                                     unsigned int a7,
                                                     unsigned int a8) const
{
  // Return the sum of the matrix as function of dimension order
  // Attention: Simple, but very inefficient algorithm!

  massert(AreIncreasing(14, a1, a2, a3, a4, a5, a6, a7, a8));

  MResponseMatrixO8 R;
  R.SetAxis(GetAxis(a1), GetAxis(a2), GetAxis(a3), GetAxis(a4), GetAxis(a5), GetAxis(a6), GetAxis(a7), GetAxis(a8));
  
  if (a8 == 14) {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      R.SetMatrix(i, m_AxesO13[i].GetSumMatrixO7(a1, a2, a3, a4, a5, a6, a7));
    }
  } else {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      R += m_AxesO13[i].GetSumMatrixO8(a1, a2, a3, a4, a5, a6, a7, a8);
    }
  }

  return R;
}


////////////////////////////////////////////////////////////////////////////////


MResponseMatrixO9 MResponseMatrixO14::GetSumMatrixO9(unsigned int a1, 
                                                     unsigned int a2,
                                                     unsigned int a3,
                                                     unsigned int a4,
                                                     unsigned int a5,
                                                     unsigned int a6,
                                                     unsigned int a7,
                                                     unsigned int a8,
                                                     unsigned int a9) const
{
  // Return the sum of the matrix as function of dimension order
  // Attention: Simple, but very inefficient algorithm!

  massert(AreIncreasing(14, a1, a2, a3, a4, a5, a6, a7, a8, a9));

  MResponseMatrixO9 R;
  R.SetAxis(GetAxis(a1), GetAxis(a2), GetAxis(a3), GetAxis(a4), GetAxis(a5), GetAxis(a6), GetAxis(a7), GetAxis(a8), GetAxis(a9));
  
  if (a9 == 14) {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      R.SetMatrix(i, m_AxesO13[i].GetSumMatrixO8(a1, a2, a3, a4, a5, a6, a7, a8));
    }
  } else {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      R += m_AxesO13[i].GetSumMatrixO9(a1, a2, a3, a4, a5, a6, a7, a8, a9);
    }
  }

  return R;
}


////////////////////////////////////////////////////////////////////////////////


MResponseMatrixO10 MResponseMatrixO14::GetSumMatrixO10(unsigned int a1, 
                                                       unsigned int a2,
                                                       unsigned int a3,
                                                       unsigned int a4,
                                                       unsigned int a5,
                                                       unsigned int a6,
                                                       unsigned int a7,
                                                       unsigned int a8,
                                                       unsigned int a9,
                                                       unsigned int a10) const
{
  // Return the sum of the matrix as function of dimension order
  // Attention: Simple, but very inefficient algorithm!

  massert(AreIncreasing(14, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10));

  MResponseMatrixO10 R;
  R.SetAxis(GetAxis(a1), GetAxis(a2), GetAxis(a3), GetAxis(a4), GetAxis(a5), GetAxis(a6), GetAxis(a7), GetAxis(a8), GetAxis(a9), GetAxis(a10));
  
  if (a10 == 14) {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      R.SetMatrix(i, m_AxesO13[i].GetSumMatrixO9(a1, a2, a3, a4, a5, a6, a7, a8, a9));
    }
  } else {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      R += m_AxesO13[i].GetSumMatrixO10(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
    }
  }

  return R;
}


////////////////////////////////////////////////////////////////////////////////


MResponseMatrixO11 MResponseMatrixO14::GetSumMatrixO11(unsigned int a1, 
                                                       unsigned int a2,
                                                       unsigned int a3,
                                                       unsigned int a4,
                                                       unsigned int a5,
                                                       unsigned int a6,
                                                       unsigned int a7,
                                                       unsigned int a8,
                                                       unsigned int a9,
                                                       unsigned int a10,
                                                       unsigned int a11) const
{
  // Return the sum of the matrix as function of dimension order
  // Attention: Simple, but very inefficient algorithm!

  massert(AreIncreasing(14, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11));

  MResponseMatrixO11 R;
  R.SetAxis(GetAxis(a1), GetAxis(a2), GetAxis(a3), GetAxis(a4), GetAxis(a5), GetAxis(a6), GetAxis(a7), GetAxis(a8), GetAxis(a9), GetAxis(a10), GetAxis(a11));
  
  if (a11 == 14) {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      R.SetMatrix(i, m_AxesO13[i].GetSumMatrixO10(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10));
    }
  } else {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      R += m_AxesO13[i].GetSumMatrixO11(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
    }
  }

  return R;
}


////////////////////////////////////////////////////////////////////////////////


MResponseMatrixO12 MResponseMatrixO14::GetSumMatrixO12(unsigned int a1, 
                                                       unsigned int a2,
                                                       unsigned int a3,
                                                       unsigned int a4,
                                                       unsigned int a5,
                                                       unsigned int a6,
                                                       unsigned int a7,
                                                       unsigned int a8,
                                                       unsigned int a9,
                                                       unsigned int a10,
                                                       unsigned int a11,
                                                       unsigned int a12) const
{
  // Return the sum of the matrix as function of dimension order
  // Attention: Simple, but very inefficient algorithm!

  massert(AreIncreasing(14, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12));

  MResponseMatrixO12 R;
  R.SetAxis(GetAxis(a1), GetAxis(a2), GetAxis(a3), GetAxis(a4), GetAxis(a5), 
            GetAxis(a6), GetAxis(a7), GetAxis(a8), GetAxis(a9), GetAxis(a10), 
            GetAxis(a11), GetAxis(a12));
  
  if (a12 == 14) {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      R.SetMatrix(i, m_AxesO13[i].GetSumMatrixO11(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11));
    }
  } else {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      R += m_AxesO13[i].GetSumMatrixO12(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
    }
  }

  return R;
}




////////////////////////////////////////////////////////////////////////////////


MResponseMatrixO13 MResponseMatrixO14::GetSumMatrixO13(unsigned int a1, 
                                                       unsigned int a2,
                                                       unsigned int a3,
                                                       unsigned int a4,
                                                       unsigned int a5,
                                                       unsigned int a6,
                                                       unsigned int a7,
                                                       unsigned int a8,
                                                       unsigned int a9,
                                                       unsigned int a10,
                                                       unsigned int a11,
                                                       unsigned int a12,
                                                       unsigned int a13) const
{
  // Return the sum of the matrix as function of dimension order
  // Attention: Simple, but very inefficient algorithm!

  massert(AreIncreasing(14, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13));

  MResponseMatrixO13 R;
  R.SetAxis(GetAxis(a1), GetAxis(a2), GetAxis(a3), GetAxis(a4), GetAxis(a5), 
            GetAxis(a6), GetAxis(a7), GetAxis(a8), GetAxis(a9), GetAxis(a10), 
            GetAxis(a11), GetAxis(a12), GetAxis(a13));
  
  if (a13 == 14) {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      R.SetMatrix(i, m_AxesO13[i].GetSumMatrixO12(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12));
    }
  } else {
    for (unsigned int i = 0; i < m_AxesO13.size(); ++i) {
      R += m_AxesO13[i].GetSumMatrixO13(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
    }
  }

  return R;
}


////////////////////////////////////////////////////////////////////////////////


MResponseMatrixO14 MResponseMatrixO14::GetSumMatrixO14(unsigned int a1, 
                                                       unsigned int a2, 
                                                       unsigned int a3, 
                                                       unsigned int a4, 
                                                       unsigned int a5, 
                                                       unsigned int a6, 
                                                       unsigned int a7, 
                                                       unsigned int a8, 
                                                       unsigned int a9, 
                                                       unsigned int a10, 
                                                       unsigned int a11, 
                                                       unsigned int a12, 
                                                       unsigned int a13, 
                                                       unsigned int a14) const
{
  // Return the sum of the matrix as function of dimension order
  // Attention: Simple, but very inefficient algorithm!

  massert(AreIncreasing(14, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14));

  return *this;
}


////////////////////////////////////////////////////////////////////////////////


bool MResponseMatrixO14::ReadSpecific(MFileResponse& Parser, 
                                     const MString& Type, 
                                     const int Version)
{
  // Read the data from file directly into this matrix

  bool Ok = true;
  MTokenizer T;

  if (Type == "ResponseMatrixO14") {
//     while (Parser.TokenizeLine(T, true) == true) {
//       if (T.GetNTokens() == 0) continue;
//       if (T.GetTokenAt(0) == "R2") {
//         if (T.GetNTokens() == 4) {
//           Set(T.GetTokenAtAsFloat(1), T.GetTokenAtAsFloat(2), T.GetTokenAtAsFloat(3));
//         } else {
//           mout<<"MResponseMatrixO14: Wrong number of arguments for token R2!"<<endl;
//           Ok = false;
//           break;
//         }
//       }
//     }
  } else if (Type == "ResponseMatrixO14Stream") {
    vector<float> x1Axis;
    vector<float> x2Axis;
    vector<float> x3Axis;
    vector<float> x4Axis;
    vector<float> x5Axis;
    vector<float> x6Axis;
    vector<float> x7Axis;
    vector<float> x8Axis;
    vector<float> x9Axis;
    vector<float> x10Axis;
    vector<float> x11Axis;
    vector<float> x12Axis;
    vector<float> x13Axis;
    vector<float> x14Axis;
    MString x1Name;
    MString x2Name;
    MString x3Name;
    MString x4Name;
    MString x5Name;
    MString x6Name;
    MString x7Name;
    MString x8Name;
    MString x9Name;
    MString x10Name;
    MString x11Name;
    MString x12Name;
    MString x13Name;
    MString x14Name;
    while (Parser.TokenizeLine(T, true) == true) {
      if (T.GetNTokens() == 0) continue;
      if (T.GetTokenAt(0) == "A1") {
        x1Axis = T.GetTokenAtAsFloatVector(1);
      } else if (T.GetTokenAt(0) == "A2") {
        x2Axis = T.GetTokenAtAsFloatVector(1);
      } else if (T.GetTokenAt(0) == "A3") {
        x3Axis = T.GetTokenAtAsFloatVector(1);
      } else if (T.GetTokenAt(0) == "A4") {
        x4Axis = T.GetTokenAtAsFloatVector(1);
      } else if (T.GetTokenAt(0) == "A5") {
        x5Axis = T.GetTokenAtAsFloatVector(1);
      } else if (T.GetTokenAt(0) == "A6") {
        x6Axis = T.GetTokenAtAsFloatVector(1);
      } else if (T.GetTokenAt(0) == "A7") {
        x7Axis = T.GetTokenAtAsFloatVector(1);
      } else if (T.GetTokenAt(0) == "A8") {
        x8Axis = T.GetTokenAtAsFloatVector(1);
      } else if (T.GetTokenAt(0) == "A9") {
        x9Axis = T.GetTokenAtAsFloatVector(1);
      } else if (T.GetTokenAt(0) == "A10") {
        x10Axis = T.GetTokenAtAsFloatVector(1);
      } else if (T.GetTokenAt(0) == "A11") {
        x11Axis = T.GetTokenAtAsFloatVector(1);
      } else if (T.GetTokenAt(0) == "A12") {
        x12Axis = T.GetTokenAtAsFloatVector(1);
      } else if (T.GetTokenAt(0) == "A13") {
        x13Axis = T.GetTokenAtAsFloatVector(1);
      } else if (T.GetTokenAt(0) == "A14") {
        x14Axis = T.GetTokenAtAsFloatVector(1);

      } else if (T.GetTokenAt(0) == "N1") {
        x1Name = T.GetTokenAfterAsString(1);
      } else if (T.GetTokenAt(0) == "N2") {
        x2Name = T.GetTokenAfterAsString(1);
      } else if (T.GetTokenAt(0) == "N3") {
        x3Name = T.GetTokenAfterAsString(1);
      } else if (T.GetTokenAt(0) == "N4") {
        x4Name = T.GetTokenAfterAsString(1);
      } else if (T.GetTokenAt(0) == "N5") {
        x5Name = T.GetTokenAfterAsString(1);
      } else if (T.GetTokenAt(0) == "N6") {
        x6Name = T.GetTokenAfterAsString(1);
      } else if (T.GetTokenAt(0) == "N7") {
        x7Name = T.GetTokenAfterAsString(1);
      } else if (T.GetTokenAt(0) == "N8") {
        x8Name = T.GetTokenAfterAsString(1);
      } else if (T.GetTokenAt(0) == "N9") {
        x9Name = T.GetTokenAfterAsString(1);
      } else if (T.GetTokenAt(0) == "N10") {
        x10Name = T.GetTokenAfterAsString(1);
      } else if (T.GetTokenAt(0) == "N11") {
        x11Name = T.GetTokenAfterAsString(1);
      } else if (T.GetTokenAt(0) == "N12") {
        x12Name = T.GetTokenAfterAsString(1);
      } else if (T.GetTokenAt(0) == "N13") {
        x13Name = T.GetTokenAfterAsString(1);
      } else if (T.GetTokenAt(0) == "N14") {
        x14Name = T.GetTokenAfterAsString(1);
      } else if (T.GetTokenAt(0) == "StartStream") {
        unsigned int StreamSize = T.GetTokenAtAsInt(1);

        if ((x1Axis.size()-1)*(x2Axis.size()-1)*(x3Axis.size()-1)*
            (x4Axis.size()-1)*(x5Axis.size()-1)*(x6Axis.size()-1)*
            (x7Axis.size()-1)*(x8Axis.size()-1)*(x9Axis.size()-1)*
            (x10Axis.size()-1)*(x11Axis.size()-1)*(x12Axis.size()-1)*
            (x13Axis.size()-1)*(x14Axis.size()-1) != StreamSize) {
          mout<<"MResponseMatrixO14: Axis sizes and stream size are not in sync!"<<endl;
          Ok = false;
          break;
        }
        SetAxis(x1Axis, x2Axis, x3Axis, x4Axis, x5Axis, x6Axis, x7Axis, x8Axis, x9Axis, x10Axis, x11Axis, x12Axis, x13Axis, x14Axis);
        SetAxisNames(x1Name, x2Name, x3Name, x4Name, x5Name, x6Name, x7Name, x8Name, x9Name, x10Name, x11Name, x12Name, x13Name, x14Name);

        bool StreamOk = true;
        float Data;
        unsigned int x1, x1_max = GetAxisBins(1); 
        unsigned int x2, x2_max = GetAxisBins(2); 
        unsigned int x3, x3_max = GetAxisBins(3); 
        unsigned int x4, x4_max = GetAxisBins(4); 
        unsigned int x5, x5_max = GetAxisBins(5); 
        unsigned int x6, x6_max = GetAxisBins(6); 
        unsigned int x7, x7_max = GetAxisBins(7); 
        unsigned int x8, x8_max = GetAxisBins(8); 
        unsigned int x9, x9_max = GetAxisBins(9); 
        unsigned int x10, x10_max = GetAxisBins(10); 
        unsigned int x11, x11_max = GetAxisBins(11); 
        unsigned int x12, x12_max = GetAxisBins(12); 
        unsigned int x13, x13_max = GetAxisBins(13); 
        unsigned int x14, x14_max = GetAxisBins(14); 
        for (x14 = 0; x14 < x14_max; ++x14) {
          for (x13 = 0; x13 < x13_max; ++x13) {
            for (x12 = 0; x12 < x12_max; ++x12) {
              for (x11 = 0; x11 < x11_max; ++x11) {
                for (x10 = 0; x10 < x10_max; ++x10) {
                  for (x9 = 0; x9 < x9_max; ++x9) {
                    for (x8 = 0; x8 < x8_max; ++x8) {
                      for (x7 = 0; x7 < x7_max; ++x7) {
                        for (x6 = 0; x6 < x6_max; ++x6) {
                          for (x5 = 0; x5 < x5_max; ++x5) {
                            for (x4 = 0; x4 < x4_max; ++x4) {
                              for (x3 = 0; x3 < x3_max; ++x3) {
                                for (x2 = 0; x2 < x2_max; ++x2) {
                                  for (x1 = 0; x1 < x1_max; ++x1) {
                                    if (StreamSize == 0) {
                                      mout<<"MResponseMatrixO14: Stream has not the correct length (too small)!"<<endl;
                                      StreamOk = false;
                                      break;
                                    }
                                    if (Parser.GetFloat(Data) == true) {
                                      SetBinContent(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, Data);
                                      StreamSize--;
                                    } else {
                                      mout<<"MResponseMatrixO14: Cannot parse a float!"<<endl;
                                      StreamOk = false;
                                      break;
                                    }
                                  }
                                  
                                  if (StreamOk == false) {
                                    mout<<"MResponseMatrixO14: Stream was not ok!"<<endl;
                                    Ok = false;
                                    break;
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  } else {
    mout<<"MResponseMatrixO14: Wrong file type: "<<Type<<endl;
    Ok = false;    
  }

  return Ok;
}


////////////////////////////////////////////////////////////////////////////////


bool MResponseMatrixO14::Write(MString FileName, bool Stream)
{
  // Write the content to file

  massert(GetNBins() > 0);

  MFileResponse File;
  if (File.Open(FileName, MFile::c_Write) == false) return false;

  MTimer Timer;
  mdebug<<"Started writting file \""<<FileName<<"\" ... This way take a while ..."<<endl;

  ostringstream s;
  WriteHeader(s);
  File.Write(s);
  
  unsigned int x1, x1_max = GetAxisBins(1); 
  unsigned int x2, x2_max = GetAxisBins(2); 
  unsigned int x3, x3_max = GetAxisBins(3); 
  unsigned int x4, x4_max = GetAxisBins(4); 
  unsigned int x5, x5_max = GetAxisBins(5); 
  unsigned int x6, x6_max = GetAxisBins(6); 
  unsigned int x7, x7_max = GetAxisBins(7); 
  unsigned int x8, x8_max = GetAxisBins(8); 
  unsigned int x9, x9_max = GetAxisBins(9); 
  unsigned int x10, x10_max = GetAxisBins(10); 
  unsigned int x11, x11_max = GetAxisBins(11); 
  unsigned int x12, x12_max = GetAxisBins(12); 
  unsigned int x13, x13_max = GetAxisBins(13); 
  unsigned int x14, x14_max = GetAxisBins(14); 

  if (Stream == false) {
    s<<"Type ResponseMatrixO14"<<endl;
    for (x14 = 0; x14 < x14_max; ++x14) {
      for (x13 = 0; x13 < x13_max; ++x13) {
        for (x12 = 0; x12 < x12_max; ++x12) {
          for (x11 = 0; x11 < x11_max; ++x11) {
            for (x10 = 0; x10 < x10_max; ++x10) {
              for (x9 = 0; x9 < x9_max; ++x9) {
                for (x8 = 0; x8 < x8_max; ++x8) {
                  for (x7 = 0; x7 < x7_max; ++x7) {
                    for (x6 = 0; x6 < x6_max; ++x6) {
                      for (x5 = 0; x5 < x5_max; ++x5) {
                        for (x4 = 0; x4 < x4_max; ++x4) {
                          for (x3 = 0; x3 < x3_max; ++x3) {
                            for (x2 = 0; x2 < x2_max; ++x2) {
                              for (x1 = 0; x1 < x1_max; ++x1) {
                                s<<"R14 "
                                 <<GetAxisContent(x1, 1)<<" "<<GetAxisContent(x2, 2)<<" "
                                 <<GetAxisContent(x3, 3)<<" "<<GetAxisContent(x4, 4)<<" "
                                 <<GetAxisContent(x5, 5)<<" "<<GetAxisContent(x6, 6)<<" "
                                 <<GetAxisContent(x7, 7)<<" "<<GetAxisContent(x8, 8)<<" "
                                 <<GetAxisContent(x9, 9)<<" "<<GetAxisContent(x10, 10)<<" "
                                 <<GetAxisContent(x11, 11)<<" "<<GetAxisContent(x12, 12)<<" "
                                 <<GetAxisContent(x13, 13)<<" "<<GetAxisContent(x14, 14)<<" "
                                 <<" "<<GetBinContent(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14)<<endl;
                                File.Write(s);
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  } else {
    s<<"Type ResponseMatrixO14Stream"<<endl;
    // Write x1-axis
    s<<"A1 ";
    File.Write(s);
    for (x1 = 0; x1 <= x1_max; ++x1) {
      File.Write(GetAxisContent(x1, 1));
    }
    s<<endl;
    s<<"N1 "<<GetAxisName(1)<<endl;
    File.Write(s);
    // Write x2-axis
    s<<"A2 ";
    File.Write(s);
    for (x2 = 0; x2 <= x2_max; ++x2) {
      File.Write(GetAxisContent(x2, 2));
    }
    s<<endl;
    s<<"N2 "<<GetAxisName(2)<<endl;
    File.Write(s);
    // Write x3-axis
    s<<"A3 ";
    File.Write(s);
    for (x3 = 0; x3 <= x3_max; ++x3) {
      File.Write(GetAxisContent(x3, 3));
    }
    s<<endl;
    s<<"N3 "<<GetAxisName(3)<<endl;
    File.Write(s);
    // Write x4-axis
    s<<"A4 ";
    File.Write(s);
    for (x4 = 0; x4 <= x4_max; ++x4) {
      File.Write(GetAxisContent(x4, 4));
    }
    s<<endl;
    s<<"N4 "<<GetAxisName(4)<<endl;
    File.Write(s);
    // Write x5-axis
    s<<"A5 ";
    File.Write(s);
    for (x5 = 0; x5 <= x5_max; ++x5) {
      File.Write(GetAxisContent(x5, 5));
    }
    s<<endl;
    s<<"N5 "<<GetAxisName(5)<<endl;
    File.Write(s);
    // Write x6-axis
    s<<"A6 ";
    File.Write(s);
    for (x6 = 0; x6 <= x6_max; ++x6) {
      File.Write(GetAxisContent(x6, 6));
    }
    s<<endl;
    s<<"N6 "<<GetAxisName(6)<<endl;
    File.Write(s);
    // Write x7-axis
    s<<"A7 ";
    File.Write(s);
    for (x7 = 0; x7 <= x7_max; ++x7) {
      File.Write(GetAxisContent(x7, 7));
    }
    s<<endl;
    s<<"N7 "<<GetAxisName(7)<<endl;
    File.Write(s);
    // Write x8-axis
    s<<"A8 ";
    File.Write(s);
    for (x8 = 0; x8 <= x8_max; ++x8) {
      File.Write(GetAxisContent(x8, 8));
    }
    s<<endl;
    s<<"N8 "<<GetAxisName(8)<<endl;
    File.Write(s);
    // Write x9-axis
    s<<"A9 ";
    File.Write(s);
    for (x9 = 0; x9 <= x9_max; ++x9) {
      File.Write(GetAxisContent(x9, 9));
    }
    s<<endl;
    s<<"N9 "<<GetAxisName(9)<<endl;
    File.Write(s);
    // Write x10-axis
    s<<"A10 ";
    File.Write(s);
    for (x10 = 0; x10 <= x10_max; ++x10) {
      File.Write(GetAxisContent(x10, 10));
    }
    s<<endl;
    s<<"N10 "<<GetAxisName(10)<<endl;
    File.Write(s);
    // Write x11-axis
    s<<"A11 ";
    File.Write(s);
    for (x11 = 0; x11 <= x11_max; ++x11) {
      File.Write(GetAxisContent(x11, 11));
    }
    s<<endl;
    s<<"N11 "<<GetAxisName(11)<<endl;
    File.Write(s);
    // Write x12-axis
    s<<"A12 ";
    File.Write(s);
    for (x12 = 0; x12 <= x12_max; ++x12) {
      File.Write(GetAxisContent(x12, 12));
    }
    s<<endl;
    s<<"N12 "<<GetAxisName(12)<<endl;
    // Write x13-axis
    s<<"A13 ";
    File.Write(s);
    for (x13 = 0; x13 <= x13_max; ++x13) {
      File.Write(GetAxisContent(x13, 13));
    }
    s<<endl;
    s<<"N13 "<<GetAxisName(13)<<endl;
    // Write x14-axis
    s<<"A14 ";
    File.Write(s);
    for (x14 = 0; x14 <= x14_max; ++x14) {
      File.Write(GetAxisContent(x14, 14));
    }
    s<<endl;
    s<<"N14 "<<GetAxisName(14)<<endl;
        

    // Write content stream
    s<<"StartStream "<<GetNBins()<<endl;
    File.Write(s);
    for (x14 = 0; x14 < x14_max; ++x14) {
      for (x13 = 0; x13 < x13_max; ++x13) {
        for (x12 = 0; x12 < x12_max; ++x12) {
          for (x11 = 0; x11 < x11_max; ++x11) {
            for (x10 = 0; x10 < x10_max; ++x10) {
              for (x9 = 0; x9 < x9_max; ++x9) {
                for (x8 = 0; x8 < x8_max; ++x8) {
                  for (x7 = 0; x7 < x7_max; ++x7) {
                    for (x6 = 0; x6 < x6_max; ++x6) {
                      for (x5 = 0; x5 < x5_max; ++x5) {
                        for (x4 = 0; x4 < x4_max; ++x4) {
                          for (x3 = 0; x3 < x3_max; ++x3) {
                            for (x2 = 0; x2 < x2_max; ++x2) {
                              for (x1 = 0; x1 < x1_max; ++x1) {
                                File.Write(GetBinContent(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14));
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    s<<endl;
    s<<"StopStream"<<endl;
    File.Write(s);
  }
  
  mdebug<<"File \""<<FileName<<"\" with "
        <<x1_max*x2_max*x3_max*x4_max*x5_max*x6_max*x7_max*x8_max*x9_max*x10_max*x11_max*x12_max*x13_max*x14_max
        <<" entries written in "<<Timer.ElapsedTime()<<" sec"<<endl;
  File.Close();

  return true;
}


////////////////////////////////////////////////////////////////////////////////


void MResponseMatrixO14::Show(float x1, float x2, float x3, float x4, 
                              float x5, float x6, float x7, float x8, 
                              float x9, float x10, float x11, 
                              float x12, float x13, float x14, bool Normalize)
{
  // Create a 3d ROOT histogram:
  
  vector<unsigned int> axes;
  vector<unsigned int> values;
  vector<bool> found(14, false);

  vector<float> ShowMatrix;
  ShowMatrix.push_back(MResponseMatrix::c_ShowX);
  ShowMatrix.push_back(MResponseMatrix::c_ShowY);
  ShowMatrix.push_back(MResponseMatrix::c_ShowZ);


  for (unsigned int s = 0; s < ShowMatrix.size(); ++s) {
    if (x1 == ShowMatrix[s]) {
      axes.push_back(1);
      values.push_back(GetAxisBin(x1, 1));
      found[0] = true;
    } else if (x2 == ShowMatrix[s]) {
      axes.push_back(2);
      values.push_back(GetAxisBin(x2, 2));
      found[1] = true;
    } else if (x3 == ShowMatrix[s]) {
      axes.push_back(3);
      values.push_back(GetAxisBin(x3, 3));
      found[2] = true;
    } else if (x4 == ShowMatrix[s]) {
      axes.push_back(4);
      values.push_back(GetAxisBin(x4, 4));
      found[3] = true;
    } else if (x5 == ShowMatrix[s]) {
      axes.push_back(5);
      values.push_back(GetAxisBin(x5, 5));
      found[4] = true;
    } else if (x6 == ShowMatrix[s]) {
      axes.push_back(6);
      values.push_back(GetAxisBin(x6, 6));
      found[5] = true;
    } else if (x7 == ShowMatrix[s]) {
      axes.push_back(7);
      values.push_back(GetAxisBin(x7, 7));
      found[6] = true;
    } else if (x8 == ShowMatrix[s]) {
      axes.push_back(8);
      values.push_back(GetAxisBin(x8, 8));
      found[7] = true;
    } else if (x9 == ShowMatrix[s]) {
      axes.push_back(9);
      values.push_back(GetAxisBin(x9, 9));
      found[8] = true;
    } else if (x10 == ShowMatrix[s]) {
      axes.push_back(10);
      values.push_back(GetAxisBin(x10, 10));
      found[9] = true;
    } else if (x11 == ShowMatrix[s]) {
      axes.push_back(11);
      values.push_back(GetAxisBin(x11, 11));
      found[10] = true;
    } else if (x12 == ShowMatrix[s]) {
      axes.push_back(12);
      values.push_back(GetAxisBin(x12, 12));
      found[11] = true;
    } else if (x13 == ShowMatrix[s]) {
      axes.push_back(13);
      values.push_back(GetAxisBin(x13, 13));
      found[12] = true;
    } else if (x14 == ShowMatrix[s]) {
      axes.push_back(14);
      values.push_back(GetAxisBin(x14, 14));
      found[13] = true;
    }
    if (axes.size() == s) break;
  }

  unsigned int NAxes = axes.size();

  if (found[0] == false) {
    axes.push_back(1);
    values.push_back(GetAxisBin(x1, 1));    
  }
  if (found[1] == false) {
    axes.push_back(2);
    values.push_back(GetAxisBin(x2, 2));    
  }
  if (found[2] == false) {
    axes.push_back(3);
    values.push_back(GetAxisBin(x3, 3));    
  }
  if (found[3] == false) {
    axes.push_back(4);
    values.push_back(GetAxisBin(x4, 4));    
  }
  if (found[4] == false) {
    axes.push_back(5);
    values.push_back(GetAxisBin(x5, 5));    
  }
  if (found[5] == false) {
    axes.push_back(6);
    values.push_back(GetAxisBin(x6, 6));    
  }
  if (found[6] == false) {
    axes.push_back(7);
    values.push_back(GetAxisBin(x7, 7));    
  }
  if (found[7] == false) {
    axes.push_back(8);
    values.push_back(GetAxisBin(x8, 8));    
  }
  if (found[8] == false) {
    axes.push_back(9);
    values.push_back(GetAxisBin(x9, 9));    
  }
  if (found[9] == false) {
    axes.push_back(10);
    values.push_back(GetAxisBin(x10, 10));    
  }
  if (found[10] == false) {
    axes.push_back(11);
    values.push_back(GetAxisBin(x11, 11));    
  }
  if (found[11] == false) {
    axes.push_back(12);
    values.push_back(GetAxisBin(x12, 12));    
  }
  if (found[12] == false) {
    axes.push_back(13);
    values.push_back(GetAxisBin(x13, 13));    
  }
  if (found[13] == false) {
    axes.push_back(14);
    values.push_back(GetAxisBin(x14, 14));    
  }

  merr<<"No error checks implemented!"<<endl;
  // e.g. overflow for values...

  mout<<"Mapping: "<<endl;
  for (unsigned int i = 0; i < axes.size(); ++i) {
    mout<<i<<": a="<<axes[i]<<" - m="<<values[i]<<endl;
  }


  if (GetNBins() > 0) {

    if (NAxes == 1) {

      TH1D* Hist = 0;
      float* Bins = new float[GetAxisBins(axes[0])+1];
      for (unsigned int i = 0; i <= GetAxisBins(axes[0]); ++i) {
        Bins[i] = GetAxisContent(i, axes[0]);
      }
      Hist = new TH1D(m_Name + "_RM14", m_Name + "_RM14", GetAxisBins(axes[0]), Bins);
      Hist->SetStats(true);
      Hist->SetContour(50);
      Hist->SetXTitle(GetAxisName(axes[0]));
      delete [] Bins;
    
      mout<<"Response matrix of order 14 with "<<GetNBins()<<" entries:"<<endl;
      double Norm = 1;
      for (unsigned int i1 = 0; i1 < GetAxisBins(axes[0]); ++i1) {
        if (Normalize == true) {
          Norm = GetAxisContent(i1+1, axes[0])-GetAxisContent(i1, axes[0]);
          if (Norm != 0) {
            Norm = 1.0/Norm;
          } else {
            Norm = 1.0;
          }
        }
        Hist->SetBinContent(i1+1, GetBinContent(i1, axes[0], 
                                                values[1], axes[1], 
                                                values[2], axes[2], 
                                                values[3], axes[3], 
                                                values[4], axes[4], 
                                                values[5], axes[5], 
                                                values[6], axes[6], 
                                                values[7], axes[7], 
                                                values[8], axes[8], 
                                                values[9], axes[9], 
                                                values[10], axes[10], 
                                                values[11], axes[11], 
                                                values[12], axes[12], 
                                                values[13], axes[13])*Norm);
      }
    
      TCanvas* Canvas = new TCanvas(m_Name + "_RM14C", m_Name + "_RM14C", 0, 0, 600, 600);
      Canvas->cd();
      Hist->Draw();
      Canvas->Update();

    } else if (NAxes == 2) {
      TH2D* Hist = 0;
      float* xBins = new float[GetAxisBins(axes[0])+1];
      float* yBins = new float[GetAxisBins(axes[1])+1];
      for (unsigned int i = 0; i <= GetAxisBins(axes[0]); ++i) {
        xBins[i] = GetAxisContent(i, axes[0]);
      }
      for (unsigned int i = 0; i <= GetAxisBins(axes[1]); ++i) {
        yBins[i] = GetAxisContent(i, axes[1]);
      }
      Hist = new TH2D(m_Name+"_RM14", m_Name+"_RM14", GetAxisBins(axes[0]), xBins, GetAxisBins(axes[1]), yBins);
      Hist->SetStats(true);
      Hist->SetContour(50);
      Hist->SetXTitle(GetAxisName(axes[0]));
      Hist->SetYTitle(GetAxisName(axes[1]));
      delete [] xBins;
      delete [] yBins;
      
      mout<<"Response matrix of order 14 with "<<GetNBins()<<" entries:"<<endl;
      double Norm = 1;
      for (unsigned int i2 = 0; i2 < GetAxisBins(axes[1]); ++i2) {
        for (unsigned int i1 = 0; i1 < GetAxisBins(axes[0]); ++i1) {
          if (Normalize == true) {
            Norm = (GetAxisContent(i1+1, axes[0])-GetAxisContent(i1, axes[0]))*
              (GetAxisContent(i2+1, axes[1])-GetAxisContent(i2, axes[1]));
            if (Norm != 0) {
              Norm = 1.0/Norm;
            } else {
              Norm = 1.0;
            }
          }
          Hist->SetBinContent(i1+1, i2+1, GetBinContent(i1, axes[0], 
                                                        i2, axes[1], 
                                                        values[2], axes[2], 
                                                        values[3], axes[3], 
                                                        values[4], axes[4], 
                                                        values[5], axes[5], 
                                                        values[6], axes[6], 
                                                        values[7], axes[7], 
                                                        values[8], axes[8], 
                                                        values[9], axes[9], 
                                                        values[10], axes[10], 
                                                        values[11], axes[11], 
                                                        values[12], axes[12], 
                                                        values[13], axes[13])*Norm);
        }
      }
      
      TCanvas* Canvas = new TCanvas(m_Name+"_RM14C", m_Name+"_RM14C", 0, 0, 600, 600);
      Canvas->cd();
      Hist->Draw("colz");
      Canvas->Update();
    } else if (NAxes == 3) {
      TH3D* Hist = 0;
      float* x1Bins = new float[GetAxisBins(axes[0])+1];
      float* x2Bins = new float[GetAxisBins(axes[1])+1];
      float* x3Bins = new float[GetAxisBins(axes[2])+1];
      for (unsigned int i = 0; i <= GetAxisBins(axes[0]); ++i) {
        x1Bins[i] = GetAxisContent(i, axes[0]);
      }
      for (unsigned int i = 0; i <= GetAxisBins(axes[1]); ++i) {
        x2Bins[i] = GetAxisContent(i, axes[1]);
      }
      for (unsigned int i = 0; i <= GetAxisBins(axes[2]); ++i) {
        x3Bins[i] = GetAxisContent(i, axes[2]);
      }
      Hist = new TH3D(m_Name+"_RM14", m_Name+"_RM14", 
                      GetAxisBins(axes[0]), x1Bins, 
                      GetAxisBins(axes[1]), x2Bins, 
                      GetAxisBins(axes[2]), x3Bins);
      Hist->SetStats(true);
      Hist->SetContour(50);
      Hist->SetXTitle(GetAxisName(axes[0]));
      Hist->SetYTitle(GetAxisName(axes[1]));
      Hist->SetZTitle(GetAxisName(axes[2]));
      delete [] x1Bins;
      delete [] x2Bins;
      delete [] x3Bins;
      
      mout<<"Response matrix of order 14 with "<<GetNBins()<<" entries:"<<endl;
      double Norm = 1;
      for (unsigned int i3 = 0; i3 < GetAxisBins(axes[2]); ++i3) {
        for (unsigned int i2 = 0; i2 < GetAxisBins(axes[1]); ++i2) {
          for (unsigned int i1 = 0; i1 < GetAxisBins(axes[0]); ++i1) {
            if (Normalize == true) {
              Norm = (GetAxisContent(i1+1, axes[0])-GetAxisContent(i1, axes[0]))*
                (GetAxisContent(i2+1, axes[1])-GetAxisContent(i2, axes[1]))*
                (GetAxisContent(i3+1, axes[2])-GetAxisContent(i3, axes[2]));
              if (Norm != 0) {
                Norm = 1.0/Norm;
              } else {
                Norm = 1.0;
              }
            }            
            Hist->SetBinContent(i1+1, i2+1, i3+1, GetBinContent(i1, axes[0], 
                                                                i2, axes[1], 
                                                                i3, axes[2], 
                                                                values[3], axes[3], 
                                                                values[4], axes[4], 
                                                                values[5], axes[5], 
                                                                values[6], axes[6], 
                                                                values[7], axes[7], 
                                                                values[8], axes[8], 
                                                                values[9], axes[9], 
                                                                values[10], axes[10], 
                                                                values[11], axes[11], 
                                                                values[12], axes[12], 
                                                                values[13], axes[13])*Norm);
          }
        }
      }

      TCanvas* Canvas = new TCanvas(m_Name + "_RM14C", m_Name + "_RM14C", 0, 0, 600, 600);
      Canvas->cd();
      Hist->Draw("box");
      Canvas->Update();

    } else {
      merr<<"Wrong number of axis: "<<NAxes<<endl;
    }

  } else {
    mout<<"Empty response matrix of order 14"<<endl;
  }
}


////////////////////////////////////////////////////////////////////////////////


ostream& operator<<(ostream& os, const MResponseMatrixO14& R)
{
  if (R.GetNBins() > 0) {
    os<<"Response matrix of order 14 with "<<R.GetNBins()<<" entries:"<<endl;
    for (unsigned int i14 = 0; i14 < R.GetAxisBins(14); ++i14) {
      for (unsigned int i13 = 0; i13 < R.GetAxisBins(13); ++i13) {
        for (unsigned int i12 = 0; i12 < R.GetAxisBins(12); ++i12) {
          for (unsigned int i11 = 0; i11 < R.GetAxisBins(11); ++i11) {
            for (unsigned int i10 = 0; i10 < R.GetAxisBins(10); ++i10) {
              for (unsigned int i9 = 0; i9 < R.GetAxisBins(9); ++i9) {
                for (unsigned int i8 = 0; i8 < R.GetAxisBins(8); ++i8) {
                  for (unsigned int i7 = 0; i7 < R.GetAxisBins(7); ++i7) {
                    for (unsigned int i6 = 0; i6 < R.GetAxisBins(6); ++i6) {
                      for (unsigned int i5 = 0; i5 < R.GetAxisBins(5); ++i5) {
                        for (unsigned int i4 = 0; i4 < R.GetAxisBins(4); ++i4) {
                          for (unsigned int i3 = 0; i3 < R.GetAxisBins(3); ++i3) {
                            for (unsigned int i2 = 0; i2 < R.GetAxisBins(2); ++i2) {
                              for (unsigned int i1 = 0; i1 < R.GetAxisBins(1); ++i1) {
                                os<<"x1/x2/x3/x4/x5/x6/x7/x8/x9/x10/x11/x12/x13/x14: "
                                  <<i1<<"/"<<i2<<"/"<<i3<<"/"<<i4<<"/"<<i5<<"/"<<i6<<"/"<<i7<<"/"
                                  <<i8<<"/"<<i9<<"/"<<i10<<"/"<<i11<<"/"<<i12<<"/"<<i13<<"/"<<i14
                                  <<"  - cc: "<<R.GetAxisBinCenter(i1, 1)
                                  <<"/"<<R.GetAxisBinCenter(i2, 2)
                                  <<"/"<<R.GetAxisBinCenter(i3, 3)
                                  <<"/"<<R.GetAxisBinCenter(i4, 4)
                                  <<"/"<<R.GetAxisBinCenter(i5, 5)
                                  <<"/"<<R.GetAxisBinCenter(i6, 6)
                                  <<"/"<<R.GetAxisBinCenter(i7, 7)
                                  <<"/"<<R.GetAxisBinCenter(i8, 8)
                                  <<"/"<<R.GetAxisBinCenter(i9, 9)
                                  <<"/"<<R.GetAxisBinCenter(i10, 10)
                                  <<"/"<<R.GetAxisBinCenter(i11, 11)
                                  <<"/"<<R.GetAxisBinCenter(i12, 12)
                                  <<"/"<<R.GetAxisBinCenter(i13, 13)
                                  <<"/"<<R.GetAxisBinCenter(i14, 14)
                                  <<"  - value : "<<R.GetBinContent(i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14)<<endl;
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  } else {
    os<<"Empty response matrix of order 14"<<endl;
  }
  return os;
}


// MResponseMatrixO14.cxx: the end...
////////////////////////////////////////////////////////////////////////////////
