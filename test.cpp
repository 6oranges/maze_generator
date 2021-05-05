#include "PPM.h"
#include "Maze.h"
#include<fstream>
#include <cstdlib>
#include <unistd.h>
#include <sstream>
#include <ctime>
#include <chrono>
/*
int main(){
  int cols=100,rows=100,size=10,margin=10;
  std::srand( std::time( 0 ) );
  PPM p=PPM();
  p.setWidth(cols*size+2*margin);
  p.setHeight(rows*size+2*margin);
  p.setMaxColorValue(255);
  Maze m=Maze();
  m.setCellSize(size);
  m.setSize(cols,rows);
  m.generate();
  m.blitWalls(p,margin,margin,255,0,0);
  std::cout<<m.solve()<<std::endl;
  std::ofstream fout("Maze.ppm");
  fout<<p;
  fout.close();
  m.blitSolution(p,margin,margin,0,255,0);
  std::ofstream sfout("Solved.ppm");
  sfout<<p;
  sfout.close();
  return 0;
}*/
int main( int argc, char **argv ) {
  std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now( );
  std::chrono::high_resolution_clock::duration   dur = now.time_since_epoch( );
  std::chrono::microseconds us = std::chrono::duration_cast< std::chrono::microseconds > ( dur );
  std::srand( us.count( ) / 100 );
  PPM ppm;
  ppm.setMaxColorValue(255);
  std::string filename;
  std::stringstream ss;
  ss.str("");
  int cols=10,rows=10,size=50,margin=10,r1,g1,b1,c;
  Maze m = Maze( );
  char c1,c2;
  while( ( c = getopt ( argc, argv, "d:p:b:ce:w:s:f:") ) != -1 ) {
    ss.clear();
    if (optarg != 0){
      ss.str( optarg );
    }
    switch( c ) {
      case 'd':
        ss >> rows >> c1 >> cols;
        if( ss && c1 == ',') {
          m.setSize(cols,rows);
          ppm.setWidth(cols*size+2*margin);
          ppm.setHeight(rows*size+2*margin);
        }
        break;
      case 'p':
        ss >> size;
        if( ss ) {
          m.setCellSize(size);
        }
        break;
      case 'b':
        ss >> margin;
        if( ss ) {
          ppm.setWidth(cols*size+2*margin);
          ppm.setHeight(rows*size+2*margin);
        }
        break;
      case 'c':
        m.generate();
        m.solve();
        break;
      case 'e':
        ss >> r1 >> c1 >> g1 >> c2 >> b1;
        if( ss && c1 == ',' && c2 == ',') {
          ppm.fill(r1,g1,b1);
        }
        break;
      case 'w':
        ss >> r1 >> c1 >> g1 >> c2 >> b1;
        if( ss && c1 == ',' && c2 == ',') {
          m.blitWalls(ppm,margin,margin,r1,g1,b1);
        }
        break;
      case 's':
        ss >> r1 >> c1 >> g1 >> c2 >> b1;
        if( ss && c1 == ',' && c2 == ',') {
          m.blitSolution(ppm,margin,margin,r1,g1,b1);
        }
        break;
      case 'f':
        ss >> filename;
        if( ss ) {
          std::ofstream fout( filename, std::ios::binary );
          if( fout ) {
            fout << ppm;
            fout.close( );
          }
          else{
            std::cout<<"error in filewriting"<<std::endl;
          }
        }
        else{
          std::cout<<"error in f"<<std::endl;
        }
        break;
      case 'h':
      case '?':
        std::cout<<"Use Case"<<std::endl;
        std::cout<<"-d rows,columns # set dimensions for maze"<<std::endl;
        std::cout<<"-p cellpadding # set the cell width and height"<<std::endl;
        std::cout<<"-b border # set the border on the image"<<std::endl;
        std::cout<<"-c # generate"<<std::endl;
        std::cout<<"-e r,g,b # Erase"<<std::endl;
        std::cout<<"-w r,g,b # Draw walls"<<std::endl;
        std::cout<<"-s r,g,b # Draw Solution"<<std::endl;
        std::cout<<"-f filename # Name"<<std::endl;
        break;
      default:
        break;
    }
  }
}
