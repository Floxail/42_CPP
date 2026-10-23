# CPP03 — Guide pas à pas : l'héritage

## Commence ici (5 min)

1. Lance `mkdir -p CPP03/ex00 CPP03/ex01 CPP03/ex02 CPP03/ex03`
2. Copie un Makefile d'un module précédent dans `CPP03/ex00/`
3. Lis la section **Notion 1** ci-dessous (10 min), puis attaque ex00

## Le plan : 4 exos, une famille de robots

Tu crées un robot `ClapTrap`, puis des robots « enfants » qui héritent de lui.

| Exo | Ce que tu crées | Notion clé | Temps estimé | Obligatoire |
| --- | --- | --- | --- | --- |
| ex00 | `ClapTrap` | Une classe simple + OCF | ~1 h 30 | Oui |
| ex01 | `ScavTrap` hérite de `ClapTrap` | Héritage, `protected`, ordre de construction | ~1 h 30 | Oui |
| ex02 | `FragTrap` hérite de `ClapTrap` | Même chose, pour t'entraîner | ~45 min | Oui |
| ex03 | `DiamondTrap` hérite de `ScavTrap` ET `FragTrap` | Héritage multiple, héritage `virtual` | 2 à 3 h | Non (bonus) |

Pour démarrer un exo, copie le précédent : `cp ex00/* ex01/`.

**Comment ce guide est écrit :** les exemples de code utilisent un autre thème (des véhicules) pour t'expliquer la notion sans te donner la solution. Le sujet demande que tu raisonnes toi-même, et à l'éval tu dois tout expliquer.

## Les règles du module (rappel)

| Règle | Sanction si non respectée |
| --- | --- |
| Compile avec `c++ -Wall -Wextra -Werror -std=c++98` | Exo refusé |
| `printf`, `malloc`, `free` | 0 |
| Corps de fonction dans un `.hpp` | 0 à l'exo |
| `.hpp` sans include guard | 0 |
| `using namespace`, `friend` | -42 |
| STL (`vector`, `map`, `<algorithm>`) | -42 |
| Chaque classe en forme canonique (4 fonctions) | Exo refusé |

Chaque `.hpp` doit inclure ce dont il a besoin (par exemple `<string>`), pour pouvoir être utilisé seul.

## À lire avant ex00 (15 min)

### Notion 1 : c'est quoi l'héritage ?

**L'idée :** une classe « enfant » récupère tout ce qu'a une classe « parent », et ajoute ou change ce qu'elle veut.

On dit : « une `Voiture` **est un** `Vehicule` ». Si la phrase « X est un Y » est vraie, l'héritage a du sens.

```cpp
class Vehicule {               // la classe parent (ou classe de base)
protected:
	std::string _marque;
	int         _vitesseMax;
public:
	Vehicule(std::string marque);
	void rouler();
};

class Voiture : public Vehicule {   // Voiture hérite de Vehicule
public:
	Voiture(std::string marque);
	void klaxonner();               // une capacité en plus
};
```

Une `Voiture` a automatiquement `_marque`, `_vitesseMax` et `rouler()`. Elle ajoute `klaxonner()`.

**Pourquoi c'est utile :** tu n'écris pas deux fois le même code. Si tu corriges `rouler()` dans `Vehicule`, toutes les classes enfants profitent de la correction.

### Notion 2 : `private`, `protected`, `public`

| Mot-clé | La classe elle-même | Les classes enfants | Le reste du programme |
| --- | --- | --- | --- |
| `private` | Oui | **Non** | Non |
| `protected` | Oui | **Oui** | Non |
| `public` | Oui | Oui | Oui |

**Le piège de l'ex01 :** en ex00, tu mets les attributs de `ClapTrap` en `private`. En ex01, `ScavTrap` doit les modifier (100 points de vie au lieu de 10). Avec `private`, il n'y a pas accès. Le sujet dit « update ClapTrap accordingly » : passe-les en `protected`.

### Notion 3 : `public` dans `class Voiture : public Vehicule`

Ce `public` dit comment les membres hérités sont vus de l'extérieur. Avec `public`, ce qui était public chez le parent reste public chez l'enfant. C'est ce que tu veux dans 99 % des cas, et dans tout ce module.

## ex00 — ClapTrap (~1 h 30)

**Fini quand :** ton main montre une attaque, des dégâts, une réparation, et un robot qui refuse d'agir quand il n'a plus d'énergie ou de vie.

**Fichiers :** `Makefile`, `main.cpp`, `ClapTrap.hpp`, `ClapTrap.cpp`

### Ce que le sujet demande

| Attribut (privé) | Valeur de départ | Rôle |
| --- | --- | --- |
| Nom | Paramètre du constructeur | Identité du robot |
| Points de vie (hit points) | 10 | Sa santé |
| Points d'énergie (energy points) | 10 | Son carburant pour agir |
| Dégâts d'attaque (attack damage) | 0 | Combien il fait perdre |

| Fonction | Ce qu'elle fait | Coût |
| --- | --- | --- |
| `void attack(const std::string& target);` | Affiche qu'il attaque `target` pour X dégâts | 1 énergie |
| `void takeDamage(unsigned int amount);` | Perd `amount` points de vie | Gratuit |
| `void beRepaired(unsigned int amount);` | Regagne `amount` points de vie | 1 énergie |

**Règle centrale :** un robot avec 0 point de vie OU 0 point d'énergie ne peut ni attaquer ni se réparer. Il affiche un message à la place.

**Les robots ne se touchent pas :** `attack("Bob")` affiche juste un message. Ça n'enlève pas de vie à un vrai objet Bob. Si tu veux montrer l'effet, ton main appelle `bob.takeDamage(...)` séparément.

### Étape 1 — `ClapTrap.hpp` (20 min)

- [ ] Include guard + `#include <string>`
- [ ] 4 attributs privés (choisis tes noms, par exemple `_name`, `_hitPoints`, `_energyPoints`, `_attackDamage`)
- [ ] Forme canonique : constructeur par défaut, constructeur de copie, `operator=`, destructeur
- [ ] Un constructeur qui prend le nom : `ClapTrap(const std::string &name);`
- [ ] Les 3 fonctions du sujet

**Pourquoi un constructeur par défaut si le sujet veut un nom ?** La forme canonique l'impose. Donne-lui un nom par défaut, par exemple `"default"`.

**Quel type pour les points ?** `unsigned int` colle aux paramètres du sujet. Attention à la soustraction, voir étape 2.

### Étape 2 — `ClapTrap.cpp` (40 min)

- [ ] Chaque constructeur et le destructeur affichent un message qui contient `ClapTrap`
- [ ] Initialise les attributs dans la **liste d'initialisation** (vu en CPP02)
- [ ] `attack` : vérifie vie et énergie, retire 1 énergie, affiche `ClapTrap <nom> attacks <cible>, causing <dégâts> points of damage!`
- [ ] `takeDamage` : retire des points de vie **sans passer sous 0**
- [ ] `beRepaired` : vérifie vie et énergie, retire 1 énergie, ajoute les points de vie

**Le piège de `takeDamage` : le débordement `unsigned`.**

Un `unsigned int` ne peut pas être négatif. Si tu fais `10 - 15` sur un `unsigned int`, tu n'obtiens pas -5 mais 4 294 967 291 : le nombre « fait le tour ». Ton robot mort aurait 4 milliards de points de vie.

La solution : compare AVANT de soustraire. Si les dégâts dépassent la vie restante, la vie passe à 0.

**Un robot mort peut-il encore prendre des dégâts ?** Le sujet ne dit rien. Choisis, affiche un message clair, et sache justifier ton choix à l'éval.

### Étape 3 — tester (30 min)

Écris un main qui montre chaque cas. Pour chacun, sache dire ce qui doit s'afficher AVANT de lancer le programme.

- [ ] Une attaque normale
- [ ] Des dégâts normaux, puis des dégâts plus grands que la vie restante (la vie doit tomber à 0, pas à 4 milliards)
- [ ] Une réparation
- [ ] 11 actions de suite : la 11e est refusée (plus d'énergie)
- [ ] Un robot à 0 point de vie qui essaie d'attaquer : refusé
- [ ] Une copie (`ClapTrap b(a);`) et une affectation (`c = a;`)

## ex01 — ScavTrap (~1 h 30)

**Fini quand :** ton main affiche la construction ClapTrap puis ScavTrap, et la destruction ScavTrap puis ClapTrap.

**Départ :** `cp ex00/* ex01/`. **Fichiers en plus :** `ScavTrap.hpp`, `ScavTrap.cpp`

### Notion 4 : l'ordre de construction et de destruction

Le sujet te pose la question : « Why? ». Voici la réponse à savoir dire à l'éval.

Une `Voiture` **contient** un `Vehicule` à l'intérieur d'elle. Pense à une maison :

1. **Construction :** on coule les fondations (le parent), puis on construit les murs (l'enfant). L'enfant a besoin que le parent soit prêt : il peut utiliser `_marque` dès son constructeur.
2. **Destruction :** on démolit les murs (l'enfant) avant les fondations (le parent). L'enfant peut encore utiliser les attributs du parent dans son destructeur.

```
Construction :  Vehicule  →  Voiture
Destruction  :  Voiture   →  Vehicule
```

### Notion 5 : appeler le constructeur du parent

Le constructeur de l'enfant choisit QUEL constructeur du parent appeler, dans sa liste d'initialisation :

```cpp
Voiture::Voiture(std::string marque) : Vehicule(marque) {
	_vitesseMax = 180;      // on change la valeur héritée
	std::cout << "Voiture constructor called" << std::endl;
}
```

- `: Vehicule(marque)` : construit la partie parent avec ce constructeur-là.
- Si tu ne l'écris pas, C++ appelle le constructeur **par défaut** du parent. Ton robot s'appellerait alors `"default"` au lieu du nom donné.
- On ne peut pas mettre `_vitesseMax(180)` dans la liste d'initialisation de l'enfant : la liste ne peut initialiser que les attributs de sa propre classe. Les attributs hérités se modifient dans le corps `{ }`.

**Pour la copie, même logique :** le constructeur de copie de l'enfant appelle celui du parent : `Voiture(const Voiture &other) : Vehicule(other)`. Ça marche car une `Voiture` **est un** `Vehicule`.

### Notion 6 : redéfinir une fonction

`ScavTrap` doit avoir son propre `attack()`, avec un message différent. Tu le redéclares dans `ScavTrap.hpp` avec le même prototype, et tu écris une nouvelle version dans `ScavTrap.cpp`.

```cpp
scav.attack("Bob");     // appelle ScavTrap::attack
```

**Attention, un piège que CPP04 expliquera :**

```cpp
ClapTrap *ptr = &scav;
ptr->attack("Bob");     // appelle ClapTrap::attack, pas ScavTrap::attack !
```

Sans le mot-clé `virtual`, C++ choisit la fonction selon le **type du pointeur** (`ClapTrap*`), pas selon l'objet réel. Tu n'en as pas besoin pour réussir CPP03. Si tu veux aller plus loin, ajoute `virtual` devant `attack` et devant `~ClapTrap()` dans `ClapTrap.hpp`, et sache expliquer pourquoi (voir le guide CPP04).

### Étape 1 — modifier ClapTrap (5 min)

- [ ] Passe les 4 attributs de `private` à `protected`

### Étape 2 — `ScavTrap.hpp` et `ScavTrap.cpp` (45 min)

- [ ] `class ScavTrap : public ClapTrap`
- [ ] `#include "ClapTrap.hpp"` dans `ScavTrap.hpp`
- [ ] Forme canonique + constructeur avec nom, chacun appelle le bon constructeur de `ClapTrap`
- [ ] Dans le corps du constructeur : vie 100, énergie 50, dégâts 20
- [ ] Messages différents de ceux de ClapTrap (avec `ScavTrap` dedans)
- [ ] Redéfinis `attack()` avec un message `ScavTrap ...`
- [ ] Ajoute `void guardGate();` : affiche que le ScavTrap passe en mode « Gate keeper »
- [ ] Ajoute `ScavTrap.cpp` dans les `SRCS` du Makefile

### Étape 3 — tester (30 min)

- [ ] Crée un `ScavTrap` : vérifie l'ordre ClapTrap → ScavTrap à la construction
- [ ] Vérifie l'ordre ScavTrap → ClapTrap à la destruction
- [ ] `attack` affiche bien le message ScavTrap, avec 20 dégâts
- [ ] `takeDamage` et `beRepaired` (hérités) marchent sur un ScavTrap
- [ ] `guardGate()`
- [ ] Les tests de l'ex00 sur un ClapTrap marchent toujours

## ex02 — FragTrap (~45 min)

**Fini quand :** un `FragTrap` se construit, se détruit dans le bon ordre et fait son `highFivesGuys()`.

**Départ :** `cp ex01/* ex02/`. **Fichiers en plus :** `FragTrap.hpp`, `FragTrap.cpp`

C'est la même chose que ScavTrap, pour ancrer la notion. Différences :

| | ScavTrap | FragTrap |
| --- | --- | --- |
| Vie | 100 | 100 |
| Énergie | 50 | **100** |
| Dégâts | 20 | **30** |
| Capacité spéciale | `guardGate()` | `highFivesGuys()` : demande un « high five » positif |
| Redéfinit `attack()` | Oui (exigé) | Pas exigé |

- [ ] `class FragTrap : public ClapTrap`, forme canonique, messages avec `FragTrap`
- [ ] Vie 100, énergie 100, dégâts 30
- [ ] `void highFivesGuys(void);`
- [ ] Ajoute `FragTrap.cpp` au Makefile
- [ ] Teste : ordre de construction/destruction, `highFivesGuys()`, une attaque (30 dégâts)

## ex03 — DiamondTrap, bonus (2 à 3 h)

**Fais-le seulement si ex00, ex01 et ex02 marchent.** Le module est validé sans ex03.

**Fini quand :** un `DiamondTrap` affiche UNE seule construction de ClapTrap, et `whoAmI()` affiche ses deux noms.

**Départ :** `cp ex02/* ex03/`. **Fichiers en plus :** `DiamondTrap.hpp`, `DiamondTrap.cpp`

### Notion 7 : l'héritage multiple et le problème du diamant

`DiamondTrap` hérite de `ScavTrap` ET de `FragTrap`. Les deux héritent de `ClapTrap`. Le dessin forme un losange (un diamant) :

```
          ClapTrap
         /        \
   ScavTrap      FragTrap
         \        /
        DiamondTrap
```

**Le problème :** par défaut, `ScavTrap` contient son propre `ClapTrap`, et `FragTrap` aussi. Un `DiamondTrap` contient donc **deux** `ClapTrap` : deux noms, deux vies, deux énergies. Et si tu écris `_hitPoints`, le compilateur ne sait pas lequel tu veux : erreur « ambiguous ».

**La solution : l'héritage `virtual`.**

```cpp
class ScavTrap : virtual public ClapTrap { ... };
class FragTrap : virtual public ClapTrap { ... };
```

`virtual` ici veut dire : « si plusieurs de mes enfants héritent de moi, partagez UN SEUL `ClapTrap` ». C'est le « trick » dont parle le sujet.

### Notion 8 : qui construit le `ClapTrap` partagé ?

Avec l'héritage virtuel, ce n'est plus `ScavTrap` ni `FragTrap` qui construit `ClapTrap`. C'est la classe **la plus dérivée**, ici `DiamondTrap`. Sa liste d'initialisation doit donc appeler `ClapTrap(...)` directement, en plus de `ScavTrap(...)` et `FragTrap(...)`.

Si tu oublies, C++ appelle le constructeur par défaut de `ClapTrap`, et ton nom `<nom>_clap_name` disparaît.

**Ordre de construction pour `class DiamondTrap : public ScavTrap, public FragTrap` :**

1. `ClapTrap` (la base virtuelle, toujours en premier, une seule fois)
2. `ScavTrap` (premier parent dans la liste)
3. `FragTrap` (second parent)
4. `DiamondTrap`

La destruction se fait dans l'ordre exactement inverse.

### Notion 9 : qui gagne pour les valeurs ?

Le sujet veut : vie de FragTrap (100), énergie de ScavTrap (50), dégâts de FragTrap (30).

**Le piège :** il n'y a qu'un seul `_energyPoints`. `ScavTrap` le met à 50, puis `FragTrap` (construit après) le remet à 100. Le dernier qui écrit gagne.

Pour retrouver l'énergie de ScavTrap, deux méthodes :

1. Dans le corps du constructeur de `DiamondTrap`, réécris les 3 valeurs toi-même.
2. Plus propre : stocke les valeurs par défaut de chaque classe dans des constantes `static const` (par exemple dans `ScavTrap` et `FragTrap`) et utilise-les dans `DiamondTrap`. Comme ça, si on change la valeur de ScavTrap, DiamondTrap suit.

### Notion 10 : deux attributs qui portent le même nom

`DiamondTrap` a son propre attribut privé `_name`, avec **exactement** le même nom que celui de `ClapTrap`. Le sujet l'impose.

- Dans `DiamondTrap`, `_name` désigne celui de `DiamondTrap`. Il **cache** (shadow) celui du parent.
- Pour atteindre celui du parent : `ClapTrap::_name`.
- **`-Wshadow`** active des warnings quand un nom en cache un autre. Avec GCC 13 (testé), il signale par exemple une variable locale ou un paramètre qui porte le même nom qu'un attribut (`std::string _name;` dans une méthode de `DiamondTrap`), mais PAS un attribut qui cache celui du parent. Clang a un warning dédié à ce cas-là : `-Wshadow-field`.
- **`-Wno-shadow`** désactive ces warnings.
- Pourquoi le sujet en parle : avec `-Werror`, un warning de shadow casse la compilation. Teste `make` en ajoutant `-Wshadow` à tes flags, lis ce qui s'affiche, et sache expliquer chaque ligne à l'éval.

### Notion 11 : quel `attack()` ?

`ScavTrap` et `FragTrap` peuvent tous les deux avoir un `attack()`. Le sujet veut celui de `ScavTrap`. Deux façons :

- `using ScavTrap::attack;` dans la classe `DiamondTrap` (ce n'est pas `using namespace`, c'est autorisé).
- Ou redéclare `attack()` dans `DiamondTrap` et appelle `ScavTrap::attack(target)` dedans.

### Étapes

- [ ] Ajoute `virtual` devant `public ClapTrap` dans `ScavTrap.hpp` et `FragTrap.hpp`
- [ ] `class DiamondTrap : public ScavTrap, public FragTrap` avec un attribut privé du même nom que celui de ClapTrap
- [ ] Constructeur : appelle `ClapTrap(nom + "_clap_name")`, `ScavTrap(...)`, `FragTrap(...)`, puis met `_name` à `nom`
- [ ] Fixe vie 100, énergie 50, dégâts 30
- [ ] `attack` de ScavTrap
- [ ] `void whoAmI();` affiche le nom du DiamondTrap ET `ClapTrap::_name`
- [ ] Teste : UNE seule ligne « ClapTrap constructor », les bonnes valeurs, `whoAmI()`, `attack()`, `guardGate()`, `highFivesGuys()`

## Avant de rendre (20 min)

### Si ça ne marche pas : les causes fréquentes

| Symptôme | Cause | Correction |
| --- | --- | --- |
| `'_hitPoints' is private within this context` | Attributs de ClapTrap en `private` | Passe-les en `protected` |
| Le robot enfant s'appelle `default` | Le constructeur enfant n'appelle pas `ClapTrap(name)` | Ajoute-le dans la liste d'initialisation |
| La vie devient 4294967291 | Soustraction sur un `unsigned` qui passe sous 0 | Compare avant de soustraire |
| `undefined reference to ScavTrap::...` | Fichier absent du Makefile | Ajoute-le dans `SRCS` |
| (ex03) Deux lignes « ClapTrap constructor » | Héritage non virtuel | `virtual public ClapTrap` dans Scav et Frag |
| (ex03) `request for member is ambiguous` | Même cause | Même correction |
| (ex03) Le nom `_clap_name` n'apparaît pas | DiamondTrap n'appelle pas `ClapTrap(...)` | Appelle-le dans sa liste d'initialisation |

### Checklist par dossier

- [ ] `make re` sans warning, avec `-std=c++98`
- [ ] Chaque classe : 4 fonctions canoniques, messages de constructeur et destructeur différents
- [ ] Aucun corps de fonction dans un `.hpp`, include guard partout
- [ ] Un main avec tes propres tests, qui montre l'ordre de construction et de destruction
- [ ] `make fclean` avant de rendre

### À savoir expliquer à l'éval, sans notes

- [ ] Qu'est-ce que l'héritage, et la phrase « est un »
- [ ] `private` vs `protected` : pourquoi tu as dû changer ClapTrap en ex01
- [ ] Pourquoi le parent est construit avant l'enfant, et détruit après
- [ ] Comment l'enfant choisit le constructeur du parent
- [ ] Pourquoi `unsigned int` peut donner 4 milliards
- [ ] (ex03) Le problème du diamant et ce que fait `virtual public`
- [ ] (ex03) Pourquoi c'est DiamondTrap qui construit ClapTrap
- [ ] (ex03) Ce que fait `-Wshadow`

**Prochaine action :** lance `mkdir -p CPP03/ex00 CPP03/ex01 CPP03/ex02 CPP03/ex03` et ouvre `ex00/ClapTrap.hpp`.
