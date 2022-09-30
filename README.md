# Mestrado sobre o problema de roteamento de mamógrafos móveis (Mobile Mammography Unit Routing Problem)



## Comandos
Para rodar o jupyter notebook com o solver Gurobi faça:

```
docker-compose -f roteamento-carretas-da-mamografia/docker-compose_optimizer-python-mip.yml up
```

Para abrir um bash em um terminal faça:

```
docker exec -it gurobi-python-mip /bin/bash
/opt/project#
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
### Rodando remotamente
#### Docker
```
    # rodando o docker-compose em "detached mode"
    sudo docker-compose -f docker-compose_optimizer-python-mip.yml up -d

    # Vendo o log para saber o endereço, por exemplo
    sudo docker-compose -f docker-compose_optimizer-python-mip.yml logs

    # Observando os containers que estão rodando
    sudo docker ps

    # parando o container(sem excluí-lo)
    sudo docker-compose -f docker-compose_optimizer-python-mip.yml stop
    sudo docker gurobi-python-mip stop

    # abrindo um console
    docker exec -it gurobi-python-mip /bin/bash
    cd /opt/project

```

#### Rodando Experimentos
```
    # connect remotly to the lab pc
    ssh user@pc_lab

    # goes to mestrado folder and run the docker on detached mode
    sudo docker-compose -f docker-compose_optimizer-python-mip.yml up -d

    # open bash of the docker instance
    sudo docker exec -it gurobi-python-mip /bin/bash
    cd /opt/project/

    # run the experiment
    nohup ./run_tests.sh &>resultados/bash_test.log &
```
#### Refs
* differences between stop/down ([link](https://stackoverflow.com/questions/55282552/difference-between-docker-compose-down-and-ctrl-c))

### Rodando análises localmente

#### Instalação
```
    source setup.sh
```

#### Ativação/Atualização
```
    # activate the virtual environment
    source mmurp_env/bin/activate

    # install packages you will need
    pip3 install -r setup/requirements.txt

    # save any installed package that will be necessary
    pip3 freeze > requirements.txt
```

#### Criando instâncias
```
    python3 src/python/build_instances.py Instancias/test/ Instancias/out_test/ -p 100 -m 100
```

#### Rodando modelo para instância
```
    src/python/run_mmurp_model.py Instancias/Vrp-Set-A_MMURP/A-n50-m4-Q80-v8-s100-d2_2_2_2.vrp > out.log
```

## C++ books
* Meyers, Scott - Effective C++ 55 Specific Ways to Improve Your Programs and Designs-Pearson Education Limited (US titles)_Addison Wesley Professional (2005)
