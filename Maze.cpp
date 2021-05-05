#include "Maze.h"
#include "PPM.h"
#include "Stack.h"
#include <cstdlib>
double positive(const double &x){
    double t=x;
    if (x<0){
        t=-t;
    }
    return t;
}
Maze::Maze( ):
  mRows(0),mColumns(0),mCellSize(100),mVerticalWalls(0),mHorizontalWalls(0),mVisited(0),mMoves(){
    setSize(4,3);
}
Maze::Maze( const int columns, const int rows ):
  mRows(rows),mColumns(columns),mCellSize(100),mVerticalWalls(0),mHorizontalWalls(0),mVisited(0),mMoves(){
    setSize(columns,rows);
}
Maze::~Maze( ){
    if( mVerticalWalls != 0 ) {
        delete [] mVerticalWalls;
        delete [] mHorizontalWalls;
        delete [] mVisited;
    }
}
int Maze::getRows( ) const{
    return mRows;
}
int Maze::getColumns( ) const{
    return mColumns;
}
int Maze::getCellSize( ) const{
    return mCellSize;
}
bool Maze::getVerticalWall( const int column, const int row ) {// returns wall to left of cell
    return mVerticalWalls[row*mColumns+column];
}
bool Maze::getHorizontalWall( const int column, const int row ){ // returns wall to top of cell
    return mHorizontalWalls[row*mColumns+column];
}
void Maze::setSize( const int columns, const int rows ){
    if( mVerticalWalls != 0 ) {
        delete [] mVerticalWalls;
        delete [] mHorizontalWalls;
        delete [] mVisited;
    }
    mColumns=columns;
    mRows=rows;
    mVerticalWalls=new bool[columns*rows+rows];
    mHorizontalWalls=new bool[rows*columns+columns];
    mVisited=new bool[rows*columns];
}
void Maze::setCellSize( const int size ){
    mCellSize=size;
}
void Maze::setVerticalWall( const int column, const int row, const bool status ){ // sets wall to left of cell
    mVerticalWalls[row*mColumns+column]=status;
}
void Maze::setHorizontalWall( const int column, const int row, const bool status ){ // sets wall to top of cell
    mHorizontalWalls[row*mColumns+column]=status;
}
void Maze::wallsReset(){ // adds all walls
    int i;
    for (i=0;i<mColumns*mRows+mRows;i++){
        mVerticalWalls[i]=true;
    }
    for (i=0;i<mRows*mColumns+mColumns;i++){
        mHorizontalWalls[i]=true;
    }
}
void Maze::visitsReset(){
    int i;
    for (i=0;i<mColumns*mRows;i++){
        mVisited[i]=false;
    }
}
// 5*4 visitables 20
// _ _ _ _ _
//|# # # # #|
// _ _ _ _ _
//|# # # # #|
// _ _ _ _ _
//|# # # # #|
// _ _ _ _ _
//|# # # # #|
// _ _ _ _ _

// 5*5 Horizontal 25
// 6*4 Vertical 24
// 0 1 2 3 4
//0#1#2#3#4#5
// 5 6 7 8 9
//6#7#8#9#A#B
// A B C D E
//C#D#E#F#G#H
// F G H I J
//I#J#K#L#M#N
// K L M N O

// 0 1 2 3 4
//00112233445
// 5 6 7 8 9
//65768798A9B
// A B C D E
//CADBECFDGEH
// F G H I J
//IFJGKHLIMJN
// K L M N O

//  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F  G  H  I  J  K  L  M  N  O
// 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24


// Given visibility find Adjacent walls
// Up wall is V
// Down wall is V+Width
// Left wall V+V/Width
// Right wall V+V/Width+1
void Maze::generate(){ // removes walls automatically
    wallsReset();
    visitsReset();
    Stack<int> places;
    places.push(0);
    int nexti[4];
    int i,index;
    while (!places.isEmpty()){
        index=places.pop();
        mVisited[index]=true;
        i=0;
        // determine which cells we could move to next
        if (index%mColumns!=0 && !mVisited[index-1]){ // left
            nexti[i]=index-1;
            i+=1;
        }
        if (index%mColumns!=mColumns-1 && !mVisited[index+1]){ // right
            nexti[i]=index+1;
            i+=1;
        }
        if (index/mColumns!=0 && !mVisited[index-mColumns]){ // up
            nexti[i]=index-mColumns;
            i+=1;
        }
        if (index/mColumns!=mRows-1 && !mVisited[index+mColumns]){ // down
            nexti[i]=index+mColumns;
            i+=1;
        }
        if (i != 0){
            // randomly choose 1 direction to go
            int ni = nexti[std::rand()%i];

            // knock out walls between this cell and the next cell
            if (ni == index+1){ // right move
                mVerticalWalls[index+index/mColumns+1]=false;
            }
            if (ni == index-1){ // left move
                mVerticalWalls[index+index/mColumns]=false;
            }
            if (ni == index+mColumns){ // down move
                mHorizontalWalls[index+mColumns]=false;
            }
            if (ni == index-mColumns){ // up move
                mHorizontalWalls[index]=false;
            }

            // recursively visit the next cell
            places.push(index);
            places.push(ni);
        }
    }
}
bool Maze::solve(){ // returns whether it was solved
    visitsReset();
    Stack<int> places;
    places.push(0);
    places.push(1);
    int todo,index;
    int finish=mColumns*mRows-1;
    while (!mMoves.isEmpty()){
        mMoves.pop();
    }
    while (!places.isEmpty()){
        todo=places.pop();
        if (todo){
            index=places.pop();
            // Mark this cell as visited.
            mVisited[index]=true;
            // Record the index in the class variable mMoves.
            mMoves.push(index);
            // If we are at the end cell, return true.
            if (index==finish){
                return true;
            }
            
            places.push(0);
            // move up if there is no wall, and it hasn't been visited. Return true if it returns true.
            if (!(mHorizontalWalls[index] || mVisited[index-mColumns])){
                places.push(index-mColumns);
                places.push(1);
            }
            // move left if there is no wall, and it hasn't been visited. Return true if it returns true.
            if (!(mVerticalWalls[index+index/mColumns] || mVisited[index-1])){
                places.push(index-1);
                places.push(1);
            }
            // move right if there is no wall, and it hasn't been visited. Return true if it returns true.
            if (!(mVerticalWalls[index+index/mColumns+1] || mVisited[index+1])){
                places.push(index+1);
                places.push(1);
            }
            // move down if there is no wall, and it hasn't been visited. Return true if it returns true.
            if (!(mHorizontalWalls[index+mColumns] || mVisited[index+mColumns])){
                places.push(index+mColumns);
                places.push(1);
            }
        }
        else{
            // This is a loser cell, so undo the move from self.mMoves, and return false to the previous cell.
            mMoves.pop();
        }
    }
    return false;
}
void Maze::blitWalls( PPM& ppm, const int x, const int y, const unsigned char r1, const unsigned char g1, const unsigned char b1 ) const{// Uses the currently calculated escape values to configure an image in the PPM object. Sets the width and height of the image to match the width and height of the Julia set. Sets the maximum color value to 63. For each pixel in the PPM object, sets the color based on the escape value for the pixel. See the following table:
    int i,j,k;
    k=0;
    for (j=0;j<mRows;j++){
        for (i=0;i<mColumns+1;i++){
            if (mVerticalWalls[k]){
                ppm.line(x + i*mCellSize,y + j*mCellSize,x + i*mCellSize,y + j*mCellSize + mCellSize,r1,g1,b1);
            }
            k+=1;
        }
    }
    k=0;
    for (j=0;j<mRows+1;j++){
        for (i=0;i<mColumns;i++){
            if (mHorizontalWalls[k]){
                ppm.line(x + i*mCellSize,y + j*mCellSize,x + i*mCellSize + mCellSize,y + j*mCellSize,r1,g1,b1);
            }
            k+=1;
        }
    }
    
}
void Maze::blitSolution( PPM& ppm, const int x, const int y, const unsigned char r1, const unsigned char g1, const unsigned char b1 ){
    int index1,index2;
    while (!mMoves.isEmpty()){
        index1 = mMoves.pop();
        if (!mMoves.isEmpty()){
            index2 = mMoves.top();
            // Now draw it graphically!
            ppm.line(
                x+index1%mColumns*mCellSize+mCellSize/2,
                y+index1/mColumns*mCellSize+mCellSize/2,
                x+index2%mColumns*mCellSize+mCellSize/2,
                y+index2/mColumns*mCellSize+mCellSize/2,
                r1,g1,b1
            );
        }
    }
}