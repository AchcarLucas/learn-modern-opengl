#include <iostream>
#include <fstream>
#include <string>

// assimp include files. These three are usually needed.
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

// The default hard-coded path. Can be overridden by supplying a path through the command line.
static std::string modelpath = "./resources/obj/rock/rock.obj";

// the global Assimp scene object
static const aiScene* g_scene = nullptr;

// Create an instance of the Importer class
static Assimp::Importer importer;

static bool Import3DFromFile(const std::string &filename) {
    // check if file exists
    std::ifstream fin(filename.c_str());
    if(fin.fail()) {
        std::cout << "couldn't open file: " << filename << std::endl;
        return false;
    }

    fin.close();

    g_scene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_Quality);

    // if the import failed, report it
    if (g_scene == nullptr) {
        std::cout << importer.GetErrorString() << std::endl;
        return false;
    }

    // now we can access the file's contents.
    std::cout << "import of scene " << filename << " succeeded." << std::endl;

    // we're done. Everything will be cleaned up by the importer destructor
    return true;
}

int load_assimp_test()
{
    if(!Import3DFromFile(modelpath)) {
        return -1;
    }

    return 0;
}
