LatticeGas
==========

Resultado do projeto de Métodos Computacionais Para a Física. Uma versão paralela pode ser encontrada aqui: https://github.com/LucasCampos/LatticeGasCUDA

==================REQUISITOS==================

Este programa requer o libglfw instalado. No Ubuntu, pode-se fazer

	sudo apt-get install libglfw-dev

==================COMPILAÇÃO==================

Para compilar este programa, é necessário usar somente

	make

Também existe a opção de compilar e rodar o programa

	make run

==================MÉTRICA=====================

Velocidade de iteração, em um processador i7-3612QM, sem visualização

|L   | ms/ite |
|----|--------|
|192 | 0.2397 |
|384 | 1.8964 |
|576 | 4.2256 |
|768 | 7.3948 |
