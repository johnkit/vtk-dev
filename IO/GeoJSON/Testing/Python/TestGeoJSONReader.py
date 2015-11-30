from __future__ import print_function

import sys
import vtk
from vtk.test import Testing

def load_geojson(input_string, feature_properties={}):
  '''Parses input_string with vtkGeoJSONReader, returns vtkPolyData

  feature_properties is a dictionary of name-default_values
  to attach as cell data in the returned vtkPolyData.
  '''
  reader = vtk.vtkGeoJSONReader()
  reader.SetInputModeToString()
  reader.SetStringInput(input_string)
  for name,default_value in feature_properties.items():
    reader.AddFeatureProperty(name, default_value)
  reader.Update()
  return reader.GetOutput()


if __name__ == '__main__'  :
  # Use feature collection example taken from the geojson spec.
  # Coped from http://geojson.org/geojson-spec.html (October 2014).
  # Features are in/near the island of Summatra (in western Indonesia).
  input_string = \
"""
{ "type": "FeatureCollection",
  "features": [
      { "type": "Feature",
        "geometry": {"type": "Point", "coordinates": [102.0, 0.5]},
        "properties": {"prop0": "value0"}
      },
      { "type": "Feature",
        "geometry": {
            "type": "LineString",
            "coordinates": [
                [102.0, 0.0], [103.0, 1.0], [104.0, 0.0], [105.0, 1.0]
            ]
        },
        "properties": {
            "prop0": "value0",
            "prop1": 0.0
        }
      },
      { "type": "Feature",
        "geometry": {
            "type": "Polygon",
            "coordinates": [
                [ [100.0, 0.0], [101.0, 0.0], [101.0, 1.0],
                  [100.0, 1.0], [100.0, 0.0] ]
            ]
        },
        "properties": {
            "prop0": "value0",
            "prop1": {"this": "that"}
        }
      }
  ]
}
"""
  prop0_default = vtk.vtkVariant('default')
  feature_properties = {'prop0': prop0_default}
  polydata = load_geojson(input_string, feature_properties)
  if polydata is None:
    print('Failed to read input string and return vtkPolyData')
    sys.exit(1)

  num_errors = 0

  # Check cell counts
  expected_verts = 1
  expected_lines = 3
  expected_polys = 1

  num_verts = polydata.GetNumberOfVerts()
  if num_verts != expected_verts:
    print('Wrong number of verts: returned %s, should be %s' % \
      (num_verts, expected_verts))
    num_errors += 1

  num_lines = polydata.GetNumberOfLines()
  if num_lines != expected_lines:
    print('Wrong number of lines: returned %s, should be %s' % \
      (num_lines, expected_lines))
    num_errors += 1

  num_polys = polydata.GetNumberOfPolys()
  if num_polys != expected_polys:
    print('Wrong number of polys: returned %s, should be %s' % \
      (num_polys, expected_polys))
    num_errors += 1

  # Check cell data
  cell_data = polydata.GetCellData()

  # All polydata generated from GeoJSON have feature-id array
  feature_id_array = cell_data.GetAbstractArray('feature-id')
  if feature_id_array is None:
    print('feature-id array missing')
    num_errors += 1

  # Test case also specified a prop0 array
  prop0_array = cell_data.GetAbstractArray('prop0')
  if prop0_array is None:
    print('prop0 array missing')
    num_errors += 1

  print('num_errors:', num_errors)
  sys.exit(num_errors)
