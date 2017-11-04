/* 
	Programa para receber e reenvir informação pela serial com 8051
							IFSP - N6lb5 2017
*/

	/* Parâmetros funcionais do programa:

	1) A informação recebida da serial deve ser mostrada nos 2 display de 7 seg em loop
	2) A informação deve ter no máximo 100 caracteres segundo tabela ASCII
	3) O último caracter deve ser obrigatoriamente a tecla enter (0x0A)
	4) Ao receber a informação ela deve ser reenviada ao precionar um botão
	5) Quando esse botão de envio for precionado, a o dado mostrado nos displays
		deve ser a informação que será enviada
	6) Quando um segundo botão for precionado o indice da informação enviado
		(quantidade do caracter - ex: mensagem tem 3 caracteres, estou mandando 
		o de número 2), deve ser mostrada no display

	*/

# include <mcs51/at89c51ed2.h>			// biblioteca do 8051

// Declaração da tabela de conversão decimal para o display 7 segmentos
// no display anodo comum os bits são : pt g f e d c b a (pt como MSB)

// Como não se altera será guardada na memória ROM (__code), acendimento com 0.

__code unsigned char NUM0 = 0b11000000; // acende a,b,c,d,e,f para número 0
__code unsigned char NUM1 = 0b11111001;	// acende b,c para número 1
__code unsigned char NUM2 = 0b10100100;	// acende a,b,d,e,g para número 2
__code unsigned char NUM3 = 0b10110000;	// acende a,b,c,d,g para número 3
__code unsigned char NUM4 = 0b10011001;	// acende b,c,g,f para número 4
__code unsigned char NUM5 = 0b10010010;	// acende a,c,d,f,g para número 5
__code unsigned char NUM6 = 0b10000010;	// acende a,c,d,e,f,g para número 6
__code unsigned char NUM7 = 0b11111000;	// acende a,b,c para o número 7
__code unsigned char NUM8 = 0b10000000; // acende a,b,c,d,e,f,g para o número 8
__code unsigned char NUM9 = 0b10010000; // acende a,b,c,d,f,g para o número 9
__code unsigned char NUMa = 0b10001000;	// acende a,b,c,e,f,g para o número A
__code unsigned char NUMb = 0b10000011; // acende c,d,e,f,g para o número b
__code unsigned char NUMc = 0b11000110; // acende a,d,e,f,g para o número C
__code unsigned char NUMd = 0b10100001; // acende b,c,d,e,g para o número d
__code unsigned char NUMe = 0b10000110; // acende a,d,e,f,g para o número E
__code unsigned char NUMf = 0b10001110; // acnede a,e,f,g para o número F


	// Area de declaração de variável

		// volatile pois é modificada de maneira inesperada pelo programa
		// __pdata : memória RAM externa de 256 bytes

__pdata unsigned char mostrar[100];				// array que guarda a mensagem a ser mostrada
__pdata unsigned volatile char info_re[100] ; 	// array da mensagem recebida
__data unsigned volatile char r = 0x00;			// contador do array
__data unsigned volatile char cont = 0x00;		// contador do timer 0 - tempo 1 seg
__data unsigned char caractere = 0x00;			// armazena dado a ser mostrado	
__data unsigned char display = 0x00;			// resultado da mascara com o dado 
__data unsigned char saida = 0x00;				// mostra o dado
__data unsigned char m = 0x00;			// contador do array mostrar[100]
__data unsigned char c = 0x00;			// contador para copiar array
__bit volatile esp = 0;					// flag para esperar 1 seg
__bit volatile msg = 0;					// flag da tecla enter
__bit bt = 0;							// chaveia os displays
__bit bt0_ant = 0;						// estado anterior do botão
__bit bt0 = 0;							// estado atual do botão (borda de descida)
__bit bt3 = 0;							// mostra o indice da transmissão


void main(void){

	// Area de setup dos timers, interrupções ou serial

	SM0 = 0;			// Modo 1 da serial, timer 1, 8 bits, assincrono (SCON)
	SM1 = 1; 			// (SCON)
	REN = 1;			// habilita a recepção de dados pela serial (SCON)

	PCON = 0b10000000;	// calculo do baud rate (PCON)

	TR1 = 1;			// habilita timer 1 (TCON)
	TR0 = 1;			// habilita timer 0 
	
	EA = 1;				// habilita a chave geral de interrupção (IE)
	ES = 1;				// habilita a interrupção da serial 
	ET0 = 1;			// habilita a inerrupção do timer 0

	TMOD = 0b00100001;	// timer 1, modo 2 auto reload, controle por software
						// timer 0, modo 1, controle por software
	TH1 = 0xE6;			// Valor inicial de contagem do timer 1
	TL1 = 0xE6;			// baud rate 2400 bps - 12MHz clock (tabela)

	// Area do programa

	while(1){

		bt0 = P3_2;			// leitura do botão acionado por borda de descida
		bt3 = P1_0;			// leitura do botão acionado por nível

		if(msg == 1 && r == 0){			// indica se a mensagem completa foi recebida

			for(c = 0 ; c < 100; c++){		// copia o array
				P0_7 = 1;					// indica se entrou nessa rotina
				mostrar[c] = info_re[c];
			}
			if(m < 100){					//contador para mostrar a mensagem recebida
				if(mostrar[m] == 0x0A){
					m = 0;
				}

				else{
					TH0 = 0x3C;			// carrega valor inicial no timer 0 (faltando 50ms)
					TL0 = 0x0B;

					while(esp == 0){	// função para esperar 1 seg

						if(bt0 == 1 && bt3 == 1){	// caso nenhum botão for acionado
						
							P0 = ~mostrar[m];		// mostra o conteudo do array invertido nos LEDs
							caractere = mostrar[m];	// prepara para aplicar a máscara 
						}			
						else{ /* transmissão*/ }

						if(bt==1){								// qual display está ligado MSB ou LSB

							display = (caractere & 0b00001111);	// aplica a máscara para mostrar o LSB
						}
						else {
							display=(caractere & 0b11110000)>>4; // aplica a máscara para mostrar o MSB
						}

						switch (display){					// decodificação do display

							case 0:   saida = NUM0; break;
							case 1:   saida = NUM1; break;
							case 2:   saida = NUM2; break;
							case 3:   saida = NUM3; break;
							case 4:   saida = NUM4; break;
							case 5:   saida = NUM5; break;
							case 6:   saida = NUM6; break;
							case 7:   saida = NUM7; break;
							case 8:   saida = NUM8; break;
							case 9:   saida = NUM9; break;
							case 0x0a:  saida = NUMa; break;
							case 0x0b:  saida = NUMb; break;
							case 0x0c:  saida = NUMc; break;
							case 0x0d:  saida = NUMd; break;
							case 0x0e:  saida = NUMe; break;
							case 0x0f:  saida = NUMf; break;
							default : saida = 0b0111111; break;
						}		

						P3_6 = 1;			// desliga os displays
						P3_7 = 1;			
						P2 = saida;			// mostra a informação no display
						P3_7 = bt^1; 		// comando para ligar o display
						P3_6 = bt;
						bt = bt ^1;			// chaveia os displays, invertendo o seu valor conforme o clock
					}
				m++;				// incrementa para mostrar o elemento seguinte do array recebido
				esp = 0;			// zera o tempo de espera
				}
			}
			else{
				m = 0;
			}

		}		
		else{	P0 = 0xFF;		// indica se a mensagem ainda não foi recebida
				P0_7 = 0;
			}
		
	}
}


// Area das sub-rotinas de interrupções

void externa_1 (void) __interrupt(0){
	return;
}

void externa_2 (void) __interrupt(2){
	return;
}

void timer_0 (void) __interrupt(1){

	cont++;

	if(cont == 20){			// (50 ms * 20) = 1 seg
		esp = 1;
		cont = 0;
	}
	else{}

	return;
}

void timer_1 (void) __interrupt(3){
	return;
}

void serial (void) __interrupt(4){

	if (RI == 1){						// flag de recepção pela serial
		if (r < 100){					 
			info_re[r] = SBUF;			
			if(info_re[r] == 0x0A){		// último caracter da mensagem é enter(0x0a)
				msg = 1;				// como recebeu o último caracter, flag de mensagem completa
				r = 0;					// reseta o contador de posição do array
				c = 0;
			}
			else{
				r++;					// caso não recebeu o "enter", continua guardando informação
			}							// na próxima posição do array
			
		}

		else{
			r = 0;
		}

		RI = 0;								// reseta a flag de recepção da serial
	
	}
	else{}
	return;
}

