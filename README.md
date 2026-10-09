# Sudoku (Qt 6 Widgets)

> This project has been created as part of the YSU GUI course curriculum by Nane Hovhikyan.

A desktop Sudoku game written in C++17 with the Qt 6 Widgets framework. The player fills in a 9×9 board using on-screen number buttons, can delete entries, and can check the board against a stored solution. A timer tracks how long the current game has been running, and result dialogs show custom images depending on the outcome.

---

## Table of Contents

1. [Features](#features)
2. [How to Play](#how-to-play)
3. [Project Structure](#project-structure)
4. [Requirements](#requirements)
5. [Building and Running](#building-and-running)
6. [Resources](#resources)
7. [Code Overview](#code-overview)
8. [Adding Your Own Puzzles](#adding-your-own-puzzles)
9. [Known Issues and Limitations](#known-issues-and-limitations)
10. [Possible Improvements](#possible-improvements)

---

## Features

- **9×9 interactive board** built from `QPushButton` cells, laid out in a `QGridLayout` with extra spacing to visually separate the nine 3×3 boxes.
- **Pre-filled cells are locked.** Given numbers are disabled and shown in a darker beige so they cannot be changed.
- **Cell selection** with highlight. Clicking a selected cell again deselects it.
- **On-screen number pad (1–9)** for entering values into the selected cell.
- **Delete button** to clear the selected cell.
- **Check Solution button** with three possible outcomes:
  - *Incomplete* – the board still has empty cells.
  - *Victory!* – the board matches the stored solution (also shows the elapsed time).
  - *Keep Trying* – the board is full but differs from the stored solution.
- **Custom result dialogs** with an image for each outcome and the buttons **OK**, **New Game** and **Retry**.
- **Game timer** shown as `Time: MM:SS`, updated every second. The timer pauses while a result dialog is open.
- **Three built-in puzzles** that cycle each time a new game is started (0 → 1 → 2 → 0 …).
- **Custom colour theme** applied with Qt style sheets (soft beige and green palette).

---

## How to Play

1. Launch the application. The first puzzle is loaded and the timer starts.
2. Click an empty (editable) cell to select it. It is highlighted in green.
3. Click a number button (1–9) to place that number in the selected cell.
4. Use **Delete** to clear the selected cell.
5. When you think you are done, click **Check Solution**.
6. In the result dialog you can:
   - **OK** – close the dialog and keep playing.
   - **New Game** – load the next puzzle and reset the timer.
   - **Retry** – reload the *current* puzzle from scratch and reset the timer.
7. The **New Game** button on the main window loads the next puzzle at any time.

---

## Project Structure

```
.
├── CMakeLists.txt     # Build configuration (Qt 6, C++17, AUTOMOC)
├── main.cpp           # Application entry point
├── sudoku.h           # MainWindow class declaration
├── sudoku.cpp         # MainWindow implementation (UI + game logic)
├── puzzles.h          # Sample puzzles and their solutions
└── resources/         # Images used in result dialogs
    ├── thinking.jpg
    ├── happy.jpg
    └── dissapointed.png
```

---

## Requirements

| Tool      | Version                     |
|-----------|-----------------------------|
| C++       | C++17-capable compiler (GCC, Clang or MSVC) |
| CMake     | 3.16 or newer               |
| Qt        | Qt 6 with the **Widgets** module |

---

## Building and Running

### From the command line

```bash
# 1. Configure (adjust the Qt path to your installation if CMake cannot find it)
cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/<kit>

# 2. Build
cmake --build build

# 3. Run (from the build directory so that the resources folder is found)
cd build
./Sudoku            # Linux / macOS
Sudoku.exe          # Windows
```

### From Qt Creator

1. Open `CMakeLists.txt` via **File → Open File or Project**.
2. Select a Qt 6 kit and click **Configure Project**.
3. Press **Run**.

> **Important:** The `resources` folder is copied next to the executable at configure time (`file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})`). Images are loaded with relative paths such as `resources/happy.jpg`, so the program must be started with the build directory as its working directory. Otherwise the dialog images will not appear.

---

## Resources

Result dialogs use the following images (200×200 max, scaled with aspect ratio preserved):

| Outcome     | File                         |
|-------------|------------------------------|
| Incomplete  | `resources/thinking.jpg`     |
| Victory     | `resources/happy.jpg`        |
| Wrong board | `resources/dissapointed.png` |

The file name `dissapointed.png` is spelled this way in the code; if you rename the file, update `sudoku.cpp` too.

---

## Code Overview

### `main.cpp`
Creates the `QApplication`, instantiates `MainWindow`, shows it and starts the event loop.

### `sudoku.h` / `sudoku.cpp`

`MainWindow` derives from `QWidget` and is organised into clearly separated setup steps called from the constructor:

| Method | Responsibility |
|--------|----------------|
| `createWidgets()` | Allocates layouts, 81 cell buttons, 9 number buttons, Delete / New Game / Check buttons, timer label and `QTimer`; initialises state. |
| `makeWidgetsLayout()` | Places the cells in the grid (inserting spacer rows/columns between 3×3 boxes), arranges the number row and bottom button row, applies the window style sheet. |
| `makeConnections()` | Connects button clicks and the timer to their slots and starts the 1-second timer. |
| `loadNewGame(bool flag)` | `false` reloads the current puzzle; `true` advances to the next puzzle (cyclic) and loads it. |
| `loadPuzzleIntoBoard(...)` | Copies a puzzle and its solution into `playerBoard` / `solutionBoard` and updates every cell button (text, enabled state, colour). |
| `handleCellClicked(row, col)` | Handles selection, deselection and highlight. |
| `handleNumberClicked(n)` | Writes `n` into the selected cell and the `playerBoard` array. |
| `handleDeleteButtonClicked()` | Clears the selected cell. |
| `handleCheckButtonClicked()` | Checks completeness and correctness and shows the appropriate dialog. |
| `showGameMessage(...)` | Stops the timer, shows a styled `QMessageBox` with a custom icon and OK / New Game / Retry buttons, handles the choice, then restarts the timer. |
| `updateTimer()` | Increments `secondsPassed` and updates the `MM:SS` label. |

### Data model
- `playerBoard[9][9]` – current values on the board (0 = empty).
- `solutionBoard[9][9]` – the stored solution for the current puzzle.
- `cellButtons[9][9]` – the button widgets representing cells.
- `selectedRow` / `selectedCol` – currently selected cell, or `-1` when nothing is selected.

### `puzzles.h`
Defines `PUZZLE_COUNT`, `SAMPLE_PUZZLES` and `SAMPLE_SOLUTIONS`, both indexed as `[puzzle][row][col]`, where `0` means an empty cell.

### `CMakeLists.txt`
Builds the `Sudoku` executable, enables `AUTOMOC` (needed for `Q_OBJECT`), links `Qt6::Widgets` and copies the `resources` folder into the build directory.

---

## Adding Your Own Puzzles

1. Open `puzzles.h`.
2. Increase `PUZZLE_COUNT` by one.
3. Append a new 9×9 grid to `SAMPLE_PUZZLES` (use `0` for empty cells).
4. Append the matching, fully solved 9×9 grid to `SAMPLE_SOLUTIONS` at the same index.
5. Rebuild.

Make sure the solution is a valid Sudoku and that every given in the puzzle appears in the same position in the solution, since the game compares the player's board with the stored solution cell by cell.

---

## Known Issues and Limitations

- **Solutions for puzzles 2 and 3 in `puzzles.h` look incorrect.** Several rows contain duplicate digits (for example the second puzzle's solution row 7 contains two `1`s, and the third puzzle's solution row 2 contains two `7`s), and some givens do not match the solution at the same position. As a result, these puzzles can never be reported as solved. Puzzle 1 (the classic example) is correct. These two solutions need to be regenerated or corrected.
- **Correctness is checked against the stored solution only**, not against Sudoku rules, so a valid alternative solution would be rejected (not an issue for puzzles with a unique solution).
- **No error highlighting.** *Check Solution* only reports a general result.
- **No keyboard input.** Numbers can only be entered with the on-screen buttons.
- **Relative image paths.** Dialog images are found only if the working directory contains the `resources` folder.
- **Timer label after Retry/New Game.** `secondsPassed` is reset to 0, but the label updates on the next timer tick.
- **Unused members.** `puzzleBank`, `solutionBank` and the `QSoundEffect` forward declaration in `sudoku.h` are not used yet.
- **Typo in a message.** "mistaked" in the *Keep Trying* dialog should read "mistakes".

---

## Possible Improvements

- Fix and verify the stored solutions (or add a solver / puzzle generator).
- Highlight conflicting numbers in rows, columns and boxes.
- Add keyboard support (digits, arrow keys, Backspace).
- Add difficulty levels and a larger puzzle bank.
- Add pencil-mark notes, hints and undo/redo.
- Add sound effects (a `QSoundEffect` forward declaration is already present).
- Load images via the Qt Resource System (`.qrc`) so they are embedded in the executable.
- Save the best times.

---

## Author

Nane Hovhikyan, Yerevan State University (YSU), GUI course.