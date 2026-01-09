# ChessQt

## Description

ChessQt is a simple chess interface built with C++ and Qt. It features a chessboard where users can move pieces, play chess and experiment with test positions.

## Demo

![ChessQt Screenshot](https://github.com/user-attachments/assets/a1ca5ed8-8d0a-43da-8fcb-fff60f5188a2)

## Features

- Basic chess rules implementation
- Piece and movement highlighting
- Practice mode (free piece movement)
- Pre made scenarios (test positions)

## Building

**Prerequisites:**
- Qt6 (or Qt5)
- CMake 3.16+
- C++17 compatible compiler

```bash
cmake --preset debug
cmake --build build/debug
./build/debug/ChessQt
```

For release builds:
```bash
cmake --workflow --preset release
```

## Project Structure

```
src/
├── board.cpp/h       # Game logic and state management
├── piece.h           # Abstract base piece class
├── king/queen/...    # Piece implementations
├── tempmove.h        # RAII helper for move simulation
└── gui/
    ├── mainwindow    # Main application window
    └── boardview     # Chess board rendering
```

> Uses `std::unique_ptr` for memory management and Qt's signal/slot architecture for UI updates.

## License

MIT License - see [LICENSE](LICENSE) for details.
