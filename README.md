# Diseño de un controlador basado en lógica difusa e Internet de las Cosas 
## Proyecto Capstone desarrollado en la Universidad Veracruzana, Facultad de Ingeniería en Electrónica y Comunicaciones

Elaborado por:
|Nombre   |Actividad   |
| ------------ | ------------ |
|Antonio del Angel Moreno Carbajal   |Desarrollo del sistema con Internet de las cosas   |
|Jose Roman García Martínez    |Diseño e implementación del controlador de lógica difusa   |
|Arturo Marrero Mendez   |Documentación del proyecto   |
 
### Objetivo general:
Proveer el servicio de una planta lineal basado en lógica difusa e Internet de las Cosas dentro de la institución, con posibilidad de extender el servicio a investigadores, docentes e industria privada. 

### Objetivo especifico: 
1. Construcción de una planta lineal con un motor de corriente directa, encoder y una plataforma móvil.
2. Diseño e implementación del algoritmo para controlar la planta lineal basado en la lógica difusa.
3. Diseño de una interfaz en Node-RED para la gestión, monitoreo y control de la planta utilizando el protocolo de comunicación MQTT y UART.

### Introducción

### Equipo y material utilizado en el proyecto
1. Raspberry pi 4 modelo de 8 GB
2. Arduino Mega 
3. Motor de corriente directa
4. Encoder 

### Software utilizado
1. Arduino IDE
2. Node-RED
3. Grafana

### Instalación de programas

A continuación se enlistan los programas utilizados para desarrollar este proyecto así como los enlaces para instalarlos en su computadora.

#### 1.- Instalación de Arduino
Arduino ofrece hardware y software libre, lo que permite desarrollar código en su plataforma, compartirlo en la comunidad y programar las tarjetas de desarrollo que ofrece dicha marca. Descarga Arduino IDE: https://www.arduino.cc/en/software

Nota: es necesario contar con un Arduino Mega 2560, debido a que el código esta enfocado en los recursos que tiene esta tarjeta.

#### 2.- Instalación de Node-RED
Node-RED es una herramienta de programación para conectar dispositivos de hardware, API y servicios en línea. Si desea profundizar más en esta herramienta siga el siguiente enlace: https://nodered.org/ 

Para la instalación en windows dirigase al siguiente enlace que detalla los comandos y herramientas a descargar para correr Node-RED en windows: https://nodered.org/docs/getting-started/windows 

Para instalar en otros Sistemas Operativos: https://nodered.org/docs/getting-started/local

#### 3.- Instalación de XAMPP
XAMPP es una distribución de Apache completamente gratuita y fácil de instalar que contiene MariaDB, PHP y Perl. El paquete de instalación de XAMPP ha sido diseñado para ser fácil de instalar y usar.
https://www.apachefriends.org/es/index.html

#### 4.- Instalación de Grafana en Windows
Revise la pagina oficial de Grafana: https://grafana.com/oss/grafana/

Dirigase a la sección de Descargas, seleccione el sistema operativo en el que desea instalar Grafana. Para este proyecto se instalará en Windows 11.
Siga los pasos de instalación que se muestran en el siguiente video:
https://www.youtube.com/watch?v=grppSMHLueA&t=87s&ab_channel=TechGuruTech

### Desarrollo

Imagen de la planta: https://uvmx-my.sharepoint.com/:f:/g/personal/antomoreno_uv_mx/EvuCFqBQq-1BlQ6VlcwIY0QBnQhaIY9Wz3qtjyPNQhQS2A?e=hPQkug
