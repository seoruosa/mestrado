# Mestrado sobre o problema de roteamento de mamógrafos móveis (Mobile Mammography Unit Routing Problem)



## Comandos
Para rodar o jupyter notebook com o solver Gurobi faça:

```
docker-compose -f roteamento-carretas-da-mamografia/docker-compose_optimizer-python-mip.yml up
```

Para abrir um bash em um terminal faça:

```
docker exec -it gurobi-python-mip /bin/bash
```

Para compilar usando g++:
```
cd /opt/project/src/cpp
g++-10 -g fibo.cpp -o fibo -std=c++2a
g++-10 -g read_instance.cpp -o out -std=c++2a && ./out
```

Para inicializar o projeto usando cmake:

```
cd roteamento-carretas-da-mamografia/src/cpp/
mkdir build && cd build && cmake ../
```

e para construir:
```
cd roteamento-carretas-da-mamografia/src/cpp/build
cmake --build .
```

Rodando um arquivo `.py` com um teste simples usando o MIP no bash
```
python3 modelo_teste.py
```

_Para utilizar o gurobi é necessário adicionar o arquivo `gurobi.lic` com a licença de uso._

--- 

## To Do
* Mudar função python de leitura de instância para devolver um objeto instância (criar classe para tal...)
* ~~Comandos para acessar bash do docker~~
* ~~Compilando código C++ no docker~~
* Configurar vscode para buildar/compilar cpp
    * Passo a passo para debugar
* ~~Mudar do notebook para um arquivo .py~~
* ~~Criar hello world no cmake~~
* Funções:
  * Testar se uma solução é factível
  * Quantidade de veículos usados
  * Distância percorrida por todos os veículos
* criar teste unitário

* Na implementação do NSGAII 
    * Adicionar melhores soluções na solução inicial
    * Criar função de splitting do cromossomo que devolva o resultado mais "dominante" -> "ótimo"
    * Testar função acima para o procedimento de busca
* Implementar modelo para construir o pareto (talvez adicionando algum procedimento de busca junto (metaheuristica)) -> alg. memético
* Testar a leitura das instâncias de MG e ES
    * provavelmente será necessário a mudança do método de ler instancia
* Implementar logging no algoritmo para colher medidas
    * Onde salvar?
* Criar função para salvar saída em formato csv formatado (com `,` no lugar de `.`)
* __Criar função de calculo de métricas de problemas MO__
    * Set Coverage Metric (Zitzler 1999)
    * Spacing Measure (Schott 1995)
    * S-metric (Zitzler & Thiele 1999)
* Criar visualizador de soluções
* vale limpar cidades com demanda zero???
* Vale instalar mip com pypy???



### NSGA-II (Need to implement)
#### Could be reused
* ~~crowding distance~~
* ~~fast non dominated sort~~
* ~~binary tournament selection~~
    * ~~select pairs of individuals~~
    * ~~compare individuals~~

#### Dependent of problem (implemented a real coded version)
* Chromossome
* Recombination
* Mutation
* initialize_population

#### C++ books
* Meyers, Scott - Effective C++ 55 Specific Ways to Improve Your Programs and Designs-Pearson Education Limited (US titles)_Addison Wesley Professional (2005)
