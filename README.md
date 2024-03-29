# Diseño de un controlador basado en lógica difusa e Internet de las Cosas 
## Proyecto Capstone desarrollado en la Universidad Veracruzana, Facultad de Ingeniería en Electrónica y Comunicaciones

Elaborado por:
|Nombre   |Actividad   |
| ------------ | ------------ |
|Antonio del Angel Moreno Carbajal   |Desarrollo del sistema con Internet de las cosas   |
|Jose Roman García Martínez    |Diseño e implementación del controlador de lógica difusa   |
|Arturo Marrero Mendez   |Documentación del proyecto   |
 
### Objetivo general:
Prestar el servicio de una planta lineal basado en Lógica Difusa e Internet de las Cosas dentro de la institución, con posibilidad de extender el servicio a investigadores, docentes e industria privada. 

### Objetivo específico: 
1. Construcción de una planta lineal con un motor de corriente directa, encoder y una plataforma móvil.
2. Diseño e implementación del algoritmo para controlar la planta lineal basado en la Lógica Difusa.
3. Diseño de una interfaz en Grafana para monitoreo de la planta utilizando el protocolo de comunicación UART.
4. Diseño de la interfaz para el ingreso de datos en Node-RED.


### Equipo y material utilizado en el proyecto
1. Computadora/Laptop
2. Arduino Mega 2560
3. Motor de corriente directa (24 VDC)
4. Encoder de cuadratura
5. Modulo Puente H Bts7960
6. Cables Dupont M-M y H-M
7. Sensor de corriente de efecto Hall

### Software utilizado
1. Arduino IDE
2. Node-RED
3. XAMPP
4. Grafana

### Instalación de programas

A continuación se enlistan los programas utilizados para desarrollar este proyecto, así como los enlaces para instalarlos en su computadora\laptop.

#### 1.- Instalación de Arduino
Arduino ofrece hardware y software libre, lo que permite desarrollar código en su plataforma, compartirlo en la comunidad y programar las tarjetas de desarrollo que ofrece dicha marca. Descarga Arduino IDE: https://www.arduino.cc/en/software

Nota: es necesario contar con un Arduino Mega 2560, debido a que el código está enfocado en los recursos que tiene esta tarjeta.

#### 2.- Instalación de Node-RED
Node-RED es una herramienta de programación para conectar dispositivos de hardware, API y servicios en línea. Si desea profundizar más en esta herramienta siga el siguiente enlace: https://nodered.org/ 

Para la instalación en Windows diríjase al siguiente enlace que detalla los comandos y herramientas a descargar para correr Node-RED en Windows: https://nodered.org/docs/getting-started/windows 

Para instalar en otros Sistemas Operativos: https://nodered.org/docs/getting-started/local

#### 3.- Instalación de XAMPP
XAMPP es una distribución de Apache completamente gratuita y fácil de instalar que contiene MariaDB, PHP y Perl. El paquete de instalación de XAMPP ha sido diseñado para ser fácil de instalar y usar. Consulte el siguiente enlace: https://www.apachefriends.org/es/index.html

#### 4.- Instalación de Grafana en Windows
Grafana es un software libre basado en licencia de Apache 2.0, que permite la visualización y el formato de datos métricos. Permite crear cuadros de mando y gráficos a partir de múltiples fuentes. Revise la página oficial de Grafana: https://grafana.com/oss/grafana/

Para instalar Grafana en su computadora\laptop diríjase a la sección de "Descargas", seleccione el sistema operativo en el que desea instalar Grafana. Cabe mencionar que, para este proyecto se instalará en Windows 11. Te recomiendo seguir los pasos de instalación que se muestran en el siguiente video:
https://www.youtube.com/watch?v=grppSMHLueA&t=87s&ab_channel=TechGuruTech


### Desarrollo

A continuación se muestra la estructura general del proyecto:

![Estructura General](https://raw.githubusercontent.com/antomoreno21/ProyectoCapston_UVPR/main/Estructura%20del%20proyecto%20capstone.png)

Imagen de la planta: 

![Planta lineal](https://raw.githubusercontent.com/antomoreno21/ProyectoCapston_UVPR/main/Planta%20lineal.png)

Instalados los programas, visualizando la planta lineal y analizada la estructura general se procede al desarrollo de este proyecto.
1. Abrir en su computadora Arduino IDE y cargar al Arduino Mega el siguiente código: [Codigo para el Arduino Mega](https://github.com/antomoreno21/ProyectoCapston_UVPR/blob/main/Codigo%20del%20Proyecto%20Capstone/Codigo%20del%20Proyecto%20Capstone.ino "Codigo para el Arduino Mega")
2. Conectar el encoder a Vcc (5 Volts) y GND
3. Conectar las salidas del encoder a los pines 18 y 19 del Arduino.
4. Los pines 12 y 13 del Arduino Mega van conectados al Puente H en su pin de control correspondiente (RPWM y LPWM).
5. Alimentar el puente H con 5 V y GND en los pines Vcc y GND (terminales macho). Además, conecte los pines R_EN y L_EN a Vcc.
6. En la bornera del puente H, conecte la fuente de alimentación para el motor en B+ y B- (es importante que la fuente sea independiente y externa al arduino, de lo contrario podría dañar partes de su computadora).
7. De la bornera del paso anterior, conecte los cables de alimentación del motor a M+ y M-.

Hasta el momento, tiene la estructura fisica necesaria. Si requiere una prueba de que el encoder y el código están funcionando, conecte el arduino a la computadora y abra el monitor serial, en el cual observará las lecturas del encoder. 

Los siguientes pasos corresponden a la configuración de Node-RED:

8. Abra Node-Red desde la terminal escribiendo "node-red"
9. Abra el navegador de internet y coloque lo siguiente: localhost:1880
10. Realizados los pasos anteriores, diríjase al menu principal ubicado en la parte superior izquierda, de clic y seleccione la opción "Import".
11. Copie el siguiente código: [Codigo para Node-RED](https://github.com/antomoreno21/ProyectoCapston_UVPR/blob/main/Interfaz%20en%20Node-Red/Flow%20en%20Node-RED.json "Codigo para Node-RED").
12. Pegue el código en la ventana emergente del paso 10 y de en Import ubicado en la parte inferior izquierda.
13. Clic en Deploy.
14. Para abrir la interfaz de control en Node-RED, diríjase a la opción de Dashboard y despliegue la interfaz en una nueva ventana del navegador. En dicha ventana podrá modificar la posición de la base móvil que tiene la planta.

Continuamos con la integración de la base de datos. Para esto utilizaremos XAMPP.

15. Abrir XAMPP en la computadora\laptop.
16. Dar clic en el botón "Start" que corresponde al servicio de Apache y MySQL. 
17. Presionar el botón "Admin" correspondiente al servicio de MySQL para abrir el gestor phpMyAdmin en el navegador y con ello generar la base de datos.
18. En la interfaz grafica de phpMyAdmin, generar una base de datos con el nombre de "proyectocapstone".
19. Generar una tabla en la base de datos anterior con el nombre "variablesdelproyecto" y agregar 5 campos con los siguientes nombres y tipos: time(timestamp-ON UPDATE CURRENT_TIMESTAMP()),  distancia(float), posicion(float), error(float) y u(float).

Con esto finaliza la creación de la base de datos, ahora configuramos la interfaz Grafica en Grafana:

20. Importe el código de la interfaz grafica a través del archivo con extensión .json o copie y pegue el contenido del archivo: [Interfaz en Grafana](https://github.com/antomoreno21/ProyectoCapston_UVPR/tree/main/Interfaz%20en%20Grafana "Interfaz en Grafana").
21. Descargado el archivo del paso 15 o el contenido del mismo en el portapapeles, diríjase a una ventana nueva del navegador y coloque lo siguiente: localhost:3000. Esto permitirá abrir la interfaz de Grafana. Es importante que ya cuente con su usuario y contraseña, de lo contrario configure estos parámetros.
22. Ubíquese en la pantalla principal de Grafana, dé clic en símbolo "+" y seleccione la opción Import. Cargue el archivo del paso anterior.
23. La interfaz de Grafana deberá desplegarse.

Para finalizar, tenga abierto Node-RED, Grafana, el Arduino Mega conectado a la computadora, así como las fuentes de alimentación con sus respectivos voltajes. Con todo esto podrá monitorear, analizar y manipular la planta lineal del proyecto.

### Resultados
El algoritmo basado en Lógica Difusa embebido en el Arduino Mega 2560 funciono de forma adecuada para controlar la planta lineal, así como era de esperar que Node-RED y Grafana permitieran el control y monitoreo de dicha planta. A continuación se muestra el comportamiento del dispositivo graficando cada variable de control de manera individual:

![Grafica individual](https://raw.githubusercontent.com/antomoreno21/ProyectoCapston_UVPR/main/Grafica%20individual.png)

A continuación se muestra una gráfica general con todas las variables de control:

![Grafica General](https://raw.githubusercontent.com/antomoreno21/ProyectoCapston_UVPR/main/Grafica%20general.png)

*Nota: Las gráficas anteriores se obtuvieron con la planta sin carga.
