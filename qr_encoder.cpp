#include "qrencode.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <functional>
#include <sstream>

// Simple PPM writer (more reliable than PNG for this purpose)
class PPMWriter {
public:
    static bool savePPM(const std::string& filename, const std::vector<unsigned char>& data, int width, int height) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        
        // Write PPM header
        file << "P3" << std::endl;
        file << width << " " << height << std::endl;
        file << "255" << std::endl;
        
        // Write image data
        for (int i = 0; i < data.size(); i += 3) {
            file << static_cast<int>(data[i]) << " "
                 << static_cast<int>(data[i + 1]) << " "
                 << static_cast<int>(data[i + 2]) << " ";
            
            // Add newline every 5 pixels for readability
            if ((i / 3) % 5 == 4) {
                file << std::endl;
            }
        }
        
        return true;
    }
};

class QREncoder {
private:
    int moduleSize;
    int margin;
    QRecLevel errorLevel;
    int version;
    
public:
    QREncoder(int moduleSize = 4, int margin = 4, QRecLevel errorLevel = QR_ECLEVEL_L, int version = 0) 
        : moduleSize(moduleSize), margin(margin), errorLevel(errorLevel), version(version) {}
    
    bool encodeToImage(const std::string& text, const std::string& outputFile) {
        // Encode the string to QR code
        QRcode* qr = QRcode_encodeString(text.c_str(), version, errorLevel, QR_MODE_8, 1);
        
        if (!qr) {
            std::cerr << "Failed to generate QR code!" << std::endl;
            return false;
        }
        
        // Calculate image dimensions
        int qrWidth = qr->width;
        int imageWidth = (qrWidth + 2 * margin) * moduleSize;
        int imageHeight = imageWidth;
        
        // Create image data (RGB format)
        std::vector<unsigned char> imageData(imageWidth * imageHeight * 3);
        
        // Fill with white background
        for (int i = 0; i < imageData.size(); i += 3) {
            imageData[i] = 255;     // R
            imageData[i + 1] = 255; // G
            imageData[i + 2] = 255; // B
        }
        
        // Draw QR code
        for (int y = 0; y < qrWidth; y++) {
            for (int x = 0; x < qrWidth; x++) {
                bool isBlack = (qr->data[y * qrWidth + x] & 1);
                
                // Calculate pixel position in image
                int startX = (x + margin) * moduleSize;
                int startY = (y + margin) * moduleSize;
                
                // Fill module with color
                for (int dy = 0; dy < moduleSize; dy++) {
                    for (int dx = 0; dx < moduleSize; dx++) {
                        int pixelX = startX + dx;
                        int pixelY = startY + dy;
                        
                        if (pixelX >= 0 && pixelX < imageWidth && pixelY >= 0 && pixelY < imageHeight) {
                            int index = (pixelY * imageWidth + pixelX) * 3;
                            if (isBlack) {
                                imageData[index] = 0;     // R
                                imageData[index + 1] = 0; // G
                                imageData[index + 2] = 0; // B
                            }
                        }
                    }
                }
            }
        }
        
        // Save as PPM
        bool success = PPMWriter::savePPM(outputFile, imageData, imageWidth, imageHeight);
        
        // Clean up
        QRcode_free(qr);
        
        if (success) {
            std::cout << "QR code saved to: " << outputFile << std::endl;
            std::cout << "Image size: " << imageWidth << "x" << imageHeight << " pixels" << std::endl;
            std::cout << "QR code version: " << qrWidth << "x" << qrWidth << " modules" << std::endl;
        } else {
            std::cerr << "Failed to save image to: " << outputFile << std::endl;
        }
        
        return success;
    }
    
    // Alternative method using PPM format (simpler)
    bool encodeToPPM(const std::string& text, const std::string& outputFile) {
        // Encode the string to QR code
        QRcode* qr = QRcode_encodeString(text.c_str(), version, errorLevel, QR_MODE_8, 1);
        
        if (!qr) {
            std::cerr << "Failed to generate QR code!" << std::endl;
            return false;
        }
        
        // Calculate image dimensions
        int qrWidth = qr->width;
        int imageWidth = (qrWidth + 2 * margin) * moduleSize;
        int imageHeight = imageWidth;
        
        // Open PPM file
        std::ofstream file(outputFile);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << outputFile << std::endl;
            QRcode_free(qr);
            return false;
        }
        
        // Write PPM header
        file << "P3" << std::endl;
        file << imageWidth << " " << imageHeight << std::endl;
        file << "255" << std::endl;
        
        // Generate image data
        for (int y = 0; y < imageHeight; y++) {
            for (int x = 0; x < imageWidth; x++) {
                // Calculate QR module position
                int moduleX = (x / moduleSize) - margin;
                int moduleY = (y / moduleSize) - margin;
                
                if (moduleX >= 0 && moduleX < qrWidth && moduleY >= 0 && moduleY < qrWidth) {
                    bool isBlack = (qr->data[moduleY * qrWidth + moduleX] & 1);
                    if (isBlack) {
                        file << "0 0 0 "; // Black
                    } else {
                        file << "255 255 255 "; // White
                    }
                } else {
                    file << "255 255 255 "; // White margin
                }
            }
            file << std::endl;
        }
        
        file.close();
        QRcode_free(qr);
        
        std::cout << "QR code saved to: " << outputFile << std::endl;
        std::cout << "Image size: " << imageWidth << "x" << imageHeight << " pixels" << std::endl;
        std::cout << "QR code version: " << qrWidth << "x" << qrWidth << " modules" << std::endl;
        
        return true;
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <text> [output_file] [module_size] [margin] [error_level]" << std::endl;
        std::cout << "  text: The string to encode" << std::endl;
        std::cout << "  output_file: Output image file (default: qrcode.ppm)" << std::endl;
        std::cout << "  module_size: Size of each QR module in pixels (default: 4)" << std::endl;
        std::cout << "  margin: Margin around QR code in modules (default: 4)" << std::endl;
        std::cout << "  error_level: Error correction level L/M/Q/H (default: L)" << std::endl;
        std::cout << std::endl;
        std::cout << "Example: " << argv[0] << " \"Hello, World!\" qrcode.ppm 8 4 H" << std::endl;
        return 1;
    }
    
    std::string text = argv[1];
    std::string outputFile = (argc > 2) ? argv[2] : "qrcode.ppm";
    int moduleSize = (argc > 3) ? std::stoi(argv[3]) : 4;
    int margin = (argc > 4) ? std::stoi(argv[4]) : 4;
    
    QRecLevel errorLevel = QR_ECLEVEL_L;
    if (argc > 5) {
        char level = argv[5][0];
        switch (level) {
            case 'L': errorLevel = QR_ECLEVEL_L; break;
            case 'M': errorLevel = QR_ECLEVEL_M; break;
            case 'Q': errorLevel = QR_ECLEVEL_Q; break;
            case 'H': errorLevel = QR_ECLEVEL_H; break;
            default:
                std::cerr << "Invalid error level. Use L, M, Q, or H." << std::endl;
                return 1;
        }
    }
    
    QREncoder encoder(moduleSize, margin, errorLevel);
    
    // Determine file format from extension
    std::string extension = outputFile.substr(outputFile.find_last_of(".") + 1);
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
    
    bool success;
    if (extension == "png") {
        success = encoder.encodeToImage(text, outputFile);
    } else {
        success = encoder.encodeToPPM(text, outputFile);
    }
    
    return success ? 0 : 1;
} 