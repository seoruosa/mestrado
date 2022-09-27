##### TODO
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
