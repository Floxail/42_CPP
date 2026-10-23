# CPP04 — Guide pas à pas : polymorphisme, classes abstraites, interfaces

## Commence ici (5 min)

1. Lance `mkdir -p CPP04/ex00 CPP04/ex01 CPP04/ex02 CPP04/ex03`
2. Vérifie que tu as `valgrind` : `valgrind --version` (sinon `sudo apt install valgrind`)
3. Lis **Notion 1** et **Notion 2** ci-dessous (15 min), puis attaque ex00

## Le plan : 4 exos, 2 projets

| Exo | Ce que tu crées | Notion clé | Temps estimé | Obligatoire |
| --- | --- | --- | --- | --- |
| ex00 | `Animal`, `Dog`, `Cat`, `WrongAnimal`, `WrongCat` | `virtual`, polymorphisme | ~2 h | Oui |
| ex01 | `Brain` dans Dog et Cat | Copie profonde, `new`/`delete`, destructeur virtuel | ~2 h 30 | Oui |
| ex02 | `Animal` devient non instanciable | Classe abstraite (`= 0`) | ~30 min | Oui |
| ex03 | `AMateria`, `Ice`, `Cure`, `Character`, `MateriaSource` | Interfaces, `clone()`, gestion de la mémoire | 4 à 6 h | Non (bonus) |

ex00 → ex01 → ex02 forment un seul projet qui grandit : copie le dossier précédent à chaque fois. ex03 repart de zéro.

**Comment ce guide est écrit :** les exemples de code utilisent un autre thème (des formes géométriques, des instruments) pour t'expliquer la notion sans te donner la solution. Le sujet demande que tu raisonnes toi-même, et à l'éval tu dois tout expliquer.

## Les règles du module (rappel)

| Règle | Sanction si non respectée |
| --- | --- |
| Compile avec `c++ -Wall -Wextra -Werror -std=c++98` | Exo refusé |
| `printf`, `malloc`, `free` | 0 |
| Corps de fonction dans un `.hpp` | 0 à l'exo |
| `.hpp` sans include guard | 0 |
| `using namespace`, `friend` | -42 |
| STL (`vector`, `map`, `<algorithm>`) | -42 |
| Fuite mémoire | Exo refusé |
| Chaque classe en forme canonique (4 fonctions) | Exo refusé |

**Nouveau dans ce module : les fuites mémoire comptent.** Chaque `new` doit avoir son `delete`. Vérifie avec `valgrind --leak-check=full ./ton_programme`. La ligne à lire : `All heap blocks were freed -- no leaks are possible`.

## À lire avant ex00 (15 min)

### Notion 1 : le type du pointeur vs le type de l'objet

En CPP03 tu as vu qu'une classe enfant **est un** parent. Donc un pointeur vers le parent peut pointer vers un enfant :

```cpp
Forme *f = new Cercle();    // un Cercle EST UNE Forme : autorisé
```

Il y a maintenant deux types en jeu :

| | Quoi | Ici |
| --- | --- | --- |
| Type **statique** | Le type écrit dans le code, celui du pointeur | `Forme*` |
| Type **dynamique** | Le type réel de l'objet en mémoire | `Cercle` |

**La question de tout le module :** quand j'appelle `f->dessiner()`, C++ appelle-t-il `Forme::dessiner` ou `Cercle::dessiner` ?

### Notion 2 : `virtual` change la réponse

**Sans `virtual`** : C++ regarde le type du **pointeur**. `f->dessiner()` appelle `Forme::dessiner`. Le choix est fait à la compilation.

**Avec `virtual`** : C++ regarde le type de l'**objet réel**. `f->dessiner()` appelle `Cercle::dessiner`. Le choix est fait pendant l'exécution.

```cpp
class Forme {
public:
	virtual void dessiner() const;   // virtual : la version de l'enfant sera utilisée
	virtual ~Forme();                // destructeur virtuel, voir ex01
};

class Cercle : public Forme {
public:
	void dessiner() const;           // redéfinit (override) celle de Forme
};
```

C'est ça, le **polymorphisme** : un seul code (`f->dessiner()`) qui se comporte différemment selon l'objet réel. Tu peux mettre des cercles, des carrés et des triangles dans un tableau de `Forme*` et tous les dessiner avec une boucle.

**Comment C++ fait (pour l'éval) :** chaque classe qui a des fonctions `virtual` reçoit une table cachée, la **vtable**, qui liste ses versions des fonctions. Chaque objet garde un pointeur caché vers la vtable de sa vraie classe. À l'appel, C++ suit ce pointeur pour trouver la bonne fonction.

**Règles pratiques :**

- `virtual` se met dans la classe **parent**. Dans l'enfant, la fonction reste virtuelle automatiquement (tu peux répéter `virtual` pour la lisibilité).
- Le prototype doit être **identique**, `const` compris. `void dessiner()` chez l'enfant ne redéfinit PAS `void dessiner() const` du parent : c'est une autre fonction.
- `override` n'existe pas en C++98. Pas le droit de l'utiliser.

## ex00 — Polymorphisme (~2 h)

**Fini quand :** le main du sujet affiche le cri du chat pour `i`, le cri du chien pour `j`, et le WrongCat affiche le cri du WrongAnimal.

**Fichiers :** `Makefile`, `main.cpp`, et un `.hpp` + `.cpp` par classe (5 classes)

### Ce que le sujet demande

| Classe | Hérite de | `type` vaut | `makeSound()` |
| --- | --- | --- | --- |
| `Animal` | — | Vide ou ce que tu veux | Un son générique |
| `Dog` | `Animal` | `"Dog"` | Un aboiement |
| `Cat` | `Animal` | `"Cat"` | Un miaulement |
| `WrongAnimal` | — | Ce que tu veux | Un son générique |
| `WrongCat` | `WrongAnimal` | `"WrongCat"` | Un miaulement… qui ne sera jamais affiché via un `WrongAnimal*` |

- `type` est un attribut **`protected`** de type `std::string` (pour que Dog et Cat puissent le remplir).
- Chaque constructeur et destructeur affiche un message **différent** par classe.

### Lis le main du sujet comme un indice

```cpp
const Animal* j = new Dog();
j->makeSound();
std::cout << j->getType() << std::endl;
```

Trois indices cachés :

1. `const Animal*` → `makeSound()` et `getType()` doivent être des fonctions **`const`**, sinon ça ne compile pas.
2. `j->makeSound()` doit aboyer → `makeSound` doit être **`virtual`** dans `Animal`.
3. `getType()` existe → il faut un getter qui retourne `type`. Il n'a pas besoin d'être virtual : il lit simplement l'attribut.

### À quoi sert WrongAnimal ?

C'est la **preuve par l'erreur**. Tu écris `WrongAnimal` exactement comme `Animal`, mais **sans** `virtual` sur `makeSound`. Résultat :

```cpp
const WrongAnimal* w = new WrongCat();
w->makeSound();     // affiche le son de WrongAnimal, pas le miaulement
```

À l'éval, tu montres les deux côte à côte et tu expliques : la seule différence est le mot `virtual`.

### Étapes

- [ ] `Animal` : forme canonique, `protected: std::string type;`, `virtual void makeSound() const;`, `std::string getType() const;`
- [ ] Destructeur de `Animal` en `virtual` (explication complète en ex01, mets-le dès maintenant)
- [ ] `Dog` et `Cat` : héritent de `Animal`, mettent `type` dans leur constructeur, redéfinissent `makeSound() const`
- [ ] `WrongAnimal` et `WrongCat` : la même chose **sans** `virtual` sur `makeSound`
- [ ] Le constructeur de copie de chaque enfant appelle celui du parent (`Dog(const Dog &o) : Animal(o)`)
- [ ] Main : le code du sujet + des `delete` pour chaque `new` + tes propres tests
- [ ] `valgrind` : aucune fuite

**Tests à ajouter :** un `Dog` créé sur la pile (`Dog d; d.makeSound();`), une copie de `Cat`, un `WrongCat` utilisé directement (`WrongCat wc; wc.makeSound();` → ici il miaule, car le type statique est `WrongCat`). Sache expliquer pourquoi ce dernier miaule alors que via `WrongAnimal*` il ne miaule pas.

## ex01 — Brain et copie profonde (~2 h 30)

**Fini quand :** ton tableau d'animaux est détruit sans fuite, avec les destructeurs dans le bon ordre, et un test prouve que la copie d'un Dog a son propre Brain.

**Départ :** `cp ex00/* ex01/`. **Fichiers en plus :** `Brain.hpp`, `Brain.cpp`

### Ce que le sujet demande

- `Brain` contient `std::string ideas[100];`
- `Dog` et `Cat` ont un attribut **privé** `Brain*`
- Constructeur de Dog/Cat : `new Brain()`. Destructeur : `delete` du Brain.
- Main : un tableau d'`Animal*`, moitié Dog, moitié Cat. À la fin, une boucle fait `delete` sur chaque élément, **en tant qu'Animal**.
- Les copies de Dog et Cat doivent être **profondes**.

### Notion 3 : pourquoi le destructeur DOIT être virtuel

```cpp
Animal *a = new Dog();
delete a;
```

**Sans `virtual ~Animal()`** : `delete` regarde le type du pointeur (`Animal*`). Seul `~Animal()` est appelé. `~Dog()` n'est jamais appelé → le `Brain` du Dog n'est jamais supprimé → **fuite mémoire**. (Techniquement, c'est même un comportement indéfini.)

**Avec `virtual ~Animal()`** : `delete` regarde l'objet réel. Appelle `~Dog()` (qui supprime le Brain), puis `~Animal()` automatiquement.

**Règle à retenir pour toute ta vie de dev C++ :** une classe faite pour être héritée a un destructeur `virtual`.

### Notion 4 : copie superficielle vs copie profonde

Ton Dog contient un **pointeur** vers un Brain. Que se passe-t-il quand on copie un Dog ?

**Copie superficielle (shallow)** : on copie le pointeur. Les deux Dog pointent vers le **même** Brain.

```
dog1 ──► [Brain #1] ◄── dog2
```

Problèmes :

1. Modifier les idées de `dog2` modifie celles de `dog1`.
2. Quand `dog2` est détruit, il supprime le Brain. `dog1` pointe vers de la mémoire libérée.
3. Quand `dog1` est détruit, il supprime le même Brain une 2e fois → **double free**, crash.

**Copie profonde (deep)** : on crée un **nouveau** Brain, copie du premier.

```
dog1 ──► [Brain #1]
dog2 ──► [Brain #2]   (mêmes idées, autre adresse)
```

**Attention :** le constructeur de copie et l'`operator=` générés automatiquement par C++ font une copie **superficielle**. C'est exactement pour ça que 42 impose la forme canonique : tu dois les écrire toi-même dès qu'il y a un pointeur.

### Notion 5 : copie profonde dans le constructeur de copie vs `operator=`

Exemple avec un `Piano` qui possède un `Partition*` :

```cpp
// Constructeur de copie : l'objet naît, il n'a encore rien.
Piano::Piano(const Piano &other) : Instrument(other) {
	_partition = new Partition(*other._partition);   // nouvelle copie
}

// Affectation : l'objet existe DÉJÀ et possède déjà une Partition.
Piano &Piano::operator=(const Piano &other) {
	if (this != &other) {
		Instrument::operator=(other);                 // copie la partie parent
		delete _partition;                            // 1. libère l'ancienne
		_partition = new Partition(*other._partition);// 2. copie la nouvelle
	}
	return *this;
}
```

- `new Partition(*other._partition)` utilise le **constructeur de copie de `Partition`**. Donc `Brain` doit aussi avoir un vrai constructeur de copie (qui copie les 100 idées une par une).
- Dans `operator=`, oublier le `delete` = fuite. Oublier le `if (this != &other)` = on supprime son propre Brain avant de le recopier depuis… lui-même.
- `Instrument::operator=(other)` : sinon la partie parent (le `type`) n'est pas copiée.

### Notion 6 : comment PROUVER que la copie est profonde

Le sujet l'exige dans tes tests. Idée de test :

1. Ajoute à `Brain` un moyen d'écrire et de lire une idée (par exemple `setIdea(int i, std::string idée)` et `getIdea(int i)`), et à `Dog` un moyen d'y accéder.
2. Crée `dog1`, mets l'idée 0 à « os ».
3. Copie : `Dog dog2(dog1);`
4. Change l'idée 0 de `dog2` en « balle ».
5. Affiche l'idée 0 de `dog1` : elle doit toujours être « os ».
6. Affiche les adresses des deux Brain : elles doivent être différentes.

Fais le même test avec `operator=` (`Dog dog3; dog3 = dog1;`).

**Test bonus classique :** copie un Dog dans un bloc `{ }`. À la sortie du bloc, la copie est détruite. Si le programme ne plante pas en détruisant l'original ensuite, la copie était profonde.

### Notion 7 : le tableau d'animaux

« Un tableau d'objets `Animal` » veut dire en pratique un tableau de **pointeurs** `Animal*` : un tableau de `Animal` tout court ne pourrait contenir ni Dog ni Cat (ils seraient « coupés » en Animal, c'est le *slicing*).

- [ ] Déclare un tableau de taille fixe, par exemple 10 `Animal*`
- [ ] Remplis la 1re moitié avec `new Dog()`, la 2e avec `new Cat()`
- [ ] À la fin, une boucle : `delete` sur chaque case
- [ ] Vérifie l'ordre affiché : `Dog destructor` → `Brain destructor` → `Animal destructor` (selon où tu supprimes le Brain, Brain peut apparaître avant ou après le message de Dog : sache expliquer ton ordre)

### Étapes

- [ ] `Brain` : forme canonique, `std::string ideas[100];`, messages de constructeur et destructeur
- [ ] Constructeur de copie et `operator=` de `Brain` : copient les 100 idées avec une boucle
- [ ] `Dog` / `Cat` : `private: Brain *_brain;`, `new Brain()` dans **chaque** constructeur, `delete` dans le destructeur
- [ ] Constructeur de copie et `operator=` de `Dog` / `Cat` : copie profonde (Notion 5)
- [ ] `~Animal()` est bien `virtual`
- [ ] Main : le code du sujet, le tableau, les tests de copie profonde
- [ ] `valgrind --leak-check=full ./prog` : aucune fuite, aucune erreur

## ex02 — Classe abstraite (~30 min)

**Fini quand :** `new Animal()` (ou `Animal a;`) ne compile plus, et tout le reste marche comme en ex01.

**Départ :** `cp ex01/* ex02/`

### Notion 8 : fonction virtuelle pure et classe abstraite

Un « Animal » tout court n'existe pas dans la vraie vie : c'est toujours un chien, un chat… Créer un objet `Animal` n'a pas de sens. On veut que le compilateur l'interdise.

```cpp
class Forme {
public:
	virtual void dessiner() const = 0;   // "= 0" : fonction virtuelle PURE
	virtual ~Forme();
};
```

- `= 0` veut dire : « je n'ai pas de version, chaque enfant DOIT écrire la sienne ».
- Une classe qui a au moins une fonction virtuelle pure est une **classe abstraite**.
- On ne peut pas créer d'objet d'une classe abstraite : `Forme f;` et `new Forme()` donnent une erreur de compilation.
- On PEUT avoir des pointeurs et des références vers elle : `Forme *f = new Cercle();` marche toujours.
- Un enfant qui n'écrit pas toutes les fonctions pures est lui-même abstrait.

### Étapes

- [ ] Rends `makeSound()` pure dans `Animal` : `virtual void makeSound() const = 0;`
- [ ] Supprime `Animal::makeSound` du `.cpp` (une fonction pure n'a pas besoin de corps)
- [ ] Optionnel : renomme `Animal` en `AAnimal` (« A » pour Abstract). Pense à renommer partout et les fichiers
- [ ] Vérifie que `Animal a;` ne compile plus (teste-le, puis retire la ligne)
- [ ] Les tests de l'ex01 marchent toujours, `valgrind` sans fuite

**Question d'éval :** « Une classe abstraite peut-elle avoir un constructeur ? » Oui. On ne peut pas la créer seule, mais quand on crée un `Dog`, la partie `Animal` est construite en premier, avec son constructeur.

## ex03 — Interfaces et Materia, bonus (4 à 6 h)

**Fais-le seulement si ex00, ex01 et ex02 marchent.** Le module est validé sans ex03. C'est l'exo le plus long du module : prévois 2 sessions.

**Fini quand :** le main du sujet affiche exactement les 2 lignes attendues, et `valgrind` ne trouve aucune fuite.

**Fichiers :** `Makefile`, `main.cpp`, et un `.hpp` + `.cpp` par classe : `AMateria`, `Ice`, `Cure`, `ICharacter` (`.hpp` seul suffit), `Character`, `IMateriaSource` (`.hpp` seul), `MateriaSource`

### Notion 9 : une interface

Une **interface** est une classe abstraite qui n'a **que** des fonctions virtuelles pures (et un destructeur virtuel). Pas d'attributs, pas de code. Elle dit « voilà ce que tu dois savoir faire », sans dire comment.

Le C++ n'a pas de mot-clé `interface` (contrairement à Java). Par convention, on préfixe leur nom par `I` : `ICharacter`, `IMateriaSource`.

**Pourquoi c'est utile :** le reste du code ne dépend que de l'interface. Le main manipule des `ICharacter*` et ne sait pas que derrière il y a un `Character`. Tu pourrais créer un `Monster` qui implémente `ICharacter`, et le main marcherait sans changer une ligne.

### Le projet en une image

```
IMateriaSource (interface)            ICharacter (interface)
       ▲                                     ▲
       │ implémente                          │ implémente
 MateriaSource                           Character
 ├── apprend jusqu'à 4 modèles          ├── un nom
 └── createMateria("ice") → clone       └── inventaire de 4 AMateria*
                                               │ use(idx, cible)
                                               ▼
                                     AMateria (classe abstraite)
                                      ├── Ice  : "* shoots an ice bolt at <nom> *"
                                      └── Cure : "* heals <nom>'s wounds *"
```

### Notion 10 : `clone()`, ou copier sans connaître le type

`Character` a un inventaire de `AMateria*`. Pour le copier en profondeur, il faut créer une copie de chaque Materia. Mais `new AMateria(...)` est impossible (abstraite), et `Character` ne sait pas si c'est un Ice ou un Cure.

La solution : demander à l'objet de se copier lui-même.

```cpp
class Forme {
public:
	virtual Forme *clone() const = 0;
};

Forme *Cercle::clone() const {
	return new Cercle(*this);    // "je crée une copie de moi-même"
}
```

Appeler `f->clone()` sur une `Forme*` qui est en réalité un Cercle donne un nouveau Cercle. C'est le patron de conception **Prototype**. Tu t'en sers à 3 endroits : la copie de `Character`, `learnMateria` et `createMateria`.

### Notion 11 : qui est propriétaire de quoi ?

C'est LA difficulté de l'exo. Chaque `new` doit avoir **un seul** responsable qui fera le `delete`.

| Action | Qui possède la Materia après ? | À faire |
| --- | --- | --- |
| `createMateria("ice")` | L'appelant (le main, dans `tmp`) | Retourne un `clone()` |
| `equip(tmp)` | Le `Character` | Il la supprimera dans son destructeur |
| `equip` avec inventaire plein | Toujours l'appelant | Rien ne se passe, l'appelant doit la supprimer |
| `unequip(idx)` | **Personne** par défaut | Interdit de faire `delete` ! Garde-la quelque part (« le sol ») |
| `learnMateria(new Ice())` | Le `MateriaSource` | Garde-la (ou clone-la et supprime l'original), et supprime-la dans le destructeur |
| Destruction de `Character` | — | `delete` des Materias de l'inventaire (+ du sol si tu le gères dans Character) |

**Le « sol » de `unequip` :** le sujet te laisse choisir. Options courantes :

1. Un tableau fixe de pointeurs « au sol » dans `Character`, supprimés dans son destructeur.
2. Le main garde l'adresse avant `unequip` et fait le `delete` lui-même.

Choisis-en une, et sache l'expliquer.

### Notion 12 : les inclusions circulaires

`AMateria::use` prend un `ICharacter&`. `ICharacter::equip` prend un `AMateria*`. Si chaque `.hpp` inclut l'autre, l'include guard coupe la boucle et une des classes est inconnue au moment où l'autre la lit : erreur de compilation.

La solution : une **déclaration anticipée** (forward declaration).

```cpp
class ICharacter;    // "cette classe existe, tu verras sa définition plus tard"

class AMateria {
	...
	virtual void use(ICharacter &target);
};
```

Ça suffit tant que tu n'utilises que des pointeurs ou des références vers `ICharacter` dans le `.hpp`. Dans `AMateria.cpp`, tu incluras le vrai `ICharacter.hpp`.

### Étapes

**Bloc 1 — AMateria, Ice, Cure (1 h 30)**

- [ ] `AMateria` : `protected: std::string type;`, le constructeur `AMateria(std::string const &type)`, `getType()`, `clone() = 0`, `use()` virtuel (peut ne rien faire), destructeur **virtuel**
- [ ] `Ice` et `Cure` : type `"ice"` et `"cure"` (minuscules), `clone()` retourne `new Ice(*this)` / `new Cure(*this)`, `use()` affiche le message exact du sujet
- [ ] `operator=` de AMateria : le sujet dit que copier le type n'a pas de sens (un Ice reste un Ice). Ne copie donc pas `type`
- [ ] Forward declaration de `ICharacter` dans `AMateria.hpp`

**Bloc 2 — ICharacter et Character (2 h)**

- [ ] `ICharacter.hpp` : copie l'interface du sujet telle quelle (le `{}` du destructeur dans le `.hpp` est donné par le sujet)
- [ ] `Character` : un nom, `AMateria *_inventory[4];`, toutes les cases à `NULL` (ou `0`) au départ
- [ ] `equip` : met la Materia dans la 1re case vide (0 → 3). Plein ou `NULL` → rien ne se passe
- [ ] `unequip` : vide la case **sans** `delete`. Index invalide ou case vide → rien
- [ ] `use(idx, target)` : appelle `_inventory[idx]->use(target)`. Index invalide ou case vide → rien
- [ ] Constructeur de copie et `operator=` : **profonds**. Supprime d'abord les Materias actuelles, puis `clone()` celles de l'autre
- [ ] Destructeur : `delete` toutes les Materias de l'inventaire

**Bloc 3 — IMateriaSource et MateriaSource (1 h)**

- [ ] `IMateriaSource.hpp` : copie l'interface du sujet
- [ ] `MateriaSource` : `AMateria *_templates[4];`, cases à `NULL` au départ
- [ ] `learnMateria(m)` : range `m` dans la 1re case vide. Plein → supprime `m` pour éviter une fuite (et affiche un message)
- [ ] `createMateria(type)` : cherche un modèle avec ce type, retourne son `clone()`. Inconnu → retourne `0`
- [ ] Copie profonde et destructeur, comme Character

**Bloc 4 — tester (1 h)**

- [ ] Le main du sujet → exactement :

```
* shoots an ice bolt at bob *
* heals bob's wounds *
```

- [ ] `./prog | cat -e` : chaque ligne finit par `$`
- [ ] `equip` 5 Materias : la 5e est refusée (et pas de fuite)
- [ ] `unequip` puis `use` sur la case vide : rien ne se passe
- [ ] `use(-1, ...)` et `use(4, ...)` : rien ne se passe, pas de crash
- [ ] `createMateria("fire")` retourne `0`, et `equip(0)` ne plante pas
- [ ] Copie d'un Character, puis destruction de l'original : la copie fonctionne toujours (copie profonde)
- [ ] `valgrind --leak-check=full ./prog` : aucune fuite

## Avant de rendre (20 min)

### Si ça ne marche pas : les causes fréquentes

| Symptôme | Cause | Correction |
| --- | --- | --- |
| Le Dog fait le son de l'Animal | `makeSound` pas `virtual` dans Animal, ou prototype différent (`const` manquant) | `virtual` + prototype identique |
| Erreur avec `const Animal*` | `makeSound()` ou `getType()` sans `const` | Ajoute `const` à la fin |
| Fuite de 100 strings par animal | `~Animal()` pas `virtual` | `virtual ~Animal();` |
| `double free` ou crash à la fin | Copie superficielle du Brain | Copie profonde (Notion 5) |
| Fuite après `dog3 = dog1` | `operator=` ne supprime pas l'ancien Brain | `delete` avant le `new` |
| `invalid use of incomplete type` | `.hpp` avec forward declaration mais `.cpp` sans le vrai include | Inclus le vrai `.hpp` dans le `.cpp` |
| `cannot allocate an object of abstract type` | Normal en ex02 sur `Animal` ; ailleurs, une fonction pure oubliée chez l'enfant | Écris la fonction manquante dans l'enfant |

### Checklist par dossier

- [ ] `make re` sans warning, avec `-std=c++98`
- [ ] Chaque classe : 4 fonctions canoniques, messages de constructeur et destructeur différents par classe
- [ ] Aucun corps de fonction dans un `.hpp` (sauf les interfaces données par le sujet), include guard partout
- [ ] Tes propres tests en plus de ceux du sujet
- [ ] `valgrind --leak-check=full` : aucune fuite
- [ ] `make fclean` avant de rendre

### À savoir expliquer à l'éval, sans notes

- [ ] Type statique vs type dynamique
- [ ] Ce que change `virtual`, et la vtable en une phrase
- [ ] Pourquoi WrongCat fait le son de WrongAnimal via un `WrongAnimal*`
- [ ] Pourquoi le destructeur de la classe de base doit être `virtual`
- [ ] Copie superficielle vs profonde, et le double free
- [ ] Pourquoi `operator=` fait `delete` avant `new`
- [ ] Fonction virtuelle pure, classe abstraite, interface : les différences
- [ ] (ex03) À quoi sert `clone()`
- [ ] (ex03) Pourquoi `unequip` ne doit pas faire `delete`, et ce que tu fais des Materias au sol

**Prochaine action :** lance `mkdir -p CPP04/ex00 CPP04/ex01 CPP04/ex02 CPP04/ex03` et ouvre `ex00/Animal.hpp`.
