*Este proyecto ha sido creado como parte del currículo de 42 por dmena-li*

# Codexion
Una simulación de concurrencia para dominar el threading y la sincronización de recursos en C

## 📝 Descripción

Codexion es una simulación de concurrencia exhaustiva en C diseñada para dominar el manejo de hilos y la sincronización de recursos. Basado en el clásico problema de los Filósofos Comensales, el proyecto modela un centro de trabajo circular donde los Coders (hilos) compiten por un número limitado de USB Dongles (mutexes) para poder compilar su trabajo.

El desafío central consiste en implementar algoritmos de arbitraje eficientes para gestionar la contienda de recursos sin que ningún programador llegue al "burnout" (inanición). La simulación garantiza la estabilidad del sistema mediante:

Planificación Adaptativa: Elección entre FIFO (equidad basada en espacios de cola explícitos) o EDF (Earliest Deadline First) para gestionar las listas de espera.

Prevención de Deadlocks: La implementación de una estrategia Par/Impar evita ciclos de dependencia.

Seguridad de Hilos: Un hilo Monitor dedicado que rastrea los cambios de estado para detectar condiciones de terminación.

## 🚀 Uso

### Ejecución
Ejecuta el programa con los siguientes argumentos para iniciar la simulación:

bash** **./codexion <coders> <t_burnout> <t_compile> <t_debug> <t_refactor> <n_compiles> <cooldown> <scheduler>** **

| Argumento | Descripción | Ejemplo |
| :--- | :--- | :--- |
| coders | Número total de coders (hilos) y dongles. | 10 |
| t_burnout | Tiempo (ms) que un coder sobrevive sin compilar. | 800 |
| t_compile | Tiempo (ms) dedicado a compilar (usando 2 dongles). | 200 |
| t_debug | Tiempo (ms) dedicado a depurar (sin dongles). | 200 |
| t_refactor | Tiempo (ms) dedicado a refactorizar (sin dongles). | 100 |
| n_compiles | La simulación se detiene si todos alcanzan este número. | 4 |
| cooldown | Tiempo (ms) que un dongle queda inactivo tras su uso. | 100 |
| scheduler | Política de arbitraje: fifo o edf. | fifo |

## ⚙️ Arquitectura Técnica

La simulación se basa en hilos POSIX (pthread) y utiliza una combinación específica de mutexes y gestión de colas explícitas para coordinar la interacción entre Coders, Dongles y el Monitor.

### Primitivas de Sincronización

El proyecto evita las difusiones estándar de variables de condición para la espera de recursos, optando en su lugar por mutexes específicos que protegen espacios de cola explícitos.

Control Global (s_data)

threads_lock & threads_cond: Actúan como un mecanismo de barrera para asegurar que todos los hilos (n_coders y monitor) inicien la simulación simultáneamente.

print_lock: Serializa la salida por consola para evitar logs entrelazados de hilos concurrentes.

flag_lock: Protege la variable global monitor_flag. Este booleano indica el fin de la simulación, ya sea por "burnout" o por completar todas las compilaciones.

Mutexes de Recurso (s_dongle)

take_lock: Representa la posesión física exclusiva de un dongle por parte de un coder.

cooldown_lock: Protege la marca de tiempo next_available_time, asegurando que el periodo de enfriamiento sea seguro entre hilos.

queue_lock: Crítico para la lógica de planificación. Protege los espacios de espera explícitos (first_in_queue y second_in_queue) en cada dongle, evitando condiciones de carrera.

Mutexes de Coder (s_coder)

compile_lock: Protege las estadísticas vitales del programador (last_compile y times_compiled). Esto permite que el hilo Monitor lea el estado de un coder sin causar carreras de datos.

### Algoritmos de Planificación

La lógica de arbitraje utiliza un enfoque basado en sondeo (polling) combinado con espacios de cola explícitos en cada estructura de dongle:

1. FIFO (First-In, First-Out)

Entrada a la cola: Los coders intentan ocupar uno de los dos espacios disponibles (first o second) protegidos por queue_lock.

Estrategia de espera: Si ambos espacios están llenos, el hilo cede la ejecución (usleep) y reintenta más tarde.

Adquisición: Una vez en la cola, el coder espera a la posición first_in_queue y a que expire el cooldown antes de bloquear el dongle.

2. EDF (Earliest Deadline First)

Verificación de prioridad: Antes de otorgar el recurso, el planificador compara la marca de tiempo last_compile de los coders en los espacios de cola.

Selección dinámica: La función closest_deadline determina qué coder en cola lleva más tiempo sin compilar. Ese hilo recibe prioridad para bloquear el dongle, evitando la inanición de los hilos críticos.

### Sistema de Monitoreo

Un Hilo Monitor dedicado se ejecuta en paralelo para asegurar que se respeten las reglas:

Rutina: Itera sobre todos los coders en un bucle continuo.

Detección de Burnout: Bloquea compile_lock para calcular el tiempo transcurrido. Si (tiempo_actual - last_compile) >= t_burnout, activa la bandera de parada.

Finalización: Rastrea el número total de coders que han compilado las veces requeridas. Si se alcanza el objetivo, la simulación termina con éxito.
