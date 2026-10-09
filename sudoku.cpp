#include "sudoku.h"
#include "puzzles.h"

#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QMessageBox>
#include <QLabel>


MainWindow::MainWindow(QWidget* parent) : QWidget(parent)
{
    setWindowTitle("Sudoku");
    createWidgets();
    makeWidgetsLayout();
    makeConnections();
    loadNewGame(false);
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
        numButtons[i]->setStyleSheet("background-color: #D4E0D2; color: #2C3531; font-weight: bold;");
    }
    deleteButton = new QPushButton("Delete", this);
    deleteButton->setStyleSheet("background-color: #E2D9CE; color: #3E3A36; font-weight: bold;");
    newGameButton = new QPushButton("New Game", this);
    newGameButton->setStyleSheet("background-color: #E2D9CE; color: #3E3A36; font-weight: bold;");

    timerLabel = new QLabel("Time: 00:00", this);
    timerLabel->setStyleSheet(
        "background-color: #737E91;"
        "color: #2C3531;"
        "font-weight: bold;"
        "font-size: 14px;"
        "border-radius: 4px;"
        "padding: 6px;"
    );
    timerLabel->setAlignment(Qt::AlignCenter);

    checkButton = new QPushButton("Check Solution", this);
    checkButton->setStyleSheet("background-color: #E2D9CE; color: #3E3A36; font-weight: bold;");
    gameTimer = new QTimer(this);
}


void MainWindow::makeWidgetsLayout()
{
    gridLayout->setSpacing(1);
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
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

    bottomLayout->addWidget(deleteButton);
    bottomLayout->addWidget(newGameButton);
    bottomLayout->addWidget(checkButton);
    mainLayout->addWidget(timerLabel);
    mainLayout->addLayout(gridLayout);
    mainLayout->addLayout(numberLayout);
    mainLayout->addLayout(bottomLayout);
    mainLayout->setAlignment(gridLayout, Qt::AlignHCenter);

    this->setStyleSheet("background-color: #F5F2EB");
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
        loadNewGame(true);
        secondsPassed = 0;
    });

    connect(checkButton, &QPushButton::clicked, this, &MainWindow::handleCheckButtonClicked);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::handleDeleteButtonClicked);
    connect(gameTimer, &QTimer::timeout, this, &MainWindow::updateTimer);
    gameTimer->start(1000);
}


void MainWindow::handleCellClicked(int row, int col)
{
    if (selectedRow == row && selectedCol == col)
    {
        cellButtons[selectedRow][selectedCol]->setStyleSheet("background-color: #EBE1D1; color: #2D4A3E; font-weight: bold;");
        selectedRow = -1;
        selectedCol = -1;
        return;
    }
    if (selectedCol != -1 && selectedRow != -1)
    {
        cellButtons[selectedRow][selectedCol]->setStyleSheet("background-color: #EBE1D1; color: #2D4A3E; font-weight: bold;");
    }
    selectedRow = row;
    selectedCol = col;
    cellButtons[selectedRow][selectedCol]->setStyleSheet("background-color: #C7D6C1; color: #2D4A3E; font-weight: bold;");
}


void MainWindow::handleNumberClicked(int number)
{
    if (selectedCol == -1 || selectedRow == -1)
    {
        return;
    }
    cellButtons[selectedRow][selectedCol]->setText(QString::number(number));
    cellButtons[selectedRow][selectedCol]->setStyleSheet("background-color: #C7D6C1; color: #2D4A3E; font-weight: bold;");
    playerBoard[selectedRow][selectedCol] = number;
    numButtons[number-1]->clearFocus();
} 


void MainWindow::showGameMessage(const QString &title, const QString &text, const QString &iconPath)
{
    gameTimer->stop();

    QString msgBoxStyle = 
        "QMessageBox { background-color: #F5F2EB; color: #2C3531; }"
        "QLabel { color: #2C3531; font-weight: bold; }"
        "QPushButton { background-color: #E8E3D9; color: #3E3A36; border-radius: 4px; padding: 6px 14px; font-weight: bold; }";

    QMessageBox msgBox(this);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    // msgBox.setIcon(QMessageBox::Information);
    QPixmap customIcon(iconPath);
    msgBox.setIconPixmap(customIcon.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    msgBox.setStyleSheet(msgBoxStyle);
    QPushButton *okButton = msgBox.addButton(QMessageBox::Ok);
    QPushButton *customNewGameButton = msgBox.addButton("New Game", QMessageBox::ActionRole);
    QPushButton *costumRetryButton = msgBox.addButton(QMessageBox::Retry);
    msgBox.exec();          // to execute the window (make it appear)

    if (msgBox.clickedButton() == customNewGameButton){
        loadNewGame(true);
        secondsPassed = 0;
    }
    else if (msgBox.clickedButton() == costumRetryButton){
        loadNewGame(false);
        secondsPassed = 0;
    }
    else if (msgBox.clickedButton() == okButton){}

    gameTimer->start(1000);
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
        showGameMessage("Incomplete", "The board is not fully filled yet. Keep going!", "resources/thinking.jpg");
    }
    else if (isCorrect)
    {
        showGameMessage("Victory!", "Congratulations! You solved this Sudoku puzzle correctly!\n\n" + timerLabel->text(), "resources/happy.jpg");
    }
    else
    {
        showGameMessage("Keep Trying", "There are some mistaked in your solution. Check your numbers!", "resources/dissapointed.png");
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

void MainWindow::handleDeleteButtonClicked()
{
    if (selectedRow == -1 || selectedCol == -1)
        return;
    cellButtons[selectedRow][selectedCol]->setText("");
    playerBoard[selectedRow][selectedCol] = 0;
    cellButtons[selectedRow][selectedCol]->setStyleSheet("background-color: #C7D6C1;");
}


void MainWindow::loadNewGame(bool flag)
{
    if (flag == false)
    {
        loadPuzzleIntoBoard(SAMPLE_PUZZLES[currentPuzzleIndex], SAMPLE_SOLUTIONS[currentPuzzleIndex]);
    }
    else{
        currentPuzzleIndex = (currentPuzzleIndex + 1) % PUZZLE_COUNT; // Cycles 0 -> 1 -> 2 -> 0
        loadPuzzleIntoBoard(SAMPLE_PUZZLES[currentPuzzleIndex], SAMPLE_SOLUTIONS[currentPuzzleIndex]);
    }
}


void MainWindow::loadPuzzleIntoBoard(const int samplePuzzle[9][9], const int sampleSolution[9][9])
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
                cellButtons[row][col]->setStyleSheet("background-color: #C8B9A6; color: #3E3A36; font-weight: bold;");
            }
            else{
                cellButtons[row][col]->setText("");
                cellButtons[row][col]->setEnabled(true);
                cellButtons[row][col]->setStyleSheet("background-color: #EBE1D1; color: #2D4A3E; font-weight: bold;");
            }
        }
    }
}
