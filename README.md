## Требования

### Для сборки

- CMake 3.26+
- C++20
- `fmt`
- `nlohmann-json`
- 
````bash
apt install -y build-essential cmake libfmt-dev nlohmann-json3-dev dpkg-dev
````

## Сборка из исходников

```bash 
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr ..
cmake --build . -j$(nproc)
cmake --install .
```

## Сборка DEB

Пакет собирается через CPack:

```bash 
cd build cpack -G DEB
apt install -y ./journald-viewer_<версия>_amd64.deb
```