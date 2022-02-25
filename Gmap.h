#include "Point.h"

class Point;
class Dart;
class Vertex;
class Edge;
class Face;
class Triangulation;
class GeneralizedMap;
class Volume;


class Vertex {
public:
    int vertex_id;
    Point point;
    float x,y,z;
    Vertex() : point(Point())
    {}
    // constructor with x,y,z arguments to immediately initialise the point member on this Vertex.
    Vertex(const float &x1, const float &y1, const float &z1, const int id)
    {
        point=Point(x1,y1,z1);
        vertex_id=id;
        x=x1;
        y=y1;
        z=z1;
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
    int incident_dart;
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
    Vertex barycenter;
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
        barycenter = Vertex(x/vertex_coord.size(),y/vertex_coord.size(),z/vertex_coord.size(),0);
    }
};

class Volume {
public:
    int volume_id;
    Dart * incident_dart;
    // a dart incident to this Volume:
    // ...
    Volume(int id):volume_id(id)
    {}
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

    // the function is to build the unordered map using ordered map.
    static std::vector<Edge> build_single_edges(const std::vector<std::vector<Edge>>& edge_list) 
    {
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

    // the function is to find a2 dart for every input dart
    static Dart * find_a2_dart(Dart * current_dart,std::vector<std::vector<Dart *>> darts_list)
    {
        Dart * incident;
        for (int i = 0; i < darts_list.size(); ++i) {
            if (i==current_dart->face->face_id) continue;
            else {
                for(auto dart:darts_list[i]) {
                    if(dart->vertex->vertex_id == current_dart->vertex->vertex_id && dart->edge->edge_id == current_dart->edge->edge_id)
                    {   incident = dart; }
                }
            }
        }
        return incident;
    }

    // the function is for find incident dart for each edge
    static void edge_incident_dart(const std::vector<std::vector<Dart *>> &darts_list,std::vector<Edge> & edges) {
        for(auto edge:edges) {
            for(const auto& darts:darts_list){
                for(auto dart:darts){
                    if (edge.edge_id==dart->edge->edge_id)
                    {
                        edge.incident_dart=dart->dart_id;
                        break;
                    }
                }
                break;
            }
        }
    }

    // second way to store darts. vectors for every face stored in a single vector.
    // also find incident dart for vertices and faces.
    // return a std::vector<std::vector<Dart>>.
    static std::vector<std::vector<Dart *>> build_darts(std::vector<Face>& faces, std::vector<Edge> & edges, std::vector<Vertex> & vertices)
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
            darts_list.emplace_back(darts);
        }
        edge_incident_dart(darts_list,edges);
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


// the class Triangulation is for output triangulated darts file.
class Triangulation{
public:
    int p0;
    int p1;
    int p2;
    Triangulation(int p_0,int p_1,int p_2):p0(p_0),p1(p_1),p2(p_2)
    {}
    static void dart_triangulation(const std::vector<std::vector<Dart *>>& darts_list, const std::vector<Face> & faces, const std::vector<Vertex> vertices,std::vector<Vertex> & vertex_list,std::vector<Triangulation> & face_list) {
        int count=0;
        Vertex d2,start,end,d1,d0;
        for (int i = 0; i < darts_list.size(); ++i)
        {
            d2=faces[i].barycenter;
            d2.vertex_id=count;
            vertex_list.emplace_back(d2);
            count++;
            for (int j = 0; j < darts_list[i].size(); ++j)
            {
                start = vertices[darts_list[i][j]->edge->edge_start];
                end = vertices[darts_list[i][j]->edge->edge_end];
                d1 = Vertex((start.x + start.x) / 2, (start.y + end.y) / 2, (start.z + end.z) / 2, count);

                d0 = * darts_list[i][j]->vertex;
                bool bool0,bool1 = false;

                for(auto vertex:vertex_list)
                {
                    if(vertex.vertex_id == d0.vertex_id) {
                        bool0 = true;
                        d0=vertex;
                    }
                    if(vertex.vertex_id == d1.vertex_id) {
                        bool1= true;
                        d1=vertex;
                    }
                }
                if (!bool0) {
                    d0.vertex_id=count;
                    vertex_list.emplace_back(d0);
                    count++;
                }
                if(! bool1) {
                    d1.vertex_id=count;
                    vertex_list.emplace_back(d1);
                    count++;
                }
                if(darts_list[i][j]->dart_id % 2 ==0){
                    Triangulation tri=Triangulation(d2.vertex_id,d2.vertex_id,d0.vertex_id);
                    face_list.emplace_back(tri);
                }
                else if(darts_list[i][j]->dart_id%2 ==1 ){
                    Triangulation tri=Triangulation(d2.vertex_id,d0.vertex_id,d1.vertex_id);
                    face_list.emplace_back(tri);
                }
            }
        }
    }
    static void output_obj(const std::vector<Vertex> & vertex_list,const std::vector<Triangulation> & face_list, const std::string& file_path) {
        std::string filename=file_path+"/result_cube.obj";
        std::ofstream output_file;
        output_file.open(filename, std::ios::out | std::ios::trunc);
        output_file << std::fixed;
        // print data into the file
        for(const auto& vertex:vertex_list)
            output_file << "v "<<vertex.point.x<<";"<<" "<<";"<< vertex.point.y<<" " << vertex.point.z <<std::endl;
        for(const auto face:face_list)
            output_file<<"f " <<face.p0<<" "<<face.p1<<" "<<face.p2<<std::endl;
        output_file.close();
    }
};

// the class to write the output csv file.
class WriteCSV {
public:
    //the function to write dart.csv
    // the output format: ID;a0;a1;a2;a3;v;e;f
    static void output_dart(const std::vector<std::vector<Dart *>>& darts_list, const std::string& file_path) {
        std::string filename=file_path+"/dart.csv";
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

    //the function to write vertices.csv
    // the output format: ID;dart;x;y;z.
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

    //the function to write edges.csv
    // the output format: ID;darts;start;end.
    static void output_edge(const std::vector<Edge>& edges, const std::string& file_path) {
        std::ofstream output_file;
        std::string filename=file_path+"/edges.csv";
        output_file.open(filename, std::ios::out | std::ios::trunc);
        output_file << std::fixed;
        output_file <<"ID;dart;start;end" << std::endl;
        // print data into the file
        for(const auto& edge:edges)
            output_file <<edge.edge_id<<";"<<edge.incident_dart<<";"<< edge.edge_start<<";" << edge.edge_end<<std::endl;
        // close file
        output_file.close();
    }

    // the function to write volume.csv
    // the output format: ID; dart
    static void output_face(const std::vector<Volume>& volumes, const std::string& file_path) {
        std::ofstream output_file;
        std::string filename=file_path+"/volumes.csv";
        output_file.open(filename, std::ios::out | std::ios::trunc);
        output_file << std::fixed;
        output_file <<"ID;dart" << std::endl;
        // print data into the file
        for(const auto& volume:volumes)
            output_file <<volume.volume_id<<";" <<volume.incident_dart->dart_id<<std::endl;
        // close file
        output_file.close();
    }
};
