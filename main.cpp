#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "Gmap.h"

int main(int argc, const char * argv[]) {

    std::string file_in = "./torus.obj";
    std::ifstream ifs;
    ifs.open(file_in);

    std::vector<Vertex> vertices; // vector to store all the vertices.
    std::vector<Face> faces; // vector to store all the faces
    std::vector<Edge> ordered_edges; // vector to store all the ordered edges (for unordered edges)
    std::vector<std::vector<Edge>>edge_list; // vector to store all the unordered edges
    std::vector<std::vector<int>> face_list; // vector to store face information which is read from obj file.
    std::vector<Volume> volumes; // vector to store volume
    int vertices_id = 1;
    int face_id=1;
    if (ifs.is_open()) {
        std::string line;
        while (getline(ifs, line)) {
            std::istringstream iss(line);
            std::string word;
            iss >> word;
            if (word == "v")
            {
                std::vector<float> coordinates;
                while (iss >> word) coordinates.push_back(std::stof(word));
                if (coordinates.size() == 3) vertices.emplace_back(coordinates[0], coordinates[1], coordinates[2],vertices_id);
                else vertices.emplace_back();
                vertices_id++;
            }
            else if (word == "f")
            {
                std::vector<int> face;
                while(iss>>word) face.push_back(std::stof(word));
                if (face.size()==4) {
                    face_list.emplace_back(face);
                    ordered_edges= GeneralizedMap::create_edges(face);
                    edge_list.emplace_back(ordered_edges);
                }
                else face_list.emplace_back(face);
            }
        }
        volumes.emplace_back(1);
    }
    std::vector<Edge> unordered_edges;
    unordered_edges=GeneralizedMap::build_single_edges(edge_list);

    std::cout<<"creating face now.."<<std::endl;

    for (int i = 0; i < face_list.size();++i)
        faces.emplace_back(face_list[i], unordered_edges, vertices, i);

    std::cout<<"creating faces completed"<<std::endl;

    std::cout<<"building darts now.."<<std::endl;

    std::vector<std::vector<Dart *>> darts_list = GeneralizedMap::build_darts(faces,unordered_edges,vertices);
    darts_list = GeneralizedMap::involutions(darts_list,faces);

    std::cout<<"building darts completed"<<std::endl;

    std::cout<<"output obj file now.."<<std::endl;
    std::vector<Vertex> vertex_list;
    std::vector<Triangulation> triangulation_list;
    Triangulation::dart_triangulation(darts_list,faces,unordered_edges,vertices,vertex_list,triangulation_list);
    Triangulation::output_obj(vertex_list,triangulation_list,".");
    std::cout<<"output obj file completed"<<std::endl;
    std::cout<<"output csv file now.."<<std::endl;

    WriteCSV::output_dart(darts_list,".");
    WriteCSV::output_edge(unordered_edges,".");
    WriteCSV::output_vertex(vertices,".");
    WriteCSV::output_faces(faces,".");
    WriteCSV::output_volume(volumes,".");

    std::cout<<"output csv file completed"<<std::endl;

    return 0;
}
