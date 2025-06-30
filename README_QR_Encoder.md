# QR Encoder

A simple C++ application that generates QR code images from text strings using the libqrencode library.

## Features

- Generate QR codes from any text string
- Customizable module size and margins
- Multiple error correction levels (L, M, Q, H)
- Output in PPM format (easily convertible to other formats)
- Command-line interface for easy integration

## Prerequisites

You need to have the libqrencode library installed. The library is already included in this project, so you just need to build it first.

### Building libqrencode

```bash
# Configure and build libqrencode
./autogen.sh
./configure
make
sudo make install
```

## Building the QR Encoder

### Option 1: Using Makefile (Recommended)

```bash
# Build the QR encoder
make -f Makefile_qr_encoder

# Or simply
make -f Makefile_qr_encoder all
```

### Option 2: Using CMake

```bash
# Create build directory
mkdir build
cd build

# Configure and build
cmake -f ../CMakeLists_qr_encoder.txt ..
make
```

### Option 3: Manual compilation

```bash
g++ -std=c++11 -Wall -Wextra -O2 qr_encoder.cpp -o qr_encoder -lqrencode
```

## Usage

### Basic Usage

```bash
./qr_encoder "Your text here" output.ppm
```

### Advanced Usage

```bash
./qr_encoder <text> [output_file] [module_size] [margin] [error_level]
```

### Parameters

- `text`: The string to encode in the QR code
- `output_file`: Output image file (default: qrcode.ppm)
- `module_size`: Size of each QR module in pixels (default: 4)
- `margin`: Margin around QR code in modules (default: 4)
- `error_level`: Error correction level L/M/Q/H (default: L)

### Error Correction Levels

- **L (Low)**: 7% recovery capacity
- **M (Medium)**: 15% recovery capacity
- **Q (Quartile)**: 25% recovery capacity
- **H (High)**: 30% recovery capacity

### Examples

```bash
# Basic QR code
./qr_encoder "Hello, World!" qrcode.ppm

# Large QR code with high error correction
./qr_encoder "https://www.example.com" example.ppm 8 4 H

# Small QR code with minimal margin
./qr_encoder "Test" test.ppm 4 2 L

# URL with medium error correction
./qr_encoder "https://github.com" github.ppm 6 4 M
```

## Output Format

The QR encoder generates PPM (Portable Pixmap) format images, which are:

- **Human-readable**: ASCII text format
- **Widely supported**: Can be opened by most image viewers
- **Convertible**: Easy to convert to PNG, JPEG, or other formats

### Converting PPM to other formats

You can convert the PPM files to other formats using ImageMagick:

```bash
# Convert to PNG
convert qrcode.ppm qrcode.png

# Convert to JPEG
convert qrcode.ppm qrcode.jpg

# Convert to SVG (vector format)
convert qrcode.ppm qrcode.svg
```

Or using GIMP, Photoshop, or any other image editor.

## Testing

Run the built-in tests:

```bash
make -f Makefile_qr_encoder test
```

This will generate three test QR codes:
- `test_qrcode.ppm` - "Hello, World!" with high error correction
- `example_qrcode.ppm` - URL with medium error correction
- `test2_qrcode.ppm` - Simple text with low error correction

## Installation

Install the QR encoder system-wide:

```bash
make -f Makefile_qr_encoder install
```

Then you can use it from anywhere:

```bash
qr_encoder "Your text" output.ppm
```

## Troubleshooting

### Common Issues

1. **"libqrencode library not found"**
   - Make sure libqrencode is built and installed
   - Run `sudo ldconfig` after installation

2. **"qrencode.h header not found"**
   - Check that the header is in the include path
   - You may need to specify the include path manually

3. **Compilation errors**
   - Ensure you're using C++11 or later
   - Check that all required headers are available

### Manual Include Path

If the headers aren't found automatically, you can specify them manually:

```bash
g++ -std=c++11 -I. qr_encoder.cpp -o qr_encoder -L. -lqrencode
```

## API Usage

You can also use the `QREncoder` class in your own C++ projects:

```cpp
#include "qr_encoder.cpp" // Include the implementation

QREncoder encoder(8, 4, QR_ECLEVEL_H); // module_size=8, margin=4, error_level=H
bool success = encoder.encodeToPPM("Your text", "output.ppm");
```

## License

This QR encoder uses the libqrencode library, which is licensed under the GNU Lesser General Public License v2.1.

## Contributing

Feel free to submit issues, feature requests, or pull requests to improve the QR encoder. 