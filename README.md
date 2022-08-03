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

### Objetivo especifico: 
1. Construcción de una planta lineal con un motor de corriente directa, encoder y una plataforma móvil.
2. Diseño e implementación del algoritmo para controlar la planta lineal basado en la Lógica Difusa.
3. Diseño de una interfaz en Grafana para monitoreo de la planta utilizando el protocolo de comunicación UART.
4. Diseño de la interfaz para el ingreso de datos en Node-RED.

### Introducción

### Equipo y material utilizado en el proyecto
1. Computadora\Laptop
2. Arduino Mega 2560
3. Motor de corriente directa (24 VDC)
4. Encoder de cuadratura
5. Modulo Puente H Bts7960
6. Cables Dupont M-M y H-M

### Software utilizado
1. Arduino IDE
2. Node-RED
3. XAMPP
4. Grafana

### Instalación de programas

A continuación se enlistan los programas utilizados para desarrollar este proyecto, así como los enlaces para instalarlos en su computadora\laptop.

#### 1.- Instalación de Arduino
Arduino ofrece hardware y software libre, lo que permite desarrollar código en su plataforma, compartirlo en la comunidad y programar las tarjetas de desarrollo que ofrece dicha marca. Descarga Arduino IDE: https://www.arduino.cc/en/software

Nota: es necesario contar con un Arduino Mega 2560, debido a que el código esta enfocado en los recursos que tiene esta tarjeta.

#### 2.- Instalación de Node-RED
Node-RED es una herramienta de programación para conectar dispositivos de hardware, API y servicios en línea. Si desea profundizar más en esta herramienta siga el siguiente enlace: https://nodered.org/ 

Para la instalación en windows dirigase al siguiente enlace que detalla los comandos y herramientas a descargar para correr Node-RED en windows: https://nodered.org/docs/getting-started/windows 

Para instalar en otros Sistemas Operativos: https://nodered.org/docs/getting-started/local

#### 3.- Instalación de XAMPP
XAMPP es una distribución de Apache completamente gratuita y fácil de instalar que contiene MariaDB, PHP y Perl. El paquete de instalación de XAMPP ha sido diseñado para ser fácil de instalar y usar. Consulte el siguiente enlace: https://www.apachefriends.org/es/index.html

#### 4.- Instalación de Grafana en Windows
Grafana es un software libre basado en licencia de Apache 2.0, que permite la visualización y el formato de datos métricos. Permite crear cuadros de mando y gráficos a partir de múltiples fuentes. Revise la pagina oficial de Grafana: https://grafana.com/oss/grafana/

Para instalar Grafana en su computadora\laptop dirigase a la sección de "Descargas", seleccione el sistema operativo en el que desea instalar Grafana. Cabe mencionar que, para este proyecto se instalará en Windows 11. Te recomiendo seguir los pasos de instalación que se muestran en el siguiente video:
https://www.youtube.com/watch?v=grppSMHLueA&t=87s&ab_channel=TechGuruTech


### Desarrollo

A continuación se muestra la estructura general del proyecto:

![Estructura General](https://raw.githubusercontent.com/antomoreno21/ProyectoCapston_UVPR/main/Estructura%20del%20proyecto%20capstone.png)

Imagen de la planta: 

![Planta lineal](https://raw.githubusercontent.com/antomoreno21/ProyectoCapston_UVPR/main/Planta%20lineal.png)

Instalados los programas, visualizando la planta lineal y analizada la estructura general se procede al desarrollo de este proyecto.
1. Abrir en su computadora Arduino IDE y cargar al Arduino Mega el siguiente código: ![](https://img.shields.io/github/stars/pandao/editor.md.svg)[Código para el Arduino Mega]
2. 

### Resultados
El algoritmo basado en Logica Difusa embebido en el Arduino Mega 2560 funciono de forma adecuada para controlar la planta lineal, así como era de esperar que Node-RED y Grafana permitieran el control y monitoreo de dicha planta. A continuación se muestra el comportamiento del dispositivo graficando cada variable de control de manera individual:

![Grafica individual](https://raw.githubusercontent.com/antomoreno21/ProyectoCapston_UVPR/main/Grafica%20individual.png)

A continuación se muestra una grafica general con todas las variables de control:

![Grafica General](https://raw.githubusercontent.com/antomoreno21/ProyectoCapston_UVPR/main/Grafica%20general.png)

*Nota: Las graficas anteriores se obtuvieron con la planta sin carga.
