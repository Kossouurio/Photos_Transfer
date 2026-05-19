# Photos Transfer - File Transfer Application

## Description

This application allows you to easily transfer photos and videos from your mobile phone to your computer via WiFi. No mobile app is required - everything is done through your phone's browser!

## Features

- Integrated HTTP server
- Responsive and modern web interface
- Drag-and-drop support
- Multiple simultaneous file transfers
- Progress bar
- Support for all image formats (JPG, PNG, GIF, HEIC, etc.)
- Support for all video formats (MP4, MOV, AVI, etc.)
- No installation needed on the phone
- Remote access possible (via Internet)

## Installation

### Requirements

- Visual Studio 2022 (or newer)
- Windows 10/11
- Local WiFi network (or Internet connection for remote access)

### Compilation

1. Open the project in Visual Studio
2. Compile in Debug or Release mode (x64 recommended)
3. The executable will be created in the output folder

## Usage

### Step 1: Start the server on your PC

1. Launch the `Photos_Transfer.exe` application
2. The server will start automatically on port 8080
3. Note the IP address displayed in the console

Example output:
```
=== Photos Transfer Application ===
====================================

Available IP addresses:
  - 192.168.1.100

Server started on port 8080

To transfer files from your phone:
1. Make sure your phone is on the same WiFi network
2. Open your mobile browser
3. Enter the address: http://192.168.1.100:8080

Destination folder: C:\Users\[Your Name]\...\Uploads

Press Ctrl+C to stop the server...
====================================
```

### Step 2: Connect from your phone

1. **Make sure your phone is connected to the same WiFi network** as your computer
2. Open your phone's browser (Chrome, Safari, Firefox, etc.)
3. Enter the address displayed by the server (e.g., `http://192.168.1.100:8080`)

### Step 3: Transfer your files

1. On the web page that appears, you have two options:
   - **Click** on the upload area to select files
   - **Drag and drop** your files directly on the area

2. Once files are selected, click **"Send Files"**

3. A progress bar appears during the transfer

4. Files are automatically saved in the `Uploads` folder next to the executable

## Remote Access (Without Local WiFi)

To access the server from anywhere on the Internet (no need to be on the same WiFi network), you have several options:

### Option 1: Port Forwarding

**Advantages**: Free, permanent, full control
**Disadvantages**: Technical configuration required

#### Steps:

1. **Find your local IP address** (already displayed by the application, e.g., 192.168.1.100)

2. **Find your public IP address**:
   - Go to https://www.whatismyip.com/
   - Note the displayed IP address (e.g., 85.123.45.67)

3. **Configure your router**:
   - Connect to your router interface (usually http://192.168.1.1 or http://192.168.0.1)
   - Look for "Port Forwarding" or "NAT" section
   - Add a new rule:
     - **External Port**: 8080
     - **Internal Port**: 8080
     - **Local IP**: Your local IP (e.g., 192.168.1.100)
     - **Protocol**: TCP
   - Save and restart the router if necessary

4. **Configure Windows Firewall**:
   - Control Panel → Windows Firewall → Advanced Settings
   - Create a new inbound rule:
     - Type: Port
     - Protocol: TCP
     - Port: 8080
     - Action: Allow
     - Name: Photos Transfer

5. **Access the server**:
   - From anywhere: `http://YOUR_PUBLIC_IP:8080`
   - Example: `http://85.123.45.67:8080`

**Important**: Your public IP can change (dynamic IP). For a permanent solution, use a DDNS service (DynDNS, No-IP, etc.)

### Option 2: Ngrok (Recommended for temporary use)

**Advantages**: Simple, fast, no router configuration
**Disadvantages**: Requires ngrok installed, URL changes each time (free version)

#### Steps:

1. **Download ngrok**:
   - Go to https://ngrok.com/download
   - Download and extract ngrok.exe

2. **Create a free account** on https://ngrok.com/ (optional but recommended)

3. **Start your Photos Transfer server**

4. **Launch ngrok** (in another terminal):
   ```bash
   ngrok http 8080
   ```

5. **Note the provided URL**:
   ```
   Forwarding   https://abc123.ngrok.io -> http://localhost:8080
   ```

6. **Share this URL**:
   - You can now access `https://abc123.ngrok.io` from anywhere
   - This URL works even from another country!

**Note**: The URL changes each time ngrok restarts (free version). Paid version = permanent URL.

### Option 3: Localtunnel (Free alternative to ngrok)

**Advantages**: Free, open-source, customizable URL
**Disadvantages**: Requires Node.js

#### Steps:

1. **Install Node.js** from https://nodejs.org/

2. **Install localtunnel**:
   ```bash
   npm install -g localtunnel
   ```

3. **Start your Photos Transfer server**

4. **Launch localtunnel**:
   ```bash
   lt --port 8080 --subdomain my-photo-transfer
   ```

5. **Access the URL**:
   - `https://my-photo-transfer.loca.lt`

### Option 4: Cloudflare Tunnel (Zero Trust)

**Advantages**: Secure, free, professional
**Disadvantages**: More complex configuration

1. Install Cloudflare Tunnel (cloudflared)
2. Follow the instructions on https://developers.cloudflare.com/cloudflare-one/connections/connect-apps/

### Comparison of Options

| Option | Difficulty | Cost | Permanent | Security |
|--------|-----------|------|-----------|----------|
| Port Forwarding | Hard | Free | Yes | Warning: HTTP unencrypted |
| Ngrok | Easy | Free/Paid | No (free version) | HTTPS |
| Localtunnel | Medium | Free | No | HTTPS |
| Cloudflare Tunnel | Hard | Free | Yes | HTTPS + Zero Trust |

## Project Structure

```
Photos_Transfer/
├── main.cpp              # Application entry point
├── Server.h              # Server class definition
├── Server.cpp            # HTTP server implementation
├── FileHandler.h         # FileHandler class definition
├── FileHandler.cpp       # File handling
└── Uploads/              # Automatically created folder for received files
```

## Technical Details

### Architecture

- **HTTP Server**: Implemented with Winsock (ws2_32.lib)
- **Default Port**: 8080
- **Protocol**: HTTP/1.1
- **Upload Format**: multipart/form-data

### Supported Formats

#### Images
- JPG, JPEG, PNG, GIF, BMP, WEBP
- HEIC, HEIF (iPhone formats)
- TIFF, TIF, ICO, SVG

#### Videos
- MP4, MOV, AVI, WMV
- FLV, MKV, WEBM, M4V
- MPG, MPEG, 3GP, MTS, M2TS

## Security

### Local Network Usage (WiFi)

For use on your local WiFi network:
- Safe: Your network is private
- Fast: No Internet passage
- Free: No external services

### WARNING: Remote Access (Internet Usage)

**ATTENTION**: Exposing this server to the Internet presents significant security risks!

#### Risks:
1. **No encryption**: The server uses HTTP (not HTTPS), your files are not encrypted
2. **No authentication**: Anyone with the URL can send files
3. **Vulnerabilities**: A simple server can have security flaws
4. **Unauthorized access**: Malicious people could:
   - Send malicious files to your PC
   - Fill up your hard drive
   - Attempt attacks

#### Security Recommendations:

MANDATORY:
- Use **ngrok, localtunnel or Cloudflare Tunnel** which provide HTTPS
- Only expose the server **temporarily** (during transfer time)
- **Close immediately** after use
- Only share the URL with trusted people
- Delete the URL from your history after use

STRONGLY RECOMMENDED:
- Add a password (code modification necessary)
- Monitor logs to detect any suspicious activity
- Limit uploaded file size
- Install an up-to-date antivirus on your PC

AVOID:
- Never leave the server exposed 24/7
- Do not use port forwarding without additional protection
- Do not share your URL publicly on social media
- Do not accept files from unknown sources

#### Possible Security Improvements (Code):

For a more secure version, you can:
1. Add password authentication
2. Implement HTTPS with SSL certificates
3. Limit the number of requests per IP
4. Add an IP whitelist
5. Implement automatic antivirus scanning
6. Add a file size limit

**Disclaimer**: The author is not responsible for damage caused by unsafe use of this application.

## Troubleshooting

### Server does not start

- Check that no other application is using port 8080
- Make sure Windows Firewall allows the application
- Run Visual Studio as administrator if necessary

### Cannot connect from phone

1. **Check WiFi network**: Phone and computer must be on the same network
2. **Check IP address**: Use exactly the address displayed by the server
3. **Firewall**: Allow the application in Windows Firewall:
   - Control Panel → Windows Firewall
   - Allow an app
   - Add `Photos_Transfer.exe`

### Files do not upload

- Check available disk space
- Make sure the Uploads folder exists and is accessible
- Try with smaller files first

## Usage Tips

1. **Performance**: For large files, be patient - speed depends on your WiFi network
2. **Organization**: Files are saved with their original name - rename them before upload if necessary
3. **Batch**: You can select multiple files at once for batch transfer

## Logs

The server displays in the console:
- Reception time of each file
- File name
- File size in bytes

Example:
```
[2024-01-15 14:32:05] File received: IMG_1234.jpg (2458392 bytes)
[2024-01-15 14:32:12] File received: VID_5678.mp4 (15847923 bytes)
```

## Future Possible Updates

- Support for HTTPS for more security
- Password authentication
- Automatic organization by date
- File preview before transfer
- Support for downloading from PC to phone
- Web interface to browse uploaded files
- Configurable file size limit
- Automatic antivirus scanning of received files
- IP whitelist
- Rate limiting (request limit)

## License

This project is provided as-is for personal and educational use.

## Author

Developed with passion to facilitate file transfer

---

**Note**: This application requires Windows and Visual Studio to be compiled. For other platforms, adaptations would be necessary (particularly replacing Winsock with a cross-platform library).

