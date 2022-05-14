
///////////////////////////////////////
/*TODO:
        Validar ip y máscara.*/
///////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

// Funciones del programa
void *connection_handler(void *);
char *int_to_dotted_decimal(int ip);
unsigned int dotted_decimal_to_int(char ip[]);
unsigned int cidr_to_mask(unsigned int cidrValue);
unsigned int get_cidr_value(char cidr[]);
unsigned int calc_network_address(unsigned int ipaddress, unsigned int netmask);
unsigned int calc_broadcast(unsigned int network, unsigned int netmask);
int get_function(char str[]);
char *get_broadcast(char str[]);
char *get_network_number(char str[]);
char *get_hosts_range(char str[]);
unsigned int get_ip(char str[]);
unsigned int get_mask(char str[]);
int isValidIpAddress(char *ipAddress);

// Función principal
int main()
{
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1)
    {
        perror("Error  al inicializar el socket");
        exit(EXIT_FAILURE);
    }
    else
        printf("Socket creado correctamente\n");

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9666);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Se enlaza el socket a la IP y puerto especificados
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
    {
        printf("Error al enlazar el socket\n");
        exit(0);
    }
    else
        printf("Socket enlazado correctamente\n");

    if (listen(server_socket, 10) != 0)
    {
        printf("Error al escuchar\n");
        exit(0);
    }
    else
        printf("Servidor escuchando...\n");

    int no_threads = 0;
    pthread_t threads[10];
    while (no_threads < 10)
    {
        int client_socket = accept(server_socket, NULL, NULL);
        if (pthread_create(&threads[no_threads], NULL, connection_handler, &client_socket) < 0)
        {
            perror("Error al crear el hilo");
            return 1;
        }
        if (client_socket < 0)
        {
            printf("Error al aceptar conexión\n");
            exit(0);
        }
        else
            printf("Conexión aceptada\n");
        no_threads++;
    }
    int k = 0;
    for (k = 0; k < 10; k++)
    {
        pthread_join(threads[k], NULL);
    }

    close(server_socket);

    return 0;
}

// Función que maneja las peticiones
void *connection_handler(void *client_socket)
{
    int socket = *(int *)client_socket;
    int read_len;
    char server_message[1000] = "\nUSO DEL SERVIDOR:\n- GET BROADCAST IP {dirección IP} MASK {mascara en formato /bits o notación X.X.X.X}\n    Por ejemplo:\n        1. GET BROADCAST IP 10.8.2.5 MASK /29\n        2. GET BROADCAST IP 172.16.0.56 MASK 255.255.255.128\n- GET NETWORK NUMBER IP {dirección IP} MASK {mascara en formato /bits o notación X.X.X.X}\n    Por ejemplo:\n        1. GET NETWORK NUMBER IP 10.8.2.5 MASK /29\n        2. GET NETWORK NUMBER IP 172.16.0.56 MASK 255.255.255.128\n- GET HOSTS RANGE IP {dirección IP} MASK {mascara en formato /bits o notación X.X.X.X}\n    Por ejemplo:\n        1. GET HOSTS RANGE IP 10.8.2.5 MASK /29\n        2. GET HOSTS RANGE IP 172.16.0.56 MASK 255.255.255.128\n\n";
    int send_status;
    send_status = send(socket, server_message, sizeof(server_message), 0);
    char client_message[100];
    char return_message[100];
    while ((read_len = recv(socket, client_message, 100, 0)) > 0)
    {
        client_message[read_len] = '\0';

        switch (get_function(client_message))
        {
        case 1: // Solicitud GET BROADCAST
            sprintf(return_message, "%s\n\n", get_broadcast(client_message));
            break;

        case 2: // Solicitud GET NETWORK NUMBER
            sprintf(return_message, "%s\n\n", get_network_number(client_message));
            break;

        case 3: // Solicitud GET HOSTS RANGE
            sprintf(return_message, "%s\n\n", get_hosts_range(client_message));
            break;

        default:
            sprintf(return_message, "%s\n\n", "No se reconoce la función solicitada. Revise la sintaxis de la solicitud");
        }

        // Envia respuesta al cliente
        send_status = send(socket, return_message, strlen(return_message), 0);
    }

    return 0;
}

// Función que comprueba cuál tipo de solicitud se recibió
int get_function(char str[])
{
    char match_broadcast[] = "GET BROADCAST";
    char match_network_number[] = "GET NETWORK NUMBER";
    char match_hosts_range[] = "GET HOSTS RANGE";

    if (strstr(str, match_broadcast) != NULL)
    {
        return 1;
    }
    else if (strstr(str, match_network_number) != NULL)
    {
        return 2;
    }
    else if (strstr(str, match_hosts_range) != NULL)
    {
        return 3;
    }
    else
    {
        return -1;
    }
}

// Función para obtener el broadcast
char *get_broadcast(char str[])
{
    unsigned int ip = get_ip(str);
    if (ip == -1)
    {
        char *error = malloc(sizeof(char) * 50);
        error = "Error al extraer IP. Revise la sintaxis de la solicitud";

        return error;
    }
    else if (ip == -2)
    {
        char *error = malloc(sizeof(char) * 50);
        error = "Formato de IP no valido. Revise la sintaxis de la IP";

        return error;
    }
    unsigned int mask = get_mask(str);
    if (mask == -1)
    {
        char *error = malloc(sizeof(char) * 50);
        error = "Error al extraer MASK. Revise la sintaxis de la solicitud";

        return error;
    }

    unsigned int network_number = calc_network_address(ip, mask);
    unsigned int broadcast = calc_broadcast(network_number, mask);
    char *broadcast_s = malloc(sizeof(char) * 50);
    broadcast_s = int_to_dotted_decimal(broadcast);

    return broadcast_s;
}

// Función para obtener el número de red
char *get_network_number(char str[])
{
    unsigned int ip = get_ip(str);
    if (ip == -1)
    {
        char *error = malloc(sizeof(char) * 50);
        error = "Error al extraer IP. Revise la sintaxis de la solicitud";

        return error;
    }
    else if (ip == -2)
    {
        char *error = malloc(sizeof(char) * 50);
        error = "Formato de IP no valido. Revise la sintaxis de la IP";

        return error;
    }
    unsigned int mask = get_mask(str);
    if (mask == -1)
    {
        char *error = malloc(sizeof(char) * 50);
        error = "Error al extraer MASK. Revise la sintaxis de la solicitud";

        return error;
    }

    unsigned int network_number = calc_network_address(ip, mask);
    char *network_number_s = malloc(sizeof(char) * 50);
    network_number_s = int_to_dotted_decimal(network_number);

    return network_number_s;
}

// Función para obtener el rango de los hosts
char *get_hosts_range(char str[])
{
    unsigned int ip = get_ip(str);
    if (ip == -1)
    {
        char *error = malloc(sizeof(char) * 50);
        error = "Error al extraer IP. Revise la sintaxis de la solicitud";

        return error;
    }
    else if (ip == -2)
    {
        char *error = malloc(sizeof(char) * 50);
        error = "Formato de IP no valido. Revise la sintaxis de la IP";

        return error;
    }
    unsigned int mask = get_mask(str);
    if (mask == -1)
    {
        char *error = malloc(sizeof(char) * 50);
        error = "Error al extraer MASK. Revise la sintaxis de la solicitud";

        return error;
    }

    unsigned int network_number = calc_network_address(ip, mask);
    unsigned int broadcast = calc_broadcast(network_number, mask);

    char *first_host = int_to_dotted_decimal(network_number + 1);
    char *last_host = int_to_dotted_decimal(broadcast - 1);

    char *hosts_range = malloc(sizeof(char) * 50);
    sprintf(hosts_range, "hosts range: [%s - %s]", first_host, last_host);

    return hosts_range;
}

// Función que extrae la ip de la solicitud
unsigned int get_ip(char str[])
{
    char str_cpy[100];
    strcpy(str_cpy, str);
    char search_ip[] = "IP";
    char delim[] = " \t\r\n\v\f";
    unsigned int ip_value;

    char *ip_start = strstr(str_cpy, search_ip);

    if (ip_start != NULL)
    {
        char *ip_flag = strtok(ip_start, delim);
        char *ip = strtok(NULL, delim);
        printf("'%s'\n", ip);

        if (isValidIpAddress(ip) == 0)
        {
            return -2;
        }

        ip_value = dotted_decimal_to_int(ip);
    }
    else
    {
        return -1;
    }

    return ip_value;
}

// Función que extrae la máscara de la solicitud
unsigned int get_mask(char str[])
{
    char str_cpy[100];
    strcpy(str_cpy, str);
    char search_mask[] = "MASK";
    char delim[] = " \t\r\n\v\f";
    unsigned int mask_value;

    char *mask_start = strstr(str_cpy, search_mask);

    if (mask_start != NULL)
    {
        char *mask_flag = strtok(mask_start, delim);
        char *mask = strtok(NULL, delim);
        printf("'%s'\n", mask);

        if (strchr(mask, '/') != NULL)
        {
            unsigned int cidr_value = get_cidr_value(mask);
            mask_value = cidr_to_mask(cidr_value);
        }
        else
        {
            mask_value = dotted_decimal_to_int(mask);
        }
    }
    else
    {
        return -1;
    }

    return mask_value;
}

// Función que convierte la máscara de bits de la ip a formato X.X.X.X
char *int_to_dotted_decimal(int ip)
{

    char *ip_s = malloc(sizeof(char) * 50);

    unsigned char bytes[4];

    // set the byte to the first byte of the ip & 0xFF masks the higher bytes
    bytes[0] = ip;
    bytes[1] = ip >> 8; // right shift by a byte and repeat
    bytes[2] = ip >> 16;
    bytes[3] = ip >> 24;

    sprintf(ip_s, "%d.%d.%d.%d", bytes[3], bytes[2], bytes[1], bytes[0]);

    return ip_s;
}

// Función que convierte la ip de formato X.X.X.X a máscara de bits
unsigned int dotted_decimal_to_int(char ip[])
{

    unsigned char bytes[4] = {0};

    sscanf(ip, "%hhd.%hhd.%hhd.%hhd", &bytes[3], &bytes[2], &bytes[1], &bytes[0]);

    // set 1 byte at a time by left shifting and ORing
    return bytes[0] | bytes[1] << 8 | bytes[2] << 16 | bytes[3] << 24;
}

// Función que convierte la máscara de red de formato bits a formato X.X.X.X
unsigned int cidr_to_mask(unsigned int cidrValue)
{

    // left shift 1 by 32 - cidr, subtract 1 from the result and XORing
    // it with a mask that has all bits set, yeilds the subnet mask
    return -1 ^ ((1 << (32 - cidrValue)) - 1);
}

// Función que extrae el valor cidr de la solicitud
unsigned int get_cidr_value(char cidr[])
{

    unsigned int cidrValue;

    sscanf(cidr, "/%u", &cidrValue);

    return cidrValue;
}

// Función que calcula el número de red mediante un AND lógico
unsigned int calc_network_address(unsigned int ipaddress, unsigned int netmask)
{
    return ipaddress & netmask;
}

// Función que calcula el broadcast mediante sumando al número de red la negación de la máscara de red
unsigned int calc_broadcast(unsigned int network, unsigned int netmask)
{
    return network + (~netmask);
}

// Función para validar ip
int isValidIpAddress(char *ipAddress)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
    return result;
}
