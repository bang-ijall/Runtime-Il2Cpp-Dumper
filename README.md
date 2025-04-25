# Runtime-Il2Cpp-Dumper
This repository contains the source code of the il2cpp dumper with runtime method

## Credit:
- Perfare: https://github.com/Perfare/Zygisk-Il2CppDumper

## How to use
### Magisk
- Install Magisk v24 or heigher and enable Zygisk option
- Install `zygisk-module.zip` in Magisk
- Add file `il2cpp-dumper.txt` into `/data/local/tmp/` directory
- Add package name into this file and continue with new line
- Start the game, `dump.cs` will be generated in the `/data/user/0/<package-name>/files/` directory

### Internal injection
- Download game apk file
- Merge the file (if `.xapk` or `.apks`) to apk and decompile the apk file
- Implement the library in folder `libs` and put this code to launcher activity

```smali
// start code
const-string v0, "JALL"
invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V
// end code
```

- Build to apk file
- Start the game, `dump.cs` will be generated in the `/data/user/0/<package-name>/files/` directory
