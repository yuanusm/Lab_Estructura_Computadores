/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "utils.h"
#include "display.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum _EstadoDificultad
{
	dificil = 2,
	medio = 10,
	facil = 20
} _EstadoDificultad;
_EstadoDificultad Dificultad_Establecida = medio;

typedef enum _EstadoJuego
{
	Bienvenida = 0,
	Partida = 1,
	Termino = 2
} _EstadoJuego;
_EstadoJuego EstadoJuego = Bienvenida;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NPremios 10
#define NObstaculos 5
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
const char clear_chars[]={0,
		0,
		0,
		0,
		0,
		0,
		0,
		0};
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RNG_HandleTypeDef hrng;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */


char buffer[10];
char comando_recibido[10];
char comando;

uint8_t flag_conteo;
uint8_t flag_numero_aletorio_listo;
uint8_t flag_refresco_mundo;
uint8_t flag_timer_100;
uint8_t flag_timer_data;
uint8_t flag_pantalla = 1;
uint8_t indice = 0;
uint16_t indice_conteo = 0;
uint32_t randomNumber;


uint8_t numero_premios = 0;
uint8_t numero_obstaculos = 0;
uint8_t pos_obstaculos[] = {0,0,0,0,0};


int8_t PersonajeArribaAbajo = 0; //moviemiento en eje Y, el cero es plano positivo es hacia arriba
int8_t PersonajeIzquierdaDerecha = 0; //moviemiento en eje X, el cero es plano positivo hacia derecha

float valorz = 0.0f;
float valory = 0.0f;
float valorx = 0.0f;

uint8_t posicionPersonajeX = 0;
uint8_t posicionPersonajeY = 0;
uint8_t posicionPersonajeY_pasada = 0;
uint8_t posicionPersonajeX_pasada = 0;
uint8_t posicionMundo = 0;
uint8_t posicionMundo_pasada = 0;
uint8_t PremiosConseguidos = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM6_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM7_Init(void);
static void MX_RNG_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*Esto es para redireccionar la funcion de printf a la consola SWV durante la sesion debug*/
//int _write(int file, char *ptr, int len)
//{
//	/* Implement your write code here, this is used by puts and printf for example */
//	int i=0;
//	for(i=0 ; i<len ; i++)
//		ITM_SendChar((*ptr++));
//	return len;
//}

int _write(int fd, char* ptr, int len)
{
	for(int i = 0; i < len ; i++)
	{
		display_putchar(ptr[i]);
	}
	return len;
}


void limpiar_buffer_comando(){ // el buffer de comando recibido se llena con los caracteres de lo ultimo mandado
	for (int i = 0; i < sizeof(comando_recibido); i++) {
		comando_recibido[i] = '\0';
	}
}



void Crear_Mundo(void){


	//Llenar con premios

	for(int i = 1; i < 0x28; i++ ){
		HAL_RNG_GenerateRandomNumber(&hrng, &randomNumber);
		randomNumber=(randomNumber%100)+1;
		//Genera numero random de 32bits segun un voltaje analogico medido
		//Toma el resto de la division por 100 y le suma uno  (32bits---> 0-99 ---> 1-100)
		//Los condicionales usan el numero randomNumber como si fuera una probabilidad




		// La estructura de los condicionales está puesta para que se generen premios en la misma
		// columna pero no obstaculos en la misma columna

		if( (numero_premios!=NPremios) && randomNumber <= 13){
			//13% de probabilidad que se genere un premio en la primera fila
			display_set_pos(0x00+i);
			display_send_data(80u);
			numero_premios+=1;
		}else if( (numero_obstaculos != NObstaculos) && (30 < randomNumber && randomNumber <= 37) && (pos_obstaculos[numero_obstaculos-1]<i-1)){
			//7% de probabilidad que se genere un obstaculo en la primera fila
			//asegura que no haya obstaculos consecutivos que limiten el paso
			display_set_pos(0x00+i);
			display_send_data(79u);
			pos_obstaculos[numero_obstaculos]=i;
			numero_obstaculos+=1;
		}else if( (numero_premios!=NPremios) && (randomNumber > 15 && randomNumber <= 28)  ){
			//13% de probabilidad que se genere un premio en la segunda fila
			display_set_pos(0x40+i);
			display_send_data(80u);
			numero_premios+=1;
		}else if ((numero_obstaculos != NObstaculos) && ( 37 < randomNumber && randomNumber <= 44) && (pos_obstaculos[numero_obstaculos-1]<i-1)){
			//7% de probabilidad que se genere un obstaculo en la segunda fila
			//asegura que no haya obstaculos consecutivos que limiten el paso
			display_set_pos(0x40+i);
			display_send_data(79u);
			pos_obstaculos[numero_obstaculos]=i;
			numero_obstaculos+=1;
		}
	}
	if((numero_premios!=NPremios || numero_obstaculos != NObstaculos)){
		// Si no se cumple la condicion de premios y obstaculos
		// se vuelve a tirar los "dados" hasta que se cumpla
		for(int i = 0; i < 0x28; i++ ){
			display_clear(); //limpia la memoria
			numero_premios = 0;
			numero_obstaculos = 0;
		}
		Crear_Mundo(); // vuelve a generar mundo
	}
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

	/* USER CODE BEGIN 1 */
	IKS01A2_MOTION_SENSOR_Axes_t lectura_acelerometro_1, lectura_acelerometro_2;

	//IKS01A2_MOTION_SENSOR_Axes_t buffer_conteo[1000];

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	IKS01A2_init();	/*Inicializar los sensores*/

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_TIM6_Init();
	MX_USART2_UART_Init();
	MX_TIM7_Init();
	MX_RNG_Init();
	/* USER CODE BEGIN 2 */


	HAL_TIM_Base_Start_IT(&htim6);
	HAL_UART_Receive_IT(&huart2, (uint8_t*)&comando, 1);


	char mensaje[100];

	//BSP_I2C1_ReadReg(0x1E<<1, 0x4F, (uint8_t*)&who_i, 1);

	uint8_t contador_segundos = 0;
	uint8_t Vidas = 3;
	char respuesta ;
	uint8_t contador_terceros = 0;
	flag_refresco_mundo = 0;
	flag_timer_100 = 0;
	flag_conteo = 0;

	setvbuf(stdout, (char*)NULL, _IONBF, 0); //pareciera que no sirve esta linea dado el poco entendimiento que tenemos acerca de ella pero sin esto el codigo no funciona para la escritura de caracteres
	uint8_t GAMEOVER =0;
	display_init();	//inicia comandos de inicializacion del  display
	display_send_cmd(0x0C); //esconde el cursor del display
	display_set_pos(0x00); // mueve el cursor a la posicion 00

	display_send_cmd(0b1000); // apagar cursor, blink y pantalla
	display_test_write_cgram_ms(); //Escribe en memoria el simbolo de player
	display_clear();	//limpia la pantalla
	display_send_cmd(0b1100);//encender display sin cursor ni blink

	Dificultad_Establecida = medio;

	char lec;
	display_set_pos(0x00);
	display_send_data(u);
	display_set_pos(0x00);
	lec = display_get_data();

	uint8_t contadorx = 0;
	uint8_t PersonajeIzquierdaDerecha_pasada = 0;

	printf("    Juego de:\nJLEEM VOG TFBP");
	HAL_Delay(3000);
	display_clear();
	display_send_cmd(0b1000); // apaga display cuando escribe el mundo
	Crear_Mundo();
	display_send_cmd(0b1100); // vuelve a encender el display

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */

		if(GAMEOVER){
			display_clear();
			printf("Score:%d\n\rGAME OVER",PremiosConseguidos);
		}


		if(flag_timer_data){
			if(comando=='\n' || comando=='\r' ||indice == 10){

				for(uint8_t i = 0; i<10 ;i++){
					comando_recibido[i] = buffer[i];
					buffer[i] = 0;
				}

				indice = 0;

			}
			flag_timer_data = 0;
		}


		if(flag_timer_100){


			++contador_segundos;



			if(contador_terceros++>=600){
				GAMEOVER=1;
				contador_terceros=0;
			}




			lectura_acelerometro_1 = Accelero_Sensor_Handler(IKS01A2_LSM6DSL_0);
			lectura_acelerometro_2 = Accelero_Sensor_Handler(IKS01A2_LSM303AGR_ACC_0);
			valorz = ((float)lectura_acelerometro_1.z+(float)lectura_acelerometro_2.z)/200.0f;
			valory = ((float)lectura_acelerometro_1.y-(float)lectura_acelerometro_2.y)/200.0f;
			valorx = (-(float)lectura_acelerometro_1.x-(float)lectura_acelerometro_2.x)/200.0f;

			if(valory>=3.0f){ // arriba es Y positivo
				PersonajeArribaAbajo = 1;
			}else
				if(valory<1.0f && valory>-1.0f){
					PersonajeArribaAbajo = 0;
				}else
					if(valory<=-3.0f){
						PersonajeArribaAbajo = -1;
					}

			if(valorx>=3.0f){ // izquierda es X positivo
				PersonajeIzquierdaDerecha = -1;
			}else
				if(valorx<1.0f && valorx>-1.0f){
					PersonajeIzquierdaDerecha = 0;
				}else
					if(valorx<=-3.0f){
						PersonajeIzquierdaDerecha = 1;
					}




			if(contador_segundos==Dificultad_Establecida){
				flag_refresco_mundo = 1;
				contador_segundos = 0;
			}
			if(contador_terceros == 30 ){
				//flag_pantalla = 0;
				contador_terceros = 0;
			}
			flag_timer_100 = 0;


			if(!strcmp(comando_recibido,"S\n")){ // Comando S de Start game

				sprintf(mensaje, "Comando: %c \n\r",comando_recibido[0]==0?1:comando_recibido[0]);
				HAL_UART_Transmit(&huart2,(uint8_t*)mensaje,strlen(mensaje),HAL_MAX_DELAY);

				//flag_conteo = 1; //Flags de iniciar calibracion
				//indice_conteo = 0;

				limpiar_buffer_comando();


			}else if(!strcmp(comando_recibido,"R\n")){ // Comando R de reiniciar

				sprintf(mensaje, "Comando: %c \n\r",comando_recibido[0]==0?1:comando_recibido[0]);
				HAL_UART_Transmit(&huart2,(uint8_t*)mensaje,strlen(mensaje),HAL_MAX_DELAY);

				HAL_NVIC_SystemReset();

				limpiar_buffer_comando();

			}else if(!strcmp(comando_recibido,"E\n")){ // Comando S de Start game

				sprintf(mensaje, "Comando: %c \n\r",comando_recibido[0]==0?1:comando_recibido[0]);
				HAL_UART_Transmit(&huart2,(uint8_t*)mensaje,strlen(mensaje),HAL_MAX_DELAY);

				Dificultad_Establecida = facil;
				contador_segundos = 0;
				limpiar_buffer_comando();


			}else if(!strcmp(comando_recibido,"M\n")){ // Comando S de Start game

				sprintf(mensaje, "Comando: %c \n\r",comando_recibido[0]==0?1:comando_recibido[0]);
				HAL_UART_Transmit(&huart2,(uint8_t*)mensaje,strlen(mensaje),HAL_MAX_DELAY);

				Dificultad_Establecida = medio;
				contador_segundos = 0;
				limpiar_buffer_comando();


			}else if(!strcmp(comando_recibido,"H\n")){ // Comando S de Start game

				sprintf(mensaje, "Comando: %c \n\r",comando_recibido[0]==0?1:comando_recibido[0]);
				HAL_UART_Transmit(&huart2,(uint8_t*)mensaje,strlen(mensaje),HAL_MAX_DELAY);

				Dificultad_Establecida = dificil;
				contador_segundos = 0;
				limpiar_buffer_comando();


			} else if(strcmp(comando_recibido,"\0\0\0\0") ){

				sprintf(mensaje, "Mensaje Desconocido \n\r%c",0);
				HAL_UART_Transmit(&huart2,(uint8_t*)mensaje,strlen(mensaje),HAL_MAX_DELAY);

				sprintf(mensaje, "Comando: %c \n\r",comando_recibido[0]==0?1:comando_recibido[0]);
				HAL_UART_Transmit(&huart2,(uint8_t*)mensaje,strlen(mensaje),HAL_MAX_DELAY);

				limpiar_buffer_comando();
			}




			//REFRESCO PERSONAJE  BEGIN

			posicionPersonajeY=PersonajeArribaAbajo==1? 1 : PersonajeArribaAbajo==-1 ? 0 : posicionPersonajeY;


			//			if((PersonajeIzquierdaDerecha == 0) || (PersonajeIzquierdaDerecha!=PersonajeIzquierdaDerecha_pasada)){
			//				contadorx = 0;
			//			}else if (PersonajeIzquierdaDerecha==PersonajeIzquierdaDerecha_pasada){
			//				contadorx +=1;
			//				contadorx= contadorx%6;
			//			}

			if(contadorx++ >= 3){
				posicionPersonajeX += PersonajeIzquierdaDerecha;
				posicionPersonajeX =  (posicionPersonajeX%16) ;
				contadorx=0;
			}

			PersonajeIzquierdaDerecha_pasada = PersonajeIzquierdaDerecha;


			if(posicionPersonajeY){
				display_set_pos(0x00 + posicionPersonajeX_pasada );
				display_send_data(' ');//d32
				if(posicionPersonajeY != posicionPersonajeY_pasada){
					display_set_pos(0x40 + (posicionPersonajeX+posicionMundo_pasada)%40);
					display_send_data(' ');//d32
				}
				display_set_pos(0x00 + (posicionPersonajeX+posicionMundo)%40);
				respuesta = display_get_data();
				PremiosConseguidos+= (respuesta=='P');
				Vidas-= (respuesta=='O');
				display_set_pos(0x00 + (posicionPersonajeX+posicionMundo)%40);
				display_send_data(0);

			}else{
				display_set_pos(0x40 + posicionPersonajeX_pasada);
				display_send_data(' ');//d32
				if(posicionPersonajeY != posicionPersonajeY_pasada){
					display_set_pos(0x00 + (posicionPersonajeX+posicionMundo_pasada)%40);
					display_send_data(' ');//d32
				}
				display_set_pos(0x40 + (posicionPersonajeX+posicionMundo)%40);
				respuesta = display_get_data();
				PremiosConseguidos+= (respuesta=='P');
				Vidas-= (respuesta=='O');
				display_set_pos(0x40 + (posicionPersonajeX+posicionMundo)%40);
				display_send_data(0);
			}
			posicionMundo_pasada = posicionMundo;
			posicionPersonajeY_pasada = posicionPersonajeY;
			posicionPersonajeX_pasada =(posicionPersonajeX+posicionMundo)%40;
			//REFRESCO PERSONAJE  END




		}


		if(flag_refresco_mundo){
			//display_send_cmd(0b01100); //mostrar pantalla

			display_send_cmd(0b11000); //moverse a la izquierda
			posicionMundo += 1;
			posicionMundo=posicionMundo%40;

			flag_refresco_mundo = 0;

			//			posicionPersonajeY=PersonajeArribaAbajo==1? 1 : PersonajeArribaAbajo==-1 ? 0 : posicionPersonajeY;
			//
			//
			//						//			if((PersonajeIzquierdaDerecha == 0) || (PersonajeIzquierdaDerecha!=PersonajeIzquierdaDerecha_pasada)){
			//						//				contadorx = 0;
			//						//			}else if (PersonajeIzquierdaDerecha==PersonajeIzquierdaDerecha_pasada){
			//						//				contadorx +=1;
			//						//				contadorx= contadorx%6;
			//						//			}
			//
			//			posicionPersonajeX += PersonajeIzquierdaDerecha;
			//			posicionPersonajeX =  (posicionPersonajeX%16) ;
			//						PersonajeIzquierdaDerecha_pasada = PersonajeIzquierdaDerecha;
			//		}


			if(posicionPersonajeY){
				display_set_pos(0x00 + posicionPersonajeX_pasada );
				display_send_data(' ');//d32
				if(posicionPersonajeY != posicionPersonajeY_pasada){
					display_set_pos(0x40 + (posicionPersonajeX+posicionMundo_pasada)%40);
					display_send_data(' ');//d32
				}
				display_set_pos(0x00 + (posicionPersonajeX+posicionMundo)%40);
				respuesta = display_get_data();
				PremiosConseguidos+= respuesta=='P';
				Vidas-= respuesta=='O';
				display_set_pos(0x00 + (posicionPersonajeX+posicionMundo)%40);
				display_send_data(0);

			}else{
				display_set_pos(0x40 + posicionPersonajeX_pasada);
				display_send_data(' ');//d32
				if(posicionPersonajeY != posicionPersonajeY_pasada){
					display_set_pos(0x00 + (posicionPersonajeX+posicionMundo_pasada)%40);
					display_send_data(' ');//d32
				}
				display_set_pos(0x40 + (posicionPersonajeX+posicionMundo)%40);
				respuesta = display_get_data();
				PremiosConseguidos+= respuesta=='P';
				Vidas-= respuesta=='O';
				display_set_pos(0x40 + (posicionPersonajeX+posicionMundo)%40);
				display_send_data(0);
			}
			posicionMundo_pasada = posicionMundo;
			posicionPersonajeY_pasada = posicionPersonajeY;
			posicionPersonajeX_pasada =(posicionPersonajeX+posicionMundo)%40;
			//REFRESCO PERSONAJE  END


			sprintf(mensaje, "De_Iz: %d\n\rAr_Ab: %d\n\r",
					PersonajeIzquierdaDerecha,
					PersonajeArribaAbajo);

			HAL_UART_Transmit(&huart2,(uint8_t*)mensaje,strlen(mensaje),HAL_MAX_DELAY);



			//display_clear();
			//display_set_pos(0x00);
			//printf( "MagField mx:%1.0f\nmy:%1.0f mz:%1.0f", (float)local_mag.x-calibracionx,(float)local_mag.y-calibraciony,(float)local_mag.z-calibracionz);
			//float valorz = ((float)lectura_acelerometro_1.z+(float)lectura_acelerometro_2.z)/200.0f;
			//float valory = ((float)lectura_acelerometro_1.y-(float)lectura_acelerometro_2.y)/200.0f;
			//float valorx = (-(float)lectura_acelerometro_1.x-(float)lectura_acelerometro_2.x)/200.0f;
			/*sprintf(mensaje, "x:%.2f\t%.2f\n\ry:%.2f\t%.2f\n\rz:%.2f\t%.2f\n\r",
					(float)lectura_acelerometro_1.x/100.0f,
					(float)lectura_acelerometro_2.x/100.0f,
					(float)lectura_acelerometro_1.y/100.0f,
					(float)lectura_acelerometro_2.y/100.0f,
					(float)lectura_acelerometro_1.z/100.0f,
					(float)lectura_acelerometro_2.z/100.0f);*/

			/*sprintf(mensaje, "x:%.2f\n\ry:%.2f\n\r",
					valorx,
					valory);
			HAL_UART_Transmit(&huart2,(uint8_t*)mensaje,strlen(mensaje),HAL_MAX_DELAY);
			 */


		}
		if(Vidas==0){
			GAMEOVER=1;
		}
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 3;
	RCC_OscInitStruct.PLL.PLLN = 24;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief RNG Initialization Function
 * @param None
 * @retval None
 */
static void MX_RNG_Init(void)
{

	/* USER CODE BEGIN RNG_Init 0 */

	/* USER CODE END RNG_Init 0 */

	/* USER CODE BEGIN RNG_Init 1 */

	/* USER CODE END RNG_Init 1 */
	hrng.Instance = RNG;
	if (HAL_RNG_Init(&hrng) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN RNG_Init 2 */

	/* USER CODE END RNG_Init 2 */

}

/**
 * @brief TIM6 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM6_Init(void)
{

	/* USER CODE BEGIN TIM6_Init 0 */

	/* USER CODE END TIM6_Init 0 */

	TIM_MasterConfigTypeDef sMasterConfig = {0};

	/* USER CODE BEGIN TIM6_Init 1 */

	/* USER CODE END TIM6_Init 1 */
	htim6.Instance = TIM6;
	htim6.Init.Prescaler = 8000-1;
	htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim6.Init.Period = 1000-1;
	htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM6_Init 2 */

	/* USER CODE END TIM6_Init 2 */

}

/**
 * @brief TIM7 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM7_Init(void)
{

	/* USER CODE BEGIN TIM7_Init 0 */

	/* USER CODE END TIM7_Init 0 */

	TIM_MasterConfigTypeDef sMasterConfig = {0};

	/* USER CODE BEGIN TIM7_Init 1 */

	/* USER CODE END TIM7_Init 1 */
	htim7.Instance = TIM7;
	htim7.Init.Prescaler = 8000-1;
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim7.Init.Period = 10000-1;
	htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_OnePulse_Init(&htim7, TIM_OPMODE_SINGLE) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM7_Init 2 */

	/* USER CODE END TIM7_Init 2 */

}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void)
{

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* USER CODE BEGIN MX_GPIO_Init_1 */

	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, LD2_Pin|D7_Pin|D6_Pin|D2_Pin
			|D3_Pin|RS_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, D1_Pin|RW_Pin|D0_Pin|E_Pin
			|D5_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : B1_Pin */
	GPIO_InitStruct.Pin = B1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : LD2_Pin D7_Pin D6_Pin D2_Pin
                           D3_Pin RS_Pin */
	GPIO_InitStruct.Pin = LD2_Pin|D7_Pin|D6_Pin|D2_Pin
			|D3_Pin|RS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : D1_Pin RW_Pin D0_Pin E_Pin
                           D5_Pin */
	GPIO_InitStruct.Pin = D1_Pin|RW_Pin|D0_Pin|E_Pin
			|D5_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : D4_Pin */
	GPIO_InitStruct.Pin = D4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(D4_GPIO_Port, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	/* USER CODE BEGIN MX_GPIO_Init_2 */

	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
