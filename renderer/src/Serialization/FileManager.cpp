#include "../../include/Serialization/FileManager.hpp"

namespace srz {

FileManager::FileManager() {}

std::string FileManager::CurrentPath() {
#ifdef WIN32
    wchar_t extr_path[MAX_PATH];
    GetModuleFileNameW(NULL, extr_path, MAX_PATH);
    std::wstring r_path = std::wstring(extr_path);
    std::string current_path = std::string(r_path.begin(), r_path.end());
    return current_path.substr(0, current_path.find_last_of("\\/"));
#endif
#ifdef linux
// TODO
#endif
}

void FileManager::OpenFileExplorer(std::string path) {
#ifdef WIN32
    ShellExecute(0, nullptr, "explorer.exe", path.c_str(), nullptr, SW_SHOWNORMAL);
#endif
#ifdef linux
// TODO
#endif
}

std::string FileManager::SelectFile(std::string name, std::string path, LPCSTR filter, bool saving) {
#ifdef WIN32
    OPENFILENAME open;
    char fileName[MAX_PATH] = "";

    ZeroMemory(&open, sizeof(open));
    open.lStructSize = sizeof(OPENFILENAME);
    open.lpstrFile = fileName;
    open.nMaxFile = MAX_PATH;
    open.lpstrTitle = name.c_str();
    open.Flags = OFN_EXPLORER | OFN_HIDEREADONLY | (saving ? OFN_NOTESTFILECREATE : OFN_FILEMUSTEXIST);
    open.lpstrInitialDir = path.c_str();
    open.lpstrFilter = filter;
    // open.nFilterIndex = 0;
    open.lpstrDefExt = "";

    std::string fileNameStr;
    if (saving ? GetSaveFileName(&open) : GetOpenFileName(&open)) fileNameStr = fileName;

    return fileNameStr;
#endif
#ifdef linux
    // TODO
    return "";
#endif
}

phy::System FileManager::LoadSystem(std::string path) {
    std::ifstream file = std::ifstream(path);
    cereal::JSONInputArchive iarchive = cereal::JSONInputArchive(file);
    phy::System system;
    iarchive(cereal::make_nvp<phy::System>("system", system));
    return system;
}

void FileManager::SaveSystem(const phy::System& system, std::string path) {
    std::ofstream file = std::ofstream(path);
    cereal::JSONOutputArchive oarchive = cereal::JSONOutputArchive(file);
    oarchive(cereal::make_nvp<phy::System>("system", system));
}

}  // namespace srz