/*
 * SISTEMA INTEGRAL DE GESTION DE CONTAMINACION DEL AIRE - QUITO
 * 
 * Este archivo contiene todas las declaraciones (estructuras, constantes y funciones)
 * necesarias para el funcionamiento del sistema de monitoreo de contaminación del aire.
 * 
 * El sistema monitorea 5 zonas urbanas de Quito y permite:
 * - Registrar datos de contaminación en tiempo real
 * - Predecir niveles de contaminación para las próximas 24 horas
 * - Generar alertas cuando se superan los límites seguros
 * - Crear recomendaciones específicas para cada zona
 * - Generar reportes completos del estado de la calidad del aire
 */

#ifndef FUNCIONES_H
#define FUNCIONES_H

// Librerías estándar de C necesarias para el funcionamiento del sistema
#include <stdio.h>   // Para operaciones de entrada/salida (printf, scanf, archivos)
#include <string.h>  // Para manipulación de cadenas de texto (strcpy, strcmp)
#include <stdlib.h>  // Para conversiones de cadenas y manejo de memoria
#include <time.h>    // Para manejo de fechas y horas

/*
 * CONSTANTES DEL SISTEMA
 * Estas constantes definen los límites y configuraciones básicas del programa
 */
#define MAX_ZONAS 10                    // Máximo número de zonas que puede monitorear el sistema
#define MAX_DIAS_HISTORICOS 7           // Máximo de días de historial que se almacenan por zona
#define MAX_NOMBRE_ZONA 50              // Máximo de caracteres para el nombre de una zona
#define MAX_LINEA 200                   // Máximo de caracteres por línea en archivos
#define ARCHIVO_DATOS "datos_contaminacion.dat"      // Nombre del archivo binario principal
#define ARCHIVO_REPORTE "reporte_contaminacion.txt"  // Nombre del archivo de reportes

/*
 * LIMITES DE CONTAMINACION SEGUN LA ORGANIZACION MUNDIAL DE LA SALUD (OMS)
 * Valores actualizados según las nuevas directrices de la OMS (2021)
 * Estos valores están en microgramos por metro cúbico (ug/m3)
 * Si los niveles superan estos límites, se considera peligroso para la salud
 */
#define LIMITE_PM25 15.0      // Partículas finas menores a 2.5 micrómetros (OMS 2021: promedio anual 15 μg/m³, diario 45 μg/m³)
#define LIMITE_PM10 45.0      // Partículas menores a 10 micrómetros (OMS 2021: promedio anual 45 μg/m³, diario 100 μg/m³)
#define LIMITE_CO2 1000.0     // Dióxido de carbono en espacios interiores (recomendación: <1000 ppm ≈ 1800 mg/m³)
#define LIMITE_SO2 40.0       // Dióxido de azufre (OMS: promedio diario 40 μg/m³)
#define LIMITE_NO2 25.0       // Dióxido de nitrógeno (OMS 2021: promedio anual 25 μg/m³, diario 50 μg/m³)

/*
 * ESTRUCTURA: DatosContaminacion
 * 
 * Esta estructura almacena todos los datos de contaminación y clima de un día específico.
 * Incluye la fecha y las mediciones de los 5 contaminantes principales, más datos climáticos.
 */
typedef struct {
    // Fecha del registro
    int dia;                    // Día del mes (1-31)
    int mes;                    // Mes del año (1-12)
    int ano;                    // Año completo (ej: 2025)
    
    // Contaminantes del aire (medidos en microgramos por metro cúbico)
    float pm25;                 // Partículas PM2.5 - las más peligrosas para los pulmones
    float pm10;                 // Partículas PM10 - polvo y partículas grandes
    float co2;                  // Dióxido de carbono - gas de efecto invernadero
    float so2;                  // Dióxido de azufre - principalmente de industrias
    float no2;                  // Dióxido de nitrógeno - principalmente de vehículos
    
    // Condiciones climáticas que afectan la contaminación
    float temperatura;          // Temperatura en grados Celsius
    float humedad;              // Humedad relativa en porcentaje (0-100%)
    float velocidad_viento;     // Velocidad del viento en km/h
} DatosContaminacion;

/*
 * ESTRUCTURA: ZonaMonitoreo
 * 
 * Representa una zona específica de la ciudad que está siendo monitoreada.
 * Cada zona tiene su ubicación, datos actuales, historial y predicciones.
 */
typedef struct {
    // Identificación de la zona
    int id;                                    // Número único de identificación (1-5)
    char nombre[MAX_NOMBRE_ZONA];              // Nombre descriptivo de la zona
    float latitud;                             // Coordenada de latitud (posición norte-sur)
    float longitud;                            // Coordenada de longitud (posición este-oeste)
    
    // Datos de contaminación
    DatosContaminacion datos_actuales;         // Última medición registrada
    DatosContaminacion historial[MAX_DIAS_HISTORICOS];  // Historial de hasta 7 días
    int num_registros_historicos;              // Número actual de días en el historial
    
    // Predicciones para las próximas 24 horas (orden: PM2.5, PM10, CO2, SO2, NO2)
    float prediccion_24h[5];                   // Array con las 5 predicciones de contaminantes
} ZonaMonitoreo;

/*
 * ESTRUCTURA: SistemaContaminacion
 * 
 * Esta es la estructura principal que contiene todo el sistema de monitoreo.
 * Incluye todas las zonas monitoreadas y la información general del sistema.
 */
typedef struct {
    ZonaMonitoreo zonas[MAX_ZONAS];     // Array con todas las zonas monitoreadas
    int num_zonas;                      // Número actual de zonas registradas en el sistema
    time_t ultima_actualizacion;        // Timestamp de la última actualización de datos
} SistemaContaminacion;

/*
 * ESTRUCTURA: Recomendacion
 * 
 * Almacena las recomendaciones que genera el sistema cuando detecta
 * niveles peligrosos de contaminación en alguna zona.
 */
typedef struct {
    char descripcion[200];              // Texto descriptivo de la recomendación
    int prioridad;                      // Nivel de prioridad: 1=Alta, 2=Media, 3=Baja
    char categoria[50];                 // Categoría: "Trafico", "Industrial", "Salud Publica"
} Recomendacion;

/*
 * ============================================================================
 * DECLARACIONES DE FUNCIONES
 * ============================================================================
 * 
 * Las siguientes son las "firmas" de todas las funciones del sistema.
 * Esto permite que el compilador conozca qué funciones existen y cómo usarlas.
 * Las implementaciones reales están en el archivo funciones.c
 */

/* 
 * FUNCIONES PRINCIPALES DEL SISTEMA
 * Estas funciones manejan la inicialización y el almacenamiento de datos
 */

// Inicializa el sistema con las 5 zonas de Quito y configuración inicial
void inicializar_sistema(SistemaContaminacion *sistema);

// Carga los datos guardados desde el archivo binario al iniciar el programa
void cargar_datos_archivo(SistemaContaminacion *sistema);

// Guarda todos los datos del sistema en el archivo binario
void guardar_datos_archivo(SistemaContaminacion *sistema);

// Agrega una nueva zona de monitoreo al sistema (con coordenadas GPS)
void agregar_zona(SistemaContaminacion *sistema, int id, char *nombre, float lat, float lng);

// Permite al usuario ingresar nuevos datos de contaminación para una zona específica
void ingresar_datos_actuales(SistemaContaminacion *sistema, int zona_id);

// Agrega datos al historial de una zona (función interna para el manejo de datos)
void agregar_datos_historicos(SistemaContaminacion *sistema, int zona_id, DatosContaminacion datos);

// Edita los datos de contaminación actuales de una zona específica
void editar_datos_zona(SistemaContaminacion *sistema, int zona_id);

// Elimina completamente una zona del sistema de monitoreo
int eliminar_zona(SistemaContaminacion *sistema, int zona_id);

// Valida que un valor numérico esté en el rango permitido y no sea negativo (excepto temperatura)
int validar_entrada_numerica(float *valor, float min, float max, int puede_ser_negativo);

// Valida y lee un número flotante del usuario con reintentos en caso de entrada inválida
int leer_float_validado(float *valor, char *mensaje, float min, float max, int puede_ser_negativo);

/*
 * FUNCIONES DE ANALISIS Y PREDICCION
 * Estas funciones procesan los datos para generar análisis y predicciones
 */

// Calcula el promedio histórico de un contaminante específico en una zona
// tipo_contaminante: 0=PM2.5, 1=PM10, 2=CO2, 3=SO2, 4=NO2
float calcular_promedio_historico(ZonaMonitoreo *zona, int tipo_contaminante);

// Genera predicciones de contaminación para las próximas 24 horas
// Usa algoritmos que consideran historial y condiciones climáticas
void predecir_contaminacion_24h(ZonaMonitoreo *zona);

// Verifica si los datos actuales superan los límites de seguridad de la OMS
// Retorna 1 si hay exceso, 0 si está dentro de límites normales
int verificar_limites_actuales(DatosContaminacion *datos);

// Verifica si las predicciones superarán los límites de seguridad
// Retorna 1 si se prevé exceso, 0 si se mantendrá en límites normales
int verificar_limites_prediccion(float *prediccion);

/*
 * FUNCIONES DE ALERTAS Y RECOMENDACIONES
 * Estas funciones generan avisos y consejos cuando hay problemas de contaminación
 */

// Analiza todas las zonas y genera alertas si hay niveles peligrosos
// Muestra tanto alertas actuales como preventivas (basadas en predicciones)
void generar_alertas(SistemaContaminacion *sistema);

// Genera recomendaciones específicas para mejorar la calidad del aire en una zona
// Las recomendaciones se almacenan en el array proporcionado
void generar_recomendaciones(SistemaContaminacion *sistema, int zona_id, Recomendacion *recomendaciones, int *num_recomendaciones);

// Muestra una alerta individual detallada (función auxiliar)
void mostrar_alerta(int zona_id, char *nombre_zona, char *contaminante, float valor, float limite);

/*
 * FUNCIONES DE REPORTES Y EXPORTACION
 * Estas funciones generan reportes completos y exportan datos
 */

// Genera un reporte completo de texto con toda la información del sistema
// El reporte se guarda en el archivo "reporte_contaminacion.txt"
void generar_reporte_completo(SistemaContaminacion *sistema);

// Exporta todos los datos del sistema a un archivo binario de respaldo
// Útil para hacer copias de seguridad o análisis externos
void exportar_datos_binario(SistemaContaminacion *sistema);

// Muestra en pantalla el estado actual de contaminación de todas las zonas
void mostrar_estado_actual(SistemaContaminacion *sistema);

// Muestra en pantalla las predicciones de contaminación para las próximas 24 horas
void mostrar_predicciones(SistemaContaminacion *sistema);

/*
 * FUNCIONES DE MENU E INTERFAZ DE USUARIO
 * Estas funciones manejan la interacción con el usuario
 */

// Muestra el menú principal con todas las opciones disponibles
void mostrar_menu_principal();

// Muestra información resumida de todas las zonas monitoreadas
void mostrar_menu_zonas(SistemaContaminacion *sistema);

// Muestra información detallada de una zona específica
void mostrar_datos_zona(ZonaMonitoreo *zona);

// Reinicia completamente el programa, eliminando todos los datos guardados
void reiniciar_programa();

/*
 * FUNCIONES AUXILIARES Y DE UTILIDAD
 * Estas funciones proporcionan cálculos y utilidades que apoyan al sistema principal
 */

// Calcula un índice general de calidad del aire basado en todos los contaminantes
// Retorna un número que representa qué tan contaminado está el aire (0-500+)
float calcular_indice_calidad_aire(DatosContaminacion *datos);

// Convierte el índice numérico a una categoría comprensible
// Retorna texto como "Buena", "Moderada", "Insalubre", etc.
char* obtener_categoria_calidad(float indice);

// Obtiene la fecha actual del sistema y la desglosa en día, mes y año
void obtener_fecha_actual(int *dia, int *mes, int *ano);

// Aplica factores climáticos a las predicciones de contaminación
// El clima (temperatura, humedad, viento) afecta cómo se acumulan los contaminantes
float aplicar_factor_climatico(float valor_base, float temperatura, float humedad, float viento);

#endif // FUNCIONES_H

/*
 * ================================================================================================
 * FIN DEL ARCHIVO DE CABECERAS
 * ================================================================================================
 * 
 * Este archivo define la estructura completa del sistema de monitoreo de contaminación del aire.
 * 
 * RESUMEN DEL SISTEMA:
 * - Monitorea 5 zonas específicas de Quito
 * - Registra 5 tipos de contaminantes (PM2.5, PM10, CO2, SO2, NO2) 
 * - Considera condiciones climáticas que afectan la contaminación
 * - Genera predicciones para 24 horas usando algoritmos que consideran el historial
 * - Alerta cuando se superan los límites seguros establecidos por la OMS
 * - Crea recomendaciones específicas para mejorar la calidad del aire
 * - Guarda todos los datos en archivos binarios para persistencia
 * - Permite generar reportes completos para análisis
 * 
 * Para usar este sistema, compile junto con funciones.c y main.c
 */ 