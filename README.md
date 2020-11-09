# Stegosaurus

This is a steganography engine that encodes binary data into an image file (specifically in PNG format).

## Building

Run PremakeScript.bat and build StegosaurusApplication with Visual Studio 2019.

There are two main projects: `StegosaurusEngine` and `StegosaurusApplication`. The engine is compiled into a `.lib` file that you could use in your own program. The Application project is a command line tool that takes user input and uses the engine library to encode and decode information to and from images.

The Engine `.lib` file will be in `bin/[Debug | Release]/windows-x86/StegosaurusEngine/`

The Application .exe file will be in `bin/[Debug | Release]/windows-x86/StegosaurusApplication/`

## StegApp Usage

```
./steg encode [Encoding Options] [Encryption Options] -I <input image> -D <data file> -O <output image>"
./steg decode [Decryption Options] -I <input image> -D <data file>"
./steg <-h | --help>   Prints this help message"

Encoding Options:
  -a  --use-alpha                Encode data in alpha channel (Default: False)
  -d  --depth <depth>            Bit depth of hidden data (Default: 2)
Encryption Options:
  -p  --pass <password>          Encryption password
  -g  --algo <algorithm>         Encryption algorithm (AES128, AES192, AES256) (Default: AES128)
Decryption Options:
  -p  --pass <password>          Decryption password (same as encryption password)
Other:
  -I  --in-image <filename>      The image to hide data in
  -D  --data-file <filename>     Binary file that contains the data to be hidden
  -O  --out-image <filename>     The resulting image
```

### Examples:

Encode the data in `data.bin` into `in.png` and stores the resulting image in `out.png`:

    ./steg encode -I "in.png" -D "data.bin" -O "out.png"


Same as above, but also encode in alpha, set depth to 4 bits, and encrypt payload with a password using AES192:

    ./steg encode -a -d 4 -p "pass123" -g AES192 -I "in.png" -D "data.bin" -O "out.png"


Decode `in.png` and stores the resulting data in `data.bin`:

    ./steg decode -I "in.png" -D "data.bin"


Same as above, but decrypt using a password. The algorithm is automatically detected:
    
    ./steg decode -p "pass123" -I "in.png" -D "data.bin"

## License
[MIT](https://choosealicense.com/licenses/mit)