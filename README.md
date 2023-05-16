# bswap

utility to swap between number bases

```
usage: bswap [option] [target base] [input number]
    --version   print out version
```

input number may be precceded by:
   0x - hexadecimal
   0b - binary
   0o - octal
   0* - decimal

if none of these are present, the number is assumed to be decimal.
pads non-decimal digits in decimal numbers with 0s.

### Install

either clone directly from the repository or grab one of the [releases](https://github.com/rarafael/bswap/releases/tag/1.0.0)

```
git clone https://github.com/rarafael/bswap
cd bswap
make
sudo make install
```

### Uninstall
    
```
cd bswap
sudo make uninstall
```
