#include <qrencode.h>
#include <iostream>

int main() {
    const char* text = "Lampara-Luna,Lampara de luna con bombilla de 14w,sku:010101010101,pi:150MXN";
    QRcode* qr = QRcode_encodeString(text, 0, QR_ECLEVEL_H, QR_MODE_8, 1);

    if (qr) {
            for (int y = 0; y < qr->width; y++) {
                for (int x = 0; x < qr->width; x++) {
	                std::cout << (qr->data[y * qr->width + x] & 1 ? "1" : "0");
	            }
            }
            QRcode_free(qr);
        } else {
	        std::cerr << "Failed to generate QR code!" << std::endl;
   	 }

    return 0;
}
