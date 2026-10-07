#ifndef SUDOKU_H
#define SUDOKU_H

#include <QWidget>

class QPushButton;
class QGridLayout;
class QVBoxLayout;
class QHBoxLayout;
class QTimer;
class QLabel;

class MainWindow: public QWidget
{
    Q_OBJECT
    public:
        MainWindow(QWidget* parent = nullptr);
    private:
        void createWidgets();
        void makeWidgetsLayout();
        void makeConnections();
        void loadNewGame();
    private slots:
        void handleCellClicked(int row, int col);
        void handleNumberClicked(int number);
        void handleCheckButtonClicked();
        void updateTimer();
    private:
        QVBoxLayout *mainLayout;
        QGridLayout *gridLayout;
        QHBoxLayout *numberLayout;

        QPushButton *cellButtons[9][9];
        QPushButton *numButtons[9];
        QPushButton *checkButton;

        int playerBoard[9][9];
        int solutionBoard[9][9];

        int selectedRow;
        int selectedCol;

        QTimer *gameTimer;
        int secondsPassed;
        QLabel* timerLabel;
};

#endif