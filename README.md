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
g++-10 -g fibo.cpp -o fibo -std=c++2a
```

Rodando um arquivo `.py` com um teste simples usando o MIP no bash
```
python3 modelo_teste.py
```

_Para utilizar o gurobi é necessário adicionar o arquivo `gurobi.lic` com a licença de uso._

--- 

## To Do

* ~~Comandos para acessar bash do docker~~
* ~~Compilando código C++ no docker~~
* Configurar vscode para buildar/compilar cpp
* ~~Mudar do notebook para um arquivo .py~~
* Criar hello world no cmake
* Funções:
  * Testar se uma solução é factível
  * Quantidade de veículos usados
  * Distância percorrida por todos os veículos
