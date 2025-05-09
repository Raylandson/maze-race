# Corrida de Robôs em Labirintos - AED2 🏆

Bem-vindo ao nosso projeto da disciplina **Algoritmos e Estruturas de Dados 2**! Desenvolvido em equipe por estudantes da faculdade, este é um jogo de **corrida de robôs em labirintos** criado com **Godot Engine** e **C++ (GDExtension)**. Implementamos cinco algoritmos de navegação para robôs — **Modified Flood Fill**, **Trémaux**, **Random**, **DFS** e **WallFollowing** — e os colocamos pra competir em labirintos gerados dinamicamente. 😎

O objetivo? Explorar como diferentes algoritmos resolvem labirintos, comparando eficiência e comportamento, tudo com uma pitada de diversão e trabalho em equipe.

---

## 🎮 Funcionalidades

- **Geração de Labirintos Dinâmicos**: Mapas variados com tamanhos configuráveis.
- **Algoritmos de Navegação**:
  - **Modified Flood Fill**: Otimizado pra encontrar o menor caminho propagando distâncias.
  - **Trémaux**: Marca caminhos ruins e evita becos com eficiência.
  - **Random**: Escolhe direções aleatoriamente, preferindo ir reto até um beco.
  - **DFS (Busca em Profundidade)**: Explora até o fim, garantindo solução completa.
  - **WallFollowing**: Segue as paredes (direita ou esquerda) pra navegar labirintos simples.
- **Visualização**: Cada robô tem uma cor única (ex.: Modified Flood Fill azul, Random amarelo) e se move em tempo real.
- **Código Modular**: Arquitetura orientada a objetos com uma classe base abstrata (`RobotBase`).

---

## 🛠️ Tecnologias

- **Godot Engine**: Motor gráfico para renderização e lógica do jogo.
- **C++ (GDExtension)**: Implementação dos algoritmos e integração com Godot.
- **GDScript**: Scripts auxiliares pra inicializar a cena e configurar robôs.

---

## 🚀 Como Rodar

### Pré-requisitos

- Baixe o **zip da release** para o seu sistema operacional (Windows ou Linux) na seção [Releases](https://github.com/Raylandson/maze-race/releases).
- **Recomendação**: Escolha a versão **Vulkan** para melhor desempenho. A versão **OpenGL** é indicada para computadores mais antigos, mas pode ter performance reduzida.
- Certifique-se de extrair todos os arquivos do zip na mesma pasta.

### Passos

1. **Baixe a Release**:
   - Acesse a aba de [Releases](https://github.com/Raylandson/maze-race/releases) no GitHub.
   - Baixe o arquivo `.zip` correspondente ao seu sistema operacional e API gráfica suportada (ex.: `maze_race_windows_vulkan_v1.0.zip`, `maze_race_linux_opengl_v1.0.zip`).

2. **Extraia os Arquivos**:
   - Descompacte o `.zip` numa pasta de sua escolha.
   - Certifique-se de que todos os arquivos (executável, `.pck`, etc.) estão na mesma pasta.

3. **Execute o Jogo**:
   - **Windows**:
     - Clique duas vezes no executável (ex.: `maze_race.exe`).
   - **Linux**::
     - Clique com o botão direito no arquivo executável, selecione **Propriedades > Permissões**.
     - Marque a opção **"Permitir executar o arquivo como programa"**.
     - Feche as propriedades e dê duplo clique no executável para rodar.
