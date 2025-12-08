#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include "Server.h"

#pragma comment(lib, "ws2_32.lib")

int main() {
    // Configuration de la console pour afficher les caracteres Unicode
    SetConsoleOutputCP(CP_UTF8);
    
    std::cout << "=== Application de Transfert de Photos et Videos ===" << '\n';
    std::cout << "=====================================================" << '\n';
    
    // Initialisation de Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "Erreur lors de l'initialisation de Winsock: " << result << '\n';
        return 1;
    }
    
    // Obtenir l'adresse IP locale
    char hostname[256];
    gethostname(hostname, sizeof(hostname));
    
    // Remplacement de gethostbyname (obsolete) par getaddrinfo (recommande)
    struct addrinfo hints = {};
    hints.ai_family = AF_INET; // IPv4 uniquement
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo* result_addrinfo = nullptr;
    int gai_ret = getaddrinfo(hostname, nullptr, &hints, &result_addrinfo);
    if (gai_ret == 0 && result_addrinfo != nullptr) {
        std::cout << "\nAdresses IP disponibles:" << '\n';
        for (struct addrinfo* ptr = result_addrinfo; ptr != nullptr; ptr = ptr->ai_next) {
            struct sockaddr_in* sockaddr_ipv4 = reinterpret_cast<struct sockaddr_in*>(ptr->ai_addr);
            char ipstr[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(sockaddr_ipv4->sin_addr), ipstr, sizeof(ipstr));
            std::cout << "  - " << ipstr << '\n';
        }
        freeaddrinfo(result_addrinfo);
    } else {
        std::cerr << "Erreur lors de la recuperation des adresses IP: " << gai_strerrorA(gai_ret) << '\n';
    }
    
    // Creation et demarrage du serveur
    const int PORT = 8080;
    Server server(PORT);
    
    std::cout << "\nServeur demarre sur le port " << PORT << '\n';
    std::cout << "\nPour transferer des fichiers depuis votre telephone:" << '\n';
    std::cout << "1. Assurez-vous que votre telephone est sur le même reseau WiFi" << '\n';
    std::cout << "2. Ouvrez votre navigateur mobile" << '\n';
    std::cout << "3. Entrez l'adresse: http://[VOTRE_IP]:" << PORT << '\n';
    std::cout << "\nDossier de destination: " << server.GetUploadDirectory() << '\n';
    std::cout << "\nAppuyez sur Ctrl+C pour arrêter le serveur..." << '\n';
    std::cout << "=====================================================" << '\n';
    
    // Demarrage du serveur
    if (!server.Start()) {
        std::cerr << "Erreur lors du demarrage du serveur" << '\n';
        WSACleanup();
        return 1;
    }
    
    // Nettoyage
    WSACleanup();
    return 0;
}
