# DataPlay
This is a sample code project for playing with data structures!

Data came from prompts to Microsoft Copilot AI.

```Please give me 50 famous quotes from 8 to 30 words long, formatted in json like: {"quote": QUOTE, "author": AUTHOR}```

Requires json-cpp package.

Install, build and run at command line with options:

```
cd DataPlay

vcpkg integrate install
vcpkg install

mkdir build
cd build/

cmake -DCMAKE_TOOLCHAIN_FILE=VCPKG_ROOT/vcpkg/scripts/buildsystems/vcpkg.cmake ..

cmake --build .

DataPlay -file Data/crytoquip.json
DataPlay -file Data/data.json -choices 7
DataPlay -file Data/data.json -noduplicates name -ignorecase -contains
DataPlay -file Data/wordpuzzle.json -filter difficulty 2
```