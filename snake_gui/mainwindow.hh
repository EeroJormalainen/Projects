/*
 * Name: Eero Jormalainen
 * Student Number: H292260
 * E-mail: eero.jormalainen@tuni.fi
 * Gui-project: Snake-gui
 * File: mainwindow.cpp
 * Description: Declares the mainwindow -class
*/
#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH
#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include "gameboard.hh"
#include "point.hh"
#include <QKeyEvent>  // For movement with keyboard


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    // Slots for adjusting the board geometry
    void on_HeightSizeBox_valueChanged(int data);
    void on_WidthSizeBox_valueChanged(int data);
    // Keeps the snake on the move
    void movement();
    // Gets everything ready for the game
    void on_ReadyButton_clicked();
    // Directs the snake using the gameboard class methods
    void keyPressEvent(QKeyEvent *event);
    // Resets the labels and informs about the win/loss
    void resetGame(QString prompt, QBrush color);
    // Reacts to snakes length by updating the label and pacing up the game
    void snakeLength();
    // Sets the labels correctly when the game is ready to start
    void labelTool();
    // Hides the infolabels of snakes lenght
    void hideInfo();
    // Draw's the food to the next spot when snake has grown
    void drawFood(int x, int y, unsigned long length);
    // Connected to Modebox -widget and turns the hardmode on/off
    void on_ModeBox_stateChanged();
    // Sets the goal for the player
    void on_GoalDial_sliderMoved(int position);
    // Separate method for snakes movements
    void drawSnake(Point head, Point tail, QBrush brush, QPen pen, int rgb);


private:

    // If true, snake accelerates per every food consumed
    bool hardMode_ = false;
    // Includes the icon directories for the drawFood -method
    std::vector<std::string> fruits_;
    // Points to keep track of the snake spots and lenght
    std::deque<Point> snakePoints_;
    // Random fruit icon representing food
    QPixmap foodPix_;
    // Mainwindow
    Ui::MainWindow *ui;
    // Scene where the game takes place
    QGraphicsScene* scene_;
    // Integer that sets up goal. When snake reaches this lenght, the game ends
    unsigned long int goal_ = 5;
    // Keeping track of the snakes length for comparison
    int snakeLenght_ = 1;
    // Timer for the movement
    QTimer timer_;
    // Initializing the gameboard
    GameBoard* field_;              // Pointer so the board doesn't get build with default parameters
    // Direction that changes by pressing keyboard
    std::string direction_;
    // Original palette of the board
    QPalette palette_;

    int BORDER_UP = 50;      // Parameters for the board geometry
    int BORDER_DOWN = 250;
    int BORDER_LEFT = 30;
    int BORDER_RIGHT = 250;
    int SIZE_FACTOR = 50;
};
#endif // MAINWINDOW_HH
