# Photos Transfer - Application de Transfert de Photos et Videos

## 📱➡️💻 Description

Cette application permet de transferer facilement des photos et videos depuis votre telephone mobile vers votre ordinateur via WiFi. Aucune application mobile n'est necessaire - tout se fait via le navigateur de votre telephone !

## ✨ Fonctionnalites

- ✅ Serveur HTTP integre
- ✅ Interface web responsive et moderne
- ✅ Support du glisser-deposer
- ✅ Transfert de multiples fichiers simultanes
- ✅ Barre de progression
- ✅ Support de tous les formats d'images (JPG, PNG, GIF, HEIC, etc.)
- ✅ Support de tous les formats videos (MP4, MOV, AVI, etc.)
- ✅ Pas d'installation necessaire sur le telephone
- ✅ Acces distant possible (via Internet)

## 🚀 Installation

### Prerequis

- Visual Studio 2022 (ou plus recent)
- Windows 10/11
- Un reseau WiFi local (ou connexion Internet pour acces distant)

### Compilation

1. Ouvrez le projet dans Visual Studio
2. Compilez en mode Debug ou Release (x64 recommande)
3. L'executable sera cree dans le dossier de sortie

## 📖 Utilisation

### Etape 1 : Demarrer le serveur sur votre PC

1. Lancez l'application `Photos_Transfer.exe`
2. Le serveur demarrera automatiquement sur le port 8080
3. Notez l'adresse IP affichee dans la console

Exemple d'affichage :
```
=== Application de Transfert de Photos et Videos ===
=====================================================

Adresses IP disponibles:
  - 192.168.1.100

Serveur demarre sur le port 8080

Pour transferer des fichiers depuis votre telephone:
1. Assurez-vous que votre telephone est sur le même reseau WiFi
2. Ouvrez votre navigateur mobile
3. Entrez l'adresse: http://192.168.1.100:8080

Dossier de destination: C:\Users\[Votre Nom]\...\Uploads

Appuyez sur Ctrl+C pour arrêter le serveur...
=====================================================
```

### etape 2 : Se connecter depuis votre telephone

1. **Assurez-vous que votre telephone est connecte au même reseau WiFi** que votre ordinateur
2. Ouvrez le navigateur de votre telephone (Chrome, Safari, Firefox, etc.)
3. Entrez l'adresse affichee par le serveur (ex: `http://192.168.1.100:8080`)

### etape 3 : Transferer vos fichiers

1. Sur la page web qui s'affiche, vous avez deux options :
   - **Cliquez** sur la zone de telechargement pour selectionner des fichiers
   - **Glissez-deposez** vos fichiers directement sur la zone

2. Une fois les fichiers selectionnes, cliquez sur **"Envoyer les fichiers"**

3. Une barre de progression s'affiche pendant le transfert

4. Les fichiers sont automatiquement sauvegardes dans le dossier `Uploads` à côte de l'executable

## 🌍 Acces Distant (Sans WiFi Local)

Pour acceder au serveur depuis n'importe où sur Internet (pas besoin d'être sur le même reseau WiFi), vous avez plusieurs options :

### Option 1 : Port Forwarding (Redirection de Port)

**Avantages** : Gratuit, permanent, contrôle total  
**Inconvenients** : Configuration technique requise

#### Etapes :

1. **Trouvez votre adresse IP locale** (deja affichee par l'application, ex: 192.168.1.100)

2. **Trouvez votre adresse IP publique** :
   - Allez sur https://www.whatismyip.com/
   - Notez l'adresse IP affichee (ex: 85.123.45.67)

3. **Configurez votre routeur** :
   - Connectez-vous à l'interface de votre routeur (generalement http://192.168.1.1 ou http://192.168.0.1)
   - Cherchez la section "Port Forwarding" ou "NAT"
   - Ajoutez une nouvelle règle :
     - **Port externe** : 8080
     - **Port interne** : 8080
     - **IP locale** : Votre IP locale (ex: 192.168.1.100)
     - **Protocole** : TCP
   - Sauvegardez et redemarrez le routeur si necessaire

4. **Configurez le pare-feu Windows** :
   - Panneau de configuration → Pare-feu Windows → Parametres avances
   - Créez une nouvelle règle entrante :
     - Type : Port
     - Protocole : TCP
     - Port : 8080
     - Action : Autoriser
     - Nom : Photos Transfer

5. **Accedez au serveur** :
   - Depuis n'importe où : `http://VOTRE_IP_PUBLIQUE:8080`
   - Exemple : `http://85.123.45.67:8080`

⚠️ **Important** : Votre IP publique peut changer (IP dynamique). Pour une solution permanente, utilisez un service DDNS (DynDNS, No-IP, etc.)

### Option 2 : Ngrok (Recommande pour usage temporaire)

**Avantages** : Simple, rapide, pas de configuration routeur  
**Inconvenients** : Necessite ngrok installe, URL change à chaque fois (version gratuite)

#### etapes :

1. **Telechargez ngrok** :
   - Allez sur https://ngrok.com/download
   - Telechargez et extraire ngrok.exe

2. **Creez un compte gratuit** sur https://ngrok.com/ (facultatif mais recommande)

3. **Demarrez votre serveur Photos Transfer**

4. **Lancez ngrok** (dans un autre terminal) :
   ```bash
   ngrok http 8080
   ```

5. **Notez l'URL fournie** :
   ```
   Forwarding   https://abc123.ngrok.io -> http://localhost:8080
   ```

6. **Partagez cette URL** :
   - Vous pouvez maintenant acceder à `https://abc123.ngrok.io` depuis n'importe où
   - Cette URL fonctionne même depuis un autre pays !

⚠️ **Note** : L'URL change à chaque redemarrage de ngrok (version gratuite). Version payante = URL permanente.

### Option 3 : Localtunnel (Alternative gratuite à ngrok)

**Avantages** : Gratuit, open-source, URL personnalisable  
**Inconvenients** : Necessite Node.js

#### etapes :

1. **Installez Node.js** depuis https://nodejs.org/

2. **Installez localtunnel** :
   ```bash
   npm install -g localtunnel
   ```

3. **Demarrez votre serveur Photos Transfer**

4. **Lancez localtunnel** :
   ```bash
   lt --port 8080 --subdomain mon-transfert-photos
   ```

5. **Accedez à l'URL** :
   - `https://mon-transfert-photos.loca.lt`

### Option 4 : Cloudflare Tunnel (Zero Trust)

**Avantages** : Securise, gratuit, professionnel  
**Inconvenients** : Configuration plus complexe

1. Installez Cloudflare Tunnel (cloudflared)
2. Suivez les instructions sur https://developers.cloudflare.com/cloudflare-one/connections/connect-apps/

### Comparaison des Options

| Option | Difficulte | Cout | Permanent | Securite |
|--------|-----------|------|-----------|----------|
| Port Forwarding | ⭐⭐⭐ | Gratuit | ✅ | ⚠️ HTTP non chiffre |
| Ngrok | ⭐ | Gratuit/Payant | ❌ (version gratuite) | ✅ HTTPS |
| Localtunnel | ⭐⭐ | Gratuit | ❌ | ✅ HTTPS |
| Cloudflare Tunnel | ⭐⭐⭐ | Gratuit | ✅ | ✅✅ HTTPS + Zero Trust |

## 📁 Structure du Projet

```
Photos_Transfer/
├── main.cpp              # Point d'entree de l'application
├── Server.h              # Definition de la classe Server
├── Server.cpp            # Implementation du serveur HTTP
├── FileHandler.h         # Definition de la classe FileHandler
├── FileHandler.cpp       # Gestion des fichiers
└── Uploads/              # Dossier cree automatiquement pour les fichiers reçus
```

## 🔧 Details Techniques

### Architecture

- **Serveur HTTP** : Implemente avec Winsock (ws2_32.lib)
- **Port par defaut** : 8080
- **Protocole** : HTTP/1.1
- **Format d'upload** : multipart/form-data

### Formats Supportes

#### Images
- JPG, JPEG, PNG, GIF, BMP, WEBP
- HEIC, HEIF (formats iPhone)
- TIFF, TIF, ICO, SVG

#### Videos
- MP4, MOV, AVI, WMV
- FLV, MKV, WEBM, M4V
- MPG, MPEG, 3GP, MTS, M2TS

## 🛡️ Securite

### ⚠️ Utilisation en Reseau Local (WiFi)

Pour une utilisation sur votre reseau WiFi local :
- ✅ Sûr : Votre reseau est prive
- ✅ Rapide : Pas de passage par Internet
- ✅ Gratuit : Aucun service externe

### ⚠️⚠️⚠️ Utilisation avec Acces Distant (Internet)

**ATTENTION** : Exposer ce serveur à Internet presente des risques de securite importants !

#### Risques :
1. **Pas de chiffrement** : Le serveur utilise HTTP (pas HTTPS), vos fichiers ne sont pas chiffres
2. **Pas d'authentification** : N'importe qui avec l'URL peut envoyer des fichiers
3. **Vulnerabilites** : Un serveur simple peut avoir des failles de securite
4. **Acces non autorise** : Des personnes malveillantes pourraient :
   - Envoyer des fichiers malveillants sur votre PC
   - Saturer votre disque dur
   - Tenter des attaques

#### Recommandations de Securite :

✅ **OBLIGATOIRE** :
- 🔒 Utilisez **ngrok, localtunnel ou Cloudflare Tunnel** qui fournissent HTTPS
- ⏱️ N'exposez le serveur que **temporairement** (le temps du transfert)
- 🚫 **Fermez immediatement** après utilisation
- 🔑 Ne partagez l'URL qu'avec les personnes de confiance
- 📱 Supprimez l'URL de votre historique après usage

✅ **FORTEMENT RECOMMANDE** :
- 🛡️ Ajoutez un mot de passe (modification du code necessaire)
- 📊 Surveillez les logs pour detecter toute activite suspecte
- 💾 Limitez la taille des fichiers acceptes
- 🚨 Installez un antivirus à jour sur votre PC

❌ **À EVITER** :
- Ne laissez **JAMAIS** le serveur expose 24/7
- N'utilisez **PAS** le port forwarding sans protection supplementaire
- Ne partagez **PAS** votre URL publiquement sur les reseaux sociaux
- N'acceptez **PAS** de fichiers de sources inconnues

#### Ameliorations de Securite Possibles (Code) :

Pour une version plus securisee, vous pouvez :
1. Ajouter une authentification par mot de passe
2. Implementer HTTPS avec des certificats SSL
3. Limiter le nombre de requêtes par IP
4. Ajouter une liste blanche d'adresses IP
5. Implementer un scan antivirus automatique
6. Ajouter une limite de taille de fichier

**Clause de non-responsabilite** : L'auteur n'est pas responsable des dommages causes par une utilisation non securisee de cette application.

## 🐛 Resolution des Problemes

### Le serveur ne demarre pas

- Verifiez qu'aucune autre application n'utilise le port 8080
- Assurez-vous que le pare-feu Windows autorise l'application
- Executez Visual Studio en tant qu'administrateur si necessaire

### Impossible de se connecter depuis le telephone

1. **Verifiez le reseau WiFi** : Le telephone et l'ordinateur doivent être sur le même reseau
2. **Verifiez l'adresse IP** : Utilisez exactement l'adresse affichee par le serveur
3. **Pare-feu** : Autorisez l'application dans le pare-feu Windows :
   - Panneau de configuration → Pare-feu Windows
   - Autoriser une application
   - Ajoutez `Photos_Transfer.exe`

### Les fichiers ne s'uploadent pas

- Verifiez l'espace disque disponible
- Assurez-vous que le dossier Uploads existe et est accessible
- Essayez avec des fichiers de plus petite taille d'abord

## 💡 Conseils d'Utilisation

1. **Performance** : Pour des gros fichiers, soyez patient - la vitesse depend de votre reseau WiFi
2. **Organisation** : Les fichiers sont sauvegardes avec leur nom d'origine - renommez-les avant l'upload si necessaire
3. **Lots** : Vous pouvez selectionner plusieurs fichiers à la fois pour un transfert groupe

## 📝 Logs

Le serveur affiche dans la console :
- L'heure de reception de chaque fichier
- Le nom du fichier
- La taille du fichier en octets

Exemple :
```
[2024-01-15 14:32:05] Fichier reçu: IMG_1234.jpg (2458392 octets)
[2024-01-15 14:32:12] Fichier reçu: VID_5678.mp4 (15847923 octets)
```

## 🔄 Mises à Jour Futures Possibles

- [ ] Support HTTPS pour plus de securite
- [ ] Authentification par mot de passe
- [ ] Organisation automatique par date
- [ ] Aperçu des fichiers avant transfert
- [ ] Support du telechargement depuis le PC vers le telephone
- [ ] Interface web pour parcourir les fichiers uploades
- [ ] Limitation de taille de fichier configurable
- [ ] Scan antivirus automatique des fichiers reçus
- [ ] Liste blanche d'adresses IP
- [ ] Rate limiting (limitation du nombre de requêtes)

## 📄 Licence

Ce projet est fourni tel quel pour usage personnel et educatif.

## 👨‍💻 Auteur

Developpe avec ❤️ pour faciliter le transfert de fichiers

---

**Note** : Cette application necessite Windows et Visual Studio pour être compilee. Pour d'autres plateformes, des adaptations seraient necessaires (notamment remplacer Winsock par une bibliotheque cross-platform).
