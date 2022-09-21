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

para rodar:

```
./NSGAII_MMURP ../../../Instancias/old_testing_instances/Vrp-Set-A_25p/A-n249-m3-Q500-v7.vrp --maxDistNodes 200 --maxNumVehicles 10 --sizePop 50 --numGen 50 --mutationRate 0.1

```

Rodando um arquivo `.py` com um teste simples usando o MIP no bash
```
python3 modelo_teste.py
```

_Para utilizar o gurobi é necessário adicionar o arquivo `gurobi.lic` com a licença de uso. Esta licença pode ser conseguida acessando o seguinte [endereço](https://license.gurobi.com/manager/licenses)_

--- 

### Rodando análises localmente

### Instalação
```
    source setup.sh
```

### Ativação/Atualização
```
    # activate the virtual environment
    source mmurp_env/bin/activate

    # install packages you will need
    pip3 install -r setup/requirements.txt

    # save any installed package that will be necessary
    pip3 freeze > requirements.txt
```

### Criando instâncias
```
    python3 src/python/build_instances.py Instancias/test/ Instancias/out_test/ -p 100 -m 100
```

### Rodando modelo para instância
```
    src/python/run_mmurp_model.py Instancias/Vrp-Set-A_MMURP/A-n50-m4-Q80-v8-s100-d2_2_2_2.vrp > out.log
```
#### TODO
* add folder on the input argument to make possible to select the output files
* add parameters to control the models creation
* create script to make possible run multiple instances

---

## To Do
* Mudar função python de leitura de instância para devolver um objeto instância (criar classe para tal...)
* ~~Comandos para acessar bash do docker~~
* ~~Compilando código C++ no docker~~
* Adicionar passo a passo para rodar todas as funções (adicionar documentação de cada uma)
* ~~Criar função para adaptar instancias para o problema MOMMURP a partir das instancias do Cordeau(Vrp-Set-A) (são do artigo dele ???)~~
* Validar que demandas das cidades podem ser atendidas por um único caminhão
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

### Função python para leitura de instância
* Ler problemas variados
    * MMURP
    * MDOVRP
* Retornar um objeto instância
* Criar alguns testes para validar desenvolvimento



## C++ books
* Meyers, Scott - Effective C++ 55 Specific Ways to Improve Your Programs and Designs-Pearson Education Limited (US titles)_Addison Wesley Professional (2005)
