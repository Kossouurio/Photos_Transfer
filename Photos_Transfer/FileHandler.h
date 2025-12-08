#pragma once
#include <string>

class FileHandler {
public:
    FileHandler();
    ~FileHandler();
    
    bool SaveFile(const std::string& filepath, const std::string& data);
    bool IsImageFile(const std::string& filename);
    bool IsVideoFile(const std::string& filename);
    std::string GetFileExtension(const std::string& filename);
    
private:
    bool IsValidFileType(const std::string& extension, const char* validExtensions[], int count);
};
