/*
02/09/2025
Salazar Martínez Liam Antonio
Antonio Zavala Julio César
3AM1
Opción 2
*/



# include <stdio.h>

int main(){
    int opcion = 0;
    int menor = 0;
    int mayor = 1000;
    int guess = 0;
    int contador = 0;
    puts("Piensa en un numero entre 0 y 999 para adivinar:");
    puts("1) Adivinado");
    puts("2) Mi número es mayor");
    puts("3) Mi número es menor");
    while(opcion != 1){
        contador++;
        guess = (mayor + menor)/2;
        printf("El número es: %d\n", guess);    
        scanf("%d", &opcion);
        if(opcion == 2)
            menor = guess;
        else if(opcion == 3)
            mayor = guess;
        else if(opcion == 1){
            puts("ADIVINADO");
            break;
        }
    }
    printf("Cantidad de preguntas hechas: %d\n", contador);
    int divisor = 2;
    int esPrimo = 1;
    while(divisor < guess){
        if(guess%divisor == 0)
            esPrimo = 0;
        divisor++;
    }
    if(esPrimo && guess > 1)
        puts("Es primo");
    else
        puts("No es primo");
    return 0;
}

/*
¿Cuál fue el número mayor de preguntas? 
El máximo fue 10, siendo esto debido a que es un algoritmo de complejidad O(log|n|)

¿Cuál fue el número menor de preguntas?
1 sola pregunta cuando el número esta justo a la mitad entre los límites (500).

¿Cuál fue el promedio de preguntas realizadas? 
El promedio de preguntas realizadas estuvo cerca del peor caso, ya que fueron 9 en la mayoría de casos.

*/