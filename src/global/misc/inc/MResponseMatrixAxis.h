/*
 * MResponseMatrixAxis.h
 *
 * Copyright (C) by Andreas Zoglauer.
 * All rights reserved.
 *
 * Please see the source-file for the copyright-notice.
 *
 */


#ifndef __MResponseMatrixAxis__
#define __MResponseMatrixAxis__


////////////////////////////////////////////////////////////////////////////////


// Standard libs:
#include <vector>
using namespace std;

// ROOT libs:

// MEGAlib libs:
#include "MGlobal.h"

// Forward declarations:


////////////////////////////////////////////////////////////////////////////////


//! This defines a response matrix axis
class MResponseMatrixAxis
{
  // public interface:
 public:
  //! Default constructor
  MResponseMatrixAxis(const MString& Name = "");
  //! Default destuctor 
  virtual ~MResponseMatrixAxis();

  // TODO: To copy, assign, move, etc. allowed
  
  
  // BUG: The following two operators are problematic, since there will be cases with derived classes where we will not be abel to identify equality
  //! Equality operator
  bool operator==(const MResponseMatrixAxis& Axis) const;
  //! Inequality operator
  bool operator!=(const MResponseMatrixAxis& Axis) const { return !(operator==(Axis)); }
  
  
  //! Clone this axis
  //! The recipient is responsible for deletion 
  virtual MResponseMatrixAxis* Clone() const;
  
  //! Set the bin edges
  void SetBinEdges(vector<double> BinEdges);
  //! Set the axis in linear mode
  void SetLinear(unsigned long NBins, double Min, double Max, double UnderFlowMin = g_DoubleNotDefined, double OverFlowMax = g_DoubleNotDefined);
  //! Set the axis in logarithmic mode
  void SetLogarithmic(unsigned long NBins, double Min, double Max, double UnderFlowMin = g_DoubleNotDefined, double OverFlowMax = g_DoubleNotDefined);
  
  //! The dimensions of the axis
  unsigned int GetDimension() const { return m_Dimension; }
  
  //! True if the axis is logarithmic
  bool IsLogarithmic() const { return m_IsLogarithmic; }
  
  //! Return the number of axis bins
  virtual unsigned long GetNumberOfBins() const { return m_NumberOfBins; }
  //! Return the axis bin given a axis value
  //! Can throw: MExceptionArbitrary, MExceptionValueOutOfBounds, MExceptionIndexOutOfBounds
  //! To avoid any exceptions, call InRange() first!
  virtual unsigned long GetAxisBin(double X1, double X2 = g_DoubleNotDefined) const;
  
  //! Return the area of the given axis bin
  //! Can throw: MExceptionIndexOutOfBounds
  virtual double GetArea(unsigned long Bin) const;
  
  //! Test if the x-value is within the range of the axis - the second value in always ignored
  virtual bool InRange(double X1, double X2 = g_DoubleNotDefined) const;
  
  //! True if the axis has 1D bin edges
  virtual bool Has1DBinEdges() const { return true; }
  //! Get the 1D bin edges
  //! Check with Has1DBinEdges first, because this is not guaranteed
  virtual vector<double> Get1DBinEdges() const { return m_BinEdges; }
  
  //! Return the axis bin edges for drawing --- those might be narrower than the real bins
  virtual vector<vector<double>> GetDrawingAxisBinEdges() const;
  //! Return the axis names
  virtual vector<MString> GetNames() const { return m_Names; }
  //! Return the minimum axis values
  virtual vector<double> GetMinima() const;
  //! Return the minimum axis values
  virtual vector<double> GetMaxima() const;
  
  //! Return the bin center(s) of the given axis bin
  //! Can throw: MExceptionIndexOutOfBounds
  virtual vector<double> GetBinCenters(unsigned long Bin) const;
  
  
  //! Write the content to a stream
  virtual void Write(ostringstream& out);
  
  // protected methods:
 protected:

  // private methods:
 private:

  // protected members:
 protected:
  //! The dimension of the axis
  unsigned int m_Dimension;
  //! The name of the axes
  vector<MString> m_Names;
  //! The bin edges
  vector<double> m_BinEdges;
  //! The number of bins
  unsigned long m_NumberOfBins;
  //! Flag if the axis is logarithmic
  bool m_IsLogarithmic;
  
  // private members:
private:
  

#ifdef ___CLING___
 public:
  ClassDef(MResponseMatrixAxis, 0) // no description
#endif

};

#endif


////////////////////////////////////////////////////////////////////////////////
