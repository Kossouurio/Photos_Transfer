#include "Server.h"
#include "FileHandler.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <windows.h>
#include <direct.h>
#include <time.h>

Server::Server(int port) : port_(port), listenSocket_(INVALID_SOCKET), isRunning_(false) {
    CreateUploadDirectory();
}

Server::~Server() {
    Stop();
}

void Server::CreateUploadDirectory() {
    char path[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, path);
    uploadDirectory_ = std::string(path) + "\\Uploads";
    _mkdir(uploadDirectory_.c_str());
}

std::string Server::GetUploadDirectory() const {
    return uploadDirectory_;
}

std::string Server::GetCurrentDateTime() {
    time_t now = time(0);
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return std::string(buffer);
}

bool Server::Start() {
    listenSocket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket_ == INVALID_SOCKET) {
        std::cerr << "Erreur lors de la creation du socket: " << WSAGetLastError() << '\n';
        return false;
    }
    
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port_);
    
    if (bind(listenSocket_, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Erreur lors du bind: " << WSAGetLastError() << '\n';
        closesocket(listenSocket_);
        return false;
    }
    
    if (listen(listenSocket_, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Erreur lors du listen: " << WSAGetLastError() << '\n';
        closesocket(listenSocket_);
        return false;
    }
    
    isRunning_ = true;
    
    while (isRunning_) {
        SOCKET clientSocket = accept(listenSocket_, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            continue;
        }
        
        HandleClient(clientSocket);
        closesocket(clientSocket);
    }
    
    return true;
}

void Server::Stop() {
    isRunning_ = false;
    if (listenSocket_ != INVALID_SOCKET) {
        closesocket(listenSocket_);
        listenSocket_ = INVALID_SOCKET;
    }
}

void Server::HandleClient(SOCKET clientSocket) {
    char buffer[4096];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    
    if (bytesReceived <= 0) {
        return;
    }
    
    buffer[bytesReceived] = '\0';
    std::string request(buffer);
    
    if (request.find("GET / ") == 0 || request.find("GET /index.html") == 0) {
        std::string response = "HTTP/1.1 200 OK\r\n";
        response += "Content-Type: text/html; charset=utf-8\r\n";
        response += "Connection: close\r\n\r\n";
        response += GetHTMLPage();
        SendResponse(clientSocket, response);
    }
    else if (request.find("POST /upload") == 0) {
        std::string contentType;
        size_t ctPos = request.find("Content-Type:");
        if (ctPos != std::string::npos) {
            size_t endPos = request.find("\r\n", ctPos);
            contentType = request.substr(ctPos + 13, endPos - ctPos - 13);
            while (contentType[0] == ' ') contentType.erase(0, 1);
        }
        
        std::string boundary = ExtractBoundary(contentType);
        if (!boundary.empty()) {
            HandleFileUpload(clientSocket, request, boundary);
        } else {
            std::string response = "HTTP/1.1 400 Bad Request\r\n";
            response += "Content-Type: text/plain\r\n";
            response += "Connection: close\r\n\r\n";
            response += "Boundary non trouve";
            SendResponse(clientSocket, response);
        }
    }
    else {
        std::string response = "HTTP/1.1 404 Not Found\r\n";
        response += "Content-Type: text/plain\r\n";
        response += "Connection: close\r\n\r\n";
        response += "Page non trouvee";
        SendResponse(clientSocket, response);
    }
}

void Server::HandleFileUpload(SOCKET clientSocket, const std::string& headers, const std::string& boundary) {
    std::vector<char> fileData;
    char buffer[8192];
    
    size_t headerEnd = headers.find("\r\n\r\n");
    if (headerEnd != std::string::npos) {
        std::string initialData = headers.substr(headerEnd + 4);
        fileData.insert(fileData.end(), initialData.begin(), initialData.end());
    }
    
    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) break;
        fileData.insert(fileData.end(), buffer, buffer + bytesReceived);
        
        std::string tempData(fileData.begin(), fileData.end());
        if (tempData.find("--" + boundary + "--") != std::string::npos) {
            break;
        }
    }
    
    std::string data(fileData.begin(), fileData.end());
    
    std::string filename;
    size_t filenamePos = data.find("filename=\"");
    if (filenamePos != std::string::npos) {
        filenamePos += 10;
        size_t filenameEnd = data.find("\"", filenamePos);
        filename = data.substr(filenamePos, filenameEnd - filenamePos);
    }
    
    size_t dataStart = data.find("\r\n\r\n");
    if (dataStart != std::string::npos) {
        dataStart += 4;
        size_t dataEnd = data.find("--" + boundary, dataStart);
        if (dataEnd != std::string::npos) {
            dataEnd -= 2;
            
            std::string filepath = uploadDirectory_ + "\\" + filename;
            FileHandler fileHandler;
            if (fileHandler.SaveFile(filepath, data.substr(dataStart, dataEnd - dataStart))) {
                std::cout << "[" << GetCurrentDateTime() << "] Fichier reçu: " << filename 
                         << " (" << (dataEnd - dataStart) << " octets)" << '\n';
                
                std::string response = "HTTP/1.1 200 OK\r\n";
                response += "Content-Type: text/plain\r\n";
                response += "Connection: close\r\n\r\n";
                response += "Fichier telecharge avec succes: " + filename;
                SendResponse(clientSocket, response);
                return;
            }
        }
    }
    
    std::string response = "HTTP/1.1 500 Internal Server Error\r\n";
    response += "Content-Type: text/plain\r\n";
    response += "Connection: close\r\n\r\n";
    response += "Erreur lors de l'enregistrement du fichier";
    SendResponse(clientSocket, response);
}

std::string Server::ExtractBoundary(const std::string& contentType) {
    size_t pos = contentType.find("boundary=");
    if (pos != std::string::npos) {
        return contentType.substr(pos + 9);
    }
    return "";
}

void Server::SendResponse(SOCKET clientSocket, const std::string& response) {
    send(clientSocket, response.c_str(), static_cast<int>(response.length()), 0);
}

std::string Server::GetHTMLPage() {
    return R"(<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Transfert de Photos et Videos</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 'Helvetica Neue', Arial, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            padding: 20px;
            display: flex;
            justify-content: center;
            align-items: center;
        }
        .container {
            background: white;
            border-radius: 20px;
            padding: 40px;
            box-shadow: 0 20px 60px rgba(0,0,0,0.3);
            max-width: 500px;
            width: 100%;
        }
        h1 {
            color: #667eea;
            font-size: 28px;
            margin-bottom: 10px;
            text-align: center;
        }
        .subtitle {
            color: #666;
            text-align: center;
            margin-bottom: 30px;
            font-size: 14px;
        }
        .upload-area {
            border: 3px dashed #667eea;
            border-radius: 15px;
            padding: 40px 20px;
            text-align: center;
            cursor: pointer;
            transition: all 0.3s ease;
            background: #f8f9ff;
        }
        .upload-area:hover {
            background: #f0f2ff;
            border-color: #764ba2;
        }
        .upload-area.dragover {
            background: #e8ebff;
            border-color: #764ba2;
            transform: scale(1.02);
        }
        .upload-icon {
            font-size: 60px;
            margin-bottom: 20px;
        }
        .upload-text {
            font-size: 18px;
            color: #667eea;
            margin-bottom: 10px;
            font-weight: 600;
        }
        .upload-hint {
            font-size: 14px;
            color: #999;
        }
        input[type="file"] {
            display: none;
        }
        .btn {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            padding: 15px 40px;
            border: none;
            border-radius: 30px;
            font-size: 16px;
            font-weight: 600;
            cursor: pointer;
            width: 100%;
            margin-top: 20px;
            transition: transform 0.2s;
            box-shadow: 0 4px 15px rgba(102, 126, 234, 0.4);
        }
        .btn:hover {
            transform: translateY(-2px);
            box-shadow: 0 6px 20px rgba(102, 126, 234, 0.6);
        }
        .btn:disabled {
            background: #ccc;
            cursor: not-allowed;
            transform: none;
        }
        .file-list {
            margin-top: 20px;
        }
        .file-item {
            background: #f8f9ff;
            padding: 15px;
            border-radius: 10px;
            margin-bottom: 10px;
            display: flex;
            justify-content: space-between;
            align-items: center;
        }
        .file-name {
            color: #333;
            font-weight: 500;
            flex: 1;
            overflow: hidden;
            text-overflow: ellipsis;
            white-space: nowrap;
            margin-right: 10px;
        }
        .file-size {
            color: #999;
            font-size: 12px;
        }
        .status {
            margin-top: 20px;
            padding: 15px;
            border-radius: 10px;
            text-align: center;
            font-weight: 500;
        }
        .status.success {
            background: #d4edda;
            color: #155724;
        }
        .status.error {
            background: #f8d7da;
            color: #721c24;
        }
        .status.info {
            background: #d1ecf1;
            color: #0c5460;
        }
        .progress {
            width: 100%;
            height: 6px;
            background: #e0e0e0;
            border-radius: 3px;
            margin-top: 10px;
            overflow: hidden;
        }
        .progress-bar {
            height: 100%;
            background: linear-gradient(90deg, #667eea 0%, #764ba2 100%);
            width: 0%;
            transition: width 0.3s;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>📱➡️💻</h1>
        <h1>Transfert de Fichiers</h1>
        <p class="subtitle">Envoyez vos photos et videos facilement</p>
        
        <div class="upload-area" id="uploadArea">
            <div class="upload-icon">📤</div>
            <div class="upload-text">Cliquez ou glissez vos fichiers ici</div>
            <div class="upload-hint">Photos et videos acceptees</div>
        </div>
        
        <input type="file" id="fileInput" multiple accept="image/*,video/*">
        
        <div class="file-list" id="fileList"></div>
        
        <button class="btn" id="uploadBtn" disabled>Envoyer les fichiers</button>
        
        <div id="status"></div>
    </div>

    <script>
        const uploadArea = document.getElementById('uploadArea');
        const fileInput = document.getElementById('fileInput');
        const fileList = document.getElementById('fileList');
        const uploadBtn = document.getElementById('uploadBtn');
        const status = document.getElementById('status');
        let selectedFiles = [];

        uploadArea.addEventListener('click', () => fileInput.click());

        uploadArea.addEventListener('dragover', (e) => {
            e.preventDefault();
            uploadArea.classList.add('dragover');
        });

        uploadArea.addEventListener('dragleave', () => {
            uploadArea.classList.remove('dragover');
        });

        uploadArea.addEventListener('drop', (e) => {
            e.preventDefault();
            uploadArea.classList.remove('dragover');
            handleFiles(e.dataTransfer.files);
        });

        fileInput.addEventListener('change', (e) => {
            handleFiles(e.target.files);
        });

        function handleFiles(files) {
            selectedFiles = Array.from(files);
            displayFiles();
            uploadBtn.disabled = selectedFiles.length === 0;
        }

        function displayFiles() {
            fileList.innerHTML = '';
            selectedFiles.forEach(file => {
                const fileItem = document.createElement('div');
                fileItem.className = 'file-item';
                
                const fileName = document.createElement('div');
                fileName.className = 'file-name';
                fileName.textContent = file.name;
                
                const fileSize = document.createElement('div');
                fileSize.className = 'file-size';
                fileSize.textContent = formatFileSize(file.size);
                
                fileItem.appendChild(fileName);
                fileItem.appendChild(fileSize);
                fileList.appendChild(fileItem);
            });
        }

        function formatFileSize(bytes) {
            if (bytes < 1024) return bytes + ' B';
            if (bytes < 1024 * 1024) return (bytes / 1024).toFixed(1) + ' KB';
            return (bytes / (1024 * 1024)).toFixed(1) + ' MB';
        }

        uploadBtn.addEventListener('click', async () => {
            if (selectedFiles.length === 0) return;
            
            uploadBtn.disabled = true;
            status.innerHTML = '<div class="status info">Envoi en cours...<div class="progress"><div class="progress-bar" id="progressBar"></div></div></div>';
            
            let successCount = 0;
            let errorCount = 0;
            
            for (let i = 0; i < selectedFiles.length; i++) {
                const file = selectedFiles[i];
                const formData = new FormData();
                formData.append('file', file);
                
                try {
                    const response = await fetch('/upload', {
                        method: 'POST',
                        body: formData
                    });
                    
                    if (response.ok) {
                        successCount++;
                    } else {
                        errorCount++;
                    }
                } catch (error) {
                    errorCount++;
                }
                
                const progress = ((i + 1) / selectedFiles.length) * 100;
                document.getElementById('progressBar').style.width = progress + '%';
            }
            
            if (errorCount === 0) {
                status.innerHTML = `<div class="status success">✅ ${successCount} fichier(s) transfere(s) avec succes !</div>`;
                selectedFiles = [];
                fileList.innerHTML = '';
                fileInput.value = '';
            } else {
                status.innerHTML = `<div class="status error">❌ ${successCount} succes, ${errorCount} echec(s)</div>`;
            }
            
            uploadBtn.disabled = selectedFiles.length === 0;
            
            setTimeout(() => {
                if (errorCount === 0) {
                    status.innerHTML = '';
                }
            }, 3000);
        });
    </script>
</body>
</html>)";
}
