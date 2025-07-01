/*
 * SISTEMA INTEGRAL DE GESTION DE CONTAMINACION DEL AIRE - QUITO
 * ARCHIVO PRINCIPAL (main.c)
 * 
 * Este es el archivo principal del programa que contiene la función main().
 * Aquí se ejecuta el menú principal y se coordinan todas las operaciones del sistema.
 * 
 * FLUJO DEL PROGRAMA:
 * 1. Inicializar el sistema con las 5 zonas de Quito
 * 2. Cargar datos guardados anteriormente (si existen)
 * 3. Mostrar menú principal y esperar selección del usuario
 * 4. Ejecutar la operación seleccionada
 * 5. Repetir hasta que el usuario elija salir
 * 6. Guardar datos antes de terminar el programa
 */

#include "funciones.h"

int main() {
    // Declaración de variables principales
    SistemaContaminacion sistema;                    // Estructura principal del sistema
    int opcion;                                      // Opción seleccionada por el usuario
    int zona_id;                                     // ID de zona para operaciones específicas
    Recomendacion recomendaciones[10];               // Array para almacenar recomendaciones
    int num_recomendaciones;                         // Contador de recomendaciones generadas
    
    // ========== INICIALIZACION DEL SISTEMA ==========
    printf("Iniciando Sistema Integral de Gestion de Contaminacion del Aire...\n");
    printf("Configurando zonas de monitoreo en Quito...\n");
    inicializar_sistema(&sistema);                   // Configurar las 5 zonas de Quito
    
    // Intentar cargar datos de sesiones anteriores
    printf("Buscando datos guardados anteriormente...\n");
    cargar_datos_archivo(&sistema);
    
    printf("Sistema inicializado con %d zonas monitoreadas.\n", sistema.num_zonas);
    printf("Presione Enter para continuar...");
    getchar(); // Pausa para que el usuario lea la información
    
    // ========== MENU PRINCIPAL DEL SISTEMA ==========
    do {
        mostrar_menu_principal();                    // Mostrar opciones disponibles
        scanf("%d", &opcion);                        // Leer selección del usuario
        
        // Procesar la opción seleccionada
        switch(opcion) {
            case 1:
                // Mostrar estado actual de todas las zonas
                mostrar_estado_actual(&sistema);
                break;
                
            case 2:
                // Mostrar predicciones para las próximas 24 horas
                mostrar_predicciones(&sistema);
                break;
                
            case 3:
                // Ingresar nuevos datos de contaminación para una zona específica
                printf("\nIngrese el ID de la zona (1-%d): ", sistema.num_zonas);
                scanf("%d", &zona_id);
                if (zona_id >= 1 && zona_id <= sistema.num_zonas) {
                    ingresar_datos_actuales(&sistema, zona_id);       // Capturar datos del usuario
                    sistema.ultima_actualizacion = time(NULL);        // Actualizar timestamp
                    guardar_datos_archivo(&sistema);                  // Guardar automáticamente
                } else {
                    printf("ERROR: ID de zona invalido. Debe estar entre 1 y %d.\n", sistema.num_zonas);
                }
                break;
                
            case 4:
                // Mostrar información detallada de las zonas
                mostrar_menu_zonas(&sistema);
                printf("\nDesea ver detalles de alguna zona? (ID/0 para salir): ");
                scanf("%d", &zona_id);
                if (zona_id >= 1 && zona_id <= sistema.num_zonas) {
                    mostrar_datos_zona(&sistema.zonas[zona_id - 1]);  // Mostrar zona específica
                }
                break;
                
            case 5:
                // Generar y mostrar alertas del sistema
                generar_alertas(&sistema);
                break;
                
            case 6:
                // Generar recomendaciones específicas para una zona
                printf("\nIngrese el ID de la zona (1-%d): ", sistema.num_zonas);
                scanf("%d", &zona_id);
                if (zona_id >= 1 && zona_id <= sistema.num_zonas) {
                    // Generar recomendaciones basadas en los datos de la zona
                    generar_recomendaciones(&sistema, zona_id, recomendaciones, &num_recomendaciones);
                    
                    printf("\n=== RECOMENDACIONES PARA ZONA %d ===\n", zona_id);
                    if (num_recomendaciones == 0) {
                        printf("No se requieren recomendaciones especiales en este momento.\n");
                        printf("Los niveles de contaminacion estan dentro de los limites normales.\n");
                    } else {
                        // Mostrar cada recomendación con su nivel de prioridad
                        for (int i = 0; i < num_recomendaciones; i++) {
                            char prioridad_str[20];
                            // Convertir código de prioridad a texto comprensible
                            switch(recomendaciones[i].prioridad) {
                                case 1: strcpy(prioridad_str, "ALTA"); break;      // Urgente
                                case 2: strcpy(prioridad_str, "MEDIA"); break;     // Importante
                                case 3: strcpy(prioridad_str, "BAJA"); break;      // Preventiva
                                default: strcpy(prioridad_str, "DESCONOCIDA");
                            }
                            
                            printf("\n%d. [PRIORIDAD %s] [%s]\n", i+1, prioridad_str, recomendaciones[i].categoria);
                            printf("   %s\n", recomendaciones[i].descripcion);
                        }
                    }
                } else {
                    printf("ERROR: ID de zona invalido. Debe estar entre 1 y %d.\n", sistema.num_zonas);
                }
                break;
                
            case 7:
                // Generar reporte completo del sistema
                printf("Generando reporte completo del sistema...\n");
                generar_reporte_completo(&sistema);
                break;
                
            case 8:
                // Exportar datos a archivo binario de respaldo
                printf("Exportando datos del sistema...\n");
                exportar_datos_binario(&sistema);
                break;
                
            case 1000:
                // Opción especial: reiniciar completamente el sistema
                printf("\n=== ADVERTENCIA ===\n");
                printf("Esta opcion eliminara TODOS los datos del sistema.\n");
                printf("Se perderan todos los registros historicos y configuraciones.\n");
                printf("Esta operacion NO se puede deshacer.\n");
                printf("Esta seguro de que desea continuar? (s/n): ");
                char confirmacion;
                scanf(" %c", &confirmacion);  // El espacio antes de %c limpia el buffer
                
                if (confirmacion == 's' || confirmacion == 'S') {
                    printf("Procediendo con el reinicio del sistema...\n");
                    reiniciar_programa();                           // Eliminar datos guardados
                    inicializar_sistema(&sistema);                  // Reinicializar con configuración base
                    printf("Sistema reiniciado exitosamente.\n");
                    printf("Todas las zonas han sido restauradas a su configuracion inicial.\n");
                } else {
                    printf("Operacion cancelada. Los datos se mantienen intactos.\n");
                }
                break;
                
            case 0:
                // Salir del programa
                printf("\n=== CERRANDO SISTEMA ===\n");
                printf("Guardando datos antes de salir...\n");
                guardar_datos_archivo(&sistema);                    // Guardar datos automáticamente
                printf("Datos guardados exitosamente.\n");
                printf("\nGracias por usar el Sistema de Gestion de Contaminacion del Aire!\n");
                printf("Recordatorio: Cuidemos nuestro planeta para las futuras generaciones.\n");
                printf("Sistema cerrado exitosamente.\n");
                break;
                
            default:
                // Opción inválida
                printf("\n=== ERROR ===\n");
                printf("Opcion invalida. Por favor, seleccione una opcion valida del menu.\n");
                printf("Las opciones disponibles son: 1-8, 1000, o 0 para salir.\n");
                break;
        }
        
        // Pausa para que el usuario pueda leer la información antes de volver al menú
        if (opcion != 0) {
            printf("\n--- Presione Enter para volver al menu principal ---");
            getchar(); // Limpiar el buffer de entrada
            getchar(); // Esperar a que el usuario presione Enter
        }
        
    } while (opcion != 0);  // Continuar hasta que el usuario elija salir (opción 0)
    
    return 0;  // Terminar el programa exitosamente
}

/*
 * ================================================================================================
 * FIN DEL ARCHIVO PRINCIPAL
 * ================================================================================================
 * 
 * Este archivo coordina toda la funcionalidad del sistema de monitoreo de contaminación.
 * 
 * OPCIONES DEL MENU:
 * 1. Ver estado actual de todas las zonas
 * 2. Ver predicciones para 24 horas
 * 3. Ingresar nuevos datos de contaminación
 * 4. Ver información detallada de zonas
 * 5. Generar alertas del sistema
 * 6. Generar recomendaciones para una zona
 * 7. Crear reporte completo
 * 8. Exportar datos a archivo binario
 * 1000. Reiniciar completamente el sistema
 * 0. Salir del programa
 * 
 * FLUJO DE DATOS:
 * - Los datos se guardan automáticamente en formato binario
 * - Se cargan automáticamente al iniciar el programa
 * - Las predicciones se actualizan cada vez que se ingresan nuevos datos
 * - Las alertas se generan en tiempo real basadas en límites de la OMS
 */