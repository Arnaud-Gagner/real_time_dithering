# DITHERING FILTER

---

TODO:
- create custom UI
- custom color(s)
- SIMD (at least grayscale)

---

## Building on Windows

build with MSVC:
> Git Bash:
```bash
rm -rf build;
mkdir build;
cd build;
cmake .. -G "Visual Studio 18 2026" -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake";
cd ..;
cmake --build build --config Release
```
