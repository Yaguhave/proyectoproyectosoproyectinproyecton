/*
 * SISTEMA INTEGRAL DE GESTION DE CONTAMINACION DEL AIRE - QUITO
 * ARCHIVO DE IMPLEMENTACION DE FUNCIONES
 * 
 * Este archivo contiene la implementación real de todas las funciones declaradas
 * en funciones.h. Aquí está la lógica completa del sistema de monitoreo.
 * 
 * ORGANIZACION DEL ARCHIVO:
 * 1. Funciones matemáticas auxiliares (reemplazan la librería math.h)
 * 2. Funciones principales del sistema (inicialización, carga/guardado)
 * 3. Funciones de análisis y predicción
 * 4. Funciones de alertas y recomendaciones
 * 5. Funciones de reportes y exportación
 * 6. Funciones de interfaz de usuario
 * 7. Funciones auxiliares y utilidades
 */

#include "funciones.h"

/*
 * ============================================================================
 * FUNCIONES MATEMATICAS AUXILIARES
 * ============================================================================
 * 
 * Estas funciones reemplazan funciones de la librería math.h para evitar
 * dependencias externas. Implementan operaciones matemáticas básicas necesarias.
 */

/**
 * Calcula la potencia de un número (base^exponente)
 * @param base: número base
 * @param exponente: exponente entero positivo
 * @return: resultado de base elevado a la potencia del exponente
 */
float potencia(float base, int exponente) {
    float resultado = 1.0;
    // Multiplica la base por sí misma 'exponente' veces
    for (int i = 0; i < exponente; i++) {
        resultado *= base;
    }
    return resultado;
}

/**
 * Calcula la raíz cuadrada de un número usando el método de Newton
 * @param numero: número del cual calcular la raíz cuadrada
 * @return: raíz cuadrada del número, o -1 si el número es negativo
 */

float raiz_cuadrada(float numero) {
    if (numero < 0) return -1; // Error: no se puede calcular raíz de número negativo
    if (numero == 0) return 0; // Caso especial: raíz de 0 es 0
    
    // Algoritmo de Newton para aproximar la raíz cuadrada
    float x = numero;                    // Estimación inicial
    float y = 1.0;                      // Segunda estimación
    float epsilon = 0.000001;           // Precisión deseada
    
    // Itera hasta alcanzar la precisión deseada
    while (x - y > epsilon) {
        x = (x + y) / 2;                // Nueva estimación (promedio)
        y = numero / x;                 // Ajuste basado en el número original
    }
    return x;
}

/**
 * Calcula el valor absoluto de un número (siempre positivo)
 * @param numero: número del cual obtener el valor absoluto
 * @return: valor absoluto del número
 */

float valor_absoluto(float numero) {
    // Si es negativo, lo convierte a positivo; si ya es positivo, lo deja igual
    return (numero < 0) ? -numero : numero;
}

/*
 * ============================================================================
 * FUNCIONES PRINCIPALES DEL SISTEMA
 * ============================================================================
 */

/**
 * Inicializa el sistema de monitoreo con la configuración inicial
 * Configura las 5 zonas de monitoreo de Quito con sus coordenadas GPS reales
 * @param sistema: puntero al sistema principal que se va a inicializar
 */
void inicializar_sistema(SistemaContaminacion *sistema) {
    // Inicializar contadores y timestamps
    sistema->num_zonas = 0;                          // Comenzar sin zonas registradas
    sistema->ultima_actualizacion = time(NULL);      // Marcar la hora de inicialización
    
    // Agregar las 5 zonas específicas de Quito con sus coordenadas GPS reales
    // Estas coordenadas corresponden a ubicaciones estratégicas de la ciudad
    agregar_zona(sistema, 1, "Sector Udlapark", -0.2298, -78.5249);        // Zona norte
    agregar_zona(sistema, 2, "Mitad del Mundo", -0.0022, -78.4558);        // Zona norte extrema
    agregar_zona(sistema, 3, "Parque la Carolina", -0.1807, -78.4678);     // Zona centro-norte
    agregar_zona(sistema, 4, "El Panecillo", -0.2274, -78.5170);           // Zona centro histórico
    agregar_zona(sistema, 5, "Catedral Metropolitana", -0.2201, -78.5123); // Zona centro histórico
}

/**
 * Carga los datos guardados desde el archivo binario al sistema
 * Se ejecuta al inicio del programa para recuperar datos anteriores
 * @param sistema: puntero al sistema donde cargar los datos
 */
void cargar_datos_archivo(SistemaContaminacion *sistema) {
    FILE *archivo = fopen(ARCHIVO_DATOS, "rb");    // Abrir archivo en modo lectura binaria
    if (archivo == NULL) {
        // Si no existe el archivo, es la primera vez que se ejecuta el programa
        printf("No se encontro archivo de datos existente. Iniciando con datos nuevos.\n");
        return;
    }
    
    // Leer la estructura completa del archivo binario
    fread(&sistema->num_zonas, sizeof(int), 1, archivo);           // Leer número de zonas
    fread(sistema->zonas, sizeof(ZonaMonitoreo), sistema->num_zonas, archivo);  // Leer todas las zonas
    fread(&sistema->ultima_actualizacion, sizeof(time_t), 1, archivo);         // Leer timestamp
    
    fclose(archivo);
    printf("Datos historicos cargados exitosamente desde archivo binario.\n");
}

/**
 * Guarda todos los datos del sistema en un archivo binario
 * Se ejecuta automáticamente después de cambios importantes
 * @param sistema: puntero al sistema cuyos datos se van a guardar
 */
void guardar_datos_archivo(SistemaContaminacion *sistema) {
    FILE *archivo = fopen(ARCHIVO_DATOS, "wb");    // Abrir archivo en modo escritura binaria
    if (archivo == NULL) {
        printf("Error al crear archivo de datos binario.\n");
        return;
    }
    
    // Escribir toda la estructura del sistema al archivo binario
    fwrite(&sistema->num_zonas, sizeof(int), 1, archivo);           // Escribir número de zonas
    fwrite(sistema->zonas, sizeof(ZonaMonitoreo), sistema->num_zonas, archivo);  // Escribir todas las zonas
    fwrite(&sistema->ultima_actualizacion, sizeof(time_t), 1, archivo);         // Escribir timestamp
    
    fclose(archivo);
    printf("Datos guardados exitosamente en archivo binario.\n");
}

/**
 * Agrega una nueva zona de monitoreo al sistema
 * @param sistema: puntero al sistema principal
 * @param id: identificador único de la zona (1-5)
 * @param nombre: nombre descriptivo de la zona
 * @param lat: coordenada de latitud GPS
 * @param lng: coordenada de longitud GPS
 */
void agregar_zona(SistemaContaminacion *sistema, int id, char *nombre, float lat, float lng) {
    // Verificar que no se exceda el límite máximo de zonas
    if (sistema->num_zonas >= MAX_ZONAS) {
        printf("No se pueden agregar más zonas. Límite alcanzado (%d zonas).\n", MAX_ZONAS);
        return;
    }
    
    // Obtener referencia a la nueva zona en el array
    ZonaMonitoreo *zona = &sistema->zonas[sistema->num_zonas];
    
    // Configurar información básica de la zona
    zona->id = id;                              // Asignar ID único
    strcpy(zona->nombre, nombre);               // Copiar nombre de la zona
    zona->latitud = lat;                        // Coordenada GPS de latitud
    zona->longitud = lng;                       // Coordenada GPS de longitud
    zona->num_registros_historicos = 0;         // Inicializar sin historial
    
    // Inicializar datos actuales con valores por defecto (cero)
    // Esto indica que aún no se han ingresado datos reales
    zona->datos_actuales.dia = 0;
    zona->datos_actuales.mes = 0;
    zona->datos_actuales.ano = 0;
    zona->datos_actuales.pm25 = 0.0;
    zona->datos_actuales.pm10 = 0.0;
    zona->datos_actuales.co2 = 0.0;
    zona->datos_actuales.so2 = 0.0;
    zona->datos_actuales.no2 = 0.0;
    zona->datos_actuales.temperatura = 0.0;
    zona->datos_actuales.humedad = 0.0;
    zona->datos_actuales.velocidad_viento = 0.0;
    
    // Inicializar predicciones con valores por defecto
    for (int i = 0; i < 5; i++) {
        zona->prediccion_24h[i] = 0.0;
    }
    
    // Incrementar el contador de zonas del sistema
    sistema->num_zonas++;
}

/**
 * Permite al usuario ingresar datos actuales de contaminación para una zona específica
 * Esta función es interactiva y solicita al usuario todos los valores necesarios
 * @param sistema: puntero al sistema principal
 * @param zona_id: ID de la zona para la cual ingresar datos
 */
void ingresar_datos_actuales(SistemaContaminacion *sistema, int zona_id) {
    ZonaMonitoreo *zona = NULL;
    
    // Buscar la zona
    for (int i = 0; i < sistema->num_zonas; i++) {
        if (sistema->zonas[i].id == zona_id) {
            zona = &sistema->zonas[i];
            break;
        }
    }
    
    if (zona == NULL) {
        printf("Zona no encontrada.\n");
        return;
    }
    
    printf("\n=== Ingreso de datos para zona: %s ===\n", zona->nombre);
    
    // Obtener fecha actual
    int dia, mes, ano;
    obtener_fecha_actual(&dia, &mes, &ano);
    
    zona->datos_actuales.dia = dia;
    zona->datos_actuales.mes = mes;
    zona->datos_actuales.ano = ano;
    
    printf("Ingrese los siguientes datos de contaminacion:\n");
    printf("PM2.5 (ug/m3): ");
    scanf("%f", &zona->datos_actuales.pm25);
    
    printf("Ingrese PM10 (ug/m3): ");
    scanf("%f", &zona->datos_actuales.pm10);
    
    printf("Ingrese CO2 (ug/m3): ");
    scanf("%f", &zona->datos_actuales.co2);
    
    printf("Ingrese SO2 (ug/m3): ");
    scanf("%f", &zona->datos_actuales.so2);
    
    printf("Ingrese NO2 (ug/m3): ");
    scanf("%f", &zona->datos_actuales.no2);
    
    printf("Ingrese temperatura (C): ");
    scanf("%f", &zona->datos_actuales.temperatura);
    
    printf("Ingrese humedad (%%): ");
    scanf("%f", &zona->datos_actuales.humedad);
    
    printf("Ingrese velocidad del viento (km/h): ");
    scanf("%f", &zona->datos_actuales.velocidad_viento);
    
    printf("Datos ingresados exitosamente.\n");
    
    // Actualizar predicción
    predecir_contaminacion_24h(zona);
}

// Función para agregar datos al historial
void agregar_datos_historicos(SistemaContaminacion *sistema, int zona_id, DatosContaminacion datos) {
    ZonaMonitoreo *zona = NULL;
    
    // Buscar la zona
    for (int i = 0; i < sistema->num_zonas; i++) {
        if (sistema->zonas[i].id == zona_id) {
            zona = &sistema->zonas[i];
            break;
        }
    }
    
    if (zona == NULL) return;
    
    // Si el historial está lleno, mover todos los elementos hacia atrás
    if (zona->num_registros_historicos >= MAX_DIAS_HISTORICOS) {
        for (int i = 0; i < MAX_DIAS_HISTORICOS - 1; i++) {
            zona->historial[i] = zona->historial[i + 1];
        }
        zona->historial[MAX_DIAS_HISTORICOS - 1] = datos;
    } else {
        zona->historial[zona->num_registros_historicos] = datos;
        zona->num_registros_historicos++;
    }
}

// Función para calcular promedio histórico de un contaminante
float calcular_promedio_historico(ZonaMonitoreo *zona, int tipo_contaminante) {
    if (zona->num_registros_historicos == 0) return 0.0;
    
    float suma = 0.0;
    for (int i = 0; i < zona->num_registros_historicos; i++) {
        switch (tipo_contaminante) {
            case 0: suma += zona->historial[i].pm25; break;
            case 1: suma += zona->historial[i].pm10; break;
            case 2: suma += zona->historial[i].co2; break;
            case 3: suma += zona->historial[i].so2; break;
            case 4: suma += zona->historial[i].no2; break;
        }
    }
    
    return suma / zona->num_registros_historicos;
}

// Función para predecir contaminación en 24h usando promedio ponderado
void predecir_contaminacion_24h(ZonaMonitoreo *zona) {
    if (zona->num_registros_historicos == 0) {
        // Sin historial, usar datos actuales como predicción
        zona->prediccion_24h[0] = zona->datos_actuales.pm25;
        zona->prediccion_24h[1] = zona->datos_actuales.pm10;
        zona->prediccion_24h[2] = zona->datos_actuales.co2;
        zona->prediccion_24h[3] = zona->datos_actuales.so2;
        zona->prediccion_24h[4] = zona->datos_actuales.no2;
        return;
    }
    
    // Calcular promedio ponderado (más peso a días recientes)
    float pesos_total = 0.0;
    for (int tipo = 0; tipo < 5; tipo++) {
        float suma_ponderada = 0.0;
        pesos_total = 0.0;
        
        for (int i = 0; i < zona->num_registros_historicos; i++) {
            // Peso mayor para días más recientes
            float peso = 1.0 + (float)i / zona->num_registros_historicos;
            float valor = 0.0;
            
            switch (tipo) {
                case 0: valor = zona->historial[i].pm25; break;
                case 1: valor = zona->historial[i].pm10; break;
                case 2: valor = zona->historial[i].co2; break;
                case 3: valor = zona->historial[i].so2; break;
                case 4: valor = zona->historial[i].no2; break;
            }
            
            suma_ponderada += valor * peso;
            pesos_total += peso;
        }
        
        float prediccion_base = suma_ponderada / pesos_total;
        
        // Aplicar factor climático
        zona->prediccion_24h[tipo] = aplicar_factor_climatico(
            prediccion_base,
            zona->datos_actuales.temperatura,
            zona->datos_actuales.humedad,
            zona->datos_actuales.velocidad_viento
        );
    }
}

// Función para verificar límites actuales
int verificar_limites_actuales(DatosContaminacion *datos) {
    int excede = 0;
    
    if (datos->pm25 > LIMITE_PM25) excede = 1;
    if (datos->pm10 > LIMITE_PM10) excede = 1;
    if (datos->co2 > LIMITE_CO2) excede = 1;
    if (datos->so2 > LIMITE_SO2) excede = 1;
    if (datos->no2 > LIMITE_NO2) excede = 1;
    
    return excede;
}

// Función para verificar límites de predicción
int verificar_limites_prediccion(float *prediccion) {
    int excede = 0;
    
    if (prediccion[0] > LIMITE_PM25) excede = 1;
    if (prediccion[1] > LIMITE_PM10) excede = 1;
    if (prediccion[2] > LIMITE_CO2) excede = 1;
    if (prediccion[3] > LIMITE_SO2) excede = 1;
    if (prediccion[4] > LIMITE_NO2) excede = 1;
    
    return excede;
}

// Función para generar alertas del sistema
void generar_alertas(SistemaContaminacion *sistema) {
    printf("\n=== SISTEMA DE ALERTAS ===\n");
    int alertas_generadas = 0;
    
    for (int i = 0; i < sistema->num_zonas; i++) {
        ZonaMonitoreo *zona = &sistema->zonas[i];
        
        // Verificar límites actuales
        if (verificar_limites_actuales(&zona->datos_actuales)) {
            printf("\nALERTA ACTUAL - Zona: %s\n", zona->nombre);
            
            if (zona->datos_actuales.pm25 > LIMITE_PM25) {
                mostrar_alerta(zona->id, zona->nombre, "PM2.5", zona->datos_actuales.pm25, LIMITE_PM25);
            }
            if (zona->datos_actuales.pm10 > LIMITE_PM10) {
                mostrar_alerta(zona->id, zona->nombre, "PM10", zona->datos_actuales.pm10, LIMITE_PM10);
            }
            if (zona->datos_actuales.co2 > LIMITE_CO2) {
                mostrar_alerta(zona->id, zona->nombre, "CO2", zona->datos_actuales.co2, LIMITE_CO2);
            }
            if (zona->datos_actuales.so2 > LIMITE_SO2) {
                mostrar_alerta(zona->id, zona->nombre, "SO2", zona->datos_actuales.so2, LIMITE_SO2);
            }
            if (zona->datos_actuales.no2 > LIMITE_NO2) {
                mostrar_alerta(zona->id, zona->nombre, "NO2", zona->datos_actuales.no2, LIMITE_NO2);
            }
            alertas_generadas++;
        }
        
        // Verificar límites de predicción
        if (verificar_limites_prediccion(zona->prediccion_24h)) {
            printf("\nALERTA PREVENTIVA (24h) - Zona: %s\n", zona->nombre);
            
            if (zona->prediccion_24h[0] > LIMITE_PM25) {
                mostrar_alerta(zona->id, zona->nombre, "PM2.5 (prediccion)", zona->prediccion_24h[0], LIMITE_PM25);
            }
            if (zona->prediccion_24h[1] > LIMITE_PM10) {
                mostrar_alerta(zona->id, zona->nombre, "PM10 (prediccion)", zona->prediccion_24h[1], LIMITE_PM10);
            }
            if (zona->prediccion_24h[2] > LIMITE_CO2) {
                mostrar_alerta(zona->id, zona->nombre, "CO2 (prediccion)", zona->prediccion_24h[2], LIMITE_CO2);
            }
            if (zona->prediccion_24h[3] > LIMITE_SO2) {
                mostrar_alerta(zona->id, zona->nombre, "SO2 (prediccion)", zona->prediccion_24h[3], LIMITE_SO2);
            }
            if (zona->prediccion_24h[4] > LIMITE_NO2) {
                mostrar_alerta(zona->id, zona->nombre, "NO2 (prediccion)", zona->prediccion_24h[4], LIMITE_NO2);
            }
            alertas_generadas++;
        }
    }
    
    if (alertas_generadas == 0) {
        printf("No hay alertas en este momento. Todos los niveles estan dentro de los limites aceptables.\n");
    }
}

// Función para mostrar alerta individual
void mostrar_alerta(int zona_id, char *nombre_zona, char *contaminante, float valor, float limite) {
    printf("   ALERTA: %s: %.2f ug/m3 (Limite: %.2f ug/m3)\n", contaminante, valor, limite);
    printf("   Exceso: %.2f ug/m3 (%.1f%% sobre el limite)\n", 
           valor - limite, ((valor - limite) / limite) * 100);
}

// Función para generar recomendaciones específicas
void generar_recomendaciones(SistemaContaminacion *sistema, int zona_id, Recomendacion *recomendaciones, int *num_recomendaciones) {
    *num_recomendaciones = 0;
    
    ZonaMonitoreo *zona = NULL;
    for (int i = 0; i < sistema->num_zonas; i++) {
        if (sistema->zonas[i].id == zona_id) {
            zona = &sistema->zonas[i];
            break;
        }
    }
    
    if (zona == NULL) return;
    
    // Verificar PM2.5
    if (zona->datos_actuales.pm25 > LIMITE_PM25 || zona->prediccion_24h[0] > LIMITE_PM25) {
        strcpy(recomendaciones[*num_recomendaciones].descripcion, 
               "Reducir trafico vehicular en zona urbana. Implementar dias sin carro.");
        recomendaciones[*num_recomendaciones].prioridad = 1;
        strcpy(recomendaciones[*num_recomendaciones].categoria, "Trafico");
        (*num_recomendaciones)++;
        
        strcpy(recomendaciones[*num_recomendaciones].descripcion, 
               "Evitar actividades fisicas intensas al aire libre, especialmente grupos vulnerables.");
        recomendaciones[*num_recomendaciones].prioridad = 1;
        strcpy(recomendaciones[*num_recomendaciones].categoria, "Salud Publica");
        (*num_recomendaciones)++;
    }
    
    // Verificar PM10
    if (zona->datos_actuales.pm10 > LIMITE_PM10 || zona->prediccion_24h[1] > LIMITE_PM10) {
        strcpy(recomendaciones[*num_recomendaciones].descripcion, 
               "Implementar riegos de vias publicas para reducir polvo suspendido.");
        recomendaciones[*num_recomendaciones].prioridad = 2;
        strcpy(recomendaciones[*num_recomendaciones].categoria, "Salud Publica");
        (*num_recomendaciones)++;
    }
    
    // Verificar CO2
    if (zona->datos_actuales.co2 > LIMITE_CO2 || zona->prediccion_24h[2] > LIMITE_CO2) {
        strcpy(recomendaciones[*num_recomendaciones].descripcion, 
               "Promover uso de transporte publico y bicicletas. Implementar zonas peatonales.");
        recomendaciones[*num_recomendaciones].prioridad = 2;
        strcpy(recomendaciones[*num_recomendaciones].categoria, "Trafico");
        (*num_recomendaciones)++;
    }
    
    // Verificar SO2
    if (zona->datos_actuales.so2 > LIMITE_SO2 || zona->prediccion_24h[3] > LIMITE_SO2) {
        strcpy(recomendaciones[*num_recomendaciones].descripcion, 
               "Inspeccionar industrias cercanas. Implementar filtros de emisiones.");
        recomendaciones[*num_recomendaciones].prioridad = 1;
        strcpy(recomendaciones[*num_recomendaciones].categoria, "Industrial");
        (*num_recomendaciones)++;
    }
    
    // Verificar NO2
    if (zona->datos_actuales.no2 > LIMITE_NO2 || zona->prediccion_24h[4] > LIMITE_NO2) {
        strcpy(recomendaciones[*num_recomendaciones].descripcion, 
               "Optimizar semaforos para reducir tiempo de espera de vehiculos.");
        recomendaciones[*num_recomendaciones].prioridad = 2;
        strcpy(recomendaciones[*num_recomendaciones].categoria, "Trafico");
        (*num_recomendaciones)++;
    }
    
    // Recomendaciones generales basadas en condiciones climáticas
    if (zona->datos_actuales.velocidad_viento < 5.0) {
        strcpy(recomendaciones[*num_recomendaciones].descripcion, 
               "Condiciones de viento bajo favorecen acumulacion de contaminantes. Monitoreo frecuente.");
        recomendaciones[*num_recomendaciones].prioridad = 3;
        strcpy(recomendaciones[*num_recomendaciones].categoria, "Salud Publica");
        (*num_recomendaciones)++;
    }
}

// Función para generar reporte completo
void generar_reporte_completo(SistemaContaminacion *sistema) {
    FILE *archivo = fopen(ARCHIVO_REPORTE, "w");
    if (archivo == NULL) {
        printf("Error al crear archivo de reporte.\n");
        return;
    }
    
    fprintf(archivo, "=== REPORTE INTEGRAL DE CONTAMINACIÓN DEL AIRE ===\n\n");
    fprintf(archivo, "Fecha del reporte: %s", ctime(&sistema->ultima_actualizacion));
    fprintf(archivo, "Número de zonas monitoreadas: %d\n\n", sistema->num_zonas);
    
    for (int i = 0; i < sistema->num_zonas; i++) {
        ZonaMonitoreo *zona = &sistema->zonas[i];
        
        fprintf(archivo, "--- ZONA %d: %s ---\n", zona->id, zona->nombre);
        fprintf(archivo, "Ubicacion: (%.4f, %.4f)\n", zona->latitud, zona->longitud);
        fprintf(archivo, "Registros historicos: %d\n\n", zona->num_registros_historicos);
        
        fprintf(archivo, "DATOS ACTUALES:\n");
        fprintf(archivo, "PM2.5: %.2f ug/m3 (Limite: %.2f)\n", zona->datos_actuales.pm25, LIMITE_PM25);
        fprintf(archivo, "PM10:  %.2f ug/m3 (Limite: %.2f)\n", zona->datos_actuales.pm10, LIMITE_PM10);
        fprintf(archivo, "CO2:   %.2f ug/m3 (Limite: %.2f)\n", zona->datos_actuales.co2, LIMITE_CO2);
        fprintf(archivo, "SO2:   %.2f ug/m3 (Limite: %.2f)\n", zona->datos_actuales.so2, LIMITE_SO2);
        fprintf(archivo, "NO2:   %.2f ug/m3 (Limite: %.2f)\n", zona->datos_actuales.no2, LIMITE_NO2);
        
        fprintf(archivo, "\nCONDICIONES CLIMATICAS:\n");
        fprintf(archivo, "Temperatura: %.1fC\n", zona->datos_actuales.temperatura);
        fprintf(archivo, "Humedad: %.1f%%\n", zona->datos_actuales.humedad);
        fprintf(archivo, "Viento: %.1f km/h\n", zona->datos_actuales.velocidad_viento);
        
        fprintf(archivo, "\nPREDICCIONES 24H:\n");
        fprintf(archivo, "PM2.5: %.2f ug/m3\n", zona->prediccion_24h[0]);
        fprintf(archivo, "PM10:  %.2f ug/m3\n", zona->prediccion_24h[1]);
        fprintf(archivo, "CO2:   %.2f ug/m3\n", zona->prediccion_24h[2]);
        fprintf(archivo, "SO2:   %.2f ug/m3\n", zona->prediccion_24h[3]);
        fprintf(archivo, "NO2:   %.2f ug/m3\n", zona->prediccion_24h[4]);
        
        float indice = calcular_indice_calidad_aire(&zona->datos_actuales);
        fprintf(archivo, "\nINDICE DE CALIDAD DEL AIRE: %.1f (%s)\n", indice, obtener_categoria_calidad(indice));
        
        // Promedios historicos
        if (zona->num_registros_historicos > 0) {
            fprintf(archivo, "\nPROMEDIOS HISTORICOS (30 dias):\n");
            fprintf(archivo, "PM2.5: %.2f ug/m3\n", calcular_promedio_historico(zona, 0));
            fprintf(archivo, "PM10:  %.2f ug/m3\n", calcular_promedio_historico(zona, 1));
            fprintf(archivo, "CO2:   %.2f ug/m3\n", calcular_promedio_historico(zona, 2));
            fprintf(archivo, "SO2:   %.2f ug/m3\n", calcular_promedio_historico(zona, 3));
            fprintf(archivo, "NO2:   %.2f ug/m3\n", calcular_promedio_historico(zona, 4));
        }
        
        fprintf(archivo, "\n==================================================\n\n");
    }
    
    fclose(archivo);
    printf("Reporte completo generado en '%s'\n", ARCHIVO_REPORTE);
}

// Función para exportar datos a archivo binario
void exportar_datos_binario(SistemaContaminacion *sistema) {
    FILE *archivo = fopen("datos_contaminacion_exportados.dat", "wb");
    if (archivo == NULL) {
        printf("Error al crear archivo de exportacion binario.\n");
        return;
    }
    
    // Escribir encabezado como estructura
    typedef struct {
        char encabezado[200];
        time_t fecha_exportacion;
    } EncabezadoExportacion;
    
    EncabezadoExportacion enc;
    strcpy(enc.encabezado, "Datos de Contaminacion - Sistema de Monitoreo Quito");
    enc.fecha_exportacion = time(NULL);
    fwrite(&enc, sizeof(EncabezadoExportacion), 1, archivo);
    
    // Escribir datos del sistema completo
    fwrite(sistema, sizeof(SistemaContaminacion), 1, archivo);
    
    fclose(archivo);
    printf("Copia de respaldo creada exitosamente en:\n");
    printf("-> datos_contaminacion_exportados.dat\n");
    printf("Este archivo contiene toda la informacion del sistema\n");
    printf("en formato binario optimizado para portabilidad.\n");
}

// Función para mostrar estado actual
void mostrar_estado_actual(SistemaContaminacion *sistema) {
    printf("\n=== ESTADO ACTUAL DE CONTAMINACION ===\n");
    printf("Ultima actualizacion: %s", ctime(&sistema->ultima_actualizacion));
    
    for (int i = 0; i < sistema->num_zonas; i++) {
        ZonaMonitoreo *zona = &sistema->zonas[i];
        printf("\n--- ZONA %d: %s ---\n", zona->id, zona->nombre);
        
        if (zona->datos_actuales.dia == 0) {
            printf("Sin datos actuales disponibles\n");
            continue;
        }
        
        printf("Fecha: %d/%d/%d\n", zona->datos_actuales.dia, zona->datos_actuales.mes, zona->datos_actuales.ano);
        
        printf("\nCONTAMINANTES:\n");
        printf("  PM2.5: %.2f ug/m3 %s\n", zona->datos_actuales.pm25, 
               zona->datos_actuales.pm25 > LIMITE_PM25 ? "EXCEDE LIMITE" : "OK");
        printf("  PM10:  %.2f ug/m3 %s\n", zona->datos_actuales.pm10, 
               zona->datos_actuales.pm10 > LIMITE_PM10 ? "EXCEDE LIMITE" : "OK");
        printf("  CO2:   %.2f ug/m3 %s\n", zona->datos_actuales.co2, 
               zona->datos_actuales.co2 > LIMITE_CO2 ? "EXCEDE LIMITE" : "OK");
        printf("  SO2:   %.2f ug/m3 %s\n", zona->datos_actuales.so2, 
               zona->datos_actuales.so2 > LIMITE_SO2 ? "EXCEDE LIMITE" : "OK");
        printf("  NO2:   %.2f ug/m3 %s\n", zona->datos_actuales.no2, 
               zona->datos_actuales.no2 > LIMITE_NO2 ? "EXCEDE LIMITE" : "OK");
        
        printf("\nCONDICIONES CLIMATICAS:\n");
        printf("  Temperatura: %.1fC\n", zona->datos_actuales.temperatura);
        printf("  Humedad: %.1f%%\n", zona->datos_actuales.humedad);
        printf("  Viento: %.1f km/h\n", zona->datos_actuales.velocidad_viento);
        
        float indice = calcular_indice_calidad_aire(&zona->datos_actuales);
        printf("\nINDICE DE CALIDAD DEL AIRE: %.1f (%s)\n", indice, obtener_categoria_calidad(indice));
    }
}

// Función para mostrar predicciones
void mostrar_predicciones(SistemaContaminacion *sistema) {
    printf("\n=== PREDICCIONES 24 HORAS ===\n");
    
    for (int i = 0; i < sistema->num_zonas; i++) {
        ZonaMonitoreo *zona = &sistema->zonas[i];
        printf("\n--- ZONA %d: %s ---\n", zona->id, zona->nombre);
        
        printf("PREDICCIONES:\n");
        printf("  PM2.5: %.2f ug/m3 %s\n", zona->prediccion_24h[0], 
               zona->prediccion_24h[0] > LIMITE_PM25 ? "EXCEDERA LIMITE" : "OK");
        printf("  PM10:  %.2f ug/m3 %s\n", zona->prediccion_24h[1], 
               zona->prediccion_24h[1] > LIMITE_PM10 ? "EXCEDERA LIMITE" : "OK");
        printf("  CO2:   %.2f ug/m3 %s\n", zona->prediccion_24h[2], 
               zona->prediccion_24h[2] > LIMITE_CO2 ? "EXCEDERA LIMITE" : "OK");
        printf("  SO2:   %.2f ug/m3 %s\n", zona->prediccion_24h[3], 
               zona->prediccion_24h[3] > LIMITE_SO2 ? "EXCEDERA LIMITE" : "OK");
        printf("  NO2:   %.2f ug/m3 %s\n", zona->prediccion_24h[4], 
               zona->prediccion_24h[4] > LIMITE_NO2 ? "EXCEDERA LIMITE" : "OK");
        
        if (zona->num_registros_historicos > 0) {
            printf("\nCOMPARACION CON PROMEDIO HISTORICO:\n");
            printf("  PM2.5: Actual %.2f vs Promedio %.2f\n", zona->prediccion_24h[0], calcular_promedio_historico(zona, 0));
            printf("  PM10:  Actual %.2f vs Promedio %.2f\n", zona->prediccion_24h[1], calcular_promedio_historico(zona, 1));
            printf("  CO2:   Actual %.2f vs Promedio %.2f\n", zona->prediccion_24h[2], calcular_promedio_historico(zona, 2));
            printf("  SO2:   Actual %.2f vs Promedio %.2f\n", zona->prediccion_24h[3], calcular_promedio_historico(zona, 3));
            printf("  NO2:   Actual %.2f vs Promedio %.2f\n", zona->prediccion_24h[4], calcular_promedio_historico(zona, 4));
        }
    }
}

// Funciones de menú y interfaz
void mostrar_menu_principal() {
    printf("\n============================================================\n");
    printf("    SISTEMA INTEGRAL DE GESTION DE CONTAMINACION DEL AIRE\n");
    printf("                      CIUDAD DE QUITO\n");
    printf("============================================================\n");
    printf("1. Mostrar estado actual de todas las zonas\n");
    printf("2. Mostrar predicciones para 24 horas\n");
    printf("3. Ingresar datos actuales de una zona\n");
    printf("4. Mostrar informacion de zonas monitoreadas\n");
    printf("5. Generar alertas del sistema\n");
    printf("6. Generar recomendaciones para una zona\n");
    printf("7. Generar reporte completo\n");
    printf("8. Exportar copia de respaldo (binario)\n");
    printf("0. Salir del sistema\n");
    printf("1000. Reiniciar programa (eliminar todos los datos)\n");
    printf("============================================================\n");
    printf("NOTA: Todos los datos se gestionan automaticamente en\n");
    printf("      formato binario para optimo rendimiento.\n");
    printf("============================================================\n");
    printf("Seleccione una opcion: ");
}

void mostrar_menu_zonas(SistemaContaminacion *sistema) {
    printf("\n=== ZONAS MONITOREADAS ===\n");
    
    if (sistema->num_zonas == 0) {
        printf("No hay zonas registradas en el sistema.\n");
        return;
    }
    
    for (int i = 0; i < sistema->num_zonas; i++) {
        ZonaMonitoreo *zona = &sistema->zonas[i];
        printf("\nZONA %d: %s\n", zona->id, zona->nombre);
        printf("   Ubicacion: (%.4f, %.4f)\n", zona->latitud, zona->longitud);
        printf("   Registros historicos: %d\n", zona->num_registros_historicos);
        
        if (zona->datos_actuales.dia != 0) {
            float indice = calcular_indice_calidad_aire(&zona->datos_actuales);
            printf("   Indice de calidad: %.1f (%s)\n", indice, obtener_categoria_calidad(indice));
        } else {
            printf("   Sin datos actuales\n");
        }
    }
}

void mostrar_datos_zona(ZonaMonitoreo *zona) {
    printf("\n=== DETALLES DE ZONA: %s ===\n", zona->nombre);
    printf("ID: %d\n", zona->id);
    printf("Coordenadas: (%.4f, %.4f)\n", zona->latitud, zona->longitud);
    printf("Registros historicos: %d\n", zona->num_registros_historicos);
    
    if (zona->datos_actuales.dia != 0) {
        printf("\nDATOS ACTUALES (%d/%d/%d):\n", 
               zona->datos_actuales.dia, zona->datos_actuales.mes, zona->datos_actuales.ano);
        printf("PM2.5: %.2f ug/m3\n", zona->datos_actuales.pm25);
        printf("PM10:  %.2f ug/m3\n", zona->datos_actuales.pm10);
        printf("CO2:   %.2f ug/m3\n", zona->datos_actuales.co2);
        printf("SO2:   %.2f ug/m3\n", zona->datos_actuales.so2);
        printf("NO2:   %.2f ug/m3\n", zona->datos_actuales.no2);
        printf("Temperatura: %.1fC\n", zona->datos_actuales.temperatura);
        printf("Humedad: %.1f%%\n", zona->datos_actuales.humedad);
        printf("Viento: %.1f km/h\n", zona->datos_actuales.velocidad_viento);
    }
    
    printf("\nPREDICCIONES 24H:\n");
    printf("PM2.5: %.2f ug/m3\n", zona->prediccion_24h[0]);
    printf("PM10:  %.2f ug/m3\n", zona->prediccion_24h[1]);
    printf("CO2:   %.2f ug/m3\n", zona->prediccion_24h[2]);
    printf("SO2:   %.2f ug/m3\n", zona->prediccion_24h[3]);
    printf("NO2:   %.2f ug/m3\n", zona->prediccion_24h[4]);
}

// Funciones auxiliares
float calcular_indice_calidad_aire(DatosContaminacion *datos) {
    // Calcular índice basado en el contaminante más crítico
    float indices[5];
    indices[0] = (datos->pm25 / LIMITE_PM25) * 100;
    indices[1] = (datos->pm10 / LIMITE_PM10) * 100;
    indices[2] = (datos->co2 / LIMITE_CO2) * 100;
    indices[3] = (datos->so2 / LIMITE_SO2) * 100;
    indices[4] = (datos->no2 / LIMITE_NO2) * 100;
    
    // Encontrar el índice máximo
    float max_indice = indices[0];
    for (int i = 1; i < 5; i++) {
        if (indices[i] > max_indice) {
            max_indice = indices[i];
        }
    }
    
    return max_indice;
}

char* obtener_categoria_calidad(float indice) {
    if (indice <= 50) return "Buena";
    else if (indice <= 100) return "Moderada";
    else if (indice <= 150) return "Insalubre para grupos sensibles";
    else if (indice <= 200) return "Insalubre";
    else if (indice <= 300) return "Muy insalubre";
    else return "Peligrosa";
}

void obtener_fecha_actual(int *dia, int *mes, int *ano) {
    time_t t = time(NULL);
    struct tm *fecha = localtime(&t);
    
    *dia = fecha->tm_mday;
    *mes = fecha->tm_mon + 1;
    *ano = fecha->tm_year + 1900;
}

float aplicar_factor_climatico(float valor_base, float temperatura, float humedad, float viento) {
    float factor = 1.0;
    
    // Factor de temperatura (temperaturas altas aumentan la contaminacion)
    if (temperatura > 25.0) {
        factor += (temperatura - 25.0) * 0.02; // 2% por grado sobre 25C
    }
    
    // Factor de humedad (alta humedad puede aumentar PM)
    if (humedad > 70.0) {
        factor += (humedad - 70.0) * 0.005; // 0.5% por punto sobre 70%
    }
    
    // Factor de viento (viento bajo aumenta acumulacion)
    if (viento < 10.0) {
        factor *= (1.0 + (10.0 - viento) * 0.05); // 5% por km/h bajo 10
    } else {
        factor *= (1.0 - (viento - 10.0) * 0.02); // Reduccion con viento alto
    }
    
    // Limitar el factor entre 0.5 y 2.0
    if (factor < 0.5) factor = 0.5;
    if (factor > 2.0) factor = 2.0;
    
    return valor_base * factor;
}

// Función para reiniciar el programa
void reiniciar_programa() {
    printf("\n=== REINICIANDO SISTEMA ===\n");
    printf("Eliminando datos actuales...\n");
    
    // Eliminar archivo de datos si existe
    remove(ARCHIVO_DATOS);
    
    printf("Datos eliminados. El programa se reiniciara con configuracion inicial.\n");
    printf("Presione Enter para continuar...");
    getchar();
    getchar();
}