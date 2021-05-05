#ifndef _PPM_H_
#define _PPM_H_
#include<iostream>
#include<vector>
class PPM {

public:
    PPM( ); // constructor
    PPM( const PPM &ppm ); // constructor
    PPM( const PPM &ppm, const unsigned int&x, const unsigned int&y, const unsigned int&dx, const unsigned int&dy); // constructor
    int getWidth( ) const;
    int getHeight( ) const;
    int getMaxColorValue( ) const;
    int getChannel( const unsigned int& row, const unsigned int& column, const unsigned int& channel ) const;
    void setWidth( const int& width );
    void setHeight( const int& height );
    void setMaxColorValue( const int& max_color_value );
    void setChannel( const unsigned int& row, const unsigned int& column, const unsigned int& channel, const unsigned int& value );
    void sepiaFilter( PPM& dst ) const;
    void fancyEdges( PPM& dst ) const;
    PPM& julia( double x, double y, double c, double a, double b, unsigned int m, double co );
    PPM& juliaGrey( double x, double y, double c, double a, double b);
    PPM& grayFromRed();
    PPM& grayFromGreen();
    PPM& grayFromBlue();
    PPM& grayFromLinear();
    PPM& invert();
    PPM& verticalEdges();
    PPM& horizontalEdges();
    PPM& blit(const PPM &p, const int&x, const int&y);
    PPM& blit(const PPM &p, const int&x, const int&y, const unsigned char&r, const unsigned char&g, const unsigned char&b);
    PPM& smooth();
    PPM& horizontalReflection();
    PPM& verticalReflection();
    PPM& line( const int x1, const int y1, const int x2, const int y2, const unsigned char r1, const unsigned char g1, const unsigned char b1 );
    PPM& replace(const unsigned char&r1, const unsigned char&g1, const unsigned char&b1, const unsigned char&r2, const unsigned char&g2, const unsigned char&b2);
    PPM& bucketFill(const unsigned int&x, const unsigned int&y, const unsigned char&r1, const unsigned char&g1, const unsigned char&b1);
    PPM& fill(const unsigned char&r1, const unsigned char&g1, const unsigned char&b1);
    bool operator== (const PPM &rhs) const;
    bool operator!= (const PPM &rhs) const;
    bool operator>= (const PPM &rhs) const;
    bool operator<= (const PPM &rhs) const;
    bool operator> (const PPM &rhs) const;
    bool operator< (const PPM &rhs) const;
    PPM& operator+= (const PPM &rhs);
    PPM& operator-= (const PPM &rhs);
    PPM operator+ (const PPM &rhs) const;
    PPM operator- (const PPM &rhs) const;
    PPM& operator+= (const int &rhs);
    PPM& operator-= (const int &rhs);
    PPM operator+ (const int &rhs) const;
    PPM operator- (const int &rhs) const;
    PPM& operator*= (const double &rhs);
    PPM& operator/= (const double &rhs);
    PPM operator* (const double &rhs) const;
    PPM operator/ (const double &rhs) const;
    friend std::ostream &operator<<(std::ostream &os, const PPM &rhs);
    friend std::istream &operator>>(std::istream &is, PPM &rhs);
private:
    unsigned int mWidth;
    unsigned int mHeight;
    unsigned int mMaxColorValue;
    std::vector<unsigned char> mColors;
};
PPM blend( const PPM& src1, const PPM& src2 );
#endif

