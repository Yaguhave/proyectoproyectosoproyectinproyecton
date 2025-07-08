/*
 * SISTEMA DE GESTION DE CONTAMINACION DEL AIRE - QUITO
 * 
 * Este archivo contiene las declaraciones principales del sistema:
 * - Estructuras de datos
 * - Constantes importantes  
 * - Declaraciones de funciones
 */

#ifndef FUNCIONES_H
#define FUNCIONES_H

// Librerías estándar de C
#include <stdio.h>   // Para entrada/salida (printf, scanf, archivos)
#include <string.h>  // Para manipulación de cadenas (strcpy, strcmp)
#include <stdlib.h>  // Para conversiones y manejo de memoria
#include <time.h>    // Para fechas y horas

// CONSTANTES DEL SISTEMA
#define MAX_ZONAS 10                    // Máximo número de zonas
#define MAX_DIAS_HISTORICOS 7           // Máximo de días de historial
#define MAX_NOMBRE_ZONA 50              // Máximo de caracteres para nombre
#define MAX_LINEA 200                   // Máximo de caracteres por línea
#define ARCHIVO_DATOS "datos_contaminacion.dat"      // Archivo principal
#define ARCHIVO_REPORTE "reporte_contaminacion.txt"  // Archivo de reportes

// LIMITES DE CONTAMINACION SEGUN LA OMS (2021)
// Valores en microgramos por metro cúbico (ug/m3)
#define LIMITE_PM25 15.0      // Partículas finas menores a 2.5 micrómetros
#define LIMITE_PM10 45.0      // Partículas menores a 10 micrómetros
#define LIMITE_CO2 1000.0     // Dióxido de carbono
#define LIMITE_SO2 40.0       // Dióxido de azufre
#define LIMITE_NO2 25.0       // Dióxido de nitrógeno

// ESTRUCTURA: DatosContaminacion
// Almacena datos de contaminación y clima de un día específico
typedef struct {
    // Fecha del registro
    int dia, mes, ano;
    
    // Contaminantes del aire (en microgramos por metro cúbico)
    float pm25;                 // Partículas PM2.5
    float pm10;                 // Partículas PM10
    float co2;                  // Dióxido de carbono
    float so2;                  // Dióxido de azufre
    float no2;                  // Dióxido de nitrógeno
    
    // Condiciones climáticas
    float temperatura;          // Temperatura en grados Celsius
    float humedad;              // Humedad relativa en porcentaje
    float velocidad_viento;     // Velocidad del viento en km/h
} DatosContaminacion;

// ESTRUCTURA: ZonaMonitoreo
// Representa una zona específica de la ciudad que está siendo monitoreada
typedef struct {
    // Identificación de la zona
    int id;                                    // Número único de identificación
    char nombre[MAX_NOMBRE_ZONA];              // Nombre descriptivo de la zona
    float latitud, longitud;                   // Coordenadas GPS
    
    // Datos de contaminación
    DatosContaminacion datos_actuales;         // Última medición registrada
    DatosContaminacion historial[MAX_DIAS_HISTORICOS];  // Historial de días
    int num_registros_historicos;              // Número actual de días en el historial
    
    // Predicciones para las próximas 24 horas (PM2.5, PM10, CO2, SO2, NO2)
    float prediccion_24h[5];
} ZonaMonitoreo;

// ESTRUCTURA: SistemaContaminacion
// Estructura principal que contiene todo el sistema de monitoreo
typedef struct {
    ZonaMonitoreo zonas[MAX_ZONAS];     // Array con todas las zonas monitoreadas
    int num_zonas;                      // Número actual de zonas registradas
    time_t ultima_actualizacion;        // Timestamp de la última actualización
} SistemaContaminacion;

// ESTRUCTURA: Recomendacion
// Almacena recomendaciones cuando hay niveles peligrosos de contaminación
typedef struct {
    char descripcion[200];              // Texto de la recomendación
    int prioridad;                      // Nivel: 1=Alta, 2=Media, 3=Baja
    char categoria[50];                 // Categoría: "Trafico", "Industrial", etc.
} Recomendacion;

// DECLARACIONES DE FUNCIONES

// Funciones principales del sistema
void inicializar_sistema(SistemaContaminacion *sistema);
void cargar_datos_archivo(SistemaContaminacion *sistema);
void guardar_datos_archivo(SistemaContaminacion *sistema);
void agregar_zona(SistemaContaminacion *sistema, int id, char *nombre, float lat, float lng);
void ingresar_datos_actuales(SistemaContaminacion *sistema, int zona_id);
void agregar_datos_historicos(SistemaContaminacion *sistema, int zona_id, DatosContaminacion datos);

// Funciones de edición y validación
void editar_datos_zona(SistemaContaminacion *sistema, int zona_id);
int eliminar_zona(SistemaContaminacion *sistema, int zona_id);
int leer_float_validado(float *valor, char *mensaje, float min, float max, int puede_ser_negativo);

// Funciones de análisis y predicción
float calcular_promedio_historico(ZonaMonitoreo *zona, int tipo_contaminante);
void predecir_contaminacion_24h(ZonaMonitoreo *zona);
int verificar_limites_actuales(DatosContaminacion *datos);
int verificar_limites_prediccion(float *prediccion);

// Funciones de alertas y recomendaciones
void generar_alertas(SistemaContaminacion *sistema);
void generar_recomendaciones(SistemaContaminacion *sistema, int zona_id, Recomendacion *recomendaciones, int *num_recomendaciones);
void mostrar_alerta(int zona_id, char *nombre_zona, char *contaminante, float valor, float limite);

// Funciones de reportes y exportación
void generar_reporte_completo(SistemaContaminacion *sistema);
void exportar_datos_binario(SistemaContaminacion *sistema);
void mostrar_estado_actual(SistemaContaminacion *sistema);
void mostrar_predicciones(SistemaContaminacion *sistema);

// Funciones de menú e interfaz
void mostrar_menu_principal();
void mostrar_menu_zonas(SistemaContaminacion *sistema);
void mostrar_datos_zona(ZonaMonitoreo *zona);
void reiniciar_programa();

// Funciones auxiliares
float calcular_indice_calidad_aire(DatosContaminacion *datos);
char* obtener_categoria_calidad(float indice);
void obtener_fecha_actual(int *dia, int *mes, int *ano);
float aplicar_factor_climatico(float valor_base, float temperatura, float humedad, float viento);

#endif // FUNCIONES_H

/*
 * FIN DEL ARCHIVO DE CABECERAS
 * 
 * Este archivo define la estructura del sistema de monitoreo.
 * Para usar este sistema, compile junto con funciones.c y main.c
 */ 