#include <qrencode.h>
#include <iostream>

int main() {
    const char* text = "http://www.banxico.org.mx";
    QRcode* qr = QRcode_encodeString(text, 0, QR_ECLEVEL_H, QR_MODE_8, 1);

    if (qr) {
            for (int y = 0; y < qr->width; y++) {
                for (int x = 0; x < qr->width; x++) {
	                std::cout << (qr->data[y * qr->width + x] & 1 ? "██" : "  ");
	            }
	            std::cout << std::endl;
            }
            QRcode_free(qr);
        } else {
	        std::cerr << "Failed to generate QR code!" << std::endl;
   	 }

    return 0;
}
