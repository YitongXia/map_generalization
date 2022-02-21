// some standard libraries that are helpfull for reading/writing text files
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

    bool status=ifs.is_open();
    std::cout<<"the result is "<<status<<std::endl;

    std::vector<Vertex> vertices;
    std::vector<Face> faces;
    int vertices_id = 1;
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
                else vertices.push_back(Vertex());
                vertices_id++;
            }
            else if (word == "f") {
                std::vector<int> face;
                while(iss>>word) face.push_back(std::stof(word));
                if (face.size()==4) faces.emplace_back(vertices[face[0]-1].point,vertices[face[1]-1].point,vertices[face[2]-1].point,vertices[face[3]-1].point);

                else faces.push_back(Face());
            }
        }
    }
    for (int i = 0; i < faces.size(); ++i) {
        faces[i].face_ver.print();
    }
    for (int i = 0; i < faces.size(); ++i) {
        faces[i]
    }



    // ## Construct generalised map using the structures from Gmap.h ##

    // ## Output generalised map to CSV ##

    // ## Create triangles from the darts ##

    // ## Write triangles to obj ##

    return 0;
}
