#pragma once
#include <string>
#include <winsock2.h>

class Server {
public:
    Server(int port);
    ~Server();
    
    bool Start();
    void Stop();
    std::string GetUploadDirectory() const;
    
private:
    int port_;
    SOCKET listenSocket_;
    std::string uploadDirectory_;
    bool isRunning_;
    
    void HandleClient(SOCKET clientSocket);
    void SendResponse(SOCKET clientSocket, const std::string& response);
    void SendFile(SOCKET clientSocket, const std::string& filename);
    void HandleFileUpload(SOCKET clientSocket, const std::string& headers, const std::string& boundary);
    std::string GetHTMLPage();
    std::string ExtractBoundary(const std::string& contentType);
    std::string ExtractFilename(const std::string& contentDisposition);
    void CreateUploadDirectory();
    std::string GetCurrentDateTime();
};
