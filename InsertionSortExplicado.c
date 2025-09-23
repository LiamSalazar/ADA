void insertion_sort(int a[], int n) {                                     
    // Espacio TOTAL: arreglo a → n ; n (parámetro) → O(1)
    // Espacio ADICIONAL del algoritmo: O(1)

    for (int i = 1; i < n; i++) {                                         
        // TIEMPO (for externo):
        //   init i=1 → 1
        //   comparaciones i<n → n
        //   incrementos i++ → n-1
        //   2n
        // ESPACIO: i → O(1)

        int key = a[i];                                                   
        // TIEMPO: 1 asignación (O(1))
        // ESPACIO: key → O(1)

        int j = i - 1;                                                    
        // TIEMPO: 1 asignación (O(1))
        // ESPACIO: j → O(1)

        while (j >= 0 && a[j] > key) {                                    
            // TIEMPO en peor caso (se desplazan i elementos):
            //   j>=0 → i+1 veces
            //   a[j] > key → i veces
            // ESPACIO: no añade memoria (O(1))

            a[j + 1] = a[j];                                              
            // TIEMPO: i asignaciones (O(i))
            // ESPACIO: O(1)

            j--;                                                          
            // TIEMPO: i decrementos (O(i))
            // ESPACIO: O(1)
        }

        a[j + 1] = key;                                                   
        // TIEMPO: 1 asignación (O(1))
        // ESPACIO: O(1)
    }
}

// -------------------------------------------------------
// FUNCIONES FINALES
//
// TIEMPO (peor caso):
//   ∑(4i+4) para i=1..n-1  + 2n
//   = 2n^2 + 4n - 4  ⇒ O(n^2)
//
// TIEMPO (mejor caso):
//   7n - 5 ⇒ O(n)
//
// ESPACIO adicional:
//   3 variables (i, j, key) ⇒ 3 ⇒ O(1)
//
// ESPACIO total:
//   n (arreglo) + 3 ⇒ n+3 ⇒ O(n)
// -------------------------------------------------------
