#include "sudoku.h"

#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>


MainWindow::MainWindow(QWidget* parent) : QWidget(parent)
{
    createWidgets();
    makeWidgetsLayout();
    makeConnections();
}


void MainWindow::createWidgets()
{
    selectedRow = -1;
    selectedCol = -1;
    secondsPassed = 0;

    mainLayout = new QVBoxLayout(this);
    gridLayout = new QGridLayout;
    numberLayout = new QHBoxLayout;

    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            cellButtons[row][col] = new QPushButton("", this);
            cellButtons[row][col]->setFixedSize(45, 45);
        }
    }

    for (int i = 0; i < 10; ++i) {
        numButtons[i] = new QPushButton(QString::number(i), this);
        numButtons[i]->setFixedSize(40, 40);
    }

    checkButton = new QPushButton("Check Solution", this);
    gameTimer = new QTimer(this);
}


void MainWindow::makeWidgetsLayout()
{
    // for (int row = 0; row < 9; row++)
    // {
    //     for (int col = 0; col < 9; col++)
    //     {
    //         gridLayout->addWidget(cellButtons[row][col], row, col);
    //     }
    // }


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
    gridLayout->setRowMinimumHeight(3, 6);
    gridLayout->setRowMinimumHeight(7, 6);
    gridLayout->setColumnMinimumWidth(3, 6);
    gridLayout->setColumnMinimumWidth(7, 6);
    

    for (int i = 0; i < 10; ++i) {
        numberLayout->addWidget(numButtons[i]);
    }

    mainLayout->addLayout(gridLayout);
    mainLayout->addLayout(numberLayout);
    mainLayout->addWidget(checkButton);
    mainLayout->setAlignment(gridLayout, Qt::AlignHCenter);

    this->setLayout(mainLayout);
}


void MainWindow::makeConnections()
{
    // Left empty for now
}


void MainWindow::handleCellClicked(int row, int col)
{
    // Left empty for now
}


void MainWindow::handleNumberClicked(int number)
{
    // Left empty for now
}


void MainWindow::handleCheckButtonClicked()
{
    // Left empty for now
}


void MainWindow::updateTimer()
{
    // Left empty for now
}