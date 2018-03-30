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
A árvore guarda suas folhas em uma AVL customizada que se arruma pela comparaçao da string de hash.

Não consegui implementar a Merkle tree como AVL, mas consegui usar uma AVL como árvore auxiliar e isso permite que tenhamos Log2(N) em inserção, procura e remoção.

### Nós
Os nós possuem informação de dados e hash, sendo que o primeiro só é preenchido quando o nó é uma folha.
Os nós possuem ponteiro para seus pais e filhos, tornando assim uma fácil manipulação quando não tiver acesso ao vetor de nós.
Os nós também conseguem se validar, verificando todos os hashs a partir deles até a raiz.

### Situação

Casos | Melhor caso ideal | Pior caso ideal | Melhor caso real | Pior caso real |
--- | --- | --- | --- |--- |
*Inserção* | 1 | Log2(N) | 1 | Log2(N) |
*Remoção* | 1 | Log2(N) | 1 | Log2(N) |
*Procura* | 1 | Log2(N) | 1 | Log2(N) |
**Syncronização** | Log2(N) | N | Log2(N) | N |
**Criação da árvore** | N | N | N | N |
**Validação da árvore** | N | N | N | N |
**Validação do nó** | N | N | N | N |
**Espaço** | N | N | N | N |

### Licença
 MIT License 
