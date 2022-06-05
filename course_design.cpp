#include<bits/stdc++.h>
#include<graphics.h>
#include<ege/sys_edit.h>
using namespace std;
char str1[1000], str2[1000], str3[1000], str4[1000], str5[1000];
char din[550] = { '\0' };
struct node
{
    char date[10];//日期yyyy/mm/dd
    double kp, max, min, sp;//kp=开盘，max=最高价，min=最低价，sp=收盘
    int flag1, flag2 = 0, flag3 = 0;//flag1=1：实心蓝白 =2：空心红 =3：十字红 flag2=1：有下影线 flag3=1：有上影线
}d[15];
void getdata()
{
    initgraph(1200, 1200, 0);
    setrendermode(RENDER_MANUAL);//不使用自动刷新，减少闪烁
    sys_edit editBox;//输入文本框
    editBox.create(true);
    editBox.move(50, 50);
    editBox.size(750, 400);
    editBox.setmaxlen(480);
    editBox.visable(true);
    editBox.setfocus();
    editBox.setfont(24, 0, "宋体");
    editBox.setbgcolor(WHITE);
    editBox.setcolor(BLACK);
    editBox.gettext(480, din);
    //文本框初始化
    mouse_msg msg = { 0 };
    for (; is_run(); delay_fps(60))
    {
        bool flag = false;
        while (mousemsg())
        {
            mouse_msg msg = getmouse();
            int x = msg.x, y = msg.y, click = msg.is_up();
            if (click == 1 && x > 990 && x < 1050 && y > 670 && y < 730)
            {
                flag = true;//点击确认后完成输入
            }
        }
        if (flag)
        {
            editBox.gettext(480, din);
            editBox.visable(false);
        }//将文本框输入读入din数组
        cleardevice();
        setbkcolor(BLACK);
        setcolor(WHITE);
        setfont(25, 0, "宋体");
        outtextrect(0, 0, 1200, 1200, "请输入股票数据，按照“yyyy/mm/dd,开盘价,最高价,最低价,收盘价”的格式输入，按行输入每一天的数据\n输入完成后请点击确认键：");
        outtextxy(1000, 700, "确认");
        if (flag)
        {
            flushkey();
            return;
        }
    }
    return;
}
void init()//初始化函数，处理输入数据
{
    getdata();
    int i, j = 0, sum = 0;
    for (i = 0; i < 480;)
    {
        char tmpkp[4], tmpmax[4], tmpmin[4], tmpsp[4];
        for (j = 0; j < 10; j++)
        {
            d[sum].date[j] = din[i];
            i++;
        }//将日期存入d数组
        i++;
        for (j = 0; j < 4; j++)
        {
            tmpkp[j] = din[i];
            i++;
        }
        i++;
        for (j = 0; j < 4; j++)
        {
            tmpmax[j] = din[i];
            i++;
        }
        i++;
        for (j = 0; j < 4; j++)
        {
            tmpmin[j] = din[i];
            i++;
        }
        i++;
        for (j = 0; j < 4; j++)
        {
            tmpsp[j] = din[i];
            i++;
        }
        i+=2;
        //分别以字符串形式临时存储开盘、最高、最低、收盘价格
        d[sum].kp = (tmpkp[0] - '0') + (tmpkp[2] - '0') * 1.0 / 10 + (tmpkp[3] - '0') * 1.0 / 100;
        d[sum].max = (tmpmax[0] - '0') + (tmpmax[2] - '0') * 1.0 / 10 + (tmpmax[3] - '0') * 1.0 / 100;
        d[sum].min = (tmpmin[0] - '0') + (tmpmin[2] - '0') * 1.0 / 10 + (tmpmin[3] - '0') * 1.0 / 100;
        d[sum].sp = (tmpsp[0] - '0') + (tmpsp[2] - '0') * 1.0 / 10 + (tmpsp[3] - '0') * 1.0 / 100;
        //将字符串形式转换为浮点形式
        sum++;
    }
    return;
}
void f1()//打印股票特性
{
    int i;
    for (i = 0; i < 15; i++)
    {
        if (d[i].sp == d[i].kp) d[i].flag1 = 3;
        if (d[i].sp < d[i].kp) d[i].flag1 = 1;
        if (d[i].sp > d[i].kp) d[i].flag1 = 2;
        if (d[i].min < d[i].sp && d[i].min < d[i].kp) d[i].flag2 = 1;
        if (d[i].max > d[i].sp && d[i].max > d[i].kp) d[i].flag3 = 1;
    }//将特性记录到数组，方便f6函数绘制
    strcpy(str1, "股票特性：\n");
    for (i = 0; i < 15; i++)
    {
        sprintf(str1, "%s%s: ", str1, d[i].date);
        switch (d[i].flag1)
        {
        case 1:sprintf(str1, "%s%s", str1, "实心蓝白蜡烛 "); break;
        case 2:sprintf(str1, "%s%s", str1, "空心红蜡烛 "); break;
        case 3:sprintf(str1, "%s%s", str1, "十字红蜡烛 "); break;
        default:break;
        }
        switch (d[i].flag2)
        {
        case 0:sprintf(str1, "%s%s", str1, "无下影线 "); break;
        case 1:sprintf(str1, "%s%s", str1, "有下影线 "); break;
        default:break;
        }
        switch (d[i].flag3)
        {
        case 0:sprintf(str1, "%s%s", str1, "无上影线\n"); break;
        case 1:sprintf(str1, "%s%s", str1, "有上影线\n"); break;
        default:break;
        }
    }//打印特性
    return;
}
void f2()//计算均值和方差
{
    int i, j;
    double avg[4] = { 0 }, var[4] = { 0 };//avg代表均值，var代表方差，0、1、2、3四个维度分别代表开盘、收盘、最高、最低
    for (i = 0; i < 4; i++)
    {
        double sum1 = 0, sum2 = 0;
        for (j = 0; j < 15; j++)
        {
            switch (i)
            {
            case 0:sum1 += d[j].kp, sum2 += d[j].kp * d[j].kp; break;
            case 1:sum1 += d[j].sp, sum2 += d[j].sp * d[j].sp; break;
            case 2:sum1 += d[j].max, sum2 += d[j].max * d[j].max; break;
            case 3:sum1 += d[j].min, sum2 += d[j].min * d[j].min; break;
            default:break;
            }
        }
        avg[i] = sum1 / 15;//均值
        var[i] = sum2 / 15 - avg[i] * avg[i];//方差=平方的均值-均值的平方
    }
    strcpy(str2, "均值和方差\n");
    for (i = 0; i < 4; i++)
    {
        switch (i)
        {
        case 0:sprintf(str2, "%s开盘价格均值为：%.3lf 方差为：%.3lf\n", str2, avg[i], var[i]); break;
        case 1:sprintf(str2, "%s收盘价格均值为：%.3lf 方差为：%.3lf\n", str2, avg[i], var[i]); break;
        case 2:sprintf(str2, "%s最高价格均值为：%.3lf 方差为：%.3lf\n", str2, avg[i], var[i]); break;
        case 3:sprintf(str2, "%s最低价格均值为：%.3lf 方差为：%.3lf\n", str2, avg[i], var[i]); break;
        default:break;
        }
    }//打印结果
    return;
}
void f3()//判断趋势
{
    int sum = 0, i;
    for (i = 0; i < 14; i++)
    {
        if (d[i].max < d[i + 1].min) sum++;
    }
    sprintf(str3, "%s缺口天数为：", str3);
    sprintf(str3, "%s%d天\n", str3, sum);
    double diff = d[14].sp - d[0].kp;//通过比较最后一天的收盘价格和第一天的开盘价格判断趋势
    sprintf(str3, "%s综合来看，", str3);
    if (diff > 0) sprintf(str3, "%s该股票处于上涨趋势", str3);
    if (diff == 0) sprintf(str3, "%s该股票处于横盘趋势", str3);
    if (diff < 0) sprintf(str3, "%s该股票处于下跌趋势", str3);
    return;
}
void f4()//输出最大值和最小值
{
    int i, j;
    double max[4] = { 0 }, min[4] = { 1e9,1e9,1e9,1e9 };//max和min分别表示最大和最小值，0、1、2、3四个维度分别代表开盘、收盘、最高、最低
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 15; j++)
        {
            switch (i)
            {
            case 0:
            {
                if (max[i] < d[j].kp) max[i] = d[j].kp;
                if (min[i] > d[j].kp) min[i] = d[j].kp;
            }; break;
            case 1:
            {
                if (max[i] < d[j].sp) max[i] = d[j].sp;
                if (min[i] > d[j].sp) min[i] = d[j].sp;
            }; break;
            case 2:
            {
                if (max[i] < d[j].max) max[i] = d[j].max;
                if (min[i] > d[j].max) min[i] = d[j].max;
            }; break;
            case 3:
            {
                if (max[i] < d[j].min) max[i] = d[j].min;
                if (min[i] > d[j].min) min[i] = d[j].min;
            }; break;
            default:break;
            }
        }
    }
    strcpy(str4, "最大值和最小值\n");
    for (i = 0; i < 4; i++)
    {
        switch (i)
        {
        case 0:sprintf(str4, "%s开盘价格最大值为：%.2lf 最小值为：%.2lf\n", str4, max[i], min[i]); break;
        case 1:sprintf(str4, "%s收盘价格最大值为：%.2lf 最小值为：%.2lf\n", str4, max[i], min[i]); break;
        case 2:sprintf(str4, "%s最高价格最大值为：%.2lf 最小值为：%.2lf\n", str4, max[i], min[i]); break;
        case 3:sprintf(str4, "%s最低价格最大值为：%.2lf 最小值为：%.2lf\n", str4, max[i], min[i]); break;
        default:break;
        }
    }//打印结果
    return;
}
void f5()//冒泡排序（收盘价降序）
{
    node tmpd[15];//复制一份d数组，本函数对复制进行修改，不改变原输入
    int i, j;
    for (i = 0; i < 15; i++) tmpd[i] = d[i];
    for (i = 0; i < 14; i++)
    {
        for (j = 0; j < 14 - i; j++)
        {
            node tmp;
            if (tmpd[j].sp < tmpd[j + 1].sp)
            {
                tmp = tmpd[j];
                tmpd[j] = tmpd[j + 1];
                tmpd[j + 1] = tmp;
            }
        }
    }//冒泡排序
    sprintf(str5, "%s按收盘价降序排列后结果为：\n排名  日期    开盘价 最高价 最低价 收盘价\n", str5);
    for (i = 0; i < 15; i++)
    {
        sprintf(str5, "%s%.2d %s %.2lf   %.2lf   %.2lf   %.2lf\n", str5, i + 1, tmpd[i].date, tmpd[i].kp, tmpd[i].max, tmpd[i].min, tmpd[i].sp);
    }
    return;
}
void f6()//绘制K线图
{
    int i;
    setbkcolor(WHITE);
    setcolor(BLACK);
    outtextxy(500, 30, "K线图");
    outtextxy(50, 500, d[0].date);
    outtextxy(400, 500, d[5].date);
    outtextxy(750, 500, d[10].date);
    setcolor(GREEN);
    int y1 = 1200 - d[0].kp * 200 / 1.0, x1 = 50, y2 = 1200 - d[1].kp * 200 / 1.0, x2 = 120;
    for (i = 2; i < 15; i++)
    {
        line(x1, y1, x2, y2);
        x1 = x2;
        x2 += 70;
        y1 = y2;
        y2 = 1200 - d[i].kp * 200 / 1.0;
    }
    setcolor(RED);
    y1 = 1200 - d[0].sp * 200 / 1.0, x1 = 50, y2 = 1200 - d[1].sp * 200 / 1.0, x2 = 120;
    for (i = 2; i < 15; i++)
    {
        line(x1, y1, x2, y2);
        x1 = x2;
        x2 += 70;
        y1 = y2;
        y2 = 1200 - d[i].sp * 200 / 1.0;
    }
    setcolor(EGERGB(100, 200, 200));
    y1 = 1200 - d[0].min * 200 / 1.0, x1 = 50, y2 = 1200 - d[1].min * 200 / 1.0, x2 = 120;
    for (i = 2; i < 15; i++)
    {
        line(x1, y1, x2, y2);
        x1 = x2;
        x2 += 70;
        y1 = y2;
        y2 = 1200 - d[i].min * 200 / 1.0;
    }
    setcolor(BLUE);
    y1 = 1200 - d[0].max * 200 / 1.0, x1 = 50, y2 = 1200 - d[1].max * 200 / 1.0, x2 = 120;
    for (i = 2; i < 15; i++)
    {
        line(x1, y1, x2, y2);
        x1 = x2;
        x2 += 70;
        y1 = y2;
        y2 = 1200 - d[i].max * 200 / 1.0;
    }
    setcolor(GREEN);
    line(950, 500, 1050, 500);
    outtextxy(1100, 500, "开盘价格");
    setcolor(RED);
    line(950, 530, 1050, 530);
    outtextxy(1100, 530, "收盘价格");
    setcolor(EGERGB(100, 200, 200));
    line(950, 560, 1050, 560);
    outtextxy(1100, 560, "最小价格");
    setcolor(BLUE);
    line(950, 590, 1050, 590);
    outtextxy(1100, 590, "最大价格");
    setfillcolor(BLUE);
    bar(500, 640, 510, 650);
    outtextxy(520, 635, "实心蓝白蜡烛");
    setfillcolor(GREEN);
    bar(710, 640, 720, 650);
    outtextxy(730, 635, "空心红蜡烛");
    setfillcolor(RED);
    bar(930, 640, 940, 650);
    outtextxy(950, 635, "实心红蜡烛");
    x1 = 50;
    for (i = 0; i < 14; i++)
    {
        y1 = 1200 - d[i].max * 200 / 1.0;
        switch (d[i].flag1)
        {
        case 1:
        {
            setfillcolor(BLUE);
            bar(x1, y1, x1 + 10, y1 - 10);
        }break;
        case 2:
        {
            setfillcolor(GREEN);
            bar(x1, y1, x1 + 10, y1 - 10);
        }break;
        case 3:
        {
            setfillcolor(RED);
            bar(x1, y1, x1 + 10, y1 - 10);
        }break;
        default:break;
        }
        setcolor(BLACK);
        if (d[i].flag2 == 1) line(x1 + 5, y1 - 3, x1 + 5, y1 + 3);
        if (d[i].flag3 == 1) line(x1 + 5, y1 - 7, x1 + 5, y1 - 13);
        x1 += 70;
    }
}
void drawinit()
{
    cleardevice();
    setbkcolor(BLACK);
    setcolor(EGERGB(0xFF, 0xFF, 0xFF));
    setfontbkcolor(EGERGB(0x00, 0x00, 0x00));
    setfont(24, 0, "宋体");
    outtextxy(100, 100, "1. 股票特性");
    outtextxy(100, 180, "2. 均值和方差");
    outtextxy(100, 260, "3. 股票趋势");
    outtextxy(100, 340, "4. 各类价格最大值和最小值");
    outtextxy(100, 420, "5. 排序");
    outtextxy(100, 500, "6. K线图");
    outtextxy(100, 580, "7. 退出");
}
void draw()
{
    mouse_msg msg = { 0 };
    drawinit();
    int mode = 0;//0代表初始界面，非0代表数字对应界面
    for (; is_run(); delay_fps(60))
    {
        while (mousemsg())
        {
            msg = getmouse();
        }
        int x = msg.x, y = msg.y, click = (int)msg.is_left();
        if (mode == 0 && click == 1)
        {
            if (x < 300 && x>80)
            {
                if (y > 70 && y < 130)
                {
                    mode = 1;
                    cleardevice();
                    outtextrect(0, 0, 1200, 1200, str1);
                    outtextxy(1000, 700, "返回");
                }
            }
            if (x < 300 && x>80)
            {
                if (y > 150 && y < 210)
                {
                    mode = 2;
                    cleardevice();
                    outtextrect(0, 0, 1200, 1200, str2);
                    outtextxy(1000, 700, "返回");
                }
            }
            if (x < 300 && x>80)
            {
                if (y > 230 && y < 290)
                {
                    mode = 3;
                    cleardevice();
                    outtextrect(0, 0, 1200, 1200, str3);
                    outtextxy(1000, 700, "返回");
                }
            }
            if (x < 500 && x>80)
            {
                if (y > 310 && y < 370)
                {
                    mode = 4;
                    cleardevice();
                    outtextrect(0, 0, 1200, 1200, str4);
                    outtextxy(1000, 700, "返回");
                }
            }
            if (x < 200 && x>80)
            {
                if (y > 390 && y < 450)
                {
                    mode = 5;
                    cleardevice();
                    outtextrect(0, 0, 1200, 1200, str5);
                    outtextxy(1000, 700, "返回");
                }
            }
            if (x < 200 && x>80)
            {
                if (y > 470 && y < 530)
                {
                    mode = 6;
                    cleardevice();
                    f6();
                    outtextxy(1000, 700, "返回");
                }
            }
            if (x < 200 && x>80)
            {
                if (y > 550 && y < 610)
                {
                    closegraph();
                    exit(0);
                }
            }
        }
        if (click == 1 && mode != 0 && x > 990 && x < 1050 && y>670 && y < 730)
        {
            drawinit();
            mode = 0;
        }
    }
}
int main()
{
    init();
    f1();
    f2();
    f3();
    f4();
    f5();
    draw();
    return 0;
}