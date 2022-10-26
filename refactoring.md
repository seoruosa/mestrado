# Reescrevendo o código

## Táticas

* Diminuir quantidade de vezes que as funções são avaliadas

* Dividir metaheurísticas em módulos para facilitar teste
* 


* Adicionar logging/profiling aos códigos
    * Definir:
        * nomenclatura de arquivos
        * cabeçalho
        * formato do arquivo
        * pastas


* Dividir NSGAII em módulos, tal que seja possível mudar:
    * operador de mutação
    * gerador de solução inicial
    * fazer o algoritmo principal ser indiferente à representação
    * não precisa recriar o vetor de próxima geração toda vez (sobrescrever a existente)
    * funções devem depender de uma única semente para os geradores de número aleatório


* Dividir em subprojetos
    * splitting
    * ler instancia
    * heuristicas
    * metaheuristicas
        * NSGAII

* Criar scripts
    * rodar experimentos 
    * instalação
    * limpeza do ambiente
    * salvando resultados
    * gerador de instâncias

## Fazer
* Reescrever NSGAII
* Reescrever função de splitting
* Adicionar testes unitários e de integração
* Adicionar inicialização de sementes para os geradores aleatórios (manter reprodutibilidade)

* Organizar e limpar o arquivo `CMakeLists.txt`
* Tem arquivos `.cpp` que estão incluindo outros arquivos `.cpp`
* Separar arquivos cpp em: testes, src, include