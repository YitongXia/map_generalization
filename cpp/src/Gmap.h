#include "Point.h"

class Point;
class Dart;
class Vertex;
class Edge;
class Face;
class Triangulation;
class GeneralizedMap;
class Volume;

// class vertex is used to store vertex information
class Vertex {
public:
    int vertex_id; // the id of the vertex is the same as the order in the obj file.
    Point point;  // coordinate information
    float x,y,z;
    Dart * incident_dart= nullptr; // pointer to incident dart.

    // constructor without arguments
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

    Vertex(const float &x1, const float &y1, const float &z1):x(x1),y(y1),z(z1)
    {}

    // test function, to output vertex coordinates.
    void print() const {
        std::cout << "(" << point[0] << ", " << point[1] << ", " << point[2] << ")" << vertex_id<<std::endl;
    }

    // overload operators.
    const Vertex operator+(const Vertex &other) const {
        return Vertex(x+other.x, y+other.y, z+other.z);
    }
    const Vertex operator/(const float &other) const {
        return Vertex(x/other, y/other, z/other);
    }
    bool operator==(const Vertex & other) const {
        return x == other.x && y == other.y && z==other.z;
    }
};

class Edge {
public:
    int edge_id;
    int edge_start;
    int edge_end;
    Dart * incident_dart= nullptr;

    // constructor with arguments.
    Edge(int a, int b,int c): edge_start(a),edge_end(b),edge_id(c)
    {}
    // constructor without arguments
    Edge()
    {}
    bool operator==(const Edge& other) const
    {
        return edge_start == other.edge_end && edge_end == other.edge_start;
    }
    bool operator !=(const Edge& other) const
    {
        return edge_start != other.edge_end && edge_end != other.edge_start;
    }
};


class Face {
public:
    int face_id; // the face id is the same as the face order in the obj file.
    std::vector<int> vertex_id;
    std::vector<Vertex> vertex_coord;
    std::vector<Edge> edge_list;
    Dart * incident_dart= nullptr;
    Vertex barycenter;

    // constructor with face, edge and vertex information.
    Face(std::vector<int> & face, std::vector<Edge> & single_edge_list ,std::vector<Vertex> & vertices,int id)
    {
        face_id=id;
        for (int i = 0; i < face.size(); i++)
        {
            vertex_coord.emplace_back(vertices[face[i]-1]);
            vertex_id.emplace_back(face[i]);
            if(i<face.size()-1)
            {
                for (auto edge:single_edge_list) {
                    if((face[i]==edge.edge_start&&face[i+1]==edge.edge_end)||(face[i]==edge.edge_end && face[i+1]==edge.edge_start))
                        edge_list.emplace_back(edge);
                }
            }
            else if(i == face.size()-1)
            {
                for (auto edge:single_edge_list)
                {
                    if((face[i]==edge.edge_start&&face[0]==edge.edge_end)||(face[i]==edge.edge_end && face[0]==edge.edge_start))
                        edge_list.emplace_back(edge);
                }
            }
        }
        //compute barycenter for each face
        compute_barycenter();
    }

    // constructor without argument
    Face():face_id(0)
    {}

    // function to compute barycenter
    void compute_barycenter() {
        float x = 0, y = 0, z = 0;
        for(auto vertex :vertex_coord) {
            x += vertex.point.x;
            y+=vertex.point.y;
            z+=vertex.point.z;
        }
        // compute the barycenter of each face.
        barycenter = Vertex(x/vertex_coord.size(),y/vertex_coord.size(),z/vertex_coord.size(),0);
    }
};

class Volume {
public:
    int volume_id;
    Dart * incident_dart= nullptr;
    // constructor with argument id.
    Volume(int id):volume_id(id)
    {}

    Volume()
    {}
};

class Dart {
public:
    int dart_id;
    Dart* a0 = nullptr;
    Dart* a1= nullptr;
    Dart* a2= nullptr;
    Dart* a3= nullptr;

    Vertex * vertex; // 0-dimension vertex
    Edge * edge; // 1-dimension vertex
    Face * face; // 2-dimension vertex

    // constructor without arguments
    Dart(int i):dart_id(i)
    {}
    // constructor with arguments: dart_id,vertex and edge.
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
    // store all the ordered edges into the vector.
    //e.g.:(1,5) and (5,1) are different edges now.
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

    // the function is to build the unordered map based on the ordered map.
    //e.g.:(1,5) and (5,1) are the equivalent in the unordered map, so the edge will be stored only once.
    static std::vector<Edge> build_single_edges(const std::vector<std::vector<Edge>>& edge_list)
    {
        std::vector<Edge> result_edge;
        for (const auto & i : edge_list)
        {
            for (auto j : i)
            {
                if (result_edge.empty()) result_edge.emplace_back(j);
                else
                {
                    int edge_size=result_edge.size();
                    bool same= true;
                    for (int k = 0; k < edge_size; ++k)
                    {
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

    // the function is to find a2 dart for every current input dart
    static Dart * find_a2_dart(Dart * current_dart,std::vector<std::vector<Dart *>> darts_list)
    {
        Dart * incident;
        for (int i = 0; i < darts_list.size(); ++i)
        {
            if (i==current_dart->face->face_id) continue;
            else
            {
                for(auto dart:darts_list[i])
                {
                    if(dart->vertex->vertex_id == current_dart->vertex->vertex_id && dart->edge->edge_id == current_dart->edge->edge_id)
                    {   incident = dart; }
                }
            }
        }
        return incident;
    }

    // the function is to find incident dart for each face
    static void face_incident_dart(const std::vector<std::vector<Dart *>> &darts_list,std::vector<Face> & faces)
    {
        for(int i=0;i<faces.size();++i) {
            for (const auto &darts: darts_list) {
                for (auto dart: darts) {
                    if (faces[i].face_id == dart->face->face_id)
                    {
                        faces[i].incident_dart = dart;
                        break;
                    }
                }
                if (faces[i].incident_dart != nullptr)
                    break;
            }
        }
    }

    // the function is to find incident dart for each edge
    static void edge_incident_dart(const std::vector<std::vector<Dart *>> &darts_list,std::vector<Edge> & edges) {
        for(int i=0;i<edges.size();++i) {
            for(const auto& darts:darts_list) {
                for(auto dart:darts) {
                    if (edges[i].edge_id==dart->edge->edge_id) {
                        edges[i].incident_dart=dart;
                        break;
                    }
                }
                if (edges[i].incident_dart != nullptr)
                    break;
            }
        }
    }

    // the function is to find incident dart for each vertex
    static void vertex_incident_dart(const std::vector<std::vector<Dart *>> &darts_list,std::vector<Vertex> & vertices) {
        for(int i=0;i<vertices.size();++i)
        {
            for(const auto& darts:darts_list)
            {
                for(auto dart:darts)
                {
                    if (vertices[i].vertex_id==dart->vertex->vertex_id)
                    {
                        vertices[i].incident_dart=dart;
                        break;
                    }
                }
                if (vertices[i].incident_dart!= nullptr) break;
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
        for (auto & face : faces)
        {
            std::vector<Dart *> darts;
            for (int i = 0; i < face.vertex_id.size(); ++i)
            {
                if (i == 0) {
                    Dart * dart_1 = new Dart(count, face.vertex_coord[i], face.edge_list[(face.edge_list.size())-1], face);
                    Dart * dart_2 = new Dart(count+1, face.vertex_coord[i],face.edge_list[i], face);
                    darts.emplace_back(dart_1);
                    darts.emplace_back(dart_2);
                }
                else
                {
                    Dart * dart_1 = new Dart(count, face.vertex_coord[i], face.edge_list[i-1], face);
                    Dart * dart_2 = new Dart(count+1, face.vertex_coord[i],face.edge_list[i], face);
                    darts.emplace_back(dart_1);
                    darts.emplace_back(dart_2);
                }
                count+=2;
            }
            darts_list.emplace_back(darts);
        }
        edge_incident_dart(darts_list,edges);
        vertex_incident_dart(darts_list,vertices);
        face_incident_dart(darts_list,faces);
        return darts_list;
    }

    // the function is to find a1, a2, a0 for each dart.
    static std::vector<std::vector<Dart *>> involutions(std::vector<std::vector<Dart *>> & darts_list, std::vector<Face> & faces) {
        for (auto &darts: darts_list)
        {
            for (int i = 0; i < darts.size(); ++i)
            {
                // in a single face (because a0 and a1 are in the same face
                for (int j = 0; j < darts.size(); ++j)
                {
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
    // constructor with arguments
    Triangulation(int p_0,int p_1,int p_2):p0(p_0),p1(p_1),p2(p_2)
    {}
    // constructor without argument.
    Triangulation()
    {}
    static void dart_triangulation(const std::vector<std::vector<Dart *>>& darts_list, const std::vector<Face> & faces,const std::vector<Edge> & edges,const std::vector<Vertex> vertices,std::vector<Vertex> & vertex_list,std::vector<Triangulation> & face_list) {

        Vertex start,end,d2,d1,d0;
        int count=1;
        for (int i = 0; i < darts_list.size(); ++i)
        {
            d2=faces[i].barycenter;
            d2.vertex_id=count;
            vertex_list.emplace_back(d2);
            count++;
            for (int j = 0; j < darts_list[i].size(); ++j)
            {
                start = vertices[darts_list[i][j]->edge->edge_start-1];
                end = vertices[darts_list[i][j]->edge->edge_end-1];
                //start= darts_list[i][j]->edge->edge_start;
                d1 = Vertex((start.x + end.x) / 2, (start.y + end.y) / 2, (start.z + end.z) / 2, count);

                d0 = * darts_list[i][j]->vertex;
                bool bool0= false,bool1 = false;

                for(auto vertex:vertex_list)
                {
                    if(vertex.point==d0.point)
                    {
                        bool0 = true;
                        d0=vertex;
                    }
                    if(vertex.point == d1.point)
                    {
                        bool1= true;
                        d1=vertex;
                    }
                }
                if (!bool0)
                {
                    d0.vertex_id=count;
                    vertex_list.emplace_back(d0);
                    count++;
                }
                if(! bool1)
                {
                    d1.vertex_id=count;
                    vertex_list.emplace_back(d1);
                    count++;
                }
                if(darts_list[i][j]->dart_id % 2 ==0)
                {
                    Triangulation tri=Triangulation(d2.vertex_id,d1.vertex_id,d0.vertex_id);
                    face_list.emplace_back(tri);
                }
                else if(darts_list[i][j]->dart_id%2 ==1 )
                {
                    Triangulation tri=Triangulation(d2.vertex_id,d0.vertex_id,d1.vertex_id);
                    face_list.emplace_back(tri);
                }
            }
        }
    }

    // the function is to writer
    static void output_obj(const std::vector<Vertex> & vertex_list,const std::vector<Triangulation> & face_list, const std::string& file_path)
    {
        std::string filename=file_path+"/triangulated.obj";
        std::ofstream output_file;
        output_file.open(filename, std::ios::out | std::ios::trunc);
        output_file << std::fixed;
        // print data into the file
        for(const auto& vertex:vertex_list)
            output_file << "v "<<vertex.point.x<<" "<< vertex.point.y<<" " << vertex.point.z <<std::endl;
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
                if(d->a3 == nullptr)
                    output_file <<"v"<< d->dart_id<<";"<< d->a0->dart_id<<";"<< d->a1->dart_id<<";"<< d->a2->dart_id<<";" << "-"<<";"<< d->vertex->vertex_id<<";"<< d->edge->edge_id<<";"<< d->face->face_id<<std::endl;
                else if(d->a3 != nullptr)
                    output_file <<"v"<< d->dart_id<<";"<< d->a0->dart_id<<";"<< d->a1->dart_id<<";"<< d->a2->dart_id<<";" <<d->a3->dart_id<<";"<< d->vertex->vertex_id<<";"<< d->edge->edge_id<<";"<< d->face->face_id<<std::endl;

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
        output_file <<"ID;incident_dart;x;y;z" << std::endl;
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
        output_file <<"ID;incident_dart" << std::endl;
        // print data into the file
        for(const auto& edge:edges)
            output_file <<edge.edge_id<<";"<<edge.incident_dart->dart_id<<std::endl;
        // close file
        output_file.close();
    }

    static void output_faces(const std::vector<Face>& faces, const std::string& file_path) {

        std::ofstream output_file;
        std::string filename=file_path+"/faces.csv";
        output_file.open(filename, std::ios::out | std::ios::trunc);
        output_file << std::fixed;
        output_file <<"ID;incident_dart" << std::endl;
        // print data into the file
        for(const auto& face:faces)
            output_file <<face.face_id<<";"<<face.incident_dart->dart_id<<std::endl;
        // close file
        output_file.close();
    }

    // the function to write volume.csv
    // the output format: ID; dart
    static void output_volume(const std::vector<Volume>& volumes, const std::string& file_path) {
        std::ofstream output_file;
        std::string filename=file_path+"/volumes.csv";
        output_file.open(filename, std::ios::out | std::ios::trunc);
        output_file << std::fixed;
        output_file <<"ID;incident_dart" << std::endl;
        // print data into the file
        for(const auto& volume:volumes)
            if (volume.incident_dart== nullptr)
                output_file <<volume.volume_id<<";" <<"-"<<std::endl;
            else
                output_file<<volume.volume_id<<";"<<volume.incident_dart->dart_id<<std::endl;
        // close file
        output_file.close();
    }
};