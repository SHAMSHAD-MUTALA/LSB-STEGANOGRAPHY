# LSB-STEGANOGRAPHY

## Overview
This project implements **Least Significant Bit (LSB) Steganography** to hide and extract secret messages within image files. The technique modifies the least significant bits of pixel values in an image to store hidden data while keeping the visual quality unchanged.

## Features
- **Encoding:** Hide a secret text file inside a BMP image.
- **Decoding:** Extract the hidden message from the modified image.
- **Minimal Distortion:** LSB modifications have negligible effects on the image.

## Usage
### 1. Compilation
Before running the program, compile the source code using GCC:
```bash
gcc encode.c decode.c -o lsb_steg
```

### 2. Encoding a Message
```bash
./lsb_steg -e beautiful.bmp secret.txt output.bmp
```
- `beautiful.bmp`: Original image file
- `secret.txt`: Text file containing the hidden message
- `output.bmp`: New image with embedded message

### 3. Decoding a Message
If you have the encoded image, extract the hidden message:
```bash
./lsb_steg -d output.bmp decoded.txt
```
- `output.bmp`: Image containing the hidden data
- `decoded.txt`: Extracted secret message

## File Structure
```
LSB_STEGANOGRAPHY/
│── .vscode/                 # VS Code settings (if applicable)
│── LSB_PROJECT/
│   ├── 1-References/        # Reference materials
│   ├── 2-OutputImages/      # Output images from steganography
│   ├── 3-Design/            # Design-related files (if applicable)
│   ├── 4-SkeletonCode/      # Core implementation files
│   │   ├── a.out            # Compiled executable (if using default GCC output)
│   │   ├── beautiful.bmp    # Original image for encoding
│   │   ├── common.h         # Common header file
│   │   ├── decode.c         # Decoding module
│   │   ├── decode.h         # Header for decoding functions
│   │   ├── decoded.txt      # Extracted secret message file
│   │   ├── encode.c         # Encoding module
│   │   ├── encode.h         # Header for encoding functions
│   │   ├── lsb_steg         # Compiled executable
│   │   ├── output.bmp       # Image after encoding
│   │   ├── secret.txt       # Secret text file to embed
│   │   ├── stego.bmp        # Another encoded image (if applicable)
│   │   ├── test_encode.c    # Test file for encoding
│   │   ├── types.h          # Type definitions
```

### **Command-Line Usage:**
#### **Encoding (Hiding a Secret Message)**
```bash
./a.out -e input.bmp secret.txt output.bmp
```
- `./a.out` → Executable file  
- `-e` → Encode mode (hide data inside an image)  
- `input.bmp` → Original BMP image  
- `secret.txt` → Text file containing the hidden message  
- `output.bmp` → Output BMP image with the embedded secret message  

#### **Decoding (Extracting the Hidden Message)**
```bash
./a.out -d output.bmp decoded.txt
```
- `./a.out` → Executable file  
- `-d` → Decode mode (extract hidden message)  
- `output.bmp` → Image containing the hidden message  
- `decoded.txt` → Extracted message saved to this file  

## Notes
- Ensure the input image is in BMP format, as LSB manipulation may not work with compressed formats like JPEG or PNG.
- The decoding process requires the exact encoded image to retrieve the original message.

## Future Improvements
- Support for multiple image formats.
- GUI-based implementation for easier usage.
- Encryption for enhanced security before embedding the message.

---
**Author:** Shamshad Mutala

