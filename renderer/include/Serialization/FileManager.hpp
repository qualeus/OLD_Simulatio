#ifndef FileManager_HPP
#define FileManager_HPP

#include <stdio.h>

#include <Structures/System.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>
#include <iostream>
#include <string>

#include "Serialization.hpp"

#ifdef WIN32
#include <windows.h>
// Must be second
#include <ShellAPI.h>
#endif

#ifdef linux
// TODO
#endif

namespace srz {

class FileManager {
   private:
    FileManager();

   public:
    static std::string CurrentPath();
    static void OpenFileExplorer(std::string path);
    static std::string SelectFile(std::string name = "Explorer", std::string path = nullptr, LPCSTR filter = "All\0*.*\0Text\0*.txt\0Json\0*.json\0", bool saving = false);

    static phy::System LoadSystem(std::string path);
    static void SaveSystem(const phy::System& system, std::string path);
};

}  // namespace srz

#endif