/*****************************************************************************************************************
    UNIVERSIDAD NACIONAL AUTONOMA DE MEXICO
    FACULTAD DE ESTUDIOS SUPERIORES -ARAGON-

    Computadoras y programacion.
    (c) Ponga su nombre y numero de cuenta aqui.

    Quiso decir: Programa principal de la aplicacion de la distancia de Levenstein.

******************************************************************************************************************/



#include "stdafx.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "corrector.h"

#define LONGITUD 32
char abecedario[LONGITUD + 7] = "abcdefghijklmnñopqrstuvwxyzáéíóú";


//Funciones publicas del proyecto
/*****************************************************************************************************************
    DICCIONARIO: Esta funcion crea el diccionario completo
    char *	szNombre				:	Nombre del archivo de donde se sacaran las palabras del diccionario
    char	szPalabras[][TAMTOKEN]	:	Arreglo con las palabras completas del diccionario
    int		iEstadisticas[]			:	Arreglo con el numero de veces que aparecen las palabras en el diccionario
    int &	iNumElementos			:	Numero de elementos en el diccionario
******************************************************************************************************************/


// Función para construir un diccionario a partir de un archivo
void Diccionario(char* szNombre, char szPalabras[][TAMTOKEN], int iEstadisticas[], int& iNumElementos) {
    FILE* fp;
    iNumElementos = 0;

    int i;
    char lectura, array[TAMTOKEN];

    for (i = 0; i < NUMPALABRAS; i++)
        iEstadisticas[i] = 0;

    fopen_s(&fp, szNombre, "r");

    int contador = 0;

    char puntuacion[] = " \t\n\r.,;() ";

    if (fp == NULL)
        return;

    while ((lectura = fgetc(fp)) != EOF) {
        bool bandera = true;

        lectura = tolower(lectura);

        for (i = 0; i < strlen(puntuacion); i++)
            if (puntuacion[i] == lectura)
                bandera = false;

        if (contador < TAMTOKEN && bandera) {
            array[contador++] = lectura;
            continue;
        }
        else if (contador == 0)
            continue;
        else if (contador == 1 && bandera) {
            contador = 0;
            continue;
        }

        array[contador] = '\0';

        for (i = 0; i < iNumElementos && !bandera; i++) {
            if (strcmp(szPalabras[i], array) == 0) {
                iEstadisticas[i]++;
                bandera = true;
            }
        }

        if (!bandera) {
            strcpy_s(szPalabras[iNumElementos], array);
            iEstadisticas[iNumElementos++]++;
        }

        contador = 0;
    }

    fclose(fp);

    // Ordena el diccionario alfabéticamente
    for (int j = 0; j < iNumElementos - 1; j++) {
        for (i = j + 1; i < iNumElementos; i++) {
            if (strcmp(szPalabras[j], szPalabras[i]) > 0) {
                // Intercambia las palabras y las estadísticas correspondientes
                char temp[TAMTOKEN];
                strcpy_s(temp, szPalabras[j]);
                strcpy_s(szPalabras[j], szPalabras[i]);
                strcpy_s(szPalabras[i], temp);

                int tempEstadistica = iEstadisticas[j];
                iEstadisticas[j] = iEstadisticas[i];
                iEstadisticas[i] = tempEstadistica;
            }
        }
    }
}
/*****************************************************************************************************************
    ClonaPalabras: toma una palabra y obtiene todas las combinaciones y permutaciones requeridas por el metodo
    char *	szPalabraLeida,						//Palabra a clonar   palabra
    char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
    int &	iNumSugeridas)						//Numero de elementos en la lista
******************************************************************************************************************/

// Función para generar palabras sugeridas a partir de una palabra dada
void ClonaPalabras(char* szPalabraLeida, char szPalabrasSugeridas[][TAMTOKEN], int& iNumSugeridas) {
    char clon[TAMTOKEN];
    iNumSugeridas = 0;
    strcpy_s(clon, szPalabraLeida);

    // Genera palabras reemplazando cada letra con todas las letras del abecedario
    for (int i = 0; i < strlen(szPalabraLeida); i++) {
        for (int j = 0; j < LONGITUD; j++) {
            clon[i] = abecedario[j];
            strcpy_s(szPalabrasSugeridas[iNumSugeridas++], clon);
        }
        clon[i] = szPalabraLeida[i];
    }

