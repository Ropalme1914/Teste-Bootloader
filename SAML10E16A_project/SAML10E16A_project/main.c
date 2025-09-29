/*
 * Created: 25/11/2021 16:15:39
 * Author : Julio Cesar 
 *
 * Versão : 2.0.0
 * - Esta versão foi ajustado onde o watch dog saiu de dentro do Main principal e ficou somente dentro da rotina da FLAG ( take_measurement = 1 ) gerenciada pelo Timer2 de 1s do loop de leitura do Sensiriun
 *   Assim se o timer se perder, e nao entrar dentro da rotina causando o acionamento da flag take_measurement = 1 , no Main principal o Watch dog sera acionado. Assim o sistema não para nunca. 
 *   Fizemos isso por que em certos momentos o Tmer 2 se perdia a o Watch Dog nao acionava nada ou seja fica no main principal sem entrar no loop causado pela flag take_measurement = 1, e estava tudo bem, porem 
 *   nunca mais o sensiriun era lido.
 *   Nesta versao colocamos tambem internamente os coeficientes A e B , onde ficou de forma " errada " mas ok a função:
 *   
 *   Temp Ajustada = Temp. * B ( ganho ) - A ( offset )    , note que o offset esta negativo e deveria ser +, mas tudo bem.
 *
 *
 * Versão : 2.1.0
 * - Esta versão foi ajustado onde conseguimos colocar corretamente dentro da funcao ( Fabrication Number. H ) o serial number, versao do hardware e firmware e data de fabricação
 *   
 *
 *
 * Versão : 2.2.0
 * - esta versão nos passamos a resetar a rede I2c, quando ocorrer o WhatchDog, pois observamos que mesmo apos o reset do watchdog o sensor da sensirium continuava travado, sem atualizar os valores, mas o 
 *   modbus continuava a operar. Assim é o tradicional congela dados sem parar a rede RS485.
 *
 *
 *
 Versão : 2.3.0
 * - esta versão nos passamos a resetar a rede I2c, quando ocorrer o WhatchDog, pois observamos que mesmo apos o reset do watchdog o sensor da sensirium continuava travado, sem atualizar os valores, mas o
 *   modbus continuava a operar. Assim é o tradicional congela dados sem parar a rede RS485.
 *
 *
 *
 Versão : 2.4.0
 *   Esta versão nos passamos a ler o sensiriun de uma forma diferente, 
 *   Configuramos ele de uma forma que voce so precisa ler ele, nãa precisa escrever nele para dai ler ele.
 *   Esse função esta descrito ( comentado ) abaixo no sensiriun 
 *   Alem disso nesta versao nos resetamos o watchdog, caso os valores lidos se repetem mais de 25 x ( temp ou umidade , continuamente ) o que caracteriza que travou o sensor, alem que
 *   Resetamos tambem se o Status do Sensor , passa de 0 ( normal ) para 1 , que seria uma falha.
 *
 *
 *
 Versao : 2.5.0
 *
 *   Esta versão nós separamos o Watch dog de forma que se acontecer o status do chip STH35 vier a ser diferente de 0 ira ocorrer o disparo do Watch Dog independente de ter ou nao travo os valores repetidos da temp e 
 *   umidade. Se os valores da temp. e umidade tambem tiverem travado ira ocorrer o watch dog de forma independente.
 *
 *
 *
 Versao : 2.5.1
 *
 *   Esta versão nós fizemos os seguintes ajustes:
 *   Colocamos a Versao 2.5.1 nas informações internas ( esquecemos de fazer isso na versao 2.5.0 )
 *   Aumentamos o vetor de dados onde ficam as informações do Modbus visto que atualmente o vetor tem espaço de 1 até 50 e vamos aumentar ate 55 e testar se tudo vai ficar ok.
 *   
 *   #define REG_INPUT_NREGS 55		// Function code 0x04 ( mudamos para 50 )
 *   
 *   A ideia é nesse  endereço colocar as variaveis de controle de erro do chip, do modbus, e do looping interno. 
 *
 *
 *
 *
 Versao : 1.1.0  para o novo Hardware 2.0.0
 *
 *   Este novo Hardware tem o pino de reset do chip da Sensiriun, assim agora temos um reset fisico de verdade. 
 *   Nos testamos esse pulso e vimos ele no osciloscopio.
 *   Assim este pino inicia em alta, ao ligar o processador ao rodar o programa este pino inicia alto, passa um tempinho ele desce fica 10ms e volta para o estado alto. Este 10ms que reseta ele.
 *   Assim a função de reset deste pino é assim puxa a tensão para baixo ( zero ).
 *
 *
 *
 Versao : 1.2.5  para o novo Hardware 2.0.0
 *
 *   Nesta versão foi pensado alguma mudancas importantes
 *   a) Agora nos não resetamos mais o Watch Dog, pelo simples fato que ocorreu um erro nele, por exemplo, se o estatus do sensiriun era diferente de zero, nos ja resetavamos o processador de imediato.
 *      Agora fazemos o diferente, para cada erro do status do chip sensiriun, nos contamos um contator e assim, quando esse contador chegar em 5 ( ou seja se o chip tiver 5 erros de status continuos ) nos entendemos 
 *      que o chip de fato travou dando erro continuo, e assim nos resetamos de fato o processador, e damos o pulso de reset no Sensiriun quando o processador reinicia
 *    
 *   b) Outro fator importante que fizemos foi que, nós somente atualizamos os valores nos endereços Modbus, se o Status do Chip Sensiriun, for igual ao zero, ou seja que a leitura deu certo.
 *      Isso garante que não teremos leituras indevidas escritas no Modbus, que poderiam causar leituras estranhas ao cliente que esta lendo o sensor.
 *
 *
 *
 Versao : 1.2.6  para o novo Hardware 2.0.0
 *
 *  Nesta versao nos fizemos a calibração do novo lote para fazer a linha de tendencia do lote. Assim chegamos em um novo Offset e Ganho
 *  São eles:
 *  Ganho = 0,8277   e  OffSet = - 4,4188   Lembrando a equação esta escrita:
 *
 *     Umidade Ajustada  = Umidade * Ganho ( B )  - Offset ( A ) 
 *
 *
 *
 * Versao : 1.2.7  para o novo Hardware 2.0.0
 *
 * Nesta versão nos conseguimos salvar na memoria Flash, os valores da Temperatura e Umidade, antes de resetar o CHIP SHT35 e assim conseguimos reaver este valor apos o reset do CHIP
 * Assim nos evitamos que o Modbus, responda algum valor estranho na rede , no momento que a maquina esta se inicializando 
 * Fizemos diversos testes e a temp e umidade nunca mais voltou a reiniciar com o valor zero, que acontecia antes.
 *
 *
 *
 * Versao : 1.3.0  para o novo Hardware 2.0.0
 * 
 * Nesta versão, fizemos uma série de modificações relacionadas à perda do enderaçamento Modbus. Notamos que, aleatoriamente, o programa sobreescrevia a parte 
 * relacionada ao baud rate, ID e calibração. Isso fazia com que o sensor parasse de funcionar. Isso era causado pela escrita dos ultimos valores dos sensores 
 * antes de um watchdog ser interrompida pelo próprio reinicio da placa, corrompendo a página da Flash. As seguintes modificações foram feitas para corrigir isso:
 * 1 - Mudamos o endereço de escrita dos ultimos valores antes de reiniciar para uma página diferente da Flash - mesmo se der erro, não perdemos os endereços Modbus
 * 2 - Aumentamos o número de medidas repetidas para reiniciar de 25 para 100 - com o filtro e a temperatura a noite estabilizando, podemos aumentar o número
 * 3 - Realimentamos o Watchdog antes de fazer a escrita - pela escrita estar no final do programa e não realimentar o Watchdog, o risco dele estourar no meio do
 * processo aumentava bastante. Realimentá-lo logo antes garante que teremos tempo mais que suficiente pra isso.
 * 4 - Criamos uma flag auxiliar para evitar que os últimos valores sejam escritos mais de uma vez. Como o programa roda em loop, mesmo  sem realimentar o watchdog,
 * nós passavamos pela função de escrita dos últimos valores umas duas ou três vezes antes de reiniciar, aumentando drásticamente as chances de um erro rolar. Agora,
 * uma vez escrito os valores, ele não fará isso mais até que o dispositivo reinicie e marque a função novamente.
 *
 *
 *
 */ 

#ifdef APP
//#include "sam.h"
#include <atmel_start.h>
#include <driver_init.h>
#include <utils.h>
#include <hal_atomic.h>
#include <hal_io.h>
#include <hal_delay.h>
#include <math.h>
#include <stdlib.h>

//#include <main.h>

#include "freeModBus/include/mb.h"
#include "freeModBus/include/mbport.h"

#include "ModBusConfig/ModBusConfig.h"

#include "eepromEmulator/eepromEmulator.h"

#include "fabrication_numbers.h"

#include "driver_init.h"
#include "utils.h"

#include "peripheral_clk_config.h"
#include "hpl_tc_config.h"

//-------------------------------// Funcao Whact Dog time  --------------------------------------------------------------------------------------------------------
void WDT_config(void)
{	
	while ( wdt_set_timeout_period(&WDT_0, 1000, 2048) != 0 ){;}			//  Wacth Dog configurado para 2 segundos. Essa linha espera o retorno da função que é zero, assim se vier zero ela sai do while
																			//	0 : Completed successfully ; -1 : Always on or enabled, can't set again ; -2 : Invalid timeout period	
								
	while ( wdt_enable(&WDT_0) != 0 ){;}									//	0 : Completed successfully. Essa linha espera o retorno da função que é zero, assim se vier zero ela sai do while
			
}


//------------------------------// O Timer 2 é configurado para gerar um loop de 1s que é a leitura das variaveis do sensor  ---------------------------------------

uint8_t take_measurement = 0;  // Flag de 1s do timer. Começa em zero

static struct timer_task TIMER_2_task1;  // TIMER_2_task2;

static void TIMER_2_task1_cb(const struct timer_task *const timer_task) 
{
	take_measurement = 1; 
}

void TIMER_2_init(void)
{
	TIMER_2_task1.interval = 1;
	TIMER_2_task1.cb       = TIMER_2_task1_cb;
	TIMER_2_task1.mode     = TIMER_TASK_ONE_SHOT;
	timer_add_task(&TIMER_2, &TIMER_2_task1);
	timer_start(&TIMER_2);
}


//-----------------------------------------// SPI nao usa seria para memoria externa
uint8_t spi_buffer[3];
struct io_descriptor *SPI_io;



//----------------------------------------// I2C utilizado para ler o Sensiriun
uint8_t i2c_buffer[6];
struct io_descriptor *I2C_io;



//---------------------------------------// Measurements  

// conversor float
typedef union {
	struct {
		uint16_t ls	:16;									// Most significant
		uint16_t ms	:16;									// Least significant
	} half;
	float f;												// Type used for register access
}SPLITFLOAT;

SPLITFLOAT temperatureA;	// variaveis de calibracao
SPLITFLOAT temperatureB;	// variaveis de calibração

SPLITFLOAT temperatureC;   // Temperatura em C
SPLITFLOAT temperatureK;   // Temp...K
SPLITFLOAT temperatureF;

SPLITFLOAT humidityA;		// variaveis de calibração 
SPLITFLOAT humidityB;		// variaveis de calibração

SPLITFLOAT humidity;       // Humidade em %


#define MOVING_AVERAGE_SIZE (uint8_t)3     // media movel para tratar o dado
uint8_t measurementNumber = 0;
uint8_t measurementNumberBeginingOffset = (MOVING_AVERAGE_SIZE-1);

double measured_temperature;
double temperatureMovingAverage [MOVING_AVERAGE_SIZE];
double adjustedTemperature;

double measured_humidity;
double humidityMovingAverage [MOVING_AVERAGE_SIZE];
double adjustedHumidity;

double measured_dewpoint;
double dewpointMovingAverage [MOVING_AVERAGE_SIZE];
double adjustedDewpoint;;

uint8_t maxPointQty = 16;


// Variaveis de Teste Temporario

uint8_t contador_erro = 0;
uint8_t Flag_teste = 0;


// Variaveis utilizadas para Registrar na Memoria a ultima temperatura e umidade antes de ser resetado.

uint8_t Flag_Rec = 0;
uint8_t Flag_Rec_Ativo = 0;


// Flag utilizada pelo sensor SHT35 para indicar uma falha
 
uint8_t Flag_Sht35 = 0;



//----------------------------------------------------------// Sensors , se eu desejo usar HTE501 eu descomento o define , ou seja tiro #, dai vou compilar com ele, mas preciso comentar o outro abaixo
//#define HTE501_UTILIZED
#ifdef HTE501_UTILIZED

#include "hte501.h"

#define MAXIMUM_TEMPERATURE (double)135
#define MINIMUM_TEMPERATURE (double)-40
#define MAXIMUM_HUMIDITY (double)100
#define MINIMUM_HUMIDITY (double)0

#define HTE501_ADDRESS (uint8_t)0x40
#define HTE501_TRESOLUTION (uint8_t)14
#define HTE501_HRESOLUTION (uint8_t)14
#define HTE501_TRH_PERIOD_MS (uint32_t)500
#define HTE501_TRH_MEASUREMENT_DURATION_US (uint32_t)9000

#define config_sensor() hte501_configure(HTE501_ADDRESS, HTE501_TRESOLUTION, HTE501_HRESOLUTION, HTE501_TRH_PERIOD_MS);
//#define read_sensor() hte501_periodicTRhRead(HTE501_ADDRESS, &measured_temperature, &measured_humidity);
//#define read_sensor() hte501_singleTRhRead(HTE501_ADDRESS, &measured_temperature, &measured_humidity);

#endif


//#define SHT4X_UTILIZED
#ifdef SHT4X_UTILIZED

#include "sht4x.h"

#define MAXIMUM_TEMPERATURE (double)125
#define MINIMUM_TEMPERATURE (double)-40
#define MAXIMUM_HUMIDITY (double)100
#define MINIMUM_HUMIDITY (double)0

#define SHT4X_ADDRESS (uint8_t)0x44
#define SHT4X_TRH_MEASUREMENT_DURATION_US (uint32_t)10000 // (uint32_t)2500

//#define config_sensor() sht4x_configure(SHT4X_ADDRESS, SHT4X_TRESOLUTION, SHT4X_HRESOLUTION, SHT4X_TRHPERIOD);
#define config_sensor() sht4x_order_measurement(SHT4X_ADDRESS);
//#define read_sensor() sht4x_singleTRhRead(SHT4X_ADDRESS, &measured_temperature, &measured_humidity);

#endif

#define SHT3X_UTILIZED
#ifdef SHT3X_UTILIZED

#include "sht3x.h"

#define MAXIMUM_TEMPERATURE (double)125
#define MINIMUM_TEMPERATURE (double)-40
#define MAXIMUM_HUMIDITY (double)100
#define MINIMUM_HUMIDITY (double)0

#define SHT3X_ADDRESS (uint8_t)0x44
#define SHT3X_TRH_MEASUREMENT_DURATION_US (uint32_t)10000 // (uint32_t)2500

//#define config_sensor() sht3x_configure(SHT3X_ADDRESS, SHT3X_TRESOLUTION, SHT3X_HRESOLUTION, SHT3X_TRHPERIOD);
#define config_sensor() sht3x_order_measurement(SHT3X_ADDRESS);
//#define read_sensor() sht3x_singleTRhRead(SHT3X_ADDRESS, &measured_temperature, &measured_humidity);

#endif

uint8_t sensorStatus = 0;
uint8_t Cont_Status_SHt35 = 0;
double Umidade_Compara = 0;
double Temperatura_Compara = 0;
uint8_t Umidade_Contador = 0;
uint8_t Temperatura_Contador = 0;

uint8_t Flag_Temp_Umd_travado = 0;

uint16_t    Contador_Modbus = 1;
uint16_t	MB_ENOERR_counter = 0;
uint16_t    MB_ENOREG_counter = 0;
uint16_t    MB_EINVAL_counter = 0;
uint16_t    MB_EPORTERR_counter = 0;
uint16_t    MB_ENORES_counter = 0;
uint16_t    MB_EIO_counter = 0;
uint16_t    MB_EILLSTATE_counter = 0;
uint16_t    MB_ETIMEDOUT_counter = 0;

uint16_t	Contador_loop_int = 0;    								
uint16_t    Contador_Watchd_int = 0;


eMBErrorCode mb_status;

#define APP_VTOR 0x00002000u

static inline void vectors_relocate_to_app(void) {
	SCB->VTOR = APP_VTOR;
	__DSB(); __ISB();
}


//----------------------------------------------------------// int main(void)
int main(void)
{
		
	//----------------------------------------------------------// Initializes MCU, drivers and middleware
	atmel_start_init();
	
	vectors_relocate_to_app();
	
	//----------------------------------------------------------// WDT
	WDT_config();
	
	//----------------------------------------------------------// Configuring I2C bus
	i2c_m_sync_enable(&I2C_0);
	i2c_m_sync_get_io_descriptor(&I2C_0, &I2C_io);
	
	io_write(I2C_io, 0x0006, 2);  // Esta função reseta o I2C no chip sensiriun toda vez que passa por aqui quando acontece o WatchDog
	io_write(I2C_io, 0x30A2, 2);  // Reseta o CHIP 
	
	wdt_feed(&WDT_0);
	
	//-------------------  Aqui resetamos o CHIP SHT35 fisicamente, um pulso, de alto para baixo e volta para alto ---------------
	
	gpio_set_pin_level(ResetSensorPin, FALSE);
	delay_ms(10);
	gpio_set_pin_level(ResetSensorPin, TRUE);

	
	//----------------------------------------------------------// sensor configuration.
	config_sensor();
	
	wdt_feed(&WDT_0);
	
	//----------------------------------------------------------// Inicializa flash Interna
	FLASH_0_init();
	
	wdt_feed(&WDT_0);
	
	//----------------------------------------------------------// Configura ModBus
	config_MB();
	
	wdt_feed(&WDT_0);

	//----------------------------------------------------------// Calibration , aqui recebemos do Modbus os valores de calibração U16 e depois transformamos em float
	temperatureA.half.ms = usRegHoldingBuf[5];	//	Temperature A	:	Float 2/2 ?C
	temperatureA.half.ls = usRegHoldingBuf[6];	//	Temperature A	:	Float 1/2 ?C
	
	temperatureB.half.ms = usRegHoldingBuf[7];	//	Temperature B	:	Float 2/2 ?C
	temperatureB.half.ls = usRegHoldingBuf[8];	//	Temperature B	:	Float 1/2 ?C
	
	humidityA.half.ms = usRegHoldingBuf[9];		//	Humidity A		:	Float 2/2 %
	humidityA.half.ls = usRegHoldingBuf[10];	//	Humidity A		:	Float 1/2 %
	
	humidityB.half.ms = usRegHoldingBuf[11];	//	Humidity A		:	Float 2/2 %
	humidityB.half.ls = usRegHoldingBuf[12];	//	Humidity A		:	Float 1/2 %
	
		
	wdt_feed(&WDT_0);

	//----------------------------------------------------------// 1 second timer , iniciando o timer
	TIMER_2_init();
	
	wdt_feed(&WDT_0);
	
	// Configurando sensor Sensiriun para modo continuo ( 2Hz )
	sht3x_order_measurement(SHT3X_ADDRESS);
	
	// Forcamos aqui uma leitura ja do SHT35 para termos dados coerente lidos
	
	#ifdef SHT4X_UTILIZED
	//sht4x_singleTRhRead(SHT4X_ADDRESS, &measured_temperature, &measured_humidity);
	sensorStatus = sht4x_read_measurement(SHT4X_ADDRESS, &measured_temperature, &measured_humidity);
	sht4x_order_measurement(SHT4X_ADDRESS);
	#endif
	
	#ifdef SHT4X_UTILIZED
	//sht4x_singleTRhRead(SHT4X_ADDRESS, &measured_temperature, &measured_humidity);
	sensorStatus = sht4x_read_measurement(SHT4X_ADDRESS, &measured_temperature, &measured_humidity);
	sht4x_order_measurement(SHT4X_ADDRESS);
	#endif
		
	
	// Le os dados salvo na memoria do processador, para carregar os mesmos no Modbus, antes de iniciar a resposta as perguntas que o sensor ira receber. 
		
	FLASH_0_read_mbConfig();
	FLASH_0_read_mbConfig_RecoveryData();
	
	usRegInputBuf[32] = ( ((uint16_t)mbConfig2[56]<<8) | (uint16_t)mbConfig2[57] );	//	Temperature Float 1/2 
	usRegInputBuf[33] = ( ((uint16_t)mbConfig2[58]<<8) | (uint16_t)mbConfig2[59] );	//	Temperature Float 2/2 
	
	usRegInputBuf[36] = ( ((uint16_t)mbConfig2[60]<<8) | (uint16_t)mbConfig2[61] );	//	Humidade	Float 1/2 
	usRegInputBuf[37] = ( ((uint16_t)mbConfig2[62]<<8) | (uint16_t)mbConfig2[63] );	//	Humidade	Float 2/2 
	
			
		//usRegInputBuf[32] =	temperatureC.half.ms;					// Celsius		 C
		//usRegInputBuf[33] =	temperatureC.half.ls;					// Celsius		 C
		
		//usRegInputBuf[36] =	humidity.half.ms;						// humidity		 %
		//usRegInputBuf[37] =	humidity.half.ls;
	
			
	
	//----------------------------------------------------------// while (1)
    while (1)
    {
				
		
		//----------------------------------------------------------// ModBus
		
		mb_status = eMBPoll();     // aqui entramos no Modbus para verificar se tem alguma mensagem
		
		
		switch(mb_status)
		{
			case MB_ENOERR:  /*!< no error. */
			MB_ENOERR_counter += 1;
			break;
			case MB_ENOREG: /*!< illegal register address. */
			MB_ENOREG_counter += 1;
			break;
			case MB_EINVAL: /*!< illegal argument. */
			MB_EINVAL_counter += 1;
			break;
			case MB_EPORTERR: /*!< porting layer error. */
			MB_EPORTERR_counter += 1;
			break;
			case MB_ENORES: /*!< insufficient resources. */
			MB_ENORES_counter += 1;
			break;
			case MB_EIO: /*!< I/O error. */
			MB_EIO_counter += 1;
			break;
			case MB_EILLSTATE: /*!< protocol stack in illegal state. */
			MB_EILLSTATE_counter += 1;
			break;
			
			case MB_ETIMEDOUT:
			MB_ETIMEDOUT_counter += 1;
			break;
		}
		
					
				
				
		//----------------------------------------------------------// Measurement
		if ( take_measurement >= 1 ) 
		{			
									
			Contador_Modbus =  MB_ETIMEDOUT_counter
			+ MB_EILLSTATE_counter
			+ MB_EIO_counter
			+ MB_ENORES_counter
			+ MB_EPORTERR_counter
			+ MB_EINVAL_counter
			+ MB_ENOREG_counter;		
			
			
			#ifdef HTE501_UTILIZED
				sensorStatus = hte501_periodicTRhRead(HTE501_ADDRESS, &measured_temperature, &measured_humidity);
			#endif
			
			#ifdef SHT4X_UTILIZED
				//sht4x_singleTRhRead(SHT4X_ADDRESS, &measured_temperature, &measured_humidity);
				sensorStatus = sht4x_read_measurement(SHT4X_ADDRESS, &measured_temperature, &measured_humidity);
				sht4x_order_measurement(SHT4X_ADDRESS);
			#endif
			
			#ifdef SHT3X_UTILIZED
								
				sensorStatus = sht3x_read_measurement(SHT3X_ADDRESS, &measured_temperature, &measured_humidity);
				//sht3x_order_measurement(SHT3X_ADDRESS);  -- comentamos por que agora não precisa mais escrever para ler o sensiriun, basta somente ler pois ele esta no modo loop automatico.
			#endif
									
			
			// Analisa se o Status do Sensor SHT35 esta em um estado de erro, ou seja não é igual a zero. Se for diferente contamos quantas vezes esse erro aconteceu continumanete.
			// Se este erro aconteceu mais que 5 vezes, nos nao resetamos o Watch Dog e ele vai exploidir e resetar tudo inclusive o sensor SHT35.
			
			if (sensorStatus == 0)
			{
				Cont_Status_SHt35 = 0;
			} else
			{
				Cont_Status_SHt35 = Cont_Status_SHt35 + 1;				
			}
			
			if ( Cont_Status_SHt35 > 5)
			{
				Flag_Sht35 = 1;
				Flag_Rec = 1;
			}
			
									
			
			// Analisa se o valor da Umidade esta se repetindo.			
			if (measured_humidity == Umidade_Compara) 
			{ 
				Umidade_Contador = Umidade_Contador + 1;
				
			}
			else 
			{
				Umidade_Contador = 0; 
				Umidade_Compara = measured_humidity;
			}
				
				
			//measured_temperature = 25; // Adicionado para testar o valor travado
			// Analisa se o valor da Temperatura esta se repetindo.
			if (measured_temperature == Temperatura_Compara)
			{
				Temperatura_Contador = Temperatura_Contador + 1;
				
			}
			else
			{
				Temperatura_Contador = 0;
				Temperatura_Compara = measured_temperature;
			}
				
								
			
			if (measurementNumber >= MOVING_AVERAGE_SIZE)
			{
				measurementNumber = 0;
			}
			
			
			// Media movel da Temperature
			temperatureMovingAverage[measurementNumber] = measured_temperature;
			measured_temperature = 0;
			for (uint8_t i = 0; i<(MOVING_AVERAGE_SIZE-measurementNumberBeginingOffset); i++)
			{
				measured_temperature += temperatureMovingAverage[i];
			}
			measured_temperature = ( measured_temperature/((double)(MOVING_AVERAGE_SIZE - measurementNumberBeginingOffset)) );
			
			
			// Media movel da Humidity
			humidityMovingAverage[measurementNumber] = measured_humidity;
			measured_humidity = 0;
			for (uint8_t i = 0; i<(MOVING_AVERAGE_SIZE-measurementNumberBeginingOffset); i++)
			{
				measured_humidity += humidityMovingAverage[i];
			}
			measured_humidity = ( measured_humidity/((double)(MOVING_AVERAGE_SIZE - measurementNumberBeginingOffset)) );
			
			
			measurementNumber++;
			
			
			
			if (measurementNumberBeginingOffset>0)
			{
				measurementNumberBeginingOffset--;
			}
			
						
			// Ajuste das Variaveis de Calibração da Temperature ( B ganho float ) e ( A offset float )	
			
			temperatureC.f = ( ( ((float)measured_temperature)*temperatureB.f ) + temperatureA.f );   //  equação ... Temperatura Ajustada  = Temperatura * Ganho ( B )  - Offset ( A ) 

			if (temperatureC.f < MINIMUM_TEMPERATURE)
			{
				temperatureC.f = MINIMUM_TEMPERATURE;
			}
			else if (temperatureC.f > MAXIMUM_TEMPERATURE)
			{
				temperatureC.f = MAXIMUM_TEMPERATURE;
			};
			
								
			
			// Ajuste das Variaveis de Calibração da Umidade ( B ganho float ) e ( A offset float )	
			
			humidity.f = ( ( ((float)measured_humidity)*humidityB.f ) + humidityA.f ); //  equação ... Umidade Ajustada  = Umidade * Ganho ( B )  + Offset ( A ) 

			if (humidity.f < MINIMUM_HUMIDITY)
			{
				humidity.f = MINIMUM_HUMIDITY;
			}
			else if (humidity.f > MAXIMUM_HUMIDITY)
			{
				humidity.f = MAXIMUM_HUMIDITY;
			};
			
			
			// --- Aqui calcula a temperatura em Kelvin e Farenhigt
			
			temperatureK.f = temperatureC.f + 273.15;
			temperatureF.f = ( (temperatureC.f * 1.8) + 32.0);
									
			
			// --- Se o sensor esta OK, os valores no endereço Modbus RTU podem ser atualizados, caso contrario se mantem o valor que ja estava la, antes do sensor apresentar um erro.
			
			if ( sensorStatus == 0)
			
			{
				
				// Passando os Valores para a tabela Modbus
				// Abaixo valores em Float
				usRegInputBuf[30] =	temperatureK.half.ms;					// Kelvin		 K
				usRegInputBuf[31] =	temperatureK.half.ls;					// Kelvin		 K
				usRegInputBuf[32] =	temperatureC.half.ms;					// Celsius		 C
				usRegInputBuf[33] =	temperatureC.half.ls;					// Celsius		 C
				usRegInputBuf[34] =	temperatureF.half.ms;					// Fahrenheit	 F
				usRegInputBuf[35] =	temperatureF.half.ls;					// Fahrenheit	 F
				usRegInputBuf[36] =	humidity.half.ms;						// humidity		 %
				usRegInputBuf[37] =	humidity.half.ls;						// humidity		 %
				
				//Abaixo valores em Inteiros
				usRegInputBuf[38] = temperatureK.f * 50.0;					// Kelvin		 K
				usRegInputBuf[39] = temperatureC.f * 100.0;					// Celsius		 C
				usRegInputBuf[40] = temperatureF.f * 50.0;					// Fahrenheit    F
				usRegInputBuf[41] = humidity.f * 100.0;						// humidity		 %
				
				//Abaixo valores
				usRegInputBuf[42] = (int16_t)(temperatureK.f);											// Kelvin		 K
				usRegInputBuf[43] = (uint16_t)((temperatureK.f - (int16_t)(temperatureK.f)) * 1000);	// Kelvin		 K
				usRegInputBuf[44] = (int16_t)(temperatureC.f);											// Celsius		?C
				usRegInputBuf[45] = ((temperatureC.f - (int16_t)(temperatureC.f)) * 1000);	            // Celsius		?C
				usRegInputBuf[46] = (int16_t)(temperatureF.f);											// Fahrenheit	?F
				usRegInputBuf[47] = ((temperatureF.f - (int16_t)(temperatureF.f)) * 1000);	            // Fahrenheit	?F
				usRegInputBuf[48] = (int16_t)(humidity.f);                                              // humidity		%
				usRegInputBuf[49] = (uint16_t)((humidity.f - (int16_t)(humidity.f)) * 1000);			// humidity		%
				
				Contador_loop_int = Contador_loop_int + 1;  // ( Temporario )
												
				
			}
			
			
			//------- Aqui escrevemos o Status do Sensor no Modbus, isso ocorre mesmo que  a leitura do Status do sensor esteja ruim --------------
			
			usRegInputBuf[50] = Contador_loop_int;  // ( Temporario )
			usRegInputBuf[51] = Contador_Watchd_int;
			usRegInputBuf[52] = Temperatura_Contador;
			usRegInputBuf[53] = Umidade_Contador;
			usRegInputBuf[54] = Cont_Status_SHt35;
			
						
			
			
			// ------ Se a temperatura ou a umidade se repetiu mais do que 25 X , uma flag é setada, para resetar o Watch Dog do processador.															 
				
			if ( (Temperatura_Contador >= 100) || (Umidade_Contador >= 100 ) ) // Aumentamos de 25 para 100 porque, a noite, 25 é muito fácil de ocorrer
			{
				Flag_Temp_Umd_travado = 1;
				Flag_Rec = 1;
			}	
									
			
			// ------ COTADOR SIMULADOR DE ERRO ( TEMPORARIO ) , VAMOS TESTAR SE AO  RESETAR O MODBUS CARREGA UM VALOR DE TEMP. VALIDO.
			
			//if ( contador_erro > 10)
			//{
			//	Flag_teste = 1;
			//	Flag_Rec = 1;
			//}	
			//else 
			//{
			//	contador_erro = contador_erro + 1;
			//	Flag_teste = 0;
			//}
						
			
			
			if ( Flag_Rec == 1 && Flag_Rec_Ativo == 0)   // Vamos salvar na memoria os dados da Temperatura e Umidade antes de resetar para resgatar os mesmos apos o reset
			{
								
				//usRegInputBuf[32] =	temperatureC.half.ms;					// Celsius		 C
				//usRegInputBuf[33] =	temperatureC.half.ls;					
				
				//usRegInputBuf[36] =	humidity.half.ms;						// humidity		 %
				//usRegInputBuf[37] =	humidity.half.ls;
				
				wdt_feed(&WDT_0); // Alimentamos o WD para evitar que ele ative enquanto a escrita na flash rola
				
				mbConfig2[56] = (uint8_t)(usRegInputBuf[32] >> 8);		// Byte 0 of  (float split into 4 bytes)
				mbConfig2[57] = (uint8_t)(usRegInputBuf[32] & 0xFF);	    // Byte 1 of  (float split into 4 bytes)
				mbConfig2[58] = (uint8_t)(usRegInputBuf[33] >> 8);		// Byte 2 of  (float split into 4 bytes)
				mbConfig2[59] = (uint8_t)(usRegInputBuf[33] & 0xFF);	    // Byte 3 of  (float split into 4 bytes)
				
				mbConfig2[60] = (uint8_t)(usRegInputBuf[36] >> 8);		// Byte 0 of  (float split into 4 bytes)
				mbConfig2[61] = (uint8_t)(usRegInputBuf[36] & 0xFF);	    // Byte 1 of  (float split into 4 bytes)
				mbConfig2[62] = (uint8_t)(usRegInputBuf[37] >> 8);		// Byte 2 of  (float split into 4 bytes)
				mbConfig2[63] = (uint8_t)(usRegInputBuf[37] & 0xFF);	    // Byte 3 of  (float split into 4 bytes)
								
				FLASH_0_write_mbConfig_RecoveryData();
				Flag_Rec_Ativo = 1; // Colocamos essa flag para essa própria função não ficar realimentando o watchdog
			}
			
			
			// ------ Se a Flag de Temperatura ou umidade estiver acionada, ou o Contador de Erro do SHT35 for > 5 do sensor estiver acionada, o watchdog ira explodir ou seja ira resetar o processador.
			
			if( ( Flag_Sht35 == 0 ) & ( Flag_Temp_Umd_travado == 0 ) )      // & ( Flag_teste == 0)
			{
				wdt_feed(&WDT_0);											// Zeramos o Watch Dog de 2 segundos, assim como esse loop é de 1s, ele nunca vai estourar.
				Contador_Watchd_int = Contador_Watchd_int + 1;
			}
									
			
			// ---- Abaixo um teste de saida pisca pisca na porta A numero 27 para olhar no osciloscopio  ----- 
			 
			//gpio_set_pin_level(MB_CFG, TRUE);
			//delay_ms(20);
			//gpio_set_pin_level(MB_CFG, FALSE);
			//delay_ms(20);
			
			
			take_measurement = 0;
			timer_add_task(&TIMER_2, &TIMER_2_task1);
				
		}	
		
				
    }//	while 
	
}// int main(void)

#endif

#ifdef BOOTLOADER


main()
{
	
}
#endif
