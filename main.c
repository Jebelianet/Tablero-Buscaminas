#include <SFML/Graphics.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

#define numFilas 16
#define numColumnas 16
#define tamanoCelda 36
#define numMinas 40

//Declaracion de variables globales
int anchoVentana = numColumnas * tamanoCelda;
int altoVentana = (numFilas + 2.6) * tamanoCelda;
int minas[numFilas][numColumnas];
int minasCercanas[numFilas][numColumnas];
int casillasDestapadas[numFilas * numColumnas];
int casillasMarcadas[numFilas * numColumnas];
sfSprite* celdas[numFilas][numColumnas];
sfTexture* textureCeldaAbierta;
sfSprite* mina;
sfTexture* textureMina;
sfVector2u dimensionesImagenMina;
int posicionesMinas[numMinas][2];
int filaUsuario, columnaUsuario;
int mouseButtonPressed = 0;
int primerMovimiento = 1;
int banderasRestantes = numMinas;
int segundos = 0;
time_t inicio;

// Prototipo de la función destaparCasillasAdyacentes
void destaparCasillasAdyacentes(int fila, int columna);

// Declarar la función generarMinas
void generarMinas(int filaUsuario, int columnaUsuario);

//Prototipo de la función mostrarMensaje
void mostrarMensaje(sfRenderWindow* window, const char* mensaje, sfColor color);

//Prototipo de la función todasCasillasDestapadas
int todasCasillasDestapadas();

//Prototipo de la función jugadorPerdio
int jugadorPerdio();

//Prototipo de la función manejarDerrota
void manejarDerrota(sfRenderWindow* window);

//Prototipo de la función manejarVictoria
void manejarVictoria(sfRenderWindow* window);

int i,j;
int main() {

    // Inicializar textureCeldaAbierta
    textureCeldaAbierta = sfTexture_createFromFile("CeldaBuscaminasAbierta2.png", NULL);

    // Crear la ventana
    sfRenderWindow* window;
    sfVideoMode mode = {anchoVentana, altoVentana, 32};
    window = sfRenderWindow_create(mode, "Buscaminas", sfResize | sfClose, NULL);

    //Crear ventana para el mapa de minas
    sfRenderWindow* ventanaMapa;

    // Cargar las texturas del sprite para las celdas
    sfTexture* textureCeldas = sfTexture_createFromFile("CeldaBuscaminas.png", NULL);
    sfTexture* textureBandera = sfTexture_createFromFile("Bandera.png", NULL);

    // Cargar la textura del sprite para la mina
    sfTexture* textureMina = sfTexture_createFromFile("mina.png", NULL);

    // Obtener las dimensiones de la imagen de la mina
    sfVector2u dimensionesImagenMina = sfTexture_getSize(textureMina);

    // Crear un sprite para la mina
    sfSprite* mina = sfSprite_create();
    sfSprite_setTexture(mina, textureMina, sfTrue);

    // Generar las casillas
    sfText* textos[numFilas][numColumnas];
    sfFont* font = sfFont_createFromFile("LVDCGO__.TTF");

    for (int i = 0; i < numFilas; i++) {
        for (int j = 0; j < numColumnas; j++) {
            celdas[i][j] = sfSprite_create();
            sfSprite_setTexture(celdas[i][j], textureCeldas, sfTrue);
            sfSprite_setPosition(celdas[i][j], (sfVector2f){j * tamanoCelda, (i + 2.6) * tamanoCelda});

            // Crear el texto para cada celda
            textos[i][j] = sfText_create();
            sfText_setFont(textos[i][j], font);
            sfText_setCharacterSize(textos[i][j], 12);
            sfText_setFillColor(textos[i][j], sfBlue);
            sfText_setString(textos[i][j], "1");
            sfText_setPosition(textos[i][j], (sfVector2f){j * tamanoCelda + 8, (i + 2.8) * tamanoCelda + 4});
        }
    }

    // Cargar la textura del sprite para la barra superior
    sfTexture* textureBarra = sfTexture_createFromFile("BarraSuperior.png", NULL);

    // Crear un sprite para la barra superior
    sfSprite* barraSuperior = sfSprite_create();
    sfSprite_setTexture(barraSuperior, textureBarra, sfTrue);
    sfSprite_setScale(barraSuperior, (sfVector2f){anchoVentana / (float)sfTexture_getSize(textureBarra).x, 1});
    sfSprite_setPosition(barraSuperior, (sfVector2f){0, 0});

    //Ajustar tamaño del cronómetro con respecto al tamaño de la ventana
    int tamanoCronometro = sqrt(anchoVentana * (tamanoCelda - 4) * 6) / 20;

    //Asignarle un tamaño mínimo al cronómetro (Para evitar que se haga demasiado pequeño)
    int tamanoMinimo = 20;
    if (tamanoCronometro < tamanoMinimo) {
        tamanoCronometro = tamanoMinimo;
    }

    //Ajustar posición del cronómetro al tamaño de la ventana
    int posX = anchoVentana / 15;
    int posY = (2.6 * tamanoCelda) / 2 - tamanoCronometro / 2;

    // Crear el cronómetro
    sfText* cronometro = sfText_create();
    sfText_setFont(cronometro, font);
    sfText_setCharacterSize(cronometro, tamanoCronometro);
    sfText_setFillColor(cronometro, sfRed);
    sfText_setPosition(cronometro, (sfVector2f){posX, posY});
    sfText_setOutlineThickness(cronometro, 1);
    sfText_setOutlineColor(cronometro, sfBlack);

    // Inicializando el cronómetro
    time_t inicio = time(NULL);
    int segundos = 0;

    // Crear un rectángulo para representar la posición actual del usuario
    sfRectangleShape* userRect = sfRectangleShape_create();
    sfRectangleShape_setSize(userRect, (sfVector2f){tamanoCelda, tamanoCelda});
    sfRectangleShape_setFillColor(userRect, sfTransparent);
    sfRectangleShape_setOutlineThickness(userRect, 2);
    sfRectangleShape_setOutlineColor(userRect, sfRed);

    // Crear el contador de banderas
    sfText* contadorBanderas = sfText_create();
    sfText_setFont(contadorBanderas, font);
    sfText_setCharacterSize(contadorBanderas, tamanoCronometro);
    sfText_setFillColor(contadorBanderas, sfRed);
    sfText_setOutlineThickness(contadorBanderas, 1);
    sfText_setOutlineColor(contadorBanderas, sfBlack);

    int filaUsuario = 0; // Fila inicial del usuario
    int columnaUsuario = 0; // Columna inicial del usuario

    // Bucle principal del juego
    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }

            // Evento de teclado
            if (event.type == sfEvtKeyPressed) {
                // Restablecer la opacidad del rectángulo del usuario
                sfRectangleShape_setOutlineColor(userRect, sfRed);

                switch (event.key.code) {
                    case sfKeyUp:
                        filaUsuario = (filaUsuario - 1 + numFilas) % numFilas;
                        break;
                    case sfKeyDown:
                        filaUsuario = (filaUsuario + 1) % numFilas;
                        break;
                    case sfKeyLeft:
                        columnaUsuario = (columnaUsuario - 1 + numColumnas) % numColumnas;
                        break;
                    case sfKeyRight:
                        columnaUsuario = (columnaUsuario + 1) % numColumnas;
                        break;
                    case sfKeyHome:
                        filaUsuario = (filaUsuario - 1 + numFilas) % numFilas;
                        columnaUsuario = (columnaUsuario - 1 + numColumnas) % numColumnas;
                        break;
                    case sfKeyEnd:
                        filaUsuario = (filaUsuario + 1) % numFilas;
                        columnaUsuario = (columnaUsuario - 1 + numColumnas) % numColumnas;
                        break;
                    case sfKeyPageUp:
                        filaUsuario = (filaUsuario - 1 + numFilas) % numFilas;
                        columnaUsuario = (columnaUsuario + 1) % numColumnas;
                        break;
                    case sfKeyPageDown:
                        filaUsuario = (filaUsuario + 1) % numFilas;
                        columnaUsuario = (columnaUsuario + 1) % numColumnas;
                        break;
                    case sfKeySpace:
                       if (casillasDestapadas[filaUsuario * numColumnas + columnaUsuario] == 0) {
                            casillasMarcadas[filaUsuario * numColumnas + columnaUsuario] = !casillasMarcadas[filaUsuario * numColumnas + columnaUsuario];
                        if (casillasMarcadas[filaUsuario * numColumnas + columnaUsuario]) {
                            sfSprite_setTexture(celdas[filaUsuario][columnaUsuario], textureBandera, sfTrue);
                            banderasRestantes--; // Disminuir el contador de banderas
                        } else {
                            sfSprite_setTexture(celdas[filaUsuario][columnaUsuario], textureCeldas, sfTrue);
                            banderasRestantes++; // Aumentar el contador de banderas
                                }
                            }
                        break;
                    case sfKeyEnter:
                        if (primerMovimiento) {
                            primerMovimiento = 0;
                            generarMinas(filaUsuario, columnaUsuario);
                        }
                        // Destapar las casillas
                        destaparCasillasAdyacentes(filaUsuario, columnaUsuario);

                        if (todasCasillasDestapadas()) {
                        manejarVictoria(window);
                        } else if (jugadorPerdio()) {
                        manejarDerrota(window);
                    }
                        break;
                    case sfKeyEscape:
                        sfRenderWindow_close(window);
                        break;
                    case sfKeyF1:
                        reiniciarJuego();
                        break;
                    case sfKeyF2:
                    // Crear una ventana para mostrar el mapa de minas
                    ventanaMapa = sfRenderWindow_create(mode, "Mapa de Minas", sfResize | sfClose, NULL);

                    // Dibujar la barra superior
                    sfRenderWindow_drawSprite(ventanaMapa, barraSuperior, NULL);

                    // Dibujar el tablero de las casillas de CeldasBuscaminas
                        for (int i = 0; i < numFilas; i++) {
                            for (int j = 0; j < numColumnas; j++) {
                            sfSprite_setPosition(celdas[i][j], (sfVector2f){j * tamanoCelda, (i + 2.6) * tamanoCelda});
                            sfRenderWindow_drawSprite(ventanaMapa, celdas[i][j], NULL);
                        }
                    }

                    // Dibujar las minas aleatorias
                        for (int i = 0; i < numFilas; i++) {
                            for (int j = 0; j < numColumnas; j++) {
                        if (minas[i][j] == 1) {
                            // Obtener las dimensiones de la casilla destapada
                            sfVector2u dimensionesCasilla = sfTexture_getSize(textureCeldas);

                            // Redimensionar la imagen de la mina para que se ajuste a las dimensiones de la casilla destapada
                            sfVector2f escalaMina;
                            escalaMina.x = (float)dimensionesCasilla.x / dimensionesImagenMina.x;
                            escalaMina.y = (float)dimensionesCasilla.y / dimensionesImagenMina.y;
                            sfSprite_setScale(mina, escalaMina);

                            sfSprite_setPosition(mina, (sfVector2f){j * tamanoCelda, (i + 2.6) * tamanoCelda});
                            sfRenderWindow_drawSprite(ventanaMapa, mina, NULL);
                        }
                    }
                }

                // Mostrar la ventana
                sfRenderWindow_display(ventanaMapa);

                // Esperar a que el usuario cierre la ventana
                while (sfRenderWindow_isOpen(ventanaMapa)) {
                sfEvent eventMapa;
                while (sfRenderWindow_pollEvent(ventanaMapa, &eventMapa)) {
                    if (eventMapa.type == sfEvtClosed) {
                sfRenderWindow_close(ventanaMapa);
                }
            }
        }

                // Cerrar la ventana
                sfRenderWindow_destroy(ventanaMapa);
                        break;
                    default:
                        break;
                }
            }
        }

        // Evento de botón del mouse
        if (event.type == sfEvtMouseButtonPressed) {
    if (!mouseButtonPressed) { // Solo ejecutar si el botón del mouse no estaba presionado antes
        mouseButtonPressed = 1;

        // Obtener las coordenadas del mouse
        sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);

        // Convertir las coordenadas del mouse a las coordenadas de la cuadrícula del tablero
        int fila = (mousePos.y - 2.6 * tamanoCelda) / tamanoCelda;
        int columna = mousePos.x / tamanoCelda;

        // Verificar si la casilla está dentro de los límites del tablero
        if (fila >= 0 && fila < numFilas && columna >= 0 && columna < numColumnas) {
            if (event.mouseButton.button == sfMouseLeft) {
                // Destapar la casilla con el clic izquierdo del mouse
                if (primerMovimiento) {
                    primerMovimiento = 0;
                    generarMinas(fila, columna);
                }
                destaparCasillasAdyacentes(fila, columna);
            } else if (event.mouseButton.button == sfMouseRight) {
                // Marcar o desmarcar la casilla con el clic derecho del mouse
                if (banderasRestantes > 0 || casillasMarcadas[fila * numColumnas + columna]) {
                    casillasMarcadas[fila * numColumnas + columna] = !casillasMarcadas[fila * numColumnas + columna];
                    if (casillasMarcadas[fila * numColumnas + columna]) {
                        sfSprite_setTexture(celdas[fila][columna], textureBandera, sfTrue);
                        banderasRestantes--; // Disminuir el contador de banderas
                    } else {
                        sfSprite_setTexture(celdas[fila][columna], textureCeldas, sfTrue);
                        banderasRestantes++; // Aumentar el contador de banderas
                    }
                }
            }

            // Verificar victoria o derrota después de una acción del mouse
            if (todasCasillasDestapadas()) {
                manejarVictoria(window); // Llama a la función que maneja la victoria
            } else if (jugadorPerdio()) {
                manejarDerrota(window); // Llama a la función que maneja la derrota
            }
        }
    }
    // Hacer que el marcador del teclado desaparezca cuando se hace clic en una casilla
    sfRectangleShape_setOutlineColor(userRect, sfTransparent);
} else if (event.type == sfEvtMouseButtonReleased) {
    mouseButtonPressed = 0; // Resetear la variable cuando se suelta el botón del mouse
}
        // Actualizar la representación visual de las casillas
        for (int i = 0; i < numFilas; i++) {
            for (int j = 0; j < numColumnas; j++) {
                if (casillasDestapadas[i * numColumnas + j] == 1) {
                    sfSprite_setTexture(celdas[i][j], textureCeldaAbierta, sfTrue);
                } else if (casillasMarcadas[i * numColumnas + j]) {
                    sfSprite_setTexture(celdas[i][j], textureBandera, sfTrue);

                    // Ajustar tamaño del sprite de la bandera
                    sfVector2u banderaSize = sfTexture_getSize(textureBandera);
                    float scaleX = (float)tamanoCelda / banderaSize.x;
                    float scaleY = (float)tamanoCelda / banderaSize.y;
                    sfSprite_setScale(celdas[i][j], (sfVector2f){scaleX, scaleY});
                } else {
                    sfSprite_setTexture(celdas[i][j], textureCeldas, sfTrue);
                    sfSprite_setScale(celdas[i][j], (sfVector2f){1, 1}); // Restaurar escala a (1, 1)
                }
            }
        }

        // Actualizar la posición del rectángulo del usuario
        sfRectangleShape_setPosition(userRect, (sfVector2f){columnaUsuario * tamanoCelda, (filaUsuario + 2.6) * tamanoCelda});

        // Actualizar el cronómetro
        time_t ahora = time(NULL);
        segundos = (int)difftime(ahora, inicio);
        int minutos = segundos / 60;
        int segundos_transcurridos = segundos % 60;

        char tiempo_str[10];
        snprintf(tiempo_str, sizeof(tiempo_str), "%02d:%02d", minutos, segundos_transcurridos);
        sfText_setString(cronometro, tiempo_str);

        // Limpiar la ventana
        sfRenderWindow_clear(window, sfBlack);

        // Dibujar el sprite de la barra superior
        sfRenderWindow_drawSprite(window, barraSuperior, NULL);

        // Dibujar los sprites de las celdas
        for (int i = 0; i < numFilas; i++) {
            for (int j = 0; j < numColumnas; j++) {
                sfRenderWindow_drawSprite(window, celdas[i][j], NULL);
                // Dibujar el texto "1" si la celda está abierta y es una de las posiciones aleatorias
                if (sfSprite_getTexture(celdas[i][j]) == textureCeldaAbierta) {
                    if (minas[i][j] == 0) {
                        switch(minasCercanas[i][j]){
                            case 1:sfText_setFillColor(textos[i][j], sfColor_fromRGB(0, 0, 255));
                            break;
                            case 2:sfText_setFillColor(textos[i][j], sfColor_fromRGB(53,142,53));
                            break;
                            case 3: sfText_setFillColor(textos[i][j], sfRed);
                            break;
                            case 4:sfText_setFillColor(textos[i][j], sfColor_fromRGB(0,0,123));
                            break;
                            case 5:sfText_setFillColor(textos[i][j], sfColor_fromRGB(128,0,0));
                            break;
                        }
                        char numeroMinasCercanas[2];
                        if(minasCercanas[i][j]!=0){
                            snprintf(numeroMinasCercanas, sizeof(numeroMinasCercanas), "%d", minasCercanas[i][j]);
                            sfText_setString(textos[i][j], numeroMinasCercanas);
                            sfRenderWindow_drawText(window, textos[i][j], NULL);
                        }
                    }
                    else{
                        // Dibujar la mina si la celda está abierta y es una de las posiciones aleatorias de las minas
                        for (int l = 0; l < numMinas; l++) {
                            if (posicionesMinas[l][0] == i && posicionesMinas[l][1] == j) {

                                // Obtener las dimensiones de la casilla destapada
                                sfVector2u dimensionesCasilla = sfTexture_getSize(textureCeldaAbierta);

                                // Redimensionar la imagen de la mina para que se ajuste a las dimensiones de la casilla destapada
                                sfVector2f escalaMina;
                                escalaMina.x = (float)dimensionesCasilla.x / dimensionesImagenMina.x;
                                escalaMina.y = (float)dimensionesCasilla.y / dimensionesImagenMina.y;
                                sfSprite_setScale(mina, escalaMina);

                                sfSprite_setPosition(mina, (sfVector2f){j * tamanoCelda, (i + 2.6) * tamanoCelda});
                                sfRenderWindow_drawSprite(window, mina, NULL);
                                break;
                            }
                        }
                    }
                }
            }
        }

    // Dibujar el rectángulo del usuario
    sfRenderWindow_drawRectangleShape(window, userRect, NULL);

    // Dibujar el cronómetro
    sfRenderWindow_drawText(window, cronometro, NULL);

    // Actualizar y dibujar el contador de banderas
    char banderas_str[10];
    snprintf(banderas_str, sizeof(banderas_str), "%d", banderasRestantes);
    sfText_setString(contadorBanderas, banderas_str);

    // Obtener el ancho del texto del contador
    sfFloatRect rect = sfText_getLocalBounds(contadorBanderas);
    float anchoTexto = rect.width;

    // Calcular la posición x del contador de banderas
    float posXContador = anchoVentana - anchoTexto - 45;

    // Establecer la posición del contador de banderas
    sfText_setPosition(contadorBanderas, (sfVector2f){posXContador, posY});

    sfRenderWindow_drawText(window, contadorBanderas, NULL);

    sfRenderWindow_display(window);

    }

    // Liberar memoria asignada con calloc
    free(casillasDestapadas);
    free(casillasMarcadas);

    // Limpiar los sprites y la fuente
    sfRectangleShape_destroy(userRect);
    for (int i = 0; i < numFilas; i++) {
        for (int j = 0; j < numColumnas; j++) {
            sfSprite_destroy(celdas[i][j]);
        }
    }
    sfTexture_destroy(textureCeldas);
    sfTexture_destroy(textureCeldaAbierta);
    sfTexture_destroy(textureBandera);
    sfTexture_destroy(textureBarra);
    sfSprite_destroy(barraSuperior);
    sfText_destroy(cronometro);
    sfText_destroy(contadorBanderas);
    sfFont_destroy(font);
    sfRenderWindow_destroy(window);

    return 0;
}

void destaparCasillasAdyacentes(int fila, int columna) {
    if (fila < 0 || fila >= numFilas || columna < 0 || columna >= numColumnas) return;
    if (casillasDestapadas[fila * numColumnas + columna] || casillasMarcadas[fila * numColumnas + columna]) return;

    casillasDestapadas[fila * numColumnas + columna] = 1;
    sfSprite_setTexture(celdas[fila][columna], textureCeldaAbierta, sfTrue);

    if (minasCercanas[fila][columna] == 0 && minas[fila][columna] == 0) {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx != 0 || dy != 0) destaparCasillasAdyacentes(fila + dx, columna + dy);
            }
        }
    }
}


void generarMinas(int filaUsuario, int columnaUsuario) {
    // Generar las minas después del primer movimiento
for (int k = 0; k < numMinas; k++) {
    int fila, columna;
    do {
        fila = rand() % numFilas;
        columna = rand() % numColumnas;
        // Verificar que la posición no se repita y no sea la primera casilla abierta ni una casilla adyacente a la primera casilla abierta
        int repetida = 0;
        for (int l = 0; l < k; l++) {
            if ((posicionesMinas[l][0] == fila && posicionesMinas[l][1] == columna) || (abs(fila - filaUsuario) <= 1 && abs(columna - columnaUsuario) <= 1)) {
                repetida = 1;
                break;
            }
        }
        if (!repetida) {
            posicionesMinas[k][0] = fila;
            posicionesMinas[k][1] = columna;
            break;
        }
    } while (1);
}

    // Generar las minas en las posiciones aleatorias
    for (int l = 0; l < numMinas; l++) {
        minas[posicionesMinas[l][0]][posicionesMinas[l][1]] = 1;
    }

    // Actualizar las minas cercanas
    for (int i = 0; i < numFilas; i++) {
        for (int j = 0; j < numColumnas; j++) {
            if (minas[i][j] == 0) {
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        int ni = i + dx;
                        int nj = j + dy;
                        if (ni >= 0 && ni < numFilas && nj >= 0 && nj < numColumnas && minas[ni][nj] == 1) {
                            minasCercanas[i][j]++;
                        }
                    }
                }
            }
        }
    }
}

// Función para mostrar un mensaje en la ventana
void mostrarMensaje(sfRenderWindow* window, const char* mensaje, sfColor textColor) {
    sfRectangleShape* fondo = sfRectangleShape_create();
    sfRectangleShape_setFillColor(fondo, sfColor_fromRGBA(0, 0, 0, 128)); // Color negro con 50% de opacidad
    sfRectangleShape_setSize(fondo, (sfVector2f){anchoVentana, altoVentana});
    sfRectangleShape_setPosition(fondo, (sfVector2f){0, 0});

    sfText* texto = sfText_create();
    sfFont* font = sfFont_createFromFile("LVDCGO__.TTF");
    sfText_setFont(texto, font);
    sfText_setCharacterSize(texto, 22);
    sfText_setFillColor(texto, textColor);
    sfText_setOutlineThickness(texto, 1);
    sfText_setString(texto, mensaje);

    // Centrar el texto en la ventana
    sfFloatRect rect = sfText_getLocalBounds(texto);
    sfText_setOrigin(texto, (sfVector2f){rect.left + rect.width / 2, rect.top + rect.height / 2});
    sfText_setPosition(texto, (sfVector2f){anchoVentana / 2, altoVentana / 2});

    // Dibujar el fondo y el texto
    sfRenderWindow_drawRectangleShape(window, fondo, NULL);
    sfRenderWindow_drawText(window, texto, NULL);
    sfRenderWindow_display(window);

    // Liberar memoria
    sfRectangleShape_destroy(fondo);
    sfText_destroy(texto);
    sfFont_destroy(font);
}

// Función para reiniciar el juego
void reiniciarJuego() {
    // Restablecer todas las variables (casillasDestapadas, casillasMarcadas, etc.)
    for (int i = 0; i < numFilas; i++) {
        for (int j = 0; j < numColumnas; j++) {
            casillasDestapadas[i * numColumnas + j] = 0;
            casillasMarcadas[i * numColumnas + j] = 0;
            minasCercanas[i][j] = 0;
        }
    }

    // Volver a generar las minas
    generarMinas(filaUsuario, columnaUsuario);

    // Restablecer variables
    primerMovimiento = 1; // Reiniciar el indicador del primer movimiento
    banderasRestantes = numMinas; // Restablecer el contador de banderas
    inicio = time(NULL);
    memset(minas, 0, sizeof(minas));
    memset(minasCercanas, 0, sizeof(minasCercanas));
    memset(casillasDestapadas, 0, sizeof(casillasDestapadas));
    memset(casillasMarcadas, 0, sizeof(casillasMarcadas));

    // Restablecer el cronómetro
    time_t inicio = time(NULL);
    segundos = 0;
}

void revelarMinas(sfRenderWindow* window) {
    // Dibujar las minas aleatorias
    for (int i = 0; i < numFilas; i++) {
        for (int j = 0; j < numColumnas; j++) {
            if (minas[i][j] == 1) {
                // Obtener las dimensiones de la casilla destapada
                sfVector2u dimensionesCasilla = sfTexture_getSize(textureCeldaAbierta);

                // Redimensionar la imagen de la mina para que se ajuste a las dimensiones de la casilla destapada
                sfVector2f escalaMina;
                escalaMina.x = (float)dimensionesCasilla.x / dimensionesImagenMina.x;
                escalaMina.y = (float)dimensionesCasilla.y / dimensionesImagenMina.y;
                sfSprite_setScale(mina, escalaMina);

                sfSprite_setPosition(mina, (sfVector2f){j * tamanoCelda, (i + 2.6) * tamanoCelda});
                sfRenderWindow_drawSprite(window, mina, NULL);
            }
        }
    }
}

// Función para verificar si el jugador ha perdido
int jugadorPerdio() {
    for (int i = 0; i < numFilas; i++) {
        for (int j = 0; j < numColumnas; j++) {
            if (minas[i][j] == 1 && casillasDestapadas[i * numColumnas + j] == 1) {
                return 1; // El jugador ha perdido
            }
        }
    }
    return 0; // El jugador no ha perdido
}

void manejarDerrota(sfRenderWindow* window) {
    // Destapar todas las casillas con minas
    for (int i = 0; i < numFilas; i++) {
        for (int j = 0; j < numColumnas; j++) {
            if (minas[i][j] == 1) {
                sfSprite_setTexture(celdas[i][j], textureMina, sfTrue);
                sfRenderWindow_drawSprite(window, celdas[i][j], NULL);
            }
        }
    }
    sfRenderWindow_display(window); // Actualizar la ventana inmediatamente
    mostrarMensaje(window, "Explotaste", sfRed);
    esperarEnterOClickParaContinuar(window);
}



void manejarVictoria(sfRenderWindow* window) {
    // Destapar todas las casillas con minas
    for (int i = 0; i < numFilas; i++) {
        for (int j = 0; j < numColumnas; j++) {
            if (minas[i][j] == 1) {
                sfSprite_setTexture(celdas[i][j], textureCeldaAbierta, sfTrue);
                // Dibujar la mina en la celda[i][j]
                sfSprite_setTexture(celdas[i][j], textureMina, sfTrue);
            }
        }
    }
    mostrarMensaje(window, "Has ganado", sfGreen);
    esperarEnterOClickParaContinuar(window);
}

int todasCasillasDestapadas() {
    for (int i = 0; i < numFilas; i++) {
        for (int j = 0; j < numColumnas; j++) {
            if (minas[i][j] == 0 && casillasDestapadas[i * numColumnas + j] == 0) {
                return 0; // Aún hay casillas sin destapar
            }
        }
    }
    return 1; // Todas las casillas sin minas han sido destapadas
}

void esperarEnterOClickParaContinuar(sfRenderWindow* window) {
    sfEvent event;
    while (sfRenderWindow_isOpen(window)) {
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtKeyPressed && event.key.code == sfKeyEnter) {
                reiniciarJuego();
                return; // Salir de la función cuando se presiona "Enter"
            }
            if (event.type == sfEvtMouseButtonPressed) {
                reiniciarJuego();
                return; // Salir de la función cuando se hace clic con el mouse
            }
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
        }
    }
}


