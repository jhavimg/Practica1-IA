#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include <queue>
using namespace std;

Action ComportamientoJugador::think(Sensores sensores)
{

	Action accion = actIDLE;
	int a;

	cout << "Posicion: fila " << sensores.posF << " columna " << sensores.posC << " ";
	switch (sensores.sentido)
	{
	case 0:
		cout << "Norte" << endl;
		break;
	case 1:
		cout << "Noreste" << endl;
		break;
	case 2:
		cout << "Este" << endl;
		break;
	case 3:
		cout << "Sureste" << endl;
		break;
	case 4:
		cout << "Sur " << endl;
		break;
	case 5:
		cout << "Suroeste" << endl;
		break;
	case 6:
		cout << "Oeste" << endl;
		break;
	case 7:
		cout << "Noroeste" << endl;
		break;
	}
	cout << "Terreno: ";
	for (int i = 0; i < sensores.terreno.size(); i++)
		cout << sensores.terreno[i];
	cout << endl;

	cout << "Superficie: ";
	for (int i = 0; i < sensores.superficie.size(); i++)
		cout << sensores.superficie[i];
	cout << endl;

	cout << "Colisión: " << sensores.colision << endl;
	cout << "Reset: " << sensores.reset << endl;
	cout << "Vida: " << sensores.vida << endl;
	cout << endl;

	if (sensores.reset)
	{
		bien_situado = false;
		zapatillas = false;
		bikini = false;
	}

	switch (last_action)
	{
	case actFORWARD:
		// Actualización en caso de avanzar

		switch (current_state.brujula)
		{
		case norte:
			current_state.fil--;
			break;
		case noreste:
			current_state.fil--;
			current_state.col++;
			break;
		case este:
			current_state.col++;
			break;
		case sureste:
			current_state.fil++;
			current_state.col++;
			break;
		case sur:
			current_state.fil++;
			break;
		case suroeste:
			current_state.fil++;
			current_state.col--;
			break;
		case oeste:
			current_state.col--;
			break;
		case noroeste:
			current_state.fil--;
			current_state.col--;
			break;
		}
		break;

	case actTURN_SL:
		// Actualización en caso de girar 45º a la izquierda
		a = current_state.brujula;
		a = (a + 7) % 8;
		current_state.brujula = static_cast<Orientacion>(a);
		break;

	case actTURN_SR:
		// Actualización en caso de girar 45º a la derecha
		a = current_state.brujula;
		a = (a + 1) % 8;
		current_state.brujula = static_cast<Orientacion>(a);
		break;

	case actTURN_BL:
		// Actualización en caso de girar 135º a la izquierda
		a = current_state.brujula;
		a = (a + 5) % 8;
		current_state.brujula = static_cast<Orientacion>(a);
		break;

	case actTURN_BR:
		// Actualización en caso de girar 135º a la derecha
		a = current_state.brujula;
		a = (a + 3) % 8;
		current_state.brujula = static_cast<Orientacion>(a);
		break;
	}

	if (sensores.posF != -1 && !bien_situado)
	{
		current_state.fil = sensores.posF;
		current_state.col = sensores.posC;
		current_state.brujula = sensores.sentido;
		bien_situado = true;
	}

	if (sensores.terreno[0] == 'D' && !zapatillas)
	{
		zapatillas = true;
	}

	if (sensores.terreno[0] == 'K' && !bikini)
	{
		bikini = true;
	}

	if (sensores.terreno[0] == 'X' && sensores.bateria <= 2500)
	{
		if (cola_acciones.empty())
		{
			for (int i = 0; i < 150; i++)
			{
				cola_acciones.push(actIDLE);
			}
		}
		else
		{
			accion = actIDLE;
		}
	}

	if (!bien_situado)
	{
		bool encontrado = false;
		for (int i = 1; i < 16 && !encontrado; i++)
		{
			if (sensores.terreno[i] == 'G')
			{
				objetivo = i;
				encontrado = true;
			}
		}
	}

	if (!bikini)
	{
		bool encontrado = false;
		for (int i = 1; i < 16 && !encontrado; i++)
		{
			if (sensores.terreno[i] == 'K')
			{
				objetivo = i;
				encontrado = true;
			}
		}
	}

	if (!zapatillas)
	{
		bool encontrado = false;
		for (int i = 1; i < 16 && !encontrado; i++)
		{
			if (sensores.terreno[i] == 'D')
			{
				objetivo = i;
				encontrado = true;
			}
		}
		for (int i = 1; i < 16; i++)
		{
			if (sensores.terreno[i] == 'M')
			{
				switch (i)
				{
				case 1:
					if (objetivo == 4 || objetivo == 9)
						objetivo = 0;

					break;
				case 2:
					if (objetivo == 6 || objetivo == 12)
						objetivo = 0;

					break;
				case 3:
					if(objetivo == 8 || objetivo == 15)
						objetivo = 0;

					break;
				case 4:
					if(objetivo == 9)
						objetivo = 0;

					break;
				case 5:
					if(objetivo == 10)
						objetivo = 0;

					break;
				case 6:
					if(objetivo == 12)
						objetivo = 0;
					
					break;
				case 7:
					if(objetivo == 14)
						objetivo = 0;
					
					break;
				case 8:
					if(objetivo == 15)
						objetivo = 0;

					break;
				}
			}
		}
		bool salir = false;
		for(int i = 9; i < 16 && !salir; i++){
			if(sensores.terreno[1] == 'B' && sensores.terreno[2] == 'B' && sensores.terreno[3] == 'B' && sensores.terreno[4] == 'B' && sensores.terreno[5] == 'B'
			&& sensores.terreno[6] == 'B' && sensores.terreno[7] == 'B' && sensores.terreno[8] == 'B' && sensores.terreno[i] != 'B'){
				salir = true;
				objetivo = i;
			}
		}
	}

	if (sensores.bateria <= 2500)
	{
		bool encontrado = false;
		for (int i = 1; i < 16 && !encontrado; i++)
		{
			if (sensores.terreno[i] == 'X')
			{
				objetivo = i;
				encontrado = true;
			}
		}
	}

	if (cola_acciones.empty())
	{
		switch (objetivo)
		{
		case 1:
			cola_acciones.push(actTURN_SL);
			cola_acciones.push(actFORWARD);
			break;
		case 3:
			cola_acciones.push(actTURN_SR);
			cola_acciones.push(actFORWARD);
			break;
		case 4:
			cola_acciones.push(actTURN_SL);
			cola_acciones.push(actFORWARD);
			cola_acciones.push(actFORWARD);
			break;
		case 5:
			cola_acciones.push(actFORWARD);
			cola_acciones.push(actTURN_SL);
			cola_acciones.push(actFORWARD);
			break;
		case 6:
			cola_acciones.push(actFORWARD);
			cola_acciones.push(actFORWARD);
			break;
		case 7:
			cola_acciones.push(actFORWARD);
			cola_acciones.push(actTURN_SR);
			cola_acciones.push(actFORWARD);
			break;
		case 8:
			cola_acciones.push(actTURN_SR);
			cola_acciones.push(actFORWARD);
			cola_acciones.push(actFORWARD);
			break;
		case 9:
			cola_acciones.push(actTURN_SL);
			cola_acciones.push(actFORWARD);
			cola_acciones.push(actFORWARD);
			cola_acciones.push(actFORWARD);
			break;
		case 10:
			cola_acciones.push(actFORWARD);
			cola_acciones.push(actTURN_SL);
			cola_acciones.push(actFORWARD);
			cola_acciones.push(actFORWARD);
			break;
		case 11:
			cola_acciones.push(actFORWARD);
			cola_acciones.push(actFORWARD);
			cola_acciones.push(actTURN_SL);
			cola_acciones.push(actFORWARD);
			break;
		case 12:
			cola_acciones.push(actFORWARD);
			cola_acciones.push(actFORWARD);
			cola_acciones.push(actFORWARD);
			break;
		case 13:
			cola_acciones.push(actFORWARD);
			cola_acciones.push(actFORWARD);
			cola_acciones.push(actTURN_SR);
			cola_acciones.push(actFORWARD);
			break;
		case 14:
			cola_acciones.push(actFORWARD);
			cola_acciones.push(actTURN_SR);
			cola_acciones.push(actFORWARD);
			cola_acciones.push(actFORWARD);
			break;
		case 15:
			cola_acciones.push(actTURN_SR);
			cola_acciones.push(actFORWARD);
			cola_acciones.push(actFORWARD);
			cola_acciones.push(actFORWARD);
			break;
		}
	}

	if (cola_acciones.empty())
	{	

		if(sensores.terreno[1] == 'M' && sensores.terreno[2] == 'M' && sensores.terreno[3] == 'M' && (current_state.brujula == 0 || current_state.brujula == 2 || current_state.brujula == 4 || current_state.brujula == 6))
		{
			cola_acciones.push(actTURN_SL);
			cola_acciones.push(actTURN_SL);
		}
		if(sensores.terreno[2] != 'M' && sensores.superficie[2] == '_' && sensores.terreno[3] == 'M' && sensores.terreno[6] != 'M' && sensores.superficie[6] == '_' && sensores.terreno[7] != 'M' && sensores.terreno[7] == '_' && (current_state.brujula == 0 || current_state.brujula == 2 || current_state.brujula == 4 || current_state.brujula == 6)){
			cola_acciones.push(actFORWARD);
			cola_acciones.push(actTURN_SR);
			cola_acciones.push(actFORWARD);
			cola_acciones.push(actTURN_SR);
		}
		if(sensores.terreno[2] != 'M' && sensores.superficie[2] == '_'&& sensores.terreno[1] != 'M' && sensores.superficie[1] == '_' && sensores.terreno[3] == 'M' && sensores.terreno[6] == 'M' && sensores.terreno[5] == 'M' && (current_state.brujula == 0 || current_state.brujula == 2 || current_state.brujula == 4 || current_state.brujula == 6)){
			cola_acciones.push(actFORWARD);
			cola_acciones.push(actTURN_SL);
			cola_acciones.push(actTURN_SL);
			cola_acciones.push(actFORWARD);
		}
		if(sensores.terreno[2] != 'M' && sensores.superficie[2] == '_' && sensores.terreno[1] == 'M' && sensores.terreno[6] != 'M' && sensores.superficie[6] == '_' && sensores.terreno[5] != 'M' && sensores.superficie[5] == '_' && (current_state.brujula == 0 || current_state.brujula == 2 || current_state.brujula == 4 || current_state.brujula == 6)){
			cola_acciones.push(actFORWARD);
			cola_acciones.push(actTURN_SL);
			cola_acciones.push(actFORWARD);
			cola_acciones.push(actTURN_SL);
			cola_acciones.push(actFORWARD);
		}
		if(sensores.terreno[2] == 'M' && sensores.terreno[1] == 'M' && sensores.terreno[3] == 'M' && (current_state.brujula == 1 || current_state.brujula == 3 || current_state.brujula == 5 || current_state.brujula == 7)){
			cola_acciones.push(actTURN_BL);
		}
		if(sensores.terreno[1] == 'M' && sensores.terreno[2] == 'M' && sensores.terreno[3] != 'M' && sensores.superficie[3] == '_' && current_state.brujula == 1){
			cola_acciones.push(actTURN_SR);
			cola_acciones.push(actFORWARD);
		}


		if (sensores.terreno[1] != 'M' && sensores.superficie[1] == '_' && sensores.terreno[2] == 'M' && sensores.terreno[3] == 'M' && (current_state.brujula == 0 || current_state.brujula == 2 || current_state.brujula == 4 || current_state.brujula == 6))
		{
			cola_acciones.push(actTURN_SL);
			cola_acciones.push(actFORWARD);
		}
		if (sensores.terreno[2] != 'M' && sensores.superficie[2] == '_' && sensores.terreno[3] == 'M' && sensores.terreno[6] != 'M' && sensores.superficie[6] == '_' && sensores.terreno[7] == 'M' && sensores.terreno[12] == 'M' && (current_state.brujula == 0 || current_state.brujula == 2 || current_state.brujula == 4 || current_state.brujula == 6))
		{
			cola_acciones.push(actFORWARD);
			cola_acciones.push(actFORWARD);
			cola_acciones.push(actTURN_SL);
			cola_acciones.push(actTURN_SL);
		}
		if(current_state.brujula == 5 && sensores.terreno[1] == 'M' && sensores.terreno[2] == 'M' && sensores.terreno[3] != 'M' && sensores.superficie[3] == '_'){
			cola_acciones.push(actTURN_SR);
			cola_acciones.push(actFORWARD);
		}

		if(sensores.terreno[3] == 'M' && sensores.terreno[2] != 'M' && sensores.terreno[6] != 'M' && sensores.terreno[7] != 'M'){
			cola_acciones.push(actFORWARD);
			cola_acciones.push(actTURN_SR);
			cola_acciones.push(actFORWARD);
		}
	}

	if (bien_situado)
	{
		PonerTerrenoEnMatriz(sensores.terreno, current_state, mapaResultado);
	}

	switch (sensores.terreno[2])
	{
	case 'T':
		if (sensores.superficie[2] == '_')
		{
			accion = actFORWARD;
			break;
		}
		else
		{
			accion = giratorio(rotacion);
			break;
		}

	case 'S':
		if (sensores.superficie[2] == '_')
		{
			accion = actFORWARD;
			break;
		}
		else
		{
			accion = giratorio(rotacion);

			break;
		}

	case 'P':
		accion = giratorio(rotacion);
		break;
	case 'G':
		if (sensores.superficie[2] == '_')
		{
			accion = actFORWARD;
			break;
		}
		else
		{
			accion = giratorio(rotacion);

			break;
		}

	case 'D':
		if (sensores.superficie[2] == '_')
		{
			accion = actFORWARD;
			break;
		}
		else
		{
			accion = giratorio(rotacion);

			break;
		}

	case 'K':
		if (sensores.superficie[2] == '_')
		{
			accion = actFORWARD;
			break;
		}
		else
		{
			accion = giratorio(rotacion);

			break;
		}
	case 'A':
		if (bikini && sensores.superficie[2] == '_')
		{
			accion = actFORWARD;
			break;
		}
		else
		{
			accion = giratorio(rotacion);

			break;
		}
	case 'M':
		accion = giratorio(rotacion);
		break;
	case 'B':
		if (zapatillas && sensores.superficie[2] == '_')
		{
			accion = actFORWARD;
			break;
		}
		else
		{
			accion = giratorio(rotacion);
			break;
		}
	case 'X':
		if (sensores.superficie[2] == '_')
		{
			accion = actFORWARD;
			break;
		}
		else
		{
			accion = giratorio(rotacion);
			break;
		}
	}

	// Determinar el efecto de la ultima accion enviada

	if (!cola_acciones.empty())
	{
		accion = cola_acciones.front();
		last_action = accion;
		cola_acciones.pop();
		return accion;
	}
	else
	{
		last_action = accion;
		return accion;
	}
}

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}

void PonerTerrenoEnMatriz(const vector<unsigned char> &terreno, const state &st, vector<vector<unsigned char>> &matriz)
{
	matriz[st.fil][st.col] = terreno[0];

	switch (st.brujula)
	{
	case 0:
		matriz[st.fil - 1][st.col - 1] = terreno[1];
		matriz[st.fil - 1][st.col] = terreno[2];
		matriz[st.fil - 1][st.col + 1] = terreno[3];
		matriz[st.fil - 2][st.col - 2] = terreno[4];
		matriz[st.fil - 2][st.col - 1] = terreno[5];
		matriz[st.fil - 2][st.col] = terreno[6];
		matriz[st.fil - 2][st.col + 1] = terreno[7];
		matriz[st.fil - 2][st.col + 2] = terreno[8];
		matriz[st.fil - 3][st.col - 3] = terreno[9];
		matriz[st.fil - 3][st.col - 2] = terreno[10];
		matriz[st.fil - 3][st.col - 1] = terreno[11];
		matriz[st.fil - 3][st.col] = terreno[12];
		matriz[st.fil - 3][st.col + 1] = terreno[13];
		matriz[st.fil - 3][st.col + 2] = terreno[14];
		matriz[st.fil - 3][st.col + 3] = terreno[15];
		break;
	case 1:
		matriz[st.fil - 1][st.col] = terreno[1];
		matriz[st.fil - 1][st.col + 1] = terreno[2];
		matriz[st.fil][st.col + 1] = terreno[3];
		matriz[st.fil - 2][st.col] = terreno[4];
		matriz[st.fil - 2][st.col + 1] = terreno[5];
		matriz[st.fil - 2][st.col + 2] = terreno[6];
		matriz[st.fil - 1][st.col + 2] = terreno[7];
		matriz[st.fil][st.col + 2] = terreno[8];
		matriz[st.fil - 3][st.col] = terreno[9];
		matriz[st.fil - 3][st.col + 1] = terreno[10];
		matriz[st.fil - 3][st.col + 2] = terreno[11];
		matriz[st.fil - 3][st.col + 3] = terreno[12];
		matriz[st.fil - 2][st.col + 3] = terreno[13];
		matriz[st.fil - 1][st.col + 3] = terreno[14];
		matriz[st.fil][st.col + 3] = terreno[15];
		break;
	case 2:
		matriz[st.fil - 1][st.col + 1] = terreno[1];
		matriz[st.fil][st.col + 1] = terreno[2];
		matriz[st.fil + 1][st.col + 1] = terreno[3];
		matriz[st.fil - 2][st.col + 2] = terreno[4];
		matriz[st.fil - 1][st.col + 2] = terreno[5];
		matriz[st.fil][st.col + 2] = terreno[6];
		matriz[st.fil + 1][st.col + 2] = terreno[7];
		matriz[st.fil + 2][st.col + 2] = terreno[8];
		matriz[st.fil - 3][st.col + 3] = terreno[9];
		matriz[st.fil - 2][st.col + 3] = terreno[10];
		matriz[st.fil - 1][st.col + 3] = terreno[11];
		matriz[st.fil][st.col + 3] = terreno[12];
		matriz[st.fil + 1][st.col + 3] = terreno[13];
		matriz[st.fil + 2][st.col + 3] = terreno[14];
		matriz[st.fil + 3][st.col + 3] = terreno[15];
		break;
	case 3:
		matriz[st.fil][st.col + 1] = terreno[1];
		matriz[st.fil + 1][st.col + 1] = terreno[2];
		matriz[st.fil + 1][st.col] = terreno[3];
		matriz[st.fil][st.col + 2] = terreno[4];
		matriz[st.fil + 1][st.col + 2] = terreno[5];
		matriz[st.fil + 2][st.col + 2] = terreno[6];
		matriz[st.fil + 2][st.col + 1] = terreno[7];
		matriz[st.fil + 2][st.col] = terreno[8];
		matriz[st.fil][st.col + 3] = terreno[9];
		matriz[st.fil + 1][st.col + 3] = terreno[10];
		matriz[st.fil + 2][st.col + 3] = terreno[11];
		matriz[st.fil + 3][st.col + 3] = terreno[12];
		matriz[st.fil + 3][st.col + 2] = terreno[13];
		matriz[st.fil + 3][st.col + 1] = terreno[14];
		matriz[st.fil + 3][st.col] = terreno[15];
		break;
	case 4:
		matriz[st.fil + 1][st.col + 1] = terreno[1];
		matriz[st.fil + 1][st.col] = terreno[2];
		matriz[st.fil + 1][st.col - 1] = terreno[3];
		matriz[st.fil + 2][st.col + 2] = terreno[4];
		matriz[st.fil + 2][st.col + 1] = terreno[5];
		matriz[st.fil + 2][st.col] = terreno[6];
		matriz[st.fil + 2][st.col - 1] = terreno[7];
		matriz[st.fil + 2][st.col - 2] = terreno[8];
		matriz[st.fil + 3][st.col + 3] = terreno[9];
		matriz[st.fil + 3][st.col + 2] = terreno[10];
		matriz[st.fil + 3][st.col + 1] = terreno[11];
		matriz[st.fil + 3][st.col] = terreno[12];
		matriz[st.fil + 3][st.col - 1] = terreno[13];
		matriz[st.fil + 3][st.col - 2] = terreno[14];
		matriz[st.fil + 3][st.col - 3] = terreno[15];
		break;
	case 5:
		matriz[st.fil + 1][st.col] = terreno[1];
		matriz[st.fil + 1][st.col - 1] = terreno[2];
		matriz[st.fil][st.col - 1] = terreno[3];
		matriz[st.fil + 2][st.col] = terreno[4];
		matriz[st.fil + 2][st.col - 1] = terreno[5];
		matriz[st.fil + 2][st.col - 2] = terreno[6];
		matriz[st.fil + 1][st.col - 2] = terreno[7];
		matriz[st.fil][st.col - 2] = terreno[8];
		matriz[st.fil + 3][st.col] = terreno[9];
		matriz[st.fil + 3][st.col - 1] = terreno[10];
		matriz[st.fil + 3][st.col - 2] = terreno[11];
		matriz[st.fil + 3][st.col - 3] = terreno[12];
		matriz[st.fil + 2][st.col - 3] = terreno[13];
		matriz[st.fil + 1][st.col - 3] = terreno[14];
		matriz[st.fil][st.col - 3] = terreno[15];
		break;
	case 6:
		matriz[st.fil * 1][st.col - 1] = terreno[1];
		matriz[st.fil][st.col - 1] = terreno[2];
		matriz[st.fil - 1][st.col - 1] = terreno[3];
		matriz[st.fil + 2][st.col - 2] = terreno[4];
		matriz[st.fil + 1][st.col - 2] = terreno[5];
		matriz[st.fil][st.col - 2] = terreno[6];
		matriz[st.fil - 1][st.col - 2] = terreno[7];
		matriz[st.fil - 2][st.col - 2] = terreno[8];
		matriz[st.fil + 3][st.col - 3] = terreno[9];
		matriz[st.fil + 2][st.col - 3] = terreno[10];
		matriz[st.fil + 1][st.col - 3] = terreno[11];
		matriz[st.fil][st.col - 3] = terreno[12];
		matriz[st.fil - 1][st.col - 3] = terreno[13];
		matriz[st.fil - 2][st.col - 3] = terreno[14];
		matriz[st.fil - 3][st.col - 3] = terreno[15];
		break;
	case 7:
		matriz[st.fil][st.col - 1] = terreno[1];
		matriz[st.fil - 1][st.col - 1] = terreno[2];
		matriz[st.fil - 1][st.col] = terreno[3];
		matriz[st.fil][st.col - 2] = terreno[4];
		matriz[st.fil - 1][st.col - 2] = terreno[5];
		matriz[st.fil - 2][st.col - 2] = terreno[6];
		matriz[st.fil - 2][st.col - 1] = terreno[7];
		matriz[st.fil - 2][st.col] = terreno[8];
		matriz[st.fil][st.col - 3] = terreno[9];
		matriz[st.fil - 1][st.col - 3] = terreno[10];
		matriz[st.fil - 2][st.col - 3] = terreno[11];
		matriz[st.fil - 3][st.col - 3] = terreno[12];
		matriz[st.fil - 3][st.col - 2] = terreno[13];
		matriz[st.fil - 3][st.col - 1] = terreno[14];
		matriz[st.fil - 3][st.col] = terreno[15];
		break;
	}
}

Action giratorio(int &rotacion)
{
	if (rotacion < 2)
	{
		rotacion = rand() % 9;
		return actTURN_SL;
	}
	else if (rotacion >= 2 && rotacion < 4)
	{
		rotacion = rand() % 9;
		return actTURN_SR;
	}
	else if (rotacion >= 4 && rotacion < 6)
	{
		rotacion = rand() % 9;
		return actTURN_BL;
	}
	else if (rotacion >= 6 && rotacion <= 8)
	{
		rotacion = rand() % 9;
		return actTURN_BR;
	}
}