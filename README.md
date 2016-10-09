# FCUL-SD

Em várias funções, e.g.: entry_create(), não realizamos a libertação de memória 
de alguns ponteiros através da função free(), visto que entravam em conflito com 
os ficheiros de testes que já faziam essa libertação - destroy(). Delegamos essa
responsabilidade então para o ficheiro de testes que serviam como cliente.

Ao realizar o teste AddVarios, verificamos que a inserção de forma ordenada funcionava
para chaves (keys) até 10 entradas e passava no teste, mas à décima primeira, tinhamos 
uma Segmentation fault, falhando o teste. Revemos o código, mas não conseguimos perceber
onde está a falta, uma vez que utilizamos a função strcmp() que à partida ordena as strings
de vários tamanhos, o que não está a funcionar neste caso.
