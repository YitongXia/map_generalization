#pragma once

#include "Point.h"

class Point;
class Dart;
class Vertex;
class Edge;
class Face;
class Volume;

/*
Below you find the basic elements that you need to build the generalised map.
The main thing you need to fill out are the links between the elements:
  * the involutions and cells on the Dart
  * the darts on the cells
One way to do this is by using pointers. eg. define a member on the dart struct like
  Struct Dart {
    // involutions:
    Dart* a0 = nullptr;
    // ...
    // cells:
    // ...

  };
Then you could create and link Darts like:

  Dart* dart_a = new Dart();
  Dart* dart_b = new Dart();
  dart_a->a0 = dart_b;
*/

class Dart {

public:
    int dart_id;
    Dart* a0 = nullptr;
    Dart* a1= nullptr;
    Dart* a2= nullptr;
    Dart* a3= nullptr;

    Vertex * vertex;
    Edge * edge;
    Face * face;

    Dart():dart_id(0)
    {}

};

class Vertex {
public:
    int vertex_id;
    Point point;
    Vertex() : point(Point())
    {}
    // constructor with x,y,z arguments to immediately initialise the point member on this Vertex.
    Vertex(const float &x, const float &y, const float &z, const int id)
    {
        point=Point(x,y,z);
        vertex_id=id;
    }
    void print() const {
        std::cout << "(" << point[0] << ", " << point[1] << ", " << point[2] << ")" << vertex_id<<std::endl; }
    // a dart incident to this Vertex:
    // ...
    Dart * incident_dart;
};

class Edge {
public:
    int edge_id;
    int edge_start;
    int edge_end;
    Edge(int a, int b,int c): edge_start(a),edge_end(b),edge_id(c)
    {}
    // a dart incident to this Edge:

    // function to compute the barycenter for this Edge (needed for triangulation output):
    // Point barycenter() {}
};


class Face {
public:
    int face_id;
    std::vector<int> vertex_id;
    std::vector<Vertex> vertex_coord;
    std::vector<Edge> edge_list;
    Dart * incident_dart;
    Point barycenter;
    Face(std::vector<int> & face, std::vector<Vertex> & vertices, int id)
    {
        face_id=id;
        for (int i = 0; i < face.size(); ++i) {
            vertex_coord.emplace_back(vertices[face[i]]);
            vertex_id.emplace_back(vertices[face[i]].vertex_id);
            if(i<face.size()-1) edge_list.emplace_back(vertices[face[i]].vertex_id,vertices[face[i+1]].vertex_id,i+1);
            else edge_list.emplace_back(vertices[face[i]].vertex_id,vertices[face[0]].vertex_id,i+1);
        }
    }
    Face():face_id(0)
    {}

    void compute_barycenter() {
        Point point1 = Point(1, 2, 3);
        barycenter = point1;
    }
    // a dart incident to this Face:
    // ...

    // function to compute the barycenter for this Face (needed for triangulation output):
    // Point barycenter() {}
};

class Volume {
public:
    int volume_id;
    // a dart incident to this Volume:
    // ...

};

class GeneralizedMap{
    static std::vector<Face> find_incident_face(const Face& current_face,const std::vector<Face>& faces)
    {
        std::vector<Face> incident_face;
        for (auto i : current_face.edge_list) {
            int start=i.edge_start;
            int end=i.edge_end;
            for (const auto & face : faces) {
                std::vector<Edge> temp=face.edge_list;
                for (auto & k : temp) {
                    if (k.edge_start==end && k.edge_end==start)
                        incident_face.emplace_back(face);
                }
            }
        }
        return incident_face;
    }

    static std::vector<Edge> find_incident_edge()
    {

    }

    static void build_darts(std::vector<Face>& faces)
    {
        for (int i = 0; i < faces.size(); ++i) {

        }
    }
};
