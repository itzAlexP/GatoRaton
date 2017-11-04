#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <wait.h>
#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <string.h>

#define MAX 100
#define SIZE_TABLERO 64
#define SIZE_FILA_TABLERO 8
#define LADO_CASILLA 64
#define RADIO_AVATAR 25.f
#define OFFSET_AVATAR 5

#define SIZE_TABLERO 64
#define LADO_CASILLA 64
#define RADIO_AVATAR 25.f
#define OFFSET_AVATAR 5


enum TipoProceso {RATON, GATO, PADRE};
char tablero[SIZE_TABLERO];

/**
 * Si vale true --> nos permite marcar casilla con el mouse
 * Si vale false --> No podemos interactuar con el tablero y aparece un letrero de "esperando"
 */
bool tienesTurno = true;

/**
 * Ahora mismo no tiene efecto, pero luego lo necesitarás para validar los movimientos
 * en función de si eres el gato o el ratón.
 */
TipoProceso quienSoy = TipoProceso::RATON;



/**
 * Cuando el jugador clica en la pantalla, se nos da una coordenada del 0 al 512.
 * Esta función la transforma a una posición entre el 0 y el 7
 */
sf::Vector2f TransformaCoordenadaACasilla(int _x, int _y)
{
    float xCasilla = _x/LADO_CASILLA;
    float yCasilla = _y/LADO_CASILLA;
    sf::Vector2f casilla(xCasilla, yCasilla);
    return casilla;
}

/**
 * Si guardamos las posiciones de las piezas con valores del 0 al 7,
 * esta función las transforma a posición de ventana (pixel), que va del 0 al 512
 */
sf::Vector2f BoardToWindows(sf::Vector2f _position)
{
    return sf::Vector2f(_position.x*LADO_CASILLA+OFFSET_AVATAR, _position.y*LADO_CASILLA+OFFSET_AVATAR);
}

/**
 * Contiene el código SFML que captura el evento del clic del mouse y el código que pinta por pantalla
 */
void DibujaSFML()
{
    sf::Vector2f casillaOrigen, casillaDestino;
    bool casillaMarcada=false;

    sf::RenderWindow window(sf::VideoMode(512,512), "El Gato y el Raton");
    while(window.isOpen())
    {
        sf::Event event;

        //Este primer WHILE es para controlar los eventos del mouse
        while(window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left && tienesTurno)
                    {
                        int x = event.mouseButton.x;
                        int y = event.mouseButton.y;
                        if (!casillaMarcada)
                        {
                            casillaOrigen = TransformaCoordenadaACasilla(x, y);
                            casillaMarcada = true;
                            //TODO: Comprobar que la casilla marcada coincide con las posición del raton (si le toca al ratón)
                            //o con la posicion de alguna de las piezas del gato (si le toca al gato)

                        }
                        else
                        {
                            casillaDestino = TransformaCoordenadaACasilla(x, y);
                            if (casillaOrigen.x == casillaDestino.x && casillaOrigen.y == casillaDestino.y)
                            {
                                casillaMarcada = false;
                                //Si me vuelven a picar sobre la misma casilla, la desmarco
                            }
                            else
                            {
                                if (quienSoy == TipoProceso::RATON)
                                {
                                    //TODO: Validar que el destino del ratón es correcto

                                    //TODO: Si es correcto, modificar la posición del ratón y enviar las posiciones al padre

                                }
                                else if (quienSoy == TipoProceso::GATO)
                                {
                                    //TODO: Validar que el destino del gato es correcto

                                    //TODO: Si es correcto, modificar la posición de la pieza correspondiente del gato y enviar las posiciones al padre
                                }
                            }
                        }
                    }
                    break;
                default:
                    break;

            }
        }

        window.clear();

        //A partir de aquí es para pintar por pantalla
        //Este FOR es para el tablero
        for (int i =0; i<8; i++)
        {
            for(int j = 0; j<8; j++)
            {
                sf::RectangleShape rectBlanco(sf::Vector2f(LADO_CASILLA,LADO_CASILLA));
                rectBlanco.setFillColor(sf::Color::White);
                if(i%2 == 0)
                {
                    //Empieza por el blanco
                    if (j%2 == 0)
                    {
                        rectBlanco.setPosition(sf::Vector2f(i*LADO_CASILLA, j*LADO_CASILLA));
                        window.draw(rectBlanco);
                    }
                }
                else
                {
                    //Empieza por el negro
                    if (j%2 == 1)
                    {
                        rectBlanco.setPosition(sf::Vector2f(i*LADO_CASILLA, j*LADO_CASILLA));
                        window.draw(rectBlanco);
                    }
                }
            }
        }

        //TODO: Para pintar el circulito del ratón
        sf::CircleShape shapeRaton(RADIO_AVATAR);
        shapeRaton.setFillColor(sf::Color::Blue);
        sf::Vector2f posicionRaton(4.f,7.f);
        posicionRaton = BoardToWindows(posicionRaton);
        shapeRaton.setPosition(posicionRaton);
        window.draw(shapeRaton);

        //Pintamos los cuatro circulitos del gato
        sf::CircleShape shapeGato(RADIO_AVATAR);
        shapeGato.setFillColor(sf::Color::Red);

        sf::Vector2f positionGato1(1.f,0.f);
        positionGato1 = BoardToWindows(positionGato1);
        shapeGato.setPosition(positionGato1);

        window.draw(shapeGato);

        sf::Vector2f positionGato2(3.f,0.f);
        positionGato2 = BoardToWindows(positionGato2);
        shapeGato.setPosition(positionGato2);

        window.draw(shapeGato);

        sf::Vector2f positionGato3(5.f,0.f);
        positionGato3 = BoardToWindows(positionGato3);
        shapeGato.setPosition(positionGato3);

        window.draw(shapeGato);

        sf::Vector2f positionGato4(7.f,0.f);
        positionGato4 = BoardToWindows(positionGato4);
        shapeGato.setPosition(positionGato4);

        window.draw(shapeGato);


        if (!tienesTurno)
        {
            //Si no tengo el turno, pinto un letrerito de "Esperando..."
            sf::Font font;
            std::string pathFont = "liberation_sans/LiberationSans-Regular.ttf";
            if (!font.loadFromFile(pathFont))
            {
                std::cout << "No se pudo cargar la fuente"<<std::endl;
            }


            sf::Text textEsperando("Esperando...", font);
            textEsperando.setPosition(sf::Vector2f(180,200));
            textEsperando.setCharacterSize(30);
            textEsperando.setStyle(sf::Text::Bold);
            textEsperando.setFillColor(sf::Color::Green);
            window.draw(textEsperando);
        }
        else
        {
            //Si tengo el turno y hay una casilla marcada, la marco con un recuadro amarillo.
            if (casillaMarcada)
            {
                sf::RectangleShape rect(sf::Vector2f(LADO_CASILLA,LADO_CASILLA));
                rect.setPosition(sf::Vector2f(casillaOrigen.x*LADO_CASILLA, casillaOrigen.y*LADO_CASILLA));
                rect.setFillColor(sf::Color::Transparent);
                rect.setOutlineThickness(5);
                rect.setOutlineColor(sf::Color::Yellow);
                window.draw(rect);
            }
        }

        window.display();
    }

}

int main()
{
    DibujaSFML();
    return 0;
}
