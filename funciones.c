#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "funciones.h"

#define ARCHIVO_DATOS "datos_zonas.txt"
#define ARCHIVO_RESPALDO "respaldo_zonas.txt"

// Carga los datos desde un archivo de texto
int cargar_zonas(Zona zonas[], int *num_zonas) {
    FILE *f = fopen(ARCHIVO_DATOS, "r");
    if (!f) return 0;
    // Leer número de zonas
    if (fscanf(f, "%d\n", num_zonas) != 1) {
        fclose(f);
        return 0;
    }
    for (int i = 0; i < *num_zonas; i++) {
        // Leer nombre de zona
        if (!fgets(zonas[i].nombre, NOMBRE_ZONA, f)) { fclose(f); return 0; }
        zonas[i].nombre[strcspn(zonas[i].nombre, "\n")] = '\0';
        // Leer días registrados
        int dias;
        if (fscanf(f, "%d\n", &dias) != 1) { fclose(f); return 0; }
        zonas[i].dias_registrados = dias;
        // Leer historial de días
        for (int j = 0; j < dias; j++) {
            RegistroDia *r = &zonas[i].historial[j];
            if (fscanf(f, "%10s %f %f %f %f %f %f %f %f\n",
                       r->fecha, &r->pm25, &r->pm10, &r->co2, &r->so2, &r->no2,
                       &r->temperatura, &r->humedad, &r->velocidad_viento) != 9) {
                fclose(f);
                return 0;
            }
        }
        qsort(zonas[i].historial, zonas[i].dias_registrados, sizeof(RegistroDia), comparar_fechas);
    }
    fclose(f);
    return 1;
}

// Guarda los datos en un archivo de texto
int guardar_zonas(Zona zonas[], int num_zonas) {
    FILE *f = fopen(ARCHIVO_DATOS, "w");
    if (!f) return 0;
    fprintf(f, "%d\n", num_zonas);
    for (int i = 0; i < num_zonas; i++) {
        fprintf(f, "%s\n", zonas[i].nombre);
        fprintf(f, "%d\n", zonas[i].dias_registrados);
        for (int j = 0; j < zonas[i].dias_registrados; j++) {
            RegistroDia *r = &zonas[i].historial[j];
            fprintf(f, "%s %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f\n",
                    r->fecha, r->pm25, r->pm10, r->co2, r->so2, r->no2,
                    r->temperatura, r->humedad, r->velocidad_viento);
        }
    }
    fclose(f);
    return 1;
}

int comparar_fechas(const void *a, const void *b) {
    RegistroDia *regA = (RegistroDia *)a;
    RegistroDia *regB = (RegistroDia *)b;
    return strcmp(regA->fecha, regB->fecha);
}

void mostrar_menu() {
    printf("\n============================================================\n");
    printf("    SISTEMA INTEGRAL DE GESTION DE CONTAMINACION DEL AIRE\n");
    printf("                      CIUDAD DE QUITO\n");
    printf("============================================================\n");
    printf("1. Mostrar estado actual de todas las zonas\n");
    printf("2. Mostrar predicciones para 24 horas\n");
    printf("3. Ingresar datos actuales de una zona\n");
    printf("4. Mostrar informacion de zonas monitoreadas\n");
    printf("5. Generar alertas y recomendaciones\n");
    printf("6. Generar reporte completo\n");
    printf("7. Exportar copia de respaldo\n");
    printf("8. Anadir nueva zona de monitoreo\n");
    printf("9. Editar datos de una zona existente\n");
    printf("10. Eliminar una zona del sistema\n");
    printf("0. Salir del sistema\n");
    printf("1000. Reiniciar programa (eliminar todos los datos)\n");
    printf("============================================================\n");
    printf("NOTA: Todos los datos se gestionan automaticamente en\n");
    printf("      formato de texto para mayor portabilidad.\n");
    printf("      El historial ahora almacena los ultimos 7 dias.\n");
    printf("============================================================\n");
    printf("Seleccione una opcion: ");
}

void limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int validar_float(float valor, float min, float max) {
    return valor >= min && valor <= max;
}

int leer_float(const char *mensaje, float min, float max, float *valor) {
    char buffer[50];
    char *endptr;
    float temp;
    int valido = 0;
    do {
        printf("%s", mensaje);
        if (!fgets(buffer, sizeof(buffer), stdin)) return 0;
        temp = strtof(buffer, &endptr);
        if (endptr == buffer || (*endptr != '\n' && *endptr != '\0')) {
            printf("Entrada invalida. Intente de nuevo.\n");
            continue;
        }
        if (temp < min || temp > max) {
            printf("Valor fuera de rango (%.2f - %.2f).\n", min, max);
            continue;
        }
        *valor = temp;
        valido = 1;
    } while (!valido);
    return 1;
}

int leer_int(const char *mensaje, int min, int max, int *valor) {
    char buffer[50];
    char *endptr;
    long temp;
    int valido = 0;
    do {
        printf("%s", mensaje);
        if (!fgets(buffer, sizeof(buffer), stdin)) return 0;
        temp = strtol(buffer, &endptr, 10);
        if (endptr == buffer || (*endptr != '\n' && *endptr != '\0')) {
            printf("Entrada invalida. Intente de nuevo.\n");
            continue;
        }
        if (temp < min || temp > max) {
            printf("Valor fuera de rango (%d - %d).\n", min, max);
            continue;
        }
        *valor = (int)temp;
        valido = 1;
    } while (!valido);
    return 1;
}

void ingresar_datos_actuales(Zona zonas[], int num_zonas) {
    int op;
    printf("\nSeleccione la zona para ingresar datos:\n");
    for (int i = 0; i < num_zonas; i++)
        printf("%d. %s\n", i + 1, zonas[i].nombre);
    if (!leer_int("Opcion: ", 1, num_zonas, &op)) return;
    
    Zona *z = &zonas[op - 1];
    if (z->dias_registrados == DIAS_HISTORIAL) {
        for (int i = 1; i < DIAS_HISTORIAL; i++)
            z->historial[i - 1] = z->historial[i];
        z->dias_registrados--;
    }
    RegistroDia *r = &z->historial[z->dias_registrados];
    if (!leer_fecha("Ingrese la fecha del nuevo registro:", r->fecha)) {
        printf("Operacion cancelada.\n");
        return;
    }
    leer_float("PM2.5: ", 0, 99999, &r->pm25);
    leer_float("PM10: ", 0, 99999, &r->pm10);
    leer_float("CO2: ", 0, 99999, &r->co2);
    leer_float("SO2: ", 0, 99999, &r->so2);
    leer_float("NO2: ", 0, 99999, &r->no2);
    leer_float("Temperatura (C): ", -50, 60, &r->temperatura);
    leer_float("Humedad (%%): ", 0, 100, &r->humedad);
    leer_float("Velocidad viento (km/h): ", 0, 500, &r->velocidad_viento);
    z->dias_registrados++;
    qsort(z->historial, z->dias_registrados, sizeof(RegistroDia), comparar_fechas);
    guardar_zonas(zonas, num_zonas);
    printf("Datos ingresados y ordenados correctamente.\n");
}

void anadir_zona(Zona zonas[], int *num_zonas) {
    if (*num_zonas >= MAX_ZONAS) {
        printf("No se pueden agregar mas zonas.\n");
        return;
    }
    
    Zona *nueva_zona = &zonas[*num_zonas];

    printf("Nombre de la nueva zona: ");
    fgets(nueva_zona->nombre, NOMBRE_ZONA, stdin);
    nueva_zona->nombre[strcspn(nueva_zona->nombre, "\n")] = 0;

    int dias_a_generar;
    if (!leer_int("\nCuantos dias de datos de ejemplo desea registrar (1-7)?\n(Se recomiendan al menos 3 para que las predicciones funcionen): ", 1, 7, &dias_a_generar)) {
        printf("Operacion cancelada.\n");
        return;
    }

    int modo_ingreso;
    printf("\nSeleccione el modo de ingreso de datos:\n");
    printf("1. Generar datos automaticamente\n");
    printf("2. Ingresar datos manualmente\n");
    if (!leer_int("Opcion: ", 1, 2, &modo_ingreso)) {
        printf("Operacion cancelada.\n");
        return;
    }

    // Se añaden los días de datos de ejemplo solicitados
    nueva_zona->dias_registrados = dias_a_generar;
    for (int i = 0; i < dias_a_generar; i++) {
        RegistroDia *r = &nueva_zona->historial[i];
        printf("\n--- Ingresando datos para el dia %d de %d ---\n", i + 1, dias_a_generar);

        if (modo_ingreso == 1) { // Generación automática
            // Usamos la fecha actual del sistema para generar fechas más realistas hacia atrás
            time_t t = time(NULL);
            struct tm *tm_info = localtime(&t);
            tm_info->tm_mday -= (dias_a_generar - 1 - i); // Restamos días para ir hacia el pasado
            mktime(tm_info); // Normalizamos la fecha
            strftime(r->fecha, sizeof(r->fecha), "%Y-%m-%d", tm_info);

            r->pm25 = 15.0f + (rand() % 200) / 10.0f;
            r->pm10 = 25.0f + (rand() % 300) / 10.0f;
            r->co2 = 400.0f + (rand() % 2000) / 10.0f;
            r->so2 = 5.0f + (rand() % 150) / 10.0f;
            r->no2 = 10.0f + (rand() % 300) / 10.0f;
            r->temperatura = 10.0f + (rand() % 150) / 10.0f;
            r->humedad = 50.0f + (rand() % 300) / 10.0f;
            r->velocidad_viento = 5.0f + (rand() % 150) / 10.0f;
            printf("Datos para fecha %s generados automaticamente.\n", r->fecha);
        } else { // Ingreso manual
            if (!leer_fecha("Ingrese la fecha (YYYY-MM-DD):", r->fecha)) {
                printf("Operacion cancelada.\n");
                // Si se cancela, es mejor detener la creación de la zona
                *num_zonas = (*num_zonas > 0) ? *num_zonas -1 : 0; // Temporalmente se deshace
                return;
            }
            leer_float("PM2.5: ", 0, 99999, &r->pm25);
            leer_float("PM10: ", 0, 99999, &r->pm10);
            leer_float("CO2: ", 0, 99999, &r->co2);
            leer_float("SO2: ", 0, 99999, &r->so2);
            leer_float("NO2: ", 0, 99999, &r->no2);
            leer_float("Temperatura (C): ", -50, 60, &r->temperatura);
            leer_float("Humedad (%%): ", 0, 100, &r->humedad);
            leer_float("Velocidad viento (km/h): ", 0, 500, &r->velocidad_viento);
        }
    }
qsort(nueva_zona->historial, nueva_zona->dias_registrados, sizeof(RegistroDia), comparar_fechas);
    (*num_zonas)++;
    guardar_zonas(zonas, *num_zonas);
    printf("\nZona agregada correctamente con %d dias de datos.\n", dias_a_generar);
    if (dias_a_generar >= 3) {
        printf("Ya puede utilizar la funcion de prediccion para esta zona.\n");
    }
}

void editar_zona(Zona zonas[], int num_zonas) {
    int op_zona;
    printf("\nSeleccione la zona a editar:\n");
    for (int i = 0; i < num_zonas; i++)
        printf("%d. %s\n", i + 1, zonas[i].nombre);
    if (!leer_int("Opcion: ", 1, num_zonas, &op_zona)) return;

    Zona *z = &zonas[op_zona - 1];
    int op_edit;

    do {
        printf("\n--- Editando Zona: %s ---\n", z->nombre);
        printf("1. Editar Nombre\n");
        printf("2. Editar todos los datos de un dia\n");
        printf("3. Editar solo la fecha de un dia\n");
        printf("0. Volver al menu principal\n");
        if (!leer_int("Opcion: ", 0, 3, &op_edit)) continue;

        switch (op_edit) {
            case 1: {
                printf("Nuevo nombre: ");
                char buffer[NOMBRE_ZONA];
                fgets(buffer, NOMBRE_ZONA, stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                if (strlen(buffer) > 0) {
                    strncpy(z->nombre, buffer, NOMBRE_ZONA);
                    printf("Nombre actualizado.\n");
                }
                break;
            }
            case 2: {
                if (z->dias_registrados == 0) {
                    printf("No hay datos historicos para editar.\n");
                    break;
                }
                printf("Seleccione el registro a editar:\n");
                for (int i = 0; i < z->dias_registrados; i++) {
                    printf("%d. %s\n", i + 1, z->historial[i].fecha);
                }
                int op_fecha;
                if (!leer_int("Opcion: ", 1, z->dias_registrados, &op_fecha)) break;
                
                RegistroDia *r = &z->historial[op_fecha - 1];
                printf("Editando datos para la fecha %s...\n", r->fecha);
                leer_float("Nuevo PM2.5: ", 0, 99999, &r->pm25);
                leer_float("Nuevo PM10: ", 0, 99999, &r->pm10);
                leer_float("Nuevo CO2: ", 0, 99999, &r->co2);
                leer_float("Nuevo SO2: ", 0, 99999, &r->so2);
                leer_float("Nuevo NO2: ", 0, 99999, &r->no2);
                leer_float("Nueva Temperatura (C): ", -50, 60, &r->temperatura);
                leer_float("Nueva Humedad (%%): ", 0, 100, &r->humedad);
                leer_float("Nueva Velocidad viento (km/h): ", 0, 500, &r->velocidad_viento);
                printf("Datos del %s actualizados.\n", r->fecha);
                break;
            }
            case 3: {
                if (z->dias_registrados == 0) {
                    printf("No hay datos historicos para editar.\n");
                    break;
                }
                printf("Seleccione el registro para cambiar la fecha:\n");
                for (int i = 0; i < z->dias_registrados; i++) {
                    printf("%d. %s\n", i + 1, z->historial[i].fecha);
                }
                int op_fecha;
                if (!leer_int("Opcion: ", 1, z->dias_registrados, &op_fecha)) break;

                RegistroDia *r = &z->historial[op_fecha - 1];
                char fecha_anterior[11];
                strcpy(fecha_anterior, r->fecha);

                char nueva_fecha[11];
                if (!leer_fecha("Ingrese la nueva fecha:", nueva_fecha)) {
                    printf("Operacion cancelada.\n");
                    break;
                }

                strcpy(r->fecha, nueva_fecha);
                printf("Fecha del registro actualizada de %s a %s.\n", fecha_anterior, r->fecha);
                
                // Ordenar el historial por fecha para mantener la consistencia cronológica
                qsort(z->historial, z->dias_registrados, sizeof(RegistroDia), comparar_fechas);
                printf("El historial de la zona ha sido reordenado cronologicamente.\n");
                break;
            }
        }
    } while (op_edit != 0);
    guardar_zonas(zonas, num_zonas);
    printf("Cambios guardados.\n");
}

void mostrar_estado_actual(Zona zonas[], int num_zonas) {
    printf("\nESTADO ACTUAL DE LAS ZONAS (ULTIMOS 7 DIAS):\n");
    for (int i = 0; i < num_zonas; i++) {
        printf("\nZona: %s\n", zonas[i].nombre);
        printf("------------------------------------------------------------\n");
        printf("Fecha      | PM2.5 | PM10 | CO2  | SO2  | NO2  | Temp | Hum | V.Viento\n");
        printf("-----------|-------|------|------|------|------|------|-----|----------\n");
        if (zonas[i].dias_registrados > 0) {
            for (int j = 0; j < zonas[i].dias_registrados; j++) {
                RegistroDia *r = &zonas[i].historial[j];
                printf("%-10s | %5.1f | %4.1f | %4.1f | %4.1f | %4.1f | %4.1f | %3.1f | %7.1f\n",
                    r->fecha, r->pm25, r->pm10, r->co2, r->so2, r->no2, r->temperatura, r->humedad, r->velocidad_viento);
            }
        } else {
            printf("No hay datos registrados.\n");
        }
    }
}

void mostrar_predicciones(Zona zonas[], int num_zonas) {
    printf("\nPREDICCIONES PARA LAS PROXIMAS 24 HORAS:\n");
    for (int i = 0; i < num_zonas; i++) {
        printf("\nZona: %s\n", zonas[i].nombre);
        printf("------------------------------------------------------------\n");
        printf("PM2.5 | PM10 | CO2  | SO2  | NO2  | Temp | Hum | V.Viento\n");
        float sumas[8] = {0};
        float pesos[3] = {0.6, 0.3, 0.1};
        int dias = zonas[i].dias_registrados;
        if (dias < 3) {
            printf("No hay suficientes datos para predecir.\n");
            continue;
        }
        for (int j = 0; j < 3; j++) {
            RegistroDia *r = &zonas[i].historial[dias-1-j];
            sumas[0] += r->pm25 * pesos[j];
            sumas[1] += r->pm10 * pesos[j];
            sumas[2] += r->co2 * pesos[j];
            sumas[3] += r->so2 * pesos[j];
            sumas[4] += r->no2 * pesos[j];
            sumas[5] += r->temperatura * pesos[j];
            sumas[6] += r->humedad * pesos[j];
            sumas[7] += r->velocidad_viento * pesos[j];
        }
        printf("%5.1f | %4.1f | %4.1f | %4.1f | %4.1f | %4.1f | %3.1f | %7.1f\n",
            sumas[0], sumas[1], sumas[2], sumas[3], sumas[4], sumas[5], sumas[6], sumas[7]);
    }
}

void mostrar_info_zonas(Zona zonas[], int num_zonas) {
    if (num_zonas == 0) {
        printf("\nNo hay zonas registradas para mostrar.\n");
        return;
    }

    int op;
    printf("\nSeleccione la zona para ver su informacion:\n");
    for (int i = 0; i < num_zonas; i++) {
        printf("%d. %s\n", i + 1, zonas[i].nombre);
    }
    if (!leer_int("Opcion: ", 1, num_zonas, &op)) return;

    Zona *z = &zonas[op - 1];

    printf("\nINFORMACION DE ZONA MONITOREADA: %s\n", z->nombre);
    printf("------------------------------------------------------------\n");
    printf("Fecha      | PM2.5 | PM10 | CO2  | SO2  | NO2  | Temp | Hum | V.Viento\n");
    printf("-----------|-------|------|------|------|------|------|-----|----------\n");
    if (z->dias_registrados > 0) {
        for (int j = 0; j < z->dias_registrados; j++) {
            RegistroDia *r = &z->historial[j];
            printf("%-10s | %5.1f | %4.1f | %4.1f | %4.1f | %4.1f | %4.1f | %3.1f | %7.1f\n",
                r->fecha, r->pm25, r->pm10, r->co2, r->so2, r->no2, r->temperatura, r->humedad, r->velocidad_viento);
        }
    } else {
        printf("No hay datos registrados para esta zona.\n");
    }
}

void generar_alertas_y_recomendaciones(Zona zonas[], int num_zonas) {
    printf("\nALERTAS Y RECOMENDACIONES DEL SISTEMA:\n");
    int alertas_generadas = 0;

    for (int i = 0; i < num_zonas; i++) {
        if (zonas[i].dias_registrados == 0) continue;

        RegistroDia *r = &zonas[i].historial[zonas[i].dias_registrados - 1];
        int alerta_zona = 0;

        // Buffer para acumular los mensajes de alerta de la zona
        char mensaje_alerta[1024] = "";

        if (r->pm25 > 25 || r->pm10 > 50) {
            alerta_zona = 1;
            strcat(mensaje_alerta, "  -> ALERTA: Niveles altos de material particulado (PM2.5/PM10).\n");
            strcat(mensaje_alerta, "     - RECOMENDACIONES:\n");
            strcat(mensaje_alerta, "       - Evitar actividad fisica intensa al aire libre.\n");
            strcat(mensaje_alerta, "       - Grupos vulnerables (niños, ancianos, personas con asma) deben permanecer en interiores.\n");
            strcat(mensaje_alerta, "       - Usar mascarillas N95 si es necesario salir.\n\n");
        }
        if (r->co2 > 1000) {
            alerta_zona = 1;
            strcat(mensaje_alerta, "  -> ALERTA: Niveles altos de Dioxido de Carbono (CO2).\n");
            strcat(mensaje_alerta, "     - RECOMENDACIONES:\n");
            strcat(mensaje_alerta, "       - Asegurar buena ventilacion en espacios cerrados.\n");
            strcat(mensaje_alerta, "       - Reducir el uso de vehiculos a combustion en la zona.\n\n");
        }
        if (r->so2 > 20) {
            alerta_zona = 1;
            strcat(mensaje_alerta, "  -> ALERTA: Niveles altos de Dioxido de Azufre (SO2).\n");
            strcat(mensaje_alerta, "     - RECOMENDACIONES:\n");
            strcat(mensaje_alerta, "       - Personas con asma deben tener especial cuidado y evitar la exposicion.\n");
            strcat(mensaje_alerta, "       - Limitar la exposicion en areas industriales o de alto trafico.\n\n");
        }
        if (r->no2 > 40) {
            alerta_zona = 1;
            strcat(mensaje_alerta, "  -> ALERTA: Niveles altos de Dioxido de Nitrogeno (NO2).\n");
            strcat(mensaje_alerta, "     - RECOMENDACIONES:\n");
            strcat(mensaje_alerta, "       - Reducir el uso de vehiculos, especialmente diesel.\n");
            strcat(mensaje_alerta, "       - Evitar la quema de combustibles fosiles.\n\n");
        }

        if (alerta_zona) {
            alertas_generadas = 1;
            printf("\n------------------------------------------------------------\n");
            printf("ALERTA EN ZONA: %s\n", zonas[i].nombre);
            printf("%s", mensaje_alerta);
        }
    }

    if (!alertas_generadas) {
        printf("\nNo hay alertas activas. La calidad del aire en todas las zonas esta dentro de los limites aceptables.\n");
    }
    printf("\n");
}

const char* obtener_categoria_ica(float pm25) {
    if (pm25 <= 12.0) return "Buena";
    if (pm25 <= 35.4) return "Moderada";
    if (pm25 <= 55.4) return "Dañina a la salud para grupos sensibles";
    if (pm25 <= 150.4) return "Dañina a la salud";
    if (pm25 <= 250.4) return "Muy dañina a la salud";
    return "Peligrosa";
}

void generar_reporte(Zona zonas[], int num_zonas) {
    FILE *f = fopen("reporte_integral.txt", "w");
    if (!f) {
        printf("No se pudo crear el reporte.\n");
        return;
    }

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char buffer_fecha[50];
    strftime(buffer_fecha, sizeof(buffer_fecha), "%a %b %d %H:%M:%S %Y", tm_info);

    fprintf(f, "=== REPORTE INTEGRAL DE CONTAMINACIÓN DEL AIRE ===\n\n");
    fprintf(f, "Fecha del reporte: %s\n", buffer_fecha);
    fprintf(f, "Número de zonas monitoreadas: %d\n\n", num_zonas);

    for (int i = 0; i < num_zonas; i++) {
        Zona *z = &zonas[i];
        fprintf(f, "--- ZONA %d: %s ---\n", i + 1, z->nombre);
        fprintf(f, "Registros historicos: %d\n\n", z->dias_registrados);

        if (z->dias_registrados > 0) {
            RegistroDia *r_actual = &z->historial[z->dias_registrados - 1];
            fprintf(f, "DATOS ACTUALES:\n");
            fprintf(f, "PM2.5: %.2f ug/m3 (Limite: 25.00)\n", r_actual->pm25);
            fprintf(f, "PM10:  %.2f ug/m3 (Limite: 50.00)\n", r_actual->pm10);
            fprintf(f, "CO2:   %.2f ppm (Limite: 1000.00)\n", r_actual->co2);
            fprintf(f, "SO2:   %.2f ug/m3 (Limite: 20.00)\n", r_actual->so2);
            fprintf(f, "NO2:   %.2f ug/m3 (Limite: 40.00)\n\n", r_actual->no2);

            fprintf(f, "CONDICIONES CLIMATICAS:\n");
            fprintf(f, "Temperatura: %.1fC\n", r_actual->temperatura);
            fprintf(f, "Humedad: %.1f%%\n", r_actual->humedad);
            fprintf(f, "Viento: %.1f km/h\n\n", r_actual->velocidad_viento);

            fprintf(f, "PREDICCIONES 24H:\n");
            if (z->dias_registrados >= 3) {
                float sumas[5] = {0};
                float pesos[3] = {0.6, 0.3, 0.1};
                for (int j = 0; j < 3; j++) {
                    RegistroDia *r_hist = &z->historial[z->dias_registrados - 1 - j];
                    sumas[0] += r_hist->pm25 * pesos[j];
                    sumas[1] += r_hist->pm10 * pesos[j];
                    sumas[2] += r_hist->co2 * pesos[j];
                    sumas[3] += r_hist->so2 * pesos[j];
                    sumas[4] += r_hist->no2 * pesos[j];
                }
                fprintf(f, "PM2.5: %.2f ug/m3\n", sumas[0]);
                fprintf(f, "PM10:  %.2f ug/m3\n", sumas[1]);
                fprintf(f, "CO2:   %.2f ppm\n", sumas[2]);
                fprintf(f, "SO2:   %.2f ug/m3\n", sumas[3]);
                fprintf(f, "NO2:   %.2f ug/m3\n\n", sumas[4]);
            } else {
                fprintf(f, "No hay suficientes datos para predecir.\n\n");
            }

            const char* categoria_ica = obtener_categoria_ica(r_actual->pm25);
            fprintf(f, "INDICE DE CALIDAD DEL AIRE: %.2f (%s)\n\n", r_actual->pm25, categoria_ica);

            fprintf(f, "PROMEDIOS HISTORICOS (%d dias):\n", z->dias_registrados);
            float promedios[5] = {0};
            for (int j = 0; j < z->dias_registrados; j++) {
                promedios[0] += z->historial[j].pm25;
                promedios[1] += z->historial[j].pm10;
                promedios[2] += z->historial[j].co2;
                promedios[3] += z->historial[j].so2;
                promedios[4] += z->historial[j].no2;
            }
            fprintf(f, "PM2.5: %.2f ug/m3\n", promedios[0] / z->dias_registrados);
            fprintf(f, "PM10:  %.2f ug/m3\n", promedios[1] / z->dias_registrados);
            fprintf(f, "CO2:   %.2f ppm\n", promedios[2] / z->dias_registrados);
            fprintf(f, "SO2:   %.2f ug/m3\n", promedios[3] / z->dias_registrados);
            fprintf(f, "NO2:   %.2f ug/m3\n", promedios[4] / z->dias_registrados);

        } else {
            fprintf(f, "No hay datos registrados para esta zona.\n");
        }
        fprintf(f, "\n==================================================\n\n");
    }

    fclose(f);
    printf("Reporte integral generado en reporte_integral.txt\n");
}

void exportar_respaldo(Zona zonas[], int num_zonas) {
    FILE *f = fopen(ARCHIVO_RESPALDO, "wb");
    if (!f) {
        printf("No se pudo exportar el respaldo.\n");
        return;
    }
    fwrite(&num_zonas, sizeof(int), 1, f);
    fwrite(zonas, sizeof(Zona), num_zonas, f);
    fclose(f);
    printf("Respaldo exportado en %s\n", ARCHIVO_RESPALDO);
}

void eliminar_zona(Zona zonas[], int *num_zonas) {
    int op;
    printf("\nSeleccione la zona a eliminar:\n");
    for (int i = 0; i < *num_zonas; i++)
        printf("%d. %s\n", i+1, zonas[i].nombre);
    if (!leer_int("Opcion: ", 1, *num_zonas, &op)) return;
    for (int i = op-1; i < *num_zonas-1; i++)
        zonas[i] = zonas[i+1];
    (*num_zonas)--;
    guardar_zonas(zonas, *num_zonas);
    printf("Zona eliminada correctamente.\n");
}

void reiniciar_programa() {
    remove(ARCHIVO_DATOS);
    printf("Todos los datos han sido eliminados.\n");
}
int leer_fecha(const char *mensaje, char *fecha_str) {
    int dia, mes, anio;
    printf("%s\n", mensaje);

    if (!leer_int("Anio (ej. 2024): ", 2000, 2050, &anio)) return 0;
    if (!leer_int("Mes (1-12): ", 1, 12, &mes)) return 0;

    int max_dias = 31;
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
        max_dias = 30;
    } else if (mes == 2) {
        // Comprobar si es año bisiesto
        if ((anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0)) {
            max_dias = 29; // Año bisiesto
        } else {
            max_dias = 28; // Año no bisiesto
        }
    }

    char mensaje_dia[50];
    sprintf(mensaje_dia, "Dia (1-%d): ", max_dias);

    if (!leer_int(mensaje_dia, 1, max_dias, &dia)) return 0;

    // Formatea la fecha en la cadena de salida
    sprintf(fecha_str, "%04d-%02d-%02d", anio, mes, dia);
    return 1;
}
