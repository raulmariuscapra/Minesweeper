#include<cstdio>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include<graphics.h>
using namespace std;

int board[101][101];
int pozitiix[9]={-1,-1,-1,0,0,1,1,1},pozitiiy[9]={-1,0,1,-1,1,-1,0,1};
int boardrevealed[100][100],boardrevealedn,nrbombe;
int n,xmax,ymax,width,height,bgcolor=COLOR(235,235,235),culori[9]={COLOR(0,189,0),COLOR(0,164,189),COLOR(43,0,255),COLOR(189,183,0),COLOR(255,188,0),COLOR(255,34,0),COLOR(255,0,188),COLOR(154,0,255)};
bool over=false,terminare=false;

void generate_bomb()
{
    srand(time(NULL));
    int x,y,i;
    do
    {
        x=rand()%n;
        y=rand()%n;
    }
    while(board[x][y]==-1);
    board[x][y]=-1;
    for(i=0;i<9;++i)
    {
        if(x+pozitiix[i]>=0&&x+pozitiix[i]<n&&y+pozitiiy[i]>=0&&y+pozitiiy[i]<n&&board[x+pozitiix[i]][y+pozitiiy[i]]!=-1)
            board[x+pozitiix[i]][y+pozitiiy[i]]++;
    }
}

void setup()
{
    int i,j;
    srand(time(NULL));
    for(i=0;i<n;++i)
    {
        for(j=0;j<n;++j)
        {
            board[i][j]=0;
        }
    }
    for(i=1;i<=n*n/9;++i)
    {
        generate_bomb();
    }
}

void GetDesktopResolution(int& horizontal, int& vertical)
{
   RECT desktop;
   // Get a handle to the desktop window
   const HWND hDesktop = GetDesktopWindow();
   // Get the size of screen to the variable desktop
   GetWindowRect(hDesktop, &desktop);
   // The top left corner will have coordinates (0,0)
   // and the bottom right corner will have coordinates
   // (horizontal, vertical)
   horizontal = desktop.right;
   vertical = desktop.bottom;
}

void drawboard()
{
    cleardevice();
    int i,j;
    char ch[20];
    for(i=0;i<n;++i)
    {
        for(j=0;j<n;++j)
        {
            if(boardrevealed[i][j]==1)
            {
                if(board[i][j]==-1)
                {
                    circle(j*width+width/2,i*height+height/2,5);
                }
                else
                {
                    setfillstyle(1,COLOR(193,223,228));
                    bar(j*width,i*height,(j+1)*width,(i+1)*height);
                    if(board[i][j])
                    {
                        setbkcolor(COLOR(193,223,228));
                        setcolor(culori[board[i][j]-1]);
                        sprintf(ch,"%i",board[i][j]);
                        outtextxy(j*width+width/2,i*height+height/2,ch);
                        setcolor(BLACK);
                        setbkcolor(bgcolor);
                    }
                }
            }
            else if(boardrevealed[i][j]==2)
            {
                setcolor(culori[4]);
                circle(j*width+width/2,i*height+height/2,5);
                setcolor(BLACK);
            }
            rectangle(j*width,i*height,(j+1)*width,(i+1)*height);
        }
    }
    swapbuffers();
}

void reveal(int x,int y)
{
    int i;
    boardrevealed[y][x]=1;
    boardrevealedn--;
    if(board[y][x]==0)
    {
        for(i=0;i<9;++i)
            if(x+pozitiix[i]>=0&&x+pozitiix[i]<n&&y+pozitiiy[i]>=0&&y+pozitiiy[i]<n&&boardrevealed[y+pozitiiy[i]][x+pozitiix[i]]==0)
                reveal(x+pozitiix[i],y+pozitiiy[i]);
    }
}

int run()
{
    over=false;
    int x,y;
    bool clicked=false,lclicked=false,rclicked=false;
    while(!over&&boardrevealedn)
    {
        drawboard();
        while(!clicked)
        {
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                lclicked=true;
                clicked=true;
            }
            else if(ismouseclick(WM_RBUTTONDOWN))
            {
                rclicked=true;
                clicked=true;
            }
        }
        if(lclicked)
        {
            getmouseclick(WM_LBUTTONDOWN,x,y);
            x=x/width;
            y=y/height;
            if(board[y][x]==-1)
            {
                over=true;
                return 0;
            }
            else
            {
                if(boardrevealed[y][x]==0)
                    reveal(x,y);
            }
            lclicked=false;
        }
        else
        {
            getmouseclick(WM_RBUTTONDOWN,x,y);
            x=x/width;
            y=y/height;
            if(boardrevealed[y][x]==0&&nrbombe>0)
            {
                boardrevealed[y][x]=2;
                boardrevealedn--;
                nrbombe--;
            }
            else
            {
                if(boardrevealed[y][x]==2)
                {
                    boardrevealed[y][x]=0;
                    boardrevealedn++;
                    nrbombe++;
                }
            }
            rclicked=false;
        }
        clicked=false;
    }
    if(boardrevealedn==0)
    {
        return 1;
    }
    return 0;
}

void menu()
{
    bool selectat=false;
    int x,y;
    height=ymax/100;
    width=xmax/100;
    cleardevice();
    outtextxy(xmax/2-textwidth((char*)"Minesweeper"),height*5,(char*)"Minesweeper");
    outtextxy(width*20-textwidth((char*)"10x10"),height*35,(char*)"10x10");
    outtextxy(width*50-textwidth((char*)"20x20"),height*35,(char*)"20x20");
    outtextxy(width*80-textwidth((char*)"30x30"),height*35,(char*)"30x30");
    outtextxy(width*50-textheight((char*)"Quit!"),height*75,(char*)"Quit!");
    rectangle(width*10,height*30,width*30,height*40);
    rectangle(width*40,height*30,width*60,height*40);
    rectangle(width*70,height*30,width*90,height*40);
    rectangle(width*40,height*70,width*60,height*80);
    swapbuffers();
    while(!selectat)
    {
        while(!ismouseclick(WM_LBUTTONDOWN));
        getmouseclick(WM_LBUTTONDOWN,x,y);
        if(y>=height*30&&y<=height*40)
        {
            if(x>=width*10&&x<=width*30)
            {
                n=10;
                selectat=true;
            }
            else if(x>=width*40&&x<=width*60)
            {
                n=20;
                selectat=true;
            }
            else if(x>=width*70&&x<=width*90)
            {
                n=30;
                selectat=true;
            }
        }
        else if(y>=height*70&&y<=height*80)
        {
            if(x>=width*40&&x<=width*60)
            {
                terminare=true;
                selectat=true;
            }
        }

    }
}

void clean()
{
    height=ymax/n;
    width=xmax/n;
    setup();
    boardrevealedn=n*n;
    nrbombe=n*n/9;
    for(int i=0;i<n;++i)
    {
        for(int j=0;j<n;++j)
        {
            boardrevealed[i][j]=0;
        }
    }
}

void setupwindow()
{
    GetDesktopResolution(xmax,ymax);
    initwindow(xmax,ymax);
    setbkcolor(bgcolor);
    setcolor(BLACK);
    cleardevice();
}

int main()
{
    int outcome,x,y;
    setupwindow();
    while(!terminare)
    {
        menu();
        if(terminare)
            break;
        clean();
        outcome=run();
        if(!outcome)
        {
            cleardevice();
            outtext((char*)"You Lost");
        }
        else
        {
            cleardevice();
            outtext((char*)"You Won");
        }
        swapbuffers();
        while(!ismouseclick(WM_LBUTTONDOWN));
        getmouseclick(WM_LBUTTONDOWN,x,y);
    }
    return 0;
}
