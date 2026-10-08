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
        void loadPuzzleIntoBoard(const int puzzle[9][9], const int solution[9][9]);
    private slots:
        void handleCellClicked(int row, int col);
        void handleNumberClicked(int number);
        void handleCheckButtonClicked();
        void handleUndoButtonClicked();
        void updateTimer();
    private:
        QVBoxLayout *mainLayout;
        QGridLayout *gridLayout;
        QHBoxLayout *bottomLayout;
        QHBoxLayout *numberLayout;

        QPushButton *cellButtons[9][9];
        QPushButton *numButtons[9];
        QPushButton *checkButton;
        QPushButton *undoButton;
        QPushButton *newGameButton;

        int puzzleBank[3][9][9];
        int solutionBank[3][9][9];
        int currentPuzzleIndex;
        int playerBoard[9][9];
        int solutionBoard[9][9];

        int selectedRow;
        int selectedCol;

        QTimer *gameTimer;
        int secondsPassed;
        QLabel* timerLabel;
};

#endif