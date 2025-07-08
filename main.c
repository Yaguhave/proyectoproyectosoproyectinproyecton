#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones.h"

int main() {
    Zona zonas[MAX_ZONAS];
    int num_zonas = 0, opcion;

    // Intenta cargar los datos existentes, si no puede, crea un archivo inicial
    if (!cargar_zonas(zonas, &num_zonas)) {
        printf("No se encontro archivo de datos o el formato es incorrecto. Creando uno nuevo...\n");
        char *nombres[] = {"UDLA Park", "Parque La Carolina", "Mitad del Mundo", "El Panecillo", "Centro Historico"};
        num_zonas = 5;

        for (int i = 0; i < num_zonas; i++) {
            strncpy(zonas[i].nombre, nombres[i], NOMBRE_ZONA - 1);
            zonas[i].nombre[NOMBRE_ZONA - 1] = '\0';
            zonas[i].dias_registrados = DIAS_HISTORIAL;
            for (int j = 0; j < DIAS_HISTORIAL; j++) {
                RegistroDia *r = &zonas[i].historial[j];
                sprintf(r->fecha, "2025-07-%02d", j + 1);
                r->pm25 = 15.0f + (rand() % 200) / 10.0f;
                r->pm10 = 25.0f + (rand() % 300) / 10.0f;
                r->co2 = 400.0f + (rand() % 2000) / 10.0f;
                r->so2 = 5.0f + (rand() % 150) / 10.0f;
                r->no2 = 10.0f + (rand() % 300) / 10.0f;
                r->temperatura = 10.0f + (rand() % 150) / 10.0f;
                r->humedad = 50.0f + (rand() % 300) / 10.0f;
                r->velocidad_viento = 5.0f + (rand() % 150) / 10.0f;
            }
        }
        guardar_zonas(zonas, num_zonas);
        printf("Archivo de datos inicial creado con 5 zonas y 7 dias de historial.\n");
    }

    do {
        mostrar_menu();
        if (scanf("%d", &opcion) != 1) {
            opcion = -1; // Opción inválida para que el bucle continúe
        }
        limpiar_buffer(); // Limpiar el buffer después de cada entrada

        switch (opcion) {
            case 1: mostrar_estado_actual(zonas, num_zonas); break;
            case 2: mostrar_predicciones(zonas, num_zonas); break;
            case 3: ingresar_datos_actuales(zonas, num_zonas); break;
            case 4: mostrar_info_zonas(zonas, num_zonas); break;
            case 5: generar_alertas_y_recomendaciones(zonas, num_zonas); break;
            case 6: generar_reporte(zonas, num_zonas); break;
            case 7: exportar_respaldo(zonas, num_zonas); break;
            case 8: anadir_zona(zonas, &num_zonas); break;
            case 9: editar_zona(zonas, num_zonas); break;
            case 10: eliminar_zona(zonas, &num_zonas); break;
            case 1000:
                reiniciar_programa();
                num_zonas = 0; // Reinicia el contador de zonas
                printf("Programa reiniciado. Por favor, reinicie la aplicacion para generar nuevos datos de ejemplo.\n");
                opcion = 0; // Forzar salida para evitar operar con datos vacíos
                break;
            case 0:
                printf("Saliendo del sistema...\n");
                break;
            default:
                printf("Opcion invalida. Por favor, intente de nuevo.\n");
        }
    } while (opcion != 0);

    return 0;
}
