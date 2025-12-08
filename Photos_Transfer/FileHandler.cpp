#include "FileHandler.h"
#include <fstream>
#include <algorithm>
#include <cctype>

FileHandler::FileHandler() {
}

FileHandler::~FileHandler() {
}

bool FileHandler::SaveFile(const std::string& filepath, const std::string& data) {
    std::ofstream outFile(filepath, std::ios::binary);
    if (!outFile.is_open()) {
        return false;
    }
    
    outFile.write(data.c_str(), data.length());
    outFile.close();
    
    return true;
}

std::string FileHandler::GetFileExtension(const std::string& filename) {
    size_t dotPos = filename.find_last_of('.');
    if (dotPos == std::string::npos) {
        return "";
    }
    
    std::string extension = filename.substr(dotPos + 1);
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
    return extension;
}

bool FileHandler::IsValidFileType(const std::string& extension, const char* validExtensions[], int count) {
    for (int i = 0; i < count; i++) {
        if (extension == validExtensions[i]) {
            return true;
        }
    }
    return false;
}

bool FileHandler::IsImageFile(const std::string& filename) {
    const char* imageExtensions[] = {
        "jpg", "jpeg", "png", "gif", "bmp", "webp", "heic", "heif", "tiff", "tif", "ico", "svg"
    };
    
    std::string extension = GetFileExtension(filename);
    return IsValidFileType(extension, imageExtensions, sizeof(imageExtensions) / sizeof(imageExtensions[0]));
}

bool FileHandler::IsVideoFile(const std::string& filename) {
    const char* videoExtensions[] = {
        "mp4", "avi", "mov", "wmv", "flv", "mkv", "webm", "m4v", "mpg", "mpeg", "3gp", "mts", "m2ts"
    };
    
    std::string extension = GetFileExtension(filename);
    return IsValidFileType(extension, videoExtensions, sizeof(videoExtensions) / sizeof(videoExtensions[0]));
}
