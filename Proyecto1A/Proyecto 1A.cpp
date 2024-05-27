//Proyecto 1A
//Estructuras de Datos
//Jos� Calder�n Barquero y Jos� Juli�n G�mez



#include <iostream>
#include <cstring>
#include <ctime>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <string>
#define FPS 60.0


using namespace std;

struct T_Vehiculo {
    char identificacion[20];
    float peso;
    float tamano;
    char tipo[20];
    T_Vehiculo* PtrSiguiente;
} *PtrT_Vehiculo;

struct Piso {
    char tipo[20];
    T_Vehiculo* listaVehiculos;
    Piso* siguiente;
};

int cantidadVehiculos;
T_Vehiculo* ultimoVehiculo;
int espaciosVacios;
time_t tiempoInicioParqueo;

void InicializarParqueo() {
    cantidadVehiculos = 0;
    ultimoVehiculo = nullptr;
    espaciosVacios = 0;
    tiempoInicioParqueo = time(nullptr);
}

// obtener variables
int ObtenerCantidadVehiculos() {
    return cantidadVehiculos;
}

T_Vehiculo* ObtenerUltimoVehiculo() {
    return ultimoVehiculo;
}

int ObtenerEspaciosVacios() {
    return espaciosVacios;
}

// actualizar variables
void AumentarCantidadVehiculos() {
    cantidadVehiculos++;
}

void DisminuirCantidadVehiculos() {
    cantidadVehiculos--;
}

void ActualizarUltimoVehiculo(T_Vehiculo* vehiculo) {
    ultimoVehiculo = vehiculo;
}

void AumentarEspaciosVacios() {
    espaciosVacios++;
}

void DisminuirEspaciosVacios() {
    espaciosVacios--;
}

T_Vehiculo* CrearVehiculo(const char* identificacion, float peso, float tamano, const char* tipo)
{
    T_Vehiculo* vehiculo = new T_Vehiculo;

    strcpy_s(vehiculo->identificacion, identificacion);
    vehiculo->peso = peso;
    vehiculo->tamano = tamano;
    strcpy_s(vehiculo->tipo, tipo);

    vehiculo->PtrSiguiente = nullptr;

    return vehiculo;
}

void AgregarInicioInventario(T_Vehiculo*& Lista, T_Vehiculo*& Nuevo)
{
    Nuevo->PtrSiguiente = Lista;
    Lista = Nuevo;
}

void AgregarFinalInventario(T_Vehiculo*& Lista, T_Vehiculo*& Nuevo)
{
    T_Vehiculo* Aux = Lista;
    if (Aux != nullptr)
    {
        while (Aux->PtrSiguiente != nullptr)
        {
            Aux = Aux->PtrSiguiente;
        }
        Aux->PtrSiguiente = Nuevo;
    }
    else
    {
        Lista = Nuevo;
    }
}

void ListarInventario(T_Vehiculo* Lista)
{
    int Contador = 1;
    T_Vehiculo* Aux = Lista;
    while (Aux != nullptr)
    {
        printf("%d ", Contador);
        printf("%s ", Aux->identificacion);
        printf("%.2f ", Aux->peso);
        printf("%.2f ", Aux->tamano);
        printf("%s\n", Aux->tipo);
        Aux = Aux->PtrSiguiente;
        Contador++;
    }
}

T_Vehiculo* BuscarVehiculo(T_Vehiculo* Lista, const char* identificacion)
{
    T_Vehiculo* Aux = Lista;
    while (Aux != nullptr)
    {
        if (strcmp(Aux->identificacion, identificacion) == 0)
            return Aux;
        Aux = Aux->PtrSiguiente;
    }
    return nullptr;
}

void GuardarInventario(T_Vehiculo* Lista)
{
    FILE* archivo;
    fopen_s(&archivo, "ARCHIVO.txt", "w+");
    if (archivo == nullptr)
    {
        printf("No se pudo abrir el archivo.");
    }
    else
    {
        T_Vehiculo* Aux = Lista;
        while (Aux != nullptr)
        {
            fprintf(archivo, "%s\n", Aux->identificacion);
            fprintf(archivo, "%.2f\n", Aux->peso);
            fprintf(archivo, "%.2f\n", Aux->tamano);
            fprintf(archivo, "%s\n", Aux->tipo);
            Aux = Aux->PtrSiguiente;
        }
    }
    fclose(archivo);
}

void CargarInventario(T_Vehiculo*& Lista)
{
    FILE* archivo;
    fopen_s(&archivo, "ARCHIVO.txt", "r");
    if (archivo == nullptr)
    {
        printf("No se pudo abrir el archivo");
    }
    else
    {
        while (!feof(archivo))
        {
            T_Vehiculo* Nuevo = new T_Vehiculo;
            fscanf_s(archivo, "%s\n", Nuevo->identificacion, sizeof(Nuevo->identificacion));
            fscanf_s(archivo, "%f\n", &Nuevo->peso);
            fscanf_s(archivo, "%f\n", &Nuevo->tamano);
            fscanf_s(archivo, "%s\n", Nuevo->tipo);
            Nuevo->PtrSiguiente = nullptr;
            AgregarFinalInventario(Lista, Nuevo);
        }
    }
    fclose(archivo);
}

void AgregarInicioPiso(T_Vehiculo*& lista, const char* identificacion, float peso, float tamano, const char* tipo) {
    T_Vehiculo* nuevoVehiculo = new T_Vehiculo;
    strcpy_s(nuevoVehiculo->identificacion, identificacion);
    nuevoVehiculo->peso = peso;
    nuevoVehiculo->tamano = tamano;
    strcpy_s(nuevoVehiculo->tipo, tipo);
    nuevoVehiculo->PtrSiguiente = lista;
    lista = nuevoVehiculo;
}

//Recibe los datos de un carro y lo a�ade a un parqueo
void AgregarVehiculo(Piso*& parqueo, const char* tipo, const char* identificacion, float peso, float tamano) {
    Piso* pisoActual = parqueo;
    while (pisoActual != nullptr) {
            AgregarInicioPiso(pisoActual->listaVehiculos, identificacion, peso, tamano, tipo);
            return;
    }
}

Piso* CrearPiso(const char* tipo) {
    Piso* nuevoPiso = new Piso;
    strcpy_s(nuevoPiso->tipo, tipo);
    nuevoPiso->listaVehiculos = nullptr;
    nuevoPiso->siguiente = nullptr;
    return nuevoPiso;
}

void AgregarPiso(Piso*& parqueo, const char* tipo) {
    Piso* nuevoPiso = CrearPiso(tipo);
    nuevoPiso->siguiente = parqueo;
    parqueo = nuevoPiso;
}

Piso* CrearParqueo(int pisosMotosBicicletas, int pisosAutos4x4, int pisosCamiones) {
    Piso* parqueo = nullptr;

    // Crear pisos para motos y bicicletas
    for (int i = 0; i < pisosMotosBicicletas; ++i) {
        AgregarPiso(parqueo, "moto_bicicleta");
    }

    // Crear pisos para autos y 4x4
    for (int i = 0; i < pisosAutos4x4; ++i) {
        AgregarPiso(parqueo, "automovil_4x4");
    }

    // Crear pisos para camiones
    for (int i = 0; i < pisosCamiones; ++i) {
        AgregarPiso(parqueo, "camion");
    }

    return parqueo;
}

//Recibe los datos de una placa e intenta eliminarlo si existe
void SacarVehiculo(Piso* parqueo, const char* identificacion) {
    Piso* pisoActual = parqueo;
    while (pisoActual != nullptr) {
        T_Vehiculo* vehiculoActual = pisoActual->listaVehiculos;
        T_Vehiculo* vehiculoAnterior = nullptr;

        while (vehiculoActual != nullptr) {
            if (strcmp(vehiculoActual->identificacion, identificacion) == 0) {
                if (vehiculoAnterior != nullptr) {
                    vehiculoAnterior->PtrSiguiente = vehiculoActual->PtrSiguiente;
                }
                else {
                    pisoActual->listaVehiculos = vehiculoActual->PtrSiguiente;
                }

                delete vehiculoActual;
                cout << "El vehiculo con identificacion '" << identificacion << "' ha sido sacado del parqueo." << endl;
                return;
            }

            vehiculoAnterior = vehiculoActual;
            vehiculoActual = vehiculoActual->PtrSiguiente;
        }

        pisoActual = pisoActual->siguiente;
    }
    cout << "No se encontr� un veh�culo con la placa " << identificacion << endl;
}

void MostrarTiposVehiculos() {
    cout << "Tipos de veh�culos que admite el parqueo:" << endl;
    cout << "moto_bicicleta" << endl;
    cout << "automovil_4x4" << endl;
    cout << "camion" << endl;
}

//Funciones Allegro simulaci�n

//Funci�n que dibuja los espacios del parqueo en la pantalla
void dibujarParqueo(int modo, int rx, int ry, int espacios, int plantas) {
    float x1 = 1, x2 = x1 + rx, y1 = 60, y2 = y1 + ry, anchoCelda = rx, altoCelda = ry;
    //Seg�n el tipo de parqueo
    switch (modo)
    {
    case 1:
        anchoCelda = anchoCelda / espacios;
        x2 = x1 + anchoCelda;
        //Recorre la cantidad de espacios y dibuja rect�ngulos
        for (int i = 0; i < espacios; i++)
        {   
            al_draw_rectangle(x1, y1, x2, y2, al_map_rgb(250, 0, 0), 0);
            x1 += anchoCelda; x2 += anchoCelda;
        }
        break;
    case 2:
        altoCelda = altoCelda / 2;
        anchoCelda = anchoCelda / ((espacios + 1) /2);
        x2 = x1 + anchoCelda;
        y2 = y1 + altoCelda;
        for (int i = 0; i < espacios/2; i++)
        {
            al_draw_rectangle(x1, y1, x2, y2, al_map_rgb(250, 0, 0), 0);
            x1 += anchoCelda; x2 += anchoCelda;
        }
        y1 += altoCelda; y2 += altoCelda;
        x1 = 1; x2 = x1 + anchoCelda;
        for (int i = espacios/2; i < espacios; i++)
        {
            al_draw_rectangle(x1, y1, x2, y2, al_map_rgb(250, 0, 0), 0);
            x1 += anchoCelda; x2 += anchoCelda;
        }
        break;
    case 3:
        espacios = plantas * 2;
        altoCelda = altoCelda / plantas;
        anchoCelda = anchoCelda / 2;
        x2 = x1 + anchoCelda;
        y2 = y1 + altoCelda;
        for (int i = 0; i < espacios / 2; i++)
        {
            x1 = 1; x2 = x1 + anchoCelda;
            for (int j = 0; j < 2; j++)
            {
                al_draw_rectangle(x1, y1, x2, y2, al_map_rgb(250, 0, 0), 0);
                x1 += anchoCelda; x2 += anchoCelda;
            }
            y1 += altoCelda; y2 += altoCelda;
        }
        break;
    default:
        break;
    }
    
}


//Funci�n que dibuja los vehiculos en la pantalla
void dibujarVehiculos(Piso* parqueo, int modo, int rx, int ry, int espacios, int plantas, ALLEGRO_BITMAP* carro) {
    
    if (parqueo->listaVehiculos == NULL) {
        return;
    }

    float x1 = 1, x2 = x1 + rx, y1 = 60, y2 = y1 + ry, anchoCarro = rx, altoCarro = ry;
    
    T_Vehiculo* carroActual = parqueo->listaVehiculos;
    int i = 0, j = 0;

    //Seg�n el tipo de parqueo
    switch (modo)
    {
    case 1:
        anchoCarro = anchoCarro / espacios;
        x2 = x1 + anchoCarro;
            //Si hay un carro y est� dentro de los espacios se dibuja un carro
            while (carroActual && i < espacios){
                al_draw_scaled_bitmap(carro, 0, 0, 1000, 1000, x1, y1, anchoCarro, altoCarro, 0);
                x1 += anchoCarro; x2 += anchoCarro;
                carroActual = carroActual->PtrSiguiente;
                i++;
            }
        break;
    case 2:
        altoCarro = altoCarro / 2;
        anchoCarro = anchoCarro / ((espacios + 1) / 2);
        x2 = x1 + anchoCarro;
        y2 = y1 + altoCarro;
        while (carroActual && i < espacios / 2) {
            al_draw_scaled_bitmap(carro, 0, 0, 1000, 1000, x1, y1, anchoCarro, altoCarro, 0);
            x1 += anchoCarro; x2 += anchoCarro;
            i++;
            carroActual = carroActual->PtrSiguiente;
        }
        y1 += altoCarro; y2 += altoCarro;
        x1 = 1; x2 = x1 + anchoCarro;
        while (carroActual && i < espacios)
        {
            al_draw_scaled_bitmap(carro, 0, 0, 1000, 1000, x1, y1, anchoCarro, altoCarro, 0);
            x1 += anchoCarro; x2 += anchoCarro;
            i++;
            carroActual = carroActual->PtrSiguiente;
        }
        break;
    case 3:
        espacios = plantas * 2;
        altoCarro = altoCarro / plantas;
        anchoCarro = anchoCarro / 2;
        x2 = x1 + anchoCarro;
        y2 = y1 + altoCarro;
        while (i < espacios / 2 && carroActual) {
            x1 = 1; x2 = x1 + anchoCarro;
            while (j<2 && carroActual)
            {
                al_draw_scaled_bitmap(carro, 0, 0, 1000, 1000, x1, y1, anchoCarro, altoCarro, 0);
                x1 += anchoCarro; x2 += anchoCarro;
                carroActual = carroActual->PtrSiguiente;
                j++;
            }
            y1 += altoCarro; y2 += altoCarro;
            i++;
        }
        break;
    default:
        break;
    }

}


//Funciones Allegro
void simulacion(int modo, int plantas = 1, int espacios=1) {
    //Se inicializa la estructura del parqueo
    InicializarParqueo();
    Piso* parqueo = nullptr;
    if (modo == 1) {
        parqueo = CrearParqueo(1, 0, 0);
    }
    if (modo == 2) {
        parqueo = CrearParqueo(2, 0, 0);
    }
    else {
        parqueo = CrearParqueo(plantas, 0, 0);
    }
    //Se crea la pantalla
    if (!al_init()) {
        al_show_native_message_box(NULL, "Ventana Emergente", "Error", "No se puede inicializar Allegro", NULL, NULL);
        return;
    }
    ALLEGRO_MONITOR_INFO monitor;
    al_get_monitor_info(0, &monitor);//Se obtiene la informaci�n del monitor como su resoluci�n mediante diferencia de puntos finales e iniciales
    const int RX = monitor.x2 - monitor.x1 - 700;
    const int RY = monitor.y2 - monitor.y1 - 300;
    ALLEGRO_DISPLAY* pantalla = al_create_display(RX, RY);
    ALLEGRO_TIMER* timer1 = al_create_timer(1.0 / FPS);
    al_set_window_title(pantalla, "Simulaci�n");//Se le pone un t�tulo a la ventana
    ALLEGRO_EVENT_QUEUE* cola_eventos = al_create_event_queue();
    al_register_event_source(cola_eventos, al_get_timer_event_source(timer1));
    al_register_event_source(cola_eventos, al_get_display_event_source(pantalla));
    al_register_event_source(cola_eventos, al_get_keyboard_event_source());
    al_register_event_source(cola_eventos, al_get_mouse_event_source());
    int X = al_get_display_width(pantalla);
    int Y = al_get_display_height(pantalla);
    ALLEGRO_FONT* fuente1;
    fuente1 = al_load_font("arial.ttf", 50, NULL);
    ALLEGRO_FONT* fuente2;
    fuente2 = al_load_font("arial.ttf", 20, NULL);
    ALLEGRO_BITMAP* flecha1; ALLEGRO_BITMAP* celda;
    ALLEGRO_BITMAP* carro;
    carro = al_load_bitmap("carro.png");
    flecha1 = al_load_bitmap("arrowicon.png");
    celda = al_load_bitmap("Cuadrado.png");

    bool hecho = true;
    int mousex = 0;
    int mousey = 0;
    int a = 0;
    int inicio = 0;
    int alturaDeFrame = 1080;
    int AnchoDeFrame = 1920;
    int cantidadPlantas = plantas;
    int cantidadEspacios = espacios;
    int anchoCelda = RX - 300; 
    int altoCelda = RY - 200; 

    al_start_timer(timer1);
    while (hecho)
    {
        ALLEGRO_EVENT eventos;
        al_wait_for_event(cola_eventos, &eventos);

        if (eventos.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            //Se registra la posici�n en x y y del mouse
            mousex = eventos.mouse.x;
            mousey = eventos.mouse.y;
        }
        if (eventos.type == ALLEGRO_EVENT_TIMER)
        {
            if (eventos.timer.source == timer1)
            {
                
                //Se dibujan el parqueo y los veh�culos
                al_clear_to_color(al_map_rgb(0, 0, 0));
                dibujarParqueo(modo, RX - 300, RY - 200, espacios, plantas);
                dibujarVehiculos(parqueo, modo, RX - 300, RY - 200, espacios, plantas, carro);
                al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * 725.0 / 768.0), ALLEGRO_ALIGN_CENTRE, "SALIR");
            }
        }
        if ((mousex >= X / 2 - 45 && mousex <= X / 2 + 35) && (mousey >= (RY * (720.0 / 768.0)) && mousey <= (RY * (755.0 / 768.0)))) {
            //Si se presiona la  opci�n de salir se sale de la cola de eventos y termina la aplicaci�n
            al_draw_text(fuente2, al_map_rgb(250, 250, 20), X / 2, (RY * (725.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "SALIR");
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (eventos.mouse.button & 1) {
                    hecho = false;
                    al_destroy_display(pantalla);
                }
            }
        }
        if (eventos.type == ALLEGRO_EVENT_KEY_DOWN) {
            //Si se presiona la tecla A se genera un carro aleatorio y se a�ade al parqueo
            if (eventos.keyboard.keycode == ALLEGRO_KEY_A) {
                srand(time(NULL));
                int placa = rand() % 9999 + 1000;
                string placaStr = to_string(placa);
                al_draw_text(fuente2, al_map_rgb(250, 250, 250), 950, 100, 0, "Llega al parqueo el carro");
                al_draw_text(fuente2, al_map_rgb(250, 250, 250), 950, 125, 0, "con placa: ");
                al_draw_text(fuente2, al_map_rgb(250, 250, 250), 1060, 125, 0, placaStr.c_str());
                al_flip_display();
                AgregarVehiculo(parqueo, "carro", placaStr.c_str(), 500, 500);
                Piso* pisoActual = parqueo;
                Sleep(2000);
               
            }
            //Si se presiona la tecla S se genera un numero de placa aleatorio y se intenta sacar del parqueo
            if (eventos.keyboard.keycode == ALLEGRO_KEY_S) {
                srand(time(NULL));
                int placa = rand() % 9999 + 1000;
                string placaStr = to_string(placa);
                al_draw_text(fuente2, al_map_rgb(250, 250, 250), 950, 100, 0, "Se solicita sacar el carro");
                al_draw_text(fuente2, al_map_rgb(250, 250, 250), 950, 125, 0, "con placa: ");
                al_draw_text(fuente2, al_map_rgb(250, 250, 250), 1060, 125, 0, placaStr.c_str());
                al_flip_display();
                SacarVehiculo(parqueo, placaStr.c_str());
                Piso* pisoActual = parqueo;
                Sleep(2000);
            }
        }
        al_flip_display();
    }
}

//Pantalla de seleccion del numero de espacios para la simulacion
void seleccionEspacios(int modo) {
    //Se crea la pantalla
    if (!al_init()) {
        al_show_native_message_box(NULL, "Ventana Emergente", "Error", "No se puede inicializar Allegro", NULL, NULL);
        return;
    }
    ALLEGRO_MONITOR_INFO monitor;
    al_get_monitor_info(0, &monitor);//Se obtiene la informaci�n del monitor como su resoluci�n mediante diferencia de puntos finales e iniciales
    const int RX = monitor.x2 - monitor.x1 - 700;
    const int RY = monitor.y2 - monitor.y1 - 300;
    ALLEGRO_DISPLAY* pantalla = al_create_display(RX, RY);
    ALLEGRO_TIMER* timer1 = al_create_timer(1.0 / FPS);
    al_set_window_title(pantalla, "Selecci�n de espacios de parqueo");//Se le pone un t�tulo a la ventana
    ALLEGRO_EVENT_QUEUE* cola_eventos = al_create_event_queue();
    al_register_event_source(cola_eventos, al_get_timer_event_source(timer1));
    al_register_event_source(cola_eventos, al_get_display_event_source(pantalla));
    al_register_event_source(cola_eventos, al_get_keyboard_event_source());
    al_register_event_source(cola_eventos, al_get_mouse_event_source());
    int X = al_get_display_width(pantalla);
    int Y = al_get_display_height(pantalla);
    ALLEGRO_FONT* fuente1;
    fuente1 = al_load_font("arial.ttf", 50, NULL);
    ALLEGRO_FONT* fuente2;
    fuente2 = al_load_font("arial.ttf", 20, NULL);
    ALLEGRO_BITMAP* flecha1;
    flecha1 = al_load_bitmap("arrowicon.png");

    bool hecho = true;
    int mousex = 0;
    int mousey = 0;
    int a = 0;
    int inicio = 0;
    int Delay = 25;

    int cantidadEspacios = 1;
    al_start_timer(timer1);
    while (hecho)
    {
        ALLEGRO_EVENT eventos;
        al_wait_for_event(cola_eventos, &eventos);

        if (eventos.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            //Se registra la posici�n en x y y del mouse
            mousex = eventos.mouse.x;
            mousey = eventos.mouse.y;
        }
        if (eventos.type == ALLEGRO_EVENT_TIMER)
        {
            if (eventos.timer.source == timer1)
            {
                //Se crean las opciones y los botones para cambiar la cantidad
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_text(fuente1, al_map_rgb(250, 250, 250), X / 2, (RY * (100.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Seleccione la cantidad de espacios:");
                al_draw_scaled_bitmap(flecha1, 0, 0, 4623, 4623, X/2-200, RY * 275/768, 100, 100, ALLEGRO_FLIP_HORIZONTAL);
                al_draw_scaled_bitmap(flecha1, 0, 0, 4623, 4623, X/2+100, RY * 275/768, 100, 100, 0);
                string cantidadStr = to_string(cantidadEspacios);
                const char* cantChar = cantidadStr.c_str();
                al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * (320.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, cantChar);
                al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, RY * (420.0 / 768.0), ALLEGRO_ALIGN_CENTRE, "Continuar");
                al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * (530.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "SALIR");
            }
        }
        //Si se posiciona el mouse en las coordenadas donde indica la opci�n disminuir
        if ((mousex >= X / 2 - 200 && mousex <= X / 2 - 100) && (mousey >= (RY * 250.0 / 768.0) && mousey <= (RY * 385.0 / 768.0))) {
            al_draw_scaled_bitmap(flecha1, 0, 0, 4623, 4623, X / 2 - 200, RY * 260 / 768, 125, 125, ALLEGRO_FLIP_HORIZONTAL);
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (eventos.mouse.button & 1) {
                    cantidadEspacios -= 1;
                }
            }
        }
        //Si se posiciona el mouse en las coordenadas donde indica la opci�n aumentar
        if ((mousex >= X / 2 + 100 && mousex <= X / 2 + 200) && (mousey >= (RY * 250.0 / 768.0) && mousey <= (RY * 385.0 / 768.0))) {
            al_draw_scaled_bitmap(flecha1, 0, 0, 4623, 4623, X / 2 + 100, RY * 260 / 768, 125, 125, 0);
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (eventos.mouse.button & 1) {
                    cantidadEspacios += 1;
                }
            }
        }
        //Si se posiciona el mouse en las coordenadas donde indica la opci�n continuar
        if ((mousex >= X / 2 - 45 && mousex <= X / 2 + 45) && (mousey >= (RY * (415.0 / 768.0)) && mousey <= (RY * (450.0 / 768.0)))) {
            al_draw_text(fuente2, al_map_rgb(250, 250, 20), X / 2, (RY * (420.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Continuar");
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (eventos.mouse.button & 1) {
                    //Se elimina esta pantalla y se pasa a otra
                    al_destroy_display(pantalla);
                    simulacion(modo, 1, cantidadEspacios);
                    hecho = false;
                }
            }
        }
        if ((mousex >= X / 2 - 45 && mousex <= X / 2 + 45) && (mousey >= (RY * (525.0 / 768.0)) && mousey <= (RY * (565.0 / 768.0)))) {
            //Si se presiona la  opci�n de salir se sale de la cola de eventos y vuelve al menu
            al_draw_text(fuente2, al_map_rgb(250, 250, 20), X / 2, (RY * (530.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "SALIR");
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (eventos.mouse.button & 1) {
                    hecho = false;
                }
            }
        }
        al_flip_display();
    }
}

//Pantalla de seleccion del numero de plantas para la simulacion
void seleccionPlantas() {
    //Se crea la pantalla
    if (!al_init()) {
        al_show_native_message_box(NULL, "Ventana Emergente", "Error", "No se puede inicializar Allegro", NULL, NULL);
        return;
    }
    ALLEGRO_MONITOR_INFO monitor;
    al_get_monitor_info(0, &monitor);//Se obtiene la informaci�n del monitor como su resoluci�n mediante diferencia de puntos finales e iniciales
    const int RX = monitor.x2 - monitor.x1 - 700;
    const int RY = monitor.y2 - monitor.y1 - 300;
    ALLEGRO_DISPLAY* pantalla = al_create_display(RX, RY);
    ALLEGRO_TIMER* timer1 = al_create_timer(1.0 / FPS);
    al_set_window_title(pantalla, "Selecci�n de plantas");//Se le pone un t�tulo a la ventana
    ALLEGRO_EVENT_QUEUE* cola_eventos = al_create_event_queue();
    al_register_event_source(cola_eventos, al_get_timer_event_source(timer1));
    al_register_event_source(cola_eventos, al_get_display_event_source(pantalla));
    al_register_event_source(cola_eventos, al_get_keyboard_event_source());
    al_register_event_source(cola_eventos, al_get_mouse_event_source());
    int X = al_get_display_width(pantalla);
    int Y = al_get_display_height(pantalla);
    ALLEGRO_FONT* fuente1;
    fuente1 = al_load_font("arial.ttf", 50, NULL);
    ALLEGRO_FONT* fuente2;
    fuente2 = al_load_font("arial.ttf", 20, NULL);
    ALLEGRO_BITMAP* flecha1;
    flecha1 = al_load_bitmap("arrowicon.png");

    bool hecho = true;
    int mousex = 0;
    int mousey = 0;
    int a = 0;
    int inicio = 0;
    bool modo;
    int Delay = 25;
    int alturaDeFrame = 1080;
    int AnchoDeFrame = 1920;

    int cantidadPlantas = 1;
    al_start_timer(timer1);
    while (hecho)
    {
        ALLEGRO_EVENT eventos;
        al_wait_for_event(cola_eventos, &eventos);

        if (eventos.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            //Se registra la posici�n en x y y del mouse
            mousex = eventos.mouse.x;
            mousey = eventos.mouse.y;
        }
        if (eventos.type == ALLEGRO_EVENT_TIMER)
        {
            if (eventos.timer.source == timer1)
            {
                //Se crean las opciones y los botones para cambiar la cantidad
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_text(fuente1, al_map_rgb(250, 250, 250), X / 2, (RY * (100.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Seleccione la cantidad de plantas:");
                al_draw_scaled_bitmap(flecha1, 0, 0, 4623, 4623, X / 2 - 200, RY * 275 / 768, 100, 100, ALLEGRO_FLIP_HORIZONTAL);
                al_draw_scaled_bitmap(flecha1, 0, 0, 4623, 4623, X / 2 + 100, RY * 275 / 768, 100, 100, 0);
                string cantidadStr = to_string(cantidadPlantas);
                const char* cantChar = cantidadStr.c_str();
                al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * (320.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, cantChar);
                al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, RY * (420.0 / 768.0), ALLEGRO_ALIGN_CENTRE, "Continuar");
                al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * (530.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "SALIR");
            }
        }
        //Si se posiciona el mouse en las coordenadas donde indica la opci�n disminuir
        if ((mousex >= X / 2 - 200 && mousex <= X / 2 - 100) && (mousey >= (RY * 250.0 / 768.0) && mousey <= (RY * 385.0 / 768.0))) {
            al_draw_scaled_bitmap(flecha1, 0, 0, 4623, 4623, X / 2 - 200, RY * 260 / 768, 125, 125, ALLEGRO_FLIP_HORIZONTAL);
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (eventos.mouse.button & 1) {
                    cantidadPlantas -= 1;
                }
            }
        }
        //Si se posiciona el mouse en las coordenadas donde indica la opci�n aumentar
        if ((mousex >= X / 2 + 100 && mousex <= X / 2 + 200) && (mousey >= (RY * 250.0 / 768.0) && mousey <= (RY * 385.0 / 768.0))) {
            al_draw_scaled_bitmap(flecha1, 0, 0, 4623, 4623, X / 2 + 100, RY * 260 / 768, 125, 125, 0);
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (eventos.mouse.button & 1) {
                    modo = true;
                    cantidadPlantas += 1;
                }
            }
        }
        //Si se posiciona el mouse en las coordenadas donde indica la opci�n continuar
        if ((mousex >= X / 2 - 45 && mousex <= X / 2 + 45) && (mousey >= (RY * (415.0 / 768.0)) && mousey <= (RY * (450.0 / 768.0)))) {
            
            al_draw_text(fuente2, al_map_rgb(250, 250, 20), X / 2, (RY * (420.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Continuar");
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (eventos.mouse.button & 1) {
                    //Si se da clic se elimina esta pantalla y se pasa a otra
                    al_destroy_display(pantalla);
                    simulacion(3, cantidadPlantas);
                    hecho = false;
                }
            }
        }
        if ((mousex >= X / 2 - 45 && mousex <= X / 2 + 45) && (mousey >= (RY * (525.0 / 768.0)) && mousey <= (RY * (565.0 / 768.0)))) {
            //Si se presiona la  opci�n de salir se sale de la cola de eventos y vuelve al men�
            al_draw_text(fuente2, al_map_rgb(250, 250, 20), X / 2, (RY * (530.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "SALIR");
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (eventos.mouse.button & 1) {
                    hecho = false;
                }
            }
        }
        al_flip_display();
    }
}

int main() {
    
    
    //Se inicializa allegro
    if (!al_init()) {
        al_show_native_message_box(NULL, "Ventana Emergente", "Error", "No se puede inicializar Allegro", NULL, NULL);
        return -1;
    }
    
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_keyboard();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(7);
    al_install_mouse();
    ALLEGRO_MONITOR_INFO monitor;
    al_get_monitor_info(0, &monitor);//Se obtiene la informaci�n del monitor como su resoluci�n mediante diferencia de puntos finales e iniciales
    const int RX = monitor.x2 - monitor.x1 - 700;
    const int RY = monitor.y2 - monitor.y1 - 300;
    ALLEGRO_DISPLAY* pantalla = al_create_display(RX, RY);
    al_set_window_title(pantalla, "Simulador de Parking");//Se le pone un t�tulo a la ventana
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE* cola_eventos = al_create_event_queue();
    al_register_event_source(cola_eventos, al_get_timer_event_source(timer));
    al_register_event_source(cola_eventos, al_get_display_event_source(pantalla));
    al_register_event_source(cola_eventos, al_get_keyboard_event_source());
    al_register_event_source(cola_eventos, al_get_mouse_event_source());
    int X = al_get_display_width(pantalla);
    int Y = al_get_display_height(pantalla);
    ALLEGRO_FONT* fuente1;
    fuente1 = al_load_font("arial.ttf", 50, NULL);
    ALLEGRO_FONT* fuente2;
    fuente2 = al_load_font("arial.ttf", 20, NULL);

    bool hecho = true;
    int mousex = 0;
    int mousey = 0;
    int a = 0;
    int inicio = 0;
    int Delay = 25;
    int alturaDeFrame = 1080;
    int AnchoDeFrame = 1920;
    al_start_timer(timer);
    while (hecho)
    {
        ALLEGRO_EVENT eventos;
        al_wait_for_event(cola_eventos, &eventos);

        if (eventos.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            //Se registra la posici�n en x y y del mouse
            mousex = eventos.mouse.x;
            mousey = eventos.mouse.y;
        }
        if (eventos.type == ALLEGRO_EVENT_TIMER)
        {
            if (eventos.timer.source == timer)
            {
                //Se dibujan las opciones
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_text(fuente1, al_map_rgb(250, 250, 250), X / 2, (RY * (100.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Simulador de Parking");
                al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * (250.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Simulador 1 solo piso en 1 planta");
                al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * (320.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Simulador 2 pisos en 1 planta");
                al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * (390.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Simulador n plantas 2 estacionamientos por planta");
                al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * (530.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "SALIR");
            }
        }
        //Si se posiciona el mouse en las coordenadas donde indica la primera opci�n
        if ((mousex >= X / 2 - 150 && mousex <= X / 2 + 150) && (mousey >= (RY * 245.0 / 768.0) && mousey <= (RY * 285.0 / 768.0))) {
            al_draw_text(fuente2, al_map_rgb(250, 250, 20), X / 2, (RY * (250.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Simulador 1 solo piso en 1 planta");
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (eventos.mouse.button & 1) {
                    //Si se presiona el click del mouse se pasa a la siguiente pantalla
                    al_destroy_display(pantalla);
                    seleccionEspacios(1);
                    main();//Se vuelve a lanzar el men� si se sale del juego
                    hecho = false;
                }
            }
        }
        //Si el mouse se posiciona sobre la segunda opcion
        if ((mousex >= X / 2 - 135 && mousex <= X / 2 + 135) && (mousey >= (RY * 315.0 / 768.0) && mousey <= (RY * 365.0 / 768.0))) {
            al_draw_text(fuente2, al_map_rgb(250, 250, 20), X / 2, (RY * (320.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Simulador 2 pisos en 1 planta");
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (eventos.mouse.button & 1) {
                    //Si se presiona el click del mouse se pasa a la siguiente pantalla
                    al_destroy_display(pantalla);
                    seleccionEspacios(2);
                    main();//Se vuelve a lanzar el men� si se sale del juego
                    hecho = false;
                }
            }
        }
        //Si se posiciona el mouse sobre la tercera opci�n
        if ((mousex >= X / 2 - 230 && mousex <= X / 2 + 230) && (mousey >= (RY * (385.0 / 768.0)) && mousey <= (RY * (425.0 / 768.0)))) {
            al_draw_text(fuente2, al_map_rgb(250, 250, 20), X / 2, RY * (390.0 / 768.0), ALLEGRO_ALIGN_CENTRE, "Simulador n plantas 2 estacionamientos por planta");
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (eventos.mouse.button & 1) {
                    //Si se presiona el click del mouse se pasa a la siguiente pantalla
                    al_destroy_display(pantalla);
                    seleccionPlantas();
                    main();//Se vuelve a lanzar el men� si se sale del juego
                    hecho = false;
                }
            }
        }
        
        if ((mousex >= X / 2 - 45 && mousex <= X / 2 + 45) && (mousey >= (RY * (525.0 / 768.0)) && mousey <= (RY * (565.0 / 768.0)))) {
            //Si se presiona la  opci�n de salir se sale de la cola de eventos y termina la aplicaci�n
            al_draw_text(fuente2, al_map_rgb(250, 250, 20), X / 2, (RY * (530.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "SALIR");
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (eventos.mouse.button & 1) {
                    hecho = false;
                }
            }
        }
        al_flip_display();
    }


    return 0;
}
