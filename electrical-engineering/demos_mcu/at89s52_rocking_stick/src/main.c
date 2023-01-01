// 使用 AT89S52 单片机控制的摇摇棒，通过摇晃可看到对应的图案
// 是全息风扇的低配版本

#include <at89x52.h>
#define KEY P3_7                // 定义画面切换按键`
unsigned char KY;               // KY作用在后面说明
unsigned char disp;             // 显示汉字指针
unsigned char pic = 0, num = 0; // pic为按键次数；num为中断次数

const unsigned char love[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x3F, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20,
    0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x0F,
    0x04, 0x10, 0x02, 0x20, 0x02, 0x20, 0x02, 0x20, 0x02, 0x20, 0x04, 0x10, 0xF8, 0x0F, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFE, 0x07, 0x00, 0x08, 0x00, 0x10, 0x00, 0x20, 0x00, 0x20, 0x00, 0x10,
    0x00, 0x08, 0xFE, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x3F, 0x82, 0x20, 0x82, 0x20,
    0x82, 0x20, 0x82, 0x20, 0x82, 0x20, 0x82, 0x20, 0x82, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*LOVE*/
};

const unsigned char loveyou[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x78, 0x00, 0xFC, 0x00, 0xFE, 0x01, 0xFE, 0x03, 0xFE, 0x07, 0xFE, 0x0F, 0xFE, 0x1F, 0xFC, 0x3F,
    0xF8, 0x7F, 0xFC, 0x3F, 0xFE, 0x1F, 0xFE, 0x0F, 0xFE, 0x07, 0xFE, 0x03, 0xFE, 0x01, 0xFC, 0x00,
    0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*心形图案*/
};

const unsigned char hehe[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x01, 0x40, 0x01, 0xC0, 0x01, 0x00, 0x00, 0x00, 0x00,
    0xF0, 0x0F, 0x08, 0x10, 0x04, 0x20, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x3F, 0x08, 0x00, 0x04, 0x00,
    0x04, 0x00, 0x04, 0x00, 0x08, 0x00, 0xF0, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x20,
    0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x3F, 0x08, 0x00, 0x04, 0x00,
    0x04, 0x00, 0x04, 0x00, 0x08, 0x00, 0xF0, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x04, 0x20, 0x08, 0x10,
    0xF0, 0x0F, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x01, 0x40, 0x01, 0xC0, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*呵呵o(∩_∩)o图案*/
};

const unsigned char hanzi[] = {
    0x20, 0x04, 0x20, 0x04, 0x22, 0x42, 0x22, 0x82,
    0xFE, 0x7F, 0x21, 0x01, 0x21, 0x01, 0x20, 0x10,
    0x20, 0x10, 0xFF, 0x08, 0x20, 0x07, 0x22, 0x1A,
    0xAC, 0x21, 0x20, 0x40, 0x20, 0xF0, 0x00, 0x00, /*"我",0*/

    0x00, 0x40, 0x40, 0x20, 0xB2, 0xA0, 0x96, 0x90,
    0x9A, 0x4C, 0x92, 0x47, 0xF6, 0x2A, 0x9A, 0x2A,
    0x93, 0x12, 0x91, 0x1A, 0x99, 0x26, 0x97, 0x22,
    0x91, 0x40, 0x90, 0xC0, 0x30, 0x40, 0x00, 0x00, /*"爱",1*/
                                                    /* (16 X 16 , 宋体 ) */

    0x80, 0x00, 0x40, 0x00, 0xF0, 0x7F, 0x2C, 0x00,
    0x43, 0x10, 0x20, 0x0C, 0x98, 0x03, 0x0F, 0x21,
    0x0A, 0x40, 0xE8, 0x3F, 0x08, 0x00, 0x88, 0x00,
    0x28, 0x03, 0x1C, 0x1C, 0x08, 0x08, 0x00, 0x00, /*"你",2*/
                                                    /* (16 X 16 , 宋体 ) */

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x5F,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*"！",3*/

    0x20, 0x04, 0x20, 0x04, 0x22, 0x42, 0x22, 0x82,
    0xFE, 0x7F, 0x21, 0x01, 0x21, 0x01, 0x20, 0x10,
    0x20, 0x10, 0xFF, 0x08, 0x20, 0x07, 0x22, 0x1A,
    0xAC, 0x21, 0x20, 0x40, 0x20, 0xF0, 0x00, 0x00, /*"我",0*/

    0x00, 0x40, 0x40, 0x20, 0xB2, 0xA0, 0x96, 0x90,
    0x9A, 0x4C, 0x92, 0x47, 0xF6, 0x2A, 0x9A, 0x2A,
    0x93, 0x12, 0x91, 0x1A, 0x99, 0x26, 0x97, 0x22,
    0x91, 0x40, 0x90, 0xC0, 0x30, 0x40, 0x00, 0x00, /*"爱",1*/
                                                    /* (16 X 16 , 宋体 ) */

    0x80, 0x00, 0x40, 0x00, 0xF0, 0x7F, 0x2C, 0x00,
    0x43, 0x10, 0x20, 0x0C, 0x98, 0x03, 0x0F, 0x21,
    0x0A, 0x40, 0xE8, 0x3F, 0x08, 0x00, 0x88, 0x00,
    0x28, 0x03, 0x1C, 0x1C, 0x08, 0x08, 0x00, 0x00, /*"你",2*/
                                                    /* (16 X 16 , 宋体 ) */

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x5F,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*"！",3*/

    0x20, 0x04, 0x20, 0x04, 0x22, 0x42, 0x22, 0x82,
    0xFE, 0x7F, 0x21, 0x01, 0x21, 0x01, 0x20, 0x10,
    0x20, 0x10, 0xFF, 0x08, 0x20, 0x07, 0x22, 0x1A,
    0xAC, 0x21, 0x20, 0x40, 0x20, 0xF0, 0x00, 0x00, /*"我",0*/

    0x00, 0x40, 0x40, 0x20, 0xB2, 0xA0, 0x96, 0x90,
    0x9A, 0x4C, 0x92, 0x47, 0xF6, 0x2A, 0x9A, 0x2A,
    0x93, 0x12, 0x91, 0x1A, 0x99, 0x26, 0x97, 0x22,
    0x91, 0x40, 0x90, 0xC0, 0x30, 0x40, 0x00, 0x00, /*"爱",1*/
                                                    /* (16 X 16 , 宋体 ) */

    0x80, 0x00, 0x40, 0x00, 0xF0, 0x7F, 0x2C, 0x00,
    0x43, 0x10, 0x20, 0x0C, 0x98, 0x03, 0x0F, 0x21,
    0x0A, 0x40, 0xE8, 0x3F, 0x08, 0x00, 0x88, 0x00,
    0x28, 0x03, 0x1C, 0x1C, 0x08, 0x08, 0x00, 0x00, /*"你",2*/
                                                    /* (16 X 16 , 宋体 ) */

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x5F,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*"！",3*/
};

/*****函数声明*****/
void display1_ltor(void);
void display2_ltor(void);
void display2_rtol(void);
void display3(void);
void display4(void);

/*****n（us）延时子程序*****/
void DelayUs(unsigned int N)
{
   unsigned int x;
   for (x = 0; x <= N; x++)
      ;
}

/*****中断服务程序*****/
void interrupt_handler(void) __interrupt 1
{
   ;
   KY = ~KY; // 每个摇动来回水银开关会在摆幅两端分别产生下降沿中断，只提取其中一次（从左向右摇才显示）
   if (KY == 0)
   {
      num++;       // 计算中断次数
      switch (pic) // 选择画面
      {
      case 0:
      {
         display1_ltor();
      }
      break;
      case 1:
      {
         display2_ltor();
      }
      break;
      case 2:
      {
         display3();
      }
      break;
      case 3:
      {
         display4();
      }
      break;
      default:
      {
         display1_ltor();
      }
      }
   }
}

/*****主函数*****/
void main(void)
{
   IT0 = 1; // 开启 INT0 中断
   EX0 = 1; // 开启外部中断
   EA = 1;  // 开总中断，下降沿中断
   KY = 0;
   KEY = 1;

   while (1) // 主程序中只检测按键
   {
      if (KEY == 0) // 画面切换键按下
      {
         DelayUs(10000); // 按键去抖
         if (KEY == 0)
            ;
         pic++;
         while (KEY == 0)
            ;
      }
      if (pic > 3)
         pic = 0;
   }
}

/*****显示子程序1（汉字）*****/
void display1_ltor(void)
{
   unsigned char i;
   if (num > 10)
   {
      disp++;
      num = 0;
   } // 12个汉字分为3次显示完（每次显示4个），每中断10次切换
   if (disp > 2)
      disp = 0;
   DelayUs(5200); // 此处延时时间依各硬件差别而各不相同，试着调整使得显示内容居中即可
   for (i = 0; i < 64; i++)
   {
      P0 = ~hanzi[disp * 128 + i * 2];
      P2 = ~hanzi[disp * 128 + i * 2 + 1];
      DelayUs(100);
   }
}

/*****显示子程序2（LOVE）*****/
void display2_ltor(void)
{
   unsigned char i;
   DelayUs(4000);
   for (i = 0; i < 64; i++)
   {
      P0 = ~love[i * 2];
      P2 = ~love[i * 2 + 1];
      DelayUs(140);
   }
}
/*****显示子程序3（心形图案）*****/
void display3(void)
{
   unsigned char i;
   DelayUs(4000);
   for (i = 0; i < 64; i++)
   {
      P0 = ~loveyou[i * 2];
      P2 = ~loveyou[i * 2 + 1];
      DelayUs(120);
   }
}

/*****显示子程序4（呵呵o(∩_∩)o图案）*****/
void display4(void)
{
   unsigned char i;
   DelayUs(4000);
   for (i = 0; i < 64; i++)
   {
      P0 = ~hehe[i * 2];
      P2 = ~hehe[i * 2 + 1];
      DelayUs(120);
   }
}
