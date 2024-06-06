#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <stdio.h>

/*int main() {
    //Creación de la ventana
    sfVideoMode mode = {800, 600, 32};
    sfRenderWindow* window = sfRenderWindow_create(mode, "Buscaminas", sfResize | sfClose, NULL);

    // Cargar la imagen del icono
      sfTexture* iconTexture = sfTexture_createFromFile("C:\\Users\\vit\\Documents\\CodeBlock\\Interfaz Buscaminas C\\CeldaBuscaminas.png", NULL);
      sfImage* iconImage = sfTexture_copyToImage(iconTexture);

    // Establecer el icono de la ventana
      sfRenderWindow_setIcon(window, sfImage_getSize(iconImage).x, sfImage_getSize(iconImage).y, sfImage_getPixelsPtr(iconImage));

    //Llamada a la función para el tipo de fuente de la letra
    sfFont* font = sfFont_createFromFile("C:\\Users\\vit\\Documents\\CodeBlock\\Interfaz Buscaminas C\\LVDCGO__.ttf");

    //Cargar la textura del sprite
     sfTexture* texture = sfTexture_createFromFile("C:\\Users\\vit\\Documents\\CodeBlock\\Interfaz Buscaminas C\\CeldaBuscaminasAbierta.png", NULL);

    //Personalizar el sprite
    sfSprite* sprite = sfSprite_create(); //Crear el sprite
    sfSprite_setTexture(sprite,texture,sfTrue); //y asignarle la textura
    sfSprite_setPosition(sprite, (sfVector2f){270,0}); // Establecer la posicion del sprite
    sfSprite_setScale(sprite, (sfVector2f){0.5f, 0.5f});// Establecer el tamaño del sprite (escalar por 2 en ambos ejes)

//    //Otro sprite
//    sfSprite* bomba=sfSprite_create();
//    sfSprite_setTexture(bomba,texture,sfTrue);
//    sfSprite_setPosition(bomba,(sfVector2f){640,30});
//    sfSprite_setScale(bomba, (sfVector2f){0.5f,0.5f});

    //Creacion de colores
    sfColor grayColor = sfColor_fromRGB(180,200,220); //Color gris



    //Creacion y personalizacion del texto 1
    sfText* texto = sfText_create(); //funcion de creacion
    sfText_setString(texto, "BUSCAMINAS"); //funcion para el mensaje a mostrar
    sfText_setFont(texto, font); //Aqui le colocamos la fuente al texto
    sfText_setCharacterSize(texto, 30);//Tamaño de la fuente
    sfText_setFillColor(texto, sfBlack);// Color de las letras
    sfText_setPosition(texto, (sfVector2f){210,230}); //Posición en eje x,y

    //Creacion del texto de jugar
    sfText * texto1 = sfText_create();
    sfText_setString(texto1,"Jugar");
    sfText_setFont(texto1,font);
    sfText_setCharacterSize(texto1,20);
    sfText_setFillColor(texto1,sfBlack);
    sfText_setPosition(texto1, (sfVector2f){335,300});

    //Creacion del texto de puntajes
    sfText * texto2 = sfText_create();
    sfText_setString(texto2,"Puntajes");
    sfText_setFont(texto2,font);
    sfText_setCharacterSize(texto2,20);
    sfText_setFillColor(texto2,sfBlack);
    sfText_setPosition(texto2, (sfVector2f){305,360});

    //Creacion del texto de créditos
    sfText * texto3 = sfText_create();
    sfText_setString(texto3,"Desarrollado por");
    sfText_setFont(texto3,font);
    sfText_setCharacterSize(texto3,20);
    sfText_setFillColor(texto3,sfBlack);
    sfText_setPosition(texto3, (sfVector2f){200,420});

    //Creacion del texto de Salir del juego
    sfText * texto4 = sfText_create();
    sfText_setString(texto4,"Salir");
    sfText_setFont(texto4,font);
    sfText_setCharacterSize(texto4,20);
    sfText_setFillColor(texto4,sfBlack);
    sfText_setPosition(texto4, (sfVector2f){355,480});


    //color de fondo para el rectangulo
    sfColor lightColor = {255, 255, 255, 64}; // Blanco con 25% de opacidad
     // Crear el rectángulo
    sfRectangleShape* rectangle = sfRectangleShape_create();
    sfRectangleShape_setFillColor(rectangle, lightColor);
    sfRectangleShape_setOutlineColor(rectangle, sfBlue);
    sfRectangleShape_setOutlineThickness(rectangle, 2.0f);

    // Ajustar el rectángulo para que envuelva el texto
    sfFloatRect textRect = sfText_getGlobalBounds(texto1);
    sfRectangleShape_setPosition(rectangle, (sfVector2f){textRect.left - 10, textRect.top - 10});
    sfRectangleShape_setSize(rectangle, (sfVector2f){textRect.width + 20, textRect.height + 20});

    //Segundo rectangulo
    sfRectangleShape* cuadro1 = sfRectangleShape_create();
    sfRectangleShape_setFillColor(cuadro1,lightColor);
    sfRectangleShape_setOutlineColor(cuadro1,sfRed);
    sfRectangleShape_setOutlineThickness(cuadro1, 3.0f);
    //ajustar el segundo rectangulo
    sfFloatRect texRect1 = sfText_getGlobalBounds(texto2);
    sfRectangleShape_setPosition(cuadro1, (sfVector2f){texRect1.left - 10, texRect1.top - 10});
    sfRectangleShape_setSize(cuadro1, (sfVector2f){texRect1.width + 20, texRect1.height + 20});

    //Tercer rectangulo
    sfRectangleShape* cuadro2 = sfRectangleShape_create();
    sfRectangleShape_setFillColor(cuadro2,lightColor);
    sfRectangleShape_setOutlineColor(cuadro2,sfYellow);
    sfRectangleShape_setOutlineThickness(cuadro2, 3.0f);
    //ajustar el Tercer rectangulo
    sfFloatRect texRect2 = sfText_getGlobalBounds(texto3);
    sfRectangleShape_setPosition(cuadro2, (sfVector2f){texRect2.left - 10, texRect2.top - 10});
    sfRectangleShape_setSize(cuadro2, (sfVector2f){texRect2.width + 20, texRect2.height + 20});

    //Cuarto Rectangulo
    sfRectangleShape* cuadro3 = sfRectangleShape_create();
    sfRectangleShape_setFillColor(cuadro3,lightColor);
    sfRectangleShape_setOutlineColor(cuadro3,sfGreen);
    sfRectangleShape_setOutlineThickness(cuadro3, 3.0f);
    //ajustar el Cuarto rectangulo
    sfFloatRect texRect3 = sfText_getGlobalBounds(texto4);
    sfRectangleShape_setPosition(cuadro3, (sfVector2f){texRect3.left - 10, texRect3.top - 10});
    sfRectangleShape_setSize(cuadro3, (sfVector2f){texRect3.width + 20, texRect3.height + 20});



    //Ventana de los desarrolladores
    sfVideoMode formato = {800, 600, 32};
    sfRenderWindow* window2 = sfRenderWindow_create(formato,"Buscaminas",sfResize|sfClose,NULL);

    //Textos de nuestros nombres
    sfText* texto5 = sfText_create(); //funcion de creacion
    sfText_setString(texto5, "Equipo de trabajo"); //funcion para el mensaje a mostrar
    sfText_setFont(texto5, font); //Aqui le colocamos la fuente al texto
    sfText_setCharacterSize(texto5, 30);//Tamaño de la fuente
    sfText_setFillColor(texto5, sfBlack);// Color de las letras
    sfText_setPosition(texto5, (sfVector2f){210,130}); //Posición en eje x,y

    //Cristian
    sfText* texto6 = sfText_create();
    sfText_setString(texto6,"Cristian Bolivar");
    sfText_setFont(texto6,font);
    sfText_setCharacterSize(texto6,20);
    sfText_setFillColor(texto6,sfBlack);
    sfText_setPosition(texto6,(sfVector2f){210,190});

    //Gilberto
    sfText* texto7 = sfText_create();
    sfText_setString(texto7,"Gilberto Sifontes");
    sfText_setFont(texto7,font);
    sfText_setCharacterSize(texto7,20);
    sfText_setFillColor(texto7,sfBlack);
    sfText_setPosition(texto7,(sfVector2f){210,250});

    //Ines
    sfText* texto8 = sfText_create();
    sfText_setString(texto8,"Ines Salazar");
    sfText_setFont(texto8,font);
    sfText_setCharacterSize(texto8,20);
    sfText_setFillColor(texto8,sfBlack);
    sfText_setPosition(texto8,(sfVector2f){210,310});

    //Johanna
    sfText* texto9 = sfText_create();
    sfText_setString(texto9,"Johanna Guedez");
    sfText_setFont(texto9,font);
    sfText_setCharacterSize(texto9,20);
    sfText_setFillColor(texto9,sfBlack);
    sfText_setPosition(texto9,(sfVector2f){210,370});

    //Jebel
    sfText* texto10 = sfText_create();
    sfText_setString(texto10,"Jebel Ruiz");
    sfText_setFont(texto10,font);
    sfText_setCharacterSize(texto10,20);
    sfText_setFillColor(texto10,sfBlack);
    sfText_setPosition(texto10,(sfVector2f){210,430});

    //Manuel
    sfText* texto11 = sfText_create();
    sfText_setString(texto11,"Manuel Jaimes");
    sfText_setFont(texto11,font);
    sfText_setCharacterSize(texto11,20);
    sfText_setFillColor(texto11,sfBlack);
    sfText_setPosition(texto11,(sfVector2f){210,490});

    //Volver a la principal
    sfText* texto12 = sfText_create();
    sfText_setString(texto11,"Atras");
    sfText_setFont(texto11,font);
    sfText_setCharacterSize(texto11,20);
    sfText_setFillColor(texto11,sfBlack);
    sfText_setPosition(texto11,(sfVector2f){0,550});
    sfRectangleShape* cuadro4 = sfRectangleShape_create(); //Su rectangulo
    sfRectangleShape_setFillColor(cuadro4,lightColor);
    sfRectangleShape_setOutlineColor(cuadro4,sfRed);
    sfRectangleShape_setOutlineThickness(cuadro4, 3.0f);
    sfFloatRect texRect4 = sfText_getGlobalBounds(texto12); //ajustar
    sfRectangleShape_setPosition(cuadro4, (sfVector2f){texRect4.left - 10, texRect4.top - 10});
    sfRectangleShape_setSize(cuadro4, (sfVector2f){texRect4.width + 20, texRect4.height + 20});

    // Variables para controlar qué ventana está visible
    sfBool menuVisible = sfTrue;
    sfBool secondVisible = sfFalse;

    //Correr el programa mientras esta abierto
    while (sfRenderWindow_isOpen(window)  && sfRenderWindow_isOpen(window2)) {

        // Procesar eventos en la ventana del menú
        if (menuVisible){
            sfEvent event;

            while (sfRenderWindow_pollEvent(window, &event)) {
                // Detectar click izquierdo en desarrollado por
                if (event.type == sfEvtMouseButtonPressed && event.mouseButton.button == sfMouseLeft)
                {
                    sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);
                    sfFloatRect rectBounds = sfRectangleShape_getGlobalBounds(cuadro2);
                    if (sfFloatRect_contains(&rectBounds, mousePos.x, mousePos.y))
                    {
                        // Ocultar ventana del menú y mostrar segunda ventana
                        sfRenderWindow_setVisible(window, sfFalse);
                        menuVisible = sfFalse;
                        secondVisible = sfTrue;
                        sfRenderWindow_setVisible(window2, sfTrue);
                    }
                }

                if (event.type == sfEvtClosed)
                    sfRenderWindow_close(window);
            }

        //Limpiar pantalla en pantalla las siguientes cosas
        sfRenderWindow_clear(window, grayColor); //La propia ventana y le da color
        sfRenderWindow_setIcon(window, sfImage_getSize(iconImage).x, sfImage_getSize(iconImage).y, sfImage_getPixelsPtr(iconImage));
        sfRenderWindow_drawText(window, texto, NULL); //dibuja el texto dado
        sfRenderWindow_drawRectangleShape(window, rectangle, NULL);
        sfRenderWindow_drawRectangleShape(window,cuadro1,NULL);
        sfRenderWindow_drawRectangleShape(window,cuadro2,NULL);
        sfRenderWindow_drawRectangleShape(window,cuadro3,NULL);


        sfRenderWindow_drawText(window,texto1,NULL);
        sfRenderWindow_drawText(window,texto2,NULL);
        sfRenderWindow_drawText(window,texto3,NULL);
        sfRenderWindow_drawText(window,texto4,NULL);
        //Dibujar el sprite en la ventana

        sfRenderWindow_drawSprite(window,sprite,NULL);
//        sfRenderWindow_drawSprite(window,bomba,NULL);

        sfRenderWindow_display(window); // Muestra la ventana actualizada

        }
        // Procesar eventos en la segunda ventana
        if (secondVisible)
        {
            sfEvent event;
            while (sfRenderWindow_pollEvent(window2, &event))
            {
                // Detectar click izquierdo en atras
                if (event.type == sfEvtMouseButtonPressed && event.mouseButton.button == sfMouseLeft)
                {
                    sfVector2i mousePos = sfMouse_getPositionRenderWindow(window2);
                    sfFloatRect rectBounds = sfRectangleShape_getGlobalBounds(cuadro4);
                    if (sfFloatRect_contains(&rectBounds, mousePos.x, mousePos.y))
                    {
                        // Ocultar segunda ventana y mostrar ventana del menú
                        sfRenderWindow_setVisible(window2, sfFalse);
                        secondVisible = sfFalse;
                        menuVisible = sfTrue;
                        sfRenderWindow_setVisible(window, sfTrue);
                    }
                }

                if (event.type == sfEvtClosed)
                    sfRenderWindow_close(window2);
            }

            // Dibujar todo en la segunda ventana
            sfRenderWindow_clear(window2,grayColor);
            sfRenderWindow_setIcon(window2, sfImage_getSize(iconImage).x, sfImage_getSize(iconImage).y, sfImage_getPixelsPtr(iconImage));
            sfRenderWindow_drawText(window2,texto5,NULL);
            sfRenderWindow_drawText(window2,texto6,NULL);
            sfRenderWindow_drawText(window2,texto7,NULL);
            sfRenderWindow_drawText(window2,texto8,NULL);
            sfRenderWindow_drawText(window2,texto9,NULL);
            sfRenderWindow_drawText(window2,texto10,NULL);
            sfRenderWindow_drawText(window2,texto11,NULL);
            sfRenderWindow_drawText(window2,texto12,NULL);
            sfRenderWindow_drawRectangleShape(window2,cuadro4,NULL);

            sfRenderWindow_display(window2);
        }


    }

    //Libera los recursos
    sfFont_destroy(font);
    sfText_destroy(texto);
    sfText_destroy(texto1);
    sfText_destroy(texto2);
    sfText_destroy(texto3);
    sfText_destroy(texto4);
    sfText_destroy(texto5);
    sfText_destroy(texto6);
    sfText_destroy(texto7);
    sfText_destroy(texto8);
    sfText_destroy(texto9);
    sfText_destroy(texto10);
    sfText_destroy(texto11);
    sfText_destroy(texto12);
    sfRectangleShape_destroy(cuadro4);
    sfRectangleShape_destroy(rectangle);
    sfRectangleShape_destroy(cuadro1);
    sfRectangleShape_destroy(cuadro2);
    sfRectangleShape_destroy(cuadro3);
    sfSprite_destroy(sprite);
//    sfSprite_destroy(bomba);
    sfTexture_destroy(texture);
    sfRenderWindow_destroy(window);
    sfRenderWindow_destroy(window2);

    return 0;

}
*/
