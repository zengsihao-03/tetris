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
		bool block[4][4];	//����
		COLORREF color;		//��ɫ
		int x;				//x����
		int y;				//y����
		int type;			//��������
		static bool
		block_all[7][4][4];	//��̬ͼ�ο⣬���ж������洢7����ά4*4���������

	public:
		Block(int x = 3, int y = 0): x(x), y(y)
		{
			//�����ȡһ������ͼ��
			get_block_rand();

			//���ѡȡһ����ɫ
			get_color_rand();
		}

		// static��Ա���� ��ʼ��ͼ�ο�
		static void init_block_all(void);

		//������ͼ��
		void creat_new_block(void);

		//���أ�������ͼ��(���ƶ���)
		void creat_new_block(Block &b_next);

		// �����ȡͼ��
		void get_block_rand(void);

		// �����ȡ��ɫ
		void get_color_rand(void);

		// ��ת
		void rotate(Game &g);

		// �ƶ�
		void move(BYTE vkcode, Game &g);

		//��������
		void auto_down(void);

		// ��ײ���
		bool collision(Game &g);

		// ���Ʒ���
		void draw_block(void);

		// ��������
		void delete_block(void);

		// �������ݼ��뵽��ͼ
		void block_join_map(Game &g);

		// ��������
		void set_xy(int x, int y);

		//�ڿ���̨��ӡ����
		void show_block_on_console(void);
};

class Game
{
	private:
		bool map[20][10] = {{{0}}}; 	//��ͼ
		int score = 0;				//����
		int second = 0;				//ʱ�䣺����
	public:
		Game()
		{
			//��ʼ����Ϸ���� --Բ�Ǿ���
			setlinecolor(BLACK);
			//���Ͻ����꣨10,10�������½����꣨370,430����Բ�ǰ뾶Ϊ5
			roundrect(10, 10, 370, 430, 5, 5);
		}
		//��ʼ��Ϸ
		void play(void);

		//��ȡ��ͼ����
		bool get_map_data(int i, int j) const;

		// ���в���
		void delete_row(void);

		//���Ƶ�ͼ
		void draw_map(void);

		//������ͼ
		void delete_map(void);

		//�����Ҳ���Ϣ
		void draw_right_info(void);

		//������������
		void listen_key_msg(Block &b);

		//�ж���Ϸ�Ƿ����
		bool game_is_over(void);

		/*��Ԫ����������BLock��������Game��˽�г�Աmap*/
		//����ӵ���ͼ
		friend void Block::block_join_map(Game &g);

		//��������ײ
		friend bool Block::collision(Game &g);

		//�����ƶ�
		friend void Block::move(BYTE vkcode, Game &g);

		//������ת
		friend void Block::rotate(Game &g);
};

/*��ʼ������ͼ����ά����*/
bool Block::block_all[7][4][4] = {{{0}}};

int main(void)
{
	srand((unsigned)time(NULL));

	//��ʼ��������380x440����
	initgraph(380, 440);

	//���û����ı���ɫ
	setbkcolor(WHITE);
	cleardevice();

	//��ʼ��Blockͼ�ο�
	Block::init_block_all();

	//������Ϸ����
	Game game;

	//��ʼ��Ϸ
	game.play();

	//��������ֹ����ر�
	//_getch();

	//�رջ���
	closegraph();

	return 0;
}

/*Block���Ա��������*/

/*
��������Block::init_bloak_all
�������ܣ���ʼ������ͼ�ο�
����������void
��������ֵ��void
����������
����汾��v1.0
*/
void Block::init_block_all(void)
{
	//������
	block_all[0][0][1] = block_all[0][1][1] = block_all[0][2][1] =
	                         block_all[0][3][1] = 1;
	//��7
	block_all[1][0][0] = block_all[1][1][0] = block_all[1][1][1] =
	                         block_all[1][1][2] = 1;
	//��7
	block_all[2][0][0] = block_all[2][0][1] = block_all[2][1][1] =
	                         block_all[2][2][1] = 1 ;
	//T
	block_all[3][0][1] = block_all[3][1][0] = block_all[3][1][1] =
	                         block_all[3][1][2] = 1;
	//����
	block_all[4][0][0] = block_all[4][0][1] = block_all[4][1][0] =
	                         block_all[4][1][1] = 1;
	//Z��
	block_all[5][0][0] = block_all[5][0][1] = block_all[5][1][1] =
	                         block_all[5][1][2] = 1;
	//��Z
	block_all[6][0][1] = block_all[6][1][0] = block_all[6][1][1] =
	                         block_all[6][2][0] = 1;
}

/*
��������Block::get_block_rand
�������ܣ������ȡһ������ͼ��
����������void
��������ֵ��void
����������
����汾��v1.0
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
��������Block::get_color_rand
�������ܣ������ȡRGB��ɫֵ
����������void
��������ֵ��void
����������
����汾��v1.0
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
��������Block::creat_new_block
�������ܣ�����һ���·���ͼ��
����������void
��������ֵ��void
����������
����汾��v1.0
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
��������Block::creat_new_block
�������ܣ����أ�����һ���·���ͼ��
����������void
��������ֵ��void
��������������һ��Block�������ã����и��ƴ���
����汾��v1.0
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
��������Block::set_xy
�������ܣ����÷�������
����������int x, int y
��������ֵ��void
��������������4*4������󶥵�����
����汾��v1.0
*/
void Block::set_xy(int x, int y)
{
	this->x = x;
	this->y = y;
}

/*
��������Block::show_block_on_console
�������ܣ�����̨���4*4������������
����������void
��������ֵ��void
����������debug
����汾��v1.0
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
��������Block::draw_block
�������ܣ����Ʒ���
����������void
��������ֵ��void
�������������Ʒ���
����汾��v1.0
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
				/*���㷽�����ϵ����ص�λ��*/
				/* j*20��С�������4*4block��ߵľ��룬i*20��С�������block�����ľ���  */
				/* x*20 ��block����map��ߵľ��룬y*20��block����map�����ľ���    */

				int left = j * 20 + 10 + 10 + x * 20; //10�Ǳ߿������ؾ���
				int top = i * 20 + 10 + 10 + y * 20;

				/*���㷽���������ص�λ��*/
				int right = left + 20;  //20��һ����������ؾ���
				int bottom = top + 20;

				/*���Ʒ���*/
				fillrectangle(left, top, right, bottom);
			}
		}
	}
}

/*
��������Block::delete_block
�������ܣ���������
����������void
��������ֵ��void
�������������������������
����汾��v1.0
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
				/*���㷽�����ϵ����ص�λ��*/
				int left = j * 20 + 10 + 10 + x * 20; //10�Ǳ߿������ؾ���
				int top = i * 20 + 10 + 10 + y * 20;

				/*���㷽���������ص�λ��*/
				int right = left + 20;  //20��һ����������ؾ���
				int bottom = top + 20;

				/*���Ʒ���*/
				//fillrectangle(left, top, right, bottom);
				clearrectangle(left, top, right, bottom);
			}
		}
	}
}

/*
��������Block::block_join_map
�������ܣ����������ݴ浽��ͼ
����������Game &g
��������ֵ��void
������������Ԫ����,&g����Game���˽�г�Աmap����4*4�ķ�������ת��Ϊ��ͼ�ϵ�����
����汾��v1.0
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
				/*�����ڵ�ͼ�е�����Ϊ��x+j,y+i�� */
				g.map[y + i][x + j] = block[i][j];
			}
		}
	}
}

/*
��������Block::auto_down
�������ܣ������Զ�����
����������void
��������ֵ��void
����������������+1
����汾��v1.0
*/
void Block::auto_down(void)
{
	y += 1;
}

/*
��������Block::move
�������ܣ������ƶ�
����������BYTE vkcode, Game &g
��������ֵ��void
�����������÷����ƶ���ͬʱ��ײ��� ��&g�����ã� �ú�������Ԫ����
����汾��v1.0
*/
void Block::move(BYTE vkcode, Game &g)
{
	if (vkcode == VK_LEFT)
	{
		x--;
		//�ж���ײ
		if (collision(g))
		{
			//��ײ�������
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
��������Block::collision
�������ܣ���ײ���
����������Game &g
��������ֵ��bool
������������ⷽ�����˶��Ƿ���ײ�����ұ߽硢�Ƿ���ײ���ײ��͵�ͼ����
����汾��v1.0
*/
bool Block::collision(Game &g)
{
	int i, j;
	for (i = 3; i >= 0; i--)
	{
		for (j = 0; j < 4; j++)
		{
			if (block[i][j] == 1 && i + y + 1 == 20) //�ж��Ƿ������ײ�
			{
				block_join_map(g);	//��������ײ�����ѷ������ݼ��뵽��ͼ
				return true;
			}
			else if (block[i][j] == 1 && (x + j < 0 || x + j == 10) ) //�ж��Ƿ�������������
			{
				return true;
			}
			else if (block[i][j] == g.map[y + i + 1][x + j]
			         && block[i][j] == true ) 			//����Ƿ����ͼ�ķ�����ײ
			{
				block_join_map(g);	//����������������ͼ���飬��ѷ������ݼ��뵽��ͼ
				return true;
			}
		}
	}

	return false;
}

/*
��������Block::rotate
�������ܣ���ת����ͼ��
����������Game &g
��������ֵ��void
����������3*3���Ľ���ת��ʮ����ת �����ֲ���ת�������ֿ�ת
����汾��v1.0
*/
void Block::rotate(Game &g)
{
	//�����β�����ת
	if (type != 4)
	{
		bool temp;
		/* 3*3��ת */
		//�Ľ���ת
		temp = block[0][0];
		block[0][0] = block[2][0];
		block[2][0] = block[2][2];
		block[2][2] = block[0][2];
		block[0][2] = temp;

		//ʮ����ת
		temp = block[0][1];
		block[0][1] = block[1][0];
		block[1][0] = block[2][1];
		block[2][1] = block[1][2];
		block[1][2] = temp;

		/*����ǳ����εĻ� */
		if (type == 0)
		{
			temp = block[1][3];
			block[1][3] = block[3][1];
			block[3][1] = temp;
		}



		/*��ײ��⣬��ʱ����ת��ȥ*/
		if (collision(g))
		{
			/* 3*3��ת */
			//�Ľ���ת
			temp = block[0][0];
			block[0][0] = block[0][2];
			block[0][2] = block[2][2];
			block[2][2] = block[2][0];
			block[2][0] = temp;

			//ʮ����ת
			temp = block[0][1];
			block[0][1] = block[1][2];
			block[1][2] = block[2][1];
			block[2][1] = block[1][0];
			block[1][0] = temp;

			/*����ǳ����εĻ� */
			if (type == 0)
			{
				temp = block[1][3];
				block[1][3] = block[3][1];
				block[3][1] = temp;
			}
		}
	}
}



/*Game���Ա��������*/

/*
��������Game::play
�������ܣ���Ϸ�����ʼ��Ϸ
����������void
��������ֵ��void
������������ʼ��Ϸ�����Ƶ�ͼ�������Ҳ���Ϣ����ȡ��ͼ���ݣ����еȲ���
����汾��v1.0
*/
void Game::play(void)
{
	//ʱ�� ����������ʱ��
	clock_t t_start = clock();
	clock_t t_end;

	//ʱ�� �����������Զ�������ٶ�
	clock_t cl_start = clock();
	clock_t cl_end;

	//���Ƶ�ͼ
	draw_map();

	//�����Ҳ���Ϣ
	draw_right_info();

	//�����������
	Block b;
	b.draw_block();

	//������һ���������
	Block b_next;
	b_next.set_xy(12, 2);
	b_next.draw_block();

	//������ͼ����ֹ����
	BeginBatchDraw();

	while (1)
	{
		//������ͼ
		delete_map();
		//������������
		listen_key_msg(b);

		/*��ʱ�Ӽ�����Զ�����*/
		cl_end = clock();

		//���ʱ�䣺ms
		double interval_time = (double(cl_end - cl_start) / CLOCKS_PER_SEC) * 1000;
		cout << interval_time << endl;

		//��������
		if (interval_time > 500)
		{
			b.auto_down();
			cl_start = cl_end;
		}

		//���������ײ
		if (b.collision(*this))
		{
			//�����ײ������һ�����鸴�Ƶ�������飬����������һ������
			b.creat_new_block(b_next);
			b_next.delete_block();
			b_next.creat_new_block();
			b_next.set_xy(12, 2);
			b_next.draw_block();
		}

		//����
		delete_row();

		//���Ƶ�ͼ
		draw_map();

		//����ʱ��
		t_end = clock();
		second = int(t_end - t_start) / CLOCKS_PER_SEC;

		//�����Ҳ���Ϣ
		draw_right_info();

		//���Ʒ���
		b.draw_block();

		FlushBatchDraw();

		//�ж���Ϸ����
		if (game_is_over())
		{
			break;
		}

		//���Ƶ�Ƶ�ʣ�10ms����һ��
		Sleep(10);
	}

	cout << "game over" << endl;

	//��Ϸ���������ָ��λ�ã����ƽ�������
	clearrectangle(40, 200, 200, 250);
	settextcolor(BLUE);
	RECT r  = {40, 200, 200, 250};
	drawtext(_T("��Ϸ��������ESC�˳�"), &r, DT_VCENTER | DT_SINGLELINE);

	FlushBatchDraw();

	//���ռ�����Ϣ��������ESCʱ�˳�
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

	//����play
}

/*
��������Game::draw_map
�������ܣ�������Ϸ��ͼ�ͷ���
����������void
��������ֵ��void
����������������Ϸ��ͼ�߿򣬸���map������㷽������ֵ������20*20�ķ���
����汾��v1.0
*/
void Game::draw_map(void)
{
	//���Ƶ�ͼ�߿�
	setlinecolor(BLACK);	//����������ɫ
	rectangle(20, 20, 220,
	          420); 	//���ƾ��Σ����Ͻ����꣨20,20�������½����꣨220,420��

	//���������ɫ
	setfillcolor(YELLOW);

	/**
	 * ���ݵ�ͼ���ݻ��Ʒ���
	 * ��ͼmap��������1������Ʒ��飬0������
	 **/
	int i, j;
	for (i = 0; i < 20; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if (map[i][j] == true)
			{
				/*���㷽�����ϵ����ص�λ��*/
				int left = j * 20 + 10 + 10; //10�Ǳ߿������ؾ���
				int top = i * 20 + 10 + 10;

				/*���㷽���������ص�λ��*/
				int right = left + 20;  //20��һ����������ؾ���
				int bottom = top + 20;

				/*���Ʒ���*/
				fillrectangle(left, top, right, bottom);
			}
		}
	}
}

/*
��������Game::delete_map
�������ܣ������ͼ
����������void
��������ֵ��void
�������������ָ���������� 
����汾��v1.0
*/
void Game::delete_map(void)
{
	clearrectangle(20, 20, 220, 420);
}

/*
��������Game::draw_right_info
�������ܣ�������Ϸ�Ҳ��������Ϣ
����������void
��������ֵ��void
����������������Ϸ�Ҳ����һ�����顢������ʱ���������Ϣ
����汾��v1.0
*/
void Game::draw_right_info(void)
{
	settextcolor(BLACK);
	//��ʽһ�� 
	TCHAR s1[] = _T("��һ������");
	outtextxy(240, 30, s1);

	TCHAR s2[20];
	_stprintf(s2, _T("������%d"), score);
	outtextxy(240, 200, s2);

	TCHAR s3[20];
	_stprintf(s3, _T("ʱ�䣺%d s"), second);
	outtextxy(240, 300, s3);
//  ��ʽ�� �� 
//	RECT r  = {240, 30, 360, 50};
//	drawtext(_T("��һ������"), &r, DT_LEFT);
//
//	RECT r1 = {240, 200, 360, 250};
//	drawtext(_T("������"), &r1, DT_LEFT);
//
//	RECT r2 = {240, 300, 360, 350};
//	drawtext(_T("ʱ�䣺"), &r2, DT_LEFT);
}

/*
��������Game::listen_key_msg
�������ܣ�����������Ϣ
����������Block &b
��������ֵ��void
�������������ݼ���vkֵ����������ת���ƶ�
����汾��v1.0
*/
void Game::listen_key_msg(Block &b)
{	
	//������Ϣ�����ṹ�� 
	ExMessage key_msg;
	
	//��ȡһ��������Ϣ������������true �� false 
	bool res = peekmessage(&key_msg, EX_KEY);

	if (res)	//����м�����Ϣ 
	{	
		if (key_msg.message == WM_KEYDOWN) //�жϼ�����Ϣ�Ƿ��Ǽ��̰��� 
		{	
			//����ǰ��£�������ж����ĸ���ֵ 
			if (key_msg.vkcode == VK_UP || key_msg.vkcode == 0x57)
			{
				cout << "��ת" << endl;
				b.rotate(*this);
			}
			else if (key_msg.vkcode == VK_LEFT || key_msg.vkcode == 0x41 )
			{
				cout << "��" << endl;
				b.move(VK_LEFT, *this);
			}
			else if (key_msg.vkcode == VK_RIGHT || key_msg.vkcode == 0x44)
			{
				cout << "��" << endl;
				b.move(VK_RIGHT, *this);
			}
			else if (key_msg.vkcode == VK_DOWN || key_msg.vkcode == 0x53)
			{
				cout << "��" << endl;
				b.move(VK_DOWN, *this);
			}

		}
	}

}

/*
��������Game::delete_row
�������ܣ�����
����������void
��������ֵ��void
��������������һ��
����汾��v1.0
*/
void Game::delete_row(void)
{
	int i, j, flag, row;
	for (i = 19; i >= 0; i--)	//�ӵ��п�ʼ
	{
		flag = 0;	//��ʶλ = 0�����ʾ����
		for (j = 0; j < 10; j++)
		{
			if (map[i][j] == false) //����пո�flag=1����ʾû������
			{
				flag = 1;
				break;
			}
		}

		/*��������������������У���������������ƣ�*/
		if (flag == 0)
		{
			row = i;
			//�ӵ�row�п�ʼ������������
			for (int m = row; m > 0; m--)
			{
				cout << m << "������" << endl;
				for (int n = 0; n < 10; n++)
				{
					map[m][n] = map[m - 1][n];
				}
			}
			cout << row + 1 << "������" << endl;
			score += 10;
		}
	}

}

/*
��������Game::game_is_over
�������ܣ��ж���Ϸ����
����������void
��������ֵ��bool
���������������ͼ������1�������򷵻�true
����汾��v1.0
*/
bool Game::game_is_over(void)
{
	int j;

	//�ж����������Ϊ1�Ŀ飬�����
	for (j = 0; j < 10; j++)
	{
		if (map[0][j] == true)
		{
			return true;
		}
	}
	return false;
}
