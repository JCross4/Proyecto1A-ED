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
    cout << "No se encontró un vehículo con la placa " << identificacion << endl;
}


void MostrarTiposVehiculos() {
    cout << "Tipos de vehículos que admite el parqueo:" << endl;
    cout << "moto_bicicleta" << endl;
    cout << "automovil_4x4" << endl;
    cout << "camion" << endl;
}

//Funciones Allegro simulación
void dibujarParqueo(int modo, int rx, int ry, int espacios, int plantas) {
    float x1 = 1, x2 = x1 + rx, y1 = 60, y2 = y1 + ry, anchoCelda = rx, altoCelda = ry;
    switch (modo)
    {
    case 1:
        anchoCelda = anchoCelda / espacios;
        x2 = x1 + anchoCelda;
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

void dibujarVehiculos(Piso* parqueo, int modo, int rx, int ry, int espacios, int plantas, ALLEGRO_BITMAP* carro) {
    
    if (parqueo->listaVehiculos == NULL) {
        return;
    }

    float x1 = 1, x2 = x1 + rx, y1 = 60, y2 = y1 + ry, anchoCarro = rx, altoCarro = ry;
    
    T_Vehiculo* carroActual = parqueo->listaVehiculos;
    int i = 0, j = 0;

    switch (modo)
    {
    case 1:
        anchoCarro = anchoCarro / espacios;
        x2 = x1 + anchoCarro;
        
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
    
    if (!al_init()) {
        al_show_native_message_box(NULL, "Ventana Emergente", "Error", "No se puede inicializar Allegro", NULL, NULL);
        return;
    }
    ALLEGRO_MONITOR_INFO monitor;
    al_get_monitor_info(0, &monitor);//Se obtiene la información del monitor como su resolución mediante diferencia de puntos finales e iniciales
    const int RX = monitor.x2 - monitor.x1 - 700;
    const int RY = monitor.y2 - monitor.y1 - 300;
    ALLEGRO_DISPLAY* pantalla = al_create_display(RX, RY);
    ALLEGRO_TIMER* timer1 = al_create_timer(1.0 / FPS);
    al_set_window_title(pantalla, "Simulación");//Se le pone un título a la ventana
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
    const int maxFrame = 1;
    int frameactual = 0;
    int contadorDeFrames = 0;
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
            //Se registra la posición en x y y del mouse
            mousex = eventos.mouse.x;
            mousey = eventos.mouse.y;
        }
        if (eventos.type == ALLEGRO_EVENT_TIMER)
        {
            if (eventos.timer.source == timer1)
            {
                
                //Se dibuja el fondo animado y las opciones del menú en el display
                al_clear_to_color(al_map_rgb(0, 0, 0));
                dibujarParqueo(modo, RX - 300, RY - 200, espacios, plantas);
                dibujarVehiculos(parqueo, modo, RX - 300, RY - 200, espacios, plantas, carro);
                al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * 725.0 / 768.0), ALLEGRO_ALIGN_CENTRE, "SALIR");
            }
        }
        if ((mousex >= X / 2 - 45 && mousex <= X / 2 + 35) && (mousey >= (RY * (720.0 / 768.0)) && mousey <= (RY * (755.0 / 768.0)))) {
            //Si se presiona la  opción de salir se sale de la cola de eventos y termina la aplicación
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
            if (eventos.keyboard.keycode == ALLEGRO_KEY_S) {
                srand(time(NULL));
                int placa = rand() % 9999 + 1000;
                string placaStr = to_string(placa);
                al_draw_text(fuente2, al_map_rgb(250, 250, 250), 950, 100, 0, "Se solicita sacar el carro");
                al_draw_text(fuente2, al_map_rgb(250, 250, 250), 950, 125, 0, "con placa: ");
                al_draw_text(fuente2, al_map_rgb(250, 250, 250), 1060, 125, 0, "1041");
                al_flip_display();
                SacarVehiculo(parqueo, "1041");
                Piso* pisoActual = parqueo;
                Sleep(2000);
            }
        }
        al_flip_display();
    }
}

void seleccionEspacios(int modo) {
    if (!al_init()) {
        al_show_native_message_box(NULL, "Ventana Emergente", "Error", "No se puede inicializar Allegro", NULL, NULL);
        return;
    }
    ALLEGRO_MONITOR_INFO monitor;
    al_get_monitor_info(0, &monitor);//Se obtiene la información del monitor como su resolución mediante diferencia de puntos finales e iniciales
    const int RX = monitor.x2 - monitor.x1 - 700;
    const int RY = monitor.y2 - monitor.y1 - 300;
    ALLEGRO_DISPLAY* pantalla = al_create_display(RX, RY);
    ALLEGRO_TIMER* timer1 = al_create_timer(1.0 / FPS);
    al_set_window_title(pantalla, "Selección de espacios de parqueo");//Se le pone un título a la ventana
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
    const int maxFrame = 1;
    int frameactual = 0;
    int contadorDeFrames = 0;
    int Delay = 25;

    int cantidadEspacios = 1;
    al_start_timer(timer1);
    while (hecho)
    {
        ALLEGRO_EVENT eventos;
        al_wait_for_event(cola_eventos, &eventos);

        if (eventos.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            //Se registra la posición en x y y del mouse
            mousex = eventos.mouse.x;
            mousey = eventos.mouse.y;
        }
        if (eventos.type == ALLEGRO_EVENT_TIMER)
        {
            if (eventos.timer.source == timer1)
            {
                if (contadorDeFrames++ >= Delay) {//Se declara un contador de frames que logra avanzar la animación del menú principal
                    if (frameactual++ >= maxFrame) {
                        frameactual = 0;
                    }
                    contadorDeFrames = 0;
                }
                //Se dibuja el fondo animado y las opciones del menú en el display
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
        //Si se posiciona el mouse en las coordenadas donde indica la opción jugar
        if ((mousex >= X / 2 - 200 && mousex <= X / 2 - 100) && (mousey >= (RY * 250.0 / 768.0) && mousey <= (RY * 385.0 / 768.0))) {
            al_draw_scaled_bitmap(flecha1, 0, 0, 4623, 4623, X / 2 - 200, RY * 260 / 768, 125, 125, ALLEGRO_FLIP_HORIZONTAL);
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (eventos.mouse.button & 1) {
                    //Si se presiona el click del mouse se inicia el juego y se indican las vidas totales y el nivel, si el juego termina se avanza de nivel hasta que se pierdan las vidas y el modo determina si es maquina vs maquina o jugador vs maquina
                    cantidadEspacios -= 1;
                }
            }
        }
        //Si el mouse se posiciona sobre DEMO se pinta de amarillo y si se hace click se lanza el juego pero en modo DEMO que significa máquina vs  máquina
        if ((mousex >= X / 2 + 100 && mousex <= X / 2 + 200) && (mousey >= (RY * 250.0 / 768.0) && mousey <= (RY * 385.0 / 768.0))) {
            al_draw_scaled_bitmap(flecha1, 0, 0, 4623, 4623, X / 2 + 100, RY * 260 / 768, 125, 125, 0);
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (eventos.mouse.button & 1) {
                    //Si se presiona el click del mouse se inicia el juego y se indican las vidas totales y el nivel, si el juego termina se avanza de nivel hasta que se pierdan las vidas y el modo determina si es maquina vs maquina o jugador vs maquina
                    cantidadEspacios += 1;
                }
            }
        }
        if ((mousex >= X / 2 - 45 && mousex <= X / 2 + 45) && (mousey >= (RY * (415.0 / 768.0)) && mousey <= (RY * (450.0 / 768.0)))) {
            al_draw_text(fuente2, al_map_rgb(250, 250, 20), X / 2, (RY * (420.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Continuar");
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (eventos.mouse.button & 1) {
                    al_destroy_display(pantalla);
                    simulacion(modo, 1, cantidadEspacios);
                    hecho = false;
                }
            }
        }
        if ((mousex >= X / 2 - 45 && mousex <= X / 2 + 45) && (mousey >= (RY * (525.0 / 768.0)) && mousey <= (RY * (565.0 / 768.0)))) {
            //Si se presiona la  opción de salir se sale de la cola de eventos y termina la aplicación
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

void seleccionPlantas() {
    if (!al_init()) {
        al_show_native_message_box(NULL, "Ventana Emergente", "Error", "No se puede inicializar Allegro", NULL, NULL);
        return;
    }
    ALLEGRO_MONITOR_INFO monitor;
    al_get_monitor_info(0, &monitor);//Se obtiene la información del monitor como su resolución mediante diferencia de puntos finales e iniciales
    const int RX = monitor.x2 - monitor.x1 - 700;
    const int RY = monitor.y2 - monitor.y1 - 300;
    ALLEGRO_DISPLAY* pantalla = al_create_display(RX, RY);
    ALLEGRO_TIMER* timer1 = al_create_timer(1.0 / FPS);
    al_set_window_title(pantalla, "Selección de plantas");//Se le pone un título a la ventana
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
    const int maxFrame = 1;
    int frameactual = 0;
    int contadorDeFrames = 0;
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
            //Se registra la posición en x y y del mouse
            mousex = eventos.mouse.x;
            mousey = eventos.mouse.y;
        }
        if (eventos.type == ALLEGRO_EVENT_TIMER)
        {
            if (eventos.timer.source == timer1)
            {
                if (contadorDeFrames++ >= Delay) {//Se declara un contador de frames que logra avanzar la animación del menú principal
                    if (frameactual++ >= maxFrame) {
                        frameactual = 0;
                    }
                    contadorDeFrames = 0;
                }
                //Se dibuja el fondo animado y las opciones del menú en el display
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
        //Si se posiciona el mouse en las coordenadas donde indica la opción jugar
        if ((mousex >= X / 2 - 200 && mousex <= X / 2 - 100) && (mousey >= (RY * 250.0 / 768.0) && mousey <= (RY * 385.0 / 768.0))) {
            al_draw_scaled_bitmap(flecha1, 0, 0, 4623, 4623, X / 2 - 200, RY * 260 / 768, 125, 125, ALLEGRO_FLIP_HORIZONTAL);
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (eventos.mouse.button & 1) {
                    //Si se presiona el click del mouse se inicia el juego y se indican las vidas totales y el nivel, si el juego termina se avanza de nivel hasta que se pierdan las vidas y el modo determina si es maquina vs maquina o jugador vs maquina
                    modo = true;
                    cantidadPlantas -= 1;
                }
            }
        }
        //Si el mouse se posiciona sobre DEMO se pinta de amarillo y si se hace click se lanza el juego pero en modo DEMO que significa máquina vs  máquina
        if ((mousex >= X / 2 + 100 && mousex <= X / 2 + 200) && (mousey >= (RY * 250.0 / 768.0) && mousey <= (RY * 385.0 / 768.0))) {
            al_draw_scaled_bitmap(flecha1, 0, 0, 4623, 4623, X / 2 + 100, RY * 260 / 768, 125, 125, 0);
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (eventos.mouse.button & 1) {
                    //Si se presiona el click del mouse se inicia el juego y se indican las vidas totales y el nivel, si el juego termina se avanza de nivel hasta que se pierdan las vidas y el modo determina si es maquina vs maquina o jugador vs maquina
                    modo = true;
                    cantidadPlantas += 1;
                }
            }
        }
        if ((mousex >= X / 2 - 45 && mousex <= X / 2 + 45) && (mousey >= (RY * (415.0 / 768.0)) && mousey <= (RY * (450.0 / 768.0)))) {
            //Si se presiona la  opción de salir se sale de la cola de eventos y termina la aplicación
            al_draw_text(fuente2, al_map_rgb(250, 250, 20), X / 2, (RY * (420.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Continuar");
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (eventos.mouse.button & 1) {
                    al_destroy_display(pantalla);
                    simulacion(3, cantidadPlantas);
                    hecho = false;
                }
            }
        }
        if ((mousex >= X / 2 - 45 && mousex <= X / 2 + 45) && (mousey >= (RY * (525.0 / 768.0)) && mousey <= (RY * (565.0 / 768.0)))) {
            //Si se presiona la  opción de salir se sale de la cola de eventos y termina la aplicación
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
    
    int opcion;
    
    
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
    al_get_monitor_info(0, &monitor);//Se obtiene la información del monitor como su resolución mediante diferencia de puntos finales e iniciales
    const int RX = monitor.x2 - monitor.x1 - 700;
    const int RY = monitor.y2 - monitor.y1 - 300;
    ALLEGRO_DISPLAY* pantalla = al_create_display(RX, RY);
    al_set_window_title(pantalla, "Simulador de Parking");//Se le pone un título a la ventana
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
    const int maxFrame = 1;
    int frameactual = 0;
    int contadorDeFrames = 0;
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
            //Se registra la posición en x y y del mouse
            mousex = eventos.mouse.x;
            mousey = eventos.mouse.y;
        }
        if (eventos.type == ALLEGRO_EVENT_TIMER)
        {
            if (eventos.timer.source == timer)
            {
                if (contadorDeFrames++ >= Delay) {//Se declara un contador de frames que logra avanzar la animación del menú principal
                    if (frameactual++ >= maxFrame) {
                        frameactual = 0;
                    }
                    contadorDeFrames = 0;
                }
                //Se dibuja el fondo animado y las opciones del menú en el display
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_text(fuente1, al_map_rgb(250, 250, 250), X / 2, (RY * (100.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Simulador de Parking");
                al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * (250.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Simulador 1 solo piso en 1 planta");
                al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * (320.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Simulador 2 pisos en 1 planta");
                al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * (390.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Simulador n plantas 2 estacionamientos por planta");
                al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * (530.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "SALIR");
            }
        }
        //Si se posiciona el mouse en las coordenadas donde indica la opción jugar
        if ((mousex >= X / 2 - 150 && mousex <= X / 2 + 150) && (mousey >= (RY * 245.0 / 768.0) && mousey <= (RY * 285.0 / 768.0))) {
            al_draw_text(fuente2, al_map_rgb(250, 250, 20), X / 2, (RY * (250.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Simulador 1 solo piso en 1 planta");
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (eventos.mouse.button & 1) {
                    //Si se presiona el click del mouse se inicia el juego y se indican las vidas totales y el nivel, si el juego termina se avanza de nivel hasta que se pierdan las vidas y el modo determina si es maquina vs maquina o jugador vs maquina
                    al_destroy_display(pantalla);
                    seleccionEspacios(1);
                    main();//Se vuelve a lanzar el menú si se sale del juego
                    hecho = false;
                }
            }
        }
        //Si el mouse se posiciona sobre DEMO se pinta de amarillo y si se hace click se lanza el juego pero en modo DEMO que significa máquina vs  máquina
        if ((mousex >= X / 2 - 135 && mousex <= X / 2 + 135) && (mousey >= (RY * 315.0 / 768.0) && mousey <= (RY * 365.0 / 768.0))) {
            al_draw_text(fuente2, al_map_rgb(250, 250, 20), X / 2, (RY * (320.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Simulador 2 pisos en 1 planta");
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (eventos.mouse.button & 1) {
                    //Si se presiona el click del mouse se inicia el juego y se indican las vidas totales y el nivel, si el juego termina se avanza de nivel hasta que se pierdan las vidas y el modo determina si es maquina vs maquina o jugador vs maquina
                    al_destroy_display(pantalla);
                    seleccionEspacios(2);
                    main();//Se vuelve a lanzar el menú si se sale del juego
                    hecho = false;
                }
            }
        }
        //Si se posiciona el mouse sobre resultados este se tinta de amarillo y si se hace click lanza el menu de resultados donde se pueden ver todas las partidas anteriores
        if ((mousex >= X / 2 - 230 && mousex <= X / 2 + 230) && (mousey >= (RY * (385.0 / 768.0)) && mousey <= (RY * (425.0 / 768.0)))) {
            al_draw_text(fuente2, al_map_rgb(250, 250, 20), X / 2, RY * (390.0 / 768.0), ALLEGRO_ALIGN_CENTRE, "Simulador n plantas 2 estacionamientos por planta");
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (eventos.mouse.button & 1) {
                    //Si se presiona el click del mouse se inicia el juego y se indican las vidas totales y el nivel, si el juego termina se avanza de nivel hasta que se pierdan las vidas y el modo determina si es maquina vs maquina o jugador vs maquina
                    al_destroy_display(pantalla);
                    seleccionPlantas();
                    main();//Se vuelve a lanzar el menú si se sale del juego
                    hecho = false;
                }
            }
        }
        
        if ((mousex >= X / 2 - 45 && mousex <= X / 2 + 45) && (mousey >= (RY * (525.0 / 768.0)) && mousey <= (RY * (565.0 / 768.0)))) {
            //Si se presiona la  opción de salir se sale de la cola de eventos y termina la aplicación
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

   /* do {
        cout << "\nMenu:" << endl;
        cout << "1. Crear pisos" << endl;
        cout << "2. Sacar un vehiculo" << endl;
        cout << "3. Buscar un vehiculo" << endl;
        cout << "4. Ver ultimo vehiculo" << endl;
        cout << "5. Cantidad de vehiculos" << endl;
        cout << "6. Espacios vacios" << endl;
        cout << "7. Agregar vehiculo" << endl;
        cout << "8. Mostrar tipos de vehiculos" << endl;
        cout << "0. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1: {
            int pisosMotosBicicletas, pisosAutos4x4, pisosCamiones;
            cout << "Ingrese la cantidad de pisos para motos y bicicletas: ";
            cin >> pisosMotosBicicletas;
            cout << "Ingrese la cantidad de pisos para autos y 4x4: ";
            cin >> pisosAutos4x4;
            cout << "Ingrese la cantidad de pisos para camiones: ";
            cin >> pisosCamiones;
            parqueo = CrearParqueo(pisosMotosBicicletas, pisosAutos4x4, pisosCamiones);
            cout << "Pisos creados correctamente" << endl;
            break;
        }
        case 2: {
            char identificacion[20];
            cout << "Ingrese la identificacion del vehiculo a sacar: ";
            cin >> identificacion;
            SacarVehiculo(parqueo, identificacion);
            break;
        }
        case 3: {
            char identificacion[20];
            cout << "Ingrese la identificacion del vehiculo a buscar: ";
            cin >> identificacion;

            // Buscar el vehículo en cada piso
            Piso* pisoActual = parqueo;
            while (pisoActual != nullptr) {
                T_Vehiculo* vehiculoEncontrado = BuscarVehiculo(pisoActual->listaVehiculos, identificacion);
                if (vehiculoEncontrado != nullptr) {
                    cout << "Vehiculo encontrado:" << endl;
                    cout << "Identificacion: " << vehiculoEncontrado->identificacion << endl;
                    cout << "Peso: " << vehiculoEncontrado->peso << endl;
                    cout << "Tamano: " << vehiculoEncontrado->tamano << endl;
                    cout << "Tipo: " << vehiculoEncontrado->tipo << endl;
                    return 0;
                }
                pisoActual = pisoActual->siguiente;
            }
            cout << "No se encontro" << endl;
            break;
        }
        case 4:
            ObtenerUltimoVehiculo();
            break;
        case 5:
            ObtenerCantidadVehiculos();
            break;
        case 6:
            ObtenerEspaciosVacios();
            break;
        case 7: {
            if (parqueo == nullptr) {
                cout << "Primero debe crear los pisos del parqueo." << endl;
            }
            else {
                char identificacion[20];
                char tipo[20];
                float peso, tamano;

                cout << "Ingrese la identificacion del vehiculo: ";
                cin >> identificacion;
                cout << "Ingrese el tipo del vehiculo: ";
                cin >> tipo;
                cout << "Ingrese el tamano del vehiculo: ";
                cin >> tamano;
                cout << "Ingrese el peso del vehiculo: ";
                cin >> peso;

                AgregarVehiculo(parqueo, tipo, identificacion, peso, tamano);
                cout << "Vehiculo agregado exitosamente." << endl;
            }
            break;
        }
        case 8: {
            MostrarTiposVehiculos();
            break;
        }
        case 0:
            cout << "Saliendo del programa..." << endl;
            break;
        default:
            cout << "La opcion no es valida" << endl;
            break;
        }
    } while (opcion != 0);*/

    return 0;
}
