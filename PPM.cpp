#include "PPM.h"
#include<string>
#include<vector>
void getcolor(unsigned int*colorout, int color){
    color %= 1530;
    if (color < 256){ // Red to yellow
       colorout[0] = 255;
       colorout[1] = color;
       colorout[2] = 0;
    }
    else if (color < 512){ // Yellow to green
       colorout[0] = 255-(color-256);
       colorout[1] = 255;
       colorout[2] = 0;
    }
    else if (color < 768){ // Green to teal
       colorout[0] = 0;
       colorout[1] = 255;
       colorout[2] = color-512;
    }
    else if (color < 1024){ // Teal to blue
       colorout[0] = 0;
       colorout[1] = 255-(color-768);
       colorout[2] = 255;
    }
    else if (color < 1280){ // Blue to purple
       colorout[0] = color-1024;
       colorout[1] = 0;
       colorout[2] = 255;
    }
    else if (color < 1536){ // Purple to red
       colorout[0] = 255;
       colorout[1] = 0;
       colorout[2] = 255-(color-1280);
    }
}
int mabs(int x){
    int t;
    t=x;
    if (t<0){
        t=-t;
    }
    return t;
}
PPM::PPM( )
  : mWidth( 0 ), mHeight( 0 ), mMaxColorValue( 0 ), mColors( 0 )  {
}
PPM::PPM( const PPM& ppm )
  : mWidth( 0 ), mHeight( 0 ), mMaxColorValue( 0 ), mColors( 0 )
{
    mWidth=ppm.mWidth;
    mHeight=ppm.mHeight;
    mMaxColorValue=ppm.mMaxColorValue;
    mColors.resize(mWidth*mHeight*3);
    for (unsigned int i=0;i<mColors.size();i++){
        mColors[i]=ppm.mColors[i];
    }
}
PPM::PPM( const PPM &ppm, const unsigned int&x, const unsigned int&y, const unsigned int&dx, const unsigned int&dy)
  : mWidth( dx ), mHeight( dy ), mMaxColorValue( 0 ), mColors( 0 )
{
    unsigned int i,j;
    mMaxColorValue=ppm.mMaxColorValue;
    mColors.resize(mWidth*mHeight*3);
    for (i=0;i<dx;i++){
        for (j=0;j<dy;j++){
            setChannel(j,i,0,ppm.getChannel(j+x,i+y,0));
            setChannel(j,i,1,ppm.getChannel(j+x,i+y,1));
            setChannel(j,i,2,ppm.getChannel(j+x,i+y,2));
        }
    }
}
int PPM::getWidth( ) const{
    return mWidth;
}
int PPM::getHeight( ) const{
    return mHeight;
}
int PPM::getMaxColorValue( ) const{
    return mMaxColorValue;
}
int PPM::getChannel( const unsigned int& row, const unsigned int& column, const unsigned int& channel ) const{
    if (row<mHeight && column<mWidth && channel<3){
        return mColors[(row * mWidth * 3 + column * 3) + channel];
    }
    return -1;
}
void PPM::setWidth( const int& width ){
    if (width>=0){
        mWidth=width;
        mColors.resize(mWidth*mHeight*3);
    }
}
void PPM::setHeight( const int& height ){
    if (height>=0){
        mHeight=height;
        mColors.resize(mWidth*mHeight*3);
    }
}
void PPM::setMaxColorValue( const int& max_color_value ){
    if (max_color_value>=0 && max_color_value<256){
        mMaxColorValue=max_color_value;
    }
}
void PPM::setChannel( const unsigned int& row, const unsigned int& column, const unsigned int& channel, const unsigned int& value ){
    if (row<mHeight && column<mWidth && channel<3 && value<=mMaxColorValue){
        mColors[(row * mWidth * 3 + column * 3) + channel]=value;
    }
}
void PPM::sepiaFilter( PPM& dst ) const{
    dst.setWidth(mWidth);
    dst.setHeight(mHeight);
    dst.setMaxColorValue(mMaxColorValue);
    unsigned int new_red;
    unsigned int new_green;
    unsigned int new_blue;
    for (unsigned long int i=0;i<mColors.size()/3;i++){
        new_red = 0.393*mColors[i*3] + 0.769*mColors[i*3+1] + 0.189*mColors[i*3+2];
        new_green = 0.349*mColors[i*3] + 0.686*mColors[i*3+1] + 0.168*mColors[i*3+2];
        new_blue = 0.272*mColors[i*3] + 0.534*mColors[i*3+1] + 0.131*mColors[i*3+2];
        if (new_red>dst.mMaxColorValue){
            new_red=dst.mMaxColorValue;
        }
        if (new_green>dst.mMaxColorValue){
            new_green=dst.mMaxColorValue;
        }
        if (new_blue>dst.mMaxColorValue){
            new_blue=dst.mMaxColorValue;
        }
        dst.mColors[i*3]=new_red;
        dst.mColors[i*3+1]=new_green;
        dst.mColors[i*3+2]=new_blue;
    }
}
PPM& PPM::line( const int x1, const int y1, const int x2, const int y2, const unsigned char r1, const unsigned char g1, const unsigned char b1 ){
    int x=x1;
    int y=y1;
    int w=x2-x1;
    int h=y2-y1;
    int dx1 = 0;
    int dy1 = 0;
    int dx2 = 0;
    int dy2 = 0;
    int i,longest,shortest,numerator;
    if (w < 0){
        dx1 = -1;
        dx2 = -1;
    }
    else if (w > 0){
        dx1 = 1;
        dx2 = 1;
    }
    if (h < 0){
        dy1 = -1;
    }
    else if (h > 0){
        dy1 = 1;
    }
    longest = mabs(w);
    shortest = mabs(h);
    if (!(longest>shortest)){
        longest = mabs(h);
        shortest = mabs(w);
        if (h<0){
            dy2 = -1;
        }
        else if (h > 0){
            dy2 = 1;
        }
        dx2 = 0;
    }
    numerator = longest >> 1;
    for (i=0;i<longest+1;i++){
        setChannel(y,x,0,r1);
        setChannel(y,x,1,g1);
        setChannel(y,x,2,b1);
        numerator += shortest;
        if (!(numerator<longest)){
            numerator -= longest;
            x += dx1;
            y += dy1;
        }
        else{
            x += dx2;
            y += dy2;
        }
    }
    return *this;
}
PPM& PPM::julia( double x, double y, double c, double a, double b, unsigned int m, double co ){
    unsigned int i,j,it;
    unsigned long int p=0;
    double nx,ny,cx,cy=y,px;//1530/m;
    unsigned int color[3]={0,0,0};
    for (i=0;i<mHeight;i++){
        cx=x;
        for (j=0;j<mWidth;j++){
            nx=cx;
            ny=cy;
            it=0;
            while (it<m&&nx*nx+ny*ny<4){
                px=nx;
                nx=nx*nx-ny*ny+a;
                ny=2*px*ny+b;
                it+=1;
            }
            getcolor(color, co*it);
            mColors[p]=color[0];
            mColors[p+1]=color[1];
            mColors[p+2]=color[2];
            cx+=c;
            p+=3;
        }
        cy-=c;
    }
    return *this;
}
PPM& PPM::juliaGrey( double x, double y, double c, double a, double b){
    unsigned int i,j,it;
    unsigned long int p=0;
    double nx,ny,cx,cy=y,px;
    for (i=0;i<mHeight;i++){
        cx=x;
        for (j=0;j<mWidth;j++){
            nx=cx;
            ny=cy;
            it=0;
            while (it<255&&nx*nx+ny*ny<4){
                px=nx;
                nx=nx*nx-ny*ny+a;
                ny=2*px*ny+b;
                it+=1;
            }
            mColors[p]=it;
            mColors[p+1]=it;
            mColors[p+2]=it;
            cx+=c;
            p+=3;
        }
        cy-=c;
    }
    return *this;
}
void PPM::fancyEdges( PPM& dst ) const {
    int ered   = 9*mMaxColorValue/10;
    int egreen = 7*mMaxColorValue/10;
    int eblue  = 1*mMaxColorValue/10;

    int red   = 1*mMaxColorValue/10;
    int green = 3*mMaxColorValue/10;
    int blue  = 9*mMaxColorValue/10;
    dst = PPM(*this);
    dst.horizontalEdges();
    dst.replace(mMaxColorValue, mMaxColorValue, mMaxColorValue, ered, egreen, eblue);
    dst.replace(0, 0, 0, red, green, blue);
}
PPM& PPM::grayFromRed (){
    int value;
    for (unsigned long int i=0;i<mColors.size()/3;i++){
        value=mColors[i*3];
        mColors[i*3+1]=value;
        mColors[i*3+2]=value;
    }
    return *this;
}
PPM& PPM::grayFromGreen (){
    int value;
    for (unsigned long int i=0;i<mColors.size()/3;i++){
        value=mColors[i*3+1];
        mColors[i*3]=value;
        mColors[i*3+2]=value;
    }
    return *this;
}
PPM& PPM::grayFromBlue (){
    int value;
    for (unsigned long int i=0;i<mColors.size()/3;i++){
        value=mColors[i*3+2];
        mColors[i*3]=value;
        mColors[i*3+1]=value;
    }
    return *this;
}
PPM& PPM::grayFromLinear (){
    int value;
    for (unsigned long int i=0;i<mColors.size()/3;i++){
        value=0.2126*mColors[i*3] + 0.7152*mColors[i*3+1] + 0.0722*mColors[i*3+2];
        mColors[i*3]=value;
        mColors[i*3+1]=value;
        mColors[i*3+2]=value;
    }
    return *this;
}
PPM& PPM::invert (){
    for (unsigned long int i=0;i<mColors.size()/3;i++){
        mColors[i*3]=mMaxColorValue-mColors[i*3];
        mColors[i*3+1]=mMaxColorValue-mColors[i*3+1];
        mColors[i*3+2]=mMaxColorValue-mColors[i*3+2];
    }
    return *this;
}
PPM& PPM::verticalEdges (){
    double value;
    double previousvalue;
    double maxdiffer;
    maxdiffer=mMaxColorValue/10.0;
    previousvalue=0.2126*mColors[0] + 0.7152*mColors[1] + 0.0722*mColors[2];
    mColors[0]=0;
    mColors[1]=0;
    mColors[2]=0;
    for (unsigned long int i=0;i<mColors.size()/3;i++){
        value=0.2126*mColors[i*3] + 0.7152*mColors[i*3+1] + 0.0722*mColors[i*3+2];
        if (i%mWidth!=0){
            if (mabs(value-previousvalue)>=maxdiffer){
                mColors[i*3]=mMaxColorValue;
                mColors[i*3+1]=mMaxColorValue;
                mColors[i*3+2]=mMaxColorValue;
            } else {
                mColors[i*3]=0;
                mColors[i*3+1]=0;
                mColors[i*3+2]=0;
            }
        } else {
            mColors[i*3]=0;
            mColors[i*3+1]=0;
            mColors[i*3+2]=0;
        }
        previousvalue=value;
    }
    return *this;
}
PPM& PPM::horizontalEdges (){
    double value;
    double previousvalue;
    int maxdiffer;
    unsigned long int currentindex;
    maxdiffer=mMaxColorValue/10.0;
    previousvalue=0.2126*mColors[0] + 0.7152*mColors[1] + 0.0722*mColors[2];
    mColors[0]=0;
    mColors[1]=0;
    mColors[2]=0;
    for (unsigned long int i=0;i<mColors.size()/3;i++){
        currentindex=i%mHeight*mWidth*3+i/mHeight*3;
        value=0.2126*mColors[currentindex] + 0.7152*mColors[currentindex+1] + 0.0722*mColors[currentindex+2];
        if (i%mHeight!=0){
            if (mabs(value-previousvalue)>=maxdiffer){
                mColors[currentindex]=mMaxColorValue;
                mColors[currentindex+1]=mMaxColorValue;
                mColors[currentindex+2]=mMaxColorValue;
            } else {
                mColors[currentindex]=0;
                mColors[currentindex+1]=0;
                mColors[currentindex+2]=0;
            }
        } else {
            mColors[currentindex]=0;
            mColors[currentindex+1]=0;
            mColors[currentindex+2]=0;
        }
        previousvalue=value;
    }
    return *this;
}
PPM& PPM::smooth(){
    unsigned int i;
    unsigned int j;
    unsigned int k;
    unsigned int n;
    unsigned int t;
    unsigned int c;
    PPM copy;
    copy=PPM(*this);
    int neighbors[8];
    for (i=0;i<mHeight;i++){
        for (j=0;j<mWidth;j++){
            for (k=0;k<3;k++){
                neighbors[0]=copy.getChannel( i-1, j-1, k );
                neighbors[1]=copy.getChannel( i-1, j, k );
                neighbors[2]=copy.getChannel( i-1, j+1, k );
                neighbors[3]=copy.getChannel( i, j-1, k );
                neighbors[4]=copy.getChannel( i, j+1, k );
                neighbors[5]=copy.getChannel( i+1, j-1, k );
                neighbors[6]=copy.getChannel( i+1, j, k );
                neighbors[7]=copy.getChannel( i+1, j+1, k );
                t=copy.getChannel( i, j, k );
                c=1;
                for (n=0;n<8;n++){
                    if (neighbors[n]!=-1){
                        t+=neighbors[n];
                        c+=1;
                    }
                }
                setChannel( i, j, k, t/c);
            }
        }
    }
    return *this;
}
PPM& PPM::blit(const PPM &p, const int&x, const int&y) {
    unsigned int i;
    unsigned int j;
    unsigned int k;
    for (i=0;i<p.mHeight;i++){
        for (j=0;j<p.mWidth;j++){
            for (k=0;k<3;k++){
                setChannel( i+y, j+x, k, p.getChannel( i, j, k ));
            }
        }
    }
    return *this;
}
PPM& PPM::blit(const PPM &p, const int&x, const int&y, const unsigned char&r, const unsigned char&g, const unsigned char&b) {
    unsigned int i;
    unsigned int j;
    unsigned int k;
    unsigned char color[3];
    color[0]=r;
    color[1]=g;
    color[2]=b;
    for (i=0;i<p.mHeight;i++){
        for (j=0;j<p.mWidth;j++){
            for (k=0;k<3;k++){
                if (p.getChannel( i, j, k )!=color[k]){
                    setChannel( i+y, j+x, k, p.getChannel( i, j, k ));
                }
            }
        }
    }
    return *this;
}
PPM& PPM::replace(const unsigned char&r1, const unsigned char&g1, const unsigned char&b1, const unsigned char&r2, const unsigned char&g2, const unsigned char&b2){
    unsigned char color1[3];
    unsigned char color2[3];
    color1[0]=r1;
    color1[1]=g1;
    color1[2]=b1;

    color2[0]=r2;
    color2[1]=g2;
    color2[2]=b2;
    for (unsigned long int i=0;i<mColors.size();i+=3){
        if (mColors[i]==color1[0]){
            if (mColors[i+1]==color1[1]){
                if (mColors[i+2]==color1[2]){
                    mColors[i]=color2[0];
                    mColors[i+1]=color2[1];
                    mColors[i+2]=color2[2];
                }
            }
        }
    }
    return *this;
}
PPM& PPM::horizontalReflection(){
    unsigned int i;
    unsigned int j;
    unsigned int k;
    unsigned char x;
    for (i=0;i<mHeight;i++){
        for (j=0;j<mWidth/2;j++){
            for (k=0;k<3;k++){
                x=getChannel( i, mWidth-j, k );
                setChannel( i, mWidth-j, k, getChannel( i, j, k ));
                setChannel( i, j, k, x);
            }
        }
    }
    return *this;
}
PPM& PPM::verticalReflection(){
    unsigned int i;
    unsigned int j;
    unsigned int k;
    unsigned char x;
    for (i=0;i<mHeight/2;i++){
        for (j=0;j<mWidth;j++){
            for (k=0;k<3;k++){
                x=getChannel( mHeight-i, j, k );
                setChannel( mHeight-i, j, k, getChannel( i, j, k ));
                setChannel( i, j, k, x);
            }
        }
    }
    return *this;
}
PPM& PPM::bucketFill(const unsigned int&x, const unsigned int&y, const unsigned char&r1, const unsigned char&g1, const unsigned char&b1){
    if (x<mWidth && y<mHeight){
        unsigned int backgroundcolor[3];
        backgroundcolor[0] = getChannel( y, x, 0 );
        backgroundcolor[1] = getChannel( y, x, 1 );
        backgroundcolor[2] = getChannel( y, x, 2 );

        unsigned int fillcolor[3];
        fillcolor[0] = r1;
        fillcolor[1] = g1;
        fillcolor[2] = b1;
        if (!(fillcolor[0]==backgroundcolor[0]&&fillcolor[1]==backgroundcolor[1]&&fillcolor[2]==backgroundcolor[2])){
            std::vector<int> xlocs(0);
            std::vector<int> ylocs(0);
            xlocs.push_back(x);
            ylocs.push_back(y);
            unsigned int currentcolor[3];
            unsigned int cx;
            unsigned int cy;
            while (xlocs.size()>0){
                cx=xlocs.back();
                xlocs.pop_back();
                cy=ylocs.back();
                ylocs.pop_back();
                if (cx<mWidth && cy<mHeight){
                    currentcolor[0] = getChannel( cy, cx, 0 );
                    currentcolor[1] = getChannel( cy, cx, 1 );
                    currentcolor[2] = getChannel( cy, cx, 2 );
                    if (backgroundcolor[0]==currentcolor[0]){
                        if (backgroundcolor[1]==currentcolor[1]){
                            if (backgroundcolor[2]==currentcolor[2]){
                                setChannel( cy, cx, 0, fillcolor[0]);
                                setChannel( cy, cx, 1, fillcolor[1]);
                                setChannel( cy, cx, 2, fillcolor[2]);
                                xlocs.push_back(cx-1);
                                ylocs.push_back(cy);

                                xlocs.push_back(cx);
                                ylocs.push_back(cy-1);

                                xlocs.push_back(cx+1);
                                ylocs.push_back(cy);

                                xlocs.push_back(cx);
                                ylocs.push_back(cy+1);
                            }
                        }
                    }
                }
            }
        }
    }
    return *this;
}
PPM& PPM::fill(const unsigned char&r1, const unsigned char&g1, const unsigned char&b1){
    for (unsigned long int i=0;i<mColors.size()/3;i++){
        mColors[i*3]=r1;
        mColors[i*3+1]=g1;
        mColors[i*3+2]=b1;
    }
    return *this;
}
bool PPM::operator== (const PPM &rhs) const
{
    return (mWidth*mHeight==rhs.mWidth*rhs.mHeight);
}
bool PPM::operator!= (const PPM &rhs) const
{
    return (mWidth*mHeight!=rhs.mWidth*rhs.mHeight);
}
bool PPM::operator>= (const PPM &rhs) const
{
    return (mWidth*mHeight>=rhs.mWidth*rhs.mHeight);
}
bool PPM::operator<= (const PPM &rhs) const
{
    return (mWidth*mHeight<=rhs.mWidth*rhs.mHeight);
}
bool PPM::operator> (const PPM &rhs) const
{
    return (mWidth*mHeight>rhs.mWidth*rhs.mHeight);
}
bool PPM::operator< (const PPM &rhs) const
{
    return (mWidth*mHeight<rhs.mWidth*rhs.mHeight);
}
PPM& PPM::operator+= (const PPM &rhs)
{
    unsigned int v;
    for (unsigned long int i=0;i<mColors.size();i++){
        v=mColors[i]+rhs.mColors[i];
        if (v>mMaxColorValue){
            v=mMaxColorValue;
        }
        mColors[i]=v;
    }
    return *this;
}
PPM& PPM::operator-= (const PPM &rhs)
{
    unsigned int v;
    for (unsigned long int i=0;i<mColors.size();i++){
        v=mColors[i]-rhs.mColors[i];
        if (v>mMaxColorValue){
            v=mMaxColorValue;
        }
        mColors[i]=v;
    }
    return *this;
}
PPM PPM::operator+ (const PPM &rhs) const
{
    PPM result;
    result=PPM(*this);
    result += rhs;
    return result;
}
PPM PPM::operator- (const PPM &rhs) const
{
    PPM result;
    result=PPM(*this);
    result -= rhs;
    return result;
}
PPM& PPM::operator+= (const int &rhs)
{
    unsigned int v;
    for (unsigned long int i=0;i<mColors.size();i++){
        v=mColors[i]+rhs;
        if (v>mMaxColorValue){
            v=mMaxColorValue;
        }
        mColors[i]=v;
    }
    return *this;
}
PPM& PPM::operator-= (const int &rhs)
{
    unsigned int v;
    for (unsigned long int i=0;i<mColors.size();i++){
        v=mColors[i]-rhs;
        if (v>mMaxColorValue){
            v=mMaxColorValue;
        }
        mColors[i]=v;
    }
    return *this;
}
PPM PPM::operator+ (const int &rhs) const
{
    PPM result;
    result=PPM(*this);
    result += rhs;
    return result;
}
PPM PPM::operator- (const int &rhs) const
{
    PPM result;
    result=PPM(*this);
    result -= rhs;
    return result;
}
PPM& PPM::operator*= (const double &rhs)
{
    unsigned int v;
    for (unsigned long int i=0;i<mColors.size();i++){
        v=mColors[i]*rhs;
        if (v>mMaxColorValue){
            v=mMaxColorValue;
        }
        mColors[i]=v;
    }
    return *this;
}
PPM& PPM::operator/= (const double &rhs)
{
    unsigned int v;
    for (unsigned long int i=0;i<mColors.size();i++){
        v=mColors[i]/rhs;
        if (v>mMaxColorValue){
            v=mMaxColorValue;
        }
        mColors[i]=v;
    }
    return *this;
}
PPM PPM::operator* (const double &rhs) const
{
    PPM result;
    result=PPM(*this);
    result *= rhs;
    return result;
}
PPM PPM::operator/ (const double &rhs) const
{
    PPM result;
    result=PPM(*this);
    result /= rhs;
    return result;
}

std::ostream &operator<<(std::ostream &os, const PPM &p) {
    char code;
    os << "P6 " << p.mWidth << " " << p.mHeight << " " << p.mMaxColorValue << "\n";
    //os.write((char *)p.mColors[0],p.mColors.size());
    for (unsigned long int i=0;i<p.mColors.size();i++){
        code=p.mColors[i];
        os.write(&code,1);
    }
    return os;
}

std::istream &operator>>(std::istream &is, PPM &p)
{
    char identifier[3] = {0,0,0};
    is >> identifier;
    is >> p.mWidth;
    is >> p.mHeight;
    is >> p.mMaxColorValue;
    is.read(identifier,1);
    p.mColors.resize(p.mWidth*p.mHeight*3);
    //is.read((char *)p.mColors[0],p.mColors.size());
    unsigned char code = 0;
    for (unsigned long int i=0;i<p.mColors.size();i++){
        is.read((char *)&code,1);
        code=code%256;
        p.mColors[i]=code;
    }
    return is;
}
PPM blend( const PPM& src1, const PPM& src2 ){
    return (src1*.5)+(src2*.5);
}

