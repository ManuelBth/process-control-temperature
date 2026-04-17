Este proyecto en base al Arduino nano esp32

Debe haber una estructura de datos principal que sea la única fuente de la verdad.
Los datos importantes que estarán en dicha estructura son:

- Constantes de control Kp, Ki, Kd, Referencia.
- Estado: manual o control-pid.
- Tiempo transcurrido en el PID.
- Variables: temperaturaPt100, PWM_Resistencia, PWM_Fan1, PWM_Fan2.
- Información del Bluetooth.

El sistema debe de usar RTOS, puesto que por un lado tenemos que tomar datos en muy poco tiempo, procesarlos en un JSON y enviarlos por Bluetooth.

Lo que propongo son 4 tareas principales.

1. Leer sensores
2. Aplicar los controles necesarios, en este caso son PWM o On/Off
3. Parsear la información y gestionarlar y mostrar cirta información en la LCD.
4. Enviar y recibir los datos por Bluetooth

Esto para aplicar un principio de separación de responsabilidades y puesto que se está hablando de tomar datos en muy poco tiempo, con más razón
