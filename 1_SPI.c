/*
 *
 *
 * Configuración en CubeMX (.ioc):
 *   - SYSCLK 80 MHz (configuración por defecto de la Nucleo-L476RG).
 *   - SPI1: Full-Duplex Master, 8 bits, MSB first,
 *           Prescaler = 64 (80 MHz / 64 = 1.25 MHz),
 *           CPOL = Low, CPHA = 1 Edge  (modo SPI 0),
 *           NSS = Software.  Pines: PA5 = SCK, PA6 = MISO, PA7 = MOSI.
 *   - USART2 (ST-LINK Virtual COM): Asynchronous, 115200 8N1 (PA2 = TX, PA3 = RX).
 *
 * Conexiones (módulo GY-BME280-3.3, alimentar con 3.3 V):
 *   VCC -> 3V3,  GND -> GND,
 *   SCL -> PA5 (D13),  SDA -> PA7 (D11, MOSI),  SDO -> PA6 (D12, MISO),
 *   CSB -> PB6 (D10)
 *
 * Nota: PA5 también maneja el LED LD2 de la Nucleo, por lo que parpadeará con el reloj SPI.
 */

/* ===================== USER CODE BEGIN Includes ===================== */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
/* ===================== USER CODE END Includes ======================= */


/* ===================== USER CODE BEGIN PD =========================== */
#define BME_CS_PORT     GPIOB
#define BME_CS_PIN      GPIO_PIN_6

#define ALTITUDE_M      0.0f   /* Altitud del lugar [m]. 0 => presión de estación (sin corregir).
                                  Para comparar con RedMeteo (presión a nivel del mar),
                                  ingrese la altitud real. */

#define REG_ID          0xD0
#define REG_RESET       0xE0
#define REG_CTRL_HUM    0xF2   /* solo BME280 */
#define REG_STATUS      0xF3
#define REG_CTRL_MEAS   0xF4
#define REG_CONFIG      0xF5
#define REG_PRESS_MSB   0xF7   /* F7..F9 presión, FA..FC temperatura */
#define REG_CALIB       0x88   /* 0x88..0x9F: 24 bytes de calibración T y P */
/* ===================== USER CODE END PD ============================= */


/* ===================== USER CODE BEGIN PV =========================== */
static uint16_t dig_T1; static int16_t dig_T2, dig_T3;
static uint16_t dig_P1; static int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
static int32_t  t_fine;
/* ===================== USER CODE END PV ============================= */


/* ===================== USER CODE BEGIN 0 ============================ */

/* ---------- Capa SPI ---------- */
static inline void cs_low(void)  { HAL_GPIO_WritePin(BME_CS_PORT, BME_CS_PIN, GPIO_PIN_RESET); }
static inline void cs_high(void) { HAL_GPIO_WritePin(BME_CS_PORT, BME_CS_PIN, GPIO_PIN_SET); }

/* Lectura: bit 7 de la dirección en 1. Se envían len+1 bytes; los datos llegan en rx[1..len]. */
static void bme_read(uint8_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t tx[25], rx[25];               /* len máximo = 24 */
    memset(tx, 0, len + 1);
    tx[0] = reg | 0x80;
    cs_low();
    HAL_SPI_TransmitReceive(&hspi1, tx, rx, len + 1, 100);
    cs_high();
    memcpy(buf, &rx[1], len);
}

static uint8_t bme_read_reg(uint8_t reg)
{
    uint8_t v;
    bme_read(reg, &v, 1);
    return v;
}

/* Escritura: bit 7 de la dirección en 0. */
static void bme_write_reg(uint8_t reg, uint8_t val)
{
    uint8_t tx[2] = { reg & 0x7F, val };
    cs_low();
    HAL_SPI_Transmit(&hspi1, tx, 2, 100);
    cs_high();
}

/* ---------- UART ---------- */
static void uart_print(const char *s)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)s, strlen(s), 100);
}

/* ---------- Sensor ---------- */
static void bme_read_calibration(void)
{
    uint8_t c[24];
    bme_read(REG_CALIB, c, 24);           /* little endian */
    dig_T1 = (uint16_t)(c[1]  << 8 | c[0]);
    dig_T2 = (int16_t) (c[3]  << 8 | c[2]);
    dig_T3 = (int16_t) (c[5]  << 8 | c[4]);
    dig_P1 = (uint16_t)(c[7]  << 8 | c[6]);
    dig_P2 = (int16_t) (c[9]  << 8 | c[8]);
    dig_P3 = (int16_t) (c[11] << 8 | c[10]);
    dig_P4 = (int16_t) (c[13] << 8 | c[12]);
    dig_P5 = (int16_t) (c[15] << 8 | c[14]);
    dig_P6 = (int16_t) (c[17] << 8 | c[16]);
    dig_P7 = (int16_t) (c[19] << 8 | c[18]);
    dig_P8 = (int16_t) (c[21] << 8 | c[20]);
    dig_P9 = (int16_t) (c[23] << 8 | c[22]);
}

static int bme_init(void)
{
    char msg[64];

    bme_write_reg(REG_RESET, 0xB6);       /* reset por software */
    HAL_Delay(10);

    uint8_t id = bme_read_reg(REG_ID);
    snprintf(msg, sizeof(msg), "ID del chip: 0x%02X\r\n", id);
    uart_print(msg);

    if (id == 0x60)
        uart_print("Detectado BME280\r\n");
    else if (id == 0x58 || id == 0x56 || id == 0x57)
        uart_print("Detectado BMP280\r\n");
    else {
        uart_print("ERROR: ID desconocido. Revise conexiones/CS/modo SPI.\r\n");
        return -1;
    }

    while (bme_read_reg(REG_STATUS) & 0x01) { HAL_Delay(1); }  /* im_update = 0 */

    bme_read_calibration();

    bme_write_reg(REG_CTRL_HUM, 0x01);    /* humedad x1 (solo BME280; antes de ctrl_meas) */
    bme_write_reg(REG_CONFIG,   0x00);    /* filtro IIR off, SPI 4 hilos (t_sb no aplica en forced) */
    bme_write_reg(REG_CTRL_MEAS, 0x54);   /* osrs_t x2, osrs_p x16, modo sleep (se dispara en forced) */
    return 0;
}

/* Compensación de temperatura (entero 32 bits, datasheet Bosch). Actualiza t_fine. */
static void compensate_temp(int32_t adc_T)
{
    int32_t v1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
    int32_t v2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) *
                    ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;
    t_fine = v1 + v2;
}

/* Compensación de presión (entero 64 bits). Devuelve Pa en formato Q24.8 (dividir por 256). */
static uint32_t compensate_press(int32_t adc_P)
{
    int64_t v1 = ((int64_t)t_fine) - 128000;
    int64_t v2 = v1 * v1 * (int64_t)dig_P6;
    v2 = v2 + ((v1 * (int64_t)dig_P5) << 17);
    v2 = v2 + (((int64_t)dig_P4) << 35);
    v1 = ((v1 * v1 * (int64_t)dig_P3) >> 8) + ((v1 * (int64_t)dig_P2) << 12);
    v1 = (((((int64_t)1) << 47) + v1)) * ((int64_t)dig_P1) >> 33;
    if (v1 == 0) return 0;
    int64_t p = 1048576 - adc_P;
    p = (((p << 31) - v2) * 3125) / v1;
    v1 = (((int64_t)dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    v2 = (((int64_t)dig_P8) * p) >> 19;
    p = ((p + v1 + v2) >> 8) + (((int64_t)dig_P7) << 4);
    return (uint32_t)p;
}

/* Lee temperatura y presión en una ráfaga de 6 bytes; retorna presión en hPa. */
static float bme_read_pressure_hPa(void)
{
    uint8_t d[6];

    bme_write_reg(REG_CTRL_MEAS, 0x55);   /* modo forced: una medición y vuelve a sleep */
    HAL_Delay(45);                        /* t_measure,max = 1.25 + 2.3*2 + (2.3*16 + 0.575) = 43.2 ms */
    for (uint8_t i = 0; i < 20 && (bme_read_reg(REG_STATUS) & 0x08); i++) {
        HAL_Delay(1);                     /* espera a measuring = 0 (bit 3 de status) */
    }

    bme_read(REG_PRESS_MSB, d, 6);

    int32_t adc_P = ((int32_t)d[0] << 12) | ((int32_t)d[1] << 4) | (d[2] >> 4);
    int32_t adc_T = ((int32_t)d[3] << 12) | ((int32_t)d[4] << 4) | (d[5] >> 4);

    compensate_temp(adc_T);               /* necesario para t_fine */
    float p = compensate_press(adc_P) / 256.0f / 100.0f;

    if (ALTITUDE_M != 0.0f)               /* reducción a nivel del mar */
        p = p / powf(1.0f - ALTITUDE_M / 44330.0f, 5.255f);
    return p;
}

/* ===================== USER CODE END 0 ============================== */


/* ===================== USER CODE BEGIN 2 (antes del while(1)) ======= */
    cs_high();                            /* CS inactivo */
    HAL_Delay(50);

    if (bme_init() != 0) {
        while (1) { }                     /* detener si el sensor no responde */
    }

    uint32_t next = HAL_GetTick();
/* ===================== USER CODE END 2 ============================== */


/* ===================== USER CODE BEGIN 3 (dentro del while(1)) ====== */
    {
        char msg[48];
        float p = bme_read_pressure_hPa();

        /* Se evita %f (newlib-nano no lo soporta por defecto): se imprime en décimas. */
        int32_t p10 = (int32_t)(p * 10.0f + 0.5f);
        snprintf(msg, sizeof(msg), "Presion: %ld.%ld hPa\r\n",
                 (long)(p10 / 10), (long)(p10 % 10));
        uart_print(msg);

        next += 1000;                     /* periodo de 1 s sin acumular deriva */
        while ((int32_t)(HAL_GetTick() - next) < 0) { }
    }
/* ===================== USER CODE END 3 ============================== */
