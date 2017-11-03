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

	// Area de declaração de variável

		// volatile pois é modificada de maneira inesperada pelo programa
		// __pdata : memória RAM externa de 256 bytes

__pdata unsigned volatile char info_re[100] ; 	// array da mensagem recebida
__data unsigned volatile char r = 0x00;			// contador do array
__bit volatile msg = 0;						// flag de toda a mensagem foi recebida

			


void main(void){

	// Area de setup dos timers, interrupções ou serial

	SM0 = 0;			// Modo 1 da serial, timer 1, 8 bits, assincrono (SCON)
	SM1 = 1; 			// (SCON)
	REN = 1;			// habilita a recepção de dados pela serial (SCON)

	PCON = 0b10000000;	// calculo do baud rate (PCON)

	TR1 = 1;			// habilita timer 1 (TCON)

	EA = 1;				// habilita a chave geral de interrupção (IE)
	ES = 1;				// habilita a interrupção da serial (IE)

	TMOD = 0b00100000;	// timer 1, modo 2 auto reload, controle por software

	TH1 = 0xE6;			// Valor inicial de contagem do timer 1
	TL1 = 0xE6;			// baud rate 2400 bps - 12MHz clock (tabela)

	// Area do programa

	while(1){

		P0_1 = 1;

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
	return;
}

void timer_1 (void) __interrupt(3){
	return;
}

void serial (void) __interrupt(4){

	if (RI == 1){						// flag de recepção pela serial
		if (r < 100){					 
			info_re[r] = SBUF;			
			if(info_re[r] == 0x0a){		// último caracter da mensagem é enter(0x0a)
				msg = 1;				// como recebeu o último caracter, flag de mensagem completa
				r = 0;					// reseta o contador de posição do array
			}
			else{
				r++;					// caso não recebeu o "enter", continua guardando informação
										// na próxima posição do array
			}
		}

		else {
			r = 0;
		}
	RI = 0;								// reseta a flag de recepção da serial
	P0_1 = 0;							// indicador se recebeu informação,acende led

	}
	else{

	}
	return;
}

