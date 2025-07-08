# Sistema Integral de Gestión de Contaminación del Aire - Quito

## Descripción
Sistema completo de monitoreo de calidad del aire para la ciudad de Quito, Ecuador. Permite registrar, analizar y predecir niveles de contaminación en 5 zonas urbanas estratégicas.

## Características Principales
- **Monitoreo en tiempo real** de 5 contaminantes principales (PM2.5, PM10, CO2, SO2, NO2)
- **Historial de 7 días** para análisis de tendencias
- **Predicciones de 24 horas** basadas en algoritmos inteligentes
- **Sistema de alertas** basado en estándares de la OMS
- **Validación robusta** de datos con protección contra valores inválidos
- **Edición y eliminación** de zonas de monitoreo
- **Reportes completos** exportables
- **Almacenamiento binario** optimizado

## Nuevas Funcionalidades (Versión 2.0)

### 🔧 Edición de Datos
- Edición individual o completa de datos de contaminación
- Preservación automática del historial al editar
- Validación en tiempo real de todos los valores

### 🗑️ Eliminación de Zonas
- Eliminación completa de zonas con confirmación doble
- Preservación de la integridad del sistema
- Advertencias claras sobre pérdida de datos

### ✅ Validación Avanzada
- **Valores no negativos**: PM2.5, PM10, CO2, SO2, NO2, humedad, velocidad del viento
- **Valores que pueden ser negativos**: Solo la temperatura (-50°C a 60°C)
- **Protección contra letras**: Solo acepta números válidos
- **Rangos específicos** para cada tipo de medición
- **Reintentos automáticos** con valores por defecto en caso de error

### 📊 Historial Optimizado
- Reducido a **7 días** para mejor rendimiento
- Visualización completa del historial en detalles de zona
- Rotación automática de datos antiguos

## Zonas Monitoreadas
1. **Sector Udlapark** (Norte) - Coordenadas: -0.2298, -78.5249
2. **Mitad del Mundo** (Norte Extrema) - Coordenadas: -0.0022, -78.4558
3. **Parque la Carolina** (Centro-Norte) - Coordenadas: -0.1807, -78.4678
4. **El Panecillo** (Centro Histórico) - Coordenadas: -0.2274, -78.5170
5. **Catedral Metropolitana** (Centro) - Coordenadas: -0.2201, -78.5123

## Menú Principal
1. **Estado actual** - Ver niveles actuales de todas las zonas
2. **Predicciones** - Proyecciones para próximas 24 horas
3. **Ingresar datos** - Registrar nuevas mediciones (con validación)
4. **Info de zonas** - Detalles completos y historial de 7 días
5. **Alertas** - Verificar excesos de límites OMS
6. **Recomendaciones** - Sugerencias específicas por zona
7. **Reporte completo** - Documento exportable
8. **Copia de respaldo** - Exportación binaria
9. **🆕 Editar datos** - Modificar datos existentes
10. **🆕 Eliminar zona** - Remover zona completa del sistema
0. **Salir** - Cierre seguro con auto-guardado
1000. **Reinicio total** - Restaurar configuración inicial

## Límites de Seguridad (OMS)
- **PM2.5**: ≤ 25.0 μg/m³
- **PM10**: ≤ 50.0 μg/m³  
- **CO2**: ≤ 40,000 μg/m³
- **SO2**: ≤ 20.0 μg/m³
- **NO2**: ≤ 40.0 μg/m³

## Rangos de Validación
- **PM2.5, PM10, SO2, NO2**: 0 - 1,000 μg/m³
- **CO2**: 0 - 100,000 μg/m³
- **Temperatura**: -50°C a 60°C (puede ser negativa)
- **Humedad**: 0% - 100%
- **Velocidad del viento**: 0 - 200 km/h

## Compilación y Ejecución
```bash
# Compilar
gcc -o sistema_contaminacion main.c funciones.c

# Ejecutar
./sistema_contaminacion
```

## Archivos Generados
- `datos_contaminacion.dat` - Base de datos binaria principal
- `reporte_contaminacion.txt` - Reportes completos
- `datos_contaminacion_exportados.dat` - Copias de respaldo

## Características Técnicas
- **Lenguaje**: C (estándar C99)
- **Archivos**: 3 archivos principales (main.c, funciones.c, funciones.h)
- **Dependencias**: Solo librerías estándar de C
- **Almacenamiento**: Binario optimizado
- **Validación**: Robusta con protección de tipos

## Seguridad de Datos
- **Auto-guardado** después de cada operación crítica
- **Validación exhaustiva** de todas las entradas
- **Confirmación doble** para operaciones destructivas
- **Respaldo automático** del historial al editar
- **Manejo de errores** con valores por defecto seguros

## Versión
**v2.0** - Sistema con validación avanzada, edición de datos y historial optimizado a 7 días
