/*
 * Name: Eero Jormalainen
 * Student Number: H292260
 * E-mail: eero.jormalainen@tuni.fi
 * Gui-project: Snake-gui
 * File: mainwindow.cpp
 * Description: Includes implementation of mainwindow GUI, which is a visual representation what is goin on in the gameboard -Class
 *              Gameboards geometry is scaled to the scene's geometry, so the snake moves on coordinate by coordinate on the scene
 *              Mainwindow also utilizes the Point class when moving the snake. The snake moves 5 coordinates per .12 seconds in the
 *              beginning, and if hardmode is on, the snake goes faster per food consumed. The game ends when the snake either dies
 *              by moving to itself or a wall. Moving is done by WASD-keys, the food is presented as fruit icons. Snake has different coloured
 *              Head and tail
 * Notes: Eventhough I'm satisfied with how the game works and looks in the gui, I'm disappointed in myself
 *        for not being able to make the movement more efficient code-vise. Clearing the field in between every step is
 *        not efficient. But all together i'm happy it's finished. More details can be read from documentation
*/

#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <iostream>
#include <QGraphicsItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , direction_("d")  //Inserting the initial direction


{
    ui->setupUi(this);
    ui->BoardView->setGeometry(BORDER_LEFT, BORDER_UP,
                               BORDER_RIGHT + 2, BORDER_DOWN + 2);
    scene_ = new QGraphicsScene(this);
    timer_.setSingleShot(false);
    connect(&timer_, &QTimer::timeout, this, &MainWindow::movement);  // Connecting the timer to movement -slot
    ui->snekInfoLabel->hide(); //these will be displayed when the player is ready.
    ui->snekSizeLabel->hide();
    ui->deathLabel->hide();

    fruits_ = {"cherries.png", "strawberry.png", "orange.png", "pear.png", "apple.png",   // Filling fruits_ vector with icons for food
               "bananas.png", "tomato.png", "grapes.png", "eggplant.png"};
    ui->BoardView->setScene(scene_);
}

MainWindow::~MainWindow()
{
    delete scene_;
    delete field_;
    delete ui;
}


void MainWindow::on_HeightSizeBox_valueChanged(int data)
{   // Adjusts the height of the screen, receives the signal and data
    // from HeighSizeBox -QWidget

    BORDER_DOWN = SIZE_FACTOR * data;
    ui->BoardView->setGeometry(BORDER_LEFT, BORDER_UP,
                               BORDER_RIGHT + 2, BORDER_DOWN + 2);
    hideInfo();
}

void MainWindow::on_WidthSizeBox_valueChanged(int data)
{   // Adjusts the width of the screen, receives the signal and data
    // from WidthSizeBox -QWidget

    BORDER_RIGHT = SIZE_FACTOR * data;
    ui->BoardView->setGeometry(BORDER_LEFT, BORDER_UP,
                               BORDER_RIGHT + 2, BORDER_DOWN + 2);
    hideInfo();
}

void MainWindow::movement()
{   // Moves the snake and the food around and paints objects
    // based on their coordinates. Scene is cleared per movement and every
    // point is painted again. Snake has different coloured head and tail and body colour changes gradiently

    Point food = field_->getFood();
    Point head = field_->getHead(); // for painting head and tail separately
    Point tail = field_->getTail();
    QPen pen(Qt::black);
    std::deque<Point> snake = field_->getSnake();  // Updating the situation
    QString whatHappened;
    QBrush brush;
    
    if (snake.size() == goal_){  // Goal reached, game over

        whatHappened = "You won !! Congratulations you legend!";
        brush = Qt::green;
        resetGame(whatHappened, brush);

    }
    if (field_->moveSnake(direction_)){
        scene_->clear();
        drawSnake(head, tail, brush, pen, 1);
        snakeLength();
        drawFood(food.getCoordinates().at(0), food.getCoordinates().at(1), snake.size());
    }


    else{      // MoveSnake returned false
        whatHappened = "Snek is ded! Another one? ;)";
        brush = Qt::red;
        resetGame(whatHappened, brush);
    }
    snakePoints_ = snake; // Updating the snake to the gui for comparison

 }

void MainWindow::keyPressEvent(QKeyEvent *event)
{   // Sets the direction based on which key was pressed, cant make a 180 degree turn
    if (event->key() == Qt::Key_A and direction_ != "d"){
        direction_ = 'a';
    }
    else if (event->key() == Qt::Key_W and direction_ != "s"){
        direction_ = 'w';
    }
    else if (event->key() == Qt::Key_S  and direction_ != "w"){
        direction_ = 's';
    }
    else if (event->key() == Qt::Key_D and direction_ != "a"){
        direction_ = 'd';
    }
}

void MainWindow::resetGame(QString prompt, QBrush color)
{   // Game is lost, new one?
    timer_.stop();
    ui->HeightSizeBox->setDisabled(false); // Enabling the buttons again so the game
    ui->WidthSizeBox->setDisabled(false);  // can be adjusted for next round
    ui->SeedBox->setDisabled(false);
    ui->ModeBox->setDisabled(false);
    ui->GoalDial->setDisabled(false);
    ui->deathLabel->show();
    QString another = "Anotha one";
    ui->ReadyButton->setText(another);
    ui->deathLabel->setText(prompt);    // Promt and colour is set based on whether player won or lost the game
    scene_->setBackgroundBrush(color);

}

void MainWindow::snakeLength()
{   //Updating the snekSizeLabel (and speed if hardmode is on)

    int length = field_->getSnake().size();
    if (length > snakeLenght_){
        ui->snekSizeLabel->setText(QString::number(length));
        if (hardMode_ and timer_.interval() > 40 ){
            timer_.setInterval(timer_.interval() - 3 *length); // makes snake go faster but not too fast
            }
        snakeLenght_ = length; // Updates the snakes length
        }
}

void MainWindow::labelTool()
{   // Works out the labels after the player is ready to (re)start.
    ui->snekInfoLabel->move(BORDER_RIGHT + 50, BORDER_UP); //Moves the info labels to correct spots
    ui->snekSizeLabel->move(BORDER_RIGHT + 135, BORDER_UP);
    ui->deathLabel->move(BORDER_RIGHT + 50, BORDER_UP + 40);
    ui->snekInfoLabel->show();
    ui->snekSizeLabel->show();
    ui->HeightSizeBox->setDisabled(true); // Disabling the adjustments and seed buttons so they can't be used
    ui->WidthSizeBox->setDisabled(true);  // during the game
    ui->SeedBox->setDisabled(true);
    ui->ModeBox->setDisabled(true);
    ui->GoalDial->setDisabled(true);
    snakeLenght_ = 1;                   // Resetting length
    ui->snekSizeLabel->setText(QString::number(snakeLenght_));
    QString reset = "Reset";
    ui->ReadyButton->setText(reset);
}

void MainWindow::hideInfo()
{ // Hides info labels
    ui->deathLabel->hide();
    ui->snekInfoLabel->hide();
    ui->snekSizeLabel->hide();
}

void MainWindow::drawFood(int x, int y, unsigned long length)
{   // Draws foos based on given points coordinates, changes when consumed

    if (length != snakePoints_.size()){
        std::string filename;
        std::uniform_int_distribution<int> ind(0, fruits_.size()-1); //The food icon switches every time a point
        filename.append(fruits_.at(ind(field_->rng_)));              // is consumed
        foodPix_ = QString::fromStdString(filename);
    }

    ui->FoodLabel->setPixmap(foodPix_.scaled(15, 15,Qt::IgnoreAspectRatio)); // Scaling the icon
    ui->FoodLabel->move(x + BORDER_LEFT, BORDER_UP + y - 5); // Scaling to position to fit the scene

}


void MainWindow::on_ReadyButton_clicked()
{   // Actions that take place after the player is ready with the adjustments and seed


    scene_->setSceneRect(0, 0, BORDER_RIGHT, BORDER_DOWN); // Updating the scenes geometry based on
                                                           // graphicsview geometry
    scene_->setBackgroundBrush(QColor::fromRgba(233));

    labelTool();        // resetting labels and snakes lenght  

    field_ = new GameBoard(BORDER_RIGHT - BORDER_LEFT +20,
                           BORDER_DOWN - BORDER_UP + 40, ui->SeedBox->value()); // Initializing and starting the game
    timer_.start(120);

}

void MainWindow::on_ModeBox_stateChanged()
{
    hardMode_ = ui->ModeBox->checkState(); // Hardmode on --> snake goes faster when expanding
}


void MainWindow::on_GoalDial_sliderMoved(int position)
{   // Sets the goal based on the players desired goal
    goal_ = position;
    ui->GoalLabel->setText(QString::number(position));
}

void MainWindow::drawSnake(Point head, Point tail, QBrush brush,QPen pen, int rgb)
{ // Snake is on the move, every point is painted separately with gradient colors

    for (auto point : field_->getSnake()){
        if (std::find(snakePoints_.begin(), snakePoints_.end(), point) != snakePoints_.end()){
            brush = QColor::fromRgb(rgb, 100, 100);
            scene_->addEllipse(point.getCoordinates().at(0), point.getCoordinates().at(1),10,10, pen, brush);
            if (rgb + 10 < 255)  // Making sure the rgbnumber doesn't get out of range (255)
                rgb += 10;
        }
     }
    brush = Qt::green;
    scene_->addEllipse(head.getCoordinates().at(0), head.getCoordinates().at(1), 10, 10, pen, brush); // Painting head

    if (field_->getSnake().size() > 1){
        brush = Qt::red;
        scene_->addEllipse(tail.getCoordinates().at(0), tail.getCoordinates().at(1),10,10, pen, brush); // Painting tail
        }
}

