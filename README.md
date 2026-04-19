## journald-viewer

`journald-viewer` — утилита для красивого вывода логов `journald` в терминал.

Она читает входные данные в формате JSON по одной записи на строку, форматирует время, хост, идентификатор процесса и текст сообщения, а также подсвечивает сообщения по уровню приоритета.

### Как это работает
![work](video/journald-viewer.gif)

### Пример использования
````bash
journalctl --since="2026-04-13 23:50:53" --until="2026-04-13 23:54" --output=json | /usr/bin/journald-viewer | less
````


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