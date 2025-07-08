#ifndef FUNCIONES_H
#define FUNCIONES_H

#define MAX_ZONAS 20
#define DIAS_HISTORIAL 7
#define NOMBRE_ZONA 40

typedef struct {
    char fecha[11]; // "YYYY-MM-DD"
    float pm25, pm10, co2, so2, no2;
    float temperatura, humedad, velocidad_viento;
} RegistroDia;

typedef struct {
    char nombre[NOMBRE_ZONA];
    int dias_registrados;
    RegistroDia historial[DIAS_HISTORIAL];
} Zona;

int cargar_zonas(Zona zonas[], int *num_zonas);
int guardar_zonas(Zona zonas[], int num_zonas);
void mostrar_menu();
void mostrar_estado_actual(Zona zonas[], int num_zonas);
void mostrar_predicciones(Zona zonas[], int num_zonas);
void ingresar_datos_actuales(Zona zonas[], int num_zonas);
void mostrar_info_zonas(Zona zonas[], int num_zonas);
void generar_alertas_y_recomendaciones(Zona zonas[], int num_zonas);
void generar_reporte(Zona zonas[], int num_zonas);
void exportar_respaldo(Zona zonas[], int num_zonas);
void anadir_zona(Zona zonas[], int *num_zonas);
void editar_zona(Zona zonas[], int num_zonas);
void eliminar_zona(Zona zonas[], int *num_zonas);
void reiniciar_programa();
int validar_float(float valor, float min, float max);
int leer_float(const char *mensaje, float min, float max, float *valor);
int leer_int(const char *mensaje, int min, int max, int *valor);
void limpiar_buffer();

#endif
