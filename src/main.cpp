#include "ghc/filesystem.hpp"
#include <ostream>
#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>
#include <exception>
using namespace ghc; // this namespace has the filesystem namespace which is
                     // "experimental" in earlier versions of clang and gcc
                     // To "simulate" usage of this api, I'm going to use the
                     // 'using declaration'

/*
 * Load the templates into the binary because I don't feel like loading them from
 * the filesystem
 */
INCTXT(commandTemplateCppFile,   "../templates/command/templateCommand.cpp");
INCTXT(commandTemplateHFile,     "../templates/command/templateCommand.h");
INCTXT(subsystemTemplateCppFile, "../templates/subsystem/templateSubsystem.cpp");
INCTXT(subsystemTemplateHFile,   "../templates/subsystem/templateSubsystem.h");
INCTXT(dotFrcGenDotJsonFile, "../templates/.frcGen.json");

/* Utility functions */

/*
 * Take in some data and replace all of the occurances
 * @param data the data to take in
 * @param size the number of bytes that data is
 * @param toFind the string to look for
 * @param toReplace the string to replace toFind with
 */
std::string findAndReplace(const char* data, unsigned int size, const char* toFind, const char* toReplace);

/*
 * Dump an std::string to a file
 * @param pathToFile the path to the file
 * @param data the data to dump to the file
 */
void dumpStringToFile(filesystem::path pathToFile, const std::string& data);

/*
 * Changes CWD to project root (the directory with the build.gradle)
 */
void gotoProjectRoot();

int main(int argc, char** argv) {
    auto options = cxxopts::Options("FRC C++ File Generator", "Generate commands and subsystems from your frc project from the cli!");
    options.add_options("FRC C++ File Generator")
        ("c,command", "Generate a command.", cxxopts::value<bool>()->default_value("false"))
        ("s,susbystem", "Generate a susbsytem.", cxxopts::value<bool>()->default_value("false"))
        ("h,help", "Print usage")
        ("g,generate-config", "Generate a .frcGen.json file at the project root.")
        ("name", "The name of the command or subsystem", cxxopts::value<std::string>()->default_value("UnReplaced"))
        ;
    options.parse_positional({"name"});
    options.positional_help("name").show_positional_help();
    auto parsedOps = options.parse(argc, argv);

    if(parsedOps.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    gotoProjectRoot();

    filesystem::path includeDir = "src/main/include/";
    filesystem::path srcDir = "src/main/cpp/";

    if(filesystem::exists(filesystem::path(".frcGen.json"))) {
        auto dotFrcGenDotJson = 
            filesystem::ifstream(filesystem::path(".frcGen.json"));
        std::stringstream buffer;
        buffer << dotFrcGenDotJson.rdbuf();
        std::string data = buffer.str();
        dotFrcGenDotJson.close();

        rapidjson::Document document;
        document.Parse(data.c_str());

        if(document.HasMember("include-directory") && document["include-directory"].IsString()) {
            includeDir = document["include-directory"].GetString();
        } else {
            std::cout << "warning: no include directory specified in .frcGen.json "
                << "or value is of incorrect type" << std::endl;
        }

        if(document.HasMember("source-directory") && document["source-directory"].IsString()) {
            srcDir = document["source-directory"].GetString();
        } else {
            std::cout << "warning: no source directory specified in .frcGen.json "
                << "or value is of incorrect type" << std::endl;
        }
    }

    if(parsedOps.count("g")) {
        dumpStringToFile(
            ".frcGen.json", 
            std::string(gdotFrcGenDotJsonFileData, gdotFrcGenDotJsonFileSize-1)
            );
        exit(1);
    }


    if(!parsedOps.count("c") && !parsedOps.count("s")) {
        std::cout << "Command or Subsystem was not specified. Not doing anything!" << std::endl;
        exit(1);
    }

    // construct filepaths
    auto cppFilePath = 
        filesystem::path(srcDir / filesystem::path(parsedOps["name"].as<std::string>() + ".cpp"));
    auto hFilePath = 
        filesystem::path(includeDir / filesystem::path( parsedOps["name"].as<std::string>() + ".h"));

    if(filesystem::exists(cppFilePath)) {
        std::cout << cppFilePath << " already exists! I will not overwrite!" << std::endl;
        exit(1);
    }

    if(filesystem::exists(hFilePath)) {
        std::cout << hFilePath << " already exists! I will not overwrite!" << std::endl;
        exit(1);
    }

    std::string cppReplaced;
    std::string hReplaced;

    if(parsedOps["s"].as<bool>()) {
        cppReplaced = findAndReplace(
          gsubsystemTemplateCppFileData, gsubsystemTemplateCppFileSize,
          "ReplaceMeSubsystem2", cppFilePath.stem().c_str());

        hReplaced = findAndReplace(
            gsubsystemTemplateHFileData, gsubsystemTemplateHFileSize,
            "ReplaceMeSubsystem2", hFilePath.stem().c_str());

    } else if (parsedOps["c"].as<bool>()) {
      cppReplaced = findAndReplace(
          gcommandTemplateCppFileData, gcommandTemplateCppFileSize,
          "ReplaceMeCommand2", cppFilePath.stem().c_str());

      hReplaced = findAndReplace(
          gcommandTemplateHFileData, gcommandTemplateHFileSize,
          "ReplaceMeCommand2", hFilePath.stem().c_str());
    }

    // dump to file
    std::cout << "dumping source file to " << cppFilePath << std::endl;
    dumpStringToFile(cppFilePath, cppReplaced);
    
    std::cout << "dumping header file to " << hFilePath << std::endl;
    dumpStringToFile(hFilePath, hReplaced);

    return 0;
}

std::string findAndReplace(const char *data, unsigned int size, const char *toFind,
                           const char *toReplace) {
    std::string str(data, size - 1);
    return std::regex_replace(str, std::regex(toFind), toReplace);
}

void dumpStringToFile(filesystem::path pathToFile, const std::string &data) {
    try{
        filesystem::create_directories(pathToFile.parent_path());
    } catch(std::exception e) {
        std::cout << "error: " << e.what() << std::endl;
        exit(1);
    }

    auto file = filesystem::ofstream(pathToFile);
    file << data;
    file.close();
}

void gotoProjectRoot() {
    auto buildDotGradle = filesystem::path("build.gradle");
    auto dotFrcGen = filesystem::path(".frcGen.json");
    if(!(filesystem::exists(dotFrcGen) || filesystem::exists(buildDotGradle))) {
        if(filesystem::current_path() == filesystem::current_path().root_path()) {
            std::cout << "You are not in a project!!!" << std::endl;
            exit(1);
        }
        filesystem::current_path(filesystem::current_path().parent_path());
        gotoProjectRoot();
    }
}
