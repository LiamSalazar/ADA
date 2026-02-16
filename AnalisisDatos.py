lista = [[1,2],[3,4],[5,6]]
lista_float = [[float(num) for num in sublista] for sublista in lista]
forma_dos = [[float(e1), float(e2)] for e1, e2 in lista]
print(lista_float)
print(forma_dos)