/*
 * GUIA DE COMPRENSION DEL CODIGO
 * SISTEMA DE MONITOREO DE CONTAMINACION DEL AIRE - QUITO
 * 
 * Esta guía explica paso a paso cómo funciona el programa para alguien
 * que está aprendiendo programación en C o necesita entender el sistema.
 */

/*
 * ============================================================================
 * 1. CONCEPTOS BASICOS QUE DEBES ENTENDER
 * ============================================================================
 */

/*
 * ESTRUCTURAS (struct):
 * Una estructura es como una "caja" que contiene varios datos relacionados.
 * Por ejemplo, DatosContaminacion contiene todos los datos de un día:
 * - Fecha (día, mes, año)
 * - Niveles de contaminantes (PM2.5, PM10, etc.)
 * - Condiciones climáticas (temperatura, humedad, viento)
 * 
 * Es como una ficha de datos que agrupa información relacionada.
 */

/*
 * PUNTEROS (*):
 * Un puntero es como una "dirección" que te dice dónde está guardado algo en la memoria.
 * Cuando vemos "SistemaContaminacion *sistema", significa que estamos
 * pasando la dirección donde está el sistema, no una copia.
 * Esto permite que las funciones modifiquen los datos originales.
 */

/*
 * ARRAYS []:
 * Un array es como una fila de casillas numeradas donde guardas cosas del mismo tipo.
 * Por ejemplo, zonas[5] es como tener 5 casillas para guardar información de 5 zonas.
 * Las casillas se numeran desde 0: zonas[0], zonas[1], zonas[2], zonas[3], zonas[4]
 */

/*
 * ============================================================================
 * 2. FLUJO PRINCIPAL DEL PROGRAMA
 * ============================================================================
 */

/*
 * PASO 1: INICIALIZACION (main.c líneas 20-30)
 * 
 * Al ejecutar el programa:
 * 1. Se crea una variable 'sistema' que contendrá todos los datos
 * 2. Se llama a inicializar_sistema() que:
 *    - Configura las 5 zonas de Quito con sus nombres y coordenadas GPS
 *    - Pone todos los contadores en cero
 *    - Marca la hora de inicio
 * 3. Se llama a cargar_datos_archivo() que:
 *    - Busca si existe un archivo con datos guardados anteriormente
 *    - Si existe, carga todos los datos al sistema
 *    - Si no existe, continúa con datos en blanco
 */

/*
 * PASO 2: MENU PRINCIPAL (main.c líneas 35-120)
 * 
 * El programa entra en un bucle que:
 * 1. Muestra el menú de opciones
 * 2. Espera que el usuario escriba un número
 * 3. Ejecuta la función correspondiente según el número elegido
 * 4. Vuelve a mostrar el menú (hasta que el usuario elija salir con 0)
 * 
 * Este es el patrón típico de un programa interactivo.
 */

/*
 * PASO 3: PROCESAMIENTO DE OPCIONES
 * 
 * Cada opción del menú llama a una función específica:
 * - Opción 1: mostrar_estado_actual() - Lee los datos y los muestra en pantalla
 * - Opción 3: ingresar_datos_actuales() - Solicita datos al usuario y los guarda
 * - Opción 5: generar_alertas() - Verifica si hay niveles peligrosos
 * - etc.
 */

/*
 * ============================================================================
 * 3. COMO FUNCIONA EL ALMACENAMIENTO DE DATOS
 * ============================================================================
 */

/*
 * ESTRUCTURA DE DATOS EN MEMORIA:
 * 
 * SistemaContaminacion
 * ├── zonas[0] (Sector Udlapark)
 * │   ├── id: 1
 * │   ├── nombre: "Sector Udlapark"
 * │   ├── latitud: -0.2298
 * │   ├── longitud: -78.5249
 * │   ├── datos_actuales (último registro)
 * │   ├── historial[30] (hasta 30 días de historia)
 * │   └── prediccion_24h[5] (predicciones de 5 contaminantes)
 * ├── zonas[1] (Mitad del Mundo)
 * ├── zonas[2] (Parque la Carolina)
 * ├── zonas[3] (El Panecillo)
 * ├── zonas[4] (Catedral Metropolitana)
 * ├── num_zonas: 5
 * └── ultima_actualizacion: (timestamp)
 */

/*
 * ARCHIVOS BINARIOS:
 * 
 * El programa guarda los datos en formato binario, no en texto legible.
 * Esto es más eficiente porque:
 * - Ocupa menos espacio en disco
 * - Se carga más rápido
 * - Mantiene la precisión exacta de los números decimales
 * 
 * Archivos que genera el programa:
 * - datos_contaminacion.dat: Datos principales (se carga automáticamente)
 * - reporte_contaminacion.txt: Reporte en texto (legible para humanos)
 * - datos_contaminacion_exportados.dat: Copia de respaldo
 */

/*
 * ============================================================================
 * 4. COMO FUNCIONAN LAS PREDICCIONES
 * ============================================================================
 */

/*
 * ALGORITMO DE PREDICCION (funciones.c línea 230):
 * 
 * Para predecir la contaminación de mañana, el sistema:
 * 
 * 1. ANALIZA EL HISTORIAL:
 *    - Toma los datos de días anteriores
 *    - Da más importancia a los días más recientes
 *    - Calcula un promedio ponderado
 * 
 * 2. CONSIDERA EL CLIMA:
 *    - Si hay poco viento, la contaminación se acumula más
 *    - Si hace mucho calor, algunos contaminantes aumentan
 *    - Si hay mucha humedad, las partículas se comportan diferente
 * 
 * 3. APLICA FACTORES DE CORRECCION:
 *    - Ajusta el resultado según las condiciones climáticas
 *    - Limita los resultados a rangos realistas
 * 
 * Es como un meteorólogo que predice el clima, pero para la contaminación.
 */

/*
 * ============================================================================
 * 5. COMO FUNCIONAN LAS ALERTAS
 * ============================================================================
 */

/*
 * SISTEMA DE ALERTAS (funciones.c línea 290):
 * 
 * El sistema compara los valores medidos con los límites de la OMS:
 * 
 * LIMITES DE SEGURIDAD:
 * - PM2.5: 25 μg/m³ (partículas muy pequeñas, las más peligrosas)
 * - PM10: 50 μg/m³ (partículas pequeñas)
 * - CO2: 40,000 μg/m³ (dióxido de carbono)
 * - SO2: 20 μg/m³ (dióxido de azufre, de industrias)
 * - NO2: 40 μg/m³ (dióxido de nitrógeno, de vehículos)
 * 
 * TIPOS DE ALERTAS:
 * - ALERTA ACTUAL: Los niveles de hoy superan el límite
 * - ALERTA PREVENTIVA: Se predice que mañana superarán el límite
 * 
 * Cada alerta muestra:
 * - Qué contaminante está alto
 * - Cuánto supera el límite
 * - En qué porcentaje excede el límite seguro
 */

/*
 * ============================================================================
 * 6. COMO LEER Y MODIFICAR EL CODIGO
 * ============================================================================
 */

/*
 * ARCHIVOS PRINCIPALES:
 * 
 * funciones.h:
 * - Define las estructuras de datos (como los planos de una casa)
 * - Declara qué funciones existen (como un índice)
 * - Define constantes (valores que no cambian)
 * 
 * funciones.c:
 * - Contiene el código real de todas las funciones
 * - Es donde está la lógica del programa
 * - Dividido en secciones temáticas
 * 
 * main.c:
 * - Contiene la función main() que ejecuta el programa
 * - Maneja el menú principal y la interacción con el usuario
 * - Coordina las llamadas a las otras funciones
 */

/*
 * COMO AGREGAR UNA NUEVA FUNCIONALIDAD:
 * 
 * 1. Planifica qué quieres hacer
 * 2. Agrega la declaración de la función en funciones.h
 * 3. Implementa la función en funciones.c
 * 4. Agrega una opción en el menú de main.c
 * 5. Compila y prueba
 * 
 * Ejemplo: Para agregar un nuevo contaminante (como ozono):
 * 1. Agregar 'float ozono;' en la estructura DatosContaminacion
 * 2. Agregar '#define LIMITE_OZONO 100.0' en las constantes
 * 3. Modificar las funciones de ingreso de datos para pedirlo
 * 4. Modificar las funciones de predicción para incluirlo
 * 5. Modificar las funciones de alertas para verificarlo
 */

/*
 * ============================================================================
 * 7. PREGUNTAS FRECUENTES
 * ============================================================================
 */

/*
 * P: ¿Por qué se usan punteros en lugar de copiar las estructuras?
 * R: Las estructuras son grandes (contienen mucha información). Copiarlas
 *    sería lento y consumiría mucha memoria. Los punteros permiten trabajar
 *    directamente con los datos originales.
 * 
 * P: ¿Por qué archivos binarios en lugar de texto?
 * R: Los archivos binarios son más rápidos de leer/escribir y ocupan menos
 *    espacio. Para datos que el usuario no necesita leer directamente,
 *    es más eficiente.
 * 
 * P: ¿Cómo se calculan las predicciones?
 * R: Se usa un promedio ponderado de datos históricos, ajustado por factores
 *    climáticos. Es una aproximación basada en patrones pasados.
 * 
 * P: ¿Se pueden cambiar las zonas monitoreadas?
 * R: Sí, modificando la función inicializar_sistema() en funciones.c.
 *    Cambiar las coordenadas y nombres de las zonas.
 * 
 * P: ¿Qué pasa si se corrompe el archivo de datos?
 * R: El programa detectará el error y empezará con datos en blanco.
 *    Por eso es importante usar la opción 8 para hacer respaldos.
 */

/*
 * ============================================================================
 * 8. CONSEJOS PARA PROGRAMADORES PRINCIPIANTES
 * ============================================================================
 */

/*
 * COMO ENTENDER EL CODIGO:
 * 
 * 1. Empieza por main.c - es el punto de entrada
 * 2. Sigue el flujo del programa paso a paso
 * 3. Lee los comentarios antes que el código
 * 4. No trates de entender todo a la vez
 * 5. Enfócate en una función específica
 * 
 * COMO DEBUGGEAR (ENCONTRAR ERRORES):
 * 
 * 1. Agrega printf() para ver qué valores tienen las variables
 * 2. Compila frecuentemente para encontrar errores de sintaxis
 * 3. Prueba una función a la vez
 * 4. Usa datos de prueba simples
 * 5. Lee los mensajes de error cuidadosamente
 * 
 * BUENAS PRACTICAS:
 * 
 * 1. Siempre comenta tu código
 * 2. Usa nombres descriptivos para variables y funciones
 * 3. Divide problemas grandes en funciones pequeñas
 * 4. Verifica los valores de entrada de las funciones
 * 5. Maneja los errores apropiadamente (archivos que no existen, etc.)
 */

/*
 * ============================================================================
 * FIN DE LA GUIA
 * ============================================================================
 * 
 * Esta guía te ayudará a entender cómo funciona el sistema de monitoreo.
 * Recuerda: la programación se aprende practicando. No tengas miedo de
 * experimentar con el código, hacer cambios y ver qué pasa.
 * 
 * ¡Buena suerte programando!
 */
