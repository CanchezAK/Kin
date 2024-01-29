#define DEV_ADDR_DAC 0x18
#define DEV_ADDR_ADC0 0x4C
#define DEV_ADDR_ADC2 0x4D
#define DEV_ADDR_ADC3 0x4E
#define DEV_ADDR_ADC1 0x4F

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "hardware/gpio.h"

uint8_t adc_data_mass[] = {0x01, 0x8B};
uint8_t dac_data_mass[] = {0x02, 0xC0}; //C0
uint8_t out_data[] = {0};
uint8_t sout_data[] = {0};
unsigned char val = 0x3D;

bool reserved_addr(uint8_t addr)
{
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

bool DeviceInit(uint8_t addr, uint8_t* data, uint8_t len)
{
    uint8_t control_data[1] = {0};
    i2c_write_blocking(i2c0, addr, data, len, false);
    i2c_read_blocking(i2c0, addr, control_data, 1, false);
    printf("%02x", control_data[0]);
    printf("\r\n");
    printf("\r\n");
    if (control_data[0] == data[1])
    {
        return true;
    } 
    else 
    {
        return false;
    }
}
#define DEV_ADDR_KBD0 0x20
#define DEV_ADDR_KBD1 0x21
#define DEV_ADDR_KBD2 0x22
void keyboard_init()
{
    /*Writing config register*/
    unsigned char data[2] = {0x03, 0xF0};
    i2c_write_blocking(i2c1, DEV_ADDR_KBD0, data, 2, false);

    data[0] = 0x03;
    data[1] = 0x00;
    i2c_write_blocking(i2c1, DEV_ADDR_KBD1, data, 2, false);

    data[0] = 0x03;
    data[1] = 0xFF;
    i2c_write_blocking(i2c1, DEV_ADDR_KBD2, data, 2, false);
    /*End of writing config register*/

    /*Enabling LED's*/
    data[0] = 0x01;
    data[1] = 0x04;
    i2c_write_blocking(i2c1, DEV_ADDR_KBD0, data, 2, false);

    data[0] = 0x01;
    data[1] = 0x3D;
    i2c_write_blocking(i2c1, DEV_ADDR_KBD1, data, 2, false);

    data[0] = 0x01;
    data[1] = 0x00;
    i2c_write_blocking(i2c1, DEV_ADDR_KBD2, data, 2, false);
    i2c_write_blocking(i2c1, DEV_ADDR_KBD0, 0x00, 1, false);
    i2c_write_blocking(i2c1, DEV_ADDR_KBD1, 0x00, 1, false);
    i2c_write_blocking(i2c1, DEV_ADDR_KBD2, 0x00, 1, false);
    /*End of enabling LED's*/
}

void keyboard_led(unsigned char LED, bool state)
{
    unsigned char data[2];
    switch (LED)
    {

    case 6:
        data[0] = 0x01;
        state == true ? data[1] = val | 1: data[1] = val & 0x3C;
        val = data[1];
        i2c_write_blocking(i2c1, DEV_ADDR_KBD1, data, 2, false);
        i2c_write_blocking(i2c1, DEV_ADDR_KBD1, 0x00, 1, false);
        break;
    case 5:
         data[0] = 0x01;
         state == true ? data[1] = 0x04 : data[1] = 0x00;
         i2c_write_blocking(i2c1, DEV_ADDR_KBD0, data, 2, false);
         i2c_write_blocking(i2c1, DEV_ADDR_KBD0, 0x00, 1, false);
        break;
    case 4:
        data[0] = 0x01;
        state == true ? data[1] = val | 0x20 : data[1] = val & 0x1D;
        val = data[1];
        printf("\r\n\tValue case 4: %02x\r\n", val);
        i2c_write_blocking(i2c1, DEV_ADDR_KBD1, data, 2, false);
        i2c_write_blocking(i2c1, DEV_ADDR_KBD1, 0x00, 1, false);
        break;
    case 3:
        data[0] = 0x01;
        state == true ? data[1] = val | 0x10 : data[1] = val & 0x2D;
        val = data[1];
        i2c_write_blocking(i2c1, DEV_ADDR_KBD1, data, 2, false);
        i2c_write_blocking(i2c1, DEV_ADDR_KBD1, 0x00, 1, false);
        break;
    case 2:
        data[0] = 0x01;
        state == true ? data[1] = val | 0x8 : data[1] = val & 0x35;
        val = data[1];
        //printf("%02x", val);
        i2c_write_blocking(i2c1, DEV_ADDR_KBD1, data, 2, false);
        i2c_write_blocking(i2c1, DEV_ADDR_KBD1, 0x00, 1, false);
        break;
    case 1:
        data[0] = 0x01;
        state == true ? data[1] = val | 0x4 : data[1] = val & 0x39;
        val = data[1];
        printf("\r\n\tValue case 1: %02x\r\n", val);
        i2c_write_blocking(i2c1, DEV_ADDR_KBD1, data, 2, false);
        i2c_write_blocking(i2c1, DEV_ADDR_KBD1, 0x00, 1, false);
        break;
    case 0:
        data[0] = 0x01;
        data[1] = 0x3D;
        i2c_write_blocking(i2c1, DEV_ADDR_KBD1, data, 2, false);
        i2c_write_blocking(i2c1, DEV_ADDR_KBD1, 0x00, 1, false);
        break;
    
    default:
        break;
    }
}

unsigned char keyboard()
{
    unsigned char key;
    unsigned char key0;
    unsigned char key1;
    i2c_read_blocking(i2c1, DEV_ADDR_KBD0, &key0, 1, false);
    key0 = key0 << 3;
    key0 = key0 >> 7;
    i2c_read_blocking(i2c1, DEV_ADDR_KBD2, &key1, 1, false);
    key1 = key1 << 2;
    key1 = key1 >> 2;
    key = (key0-1) | key1;
    return key;
}

int main() 
{
    stdio_init_all();
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, true);
    gpio_put(PICO_DEFAULT_LED_PIN, false);
    gpio_init(6);
    //gpio_init(7);
    gpio_set_dir(6, true);
    //gpio_set_dir(7, GPIO_IN);
    //gpio_pull_up(7);
    //gpio_put(6, false);
    //while (gpio_get(7));
    //gpio_put(6, true);
    i2c_init(i2c0, 100 * 1000);
    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);
    gpio_pull_up(4);
    gpio_pull_up(5);   
    bi_decl(bi_2pins_with_func(4, 5, GPIO_FUNC_I2C));

    i2c_init(i2c1, 100 * 1000);
    gpio_set_function(26, GPIO_FUNC_I2C);
    gpio_set_function(27, GPIO_FUNC_I2C);
    gpio_pull_up(26);
    gpio_pull_up(27);   
    bi_decl(bi_2pins_with_func(26, 27, GPIO_FUNC_I2C));

    keyboard_init();
    while(1)
    {
        uint8_t key = keyboard();
        bool reset_status = 0;
        bool reset_status_one = 0;
        switch (key)
        {
        case 0x3F:
            
            break;
        case 0x3E:
            keyboard_led(6, false);
            gpio_put(6, false);
            sleep_ms(250);
            gpio_put(6, true);
            while (keyboard() == 0x3E);
            
            for (uint8_t l = 0; l <= 6; l++)
            {
                keyboard_led(l, true);
                keyboard_led(0, true);
            }
            break;
        case 0xFF:
            reset_status = DeviceInit(DEV_ADDR_ADC3, adc_data_mass, 2);
            reset_status == true ? keyboard_led(5, false) : keyboard_led(5, true);
            break;
        case 0x1F:
            reset_status = DeviceInit(DEV_ADDR_ADC2, adc_data_mass, 2);
            reset_status == true ? keyboard_led(4, false) : keyboard_led(4, true);
            break;
        case 0x2F:
            reset_status = DeviceInit(DEV_ADDR_ADC1, adc_data_mass, 2);
            reset_status == true ? keyboard_led(3, false) : keyboard_led(3, true);
            break;
        case 0x37:
            reset_status = DeviceInit(DEV_ADDR_ADC0, adc_data_mass, 2);
            reset_status == true ? keyboard_led(2, false) : keyboard_led(2, true);
            break;
        case 0x3B:
            reset_status_one = DeviceInit(DEV_ADDR_DAC, dac_data_mass, 2);
            dac_data_mass[0] = 0x03;
            dac_data_mass[1] = 0xC0;
            reset_status_one = DeviceInit(DEV_ADDR_DAC, dac_data_mass, 2);
            dac_data_mass[0] = 0x02;
            dac_data_mass[1] = 0x80;
            reset_status_one = DeviceInit(DEV_ADDR_DAC, dac_data_mass, 2);
            reset_status_one == true ? keyboard_led(1, false) : keyboard_led(1, true);
            break;
        default:
            break;
        }
        printf("%02x\r\n", key);
    }
/*
    static uint8_t status = 0;
    uint8_t reset_status = DeviceInit(DEV_ADDR_ADC0, adc_data_mass, 2);
    status |= (reset_status << 0);
    reset_status = DeviceInit(DEV_ADDR_ADC1, adc_data_mass, 2);
    status |= (reset_status << 1);
    reset_status = DeviceInit(DEV_ADDR_ADC2, adc_data_mass, 2);
    status |= (reset_status << 2);
    reset_status = DeviceInit(DEV_ADDR_ADC3, adc_data_mass, 2);
    status |= (reset_status << 3);
    reset_status = DeviceInit(DEV_ADDR_DAC, dac_data_mass, 2);
    status |= (reset_status << 4);
    dac_data_mass[0] = 0x03;
    reset_status = DeviceInit(DEV_ADDR_DAC, dac_data_mass, 2);
    status |= (reset_status << 5);
    printf("%02x", status);
            printf("\r\n");
        while(1)
        {
            switch (status)
            {
            case 0b00000001:
                printf("Error %02x, ADC0 started, but any other device not", status);
                gpio_put(PICO_DEFAULT_LED_PIN, true);
                sleep_ms(75);
                gpio_put(PICO_DEFAULT_LED_PIN, false);
                sleep_ms(75);
                break;
            case 0b00000011:
                printf("Error %02x, ADC0 & ADC1 started, but any other device not", status);
                gpio_put(PICO_DEFAULT_LED_PIN, true);
                sleep_ms(125);
                gpio_put(PICO_DEFAULT_LED_PIN, false);
                sleep_ms(125);
                break;
            case 0b00000111:
                printf("Error %02x, ADC0 & ADC1 & ADC2 started, but any other device not", status);
                gpio_put(PICO_DEFAULT_LED_PIN, true);
                sleep_ms(250);
                gpio_put(PICO_DEFAULT_LED_PIN, false);
                sleep_ms(250);
                break;
            case 0b00001111:
                printf("Error %02x, ADC0 & ADC1 & ADC2 & ADC3 started, but any other device not", status);
                gpio_put(PICO_DEFAULT_LED_PIN, true);
                sleep_ms(500);
                gpio_put(PICO_DEFAULT_LED_PIN, false);
                sleep_ms(500);
                break;
            case 0b00011111:
                printf("Error %02x, ADC0 & ADC1 & ADC2 & ADC3 started, but DAC answer only reg 0x02", status);
                gpio_put(PICO_DEFAULT_LED_PIN, true);
                sleep_ms(1000);
                gpio_put(PICO_DEFAULT_LED_PIN, false);
                sleep_ms(1000);
                break;
            case 0b00111111:
                printf("Starting complete!");
                gpio_put(PICO_DEFAULT_LED_PIN, true);
                sleep_ms (1000);
                break;
            case 0b00110011:
                printf("ADC0 & ADC1 & DAC0 started, ADC2 & ADC3 not");
                gpio_put(PICO_DEFAULT_LED_PIN, true);
                sleep_ms (2500);
                gpio_put(PICO_DEFAULT_LED_PIN, false);
                sleep_ms (2500);
                break;
            
            default:
                printf("Error %02x, didn't init 0x3F - error device(s) in binary translation", status);
                sleep_ms (1000);
                break;
            }*/
        /*for (int addr = 0; addr < (1 << 7); ++addr) {
        if (addr % 16 == 0) {
            printf("%02x ", addr);
        }

        // Perform a 1-byte dummy read from the probe address. If a slave
        // acknowledges this address, the function returns the number of bytes
        // transferred. If the address byte is ignored, the function returns
        // -1.

        // Skip over any reserved addresses.
        int ret;
        uint8_t rxdata;
        if (reserved_addr(addr))
            ret = PICO_ERROR_GENERIC;
        else
            ret = i2c_read_blocking(i2c1, addr, &rxdata, 1, false);

        printf(ret < 0 ? "." : "0x%02x", addr);
        printf(addr % 16 == 15 ? "\n" : "  ");
        }
        printf("Done.\n");
        }*/
    /*i2c_write_blocking(i2c0, 0x4C, data_mass, 2, false);
    i2c_read_blocking(i2c0, 0x4C, out_data, 1, false);

    i2c_write_blocking(i2c0, 0x4C, data_mass, 2, false);
    i2c_read_blocking(i2c0, 0x4C, out_data, 1, false);
    
    i2c_write_blocking(i2c0, 0x4C, data_mass, 2, false);
    i2c_read_blocking(i2c0, 0x4C, out_data, 1, false);
    i2c_write_blocking(i2c0, 0x18, data_mass, 1, false);
    i2c_read_blocking(i2c0, 0x18, sout_data, 1, false);
    data_mass[0] = 0x02;
    data_mass[1] = 0xC0;
    sout_data[0] = 0x00;
    i2c_write_blocking(i2c0, 0x18, data_mass, 2, false);
    i2c_read_blocking(i2c0, 0x18, sout_data, 1, false);
    data_mass[0] = 0x03;
    sout_data[0] = 0x00;
    i2c_write_blocking(i2c0, 0x18, data_mass, 2, false);
    i2c_read_blocking(i2c0, 0x18, sout_data, 1, false);
    if (sout_data[0] == 0xC0 && out_data[0] == 0x8B)
    {
        sleep_ms(1000);
        gpio_put(PICO_DEFAULT_LED_PIN, true);
    }
    while (1) 
    {
        printf("%x", out_data[0]);
        printf("\r\n");
        printf("%x", sout_data[0]);
        printf("\r\n");
        sleep_ms(250);
    }*/
}







/*

*/