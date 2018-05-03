#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctime>
using namespace std;
/********************************************************
*	Point.h : 棋盘点类                                  *
*	张永锋                                              *
*	zhangyf07@gmail.com                                 *
*	2010.8                                              *
*********************************************************/
#define GEN 3
#define RUNTRY 50
#define RUNDEEP -30
int vacant;
//int warn[13][13],warnx[13][13],warny[13][13],warnnum;

class Point{
public:
	int x;
	int y;

	Point(int x, int y){
		this->x = x;
		this->y = y;
	}
};


bool userWin(const int x, const int y, const int M, const int N, int* const* board){
	//横向检测
    int i, j;
    int count = 0;
    for (i = y; i >= 0; i--)
        if (!(board[x][i] == 1))
            break;
    count += (y - i);
    for (i = y; i < N; i++)
        if (!(board[x][i] == 1))
            break;
    count += (i - y - 1);
    if (count >= 4) return true;

    //纵向检测
    count = 0;
    for (i = x; i < M; i++)
        if (!(board[i][y] == 1))
            break;
    count += (i - x);
    if (count >= 4) return true;

    //左下-右上
    count = 0;
    for (i = x, j = y; i < M && j >= 0; i++, j--)
        if (!(board[i][j] == 1))
            break;
    count += (y - j);
    for (i = x, j = y; i >= 0 && j < N; i--, j++)
        if (!(board[i][j] == 1))
            break;
    count += (j - y - 1);
    if (count >= 4) return true;

    //左上-右下
    count = 0;
    for (i = x, j = y; i >= 0 && j >= 0; i--, j--)
        if (!(board[i][j] == 1))
            break;
    count += (y - j);
    for (i = x, j = y; i < M && j < N; i++, j++)
        if (!(board[i][j] == 1))
            break;
    count += (j - y - 1);
    if (count >= 4) return true;

    return false;
}

bool machineWin(const int x, const int y, const int M, const int N, int* const* board){
	//横向检测
    int i, j;
    int count = 0;
    for (i = y; i >= 0; i--)
        if (!(board[x][i] == 2))
            break;
    count += (y - i);
    for (i = y; i < N; i++)
        if (!(board[x][i] == 2))
            break;
    count += (i - y - 1);
    if (count >= 4) return true;

    //纵向检测
    count = 0;
    for (i = x; i < M; i++)
        if (!(board[i][y] == 2))
            break;
    count += (i - x);
    if (count >= 4) return true;

    //左下-右上
    count = 0;
    for (i = x, j = y; i < M && j >= 0; i++, j--)
        if (!(board[i][j] == 2))
            break;
    count += (y - j);
    for (i = x, j = y; i >= 0 && j < N; i--, j++)
        if (!(board[i][j] == 2))
            break;
    count += (j - y - 1);
    if (count >= 4) return true;

    //左上-右下
    count = 0;
    for (i = x, j = y; i >= 0 && j >= 0; i--, j--)
        if (!(board[i][j] == 2))
            break;
    count += (y - j);
    for (i = x, j = y; i < M && j < N; i++, j++)
        if (!(board[i][j] == 2))
            break;
    count += (j - y - 1);
    if (count >= 4) return true;

    return false;
}

bool isTie(const int N, const int* top){
	bool tie = true;
    for (int i = 0; i < N; i++)
    {
        if (top[i] > 0)
        {
            tie = false;
            break;
        }
    }
    return tie;
}

void clearArray(int M, int N, int** board){
	for(int i = 0; i < M; i++){
		delete[] board[i];
	}
	delete[] board;
}

void printboard(int M, int N, int** board, int* top)
{
    for(int i=0;i<M;++i)
    {
        for(int j=0;j<N;++j)
        {
            if(board[i][j]!=0){ printf("%d ",board[i][j]);}else {printf("  ");}
        }
        printf("\n");
    }
}
/*
void printboard(int M, int N, int** board, int* top)
{
    int sum1=0,sum2=-2,flag=0,flag2=0;
    for(int i=0;i<M;++i)
    {
        for(int j=0;j<N;++j)
        {
            if(board[i][j]!=0){ printf("%d ",board[i][j]);}else {printf("  ");}
            if(board[i][j]!=0 && i<M-1 && board[i+1][j]==0)flag=1;
            if(board[i][j]==1) sum1++;
            if(board[i][j]==2) sum2++;
        }
        printf("\n");
    }
    for(int j=0;j<N;++j)
    {
//        printf("%d ",top[j]);
        for(int i=0;i<top[j];++i)
            if(board[i][j]!=0)flag2=1;
    }

    if(flag==1){printf("boarderror");getchar();}
    if(flag2==1){printf("toperror");getchar();}
    if(sum1-sum2>1){printf("1>2!%d %d\n",sum1,sum2);getchar();}
    if(sum1-sum2==1){printf("1b12duo1!%d %d\n",sum1,sum2);}
    if(sum1==sum2){printf("1==2!%d %d\n",sum1,sum2);}
    if(sum1-sum2==-1){printf("2bi1duo1!%d %d",sum1,sum2);}
    if(sum1-sum2<-1){printf("2>1!%d %d",sum1,sum2);getchar();}
//    if(sum>9) getchar();
}
*/
/*
	策略函数接口,该函数被对抗平台调用,每次传入当前状态,要求输出你的落子点,该落子点必须是一个符合游戏规则的落子点,不然对抗平台会直接认为你的程序有误
	
	input:
		为了防止对对抗平台维护的数据造成更改，所有传入的参数均为const属性
		M, N : 棋盘大小 M - 行数 N - 列数 均从0开始计， 左上角为坐标原点，行用x标记，列用y标记
		top : 当前棋盘每一列列顶的实际位置. e.g. 第i列为空,则_top[i] == M, 第i列已满,则_top[i] == 0
		_board : 棋盘的一维数组表示, 为了方便使用，在该函数刚开始处，我们已经将其转化为了二维数组board
				你只需直接使用board即可，左上角为坐标原点，数组从[0][0]开始计(不是[1][1])
				board[x][y]表示第x行、第y列的点(从0开始计)
				board[x][y] == 0/1/2 分别对应(x,y)处 无落子/有用户的子/有程序的子,不可落子点处的值也为0
		lastX, lastY : 对方上一次落子的位置, 你可能不需要该参数，也可能需要的不仅仅是对方一步的
				落子位置，这时你可以在自己的程序中记录对方连续多步的落子位置，这完全取决于你自己的策略
		noX, noY : 棋盘上的不可落子点(注:其实这里给出的top已经替你处理了不可落子点，也就是说如果某一步
				所落的子的上面恰是不可落子点，那么UI工程中的代码就已经将该列的top值又进行了一次减一操作，
				所以在你的代码中也可以根本不使用noX和noY这两个参数，完全认为top数组就是当前每列的顶部即可,
				当然如果你想使用lastX,lastY参数，有可能就要同时考虑noX和noY了)
		以上参数实际上包含了当前状态(M N _top _board)以及历史信息(lastX lastY),你要做的就是在这些信息下给出尽可能明智的落子点
	output:
		你的落子点Point
*/

                //walk(M,N,top,board,lastX,lastY,noX,noY,lastX,lastY+1,value,side,ans);
int force(const int M, const int N, int* top, int** board,
	int lastX, int lastY, const int noX, const int noY, int* req, int* req2, int& reqf, int& req2f, int side, int dev); //1 能赢 -1 投降 0 平
inline int bd(int M,int N,int lastX,int lastY,int** board,int xd, int yd, int dev)
{
    int ret=lastX+xd+yd*dev;
    if(ret<0 || ret>=M) return -10;
    return board[ret][lastY+yd];
}
void walk(const int M, const int N, int* top,  int** board, 
	int lastX, int lastY, const int noX, const int noY, int &x, int &y, double value, int side, int gen, double &ans );
bool seetop(const int M, const int N, int* top,  int** board, 
	int lastX, int lastY, const int noX, const int noY,int side) //自己的(1 能赢 -1 投降 0 平(对自己))
{

    lastX--;if(lastX==noX && lastY==noY) lastX--; if(lastX<0) return false;
//    if(board[lastX+1][lastY]!=0 || (top[lastY]!=lastX && (noY!=lastY || noX!=lastX+1))    || board[lastX][lastY]==0){printf("errorseetop%d %d\n",lastX-1,lastY);getchar();}

    board[lastX][lastY]=side;
   // printboard(M,N,board,top);printf("seetopboard\n");
    bool ret=(side==1 && userWin(lastX,lastY,M,N,board) || side==2 && machineWin(lastX,lastY,M,N,board));
    board[lastX][lastY]=0;
    return ret;
}
inline void step(const int M, const int N, int* top,  int** board, //已知即将落子lastx lasty情况下,在这里落下这颗子并且探索ans
	int lastX, int lastY, const int noX, const int noY, int x, int y, double value, int side, int gen, double &ans ) //自己的(1 能赢 -1 投降 0 平(对自己))
{
	if(ans==0)
	{
		ans=0;
	}
	if(value==0)
	{
		value=0;
	}
    if(ans==1) {return;}

  	//getchar();
    if(top[lastY]==0 || (top[lastY]==1 && noY==lastY && noX==0))return;
    
     if(vacant==0){
	 ans=0;return;}
	 vacant--;
    
    top[lastY]--;if(lastY==noY && top[lastY]==noX) top[lastY]--;
//    if(lastX!=top[lastY] || board[lastX][lastY]!=0) {printf("toperror! %d %d %d\n",lastX,lastY,top[lastY]);getchar();}
    board[lastX][lastY]=3-side;
    // printf("step:%d %d gen:%d\n",lastX,lastY,gen);printboard(M,N,board,top);//if(gen==5)getchar();

    int oldlastX=lastX,oldlastY=lastY;

//    int ma=0;for(int i=0;i<M;i++)for(int j=0;j<N;j++)if(board[i][j]!=0)ma++;if(ma>12-gen){printf("overcount %d",ma);getchar();}
    if(gen==0)
    {
        double sum=0;
		//printboard(M,N,board,top);//getchar();
        for(int i=0;i<RUNTRY;++i)
        {
            ans=-1;
            walk(M,N,top,board,lastX,lastY,noX,noY,x,y,value,3-side,RUNDEEP,ans);//changeans
            sum+=ans;
        //    if(ans!=0){printf("runans!=0 %lf",ans);getchar();}
            
        }
        ans=sum/double(RUNTRY);
        if(ans>0)
        {
        	ans=ans;
        }
        if(ans<0)
        {
        	ans=ans;
        }
        if(ans==0)
        {
        	ans=ans;
        }
        //ans=ans;
        
    }
    else if(gen<0)
        walk(M,N,top,board,lastX,lastY,noX,noY,x,y,value,3-side,gen+1,ans);
    else
    {
        walk(M,N,top,board,lastX,lastY,noX,noY,x,y,value,3-side,gen-1,ans);
        if(ans==0)
        {
        //	 printf("step:%d %d gen:%d\n",lastX,lastY,gen);printboard(M,N,board,top);//if(gen==5)getchar();

        	ans=ans;
        }
         if(ans!=0)
        {
        //	 printf("step:%d %d gen:%d\n",lastX,lastY,gen);printboard(M,N,board,top);//if(gen==5)getchar();

        	ans=ans;
        }
    }
    ans=-ans;
   // printf("stepf:%d %d %d %d ans:%d gen:%d\n",lastX,lastY,x,y,ans,gen);//if(gen==5)getchar();
  //  ma=0;for(int i=0;i<M;i++)for(int j=0;j<N;j++)if(board[i][j]!=0)ma++;if(ma>12-gen){printf("overcount %d",ma);getchar();}
    //printboard(M,N,board,top);
    board[lastX][lastY]=0;
    top[lastY]++;if(lastY==noY && top[lastY]==noX) top[lastY]++;
    vacant++;
}

void walk(const int M, const int N, int* top,  int** board, 
	int lastX, int lastY, const int noX, const int noY, int &x, int &y, double value, int side, int gen, double &ans ) //自己的(1 能赢 -1 投降 0 平(对自己))
{
	if(ans>0 && ans<1)
        {
        	ans=ans;
        }
        if(ans<0 && ans>-1)
        {
        	ans=ans;
        }
        if(ans==0)
        {
        	ans=ans;
        }
//	if(gen<0){
//	ans=0;
//	printboard(M,N,board,top);
//	printf("gen=%d",gen);
	//getchar();
//	}
	if(gen==-43)
	{
		gen=-43;
	}
    if(gen==-2){
	ans=0;
//	printboard(M,N,board,top);
//	printf("gen=-1");
//	getchar();
	return;}//run
ans=-1;
//        return run(M,N,top,board,lastX,lastY+1,noX,noY,x,y,value,side,gen);
    if(top[lastY]<0) {printf("errorovertop\n");getchar();}

    int reqf[3],req2f[3],req[3][3],req2[3][3],ans2[3],f4=0,f3=-1;
    //竖着的
    if(lastX<M-2 && bd(M,N,lastX,lastY,board,+1,0,0)==side && bd(M,N,lastX,lastY,board,+2,0,0)==side && lastX>0 && (noX!=lastX-1 || noY!=lastY)){f4=1;x=lastX-1;y=lastY;}
    
    //横着的
    for(int dev=0;dev<=2;++dev)
    {
        ans2[dev]=force(M,N,top,board,lastX,lastY,noX,noY,req[dev],req2[dev],reqf[dev],req2f[dev],side,dev-1);
    //    printf("ans2force:%d %d\n",dev,ans2[dev]);//getchar();
        if(ans2[dev]==1)
        {
            if(f4==0){x=lastX+(dev-1)*req[dev][0];y=lastY+req[dev][0];f4=1;}
            else if (x!=lastX+(dev-1)*req[dev][0] || y!=lastY+req[dev][0]) {ans=-1;
           // printf("-1muti4\n");printboard(M,N,board,top);
            return;}
        }
         if (ans2[dev]==2)//垫起来送4了 这里是对手给自己垫起来
        {
            if(f4==0){x=lastX-1;y=lastY;f4=1;}
            else if (x!=lastX-1 || y!=lastY) {ans=-1;
          //  printf("-1multi2\n");printboard(M,N,board,top);
            return;}
        }
        if (ans2[dev]==22)//遇到noxnoy 这里是对手给自己垫起来
        {
            if(f4==0){x=lastX-2;y=lastY;f4=1;}
            else if (x!=lastX-2 || y!=lastY) {ans=-1;
         //   printf("-1multi2\n");printboard(M,N,board,top);
            return;}
        }

    }
    if(f4==1)
    {
//       printf("\n4callstep %d %d\n",x,y);printboard(M,N,board,top);//if(gen==5)getchar();
    //    if(warn[x][y]==side){printf("warnconcede %d %d %d\n",x,y,side);for(int i=0;i<M;++i,printf("\n"))for(int j=0;j<N;++j)printf("%d ",warn[i][j]);printboard(M,N,board);ans=-1;return;}//对手走warn格子连4是能检测到的.这里只管自己不要踩到.
       if(seetop(M,N,top,board,x,y,noX,noY,side)) {ans=-1;return;}
	    if(gen<0)
            step(M,N,top,board,x,y,noX,noY,x,y,value,side,gen-1,ans);
        else
            step(M,N,top,board,x,y,noX,noY,x,y,value,side,gen+1,ans);
 //       printf("\nfinish4callstep %d %d ans:%d gen:%d\n",x,y,ans,gen);//if(gen==5)getchar();
        return;
    }
    for(int dev=0;dev<=2;++dev)
    {
        if(ans2[dev]==3)
        {
            if(f3==-1)
            {
                f3=dev;
                /*
                for(int i=0;i<reqf[f3];++i)
                {
     //               printf("+%d %d %d %d\n",reqf[f3],lastX+req[f3][i]*(f3-1),lastY+req[f3][i],f3);fflush(stdout);
                    if(warn[lastX+req[f3][i]*(f3-1)][lastY+req[f3][i]]==side){reqf[f3]--;req[f3][i]=req[f3][reqf[f3]];printf("wannfix%d\n",reqf[f3]);}
       //             printf("-");fflush(stdout);
                }
                if(reqf[f3]==0){printf("warn3\n");ans=-1;return;}*/
            }
            else
            {
 //               printf("cdr!\n");
                for(int i=0;i<reqf[f3];++i)
                {
                    int j;
                    for(j=0;j<reqf[dev];++j)
                        if(req[f3][i]==req[dev][j]) break;
                    if(j==reqf[dev]){reqf[f3]--;req[f3][i]=req[f3][reqf[f3]];
                   // printf("3fix\n");
                    }
                }
                if(reqf[f3]==0){
                    ans=-1;
               // printf("multi3\n");
                return;
                }
            }
        }
    }
    if(f3>=0)
    {
        //cut--v--
        if(reqf[f3]==0){
            ans=-1;
         //   printf("multi3cut\n");
            return;
            }
        double oldans=-1;
        if(gen<0)
        {
            int i0=rand()%reqf[f3];int i=i0;
            do
            {
            	++i;
            	if(i>=reqf[f3])i=0;
				x=lastX+req[f3][i]*(f3-1);y=lastY+req[f3][i];
				if(seetop(M,N,top,board,x,y,noX,noY,side)) continue;
           		step(M,N,top,board,x,y,noX,noY,x,y,value,side,gen-1,ans);
       	//	    printf("return3ans-gen<0 %lf\n",ans);
            	return;
            }while(i!=i0);
            ans=-1;return;
        }

        for(int i=0;i<reqf[f3];++i)
        {
            //getchar();
            x=lastX+req[f3][i]*(f3-1);y=lastY+req[f3][i];
            if(seetop(M,N,top,board,x,y,noX,noY,side)) continue;
            step(M,N,top,board,x,y,noX,noY,x,y,-oldans,side,gen+1,ans);
            if(ans>value){ans=value;
	//		printf("ans>value %d\n",ans);
			return;}
            if(oldans<ans) {oldans=ans;}
        }
        ans=oldans;
    //    printf("return3ans %lf\n",ans);
        return;
    }
    else
    {
        double oldans=-1;
     /*
        if(gen<0)
        {
            int i;do{i=rand()%N;}while(top[i]<=0 || i==noY && top[i]==1 && noX==0);
            x=top[i]-1;y=i;if(x==noX && y==noY) --x;
            step(M,N,top,board,x,y,noX,noY,x,y,value,side,gen,ans);
            printf("gen<0stepfinish\n");
            return;
        }
       
        if(gen==0)
        {
            for(int i=0;i<N;++i)
            {
                if(top[i]<=0 || i==noY && top[i]==1 && noX==0)continue;// || warn[top[i]-1][i]==side)continue;
            step(M,N,top,board,lastX+req[f3][i]*(f3-1),lastY+req[f3][i],noX,noY,x,y,-oldans,side,gen+1,ans);
                int newvalue=rand();
                if(newvalue>value)
                {
                    value=newvalue;
                    x=top[i]-1;y=i;if(x==noX && y==noY) --x;
                }
            }
            if(value==-1){
                printf("gen0no2error\n");
                getchar();
                    ans=-1;
                        }else ans=0;
            return;
        }*/
        if(gen<0)
        {
            int i0=rand()%N;int i=i0;
            do
            {
            	++i;
            	if(i>=N)i=0;

				if(top[i]<=0 || i==noY && top[i]==1 && noX==0)continue;// || warn[top[i]-1][i]==side)continue;
            x=top[i]-1;y=i;if(x==noX && y==noY) --x;
//            printf("\n2callstep %d %d gen:%d\n",top[i]-1,i,gen);
			if(seetop(M,N,top,board,x,y,noX,noY,side)) continue;
            step(M,N,top,board,x,y,noX,noY,x,y,-oldans,side,gen,ans);
            
            
		
       //		    printf("return4ans-gen<0 %lf\n",ans);
            	return;
            }while(i!=i0);
            ans=-1;return;
        }
        for(int i=0;i<N;++i)
        {
            if(top[i]<=0 || i==noY && top[i]==1 && noX==0)continue;// || warn[top[i]-1][i]==side)continue;
            x=top[i]-1;y=i;if(x==noX && y==noY) --x;
//            printf("\n2callstep %d %d gen:%d\n",top[i]-1,i,gen);
			if(seetop(M,N,top,board,x,y,noX,noY,side)) continue;
            step(M,N,top,board,x,y,noX,noY,x,y,-oldans,side,gen,ans);
  //          printf("\n2callend %d %d ans:%d gen:%d\n",top[i]-1,i,ans,gen);
            if(ans>value) {ans=value;
	//		printf("ans>value");
			return;}
            if(oldans<ans) {
			x=top[i]-1;y=i;oldans=ans;
			if(oldans==0)
			{
			oldans++;
			oldans--;
		}
			if(oldans!=0)
			{
				
			oldans++;
			oldans--;
		}
			}
            /*
            else if(oldans<0 && ans==0)   {x=top[i]-1;y=i;oldans=ans;value=rand();}
            else if(oldans==0 && ans==0)
            {
                int newvalue=rand();
                if(newvalue>value)
                {
                    value=newvalue;
                    x=top[i]-1;y=i;if(x==noX && y==noY) --x;
                }
            }*/
        }
        ans=oldans;
 //       printf("ret4ans %lf\n",ans);
    }
   // printf("returns\n");
    return;
}

/*
        printf("force gen:%d lastX:%d lastY:%d ans2:%d",gen,lastX,lastY, ans2);
        printboard(M,N,board);
        getchar();
        if(ans2==1) {x=lastX;y=lastY;ans=1;break;}
        if(ans2==0 && ans==-1) {x=lastX;y=lastY;ans=0;}
    }
*/
//    if(ans==-1) {printf("ans-1");getchar();}

/*
    int oldx=lastX,oldy=lastY;
    for(lastY=0;lastY<N;++lastY)
    {
        if(top[lastY]==0) continue;
        walk(M,N,top,board,lastX,lastY,noX,noY,x,y,value,side,gen,ans);
        printf("walks ans:%d",ans);
    }
    return -ans;*/
inline bool topt(int noX, int noY,int dev,int* top,int lastX,int lastY,int yd, int xd)
{
    return(top[lastY+yd]-yd*dev==lastX+xd && (lastY+yd!=noY || lastX+xd+yd*dev!=noX) || top[lastY+yd]-yd*dev==lastX+xd+1 && lastY+yd==noY && lastX+xd+yd*dev==noX);
}

int force(const int M, const int N, int* top,  int** board, 
	int lastX, int lastY, const int noX, const int noY, int* req, int* req2, int& reqf, int& req2f, int side, int dev) //0 安全 1 横着3字 3 两字 多选 -1 投降 0 平
{
  //  printf("foice");
    int ans=-1;//还没计算过
    reqf=0;req2f=0;
    //先看自己能不能连四个

    //再看对手的三个
    //上下
    //左右
    if(lastY<N-2 && bd(M,N,lastX,lastY,board,0,+1,dev)==side && bd(M,N,lastX,lastY,board,0,+2,dev)==side)//_xoo_
    {
        if(lastY>0)
        {
            if(topt(noX,noY,dev,top,lastX,lastY,-1,+1))
            {
                if(lastY<N-3 && topt(noX,noY,dev,top,lastX,lastY,+3,+1))return -1;else {req[0]=-1;reqf=1;return 1;}
            }
        }
        if(lastY<N-3)
        {
            if(topt(noX,noY,dev,top,lastX,lastY,+3,+1)){req[0]=3;reqf=1;return 1;}
        }
    }
    //printf("\n%d %d %d %d %d",lastX,lastY,bd(M,N,lastX,lastY,board,0,-2,dev),bd(M,N,lastX,lastY,board,0,-3,dev),  side);
    if(lastY<N-3 && bd(M,N,lastX,lastY,board,0,+1,dev)==side && bd(M,N,lastX,lastY,board,0,+3,dev)==side)
        if(topt(noX,noY,dev,top,lastX,lastY,+2,+1)){req[0]=2;reqf=1;return 1;}//xo_o

    if(lastY<N-2 && bd(M,N,lastX,lastY,board,0,+2,dev)==side && lastY>0 && bd(M,N,lastX,lastY,board,0,-1,dev)==side)
        if(topt(noX,noY,dev,top,lastX,lastY,+1,+1)){req[0]=1;reqf=1;return 1;}//ox_o

    if(lastY>=3 && bd(M,N,lastX,lastY,board,0,-2,dev)==side && bd(M,N,lastX,lastY,board,0,-3,dev)==side)
        if(topt(noX,noY,dev,top,lastX,lastY,-1,+1)){req[0]=-1;reqf=1;return 1;}//oo_x

    if(lastY<N-3 && bd(M,N,lastX,lastY,board,0,+2,dev)==side && bd(M,N,lastX,lastY,board,0,+3,dev)==side)
        if(topt(noX,noY,dev,top,lastX,lastY,+1,+1)){req[0]=1;reqf=1;return 1;}//x_oo

    if(lastY<N-1 && bd(M,N,lastX,lastY,board,0,+1,dev)==side && lastY>=2 && bd(M,N,lastX,lastY,board,0,-2,dev)==side)
        if(topt(noX,noY,dev,top,lastX,lastY,-1,+1)){req[0]=-1;reqf=1;return 1;}//o_xo

    if(lastY>=3 && bd(M,N,lastX,lastY,board,0,-1,dev)==side && bd(M,N,lastX,lastY,board,0,-3,dev)==side)
        if(topt(noX,noY,dev,top,lastX,lastY,-2,+1)){req[0]=-2;reqf=1;return 1;}//o_ox

    if(lastY<N-1 && bd(M,N,lastX,lastY,board,0,+1,dev)==side && lastY>0 && bd(M,N,lastX,lastY,board,0,-1,dev)==side)//_oxo_
    {
        if(lastY>1)
        {
            if(topt(noX,noY,dev,top,lastX,lastY,-2,+1))
            {
                if(lastY<N-2 && topt(noX,noY,dev,top,lastX,lastY,+2,+1))return -1;else {req[0]=-2;reqf=1;return 1;}
            }
        }
        if(lastY<N-2)
        {
            if(topt(noX,noY,dev,top,lastX,lastY,+2,+1)){req[0]=2;reqf=1;return 1;}
        }
    }
    if(lastY>1 && bd(M,N,lastX,lastY,board,0,-1,dev)==side && bd(M,N,lastX,lastY,board,0,-2,dev)==side)//_oox_
    {
        if(lastY>2)
        {
            if(topt(noX,noY,dev,top,lastX,lastY,-3,+1))
            {
                if(lastY<N-1 && topt(noX,noY,dev,top,lastX,lastY,+1,+1))return -1;else {req[0]=-3;reqf=1;return 1;}
            }
        }
        if(lastY<N-1)
        {
            if(topt(noX,noY,dev,top,lastX,lastY,+1,+1)){req[0]=1;reqf=1;return 1;}
        }
    }
    /*
    if(lastY<N-1 && bd(M,N,lastX,lastY,board,0,+1,dev)==side && lastY>1 && bd(M,N,lastX,lastY,board,0,-2,dev)==side)
        if(topt(noX,noY,dev,top,lastX,lastY,-1,+1)){req[0]=-1;reqf=1;return 1;}//o_xo
    if(lastY>2 && bd(M,N,lastX,lastY,board,0,-3,dev)==side && bd(M,N,lastX,lastY,board,0,-2,dev)==side)
        if(topt(noX,noY,dev,top,lastX,lastY,-1,+1)){req[0]=-1;reqf=1;return 1;}//oo_x
    if(lastY>2 && bd(M,N,lastX,lastY,board,0,-3,dev)==side && bd(M,N,lastX,lastY,board,0,-1,dev)==side)
        if(topt(noX,noY,dev,top,lastX,lastY,-2,+1)){req[0]=-2;reqf=1;return 1;}//o_ox
            */
        //重新触发 
    if(lastX>0)
    {
        if(lastX-1==noX && lastY==noY && lastX>=2)
        {
            if(lastY<M-3 && bd(M,N,lastX,lastY,board,-2,+1,dev)==side && bd(M,N,lastX,lastY,board,-2,+2,dev)==side && bd(M,N,lastX,lastY,board,-2,+3,dev)==side){req[0]=-2;reqf=1;return 22;}//=ooo
            if(lastY<M-2 && bd(M,N,lastX,lastY,board,-2,+1,dev)==side && bd(M,N,lastX,lastY,board,-2,+2,dev)==side && lastY>=1  && bd(M,N,lastX,lastY,board,-2,-1,dev)==side){req[0]=-2;reqf=1;return 22;}//o=oo
            if(lastY<M-1 && bd(M,N,lastX,lastY,board,-2,+1,dev)==side && lastY>=2 && bd(M,N,lastX,lastY,board,-2,-2,dev)==side  && bd(M,N,lastX,lastY,board,-2,-1,dev)==side){req[0]=-2;reqf=1;return 22;}//oo=o
            if(lastY>=3 && bd(M,N,lastX,lastY,board,-2,-1,dev)==side && bd(M,N,lastX,lastY,board,-2,-2,dev)==side && bd(M,N,lastX,lastY,board,-2,-3,dev)==side){req[0]=-2;reqf=1;return 22;}//=ooo
        }
        else
        {
            if(lastY<M-3 && bd(M,N,lastX,lastY,board,-1,+1,dev)==side && bd(M,N,lastX,lastY,board,-1,+2,dev)==side && bd(M,N,lastX,lastY,board,-1,+3,dev)==side){req[0]=-1;reqf=1;return 2;}//=ooo
            if(lastY<M-2 && bd(M,N,lastX,lastY,board,-1,+1,dev)==side && bd(M,N,lastX,lastY,board,-1,+2,dev)==side && lastY>=1  && bd(M,N,lastX,lastY,board,-1,-1,dev)==side){req[0]=-1;reqf=1;return 2;}//o=oo
            if(lastY<M-1 && bd(M,N,lastX,lastY,board,-1,+1,dev)==side && lastY>=2 && bd(M,N,lastX,lastY,board,-1,-2,dev)==side  && bd(M,N,lastX,lastY,board,-1,-1,dev)==side){req[0]=-1;reqf=1;return 2;}//oo=o
            if(lastY>=3 && bd(M,N,lastX,lastY,board,-1,-1,dev)==side && bd(M,N,lastX,lastY,board,-1,-2,dev)==side && bd(M,N,lastX,lastY,board,-1,-3,dev)==side){req[0]=-1;reqf=1;return 2;}//=ooo
        }
    }

    

 //   printf("into 3");

    
//攻击性>两个点
//两个点
    if(lastY<N-2 && bd(M,N,lastX,lastY,board,0,+1,dev)==side && lastY>0 && topt(noX,noY,dev,top,lastX,lastY,-1,+1) && topt(noX,noY,dev,top,lastX,lastY,+2,+1))
    {
        if(lastY>=2 && topt(noX,noY,dev,top,lastX,lastY,-2,+1))
        {
            if(lastY<N-3 && topt(noX,noY,dev,top,lastX,lastY,+3,+1))
            {
     //           printf("31\n");
                req[0]=-1;req[1]=2;reqf=2;return 3;
            }
            else
            {
       //         printf("32\n");
                req[0]=-1;req[1]=-2;req[2]=2;reqf=3;return 3;
            }
        }
        else if(lastY<N-3 && topt(noX,noY,dev,top,lastX,lastY,+3,+1))
        {
         //       printf("33\n");
            req[0]=-1;req[1]=2;req[2]=3;reqf=3;return 3;
        }
    }

    if(lastY<N-1 && lastY>1 && bd(M,N,lastX,lastY,board,0,-1,dev)==side && topt(noX,noY,dev,top,lastX,lastY,+1,+1)
        && topt(noX,noY,dev,top,lastX,lastY,-2,+1))//_ox_
    {
        if(lastY<N-2 && topt(noX,noY,dev,top,lastX,lastY,+2,+1))
        {
            if(lastY>2 && topt(noX,noY,dev,top,lastX,lastY,-3,+1))
            {
           //     printf("34\n");
                req[0]=-2;req[1]=1;reqf=2;return 3;
            }
            else
            {
             //   printf("35\n");
                req[0]=-2;req[1]=1;req[2]=2;reqf=3;return 3;
            }
        }
        else if(lastY>2 && topt(noX,noY,dev,top,lastX,lastY,-3,+1))//__ox_
        {
               // printf("36 %d %d %d\n",lastX,lastY,dev);printboard(M,N,board,top);
            req[0]=-3;req[1]=-2;req[2]=1;reqf=3;return 3;
        }
    }
    if(lastY<N-1 && lastY>=3 && bd(M,N,lastX,lastY,board,0,-2,dev)==side && topt(noX,noY,dev,top,lastX,lastY,-1,+1) 
        && topt(noX,noY,dev,top,lastX,lastY,-3,+1)&& topt(noX,noY,dev,top,lastX,lastY,+1,+1))//_o_x_
    {
               // printf("37\n");
        req[0]=1;req[1]=-1;req[2]=-3;reqf=3;return 3;
    }
    if(lastY<N-3 && lastY>=1 && bd(M,N,lastX,lastY,board,0,+2,dev)==side && topt(noX,noY,dev,top,lastX,lastY,+1,+1) 
        && topt(noX,noY,dev,top,lastX,lastY,+3,+1)
        && topt(noX,noY,dev,top,lastX,lastY,-1,+1))//_x_o_
    {
               // printf("38 %d %d %d\n",lastX,lastY,dev);printboard(M,N,board,top);
        req[0]=3;req[1]=1;req[2]=-1;reqf=3;return 3;
    }
//    printf("into 0");
    return 0;
    //random

                




    //左下至右上
    /*
    if(lastY<N-2 && lastX>=2 && bd(M,N,lastX,lastY,board,-1,+1,dev)==side && bd(M,N,lastX,lastY,board,-2,+2,dev)==side)//_xoo_
        if(lastY>0 && top[lastY-1]+1*dev=lastX+2)
            {if(lastY<N-3 && top[lastY+3]-3*dev=lastX-2)return 0;else {walk(M,N,top,board,lastX+1,lastY-1,noX,noY,x,y,value,side,gen,ans);return -ans;}}
        if(lastY<N-3 && top[lastY+3]-3*dev=lastX+1){walk(M,N,top,board,lastX-3,lastY+3,noX,noY,x,y,value,side,gen,ans);return -ans;}

    if(lastY<N-3 && bd(M,N,lastX,lastY,board,0,+1,dev)==side && bd(M,N,lastX,lastY,board,0,+3,dev)==side && top[lastY+2]-2*dev=lastX+1){y=lastY+2;x=lastX;return;}//xo_o
    if(lastY<N-3 && bd(M,N,lastX,lastY,board,0,+2,dev)==side && bd(M,N,lastX,lastY,board,0,+3,dev)==side && top[lastY+1]-1*dev=lastX+1){y=lastY+1;x=lastX;return;}//x_oo
    if(lastY<N-2 && bd(M,N,lastX,lastY,board,0,+2,dev)==side && lastY>0 && bd(M,N,lastX,lastY,board,0,-1,dev)==side && top[lastY+1]-1*dev=lastX+1){y=lastY+1;x=lastX;return;}//ox_o
    if(lastY<N-1 && bd(M,N,lastX,lastY,board,0,+1,dev)==side && lastY>0 && bd(M,N,lastX,lastY,board,0,-1,dev)==side)//_oxo_
        if(lastY>1 && top[lastY-2]+2*dev=lastX+1)
            {if(lastY<N-2 && top[lastY+2]-2*dev=lastX+1)return 0;else {walk(M,N,top,board,lastX,lastY-2,noX,noY,x,y,value,side,gen,ans);return -ans;}}
        if(lastY<N-2 && top[lastY+2]-2*dev=lastX+1){walk(M,N,top,board,lastX,lastY+2,noX,noY,x,y,value,side,gen,ans);return -ans;}
    if(lastY>1 && bd(M,N,lastX,lastY,board,0,-1,dev)==side && bd(M,N,lastX,lastY,board,0,-2,dev)==side)//_oox_
        if(lastY>2 && top[lastY-3]+3*dev=lastX+1)
            {if(lastY<N-1 && top[lastY+1]-1*dev=lastX+1)return 0;else {walk(M,N,top,board,lastX,lastY-3,noX,noY,x,y,value,side,gen,ans);return -ans;}}
        if(lastY<N-1 && top[lastY+1]-1*dev=lastX+1){walk(M,N,top,board,lastX,lastY+1,noX,noY,x,y,value,side,gen,ans);return -ans;}
    if(lastY<N-1 && bd(M,N,lastX,lastY,board,0,+1,dev)==side && lastY>1 && bd(M,N,lastX,lastY,board,0,-2,dev)==side && top[lastY-1]+1*dev=lastX+1){y=lastY-1;x=lastX;return;}//o_xo
    if(lastY>2 && bd(M,N,lastX,lastY,board,0,-3,dev)==side && bd(M,N,lastX,lastY,board,0,-2,dev)==side && top[lastY-1]+1*dev=lastX+1){y=lastY-1;x=lastX;return;}//oo_x
    if(lastY>2 && bd(M,N,lastX,lastY,board,0,-3,dev)==side && bd(M,N,lastX,lastY,board,0,-1,dev)==side && top[lastY-2]+2*dev=lastX+1){y=lastY-2;x=lastX;return;}//o_ox
        */
}
    
/*
 void force(const int M, const int N, const int* top, const int** board, 
	const int lastX, const int lastY, const int noX, const int noY, int x, int y){
{
    if force(M,N,top,board,lastX,lastY,noX,noY,step,x,y,value)return;

    
    //输入step:还剩多少步
	for (int i = N-1; i >= 0; i--) {
		if (top[i] > 0) {
			newx = top[i] - 1;
			newy = i;
            int x,y,value;
			break;
		}

}*/
void putpoint(int* top,int** board,int y,int &lastX, int &lastY,int side,int noX, int noY)
{
//    if(x!=top[y]-1){printf("putpoint error!");getchar();}
    board[top[y]-1][y]=side;
    top[y]--;
    if(y==noY && top[y]==noX) top[y]--;
    lastX=top[y];lastY=y;
    vacant--;
}
/*
void removepoint(int* top,int** board,int y,int &lastX, int &lastY,int side)
{
//    if(x!=top[y] || board[x][y]!=side){printf("putpoint error!");getchar();}
    board[top[y]-1][y]=side;
    top[y]--;
    lastX=top[y];lastY=y;
}
int step(const int M, const int N, int* top,  int** board, 
	int lastX, int lastY, const int noX, const int noY, int &x, int &y, int &value, int side, int ans, int gen) //0 投降 1 对面有三个 2 竖着的3个 3 对面有两个 4 自由
{
    putpoint(top,board,x,y,lastX,lastY,side);
    search(M,N,top,board,lastX,lastY,noX,noY,x,y,value,side,ans,gen-1);
    removepoint(top,board,x,y,lastX,lastY,side);
}


int search(const int M, const int N, int* top,  int** board, 
	const int lastX, const int lastY, const int noX, const int noY, int &x, int &y, int &value, int side, int ans, int gen) //0 投降 1 对面有三个 2 竖着的3个 3 对面有两个 4 自由
{
    for(int dev=-1;dev<=1;++dev)
    {
        ans=force(M,N,top,board,lastX,lastY,noX,noY,x,y,value,side,dev,4);
        if(ans>=0)return ans;
         
        force(M,N,top,board,lastX,lastY,noX,noY,x,y,value,side,dev,ans,gen);
    }
int getPoint(const int M, const int N, int* top,  int** board, 
	const int lastX, const int lastY, const int noX, const int noY, int &x, int &y, int &value, int side) //0 投降 1 对面有三个 2 竖着的3个 3 对面有两个 4 自由
{

//    board[noX][noY]=-1; !!!!!!!!!!!!!!!!!!!!!!!!!!
    //for debug!
    do { y=rand()%N; } while(top[y]==0);
    x=top[y]-1;
    //bd(M,N,lastX,lastY,board,0,0,dev)=4;
    //printboard(M,N,board);

    int maxans=0, finalx=x, finaly=y, ans=0;
//        return run(M,N,top,board,lastX,lastY+1,noX,noY,x,y,value,side,gen);

    for(int dev=-1;dev<=1;++dev)
    {
        int ans2=force(M,N,top,board,lastX,lastY,noX,noY,3-side,dev);
        if(ans2==-1) {ans=1;printf("game ends");getchar();break;}
        if(ans2==0 && ans==-1) {ans=0;}
    }

//        if(ans>maxans) {maxans=ans;finalx=x;finaly=y;}


    //mkvalue(M,N,top,board,lastX,lastY,noX,noY,10,x,y,value);
	//clearArray(M, N, board);
	return ans;
}

void getPoint(const int M, const int N, int* top,  int** board, 
	int lastX, int lastY, const int noX, const int noY, int &x, int &y, int &value, int side) //自己的(1 能赢 -1 投降 0 平(对自己))
{
    printf("lastX:%d",lastX);
    int gen=GEN,ans=-1,finalx,finaly;

//        return run(M,N,top,board,lastX,lastY+1,noX,noY,x,y,value,side,gen);
    if(top[lastY]<=0) {printf("errorovertop");getchar();}

    int reqf[3],req2f[3],req[3][3],req2[3][3],ans2[3],f4=0,f3=-1;
    //竖着的
    if(lastX<M-2 && bd(M,N,lastX,lastY,board,+1,0,0)==side && bd(M,N,lastX,lastY,board,+2,0,0)==side && lastX>0){f4=1;x=lastX-1;y=lastY;}
    printf("lastX:%d",lastX);
    
    //横着的
    for(int dev=0;dev<=2;++dev)
    {
        ans2[dev]=force(M,N,top,board,lastX,lastY,noX,noY,req[dev],req2[dev],reqf[dev],req2f[dev],side,dev-1);
        if(ans2[dev]==-1){printf("conceded");getchar();return;}
        if(ans2[dev]==1)
        {
            if(f4==0){x=lastX+(dev-1)*req[dev][0];y=lastY+req[dev][0];f4=1;
            printf("lastX:%d",lastX);
            printf("2fil:%d %d %d\n",x,y,dev);getchar();}
            else if (x!=lastX+(dev-1)*req[dev][0] || y!=lastY+req[dev][0]) {printf("concedes");getchar();return;}
        }
    }
    if(f4==1)
    {
        printf("f4==1 ans:%d",ans);
        finalx=x;finaly=y;
        printf("fil:%d %d\n",finalx,finaly);getchar();
        step(M,N,top,board,x,y,noX,noY,x,y,value,side,gen-1,ans);
        x=finalx;y=finaly;
        return;
    }
    for(int dev=0;dev<=2;++dev)
    {
        if(ans2[dev]==3)
        {
            if(f3==-1) f3=dev;
            else
            {
                for(int i=0;i<reqf[f3];++i)
                {
                    int j;
                    for(j=0;j<reqf[dev];++j)
                        if(req[f3][i]==req[dev][j]) break;
                    if(j==reqf[dev]){reqf[f3]--;req[f3][i]=reqf[f3];}
                }
                if(reqf[f3]==0){ans=-1;return;}
            }
        }
    }
    if(f3>=0)
    {
        for(int i=0;i<reqf[f3];++i)
        {
            x=lastX+req[f3][i]*(f3-1);y=lastY+req[f3][i];
            finalx=x;finaly=y;
            printf("f3==1 ans:%d",ans);getchar();
            step(M,N,top,board,x,y,noX,noY,x,y,value,side,gen-1,ans);
            x=finalx;y=finaly;
        }
        return;
    }
    else
    {
        printf("f2==1 ans:%d",ans);//getchar();
        for(int i=0;i<N;++i) step(M,N,top,board,top[i]-1,i,noX,noY,x,y,value,side,gen-1,ans);
    }
    return;
}

*/

/*
	getPoint函数返回的Point指针是在本dll模块中声明的，为避免产生堆错误，应在外部调用本dll中的
	函数来释放空间，而不应该在外部直接delete
*/
 void clearPoint(Point* p){
	delete p;
	return;
}

/*
	清除top和board数组
*/
int main()
{
    srand(6);
  //  srand((unsigned)time(NULL));
    int M=10,N=10;
//	int x = -1, y = -1;//最终将你的落子点存到x,y中
	int** board = new int*[M];
	for(int i = 0; i < M; i++){
		board[i] = new int[N];
        memset(board[i],0,4*N);
        //memset(warn[i],0,4*13);
	}
    int* top=new int[N];
    for(int i=0;i<N;++i){top[i]=M;}

    int lastY=rand()%N;
    int lastX=M-1;
    int noX=4,noY=3;
    
//    do { noX=rand()%M; noY=rand()%N;}while (noX==lastX and noY == lastY);
    //putpoint(top,board,lastY,lastX,lastY);
   // board[noX][noY]=-1; 

    int side=2;
    while(1)
    {
    	board[noX][noY]=-1;
        lastX=0;
        lastY=0;
        vacant=M*N-1;
//        putpoint(top,board,0,lastX,lastY,1);
        putpoint(top,board,1,lastX,lastY,2,noX,noY);
        putpoint(top,board,7,lastX,lastY,1,noX,noY);
        putpoint(top,board,3,lastX,lastY,2,noX,noY);
  //      putpoint(top,board,2,lastX,lastY,2);
    //    putpoint(top,board,4,lastX,lastY,2);
        board[lastX][lastY]+=2;
 //       printboard(M,N,board);
        board[lastX][lastY]-=2;
        for(int round=0;round<M*N-2;++round)
        {
            int x,y,value=1;
            int gen=GEN,fl1=0;
            if(round<10) gen--;
            double ans;
            for(y=0;y<N;++y)
            {
                x=top[y]-1;
                if(y==noY && x==noX) --x;
                if(x<0)  continue;

                board[x][y]=3-side;
                if(side==2 && userWin(x,y,M,N,board) || side==1 && machineWin(x,y,M,N,board)) {fl1=1;ans=1;board[x][y]=0;break;}
                board[x][y]=0;
            }

            if(fl1==0) walk( M,  N,  top,  board, lastX, lastY, noX, noY,x,y,value,side,gen,ans);//side为对手的代号
        //    printf("returned:%d %d\n",x,y);
            if(ans==-1){printf("%d conceded.",3-side);if(y<0||y>=N||top[y]==0)do{y=(rand()+1)%N;}while(top[y]==0 || top[y]==1 && y==noY && noX==0);x=top[y]-1;if(y==noY && x==noX) --x;
			//getchar();
			}
//            if(x!=top[y]-1){printf("errtop!(fix) %d %d %d",x,y,top[y]);y=(rand()+1)%N;x=top[y]-1;getchar();}
            side=3-side;

            printf("%d %d\n",x,y);fflush(stdout);if(x<0)getchar(); 
            if(board[x][y]!=0) {printf("erroroverlap%d %d\n",x,y);x=top[y]-1;//getchar();
			}
            putpoint(top,board,y,lastX,lastY,side,noX,noY);
            board[x][y]+=2;
            printf("lst ans:%lf\n",ans);
            printboard(M,N,board,top);
            //if(ans!=0)getchar();
            //getchar();
            board[x][y]-=2;
            if(side==2 && machineWin(x,y,M,N,board) || side==1 && userWin(x,y,M,N,board)){printf("win!");
			getchar();
			break;}
//            if(side==1 && userWin(x,y,M,N,board)){printf("1win");getchar();break;}
//            if(side ==2 && machineWin(x,y,M,N,board)){printf("2win");getchar();break;}
    //        if(ans!=4 and ans!=2) getchar();
        }
        printf("game end.");
        for(int i = 0; i < M; i++){
            memset(board[i],0,4*N);
//            memset(warn[i],0,4*13);
        }
        for(int i=0;i<N;++i){top[i]=M;}
    }
    return 0;
}
