#include <iostream>
#include <cstring>
#include <ctime>

#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>


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
        if (strcmp(pisoActual->tipo, tipo) == 0) {
            AgregarInicioPiso(pisoActual->listaVehiculos, identificacion, peso, tamano, tipo);
            return;
        }
        pisoActual = pisoActual->siguiente;
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
    cout << "No se encontró un vehículo con la placa '" << identificacion << endl;
}


void MostrarTiposVehiculos() {
    cout << "Tipos de vehículos que admite el parqueo:" << endl;
    cout << "moto_bicicleta" << endl;
    cout << "automovil_4x4" << endl;
    cout << "camion" << endl;
}

int main() {
    InicializarParqueo();

    int opcion;
    Piso* parqueo = nullptr;
    
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
    const int RX = monitor.x2 - monitor.x1 - 500;
    const int RY = monitor.y2 - monitor.y1 - 500;


    ALLEGRO_DISPLAY* pantalla = al_create_display(RX, RY);

    al_set_window_title(pantalla, "Simulador de Parking");//Se le pone un título a la ventana

    do {
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
    } while (opcion != 0);

    return 0;
}
