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


void main(void){

	// Area de setup dos timers, interrupções ou serial



	// Area do programa

	while(1){







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
	return;
}

