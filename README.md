# MerkleTree
Implementação em c++ da MerkleTree

### HASH
Para os hashs da árvore, usei a implementação de SHA512 da biblioteca Crypto++(5.6.1-9): https://cryptopp.com/

Os Hashs são calculados com Double SHA512, que são duas aplicações da função hash SHA512 segundo as seguintes condições:
  - Se o nó for uma folha, o hash será a aplicação do Double SHA512 no valor do dado;
  - Se o nó não for uma folha e possuir os filhos da esquerda e da direita aplicaremos o Double SHA512 no dado `hash_esquerda || hash_direita`;
  - Se o nó só possuir apenas o filho da esquerda aplicaremos o Double SHA512 no dado `hash_esquerda || hash_esquerda`;
  - Se o nó só possuir apenas o filho da esquerda aplicaremos o Double SHA512 no dado `hash_direita || hash_direita`.
  
OBS: Entenda || como concatenação, e lembre-se de que ela é realizada na ordem que foi informada acima.

### Árvore
A árvore guarda seus nós em um vector<MerkleTreeNode*> ordenada por níveis. Sempre tentando manter as folhas no mesmo nível, no máximo ficam também no nível acima.
Quando a árvore não possui todos as folhas em utilização fica com muitos nós vazios ocupados. Como o número de folhas é base dois, dependendo do número de dados que queremos inserir na árvore, teremos ocupado um maior espaço do que necessário.
A árvore possui funçes de validação e sincronização, garantindo um grande possibilidade de aplicação.

Acredito que a única forma que conseguir log(N) para inserção, remoção e procura seja com uma AVL, porém ainda não encontrei uma maneira de implementar a MerkleTree como AVL.

### Nós
Os nós possuem informação de dados e hash, sendo que o primeiro só é preenchido quando o nó é uma folha.
Os nós possuem ponteiro para seus pais e filhos, tornando assim uma fácil manipulação quando não tiver acesso ao vetor de nós.
Os nós também conseguem se validar, verificando todos os hashs a partir deles até a raiz.

### Situação

Casos | Melhor caso ideal | Pior caso ideal | Melhor caso real | Pior caso real |
--- | --- | --- | --- |--- |
*Inserção* | Log2(N) | Log2(N) | Log2(N) | N + Log2(N) |
*Remoção* | Log2(N) | Log2(N) | Log2(N) | N + Log2(N) |
*Procura* | Log2(N) | Log2(N) | 1 | N |
**Syncronização** | Log2(N) | N | Log2(N) | N |
**Criação da árvore** | N | N | N | N |
**Validação da árvore** | N | N | N | N |
**Validação do nó** | Log2(N) | Log2(N) | Log2(N) | Log2(N) |
**Espaço** | N | N | N | N |

### Licença
 MIT License 
