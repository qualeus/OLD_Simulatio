#ifndef FileManager_HPP
#define FileManager_HPP

#include <stdio.h>

#include <Structures/System.hpp>
#include <cereal/archives/json.hpp>
#include <iostream>
#include <string>

#ifdef WIN32
#include <windows.h>
// Must be second
#include <ShellAPI.h>
#endif

#ifdef linux
// TODO
#endif

namespace ovl {

class FileManager {
   private:
    FileManager();

   public:
    static std::string CurrentPath();
    static void OpenFileExplorer(std::string path);
    static std::string SelectFile(std::string path = nullptr, std::string filter = "All\0*.*\0Text\0*.txt\0Json\0*.json\0", bool saving = false);

    static phy::System LoadSystem(std::string path);
    static void SaveSystem(const phy::System& system, std::string path);
};

}  // namespace ovl

#endif