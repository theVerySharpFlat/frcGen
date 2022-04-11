#include <ostream>

extern "C" {
#include <incbin.h>
}

#include <cxxopts.hpp>
#include <string>
#include <iostream>
#include <algorithm>
#include <cstdlib>

#include <ghc/filesystem.hpp>
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

int main(int argc, char** argv) {
    auto options = cxxopts::Options("FRC C++ File Generator", "Generate commands and subsystems from your frc project from the cli!");
    options.add_options("FRC C++ File Generator")
        ("c,command", "Generate a command.", cxxopts::value<bool>()->default_value("false"))
        ("s,susbystem", "Generate a susbsytem.", cxxopts::value<bool>()->default_value("false"))
        ("h,help", "Print usage")
        ("name", "The name of the command or subsystem", cxxopts::value<std::string>()->default_value("MyCommand"))
        ;
    options.parse_positional("name");
    auto parsedOps = options.parse(argc, argv);

    if(parsedOps.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    if(!parsedOps.count("c") && !parsedOps.count("s")) {
        std::cout << "Command or Subsystem was not specified. Not doing anything!" << std::endl;
        exit(1);
    }

    // construct filepaths
    auto cppFilePath = filesystem::path("src/main/cpp/" + parsedOps["name"].as<std::string>() + ".cpp");
    auto hFilePath = filesystem::path("src/main/include/" + parsedOps["name"].as<std::string>() + ".h");

    if(!filesystem::exists(filesystem::absolute(cppFilePath).parent_path())) {
        std::cout << cppFilePath.parent_path() << " does not exist!" << std::endl;
        exit(1);
    }

    if(!filesystem::exists(filesystem::absolute(hFilePath).parent_path())) {
        std::cout << hFilePath.parent_path() << " does not exist!" << std::endl;
        exit(1);
    }

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
    dumpStringToFile(cppFilePath, cppReplaced);
    dumpStringToFile(hFilePath, hReplaced);

    return 0;
}

std::string findAndReplace(const char *data, unsigned int size, const char *toFind,
                           const char *toReplace) {
    std::string str(data, size - 1);
    return std::regex_replace(str, std::regex(toFind), toReplace);
}

void dumpStringToFile(filesystem::path pathToFile, const std::string &data) {
    auto file = filesystem::ofstream(pathToFile);
    file << data;
    file.close();
}
