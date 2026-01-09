## ChessQt
### Description
ChessQt is a simple chess interface built with C++ and Qt. It features a chessboard where users can move pieces, experiment with 
test positions, and choose between a classic game or a free board practice mode.

![image](https://github.com/user-attachments/assets/a1ca5ed8-8d0a-43da-8fcb-fff60f5188a2)

### Building

**Prerequisites:** 
- Qt6 (or Qt5)
- CMake 3.16+

**Configure and build:**
```bash
cmake --preset debug
cmake --build build/debug
```

**With workflow preset (configure + build):**
```bash
cmake --workflow --preset debug
```

**Release build:**
```bash
cmake --workflow --preset release
```

**Run:**
```bash
./build/debug/ChessQt
```

**Format code:**
```bash
cd build/debug && make format
```
