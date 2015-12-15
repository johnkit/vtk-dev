/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkGeoJSONReader.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkGeoJSONReader - Convert Geo JSON format to vtkPolyData
// .SECTION Description
// Outputs a vtkPolyData from the input
// Geo JSON Data (http://www.geojson.org)

#ifndef __vtkGeoJSONReader_h
#define __vtkGeoJSONReader_h

// VTK Includes
#include "vtkIOGeoJSONModule.h" // For export macro
#include "vtkPolyDataAlgorithm.h"

class vtkPolyData;
namespace Json
{
  class Value;
}

class VTKIOGEOJSON_EXPORT vtkGeoJSONReader: public vtkPolyDataAlgorithm
{
public:
  static vtkGeoJSONReader* New();
  vtkTypeMacro(vtkGeoJSONReader,vtkPolyDataAlgorithm);
  virtual void PrintSelf(ostream &os, vtkIndent indent);

  // Decription:
  // File used as data input when Input Mode is INPUT_FILE
  vtkSetStringMacro(FileName);
  vtkGetStringMacro(FileName);

  // Description:
  // String used as data input when InputMode is INPUT_STRING
  vtkSetStringMacro(StringInput);
  vtkGetStringMacro(StringInput);

  // Description:
  // Json::Value used as data input when InputMode is INPUT_JSON
  void SetJsonInput(Json::Value *value);

  // Description:
  // Set/get the Input Mode that will be used by when the Update() method is called.
  // The only valid enums are vtkGeoJSONReader::INPUT_FILE, INPUT_STRING, INPUT_JSON.
  // The default is INPUT_FILE.
  vtkSetMacro(InputMode, int);
  vtkGetMacro(InputMode, int);
  void SetInputModeToFile()
    {this->SetInputMode(vtkGeoJSONReader::INPUT_FILE);};
  void SetInputModeToString()
    {this->SetInputMode(vtkGeoJSONReader::INPUT_STRING);};
  void SetInputModeToJson()
    {this->SetInputMode(vtkGeoJSONReader::INPUT_JSON);};
  void StringInputModeOn()  // legacy
    {
    vtkWarningMacro("This method is DEPRECATED; use SetInputModeToString() instead");
    this->SetInputModeToString();
    }

  // Description
  // Set/get whether to convert all output polygons to triangles.
  // Note that if OutinePolygons mode is on, then no output polygons
  // are generated, and in that case, this option is not relevant.
  // The default is off.
  vtkSetMacro(TriangulatePolygons, bool);
  vtkGetMacro(TriangulatePolygons, bool);
  vtkBooleanMacro(TriangulatePolygons, bool);

  // Description
  // Set/get option to generate the border outlining each polygon,
  // so that the output cells for polygons are vtkPolyLine instances.
  // The default is off.
  vtkSetMacro(OutlinePolygons, bool);
  vtkGetMacro(OutlinePolygons, bool);
  vtkBooleanMacro(OutlinePolygons, bool);

  // Description:
  // Set/get name of data array for serialized GeoJSON "properties" node.
  // If specified, data will be stored as vtkCellData/vtkStringArray.
  vtkSetStringMacro(SerializedPropertiesArrayName);
  vtkGetStringMacro(SerializedPropertiesArrayName);

  // Description:
  // Specify feature property to read in with geometry objects
  // Note that defaultValue specifies both type & value
  void AddFeatureProperty(const char *name, vtkVariant& typeAndDefaultValue);

  enum
  {
    INPUT_FILE = 0,
    INPUT_STRING,
    INPUT_JSON
  };

protected:
  vtkGeoJSONReader();
  virtual ~vtkGeoJSONReader();

  // Decription:
  // Core implementation of the
  int RequestData(vtkInformation* request, vtkInformationVector** inputVector,
                  vtkInformationVector* outputVector);
  char *FileName;
  char *StringInput;
  Json::Value *JsonInput;
  int  InputMode;
  bool TriangulatePolygons;
  bool OutlinePolygons;
  char *SerializedPropertiesArrayName;

private:
  class GeoJSONReaderInternal;
  GeoJSONReaderInternal *Internal;

  vtkGeoJSONReader(const vtkGeoJSONReader&);  // Not implemented
  void operator=(const vtkGeoJSONReader&);    // Not implemented
};

#endif // __vtkGeoJSONReader_h
