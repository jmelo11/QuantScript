# QuantScript
Scripting language for describing financial cashflows

Pendientes de implementacion:

1- Actualmente esta pendiente la implementación de los nodos de definicion de variables.
	-Problema a resolver: cuando se define un macro, por ejemplo S=100, "S" seria remplazado por macroReplacer por 100, quedando constante
	-La idea es implementar un indexador que remplace los nodos "varible" por nodos "difinicion". Para esto es necesario utilizar un iterador especial.

2- Debe existir un nodo temporal, o idexar el tiempo a los AADNumbers, por que es necesario poder calcular sensibilidades al envejecimiento [theta]

3-implementar vectores

4-implementar modelos de tasas

5-implementar multiples monedas

Pendientes de ordenamiento del proyecto:

Es necesario documentar y ordenar el codigo. Paso 1 seran ordernar los AADNumbers
	- comentar
	- renombrar variables
	- definir setters y getters

- Necesito crear tareas pequeñas para poder avanzar, aun que sea lento...
- sera buena idea pasarlo a github?


## Tareas
1- migrar proyecto a github, y cambiar los nombres /OK
	- se creo la carpeta, se migraron los archivos y se pudo compilar, aleluya, pero falta subirlo. /OK

2- Antes de subirlo, hay que revisar bien que se ignoren los archivos compilados, y ver como se
puede referenciar quantlib y boost a las versiones que esten en github.
	- Al parecer se podría con quantlib, pero boost no se encuentra en github.

3- hay un problema con el archivo "parse.h", por que se confunde con un archivo de la libreria estandar. Hay que revisar bien como 
se cambia los directorios.

4- encuentro que se ven medio desordenado tener los archivos con formato "carpeta/archivo" por que queda la carpeta con el miso nombre que el archivo.
	- para mejor orden, cambiarle el nombre a la carpeta QuantScript por "qs" -parecido a lo que hace quantlib- para que las rutas queden como qs/.../...
	- C++: como se hara para utilizar <> en vez de ""?

5- Comentar AADNumbers 
6- Renombrar variables y definir setter y getters
7- Cambiar usuarios de estos numeros para que utilizen los setters y guetters.

POR AHORA...

