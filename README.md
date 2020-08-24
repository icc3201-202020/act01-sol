# Actividad Evaluada 1
Actividad Evaluada 1 - Parte práctica\
**NOMBRE: [Escribe aquí tu nombre]**\
**CORREO: [Escribe aquí tu correo @miuandes.cl]**

En esta parte de la actividad deberás completar el código de dos programas, `main` y `child`, de modo que 
el programa `main` pueda iniciar el programa `child` en un proceso hijo, y enviarle un flujo de dígitos 0-9 
a través de un pipe.

Debes empezar por revisar el programa `main` y revisar el comentario que especifica el código que se debe
completar. Primero, se debe crear un proceso hijo con `fork`. Luego, se debe ejecutar desde el proceso hijo
el programa `child` (ruta `./child`) con la llamada `execv`, pasando como argumento a dicho programa a través
de exec el descriptor del extremo de lectura del pipe, a fin de que el proceso hijo pueda leer caracteres
desde allí. El proceso padre debe esperar a que el hijo termien.

Luego, debes modificar el programa `child` para agregarle dos threads. El primer thread convierte los
caracteres leídos desde el pipe, desde tipo `char` a `unsigned short`, ejecutando la función `convertandsum`,
la cual además calcula la suma de los dígitos. Finalmente, el segundo thread ejecuta quicksort sobre los
dígitos convertidos.

Para facilitar la compilación y pruebas, simplemente ejecuta `make` y `make clean` en este directorio.
Es normal que veas dos warnings en el código inicial. Éstos desaparecerán cuando hayas completado los
programas.

No olvides [configurar git](https://linuxize.com/post/how-to-configure-git-username-and-email/) con tu 
usuario, nombre y correo, a fin de que que tu entrega quede correctamente a tu nombre.
