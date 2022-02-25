#include "Point.h"

class Point;
class Dart;
class Vertex;
class Edge;
class Face;
class Volume;


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
    bool operator==(const Edge& other) const
    {
        return edge_start == other.edge_end && edge_end == other.edge_start;
    }
    bool operator !=(const Edge& other) const
    {
        return edge_start != other.edge_end && edge_end != other.edge_start;
    }
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
    Face(std::vector<int> & face, std::vector<Edge> & single_edge_list ,std::vector<Vertex> & vertices,int id)
    {
        face_id=id;
        for (int i = 0; i < face.size(); i++) {
            vertex_coord.emplace_back(vertices[face[i]-1]);
            vertex_id.emplace_back(face[i]);
            if(i<face.size()-1) {
                for (auto edge:single_edge_list) {
                    if((face[i]==edge.edge_start&&face[i+1]==edge.edge_end)||(face[i]==edge.edge_end && face[i+1]==edge.edge_start))
                        edge_list.emplace_back(edge);
                }
            }
            else if(i == face.size()-1) {
                for (auto edge:single_edge_list) {
                    if((face[i]==edge.edge_start&&face[0]==edge.edge_end)||(face[i]==edge.edge_end && face[0]==edge.edge_start))
                        edge_list.emplace_back(edge);
                }
            }

        }
        compute_barycenter();
    }
    Face():face_id(0)
    {}

    void compute_barycenter() {
        float x = 0, y = 0, z = 0;
        for(auto vertex :vertex_coord) {
            x += vertex.point.x;
            y+=vertex.point.y;
            z+=vertex.point.z;
        }
        barycenter = Point(x/3,y/3,z/3);
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

    Dart(int i):dart_id(i)
    {}

    Dart(int id, Vertex & current_vertex, Edge & current_edge, Face & current_face)
    {
        dart_id=id;
        vertex = & current_vertex;
        edge= & current_edge;
        face = & current_face;
    }

};

class GeneralizedMap{
public:
    static void initialize_incident_dart(const std::vector<std::vector<Dart *>> & darts_list,std::vector<Face> & faces, std::vector<Edge> & edges, std::vector<Vertex> & vertices )
    {
        for(auto face : faces) {
            for (auto darts:darts_list) {

            }
        }
    }

    static std::vector<Edge> create_edges(std::vector<int> &face)
    {
        std::vector<Edge> edges;
        for (int i = 0; i < face.size(); i++) {
            if(i<face.size()-1) edges.emplace_back(face[i],face[i+1],i);
                // in this case when i==3
            else if (i == face.size()-1) edges.emplace_back(face[i],face[0],i);
        }
        return edges;
    }

    static std::vector<Edge> build_single_edges(const std::vector<std::vector<Edge>>& edge_list) {

        std::vector<Edge> result_edge;
        for (const auto & i : edge_list) {
            for (auto j : i) {
                if (result_edge.empty()) result_edge.emplace_back(j);
                else
                {
                    int edge_size=result_edge.size();
                    bool same= true;
                    for (int k = 0; k < edge_size; ++k) {
                        if (result_edge[k].edge_start==j.edge_end && result_edge[k].edge_end==j.edge_start){
                            same= false;
                            continue;
                        }
                    }
                    if(same) result_edge.emplace_back(j);
                }
            }
        }
        for (int i = 0; i < result_edge.size(); ++i) {result_edge[i].edge_id=i;}
        return result_edge;
    }


    static Dart * find_a2_dart(Dart * current_dart,std::vector<std::vector<Dart *>> darts_list)
    {
        Dart * incident;
        for (int i = 0; i < darts_list.size(); ++i) {
            if (i==current_dart->face->face_id) continue;
            else {
                for(auto dart:darts_list[i]) {
                    if(dart->vertex->vertex_id == current_dart->vertex->vertex_id && dart->edge->edge_id == current_dart->edge->edge_id)
                    {   incident = dart;}
                }
            }
        }
        return incident;
    }

    // second way to store darts. vectors for every face stored in a single vector.
    // return a std::vector<std::vector<Dart>>.
    static std::vector<std::vector<Dart *>> build_darts(std::vector<Face>& faces, std::vector<Vertex> & vertices)
    {
        std::vector<std::vector<Dart *>> darts_list;
        // the number of darts in each face is vertices.size()*2 ?
        int count=0;
        for (auto & face : faces) {
            std::vector<Dart *> darts;
            for (int i = 0; i < face.vertex_id.size(); ++i) {

                if (i == 0) {
                    Dart * dart_1 = new Dart(count, face.vertex_coord[i], face.edge_list[(face.edge_list.size())-1], face);
                    Dart * dart_2 = new Dart(count+1, face.vertex_coord[i],face.edge_list[i], face);
                    darts.emplace_back(dart_1);
                    darts.emplace_back(dart_2);
                    vertices[i].incident_dart=dart_1;
                }
                else {
                    Dart * dart_1 = new Dart(count, face.vertex_coord[i], face.edge_list[i-1], face);
                    Dart * dart_2 = new Dart(count+1, face.vertex_coord[i],face.edge_list[i], face);
                    darts.emplace_back(dart_1);
                    darts.emplace_back(dart_2);
                    vertices[i].incident_dart=dart_1;
                }
                count+=2;
            }
            face.incident_dart=vertices[face.vertex_id[0]].incident_dart;
            darts_list.emplace_back(darts);}
        return darts_list;
    }

    static std::vector<std::vector<Dart *>> involutions(std::vector<std::vector<Dart *>> & darts_list, std::vector<Face> & faces) {
        for (auto &darts: darts_list) {
            for (int i = 0; i < darts.size(); ++i) {
                // in a single face (because a0 and a1 are in the same face
                for (int j = 0; j < darts.size(); ++j) {
                    // a0
                    if (darts[j]->edge == darts[i]->edge && darts[j]->dart_id != darts[i]->dart_id)
                        darts[i]->a0 = darts[j];
                    // a1
                    if (darts[j]->vertex == darts[i]->vertex && darts[j]->edge != darts[i]->edge)
                        darts[i]->a1 = darts[j];
                }
            }
        }
        for (const auto &darts: darts_list) {

            for (auto dart: darts) {
                dart->a2 = find_a2_dart(dart, darts_list);
            }
        }
        return darts_list;
    }
};

class Triangulation{
public:
    static void dart_triangulation(const std::vector<std::vector<Dart *>>& darts_list) {

    }
};

class WriteOBJ {
public:
    static void output_dart(const std::vector<std::vector<Dart *>>& darts_list, const std::string& file_path) {
        std::string filename=file_path+"/involution.csv";
        std::ofstream output_file;
        output_file.open(filename, std::ios::out | std::ios::trunc);
        output_file << std::fixed;
        output_file <<"ID;a0;a1;a2;a3;v;e;f" << std::endl;
        // print data into the file
        for(const auto& darts:darts_list)
            for (auto d : darts) {
                output_file << d->dart_id<<";"<< d->a0->dart_id<<";"<< d->a1->dart_id<<";"<< d->a2->dart_id<<";" << "-"<<";"<< d->vertex->vertex_id<<";"<< d->edge->edge_id<<";"<< d->face->face_id<<std::endl;
            }
        // close file
        output_file.close();
    }
    static void output_vertex(const std::vector<Vertex>& vertices, const std::string& file_path) {
        std::ofstream output_file;
        std::string filename=file_path+"/vertices.csv";
        output_file.open(filename, std::ios::out | std::ios::trunc);
        output_file << std::fixed;
        output_file <<"ID;dart;x;y;z" << std::endl;
        // print data into the file
        for(const auto& ver:vertices)
            output_file << ver.vertex_id<<";"<< ver.incident_dart->dart_id<<";"<< ver.point.x<<";"<< ver.point.y<<";" << ver.point.z<<std::endl;
        // close file
        output_file.close();
    }
    static void output_edge(const std::vector<Vertex>& vertices, const std::string& file_path) {
        std::ofstream output_file;
        std::string filename=file_path+"/edges.csv";
        output_file.open(filename, std::ios::out | std::ios::trunc);
        output_file << std::fixed;
        output_file <<"ID;dart;x;y;z" << std::endl;
        // print data into the file
        for(const auto& ver:vertices)
            output_file << ver.vertex_id<<";"<< ver.incident_dart->dart_id<<";"<< ver.point.x<<";"<< ver.point.y<<";" << ver.point.z<<std::endl;
        // close file
        output_file.close();
    }

    static void output_face(const std::vector<Vertex>& vertices, const std::string& file_path) {
        std::ofstream output_file;
        std::string filename=file_path+"/faces.csv";
        output_file.open(filename, std::ios::out | std::ios::trunc);
        output_file << std::fixed;
        output_file <<"ID;dart;x;y;z" << std::endl;
        // print data into the file
        for(const auto& ver:vertices)
            output_file << ver.vertex_id<<";"<< ver.incident_dart->dart_id<<";"<< ver.point.x<<";"<< ver.point.y<<";" << ver.point.z<<std::endl;
        // close file
        output_file.close();
    }
};
