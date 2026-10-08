#include "sudoku.h"

#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QMessageBox>
#include <QLabel>


MainWindow::MainWindow(QWidget* parent) : QWidget(parent)
{
    createWidgets();
    makeWidgetsLayout();
    makeConnections();
    loadNewGame();
}


void MainWindow::createWidgets()
{
    selectedRow = -1;
    selectedCol = -1;
    secondsPassed = 0;
    currentPuzzleIndex = 0;

    mainLayout = new QVBoxLayout(this);
    gridLayout = new QGridLayout;
    numberLayout = new QHBoxLayout;
    bottomLayout = new QHBoxLayout;

    for (int row = 0; row < 9; row++)
    {
        for(int col = 0; col < 9; col++)
        {
            playerBoard[row][col] = 0;
            solutionBoard[row][col] = 0;
        }
    }

    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            cellButtons[row][col] = new QPushButton("", this);
            cellButtons[row][col]->setFixedSize(45, 45);
        }
    }

    for (int i = 0; i < 9; ++i) {
        numButtons[i] = new QPushButton(QString::number(i+1), this);
        numButtons[i]->setFixedSize(40, 40);
        numButtons[i]->setStyleSheet("background-color: #C2F7A6; color: #000E59");
    }
    undoButton = new QPushButton("Undo", this);
    newGameButton = new QPushButton("New Game", this);

    timerLabel = new QLabel("Time: 00:00", this);
    timerLabel->setAlignment(Qt::AlignCenter);

    checkButton = new QPushButton("Check Solution", this);
    gameTimer = new QTimer(this);
}


void MainWindow::makeWidgetsLayout()
{
    gridLayout->setSpacing(1);
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            // shift by 1 extra slot after every 3 cells
            int layoutRow = row + row / 3;
            int layoutCol = col + col / 3;
            gridLayout->addWidget(cellButtons[row][col], layoutRow, layoutCol, Qt::AlignCenter);
        }
    }
    gridLayout->setRowMinimumHeight(3, 13);
    gridLayout->setRowMinimumHeight(7, 13);
    gridLayout->setColumnMinimumWidth(3, 3);
    gridLayout->setColumnMinimumWidth(7, 3);
    

    for (int i = 0; i < 9; ++i) {
        numberLayout->addWidget(numButtons[i]);
    }

    bottomLayout->addWidget(undoButton);
    bottomLayout->addWidget(newGameButton);
    bottomLayout->addWidget(checkButton);
    mainLayout->addWidget(timerLabel);
    mainLayout->addLayout(gridLayout);
    mainLayout->addLayout(numberLayout);
    mainLayout->addLayout(bottomLayout);
    mainLayout->setAlignment(gridLayout, Qt::AlignHCenter);

    this->setLayout(mainLayout);
}


void MainWindow::makeConnections()
{
    for(int row = 0; row < 9; row++)
    {
        for(int col = 0; col < 9; col++)
        {
            connect(cellButtons[row][col], &QPushButton::clicked, this, [this, row, col](){
                handleCellClicked(row,col);
            });
        }
    }

    for(int i = 0; i < 9; i++)
    {
        connect(numButtons[i], &QPushButton::clicked, this, [this, i](){
            handleNumberClicked(i+1);
        });
    }

    connect(newGameButton, &QPushButton::clicked, this, [this](){
        currentPuzzleIndex = (currentPuzzleIndex + 1) % 3; // Cycles 0 -> 1 -> 2 -> 0
        loadNewGame();
        secondsPassed = 0;
    });

    connect(checkButton, &QPushButton::clicked, this, &MainWindow::handleCheckButtonClicked);
    connect(undoButton, &QPushButton::clicked, this, &MainWindow::handleUndoButtonClicked);
    connect(gameTimer, &QTimer::timeout, this, &MainWindow::updateTimer);
    gameTimer->start(1000);
}


void MainWindow::handleCellClicked(int row, int col)
{
    if (selectedRow == row && selectedCol == col)
    {
        cellButtons[selectedRow][selectedCol]->setStyleSheet("background-color: #F7DDA6");
        selectedRow = -1;
        selectedCol = -1;
        return;
    }
    if (selectedCol != -1 && selectedRow != -1)
    {
        cellButtons[selectedRow][selectedCol]->setStyleSheet("background-color: #F7DDA6; color: #5C0F00;");
    }
    selectedRow = row;
    selectedCol = col;
    cellButtons[selectedRow][selectedCol]->setStyleSheet("background-color: #BF867C;");
}


void MainWindow::handleNumberClicked(int number)
{
    if (selectedCol == -1 || selectedRow == -1)
    {
        return;
    }
    cellButtons[selectedRow][selectedCol]->setText(QString::number(number));
    cellButtons[selectedRow][selectedCol]->setStyleSheet("background-color: #BF867C; color: #5C0F00; font-weight: bold;");
    playerBoard[selectedRow][selectedCol] = number;
    numButtons[number-1]->clearFocus();
} 


void MainWindow::handleCheckButtonClicked()
{
    bool isComplete = true;
    bool isCorrect = true;
    for(int row = 0; row < 9; row++)
    {
        for(int col = 0; col < 9; col++)
        {
            if (playerBoard[row][col] == 0)
            {
                isComplete = false;
            }
            if (playerBoard[row][col] != solutionBoard[row][col])
            {
                isCorrect = false;
            }
        }
    }
    if (!isComplete)
    {
        QMessageBox::warning(this, "Incomplete", "The board is not fully filled yet. Keep going!");
    }
    else if (isCorrect)
    {
        QMessageBox::information(this, "Victory!", "Congratulations! You solved this Sudoku pazzle correctly!");
    }
    else
    {
        QMessageBox::warning(this, "Keep Trying", "There are some mistaked in your solution. Check your numbers!");
    }
}


void MainWindow::updateTimer()
{
    secondsPassed++;
    int minutes = secondsPassed / 60;
    int seconds = secondsPassed % 60;
    QString minStr = minutes < 10 ? "0" + QString::number(minutes) : QString::number(minutes);
    QString secStr = seconds < 10 ? "0" + QString::number(seconds) : QString::number(seconds);
    timerLabel->setText("Time: " + minStr + ":" + secStr); 
}

void MainWindow::handleUndoButtonClicked()
{
    cellButtons[selectedRow][selectedCol]->setText("");
    playerBoard[selectedRow][selectedCol] = 0;
    cellButtons[selectedRow][selectedCol]->setStyleSheet("background-color: #BF867C;");
}


void MainWindow::loadNewGame()
{
    int samplePuzzle1[9][9] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };

    int sampleSolution1[9][9] = {
        {5, 3, 4, 6, 7, 8, 9, 1, 2},
        {6, 7, 2, 1, 9, 5, 3, 4, 8},
        {1, 9, 8, 3, 4, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 9}
    };

    int samplePuzzle2[9][9] = {
        {0, 2, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 6, 0, 0, 0, 0, 3},
        {0, 7, 4, 0, 8, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 3, 0, 0, 2},
        {0, 8, 0, 0, 4, 0, 0, 1, 0},
        {6, 0, 0, 5, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 7, 8, 0},
        {5, 0, 0, 0, 0, 9, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 4, 0}
    };

    int sampleSolution2[9][9] = {
        {1, 2, 6, 4, 3, 7, 9, 5, 8},
        {8, 5, 9, 6, 2, 1, 4, 7, 3},
        {3, 7, 4, 9, 8, 5, 1, 2, 6},
        {4, 1, 5, 7, 9, 3, 8, 6, 2},
        {7, 8, 3, 2, 4, 6, 5, 1, 9},
        {6, 9, 2, 5, 1, 8, 3, 7, 4},
        {9, 4, 1, 3, 1, 2, 7, 8, 5},
        {5, 3, 7, 8, 6, 9, 2, 4, 1},
        {2, 6, 8, 1, 7, 4, 6, 9, 7}
    };

    int samplePuzzle3[9][9] = {
        {0, 0, 0, 0, 0, 0, 2, 0, 0},
        {0, 8, 0, 0, 0, 7, 0, 9, 0},
        {6, 0, 2, 0, 1, 0, 5, 0, 0},
        {0, 7, 0, 0, 0, 3, 0, 0, 1},
        {0, 0, 1, 0, 0, 0, 8, 0, 0},
        {9, 0, 0, 6, 0, 0, 0, 4, 0},
        {0, 0, 5, 0, 4, 0, 6, 0, 3},
        {0, 3, 0, 9, 0, 0, 0, 8, 0},
        {0, 0, 9, 0, 0, 0, 0, 0, 0}
    };

    int sampleSolution3[9][9] = {
        {1, 5, 3, 4, 9, 6, 2, 7, 8},
        {4, 8, 7, 2, 5, 7, 3, 9, 6},
        {6, 9, 2, 8, 1, 3, 5, 1, 4},
        {5, 7, 6, 1, 8, 3, 4, 2, 1},
        {3, 4, 1, 7, 2, 9, 8, 6, 5},
        {9, 2, 8, 6, 3, 5, 1, 4, 7},
        {8, 1, 5, 3, 4, 2, 6, 7, 3},
        {7, 3, 4, 9, 6, 1, 7, 8, 2},
        {2, 6, 9, 5, 7, 8, 9, 3, 5}
    };

    for(int r=0; r<9; ++r) {
        for(int c=0; c<9; ++c) {
            puzzleBank[0][r][c] = samplePuzzle1[r][c]; solutionBank[0][r][c] = sampleSolution1[r][c];
            puzzleBank[1][r][c] = samplePuzzle2[r][c]; solutionBank[1][r][c] = sampleSolution2[r][c];
            puzzleBank[2][r][c] = samplePuzzle3[r][c]; solutionBank[2][r][c] = sampleSolution3[r][c];
        }
    }

    loadPuzzleIntoBoard(puzzleBank[currentPuzzleIndex], solutionBank[currentPuzzleIndex]);
}


void MainWindow::loadPuzzleIntoBoard(int samplePuzzle[9][9], int sampleSolution[9][9])
{
    for (int row = 0; row < 9; row++)
    {
        for(int col = 0; col < 9; col++)
        {
            playerBoard[row][col] = samplePuzzle[row][col];
            solutionBoard[row][col] = sampleSolution[row][col];
            if (samplePuzzle[row][col] != 0)
            {
                cellButtons[row][col]->setText(QString::number(samplePuzzle[row][col]));
                cellButtons[row][col]->setEnabled(false);
                cellButtons[row][col]->setStyleSheet("background-color: #d8b4fe; color: white");
            }
            else{
                cellButtons[row][col]->setText("");
                cellButtons[row][col]->setEnabled(true);
                cellButtons[row][col]->setStyleSheet("background-color: #F7DDA6");
            }
        }
    }
}
