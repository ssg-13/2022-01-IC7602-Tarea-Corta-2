# Tarea Corta 2

Servidor para cálculo de subredes IPv4.

## Descripción

Esta tarea implementa un pequeño servidor utilizando sockets e hilos en lenguaje de programación C, que realiza ciertos cálculos sobre direcciones IPv4 haciendo uso de operadores bitwise. Se utiliza Docker y Docker compose, de manera que la configuración está completamente automatizada. 

## Cómo comenzar

### Dependencias

* La aplicación fue desarrollada y probada para sistema operativo Linux.
* Se debe contar con una instalación de Git en su sistema. Para instalar Git puede consultar la guía oficial disponible en GitHub [aquí](https://github.com/git-guides/install-git).
* Se debe contar con una instalación de Docker en su sistema. Instrucciones para la instalación:
```
# Preparar el repositorio

sudo apt-get update

sudo apt-get install \
    ca-certificates \
    curl \
    gnupg \
    lsb-release

curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg

echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu \
  $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null

# Instalar Docker Engine

sudo apt-get update

sudo apt-get install docker-ce docker-ce-cli containerd.io docker-compose-plugin

# Verificar instalación

sudo docker run hello-world
```
* Se debe contar con una instalación de Docker compose en su sistema. Instrucciones para la instalación:
```
# Descargar archivo ejecutable

sudo curl -L "https://github.com/docker/compose/releases/download/1.26.0/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose

# Otorgar permisos de ejecución

sudo chmod +x /usr/local/bin/docker-compose

# Verificar instalación

docker-compose --version
```

### Instalación

* Descargar o clonar repositorio en su computadora.

```
git clone git@github.com:ssg-13/2022-01-IC7602-Tarea-Corta-2.git 
```

### Ejecución del programa

Dentro de la carpeta del repositorio:

* Construir las imágenes utilizando Docker compose. 
```
sudo docker-compose build 
```

* Levantar los contenedores utilizando Docker compose. 
```
sudo docker-compose up -d  
```
### Uso del servidor

```
GET BROADCAST IP {dirección IP} MASK {mascara en formato /bits o notación X.X.X.X}
```

Por ejemplo:
  1. **GET BROADCAST IP 10.8.2.5 MASK /29**
  a. Retorna 10.8.2.7
  2. **GET BROADCAST IP 172.16.0.56 MASK 255.255.255.128**
  a. Retorna 172.16.0.127

```
GET NETWORK NUMBER IP {dirección IP} MASK {mascara en formato /bits o notación X.X.X.X}
```

Por ejemplo:
  1. **GET NETWORK NUMBER IP 10.8.2.5 MASK /29**
  a. Retorna 10.8.2.0
  2. **GET NETWORK NUMBER IP 172.16.0.56 MASK 255.255.255.128**
  a. Retorna 172.16.0.0

```
GET HOSTS RANGE IP {dirección IP} MASK {mascara en formato /bits o notación X.X.X.X}
```

Por ejemplo:
  1. **GET HOSTS RANGE IP 10.8.2.5 MASK /29**
  a. hosts range: [10.8.2.1 - 10.8.2.6]
  2. **GET HOSTS RANGE IP 172.16.0.56 MASK 255.255.255.128**
  a. Retorna hosts range: [172.16.0.1 - 172.16.0.126]

### Conectar con el servidor

Abrir una terminal y ejecutar:

```
telnet localhost 9666
```

## Pruebas realizadas

### Prueba conexión con el servidor

Se verifica que el servidor establezca conexión con el cliente utilizando el comando ***telnet***. Se verifica también la conexión desde más de un cliente simultaneamente.

#### Prueba conexión desde un cliente

```
telnet localhost 9666
```

![conexión con un cliente](/server/img/conexion1.png "conexión con un cliente")

#### Prueba conexión desde más de un cliente

```
telnet localhost 9666
```

![conexión con más un cliente](/server/img/conexion2.png "conexión con más un cliente")

### Prueba GET BROADCAST

Se verifica que el servidor responda correctamente a la solicitud GET BROADCAST.

#### Prueba con máscara en formato bits

```
telnet localhost 9666
GET BROADCAST IP 10.8.2.5 MASK /29
```

![Prueba GET BROADCAST](/server/img/broadcast1.png "Prueba GET BROADCAST")

#### Prueba con máscara en notación X.X.X.X

```
telnet localhost 9666
GET BROADCAST IP 172.16.0.56 MASK 255.255.255.128
```

![Prueba GET BROADCAST](/server/img/broadcast2.png "Prueba GET BROADCAST")

### Prueba GET NETWORK NUMBER

Se verifica que el servidor responda correctamente a la solicitud GET NETWORK NUMBER.

#### Prueba con máscara en formato bits

```
telnet localhost 9666
GET NETWORK NUMBER IP 10.8.2.5 MASK /29
```

![Prueba GET NETWORK NUMBER](/server/img/network1.png "Prueba GET NETWORK NUMBER")

#### Prueba con máscara en notación X.X.X.X

```
telnet localhost 9666
GET NETWORK NUMBER IP 172.16.0.56 MASK 255.255.255.128
```

![Prueba GET NETWORK NUMBER](/server/img/network2.png "Prueba GET NETWORK NUMBER")

### Prueba GET HOSTS RANGE

Se verifica que el servidor responda correctamente a la solicitud GET HOSTS RANGE.

#### Prueba con máscara en formato bits

```
telnet localhost 9666
GET HOSTS RANGE IP 10.8.2.5 MASK /29
```

![Prueba GET HOSTS RANGE](/server/img/hosts1.png "Prueba GET HOSTS RANGE")

#### Prueba con máscara en notación X.X.X.X

```
telnet localhost 9666
GET HOSTS RANGE IP 172.16.0.56 MASK 255.255.255.128
```

![Prueba GET HOSTS RANGE](/server/img/hosts2.png "Prueba GET HOSTS RANGE")

## Autores
 
[Sebastián Solórzano](https://github.com/ssg-13)  

## Historial de versiones

* 0.1
    * Versión inicial

## Reconocimientos

* [Bitwise Operations and Subnetting](https://neverthenetwork.com/notes/bitwise_subnetting)
* [Multithreaded server in C](https://github.com/Gurpremm/Multithreaded-Echo-Client-Server-in-C/blob/master/server-socket.c)