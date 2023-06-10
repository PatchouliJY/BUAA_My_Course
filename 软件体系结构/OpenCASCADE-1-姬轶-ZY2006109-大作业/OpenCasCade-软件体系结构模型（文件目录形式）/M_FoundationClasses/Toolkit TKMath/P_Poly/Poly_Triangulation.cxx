// Created on: 1995-03-06
// Created by: Laurent PAINNOT
// Copyright (c) 1995-1999 Matra Datavision
// Copyright (c) 1999-2014 OPEN CASCADE SAS
//
// This file is part of Open CASCADE Technology software library.
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 2.1 as published
// by the Free Software Foundation, with special exception defined in the file
// OCCT_LGPL_EXCEPTION.txt. Consult the file LICENSE_LGPL_21.txt included in OCCT
// distribution for complete text of the license and disclaimer of any warranty.
//
// Alternatively, this file may be used under the terms of Open CASCADE
// commercial license or contractual agreement.

#include <Poly_Triangulation.hxx>

#include <gp_Pnt.hxx>
#include <Poly_Triangle.hxx>
#include <Standard_DomainError.hxx>
#include <Standard_Dump.hxx>
#include <Standard_NullObject.hxx>
#include <Standard_Type.hxx>

IMPLEMENT_STANDARD_RTTIEXT (Poly_Triangulation, Standard_Transient)

//=======================================================================
//function : Poly_Triangulation
//purpose  : 
//=======================================================================
Poly_Triangulation::Poly_Triangulation()
: myCachedMinMax (NULL),
  myDeflection   (0)
{
}

//=======================================================================
//function : Poly_Triangulation
//purpose  : 
//=======================================================================
Poly_Triangulation::Poly_Triangulation(const Standard_Integer theNbNodes,
                                       const Standard_Integer theNbTriangles,
                                       const Standard_Boolean theHasUVNodes)
: myCachedMinMax (NULL),
  myDeflection   (0),
  myNodes        (1, theNbNodes),
  myTriangles    (1, theNbTriangles)
{
  if (theHasUVNodes) myUVNodes = new TColgp_HArray1OfPnt2d(1, theNbNodes);
}

//=======================================================================
//function : Poly_Triangulation
//purpose  :
//=======================================================================
Poly_Triangulation::Poly_Triangulation(const Standard_Integer theNbNodes,
                                       const Standard_Integer theNbTriangles,
                                       const Standard_Boolean theHasUVNodes,
                                       const Standard_Boolean theHasNormals)
: myDeflection(0),
  myNodes     (1, theNbNodes),
  myTriangles (1, theNbTriangles)
{
  if (theHasUVNodes)
  {
    myUVNodes = new TColgp_HArray1OfPnt2d(1, theNbNodes);
  }
  if (theHasNormals)
  {
    myNormals = new TShort_HArray1OfShortReal(1, theNbNodes * 3);
  }
}

//=======================================================================
//function : Poly_Triangulation
//purpose  :
//=======================================================================
Poly_Triangulation::Poly_Triangulation(const TColgp_Array1OfPnt&    theNodes,
                                       const Poly_Array1OfTriangle& theTriangles)
: myCachedMinMax (NULL),
  myDeflection   (0),
  myNodes        (1, theNodes.Length()),
  myTriangles    (1, theTriangles.Length())
{
  myNodes = theNodes;
  myTriangles = theTriangles;
}

//=======================================================================
//function : Poly_Triangulation
//purpose  : 
//=======================================================================

Poly_Triangulation::Poly_Triangulation(const TColgp_Array1OfPnt&    theNodes,
                                       const TColgp_Array1OfPnt2d&  theUVNodes,
                                       const Poly_Array1OfTriangle& theTriangles)
: myCachedMinMax (NULL),
  myDeflection   (0),
  myNodes        (1, theNodes.Length()),
  myTriangles    (1, theTriangles.Length())
{
  myNodes = theNodes;
  myTriangles = theTriangles;
  myUVNodes = new TColgp_HArray1OfPnt2d (1, theNodes.Length());
  myUVNodes->ChangeArray1() = theUVNodes;
}

//=======================================================================
//function : ~Poly_Triangulation
//purpose  :
//=======================================================================
Poly_Triangulation::~Poly_Triangulation()
{
  delete myCachedMinMax;
}

//=======================================================================
//function : Copy
//purpose  : 
//=======================================================================

Handle(Poly_Triangulation) Poly_Triangulation::Copy() const
{
  return new Poly_Triangulation (this);
}

//=======================================================================
//function : Poly_Triangulation
//purpose  : 
//=======================================================================

Poly_Triangulation::Poly_Triangulation (const Handle(Poly_Triangulation)& theTriangulation)
: myCachedMinMax(NULL),
  myDeflection(theTriangulation->myDeflection),
  myNodes(theTriangulation->Nodes()),
  myTriangles(theTriangulation->Triangles())
{
  SetCachedMinMax (theTriangulation->CachedMinMax());
  if (theTriangulation->HasUVNodes())
  {
    myUVNodes = new TColgp_HArray1OfPnt2d(theTriangulation->myUVNodes->Array1());
  }
  if (theTriangulation->HasNormals())
  {
    myNormals = new TShort_HArray1OfShortReal(theTriangulation->myNormals->Array1());
  }
}

//=======================================================================
//function : Deflection
//purpose  : 
//=======================================================================

void Poly_Triangulation::Deflection(const Standard_Real theDeflection)
{
  myDeflection = theDeflection;
}

//=======================================================================
//function : RemoveUVNodes
//purpose  : 
//=======================================================================

void Poly_Triangulation::RemoveUVNodes()
{
  myUVNodes.Nullify();
}

//=======================================================================
//function : Node
//purpose  : 
//=======================================================================

const gp_Pnt& Poly_Triangulation::Node (const Standard_Integer theIndex) const
{
  if (theIndex < 1 || theIndex > myNodes.Size())
  {
    throw Standard_OutOfRange ("Poly_Triangulation::Node : index out of range");
  }
  return myNodes.Value (theIndex);
}

//=======================================================================
//function : ChangeNode
//purpose  : 
//=======================================================================

gp_Pnt& Poly_Triangulation::ChangeNode (const Standard_Integer theIndex)
{
  if (theIndex < 1 || theIndex > myNodes.Size())
  {
    throw Standard_OutOfRange ("Poly_Triangulation::ChangeNode : index out of range");
  }
  return myNodes.ChangeValue (theIndex);
}

//=======================================================================
//function : UVNode
//purpose  : 
//=======================================================================

const gp_Pnt2d& Poly_Triangulation::UVNode (const Standard_Integer theIndex) const
{
  if (myUVNodes.IsNull() || theIndex < 1 || theIndex > myUVNodes->Size())
  {
    throw Standard_OutOfRange ("Poly_Triangulation::UVNode : index out of range");
  }
  return myUVNodes->Value (theIndex);
}

//=======================================================================
//function : ChangeUVNode
//purpose  : 
//=======================================================================

gp_Pnt2d& Poly_Triangulation::ChangeUVNode (const Standard_Integer theIndex)
{
  if (myUVNodes.IsNull() || theIndex < 1 || theIndex > myUVNodes->Size())
  {
    throw Standard_OutOfRange ("Poly_Triangulation::ChangeUVNode : index out of range");
  }
  return myUVNodes->ChangeValue (theIndex);
}

//=======================================================================
//function : Triangle
//purpose  : 
//=======================================================================

const Poly_Triangle& Poly_Triangulation::Triangle (const Standard_Integer theIndex) const
{
  if (theIndex < 1 || theIndex > myTriangles.Size())
  {
    throw Standard_OutOfRange ("Poly_Triangulation::Triangle : index out of range");
  }
  return myTriangles.Value (theIndex);
}

//=======================================================================
//function : ChangeTriangle
//purpose  : 
//=======================================================================

Poly_Triangle& Poly_Triangulation::ChangeTriangle (const Standard_Integer theIndex)
{
  if (theIndex < 1 || theIndex > myTriangles.Size())
  {
    throw Standard_OutOfRange ("Poly_Triangulation::ChangeTriangle : index out of range");
  }
  return myTriangles.ChangeValue (theIndex);
}

//=======================================================================
//function : SetNormals
//purpose  : 
//=======================================================================

void Poly_Triangulation::SetNormals (const Handle(TShort_HArray1OfShortReal)& theNormals)
{

  if(theNormals.IsNull() || theNormals->Length() != 3 * NbNodes()) {
    throw Standard_DomainError("Poly_Triangulation::SetNormals : wrong length");
  }

  myNormals = theNormals;
}

//=======================================================================
//function : Normals
//purpose  : 
//=======================================================================

const TShort_Array1OfShortReal& Poly_Triangulation::Normals() const
{

  if(myNormals.IsNull() || myNormals->Length() != 3 * NbNodes()) {
    throw Standard_NullObject("Poly_Triangulation::Normals : "
                              "wrong length or null array");
  }

  return myNormals->Array1();
}

//=======================================================================
//function : ChangeNormals
//purpose  : 
//=======================================================================

TShort_Array1OfShortReal& Poly_Triangulation::ChangeNormals()
{

  if(myNormals.IsNull() || myNormals->Length() != 3 * NbNodes()) {
    throw Standard_NullObject("Poly_Triangulation::ChangeNormals : "
                              "wrong length or null array");
  }

  return myNormals->ChangeArray1();
}

//=======================================================================
//function : HasNormals
//purpose  : 
//=======================================================================

Standard_Boolean Poly_Triangulation::HasNormals() const
{
  if(myNormals.IsNull() || myNormals->Length() != 3 * NbNodes()) {
    return Standard_False;
  }
  return Standard_True;
}

//=======================================================================
//function : SetNormal
//purpose  : 
//=======================================================================

void Poly_Triangulation::SetNormal (const Standard_Integer theIndex, const gp_Dir& theNormal)
{
  if (myNormals.IsNull() || theIndex < 1 || theIndex > myNodes.Size())
  {
    throw Standard_NullObject ("Poly_Triangulation::SetNormal : empty array or index out of range");
  }

  myNormals->ChangeValue (theIndex * 3 - 2) = (Standard_ShortReal) theNormal.X();
  myNormals->ChangeValue (theIndex * 3 - 1) = (Standard_ShortReal) theNormal.Y();
  myNormals->ChangeValue (theIndex * 3)     = (Standard_ShortReal) theNormal.Z();
}

//=======================================================================
//function : Normal
//purpose  : 
//=======================================================================

gp_Dir Poly_Triangulation::Normal (const Standard_Integer theIndex) const
{
  if (myNormals.IsNull() || theIndex < 1 || theIndex > myNodes.Size())
  {
    throw Standard_NullObject ("Poly_Triangulation::Normal : empty array or index out of range");
  }

  gp_Dir N(myNormals->Value(theIndex * 3 - 2),
           myNormals->Value(theIndex * 3 - 1),
           myNormals->Value(theIndex * 3));

  return N;
}

// =======================================================================
// function : DumpJson
// purpose  :
// =======================================================================
void Poly_Triangulation::DumpJson (Standard_OStream& theOStream, Standard_Integer) const
{
  OCCT_DUMP_TRANSIENT_CLASS_BEGIN (theOStream)

  OCCT_DUMP_FIELD_VALUE_NUMERICAL (theOStream, myDeflection)

  OCCT_DUMP_FIELD_VALUE_NUMERICAL (theOStream, myNodes.Size())
  if (!myUVNodes.IsNull())
    OCCT_DUMP_FIELD_VALUE_NUMERICAL (theOStream, myUVNodes->Size())
  if (!myNormals.IsNull())
    OCCT_DUMP_FIELD_VALUE_NUMERICAL (theOStream, myNormals->Size())
  OCCT_DUMP_FIELD_VALUE_NUMERICAL (theOStream, myTriangles.Size())
}

// =======================================================================
// function : CachedMinMax
// purpose  :
// =======================================================================
const Bnd_Box& Poly_Triangulation::CachedMinMax() const
{
  static const Bnd_Box anEmptyBox;
  return (myCachedMinMax == NULL) ? anEmptyBox : *myCachedMinMax;
}

// =======================================================================
// function : SetCachedMinMax
// purpose  :
// =======================================================================
void Poly_Triangulation::SetCachedMinMax (const Bnd_Box& theBox)
{
  if (theBox.IsVoid())
  {
    unsetCachedMinMax();
    return;
  }
  if (myCachedMinMax == NULL)
  {
    myCachedMinMax = new Bnd_Box();
  }
  *myCachedMinMax = theBox;
}

// =======================================================================
// function : unsetCachedMinMax
// purpose  :
// =======================================================================
void Poly_Triangulation::unsetCachedMinMax()
{
  if (myCachedMinMax != NULL)
  {
    delete myCachedMinMax;
    myCachedMinMax = NULL;
  }
}

// =======================================================================
// function : MinMax
// purpose  :
// =======================================================================
Standard_Boolean Poly_Triangulation::MinMax (Bnd_Box& theBox, const gp_Trsf& theTrsf, const bool theIsAccurate) const
{
  Bnd_Box aBox;
  if (HasCachedMinMax() &&
      (!HasGeometry() || !theIsAccurate ||
       theTrsf.Form() == gp_Identity || theTrsf.Form() == gp_Translation ||
       theTrsf.Form() == gp_PntMirror || theTrsf.Form() == gp_Scale))
  {
    aBox = myCachedMinMax->Transformed (theTrsf);
  }
  else
  {
    aBox = computeBoundingBox (theTrsf);
  }
  if (aBox.IsVoid())
  {
    return Standard_False;
  }
  theBox.Add (aBox);
  return Standard_True;
}

// =======================================================================
// function : computeBoundingBox
// purpose  :
// =======================================================================
Bnd_Box Poly_Triangulation::computeBoundingBox (const gp_Trsf& theTrsf) const
{
  Bnd_Box aBox;
  if (theTrsf.Form() == gp_Identity)
  {
    for (Standard_Integer aNodeIdx = 1; aNodeIdx <= NbNodes(); aNodeIdx++)
    {
      aBox.Add (myNodes[aNodeIdx]);
    }
  }
  else
  {
    for (Standard_Integer aNodeIdx = 1; aNodeIdx <= NbNodes(); aNodeIdx++)
    {
      aBox.Add (myNodes[aNodeIdx].Transformed (theTrsf));
    }
  }
  return aBox;
}
