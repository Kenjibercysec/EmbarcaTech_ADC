# EmbarcaTech_ADC

Este projeto é projetado para o Raspberry Pi Pico W e demonstra o uso de vários periféricos de hardware, incluindo ADC, GPIO, I2C e PWM. O projeto inclui um joystick, botões, LEDs e um display OLED SSD1306.

## Funcionalidades

- Controle do joystick para LEDs e display
- Botão A para alternar o modo LED
- Botão B para alternar o modo display
- Botão SW do joystick para alternar o modo de moldura no display
- Controle de PWM para brilho do LED
- Comunicação I2C com display OLED SSD1306

## Configuração de Hardware

- **Botão A**: Conectado ao GPIO 5
- **Botão B**: Conectado ao GPIO 6
- **LED Verde**: Conectado ao GPIO 11
- **LED Azul**: Conectado ao GPIO 12
- **LED Vermelho**: Conectado ao GPIO 13
- **I2C SDA**: Conectado ao GPIO 14
- **I2C SCL**: Conectado ao GPIO 15
- **Botão SW do Joystick**: Conectado ao GPIO 22
- **Eixo X do Joystick**: Conectado ao GPIO 26
- **Eixo Y do Joystick**: Conectado ao GPIO 27
- **Display OLED SSD1306**: Endereço I2C 0x3C

## Configuração de Software

1. Clone o repositório:
    ```sh
    git clone https://github.com/seuusuario/EmbarcaTech_ADC.git
    cd EmbarcaTech_ADC
    ```

2. Inicialize o Pico SDK:
    ```sh
    git submodule update --init
    ```

3. Crie um diretório de build e navegue até ele:
    ```sh
    mkdir build
    cd build
    ```

4. Configure o projeto com CMake:
    ```sh
    cmake ..
    ```

5. Compile o projeto usando Ninja:
    ```sh
    ninja
    ```

6. Carregue o firmware no seu Raspberry Pi Pico W:
    ```sh
    cp EmbarcaTech_ADC.uf2 /media/seuusuario/RPI-RP2/
    ```

## Uso

- Pressione **Botão A** para alternar o modo LED.
- Pressione **Botão B** para alternar o modo display.
- Pressione o **Botão SW do Joystick** para alternar o modo de moldura no display.
- Mova o joystick para controlar o brilho dos LEDs e a posição do quadrado no display.

## Vídeo de Teste

Assista ao vídeo de teste [aqui](https://youtube.com/shorts/6fg_SOUc9wM?feature=share).

