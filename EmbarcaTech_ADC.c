// Include libraries
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"     
#include "lib/font.h"
#include "lib/ssd1306.h"

// Define communication channels
#define BOTAO_A 5 // Button A pin
#define BOTAO_B 6 // Button B pin
#define LED_PIN_GREEN 11 // Green LED pin
#define LED_PIN_BLUE 12 // Blue LED pin
#define LED_PIN_RED 13  // Red LED pin
#define I2C_SDA 14 // SDA pin
#define I2C_SCL 15 // SCL pin
#define SW_PIN 22 // Joystick SW button pin
#define VRX_PIN 26 // Joystick X-axis pin
#define VRY_PIN 27 // Joystick Y-axis pin
#define I2C_PORT i2c1 // I2C port
#define display_address 0x3C // Display address

// Define global variables
uint actual_time = 0;
bool led_mode = true;
bool display_mode = true;
bool frame_mode = false;
ssd1306_t ssd;

// Initialize and set a GPIO pin for PWM
uint pwm_init_gpio(uint gpio, uint wrap) {
    gpio_set_function(gpio, GPIO_FUNC_PWM); // Set pin to PWM function
    uint slice_num = pwm_gpio_to_slice_num(gpio); // Get PWM slice number
    pwm_set_wrap(slice_num, wrap); // Set PWM wrap value
    pwm_set_enabled(slice_num, true); // Enable PWM
    return slice_num; // Return PWM slice number
}

// Button callback function (interrupt)
void callback_abtn(uint gpio, uint32_t events) {
    uint time = to_ms_since_boot(get_absolute_time());
    if (time - actual_time > 300) { // Debounce
        actual_time = time; // Update last action time
        if (gpio == BOTAO_A){
            led_mode = !led_mode; // Toggle LED mode
            printf("Joystick: %s\n", led_mode ? "Led Enabled" : "Led Disabled");
            if (!led_mode){
                pwm_set_gpio_level(LED_PIN_RED, 0); // Turn off Red LED
                pwm_set_gpio_level(LED_PIN_BLUE, 0); // Turn off Blue LED
                gpio_put(LED_PIN_GREEN, false); // Turn off Green LED
            }
        }
        
        if (gpio == BOTAO_B){
            display_mode = !display_mode; // Toggle display mode
            printf("Joystick: %s\n", display_mode ? "Display Enabled" : "Display Disabled");
            
            if (!display_mode){
                ssd1306_fill(&ssd, false); // Clear display
                ssd1306_send_data(&ssd);
            }
        }
        
        if (gpio == SW_PIN){   
            frame_mode = !frame_mode; // Toggle frame mode
            printf("Joystick: %s\n", frame_mode ? "Frame Mode Enabled" : "Frame Mode Disabled");
        }
    }
}

// Main function
int main(){
    stdio_init_all();
    adc_init();
    // Initialize and configure Green LED
    gpio_init(LED_PIN_GREEN);
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);
    gpio_put(LED_PIN_GREEN, false); 
    // Initialize and configure Red LED
    gpio_init(LED_PIN_RED);
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);
    gpio_put(LED_PIN_RED, false); 
    // Initialize and configure Blue LED
    gpio_init(LED_PIN_BLUE);
    gpio_set_dir(LED_PIN_BLUE, GPIO_OUT);
    gpio_put(LED_PIN_BLUE, false);
    // Inicialização e configuração do Botão A
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);
    // Inicialização e configuração do Botão B
    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_B);
    // Inicialização e configuração do Botão SW
    gpio_set_dir(SW_PIN, GPIO_IN);
    gpio_pull_up(SW_PIN); 
    // Inicialização dos pinos do ADC
    adc_gpio_init(VRX_PIN); 
    adc_gpio_init(VRY_PIN); 
    gpio_init(SW_PIN);
    
    //inicialização a comunição I2C
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    
    //Inicializa o display SSD1306
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, display_address, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    //inicializa os pinos dos leds para PWM
    uint pwm_wrap = 4096;  // Valor máximo para o contador PWM
    uint red_pwm_slice = pwm_init_gpio(LED_PIN_RED, pwm_wrap);  // Inicializa o LED vermelho para PWM
    uint blue_pwm_slice = pwm_init_gpio(LED_PIN_BLUE, pwm_wrap);  // Inicializa o LED azul para PWM
    
    bool cor = true; // Variável para controlar a cor do quadrado 
    
    uint16_t vrx_value; // Variável para armazenar o valor lido do eixo X do joystick
    uint16_t vry_value; // Variável para armazenar o valor lido do eixo Y do joystick
    int quadrado_x = 30;    // Posição inicial X do quadrado no display
    int quadrado_y = 30;    // Posição inicial Y do quadrado no display
    int quadrado_largura = 8; // Largura do quadrado no display
    int quadrado_altura = 8; // Altura do quadrado no display

    // Configura as interrupções para os botões A, B e SW
    gpio_set_irq_enabled_with_callback(SW_PIN, GPIO_IRQ_EDGE_FALL, true, &callback_abtn);
    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &callback_abtn);  
    gpio_set_irq_enabled_with_callback(BOTAO_B, GPIO_IRQ_EDGE_FALL, true, &callback_abtn);  

    // Loop principal do programa
    while (true) {
        adc_select_input(0); // Seleciona o canal 0 do ADC (pino VRX)
        vrx_value = adc_read(); // Lê o valor analógico do eixo X do joystick

        adc_select_input(1); // Seleciona o canal 1 do ADC (pino VRY)
        vry_value = adc_read(); // Lê o valor analógico do eixo Y do joystick
        
        bool sw_value = gpio_get(SW_PIN) == 0; // Verifica se o botão SW está pressionado
        if (led_mode) {
            int blue_level = 0; // Variável para armazenar o nível de brilho do LED azul
            int red_level = 0;  // Variável para armazenar o nível de brilho do LED vermelho
            
            // Mapeia os valores lidos do joystick para o nível de brilho dos LEDs
            if (vrx_value > 2400) {
                blue_level = vrx_value - 2400;   // Aumenta o brilho do LED vermelho se VRX > 2400
            } else if (vrx_value < 1700){
                blue_level = 1700 - vrx_value;   // Aumenta o brilho do LED vermelho se VRX < 1700
            }
            
            if (vry_value > 2400) {
                red_level = vry_value - 2400; // Aumenta o brilho do LED azul se VRY > 2400
            } else if (vry_value < 1700) {
                red_level = 1700 - vry_value; // Aumenta o brilho do LED azul se VRY < 1700
            }
             
            if (sw_value) {
                gpio_put(LED_PIN_GREEN, true); // Liga o LED verde
            } else {
                gpio_put(LED_PIN_GREEN, false); // Desliga o LED verde
            }

            // Define o nível de brilho dos LEDs vermelho e azul (PWM)
            pwm_set_gpio_level(LED_PIN_RED, red_level);
            pwm_set_gpio_level(LED_PIN_BLUE, blue_level);
        }
            
        if (display_mode){
                uint16_t vrx_value_inverter = 4095 - vrx_value; // Inverte o valor de VRX para controlar a posição do quadrado
                
                // Mapeia os valores lidos do joystick para a posição do quadrado no display
                quadrado_x = 30 + (vrx_value_inverter - 2048) / 100; // Ajuste o divisor para controlar a sensibilidade
                quadrado_y = 62 + (vry_value - 2048) / 35; // Ajuste o divisor para controlar a sensibilidade
                
                // Limites para o quadrado não sair da tela
            if (quadrado_x < 3) quadrado_x = 3;
            if (quadrado_x > 118 - quadrado_largura) quadrado_x = 118 - quadrado_largura;
            if (quadrado_y < 10) quadrado_y = 10;
            if (quadrado_y > 118 - quadrado_altura) quadrado_y = 118 - quadrado_altura;
            
            
            ssd1306_fill(&ssd, !cor); // Limpa o display
            if (frame_mode){
                ssd1306_rect(&ssd, 5, 5, 118, 56, cor, !cor); // Desenha um retângulo
                ssd1306_rect(&ssd, 7, 7, 114, 52, cor, !cor); // Desenha um retângulo
                ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor); // Desenha um retângulo
                ssd1306_rect(&ssd, quadrado_x, quadrado_y, quadrado_largura, quadrado_altura, cor, 0); // Desenha o quadrado sem preenchimento
            }else{
                ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor); // Desenha um retângulo
                ssd1306_rect(&ssd, quadrado_x, quadrado_y, quadrado_largura, quadrado_altura, cor, 1); // Desenha o quadrado preenchido
            }
            
            ssd1306_send_data(&ssd); // Atualiza o display
        }
            
            
            sleep_ms(100);
        }
        return 0;
    }
