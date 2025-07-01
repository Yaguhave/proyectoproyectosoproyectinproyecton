# Sistema Integral de Gestión de Contaminación del Aire - Quito

## 📋 Descripción General

Este sistema está diseñado para monitorear y predecir la contaminación del aire en 5 zonas urbanas específicas de Quito, Ecuador. El programa permite registrar datos de contaminación en tiempo real, generar predicciones para las próximas 24 horas, crear alertas cuando se superan los límites seguros, y proporcionar recomendaciones específicas para mejorar la calidad del aire.

## 🎯 Características Principales

### Monitoreo en Tiempo Real
- **5 Zonas de Quito**: Sector Udlapark, Mitad del Mundo, Parque la Carolina, El Panecillo, Catedral Metropolitana
- **5 Contaminantes**: PM2.5, PM10, CO2, SO2, NO2
- **Datos Climáticos**: Temperatura, humedad, velocidad del viento

### Análisis y Predicción
- **Predicciones de 24 horas** basadas en historial y condiciones climáticas
- **Análisis de tendencias** usando promedios ponderados
- **Índice de calidad del aire** basado en estándares de la OMS

### Sistema de Alertas
- **Alertas automáticas** cuando se superan los límites de seguridad
- **Alertas preventivas** basadas en predicciones
- **Recomendaciones específicas** por zona y tipo de contaminación

### Persistencia de Datos
- **Almacenamiento binario** para mayor eficiencia
- **Carga automática** de datos al iniciar
- **Guardado automático** después de cambios importantes
- **Exportación de respaldo** en formato binario

## 📁 Estructura de Archivos

```
proyecto/
├── funciones.h          # Declaraciones de estructuras y funciones
├── funciones.c          # Implementación de todas las funciones
├── main.c              # Programa principal con menú de usuario
├── README.md           # Este archivo de documentación
├── datos_contaminacion.dat      # Datos guardados (generado automáticamente)
├── reporte_contaminacion.txt    # Reportes generados (creado por opción 7)
└── datos_contaminacion_exportados.dat  # Archivo de exportación (opción 8)
```

## 🚀 Cómo Compilar y Ejecutar

### Requisitos
- Compilador de C (como GCC)
- Sistema operativo Windows, Linux o macOS
- Terminal o línea de comandos

### Compilación
```bash
gcc -o sistema main.c funciones.c
```

### Ejecución
```bash
./sistema
```
(En Windows: `sistema.exe`)

## 🖥️ Uso del Sistema

### Menú Principal
Al ejecutar el programa, verás el siguiente menú:

```
============================================================
    SISTEMA INTEGRAL DE GESTION DE CONTAMINACION DEL AIRE
============================================================
1. Mostrar estado actual de todas las zonas
2. Mostrar predicciones para 24 horas
3. Ingresar datos actuales de una zona
4. Mostrar informacion de zonas monitoreadas
5. Generar alertas del sistema
6. Generar recomendaciones para una zona
7. Generar reporte completo
8. Exportar datos a archivo binario
0. Salir del sistema
1000. Reiniciar programa (eliminar todos los datos)
============================================================
```

### Opciones Detalladas

**Opción 1: Estado Actual**
- Muestra los datos más recientes de todas las zonas
- Indica si los niveles están dentro de límites seguros
- Calcula el índice de calidad del aire

**Opción 2: Predicciones**
- Muestra las predicciones para las próximas 24 horas
- Compara con promedios históricos
- Indica si se esperan excesos de límites

**Opción 3: Ingresar Datos**
- Permite capturar nuevos datos de una zona específica
- Solicita mediciones de los 5 contaminantes
- Requiere datos climáticos (temperatura, humedad, viento)
- Actualiza automáticamente las predicciones

**Opción 4: Información de Zonas**
- Lista todas las zonas con sus coordenadas GPS
- Muestra el número de registros históricos
- Permite ver detalles específicos de cada zona

**Opción 5: Alertas**
- Genera alertas automáticas si hay niveles peligrosos
- Incluye alertas preventivas basadas en predicciones
- Calcula el porcentaje de exceso sobre los límites

**Opción 6: Recomendaciones**
- Genera consejos específicos por zona
- Clasifica por prioridad (Alta, Media, Baja)
- Categoriza por tipo (Tráfico, Industrial, Salud Pública)

**Opción 7: Reporte Completo**
- Crea un archivo de texto con toda la información
- Incluye datos actuales, predicciones e índices
- Útil para análisis detallado o presentaciones

**Opción 8: Exportar Datos**
- Crea una copia de respaldo en formato binario
- Incluye todos los datos del sistema
- Útil para transferir datos o hacer respaldos

**Opción 1000: Reiniciar Sistema**
- Elimina TODOS los datos guardados
- Restaura el sistema a configuración inicial
- Requiere confirmación del usuario

## 📊 Límites de Contaminación (OMS)

El sistema utiliza los siguientes límites basados en las recomendaciones de la Organización Mundial de la Salud:

| Contaminante | Límite (μg/m³) | Descripción |
|--------------|----------------|-------------|
| PM2.5        | 25.0          | Partículas finas menores a 2.5 micrómetros |
| PM10         | 50.0          | Partículas menores a 10 micrómetros |
| CO2          | 40,000.0      | Dióxido de carbono |
| SO2          | 20.0          | Dióxido de azufre |
| NO2          | 40.0          | Dióxido de nitrógeno |

## 🏙️ Zonas Monitoreadas en Quito

| ID | Zona | Latitud | Longitud | Descripción |
|----|------|---------|----------|-------------|
| 1  | Sector Udlapark | -0.2298 | -78.5249 | Zona norte de Quito |
| 2  | Mitad del Mundo | -0.0022 | -78.4558 | Zona norte extrema |
| 3  | Parque la Carolina | -0.1807 | -78.4678 | Zona centro-norte |
| 4  | El Panecillo | -0.2274 | -78.5170 | Centro histórico |
| 5  | Catedral Metropolitana | -0.2201 | -78.5123 | Centro histórico |

## 🔧 Funcionalidades Técnicas

### Algoritmo de Predicción
- **Promedio ponderado**: Da mayor peso a datos más recientes
- **Factor climático**: Considera temperatura, humedad y viento
- **Validación automática**: Verifica la coherencia de datos

### Gestión de Memoria
- **Estructuras eficientes**: Optimizadas para el rendimiento
- **Arrays de tamaño fijo**: Evita problemas de memoria dinámica
- **Límites controlados**: Máximo 10 zonas, 30 días de historial

### Persistencia de Datos
- **Formato binario**: Más rápido y eficiente que texto
- **Carga automática**: Al iniciar el programa
- **Guardado automático**: Después de cambios importantes
- **Validación de archivos**: Manejo de errores de E/S

## 🚨 Solución de Problemas

### Errores Comunes

**Error: "No se encontró archivo de datos"**
- Es normal en la primera ejecución
- El sistema creará automáticamente el archivo

**Error: "ID de zona inválido"**
- Verificar que el ID esté entre 1 y 5
- Las zonas están predefinidas en el sistema

**Error: "Error al crear archivo"**
- Verificar permisos de escritura en el directorio
- Asegurarse de tener espacio en disco

### Consejos de Uso

1. **Ingreso de Datos**: Usar valores realistas (no números negativos para contaminantes)
2. **Respaldos**: Usar la opción 8 regularmente para crear copias de seguridad
3. **Análisis**: Usar la opción 7 para generar reportes detallados
4. **Limpieza**: Solo usar la opción 1000 si realmente necesitas empezar de cero

## 📈 Interpretación de Resultados

### Índice de Calidad del Aire
- **0-50**: Buena calidad
- **51-100**: Calidad moderada
- **101-150**: Insalubre para grupos sensibles
- **151-200**: Insalubre
- **201-300**: Muy insalubre
- **300+**: Peligrosa

### Recomendaciones por Categoría
- **Tráfico**: Medidas relacionadas con vehículos y transporte
- **Industrial**: Acciones sobre emisiones industriales
- **Salud Pública**: Consejos para proteger a la población

## 🔄 Actualizaciones y Mantenimiento

### Agregar Nueva Zona
1. Modificar `MAX_ZONAS` en `funciones.h`
2. Agregar llamada a `agregar_zona()` en `inicializar_sistema()`
3. Recompilar el programa

### Cambiar Límites de Contaminación
1. Modificar las constantes `LIMITE_*` en `funciones.h`
2. Recompilar el programa

### Agregar Nuevo Contaminante
1. Modificar estructura `DatosContaminacion`
2. Actualizar funciones de predicción y análisis
3. Modificar interfaces de usuario

## 📞 Información de Contacto

Este sistema fue desarrollado como parte de un proyecto académico para el monitoreo de calidad del aire en Quito. Para preguntas técnicas o sugerencias de mejora, consulte la documentación en el código fuente.

## 📄 Licencia

Este proyecto es de código abierto y puede ser utilizado con fines educativos y de investigación.

---

**Nota**: Este sistema está diseñado para propósitos educativos y de demostración. Para uso en producción, se recomienda validación adicional y pruebas exhaustivas.
