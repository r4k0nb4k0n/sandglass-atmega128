/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyechoi <hyechoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 16:59:44 by hyechoi           #+#    #+#             */
/*   Updated: 2020/12/16 15:16:57 by hyechoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
int main(void)
{
	DDRA = 0xff;
	0b11111111 DDRG = 0x00;
	0b00000000 while (1)
	{
		if (PING & 0b00000010)
			PORTA = 0b10000000;
		else
			PORTA = 0b00000001;
	}
}*/

/*#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

unsigned char arr[10][8]={
{0xff, 0xff, 0x81, 0x7e, 0x7e, 0x81, 0xff, 0xff},
{0xff, 0xff, 0x7b, 0x7d, 0x00, 0x7f, 0xff, 0xff},
{0xff, 0xff, 0x1d, 0x6e, 0x76, 0x79, 0xff, 0xff},
{0xff, 0xff, 0xbd, 0x76, 0x76, 0x89, 0xff, 0xff},
{0xff, 0xe7, 0xeb, 0xed, 0x00, 0xef, 0xff, 0xff},
{0xff, 0xff, 0x70, 0x76, 0x76, 0x8e, 0xff, 0xff},
{0xff, 0xff, 0x83, 0x6d, 0x6e, 0x6e, 0x9d, 0xff},
{0xff, 0xff, 0x7e, 0x9e, 0xe6, 0xf8, 0xff, 0xff},
{0xff, 0xff, 0x89, 0x76, 0x76, 0x89, 0xff, 0xff},
{0xff, 0xff, 0xb9, 0x76, 0x76, 0x81, 0xff, 0xff}
};

int main()
{
	DDRA = 0xff;
	DDRD = 0xff;

	int i, j, count=0;
	//count는 글자가 너무 빨리 넘어가는 것을 막아주는 용도
	while(1)
	{
		for(j=0; j<10; j++){
			while(count<60){
				for(i=0; i<8; i++){
					PORTA = 0x01<<i;
					PORTD = arr[j][i];
					_delay_ms(1);
				}
				count++;
			}
			count = 0;
		}
	}
	return 0;
}*/

#define F_CPU 16000000UL
// ATmega128의 레지스터 등이 정의되어 있음
#include <avr/io.h>
// _delay_ms() 함수 등이 정의되어 있음
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#define __DELAY_BACKWARD_COMPATIBLE_ _ // for Atmel Studio 6
void buzzer(int hz, int count);
void start_sandglass();
void timer_end_animation();

// C 언어의 주 실행 함수
int main(void){
	// start_sandglass();
	// [buzzer test]
	// int i;
	// DDRB = 0x10;
	// while(1) {
	// 	for(i=0; i<1; i++) {
	// 		buzzer(480, 12);
	// 		buzzer(320, 8); 
	// 	}
	// 	_delay_ms(2000); 
	// }

	// [기울기 센서 test]
	DDRA = 0xff; // 출력모드
	DDRG = 0x00; // 입력모드
	while(1){
		start_sandglass();
	}
}


void buzzer(int hz, int count) {
	int i, j, ms;
	ms = 500/400;
	
	for(i=0; i<count; i++) {
		PORTB = 0x10; 
		_delay_ms(ms);
		PORTB = 0x00; 
		_delay_ms(ms);
	} 
}


void start_sandglass(){
	unsigned char i; // 8비트의 변수 선언
	unsigned char j;
	unsigned long count;

	count = 0;
	// LDM(8*8)에 표시할 모양 비트들을 저장(삼각형 모양)
	char COLS[11][8] = {
		{
			0b00000000,
			0b10000001,
			0b11000011,
			0b11100111,
			0b11111111,
			0b11111111,
			0b11111111,
			0b11111111,
		},
		{
			0b00011000,
			0b10000001,
			0b11000011,
			0b11100111,
			0b11111111,
			0b11111111,
			0b11111111,
			0b11100111,
		},
		{
			0b00111100,
			0b10000001,
			0b11000011,
			0b11100111,
			0b11111111,
			0b11111111,
			0b11111111,
			0b11000011,
		},
		{
			0b01111110,
			0b10000001,
			0b11000011,
			0b11100111,
			0b11111111,
			0b11111111,
			0b11111111,
			0b10000001,
		},
		{
			0b11111111,
			0b10000001,
			0b11000011,
			0b11100111,
			0b11111111,
			0b11111111,
			0b11111111,
			0b00000000,
		},
		{
			0b11111111,
			0b10011001,
			0b11000011,
			0b11100111,
			0b11111111,
			0b11111111,
			0b11100111,
			0b00000000,
		},
		{
			0b11111111,
			0b10111101,
			0b11000011,
			0b11100111,
			0b11111111,
			0b11111111,
			0b11000011,
			0b00000000,
		},
		{
			0b11111111,
			0b11111111,
			0b11000011,
			0b11100111,
			0b11111111,
			0b11111111,
			0b10000001,
			0b00000000,
		},
		{
			0b11111111,
			0b11111111,
			0b11011011,
			0b11100111,
			0b11111111,
			0b11100111,
			0b10000001,
			0b00000000,
		},
		{
			0b11111111,
			0b11111111,
			0b11111111,
			0b11100111,
			0b11111111,
			0b11000011,
			0b10000001,
			0b00000000,
		},
		{
			0b11111111,
			0b11111111,
			0b11111111,
			0b11111111,
			0b11100111,
			0b11000011,
			0b10000001,
			0b00000000,
		},
	};

	DDRA = 0b11111111;
	DDRD = 0b11111111;

	bool is_electricity_on = false;
	if(PING & 0b00000010){
		//전기 통하고 있는 상태 모래시계 작동중
		is_electricity_on = true;
	}

	for (j = 0; j < 11; j++)
	{
		while (count < 30)
		{
			// 8줄의 LED 매트릭스가 켜지도록 동작을 반복함
			for (i = 0; i < 8; i++)
			{
				// 8비트의 ROWS 신호 중 1비트만 논리 1로 출력
				if (is_electricity_on)
				{
					PORTA = (0b10000000 >> (i));
					if(!(PING & 0b00000010)){
						//전기 안통하게 될 때
						return;
					}
				}else{
					PORTA = (0b00000001 << (i));
					if(PING & 0b00000010){
						//전기 통하게 될 때
						return;
					}
				}
				
				// COLS 신호에 데이터 출력
				PORTD = COLS[j][i];

				_delay_ms(2); // 2ms 동안 대기
			}
			count++;
		}
		count = 0;
	}
	timer_end_animation();
	
	// 함수의 형태와 같이 정수형(int)의 값을 반환함
	// return 1;
}


void timer_end_animation(){
	unsigned char i; // 8비트의 변수 선언

	char COLS[11][8] = {
		{
			0b00000000,
			0b10000001,
			0b11000011,
			0b11100111,
			0b11111111,
			0b11111111,
			0b11111111,
			0b11111111,
		},
		{
			0b11111111,
			0b11111111,
			0b11111111,
			0b11111111,
			0b11100111,
			0b11000011,
			0b10000001,
			0b00000000,
		}
	};
	
	//전기 안통하고 있는 상태 모래시계 작동중
	bool is_electricity_on = false;
	int electricity_num = 0;
	if(PING & 0b00000010){
		//전기 통하고 있는 상태 모래시계 작동중
		is_electricity_on = true;
		electricity_num = 1;
	}

	while (1)
	{
		// 8줄의 LED 매트릭스가 켜지도록 동작을 반복함
		for (i = 0; i < 8; i++)
		{
			// 8비트의 ROWS 신호 중 1비트만 논리 1로 출력
			PORTA = (0b10000000 >> (i));
			
			// COLS 신호에 데이터 출력
			PORTD = COLS[electricity_num][i];

			_delay_ms(2); // 2ms 동안 대기
		}
		
		if (is_electricity_on)
		{
			if(!(PING & 0b00000010)){
				//전기 안통하게 될 때
				break;
			}
		}else{
			if(PING & 0b00000010){
				//전기 통하게 될 때
				break;
			}
		}
		
	}
	
}