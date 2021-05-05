#ifndef _MAZE_H_
#define _MAZE_H_
#include "PPM.h"
#include "Stack.h"
class Maze {
public:
    Maze( );
    Maze( const int columns, const int rows );
    ~Maze( );
    int getRows( ) const;
    int getColumns( ) const;
    int getCellSize( ) const;
    bool getVerticalWall( const int column, const int row ); // returns wall to left of cell
    bool getHorizontalWall( const int column, const int row ); // returns wall to top of cell
    
    void setSize( const int columns, const int rows );
    void setCellSize( const int size );
    void setVerticalWall( const int column, const int row, const bool status ); // sets wall to left of cell
    void setHorizontalWall( const int column, const int row, const bool status ); // sets wall to top of cell

    void wallsReset(); // adds all walls
    void generate(); // removes walls automatically
    bool solve(); // returns whether it was solved

    void blitWalls( PPM& ppm, const int x, const int y, const unsigned char r1, const unsigned char g1, const unsigned char b1 ) const;// Uses the currently calculated escape values to configure an image in the PPM object. Sets the width and height of the image to match the width and height of the Julia set. Sets the maximum color value to 63. For each pixel in the PPM object, sets the color based on the escape value for the pixel. See the following table:
    void blitSolution( PPM& ppm, const int x, const int y, const unsigned char r1, const unsigned char g1, const unsigned char b1 );
protected:
    void visitsReset();
    int mRows;
    int mColumns;
    int mCellSize;
    bool* mVerticalWalls;
    bool* mHorizontalWalls;
    bool* mVisited;
    Stack<int> mMoves;
};
#endif
