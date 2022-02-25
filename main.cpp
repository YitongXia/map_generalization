#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "Gmap.h"

int main(int argc, const char * argv[]) {

    std::string file_in = "./cube.txt";
    std::ifstream ifs;
    ifs.open(file_in);

    std::vector<Vertex> vertices;
    std::vector<Face> faces;
    std::vector<Edge> ordered_edges;
    std::vector<std::vector<Edge>>edge_list;
    std::vector<std::vector<int>> face_list;
    int vertices_id = 1;
    int face_id=1;
    if (ifs.is_open()) {
        std::string line;
        while (getline(ifs, line)) {
            std::istringstream iss(line);
            std::string word;
            iss >> word;
            if (word == "v") {
                std::vector<float> coordinates;
                while (iss >> word) coordinates.push_back(std::stof(word));
                if (coordinates.size() == 3) vertices.emplace_back(coordinates[0], coordinates[1], coordinates[2],vertices_id);
                else vertices.emplace_back();
                vertices_id++;
            }
            else if (word == "f") {
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
    }
    std::vector<Edge> unordered_edges;
    unordered_edges=GeneralizedMap::build_single_edges(edge_list);

    std::cout<<"creating face now"<<std::endl;

    for (int i = 0; i < face_list.size();++i)
        faces.emplace_back(face_list[i], unordered_edges, vertices, i);

    std::cout<<"building faces completed"<<std::endl;
    std::cout<<"building darts now"<<std::endl;

    std::vector<std::vector<Dart *>> darts_list = GeneralizedMap::build_darts(faces,unordered_edges,vertices);
    darts_list = GeneralizedMap::involutions(darts_list,faces);

    std::cout<<"building darts completed"<<std::endl;

    for(const auto& darts:darts_list) {
        for (auto dart:darts) {
            if (dart->a2 == nullptr) {
                //std::cout<<"the dart "<<dart->dart_id<<" ,vertex is "<<dart->vertex->vertex_id<<", edge is "<<dart->a1->edge->edge_id<<", face is "<<dart->face->face_id<<std::endl;
                std::cout<<"the dart "<<dart->dart_id<<" ,a0 is "<<dart->a0->dart_id<<", a1 is "<<dart->a1->dart_id<<", a2 is null"<<std::endl;
            }
            else
            {
                //std::cout << "the dart " << dart->dart_id << " ,vertex is " << dart->vertex->vertex_id << ", edge is "<< dart->a1->edge->edge_id << ", face is " << dart->face->face_id << std::endl;
                std::cout << "the dart " << dart->dart_id << " ,a0 is " << dart->a0->dart_id << ", a1 is "
                          << dart->a1->dart_id << ", a2 is " << dart->a2->dart_id << std::endl;
            }
        }
    }

    //still need to debug
    std::vector<Vertex> vertex_list;
    std::vector<Triangulation> triangulation_list;
    Triangulation::dart_triangulation(darts_list,faces,vertices,vertex_list,triangulation_list);
    Triangulation::output_obj(vertex_list,triangulation_list,"/");
    
    WriteCSV::output_dart(darts_list,".");

    // ## Construct generalised map using the structures from Gmap.h ##

    // ## Output generalised map to CSV ##

    // ## Create triangles from the darts ##

    // ## Write triangles to obj ##

    return 0;
}
