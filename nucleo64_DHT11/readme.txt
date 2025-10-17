This application is for DHT 11 temperature and humidity sensor. It is made for stm32 Nucleo64-c031c6 card. Create a project for Nucleo64-c031c6.Firstly, clear all pins, set clock to 48Mhz, set "System Core"/RCC HSE-> High Speed Crystal/Ceramic Resonator. 
Open timer choose TIM1 make "Clock Source"-> "Internal Clock". Configure TIM1 parameters as Prescalar->47, Counter Mode->Up, Counter Period->65535.
Lastly open Connectivity/USART2 make mode as Asynchronous.
In the end save and generate the code. After that everything will be set up. Copy paste the code and use Terminal Emulator (tera term or similar) to see data send from uart.
Connect DHT11 pins correctly with stm32 board. In this project PA5 pin is set as signal. Ground and Power pin can be chosen freely. Look power ratings of your sensor from datasheet.
This project card supports Virtual COM Port. If your card does not have you must use UART to usb converter.
