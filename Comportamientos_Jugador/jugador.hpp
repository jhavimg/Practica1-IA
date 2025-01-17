#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
#include <queue>
using namespace std;

struct state{
  int fil;
  int col;
  Orientacion brujula;
};

class ComportamientoJugador : public Comportamiento
{

public:
  ComportamientoJugador(unsigned int size) : Comportamiento(size)
  {
    // Constructor de la clase
    // Dar el valor inicial a las variables de estado
    current_state.fil = current_state.col = 99;
    current_state.brujula = norte;
    last_action = actIDLE;

    bien_situado = false;
    zapatillas = false;
    bikini = false;

    rotacion = 0;

    objetivo = 0;
  }

  ComportamientoJugador(const ComportamientoJugador &comport) : Comportamiento(comport) {}
  ~ComportamientoJugador() {}

  Action think(Sensores sensores);
  int interact(Action accion, int valor);

private:
  // Declarar aquí las variables de estado
    state current_state;
    Action last_action;

    queue<Action> cola_acciones;

    bool bien_situado;
    bool zapatillas;
    bool rotar;
    bool bikini;

    int rotacion;

    int objetivo;
};

void PonerTerrenoEnMatriz(const vector<unsigned char> &terreno, const state &st, vector< vector< unsigned char> > &matriz);

Action giratorio(int &rotar);

#endif
