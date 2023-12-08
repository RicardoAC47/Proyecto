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

    int k;
    // Genera palabras insertando cada letra del abecedario en todas las posiciones posibles
    for (k = 1; k < strlen(szPalabraLeida) + 1; k++) {
        clon[k] = szPalabraLeida[k - 1];
    }
    clon[k] = '\0';

    for (int i = 0; i < strlen(szPalabraLeida) + 1; i++) {
        for (int j = 0; j < LONGITUD; j++) {
            clon[i] = abecedario[j];
            strcpy_s(szPalabrasSugeridas[iNumSugeridas++], clon);
        }
        clon[i] = szPalabraLeida[i];
    }

    int contador = 0;
    // Genera palabras eliminando cada letra de la palabra original
    for (int i = 0; i < strlen(szPalabraLeida) && strlen(szPalabraLeida) != 1; i++) {
        for (int j = 0; j < strlen(szPalabraLeida); j++) {
            if (j != i)
                clon[contador++] = szPalabraLeida[j];
        }
        clon[contador] = '\0';
        strcpy_s(szPalabrasSugeridas[iNumSugeridas++], clon);
        strcpy_s(clon, szPalabraLeida);
        contador = 0;
    }
    // Genera palabras intercambiando letras adyacentes
    for (int i = 0; i < strlen(szPalabraLeida) - 1; i++) {
        clon[i] = szPalabraLeida[i + 1];
        clon[i + 1] = szPalabraLeida[i];
        strcpy_s(szPalabrasSugeridas[iNumSugeridas++], clon);
        strcpy_s(clon, szPalabraLeida);
    }
    strcpy_s(szPalabrasSugeridas[iNumSugeridas++], szPalabraLeida);

    // Ordena las palabras sugeridas alfabéticamente
    for (int j = 0; j < iNumSugeridas - 1; j++) {
        for (int i = j + 1; i < iNumSugeridas; i++) {
            if (strcmp(szPalabrasSugeridas[j], szPalabrasSugeridas[i]) > 0) {
                // Intercambia las palabras sugeridas
                char temp[TAMTOKEN];
                strcpy_s(temp, szPalabrasSugeridas[j]);
                strcpy_s(szPalabrasSugeridas[j], szPalabrasSugeridas[i]);
                strcpy_s(szPalabrasSugeridas[i], temp);
            }
        }
    }
}
/*****************************************************************************************************************
    ListaCandidatas: Esta funcion recupera desde el diccionario las palabras validas y su peso
    Regresa las palabras ordenadas por su peso
    char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
    int		iNumSugeridas,						//Lista de palabras clonadas
    char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
    int		iEstadisticas[],					//Lista de las frecuencias de las palabras
    int		iNumElementos,						//Numero de elementos en el diccionario
    char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
    int		iPeso[],							//Peso de las palabras en la lista final
    int &	iNumLista)							//Numero de elementos en la szListaFinal
******************************************************************************************************************/

// Función para obtener una lista final de palabras sugeridas y sus pesos
void ListaCandidatas(char szPalabrasSugeridas[][TAMTOKEN], int iNumSugeridas, char szPalabras[][TAMTOKEN],
    int iEstadisticas[],
    int iNumElementos, char szListaFinal[][TAMTOKEN], int iPeso[], int& iNumLista) {
    iNumLista = 0;
    // Itera sobre las palabras sugeridas y las compara con el diccionario
    for (int i = 0; i < iNumSugeridas; i++) {
        for (int j = 0; j < iNumElementos; j++) {
            // Compara la palabra sugerida con las palabras del diccionario
            if (strcmp(szPalabrasSugeridas[i], szPalabras[j]) == 0) {
                bool band = false;
                // Verifica si la palabra ya está en la lista final
                for (int k = 0; k < iNumLista && !band; k++)
                    if (strcmp(szListaFinal[k], szPalabras[j]) == 0)
                        band = true;

                if (band) continue;

                // Agrega la palabra sugerida a la lista final
                strcpy_s(szListaFinal[iNumLista], szPalabrasSugeridas[i]);
                iPeso[iNumLista++] = iEstadisticas[j];
            }
        }
    }

    // Ordena la lista final por pesos en orden descendente
    for (int i = 0; i < iNumLista; i++) {
        for (int j = 0; j < iNumLista - 1; j++) {
            if (iPeso[j] < iPeso[j + 1]) {
                // Intercambia las palabras y los pesos correspondientes
                int iaux;
                char caux[TAMTOKEN];

                strcpy_s(caux, szListaFinal[j + 1]);
                iaux = iPeso[j + 1];

                strcpy_s(szListaFinal[j + 1], szListaFinal[j]);
                iPeso[j + 1] = iPeso[j];

                strcpy_s(szListaFinal[j], caux);
                iPeso[j] = iaux;
            }
        }
    }
}