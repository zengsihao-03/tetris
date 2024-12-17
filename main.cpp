#include<iostream>
#include<windows.h>
#include<graphics.h>
#include<conio.h>
#include<cstdlib>
#include<ctime>
using namespace std;

class Game;
class Block
{
	private:
		bool block[4][4];	//方块
		COLORREF color;		//颜色
		int x;				//x坐标
		int y;				//y坐标
		int type;			//方块类型
		static bool
		block_all[7][4][4];	//静态图形库，所有对象共享，存储7个二维4*4方块的数据

	public:
		Block(int x = 3, int y = 0): x(x), y(y)
		{
			//随机抽取一个方块图形
			get_block_rand();

			//随机选取一种颜色
			get_color_rand();
		}

		// static成员函数 初始化图形库
		static void init_block_all(void);

		//生成新图形
		void creat_new_block(void);

		//重载：生成新图形(复制对象)
		void creat_new_block(Block &b_next);

		// 随机抽取图形
		void get_block_rand(void);

		// 随机抽取颜色
		void get_color_rand(void);

		// 旋转
		void rotate(Game &g);

		// 移动
		void move(BYTE vkcode, Game &g);

		//自由下落
		void auto_down(void);

		// 碰撞检测
		bool collision(Game &g);

		// 绘制方块
		void draw_block(void);

		// 擦除方块
		void delete_block(void);

		// 方块数据加入到地图
		void block_join_map(Game &g);

		// 设置坐标
		void set_xy(int x, int y);

		//在控制台打印调试
		void show_block_on_console(void);
};

class Game
{
	private:
		bool map[20][10] = {{{0}}}; 	//地图
		int score = 0;				//分数
		int second = 0;				//时间：秒数
	public:
		Game()
		{
			//初始化游戏画布 --圆角矩形
			setlinecolor(BLACK);
			//左上角坐标（10,10），右下角坐标（370,430），圆角半径为5
			roundrect(10, 10, 370, 430, 5, 5);
		}
		//开始游戏
		void play(void);

		//获取地图数据
		bool get_map_data(int i, int j) const;

		// 消行操作
		void delete_row(void);

		//绘制地图
		void draw_map(void);

		//擦除地图
		void delete_map(void);

		//绘制右侧信息
		void draw_right_info(void);

		//监听按键操作
		void listen_key_msg(Block &b);

		//判断游戏是否结束
		bool game_is_over(void);

		/*友元函数，用于BLock类对象访问Game类私有成员map*/
		//方块加到地图
		friend void Block::block_join_map(Game &g);

		//方块检测碰撞
		friend bool Block::collision(Game &g);

		//方块移动
		friend void Block::move(BYTE vkcode, Game &g);

		//方块旋转
		friend void Block::rotate(Game &g);
};

/*初始化方块图形三维数组*/
bool Block::block_all[7][4][4] = {{{0}}};

int main(void)
{
	srand((unsigned)time(NULL));

	//初始化画布，380x440像素
	initgraph(380, 440);

	//设置画布的背景色
	setbkcolor(WHITE);
	cleardevice();

	//初始化Block图形库
	Block::init_block_all();

	//创建游戏对象
	Game game;

	//开始游戏
	game.play();

	//阻塞，防止程序关闭
	//_getch();

	//关闭画布
	closegraph();

	return 0;
}

/*Block类成员函数定义*/

/*
函数名：Block::init_bloak_all
函数功能：初始化方块图形库
函数参数：void
函数返回值：void
函数描述：
程序版本：v1.0
*/
void Block::init_block_all(void)
{
	//长条形
	block_all[0][0][1] = block_all[0][1][1] = block_all[0][2][1] =
	                         block_all[0][3][1] = 1;
	//倒7
	block_all[1][0][0] = block_all[1][1][0] = block_all[1][1][1] =
	                         block_all[1][1][2] = 1;
	//正7
	block_all[2][0][0] = block_all[2][0][1] = block_all[2][1][1] =
	                         block_all[2][2][1] = 1 ;
	//T
	block_all[3][0][1] = block_all[3][1][0] = block_all[3][1][1] =
	                         block_all[3][1][2] = 1;
	//田字
	block_all[4][0][0] = block_all[4][0][1] = block_all[4][1][0] =
	                         block_all[4][1][1] = 1;
	//Z字
	block_all[5][0][0] = block_all[5][0][1] = block_all[5][1][1] =
	                         block_all[5][1][2] = 1;
	//倒Z
	block_all[6][0][1] = block_all[6][1][0] = block_all[6][1][1] =
	                         block_all[6][2][0] = 1;
}

/*
函数名：Block::get_block_rand
函数功能：随机抽取一个方块图形
函数参数：void
函数返回值：void
函数描述：
程序版本：v1.0
*/
void Block::get_block_rand(void)
{
	type = rand() % 7;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			block[i][j] = block_all[type][i][j];
		}
	}
}

/*
函数名：Block::get_color_rand
函数功能：随机获取RGB颜色值
函数参数：void
函数返回值：void
函数描述：
程序版本：v1.0
*/
void Block::get_color_rand(void)
{
	int r, g, b;
	r = rand() % 255;
	g = rand() % 255;
	b = rand() % 255;

	color = RGB(r, g, b);
}

/*
函数名：Block::creat_new_block
函数功能：创建一个新方块图形
函数参数：void
函数返回值：void
函数描述：
程序版本：v1.0
*/
void Block::creat_new_block(void)
{
	set_xy(3, 0);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			block[i][j] = 0;
		}
	}
	get_block_rand();
	get_color_rand();
}

/*
函数名：Block::creat_new_block
函数功能：重载：创建一个新方块图形
函数参数：void
函数返回值：void
函数描述：传入一个Block对象引用，进行复制创建
程序版本：v1.0
*/
void Block::creat_new_block(Block &b_next)
{
	set_xy(3, 0);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			block[i][j] = b_next.block[i][j];
		}
	}

	this->type = b_next.type;
	this->color = b_next.color;
}

/*
函数名：Block::set_xy
函数功能：设置方块坐标
函数参数：int x, int y
函数返回值：void
函数描述：设置4*4方块的左顶点坐标
程序版本：v1.0
*/
void Block::set_xy(int x, int y)
{
	this->x = x;
	this->y = y;
}

/*
函数名：Block::show_block_on_console
函数功能：控制台输出4*4方块数组数据
函数参数：void
函数返回值：void
函数描述：debug
程序版本：v1.0
*/
void Block::show_block_on_console(void)
{
	int i, j;
	cout << "type:" << type << endl;
	cout << "x: " << x << " ,y: " << y << endl;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			cout << block[i][j] << " ";
		}
		cout << endl;
	}
}

/*
函数名：Block::draw_block
函数功能：绘制方块
函数参数：void
函数返回值：void
函数描述：绘制方块
程序版本：v1.0
*/
void Block::draw_block(void)
{
	setlinecolor(BLACK);
	setfillcolor(this->color);

	int i, j;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (block[i][j] == true)
			{
				/*计算方块左上的像素点位置*/
				/* j*20是小方块距离4*4block左边的距离，i*20是小方块距离block顶部的距离  */
				/* x*20 是block距离map左边的距离，y*20是block距离map顶部的距离    */

				int left = j * 20 + 10 + 10 + x * 20; //10是边框间的像素距离
				int top = i * 20 + 10 + 10 + y * 20;

				/*计算方块右下像素点位置*/
				int right = left + 20;  //20是一个方块的像素距离
				int bottom = top + 20;

				/*绘制方块*/
				fillrectangle(left, top, right, bottom);
			}
		}
	}
}

/*
函数名：Block::delete_block
函数功能：擦除方块
函数参数：void
函数返回值：void
函数描述：根据坐标擦除方块
程序版本：v1.0
*/
void Block::delete_block(void)
{
	int i, j;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (block[i][j] == true)
			{
				/*计算方块左上的像素点位置*/
				int left = j * 20 + 10 + 10 + x * 20; //10是边框间的像素距离
				int top = i * 20 + 10 + 10 + y * 20;

				/*计算方块右下像素点位置*/
				int right = left + 20;  //20是一个方块的像素距离
				int bottom = top + 20;

				/*绘制方块*/
				//fillrectangle(left, top, right, bottom);
				clearrectangle(left, top, right, bottom);
			}
		}
	}
}

/*
函数名：Block::block_join_map
函数功能：将方块数据存到地图
函数参数：Game &g
函数返回值：void
函数描述：友元函数,&g访问Game类的私有成员map，将4*4的方块坐标转换为地图上的坐标
程序版本：v1.0
*/
void Block::block_join_map(Game &g)
{
	int i, j;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (block[i][j] == true)
			{
				/*方块在地图中的坐标为（x+j,y+i） */
				g.map[y + i][x + j] = block[i][j];
			}
		}
	}
}

/*
函数名：Block::auto_down
函数功能：方块自动下落
函数参数：void
函数返回值：void
函数描述：纵坐标+1
程序版本：v1.0
*/
void Block::auto_down(void)
{
	y += 1;
}

/*
函数名：Block::move
函数功能：方块移动
函数参数：BYTE vkcode, Game &g
函数返回值：void
函数描述：让方块移动，同时碰撞检测 ，&g是引用， 该函数是友元函数
程序版本：v1.0
*/
void Block::move(BYTE vkcode, Game &g)
{
	if (vkcode == VK_LEFT)
	{
		x--;
		//判断碰撞
		if (collision(g))
		{
			//碰撞了则回溯
			x++;
		}
	}
	else if (vkcode == VK_RIGHT)
	{
		x++;
		if (collision(g))
		{
			x--;
		}
	}
	else if (vkcode == VK_DOWN)
	{
		y++;
	}
}
/*
函数名：Block::collision
函数功能：碰撞检测
函数参数：Game &g
函数返回值：bool
函数描述：检测方块在运动是否碰撞到左右边界、是否碰撞到底部和地图方块
程序版本：v1.0
*/
bool Block::collision(Game &g)
{
	int i, j;
	for (i = 3; i >= 0; i--)
	{
		for (j = 0; j < 4; j++)
		{
			if (block[i][j] == 1 && i + y + 1 == 20) //判断是否碰到底部
			{
				block_join_map(g);	//如果碰到底部，则把方块数据加入到地图
				return true;
			}
			else if (block[i][j] == 1 && (x + j < 0 || x + j == 10) ) //判断是否碰到左右两边
			{
				return true;
			}
			else if (block[i][j] == g.map[y + i + 1][x + j]
			         && block[i][j] == true ) 			//检测是否与地图的方块碰撞
			{
				block_join_map(g);	//如果下落过程碰到地图方块，则把方块数据加入到地图
				return true;
			}
		}
	}

	return false;
}

/*
函数名：Block::rotate
函数功能：旋转方块图形
函数参数：Game &g
函数返回值：void
函数描述：3*3内四角旋转、十字旋转 ，田字不旋转，竖条分开转
程序版本：v1.0
*/
void Block::rotate(Game &g)
{
	//田字形不做旋转
	if (type != 4)
	{
		bool temp;
		/* 3*3旋转 */
		//四角旋转
		temp = block[0][0];
		block[0][0] = block[2][0];
		block[2][0] = block[2][2];
		block[2][2] = block[0][2];
		block[0][2] = temp;

		//十字旋转
		temp = block[0][1];
		block[0][1] = block[1][0];
		block[1][0] = block[2][1];
		block[2][1] = block[1][2];
		block[1][2] = temp;

		/*如果是长条形的话 */
		if (type == 0)
		{
			temp = block[1][3];
			block[1][3] = block[3][1];
			block[3][1] = temp;
		}



		/*碰撞检测，逆时针旋转回去*/
		if (collision(g))
		{
			/* 3*3旋转 */
			//四角旋转
			temp = block[0][0];
			block[0][0] = block[0][2];
			block[0][2] = block[2][2];
			block[2][2] = block[2][0];
			block[2][0] = temp;

			//十字旋转
			temp = block[0][1];
			block[0][1] = block[1][2];
			block[1][2] = block[2][1];
			block[2][1] = block[1][0];
			block[1][0] = temp;

			/*如果是长条形的话 */
			if (type == 0)
			{
				temp = block[1][3];
				block[1][3] = block[3][1];
				block[3][1] = temp;
			}
		}
	}
}



/*Game类成员函数定义*/

/*
函数名：Game::play
函数功能：游戏类对象开始游戏
函数参数：void
函数返回值：void
函数描述：开始游戏：绘制地图，绘制右侧信息，获取地图数据，消行等操作
程序版本：v1.0
*/
void Game::play(void)
{
	//时间 ：用来绘制时间
	clock_t t_start = clock();
	clock_t t_end;

	//时钟 ：用来调整自动下落的速度
	clock_t cl_start = clock();
	clock_t cl_end;

	//绘制地图
	draw_map();

	//绘制右侧信息
	draw_right_info();

	//创建方块对象
	Block b;
	b.draw_block();

	//创建下一个方块对象
	Block b_next;
	b_next.set_xy(12, 2);
	b_next.draw_block();

	//批量绘图，防止屏闪
	BeginBatchDraw();

	while (1)
	{
		//擦除地图
		delete_map();
		//监听按键操作
		listen_key_msg(b);

		/*在时钟间隔内自动下落*/
		cl_end = clock();

		//间隔时间：ms
		double interval_time = (double(cl_end - cl_start) / CLOCKS_PER_SEC) * 1000;
		cout << interval_time << endl;

		//自由下落
		if (interval_time > 500)
		{
			b.auto_down();
			cl_start = cl_end;
		}

		//检测下落碰撞
		if (b.collision(*this))
		{
			//如果碰撞后则将下一个方块复制到这个方块，继续生成下一个方块
			b.creat_new_block(b_next);
			b_next.delete_block();
			b_next.creat_new_block();
			b_next.set_xy(12, 2);
			b_next.draw_block();
		}

		//消行
		delete_row();

		//绘制地图
		draw_map();

		//设置时间
		t_end = clock();
		second = int(t_end - t_start) / CLOCKS_PER_SEC;

		//绘制右侧信息
		draw_right_info();

		//绘制方块
		b.draw_block();

		FlushBatchDraw();

		//判断游戏结束
		if (game_is_over())
		{
			break;
		}

		//绘制的频率，10ms绘制一次
		Sleep(10);
	}

	cout << "game over" << endl;

	//游戏结束，清除指定位置，绘制结束文字
	clearrectangle(40, 200, 200, 250);
	settextcolor(BLUE);
	RECT r  = {40, 200, 200, 250};
	drawtext(_T("游戏结束！按ESC退出"), &r, DT_VCENTER | DT_SINGLELINE);

	FlushBatchDraw();

	//接收键盘消息，当按下ESC时退出
	ExMessage exmsg;
	while (1)
	{
		int res = peekmessage(&exmsg, EX_KEY);
		if (res)
		{
			if (exmsg.message == WM_KEYDOWN)
			{
				if (exmsg.vkcode == VK_ESCAPE)
				{
					return;
				}
			}
		}
	}

	//结束play
}

/*
函数名：Game::draw_map
函数功能：绘制游戏地图和方块
函数参数：void
函数返回值：void
函数描述：绘制游戏地图边框，根据map数组计算方块像素值并绘制20*20的方块
程序版本：v1.0
*/
void Game::draw_map(void)
{
	//绘制地图边框
	setlinecolor(BLACK);	//设置线条颜色
	rectangle(20, 20, 220,
	          420); 	//绘制矩形，左上角坐标（20,20），右下角坐标（220,420）

	//设置填充颜色
	setfillcolor(YELLOW);

	/**
	 * 根据地图数据绘制方块
	 * 地图map数组中是1的则绘制方块，0不绘制
	 **/
	int i, j;
	for (i = 0; i < 20; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if (map[i][j] == true)
			{
				/*计算方块左上的像素点位置*/
				int left = j * 20 + 10 + 10; //10是边框间的像素距离
				int top = i * 20 + 10 + 10;

				/*计算方块右下像素点位置*/
				int right = left + 20;  //20是一个方块的像素距离
				int bottom = top + 20;

				/*绘制方块*/
				fillrectangle(left, top, right, bottom);
			}
		}
	}
}

/*
函数名：Game::delete_map
函数功能：清除地图
函数参数：void
函数返回值：void
函数描述：清除指定矩形区域 
程序版本：v1.0
*/
void Game::delete_map(void)
{
	clearrectangle(20, 20, 220, 420);
}

/*
函数名：Game::draw_right_info
函数功能：绘制游戏右侧的文字信息
函数参数：void
函数返回值：void
函数描述：绘制游戏右侧的下一个方块、分数、时间等文字信息
程序版本：v1.0
*/
void Game::draw_right_info(void)
{
	settextcolor(BLACK);
	//方式一： 
	TCHAR s1[] = _T("下一个方块");
	outtextxy(240, 30, s1);

	TCHAR s2[20];
	_stprintf(s2, _T("分数：%d"), score);
	outtextxy(240, 200, s2);

	TCHAR s3[20];
	_stprintf(s3, _T("时间：%d s"), second);
	outtextxy(240, 300, s3);
//  方式二 ： 
//	RECT r  = {240, 30, 360, 50};
//	drawtext(_T("下一个方块"), &r, DT_LEFT);
//
//	RECT r1 = {240, 200, 360, 250};
//	drawtext(_T("分数："), &r1, DT_LEFT);
//
//	RECT r2 = {240, 300, 360, 350};
//	drawtext(_T("时间："), &r2, DT_LEFT);
}

/*
函数名：Game::listen_key_msg
函数功能：监听按键消息
函数参数：Block &b
函数返回值：void
函数描述：根据键盘vk值操作方块旋转、移动
程序版本：v1.0
*/
void Game::listen_key_msg(Block &b)
{	
	//定义消息函数结构体 
	ExMessage key_msg;
	
	//获取一个键盘消息，并立即返回true 或 false 
	bool res = peekmessage(&key_msg, EX_KEY);

	if (res)	//如果有键盘消息 
	{	
		if (key_msg.message == WM_KEYDOWN) //判断键盘消息是否是键盘按下 
		{	
			//如果是按下，则继续判断是哪个键值 
			if (key_msg.vkcode == VK_UP || key_msg.vkcode == 0x57)
			{
				cout << "旋转" << endl;
				b.rotate(*this);
			}
			else if (key_msg.vkcode == VK_LEFT || key_msg.vkcode == 0x41 )
			{
				cout << "左" << endl;
				b.move(VK_LEFT, *this);
			}
			else if (key_msg.vkcode == VK_RIGHT || key_msg.vkcode == 0x44)
			{
				cout << "右" << endl;
				b.move(VK_RIGHT, *this);
			}
			else if (key_msg.vkcode == VK_DOWN || key_msg.vkcode == 0x53)
			{
				cout << "下" << endl;
				b.move(VK_DOWN, *this);
			}

		}
	}

}

/*
函数名：Game::delete_row
函数功能：消行
函数参数：void
函数返回值：void
函数描述：消除一行
程序版本：v1.0
*/
void Game::delete_row(void)
{
	int i, j, flag, row;
	for (i = 19; i >= 0; i--)	//从底行开始
	{
		flag = 0;	//标识位 = 0，则表示满行
		for (j = 0; j < 10; j++)
		{
			if (map[i][j] == false) //如果有空格，flag=1，表示没有满行
			{
				flag = 1;
				break;
			}
		}

		/*如果该行满，则消除该行（将上面的行往下移）*/
		if (flag == 0)
		{
			row = i;
			//从第row行开始往上逐行下移
			for (int m = row; m > 0; m--)
			{
				cout << m << "行下移" << endl;
				for (int n = 0; n < 10; n++)
				{
					map[m][n] = map[m - 1][n];
				}
			}
			cout << row + 1 << "行消除" << endl;
			score += 10;
		}
	}

}

/*
函数名：Game::game_is_over
函数功能：判断游戏结束
函数参数：void
函数返回值：bool
函数描述：如果地图首行有1的数据则返回true
程序版本：v1.0
*/
bool Game::game_is_over(void)
{
	int j;

	//判断如果首行有为1的块，则结束
	for (j = 0; j < 10; j++)
	{
		if (map[0][j] == true)
		{
			return true;
		}
	}
	return false;
}
