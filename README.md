# DataPlay
This is a sample code project for playing with data structures!

```
cd DataPlay

vcpkg integrate install
vcpkg install

mkdir build
cd build/

cmake -DCMAKE_TOOLCHAIN_FILE=VCPKG_ROOT/vcpkg/scripts/buildsystems/vcpkg.cmake ..

cmake --build .

./DataPlay ../Data/data.json 
```