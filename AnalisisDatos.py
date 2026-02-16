lista = [[1,2],[3,4],[5,6]]
lista_float = [[float(num) for num in sublista] for sublista in lista]
forma_dos = [[float(e1), float(e2)] for e1, e2 in lista]
print(lista_float)
print(forma_dos)

lista_zeroCo = []
lista_zeroCo = [[0 for j in range(2)] for i in range(2)]

lista_zero = []
for i in range(2):
    sublista = []
    for j in range(2):
        sublista.append(0)
    lista_zero.append(sublista)
print(lista_zero)
print(lista_zeroCo)