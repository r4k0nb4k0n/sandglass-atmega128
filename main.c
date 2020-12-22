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

#define F_CPU 16000000UL
// ATmega128의 레지스터 등이 정의되어 있음
#include <avr/io.h>
#include <avr/interrupt.h>
// _delay_ms() 함수 등이 정의되어 있음
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#define TIMER_AMOUNT 10
#define __DELAY_BACKWARD_COMPATIBLE_ _ // for Atmel Studio 6

volatile unsigned int tick;
volatile unsigned int timer;
volatile unsigned int status;

void buzzer();
void init_timer();
void start_sandglass();
void timer_end_animation();
void timer_end_animation_once();

ISR(TIMER0_OVF_vect)
{
	tick++;
	if (timer == 0)
	{
		timer_end_animation_once();
		timer_end_animation();
	}
	else if (tick == 361)
	{
		PORTB = 0b00001000;
	}
	else if (tick == 721)
	{
		PORTB = 0b11110111;
		tick = 0;
		timer--;
	}
}

// C 언어의 주 실행 함수
int main(void)
{

	// Prescaler = FCPU/1024
	TCCR0 |= (1 << CS02) | (1 << CS00);
	//Enable Overflow Interrupt Enable
	TIMSK |= (1 << TOIE0);
	//Initialize Counter
	TCNT0 = 0;
	//Initialize our varriable
	init_timer();
	// [기울기 센서 test]
	DDRA = 0xff; // 출력모드
	DDRB = 0x10;
	DDRG = 0x00; // 입력모드
	sei();
	while (1)
	{
		start_sandglass();
	}
}

void buzzer()
{
	PORTB = 0x10;
	_delay_ms(0.1);
	PORTB = 0x00;
	_delay_ms(0.1); // 2ms 동안 대기
}

void init_timer()
{
	tick = 0;
	timer = TIMER_AMOUNT;
}

void start_sandglass()
{
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
	if (PING & 0b00000010)
	{
		//전기 통하고 있는 상태 모래시계 작동중
		is_electricity_on = true;
		status = PING & 0b00000010;
		init_timer();
	}
	else
	{
		status = !(PING & 0b00000010);
	}

	for (j = 0; j < 11; j++)
	{
		while (count < 90)
		{
			// 8줄의 LED 매트릭스가 켜지도록 동작을 반복함
			for (i = 0; i < 8; i++)
			{
				// 8비트의 ROWS 신호 중 1비트만 논리 1로 출력
				if (is_electricity_on)
				{
					PORTA = (0b10000000 >> (i));
					if (!(PING & 0b00000010))
					{
						//전기 안통하게 될 때
						status = !(PING & 0b00000010);
						init_timer();
						return;
					}
				}
				else
				{
					PORTA = (0b00000001 << (i));
					if (PING & 0b00000010)
					{
						//전기 통하게 될 때
						status = PING & 0b00000010;
						init_timer();
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

void timer_end_animation()
{
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
		}};

	//전기 안통하고 있는 상태 모래시계 작동중
	bool is_electricity_on = false;
	int electricity_num = 0;
	if (PING & 0b00000010)
	{
		//전기 통하고 있는 상태 모래시계 작동중
		is_electricity_on = true;
		electricity_num = 1;
		status = PING & 0b00000010;
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
			if (!(PING & 0b00000010))
			{
				//전기 안통하게 될 때
				status = !(PING & 0b00000010);
				init_timer();
				break;
			}
		}
		else
		{
			if (PING & 0b00000010)
			{
				//전기 통하게 될 때
				status = PING & 0b00000010;
				init_timer();
				break;
			}
		}
	}
}

void timer_end_animation_once()
{
	unsigned char i; // 8비트의 변수 선언
	unsigned char j;

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
		}};

	//전기 안통하고 있는 상태 모래시계 작동중
	bool is_electricity_on = false;
	int electricity_num = 0;
	if (PING & 0b00000010)
	{
		//전기 통하고 있는 상태 모래시계 작동중
		is_electricity_on = true;
		electricity_num = 1;
		status = PING & 0b00000010;
	}

	j = 0;
	while (j < 120)
	{
		j++;
		// 8줄의 LED 매트릭스가 켜지도록 동작을 반복함
		for (i = 0; i < 8; i++)
		{
			// 8비트의 ROWS 신호 중 1비트만 논리 1로 출력
			PORTA = (0b10000000 >> (i));
			// COLS 신호에 데이터 출력
			PORTD = COLS[electricity_num][i];
			buzzer();
		}

		if (is_electricity_on)
		{
			if (!(PING & 0b00000010))
			{
				//전기 안통하게 될 때
				status = !(PING & 0b00000010);
				init_timer();
				break;
			}
		}
		else
		{
			if (PING & 0b00000010)
			{
				//전기 통하게 될 때
				status = PING & 0b00000010;
				init_timer();
				break;
			}
		}
	}
}