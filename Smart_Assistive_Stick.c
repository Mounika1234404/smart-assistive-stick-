/*
 * SMART BLIND STICK - SINGLE APPLICATION FILE
 * MCU: STM32F401CCU6
 * HC-SR04 + soil ADC + buzzer + vibration + LEDs + SSD1306 OLED
 * + HC-05 UART emergency alert + PC13 emergency button.
 * Add this as the ONLY application C file in Keil.
 * Keep CMSIS, startup file and system_stm32f4xx.c.
 */
#include "stm32f401xc.h"
#include "system_stm32f4xx.h"
#include <stdint.h>
#include <string.h>

/* ===== gpio.h ===== */

#ifndef GPIO_H
#define GPIO_H

#define VIBRATION_PIN  4U
#define GREEN_LED_PIN  5U
#define BUZZER_PIN     6U
#define YELLOW_LED_PIN 7U

void GPIO_InitAll(void);
void GPIO_WriteOutput(uint8_t pin, uint8_t state);

#endif


/* ===== adc.h ===== */

#ifndef ADC_H
#define ADC_H

#define ADC_READ_ERROR 0xFFFFU

void SoilADC_Init(void);
uint16_t SoilADC_ReadRaw(void);

#endif


/* ===== ultrasonic.h ===== */

#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#define ULTRASONIC_NO_ECHO 0xFFFFU

void Ultrasonic_Init(void);
uint16_t Ultrasonic_ReadDistanceCm(void);

#endif


/* ===== uart.h ===== */

#ifndef UART_H
#define UART_H

void UART1_Init(void);
void UART1_SendChar(char value);
void UART1_SendString(const char *text);
void UART1_SendUInt(unsigned int value);

/* Compatibility with existing app.c */
void UART2_Init(void);
void UART2_SendChar(char value);
void UART2_SendString(const char *text);
void UART2_SendUInt(unsigned int value);

#endif


/* ===== i2c.h ===== */

#ifndef I2C_H
#define I2C_H

void I2C1_Init(void);
uint8_t I2C1_Write(uint8_t control, const uint8_t *data,
                   uint16_t length);

#endif


/* ===== oled.h ===== */

#ifndef OLED_H
#define OLED_H

void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawLine(uint8_t page, const char *text);
uint8_t OLED_IsReady(void);

#endif


/* ===== timebase.h ===== */
#ifndef TIMEBASE_H
#define TIMEBASE_H

/* Start the Cortex-M DWT cycle counter. */
void Timebase_Init(void);

/* Busy-wait delay functions. */
void Timebase_DelayUs(uint32_t us);
void Timebase_DelayMs(uint32_t ms);

#endif


/* ===== app.h ===== */

#ifndef APP_H
#define APP_H

void APP_Init(void);
void APP_RunOnce(void);

#endif


/* ===== timebase.c ===== */

static uint32_t cycles_per_us = 16U;

void Timebase_Init(void)
{
    /* Enable trace block so DWT cycle counting can run. */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    /* Reset and enable the 32-bit cycle counter. */
    DWT->CYCCNT = 0U;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    /* Derive timing from the configured core clock. */
    cycles_per_us = SystemCoreClock / 1000000U;

    if (cycles_per_us == 0U)
    {
        cycles_per_us = 16U;
    }
}

void Timebase_DelayUs(uint32_t us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t cycles = us * cycles_per_us;

    /* Unsigned subtraction remains valid across counter wraparound. */
    while ((uint32_t)(DWT->CYCCNT - start) < cycles)
    {
        /* Wait for the required cycle count. */
    }
}

void Timebase_DelayMs(uint32_t ms)
{
    while (ms > 0U)
    {
        Timebase_DelayUs(1000U);
        ms--;
    }
}


/* ===== gpio.c ===== */

void GPIO_InitAll(void)
{
    /* Enable GPIO port A and B peripheral clocks. */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN |
                    RCC_AHB1ENR_GPIOBEN;
    (void)RCC->AHB1ENR;

    /*
     * PA0 = ECHO input.
     * PA1 = TRIG output.
     * PA4 = analog input for soil sensor.
     */
    GPIOA->MODER &= ~((3U << 0U) |
                      (3U << 2U) |
                      (3U << 8U));

    GPIOA->MODER |= (1U << 2U) | (3U << 8U);

    GPIOA->PUPDR &= ~((3U << 0U) |
                      (3U << 2U) |
                      (3U << 8U));

    /* Configure PA1 as push-pull output and start TRIG LOW. */
    GPIOA->OTYPER &= ~(1U << 1U);
    GPIOA->BSRR = (1U << (1U + 16U));

    /*
     * PB4 = vibration motor driver input.
     * PB5 = green LED.
     * PB6 = buzzer driver input.
     * PB7 = yellow LED.
     */
    GPIOB->MODER &= ~((3U << 8U) |
                      (3U << 10U) |
                      (3U << 12U) |
                      (3U << 14U));

    GPIOB->MODER |= (1U << 8U) |
                    (1U << 10U) |
                    (1U << 12U) |
                    (1U << 14U);

    GPIOB->OTYPER &= ~((1U << 4U) |
                       (1U << 5U) |
                       (1U << 6U) |
                       (1U << 7U));

    /* Start with all outputs OFF. */
    GPIOB->BSRR = (1U << (4U + 16U)) |
                  (1U << (5U + 16U)) |
                  (1U << (6U + 16U)) |
                  (1U << (7U + 16U));
									
		/* PC13 emergency button input with internal pull-up */
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

		GPIOC->MODER &= ~(3U << (13U * 2U));

		GPIOC->PUPDR &= ~(3U << (13U * 2U));
		GPIOC->PUPDR |=  (1U << (13U * 2U));
}

void GPIO_WriteOutput(uint8_t pin, uint8_t state)
{
    if (pin > 15U)
    {
        return;
    }

    /* BSRR sets a pin with lower bits and resets it with upper bits. */
    if (state != 0U)
    {
        GPIOB->BSRR = (1U << pin);
    }
    else
    {
        GPIOB->BSRR = (1U << (pin + 16U));
    }
		
}






/* ===== uart.c ===== */

void UART1_Init(void)
{
    /* Enable GPIOA and USART1 clocks */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    (void)RCC->APB2ENR;

    /* PA9 = TX, PA10 = RX: Alternate-function mode */
    GPIOA->MODER &= ~((3U << 18U) | (3U << 20U));
    GPIOA->MODER |=  ((2U << 18U) | (2U << 20U));

    /* AF7 selects USART1 on PA9 and PA10 */
    GPIOA->AFR[1] &= ~((0xFU << 4U) | (0xFU << 8U));
    GPIOA->AFR[1] |=  ((7U << 4U) | (7U << 8U));

    /* Configure pin speed and no pull-up/pull-down */
    GPIOA->OSPEEDR |= (3U << 18U) | (3U << 20U);
    GPIOA->PUPDR &= ~((3U << 18U) | (3U << 20U));

    /*
     * Assumption: PCLK2 = 16 MHz.
     * Baud rate = 9600; BRR = 1667.
     * Verify APB2 clock in your project.
     */
    USART1->CR1 = 0U;
    USART1->CR2 = 0U;
    USART1->CR3 = 0U;
    USART1->BRR = 1667U;

    /* Enable transmitter, receiver and USART1 */
    USART1->CR1 = USART_CR1_TE |
                  USART_CR1_RE |
                  USART_CR1_UE;
}

void UART1_SendChar(char value)
{
    while ((USART1->SR & USART_SR_TXE) == 0U)
    {
    }

    USART1->DR = (uint8_t)value;
}

void UART1_SendString(const char *text)
{
    if (text == 0)
    {
        return;
    }

    while (*text != '\0')
    {
        UART1_SendChar(*text);
        text++;
    }
}

void UART1_SendUInt(unsigned int value)
{
    char digits[10];
    unsigned int count = 0U;

    if (value == 0U)
    {
        UART1_SendChar('0');
        return;
    }

    while ((value > 0U) && (count < sizeof(digits)))
    {
        digits[count] = (char)('0' + (value % 10U));
        value /= 10U;
        count++;
    }

    while (count > 0U)
    {
        count--;
        UART1_SendChar(digits[count]);
    }
}

/*
 * Compatibility functions:
 * Existing app.c can keep calling UART2_* names,
 * but these functions use USART1 hardware.
 */
void UART2_Init(void)
{
    UART1_Init();
}

void UART2_SendChar(char value)
{
    UART1_SendChar(value);
}

void UART2_SendString(const char *text)
{
    UART1_SendString(text);
}

void UART2_SendUInt(unsigned int value)
{
    UART1_SendUInt(value);
}


/* ===== i2c.c ===== */

#define OLED_ADDRESS_WRITE 0x78U
#define I2C_TIMEOUT 200000U

static uint8_t WaitFlag(uint32_t flag)
{
    uint32_t timeout = I2C_TIMEOUT;

    while ((I2C1->SR1 & flag) == 0U)
    {
        if ((I2C1->SR1 & (I2C_SR1_AF |
                          I2C_SR1_BERR |
                          I2C_SR1_ARLO)) != 0U)
        {
            I2C1->CR1 |= I2C_CR1_STOP;
            return 0U;
        }

        if (timeout == 0U)
        {
            I2C1->CR1 |= I2C_CR1_STOP;
            return 0U;
        }

        timeout--;
    }

    return 1U;
}

void I2C1_Init(void)
{
    /* Enable GPIOB and I2C1 clocks. */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    (void)RCC->APB1ENR;

    /* PB8 and PB9 alternate function mode. */
    GPIOB->MODER &= ~((3U << 16U) | (3U << 18U));
    GPIOB->MODER |=  ((2U << 16U) | (2U << 18U));

    /* I2C uses open-drain outputs. */
    GPIOB->OTYPER |= (1U << 8U) | (1U << 9U);

    /* Select AF4 (I2C1) on PB8/PB9. */
    GPIOB->AFR[1] &= ~((0xFU << 0U) | (0xFU << 4U));
    GPIOB->AFR[1] |=  ((4U << 0U) | (4U << 4U));

    /* Reset I2C peripheral to a known state. */
    I2C1->CR1 = I2C_CR1_SWRST;
    I2C1->CR1 = 0U;

    /*
     * Assumption: APB1 clock is 16 MHz.
     * Standard mode: CCR = 80 gives about 100 kHz.
     * TRISE = 17 for a 16 MHz peripheral clock.
     */
    I2C1->CR2 = 16U;
    I2C1->CCR = 80U;
    I2C1->TRISE = 17U;
    I2C1->CR1 = I2C_CR1_PE;
}

uint8_t I2C1_Write(uint8_t control, const uint8_t *data,
                   uint16_t length)
{
    uint16_t i;
    uint32_t timeout = I2C_TIMEOUT;

    /* Wait until the bus is idle. */
    while ((I2C1->SR2 & I2C_SR2_BUSY) != 0U)
    {
        if (timeout-- == 0U)
        {
            return 0U;
        }
    }

    /* Generate START condition. */
    I2C1->CR1 |= I2C_CR1_START;
    if (WaitFlag(I2C_SR1_SB) == 0U)
    {
        return 0U;
    }

    /* Send 7-bit address 0x3C shifted left, with write bit 0. */
    I2C1->DR = OLED_ADDRESS_WRITE;

    if (WaitFlag(I2C_SR1_ADDR) == 0U)
    {
        return 0U;
    }

    /* Reading SR1 followed by SR2 clears the ADDR flag. */
    (void)I2C1->SR1;
    (void)I2C1->SR2;

    /* Send command/data control byte. */
    if (WaitFlag(I2C_SR1_TXE) == 0U)
    {
        return 0U;
    }
    I2C1->DR = control;

    /* Send payload bytes. */
    for (i = 0U; i < length; i++)
    {
        if (WaitFlag(I2C_SR1_TXE) == 0U)
        {
            return 0U;
        }
        I2C1->DR = data[i];
    }

    /* Wait until final byte is shifted out, then STOP. */
    if (WaitFlag(I2C_SR1_BTF) == 0U)
    {
        return 0U;
    }

    I2C1->CR1 |= I2C_CR1_STOP;
    return 1U;
}


/* ===== adc.c ===== */

void SoilADC_Init(void)
{
    /* Enable ADC1 peripheral clock. */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    (void)RCC->APB2ENR;

    /* ADC prescaler: PCLK2 / 4. With PCLK2=16 MHz, ADC clock=4 MHz. */
    ADC->CCR &= ~(3U << 16U);
    ADC->CCR |=  (1U << 16U);

    /* Select 12-bit resolution and software-triggered conversion. */
    ADC1->CR1 = 0U;
    ADC1->CR2 = 0U;

    /* Channel 4 (PA4): long sample time for stable acquisition. */
    ADC1->SMPR2 &= ~(7U << 12U);
    ADC1->SMPR2 |=  (7U << 12U);

    /* One conversion in the sequence, channel 4 first. */
    ADC1->SQR1 = 0U;
    ADC1->SQR3 = 4U;

    /* Turn on ADC1 and allow a short startup delay. */
    ADC1->CR2 |= ADC_CR2_ADON;
    Timebase_DelayMs(2U);
}

uint16_t SoilADC_ReadRaw(void)
{
    uint32_t timeout = 100000U;

    /* Clear old status flags before starting a conversion. */
    ADC1->SR = 0U;

    /* Start one software-triggered ADC conversion. */
    ADC1->CR2 |= ADC_CR2_SWSTART;

    /* Wait for end-of-conversion with a timeout. */
    while ((ADC1->SR & ADC_SR_EOC) == 0U)
    {
        if (timeout == 0U)
        {
            return ADC_READ_ERROR;
        }

        timeout--;
    }

    /* Reading DR returns the 12-bit conversion result. */
    return (uint16_t)(ADC1->DR & 0x0FFFU);
}


/* ===== ultrasonic.c ===== */

#define TRIG_PIN 1U
#define ECHO_PIN 0U
#define ECHO_TIMEOUT_US 30000U

void Ultrasonic_Init(void)
{
    /* PA1/PA0 are configured by GPIO_InitAll(). */
    GPIOA->BSRR = (1U << (TRIG_PIN + 16U));
}

uint16_t Ultrasonic_ReadDistanceCm(void)
{
    uint32_t start;
    uint32_t echo_start;
    uint32_t pulse_us;
    uint32_t cycles_per_us = SystemCoreClock / 1000000U;

    if (cycles_per_us == 0U)
    {
        return ULTRASONIC_NO_ECHO;
    }

    /* HC-SR04 requires a trigger pulse of at least 10 us. */
    GPIOA->BSRR = (1U << (TRIG_PIN + 16U));
    Timebase_DelayUs(3U);
    GPIOA->BSRR = (1U << TRIG_PIN);
    Timebase_DelayUs(10U);
    GPIOA->BSRR = (1U << (TRIG_PIN + 16U));

    /* Wait for the rising edge of ECHO, with timeout. */
    start = DWT->CYCCNT;
    while ((GPIOA->IDR & (1U << ECHO_PIN)) == 0U)
    {
        if (((uint32_t)(DWT->CYCCNT - start) / cycles_per_us)
            >= ECHO_TIMEOUT_US)
        {
            return ULTRASONIC_NO_ECHO;
        }
    }

    /* Record the start of the HIGH pulse. */
    echo_start = DWT->CYCCNT;

    /* Wait for the falling edge, again with timeout. */
    while ((GPIOA->IDR & (1U << ECHO_PIN)) != 0U)
    {
        if (((uint32_t)(DWT->CYCCNT - echo_start) / cycles_per_us)
            >= ECHO_TIMEOUT_US)
        {
            return ULTRASONIC_NO_ECHO;
        }
    }

    pulse_us = (uint32_t)(DWT->CYCCNT - echo_start) / cycles_per_us;

    /*
     * Approximate HC-SR04 conversion:
     * distance_cm = echo_high_time_us / 58.
     */
    if ((pulse_us == 0U) || (pulse_us >= ECHO_TIMEOUT_US))
    {
        return ULTRASONIC_NO_ECHO;
    }

    return (uint16_t)(pulse_us / 58U);
}


/* ===== oled.c ===== */

#define OLED_WIDTH 128U

static uint8_t oled_ready = 0U;

/* Character set: space, A-Z, 0-9, colon, dot, dash, question mark. */
static const uint8_t font[][5] =
{
    {0,0,0,0,0},                 /* space */
    {0x7E,0x11,0x11,0x11,0x7E}, /* A */
    {0x7F,0x49,0x49,0x49,0x36}, /* B */
    {0x3E,0x41,0x41,0x41,0x22}, /* C */
    {0x7F,0x41,0x41,0x22,0x1C}, /* D */
    {0x7F,0x49,0x49,0x49,0x41}, /* E */
    {0x7F,0x09,0x09,0x09,0x01}, /* F */
    {0x3E,0x41,0x49,0x49,0x7A}, /* G */
    {0x7F,0x08,0x08,0x08,0x7F}, /* H */
    {0,0x41,0x7F,0x41,0},       /* I */
    {0x20,0x40,0x41,0x3F,0x01}, /* J */
    {0x7F,0x08,0x14,0x22,0x41}, /* K */
    {0x7F,0x40,0x40,0x40,0x40}, /* L */
    {0x7F,0x02,0x0C,0x02,0x7F}, /* M */
    {0x7F,0x04,0x08,0x10,0x7F}, /* N */
    {0x3E,0x41,0x41,0x41,0x3E}, /* O */
    {0x7F,0x09,0x09,0x09,0x06}, /* P */
    {0x3E,0x41,0x51,0x21,0x5E}, /* Q */
    {0x7F,0x09,0x19,0x29,0x46}, /* R */
    {0x46,0x49,0x49,0x49,0x31}, /* S */
    {0x01,0x01,0x7F,0x01,0x01}, /* T */
    {0x3F,0x40,0x40,0x40,0x3F}, /* U */
    {0x1F,0x20,0x40,0x20,0x1F}, /* V */
    {0x3F,0x40,0x38,0x40,0x3F}, /* W */
    {0x63,0x14,0x08,0x14,0x63}, /* X */
    {0x07,0x08,0x70,0x08,0x07}, /* Y */
    {0x61,0x51,0x49,0x45,0x43}, /* Z */
    {0x3E,0x51,0x49,0x45,0x3E}, /* 0 */
    {0,0x42,0x7F,0x40,0},       /* 1 */
    {0x42,0x61,0x51,0x49,0x46}, /* 2 */
    {0x21,0x41,0x45,0x4B,0x31}, /* 3 */
    {0x18,0x14,0x12,0x7F,0x10}, /* 4 */
    {0x27,0x45,0x45,0x45,0x39}, /* 5 */
    {0x3C,0x4A,0x49,0x49,0x30}, /* 6 */
    {0x01,0x71,0x09,0x05,0x03}, /* 7 */
    {0x36,0x49,0x49,0x49,0x36}, /* 8 */
    {0x06,0x49,0x49,0x29,0x1E}, /* 9 */
    {0,0x36,0x36,0,0},          /* : */
    {0,0x60,0x60,0,0},          /* . */
    {0x08,0x08,0x08,0x08,0x08}, /* - */
    {0x02,0x01,0x51,0x09,0x06}  /* ? */
};

static uint8_t FontIndex(char c)
{
    if (c == ' ')
        return 0U;

    if (c >= 'A' && c <= 'Z')
        return (uint8_t)(1U + c - 'A');

    if (c >= '0' && c <= '9')
        return (uint8_t)(27U + c - '0');

    if (c == ':')
        return 37U;

    if (c == '.')
        return 38U;

    if (c == '-')
        return 39U;

    return 40U; /* Unsupported character -> '?' */
}

static uint8_t OLED_Command(uint8_t command)
{
    return I2C1_Write(0x00U, &command, 1U);
}

static void OLED_SetPage(uint8_t page)
{
    (void)OLED_Command((uint8_t)(0xB0U | page));
    (void)OLED_Command(0x00U);
    (void)OLED_Command(0x10U);
}

void OLED_Init(void)
{
    static const uint8_t commands[] =
    {
        0xAE,       /* Display OFF during setup */
        0xD5, 0x80, /* Display clock */
        0xA8, 0x3F, /* 64 multiplex rows */
        0xD3, 0x00, /* Display offset */
        0x40,       /* Start line */
        0x8D, 0x14, /* Enable charge pump */
        0x20, 0x02, /* Page addressing mode */
        0xA1,       /* Segment remap */
        0xC8,       /* COM scan direction */
        0xDA, 0x12, /* COM pin configuration */
        0x81, 0x7F, /* Contrast */
        0xD9, 0xF1, /* Pre-charge */
        0xDB, 0x40, /* VCOMH */
        0xA4,       /* Display RAM */
        0xA6,       /* Normal display */
        0x2E,       /* Scrolling OFF */
        0xAF        /* Display ON */
    };

    uint16_t i;

    oled_ready = 0U;
    Timebase_DelayMs(100U);

    for (i = 0U; i < sizeof(commands); i++)
    {
        if (OLED_Command(commands[i]) == 0U)
        {
            return;
        }
    }

    oled_ready = 1U;
    OLED_Clear();
}

uint8_t OLED_IsReady(void)
{
    return oled_ready;
}

void OLED_Clear(void)
{
    uint8_t page;
    uint8_t blank[OLED_WIDTH];

    memset(blank, 0, sizeof(blank));

    if (oled_ready == 0U)
        return;

    for (page = 0U; page < 8U; page++)
    {
        OLED_SetPage(page);
        (void)I2C1_Write(0x40U, blank, OLED_WIDTH);
    }
}

void OLED_DrawLine(uint8_t page, const char *text)
{
    uint8_t buffer[OLED_WIDTH];
    uint16_t x = 4U;    /* Shift text 4 pixels to the right */
    uint8_t col;
    uint8_t count = 0U;

    if ((oled_ready == 0U) || (page > 7U) || (text == 0))
        return;

    memset(buffer, 0, sizeof(buffer));

    /* 20 characters fit safely: 4-pixel offset + 20*6 = 124. */
    while ((*text != '\0') && (count < 20U))
    {
        uint8_t index = FontIndex(*text);

        for (col = 0U; col < 5U; col++)
        {
            buffer[x++] = font[index][col];
        }

        buffer[x++] = 0U; /* Space between characters. */
        text++;
        count++;
    }

    OLED_SetPage(page);
    (void)I2C1_Write(0x40U, buffer, OLED_WIDTH);
}


/* ===== app.c ===== */

/* Change to 1 for active-LOW buzzer/motor driver modules. */
#define BUZZER_ACTIVE_LOW    0U
#define MOTOR_ACTIVE_LOW     0U
#define OBJECT_DISTANCE_CM  70U
#define APP_PERIOD_MS       300U

/*
 * SOIL THRESHOLDS
 *
 * Assumption:
 * Lower ADC = wet soil
 * Higher ADC = dry soil
 *
 * These are initial example values.
 * Calibrate them using actual sensor readings.
 */
#define SOIL_WET_THRESHOLD  450U
#define SOIL_DRY_THRESHOLD  800U



/* Retain the previous soil status between thresholds. */
static uint8_t soil_is_wet = 0U;

static uint8_t button_ready = 1U;


/* Average 10 ADC samples to reduce fluctuations. */
static uint16_t SoilADC_ReadAverage(void)
{
    uint32_t total = 0U;
    uint8_t i;
    uint16_t value;

    for (i = 0U; i < 10U; i++)
    {
        value = SoilADC_ReadRaw();

        if (value == ADC_READ_ERROR)
        {
            return ADC_READ_ERROR;
        }

        total += value;
        Timebase_DelayMs(5U);
    }

    return (uint16_t)(total / 10U);
}

void APP_Init(void)
{
    GPIO_InitAll();
    UART2_Init();
    Timebase_Init();
    Ultrasonic_Init();
    SoilADC_Init();
    I2C1_Init();
    OLED_Init();

    UART2_SendString("\r\nSMART BLIND STICK STARTED\r\n");
    UART2_SendString("UART=9600 8N1\r\n");

    if (OLED_IsReady() != 0U)
    {
        OLED_DrawLine(0U, "SMART BLIND STICK");
        OLED_DrawLine(1U, "SYSTEM STARTING");
        OLED_DrawLine(2U, "PLEASE WAIT");
        OLED_DrawLine(3U, "                    ");
        OLED_DrawLine(4U, "SOIL: CHECKING");
        OLED_DrawLine(5U, "                    ");
        OLED_DrawLine(6U, "VIBRATION: OFF");
        OLED_DrawLine(7U, "                    ");

        Timebase_DelayMs(700U);
    }
    else
    {
        UART2_SendString("OLED_INIT_ERROR\r\n");
    }
}

void APP_RunOnce(void)
{
    uint16_t distance;
    uint16_t soil;

    uint8_t buzzer = 0U;
    uint8_t vibration = 0U;

    const char *object_status;
    const char *soil_status;

    char line[22];

    /* Read sensors */
    distance = Ultrasonic_ReadDistanceCm();
    soil = SoilADC_ReadAverage();

    /*
     * OBJECT DETECTION
     * Object within 70 cm -> buzzer ON.
     * No echo or farther than 70 cm -> buzzer OFF.
     */
    if ((distance != ULTRASONIC_NO_ECHO) &&
        (distance <= OBJECT_DISTANCE_CM))
    {
        object_status = "OBJECT DETECTED";
        buzzer = 1U;
    }
    else if (distance == ULTRASONIC_NO_ECHO)
    {
        object_status = "NO ECHO";
        buzzer = 0U;
    }
    else
    {
        object_status = "NO OBJECT";
        buzzer = 0U;
    }

    /*
     * SOIL MOISTURE
     * Hysteresis prevents frequent WET/DRY switching.
     */
    if (soil == ADC_READ_ERROR)
    {
        soil_status = "SENSOR ERROR";
        vibration = 0U;
    }
    else
    {
        if (soil <= SOIL_WET_THRESHOLD)
        {
            soil_is_wet = 1U;
        }
        else if (soil >= SOIL_DRY_THRESHOLD)
        {
            soil_is_wet = 0U;
        }
        /* Between thresholds, retain previous state. */

        if (soil_is_wet != 0U)
        {
            soil_status = "WET";
            vibration = 1U;
        }
        else
        {
            soil_status = "DRY";
            vibration = 0U;
        }
    }

    /* Control outputs independently */
    GPIO_WriteOutput(BUZZER_PIN, buzzer);
    GPIO_WriteOutput(VIBRATION_PIN, vibration);

    /* LEDs indicate object detection */
    if ((distance != ULTRASONIC_NO_ECHO) &&
        (distance <= OBJECT_DISTANCE_CM))
    {
        GPIO_WriteOutput(GREEN_LED_PIN, 0U);
        GPIO_WriteOutput(YELLOW_LED_PIN, 1U);
    }
    else
    {
        GPIO_WriteOutput(GREEN_LED_PIN, 1U);
        GPIO_WriteOutput(YELLOW_LED_PIN, 0U);
    }

    /*
     * OLED display
     * Only object status, soil status and vibration.
     */
    OLED_DrawLine(0U, "SMART BLIND STICK");

    OLED_DrawLine(1U, "OBJECT STATUS");
    strcpy(line, object_status);
    OLED_DrawLine(2U, line);

    OLED_DrawLine(3U, "                    ");

    OLED_DrawLine(4U, "SOIL MOISTURE");
    strcpy(line, "SOIL: ");
    strcat(line, soil_status);
    OLED_DrawLine(5U, line);

    if (vibration != 0U)
    {
        OLED_DrawLine(6U, "VIBRATION: ON");
    }
    else
    {
        OLED_DrawLine(6U, "VIBRATION: OFF");
    }

    OLED_DrawLine(7U, "                    ");
		
		
		/* Bluetooth: send message only when emergency button is pressed */
		if ((GPIOC->IDR & (1U << 13U)) == 0U)
		{
				if (button_ready != 0U)
				{
						Timebase_DelayMs(30U);  /* Debounce */

						if ((GPIOC->IDR & (1U << 13U)) == 0U)
						{
								button_ready = 0U;

								UART2_SendString("\r\nEMERGENCY ALERT!\r\n");
								UART2_SendString(
										"The blind stick user needs help.\r\n"
								);
						}
				}
		}
		else
		{
				/* Allow another message after button release */
				button_ready = 1U;
		}




    Timebase_DelayMs(APP_PERIOD_MS);
}


/* ===== main.c ===== */

int main(void)
{
    /* Update CMSIS SystemCoreClock from the system clock configuration. */
    SystemCoreClockUpdate();

    /*
     * Initialize timebase before any microsecond/millisecond delays.
     * Application initializes all peripherals afterward.
     */
    Timebase_Init();
    APP_Init();


    /* Main: read sensors, update outputs, display and transmit. */
    while (1)
    {
			
        APP_RunOnce();
    }
}




