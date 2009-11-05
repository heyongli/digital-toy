#include <iot13v.h>

void Delay()

	{

 	  unsigned char a, b;

	  for (a = 1; a; a++)

		 for (b = 1; b; b++)

			;

	}



void LED_On(int i)

	{

 	 PORTB=~(1<<i);	/* 输出低电平，使LED发光 */

	 Delay();

	}



void main()

	{

	int i;

	DDRB = 0xFF;	/* 定义B口为输出*/

	PORTB = 0xFF;	/* 关闭全部LED */



	while (1)

		{

		/* 往前步进 */

		for (i = 0; i < 8; i++)

			LED_On(i);

		/* 往后步进 */

		for (i = 8; i > 0; i--)

			LED_On(i);

		/*  跳 跃  */

		for (i = 0; i < 8; i += 2)

			LED_On(i);

		for (i = 7; i > 0; i -= 2)

			LED_On(i);

		}

	}
