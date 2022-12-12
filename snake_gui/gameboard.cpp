/*
####################################################################
# COMP.CS.110 Programming 2: Structures, 2022                      #
#                                                                  #
# Project4: Snake                                                  #
# Program description: Implements a game called Snake.             #
#                                                                  #
# File: gameboard.cpp                                              #
# Description: Defines a class representing the gameboard.         #
#                                                                  #
# Author: Eero Jormalainen., eero.jormalainen@tuni.fi              #
####################################################################
*/

#include "gameboard.hh"
#include <iostream>
#include <vector>

GameBoard::GameBoard() : GameBoard(DEFAULT_WIDTH, DEFAULT_HEIGTH, DEFAULT_SEED)
{    
}

GameBoard::GameBoard(int width, int height, int seed_value):
    width_(width), height_(height)
{
    rng_.seed(seed_value*1000);  // Scaling the seed value up, since with smaller seeds the
                                 // first random number was always 0.
    // Creating a snake whose head is about in the middle of the gameboard
    const Point head((width_) / 2, (height_) / 2);
    snake_.push_back(head);

    // Putting food somewhere
    moveFood();
}

GameBoard::~GameBoard()
{
}

bool GameBoard::gameOver() const
{
    return gameLost() or gameWon();
}

bool GameBoard::gameLost() const
{
    return dead_;
}

bool GameBoard::gameWon() const
{
    return static_cast<int>(snake_.size()) >= width_ * height_;
}

bool GameBoard::moveSnake(const std::string& direction)
{
    // If the snake can't / doesn't need to move, do nothing
    if (gameOver())
    {
        return false;
    }

    // Figuring out the destination coordinates
    Point new_head = getHead();
    new_head.move(direction);

    // Checking if the snake died due to running against a wall

    if(not new_head.isInside(0, 0, width_ , height_ ))
    {
        Point food = getHead();

        dead_ = true;
        return false;
    }

    // Actually moving the snake
    return moveSnakeAndFood(new_head);
}

std::deque<Point> GameBoard::getSnake()
{
    return snake_;
}
Point GameBoard::getFood()
{
    return food_;
}

bool GameBoard::isSnakePoint(const Point& point) const
{
    // Checking if any of the Points stored in snake_ match the given point
    for(const Point& part : snake_)
    {
        if (part == point)
        {
            return true;
        }
    }
    return false;
}

const Point& GameBoard::getHead() const
{
    // This should never happen: snake_ contains at least one element (head)
    if(snake_.size() == 0)
    {
        std::cout << "Internal error: empty snake" << std::endl;
    }
    // The snake's head is the last one in the deque
    return snake_.back();
}

const Point& GameBoard::getTail() const
{
    // This should never happen: snake_ contains at least one element (head)
    if(snake_.size() == 0)
    {
        std::cout << "Internal error: empty snake" << std::endl;
    }
    // The snake's tail is the first one in the deque
    return snake_.front();
}

void GameBoard::moveFood()
{
    // Moving food out of sight when it's no longer needed
    if (gameWon())
    {
        food_.setPosition(-1, -1);
        return;
    }

    // Keeping to try random points until an empty square is found
    std::uniform_int_distribution<int> width_dist(0, width_ );
    std::uniform_int_distribution<int> height_dist(0, height_);
    int wd = width_dist(rng_); // Saving the numbers so it can be rounded to closest 10
    int hd = height_dist(rng_);



    while (true)
    {
        food_.setPosition(wd-wd%10, hd - hd%10);
        if (not isSnakePoint(food_))
        {
            // Snakeless point found, stop moving the food around
            return;
        }
    }
}

bool GameBoard::moveSnakeAndFood(const Point& new_head)
{
    // There shouldn't be any problems if the snake is only a head or
    // if it doesn't yet occupy the point it's moving into.
    if (snake_.size() > 1 and isSnakePoint(new_head))
    {
        const Point& neck = snake_.at(snake_.size() - 2);
        if (new_head == neck)
        {
            // If the destination point is the point before the head,
            // don't move but don't die either
            return false;

        }
        else if (new_head != getTail())
        {
            // If the destination point is the snake's body point, other than
            // "neck" or tail, the snake dies
            dead_ = true;
            return false;
        }
    }

    // Moving the snake
    snake_.push_back(new_head);

    // New food must be placed somewhere once one gets eaten.
    // Also, the snake should stretch a little.
    if (new_head == food_)
    {
        moveFood();
    }
    else // Otherwise the point is of the old tail becomes empty.
    {
        snake_.pop_front();
    }
    return true;
}
