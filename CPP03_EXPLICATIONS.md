# CPP03 — L'héritage expliqué (préparation à l'éval)

23/10/2026

## Démarrer ici

**Action tout de suite (2 min) :** lance `cd CPP03/ex01 && make && ./scavtrap`, puis lis la section [ex01](#ex01--scavtrap) en regardant la sortie à côté.

**Le module en 5 lignes :**

1. ex00 : tu crées un robot `ClapTrap` (une classe simple, en forme canonique).
2. ex01 : tu crées `ScavTrap`, un robot **enfant** de `ClapTrap`. Il récupère tout ce que `ClapTrap` sait faire, et ajoute des choses.
3. ex02 : tu crées `FragTrap`, un deuxième enfant de `ClapTrap`. Même principe, pour t'entraîner.
4. ex03 (bonus) : `DiamondTrap`, enfant de `ScavTrap` ET de `FragTrap`. Absent de ton dépôt.
5. Le seul vrai sujet du module : **l'héritage**.

**État de ton code au 23/10/2026 :**

| Exo | Compile | Sortie conforme | À faire |
| --- | --- | --- | --- |
| ex00 | Oui | Oui | Rien |
| ex01 | Oui | Oui | Facultatif : ajouter le nom dans le message du constructeur |
| ex02 | Oui | Oui | Rien |
| ex03 | Absent | — | Bonus, facultatif |

**Comment lire ce doc :**

- Section 1 = les notions des anciens modules (révision rapide).
- Sections 2 à 4 = un exo chacune, avec ton code.
- Section 5 = les questions de l'éval avec la réponse courte. Si tu as peu de temps, lis seulement celle-là.
- Le [lexique](#lexique--tous-les-mots-en-une-ligne) à la fin donne chaque mot en une phrase.

Lis une section, ferme, reviens plus tard.

## Sommaire

1. [Révision des anciens modules](#1-révision-des-anciens-modules)
2. [ex00 — ClapTrap](#ex00--claptrap)
3. [ex01 — ScavTrap](#ex01--scavtrap)
4. [ex02 — FragTrap](#ex02--fragtrap)
5. [Avant l'éval : questions, pièges, checklist](#5-avant-léval--questions-pièges-checklist)
6. [Lexique](#lexique--tous-les-mots-en-une-ligne)

---

## 1. Révision des anciens modules

Tu réutilises tout ça dans CPP03. Le correcteur peut te poser des questions dessus.

### Classe et objet (CPP00)

- **Classe** = le plan de construction. `ClapTrap` décrit ce qu'est un robot.
- **Objet** (ou **instance**) = un robot réel, fabriqué à partir du plan. `ClapTrap a("Randy");` crée un objet.
- **Attribut** = une donnée rangée dans l'objet (`_name`, `_hitPoints`…).
- **Méthode** (ou **fonction membre**) = une action que l'objet sait faire (`attack`, `takeDamage`…).

Image : la classe est la recette du gâteau, l'objet est le gâteau.

### `.hpp` et `.cpp` (CPP00)

- **`.hpp`** (header) = la **déclaration** : « voici ce qui existe ». Pas de code dans les fonctions (interdit par le sujet).
- **`.cpp`** = la **définition** : « voici comment ça marche ».
- **Include guard** = `#ifndef CLAPTRAP_HPP / #define CLAPTRAP_HPP / #endif`. Empêche le fichier d'être inclus deux fois. Sans lui : erreur « redefinition of class ». Dans ex02, `main.cpp` inclut `ClapTrap.hpp`, puis `ScavTrap.hpp` qui l'inclut aussi : le guard évite le doublon.

### `private`, `public` (CPP00)

- **`public`** = accessible par tout le monde (le `main` peut appeler `a.attack()`).
- **`private`** = accessible seulement par les méthodes de la classe elle-même.
- **Encapsulation** = cacher les données en `private` et donner des méthodes pour les utiliser. Le `main` ne peut pas écrire `a._hitPoints = 9999`, il doit passer par `takeDamage` / `beRepaired`.
- **Getter** = méthode qui renvoie un attribut privé (`getHitPoints()`). Elle est `const` car elle ne modifie rien.

### Constructeur, destructeur (CPP00 / CPP01)

- **Constructeur** = fonction appelée automatiquement à la création de l'objet. Même nom que la classe, pas de type de retour.
- **Destructeur** = fonction appelée automatiquement à la mort de l'objet. `~ClapTrap()`.
- **Liste d'initialisation** = la partie après `:` dans le constructeur :

```cpp
ClapTrap::ClapTrap(const std::string& name) : _name(name), _hitPoints(10), _energyPoints(10), _attackDamage(0) {
```

Les attributs reçoivent leur valeur **pendant** leur création, avant le `{`. C'est plus propre que de les remplir dans le corps.

### Pile (stack) et tas (heap) (CPP01)

- Ton `main` crée tous les robots **sur la pile** : `ScavTrap scav("Scavvy");`. Ils meurent tout seuls à la fin du `main`, dans **l'ordre inverse** de création.
- Avec `new`, l'objet serait **sur le tas** et il faudrait un `delete`. Tu n'utilises pas `new` dans CPP03, donc aucun risque de fuite mémoire.

### Référence et `const` (CPP01)

- **Référence** `&` = un autre nom pour une variable existante. Pas de copie.
- `const std::string& name` = « je reçois la chaîne sans la copier, et je promets de ne pas la modifier ».
- Méthode `const` (`getName() const`) = « cette méthode ne modifie pas l'objet ».

### `this` (CPP01 / CPP02)

- `this` = un pointeur vers l'objet sur lequel on appelle la méthode.
- `*this` = l'objet lui-même. `return *this;` dans `operator=` renvoie l'objet pour permettre `a = b = c;`.

### Forme canonique orthodoxe, OCF (CPP02)

Chaque classe doit avoir ces 4 fonctions :

| Fonction | Signature dans ton code | Quand elle est appelée |
| --- | --- | --- |
| Constructeur par défaut | `ClapTrap();` | `ClapTrap d;` |
| Constructeur de copie | `ClapTrap(const ClapTrap& other);` | `ClapTrap c(a);` |
| Opérateur d'affectation | `ClapTrap& operator=(const ClapTrap& other);` | `d = b;` (les deux existent déjà) |
| Destructeur | `~ClapTrap();` | Fin de vie de l'objet |

**Différence copie / affectation :** le constructeur de copie **crée** un nouvel objet. L'affectation **remplace** le contenu d'un objet qui existe déjà.

**Le test `if (this != &other)`** = protection contre `a = a;` (auto-affectation). Sans ce test, avec de la mémoire allouée, on pourrait libérer ses propres données avant de les copier.

### `unsigned int` (C de base)

- Un `unsigned int` ne peut pas être négatif. `0 - 1` donne **4 294 967 295** (on appelle ça un **underflow**).
- C'est pour ça que ton `takeDamage` fait `if (amount >= _hitPoints) _hitPoints = 0;` au lieu de soustraire directement. Bonne réponse à donner à l'éval.

---

## ex00 — ClapTrap

**L'idée en une phrase :** une classe simple qui représente un robot avec des points de vie, d'énergie et d'attaque.

### Ce que le sujet demande

| Attribut | Valeur de départ | Rôle |
| --- | --- | --- |
| `_name` | donné au constructeur | Le nom du robot |
| `_hitPoints` | 10 | La vie |
| `_energyPoints` | 10 | Le carburant pour agir |
| `_attackDamage` | 0 | Les dégâts infligés |

3 actions :

| Méthode | Coût en énergie | Effet |
| --- | --- | --- |
| `attack(target)` | 1 | Affiche un message (ne touche pas vraiment la cible) |
| `takeDamage(amount)` | 0 | Retire des points de vie |
| `beRepaired(amount)` | 1 | Ajoute des points de vie |

Règle : sans vie OU sans énergie, le robot ne peut ni attaquer ni se réparer.

### Ton code, fonction par fonction

**`attack`** (`ex00/ClapTrap.cpp:43`)

1. Mort ? → message, on sort (`return`).
2. Plus d'énergie ? → message, on sort.
3. Sinon : `_energyPoints--` puis le message.

**`takeDamage`** (`ex00/ClapTrap.cpp:56`)

1. Déjà mort ? → message, on sort.
2. Si les dégâts dépassent la vie → la vie tombe à 0 (pas d'underflow).
3. Sinon on soustrait.
4. Pas de coût en énergie : subir des dégâts ne demande pas d'effort.

**`beRepaired`** (`ex00/ClapTrap.cpp:68`)

Mêmes vérifications que `attack`, puis `_hitPoints += amount`.

**Pourquoi `attack` ne retire pas de vie à la cible ?** La cible est un simple `std::string`, pas un robot. Le sujet veut juste un message. C'est le `main` qui appelle `takeDamage` à la main.

### La sortie de l'ex00, expliquée

```
ClapTrap constructor called for Randy        ← ClapTrap a("Randy");
ClapTrap constructor called for Stan         ← ClapTrap b("Stan");
ClapTrap copy constructor called for Randy   ← ClapTrap c(a);
ClapTrap default constructed                 ← ClapTrap d;
ClapTrap copy assignment operator called     ← d = b;
```

La boucle de 12 attaques sur `b` : 10 marchent, les 2 dernières affichent « has no energy ». C'est la preuve que l'énergie fonctionne. Montre-le au correcteur.

---

## ex01 — ScavTrap

**L'idée en une phrase :** `ScavTrap` **est un** `ClapTrap`, avec d'autres valeurs de départ, son propre message d'attaque et une capacité en plus (`guardGate`).

### Notion 1 : l'héritage

```cpp
class ScavTrap : public ClapTrap {
```

- `ClapTrap` = la **classe de base** (ou **classe mère**, **parent**).
- `ScavTrap` = la **classe dérivée** (ou **classe fille**, **enfant**).
- L'enfant **récupère** tous les attributs et toutes les méthodes du parent, sans les réécrire.
- Relation **« est un »** (*is-a*) : un ScavTrap **est un** ClapTrap. C'est la phrase à dire à l'éval.

**Pourquoi c'est utile :** `takeDamage`, `beRepaired`, les getters… sont écrits **une seule fois** dans `ClapTrap`. `ScavTrap` les utilise gratuitement. Dans ton `main`, `scav.takeDamage(30)` appelle la fonction de `ClapTrap`.

**Contre-exemple :** une voiture **a un** moteur (*has-a*), elle n'**est** pas un moteur. Là on mettrait un attribut `Moteur _moteur;`, pas de l'héritage. On appelle ça la **composition**.

### Notion 2 : `protected` (le changement dans ClapTrap)

Tu as remplacé `private:` par `protected:` dans `ex01/ClapTrap.hpp:21`.

| Mot-clé | La classe elle-même | Les enfants | Le reste (`main`) |
| --- | --- | --- | --- |
| `private` | Oui | **Non** | Non |
| `protected` | Oui | **Oui** | Non |
| `public` | Oui | Oui | Oui |

**Pourquoi c'était obligatoire :** `ScavTrap` écrit `_hitPoints = 100;`. Avec `private`, l'enfant possède bien `_hitPoints` en mémoire, mais n'a **pas le droit** d'y toucher → erreur de compilation. `protected` ouvre la porte aux enfants seulement. Le `main` reste bloqué : l'encapsulation est gardée.

### Notion 3 : le `public` dans `: public ClapTrap`

C'est le **type d'héritage**. Il dit comment l'enfant « montre » ce qu'il a hérité.

| Héritage | Le `public` du parent devient… | Conséquence |
| --- | --- | --- |
| `public` | `public` | Le `main` peut appeler `scav.takeDamage()` |
| `protected` | `protected` | Le `main` ne peut plus |
| `private` | `private` | Le `main` ne peut plus, les petits-enfants non plus |

On utilise presque toujours `public`. Sans le mot, une `class` hérite en `private` par défaut, et `scav.takeDamage(30)` ne compilerait plus.

### Notion 4 : l'ordre de construction et de destruction

Regarde ta sortie :

```
ClapTrap constructor called for Scavvy   ← 1. le parent d'abord
ScavTrap constructor called              ← 2. l'enfant ensuite
...
ScavTrap destructor called for Scavvy    ← 1. l'enfant d'abord
ClapTrap destructor called for Scavvy    ← 2. le parent ensuite
```

**Règle :** construction = parent → enfant. Destruction = enfant → parent (l'inverse).

**Image :** une maison. On construit les fondations (parent) avant les murs (enfant). On démolit les murs avant les fondations.

**Pourquoi :** l'enfant s'appuie sur le parent. Pendant que le constructeur de `ScavTrap` tourne, `_name` doit déjà exister. Pendant que le destructeur de `ScavTrap` affiche `_name`, `_name` doit encore exister.

**Question piège :** « Tu n'as jamais écrit d'appel au destructeur de ClapTrap, pourquoi il s'affiche ? » → Le compilateur appelle **automatiquement** le destructeur du parent à la fin du destructeur de l'enfant.

### Notion 5 : appeler le constructeur du parent

```cpp
ScavTrap::ScavTrap(const std::string& name) : ClapTrap(name) {
	_hitPoints = 100;
	_energyPoints = 50;
	_attackDamage = 20;
```

- `: ClapTrap(name)` = « construis d'abord ma partie ClapTrap, avec ce nom ».
- Si tu ne l'écris pas, le compilateur appelle `ClapTrap()` (celui par défaut) → le nom serait `"default"`.

**Pourquoi les valeurs 100 / 50 / 20 sont dans le corps `{ }` et pas dans la liste d'initialisation ?**

Tu ne peux pas écrire `: ClapTrap(name), _hitPoints(100)`. La liste d'initialisation d'une classe ne peut initialiser que **ses propres** attributs et **son parent direct**. `_hitPoints` appartient à `ClapTrap`, il est déjà créé par `ClapTrap(name)` avec la valeur 10. On le **remplace** ensuite dans le corps. Erreur de compilation sinon : `class 'ScavTrap' does not have any field named '_hitPoints'`.

### Notion 6 : la copie dans une classe enfant

```cpp
ScavTrap::ScavTrap(const ScavTrap& other) : ClapTrap(other) {
```

- `other` est un `ScavTrap`, mais un ScavTrap **est un** ClapTrap. On peut donc le donner à `ClapTrap(const ClapTrap&)`. On appelle ça un **upcast** (conversion enfant → parent), il est automatique.
- **Si tu oublies `: ClapTrap(other)`** : le compilateur appelle `ClapTrap()` par défaut. La copie aurait le nom `"default"` et 10 HP au lieu des valeurs de l'original. Bug classique, bonne question d'éval.

```cpp
ScavTrap& ScavTrap::operator=(const ScavTrap& other) {
	if (this != &other) {
		ClapTrap::operator=(other);
	}
	return *this;
}
```

- `ClapTrap::operator=(other)` = « utilise l'affectation de mon parent pour copier les attributs ».
- `ClapTrap::` = **opérateur de résolution de portée**. Il dit dans quelle classe chercher la fonction.
- Pourquoi ne pas recopier les 4 attributs à la main ? Ça marcherait, mais on dupliquerait le code. Si `ClapTrap` gagne un attribut, il faudrait penser à modifier tous les enfants.

### Notion 7 : redéfinir une fonction (`attack`)

`ScavTrap` a sa propre `attack` (`ex01/ScavTrap.cpp:46`), qui affiche « ScavTrap » au lieu de « ClapTrap ».

- **Redéfinition** = l'enfant écrit une fonction avec exactement la même signature que le parent. Pour un objet `ScavTrap`, c'est la version de l'enfant qui est utilisée.
- Le parent n'est pas modifié : `clap.attack()` affiche toujours « ClapTrap ».
- Ton `main` le montre (`Testing ClapTrap vs ScavTrap`) : 0 dégât pour Clappy, 20 pour Scavvy.

**Pourquoi « ClapTrap Scavvy takes 30 damage » et pas « ScavTrap » ?** `takeDamage` n'est pas redéfini dans `ScavTrap`. C'est la fonction de `ClapTrap` qui tourne, avec son message. C'est normal et conforme au sujet. Le correcteur peut le demander.

### Notion 8 : `guardGate`

Une méthode qui existe **seulement** dans `ScavTrap`. Un `ClapTrap` ne l'a pas : `clap.guardGate()` ne compile pas. L'héritage marche dans un seul sens : l'enfant a tout du parent, le parent n'a rien de l'enfant.

### Ce qui va arriver en CPP04 (à savoir citer)

Teste mentalement :

```cpp
ClapTrap* p = &scav;
p->attack("X");
```

Ça affiche **« ClapTrap »**, pas « ScavTrap ». Le compilateur regarde le type du **pointeur** (`ClapTrap*`), pas le vrai type de l'objet. Pour qu'il regarde le vrai objet, il faut mettre `virtual` devant `attack` dans `ClapTrap`. C'est le **polymorphisme**, sujet de CPP04.

Même chose pour le destructeur : avec `delete p;` sur un `ClapTrap*` qui pointe vers un `ScavTrap`, seul `~ClapTrap` serait appelé si le destructeur n'est pas `virtual`. Ton code n'utilise ni pointeur ni `new`, donc il n'y a pas de bug aujourd'hui. Le sujet CPP03 ne demande pas `virtual`.

### La sortie de l'ex01, expliquée

```
=== Testing copy constructor ===
ClapTrap copy constructor called for Scavvy   ← : ClapTrap(other)
ScavTrap copy constructor called for Scavvy
scav2 HP: 80                                  ← 100 - 30 + 10, bien copié

=== Destruction (reverse order) ===
ClapTrap destructor called for Clappy         ← créé en dernier, meurt en premier
ScavTrap destructor called for Scavvy         ← scav2
ClapTrap destructor called for Scavvy
ScavTrap destructor called for Scavvy         ← scav
ClapTrap destructor called for Scavvy
```

**Petit détail facultatif :** tes deux constructeurs de ScavTrap affichent « ScavTrap constructor called » sans nom. Dans ex02 tu affiches le nom. Ajouter `<< " for " << _name` dans ex01 rend la sortie plus lisible.

---

## ex02 — FragTrap

**L'idée en une phrase :** un deuxième enfant de `ClapTrap`, construit exactement comme `ScavTrap`.

### Ce que le sujet demande

| | ClapTrap | ScavTrap | FragTrap |
| --- | --- | --- | --- |
| HP | 10 | 100 | 100 |
| Énergie | 10 | 50 | 100 |
| Attaque | 0 | 20 | 30 |
| Capacité spéciale | — | `guardGate()` | `highFivesGuys()` |
| Redéfinit `attack` | — | Oui | Non (pas demandé) |

### Ce qu'il faut savoir expliquer

- **Deux enfants, un parent :** `ScavTrap` et `FragTrap` sont **frères**. Ils ne se connaissent pas. Un `FragTrap` n'a pas `guardGate()`.
- **Pas de redéfinition d'`attack`** : `frag.attack("Target")` affiche « **ClapTrap** Fraggy attacks Target, causing **30** points ». Le message vient de `ClapTrap`, mais les 30 dégâts viennent de l'attribut `_attackDamage` modifié par le constructeur de `FragTrap`. **Une même fonction, des données différentes.** Bon point à montrer.
- **Ordre de destruction** dans ta sortie : Fraggy → Scavvy → Clappy. Ordre inverse de création, et pour chacun enfant puis parent.

### La sortie de l'ex02, expliquée

```
FragTrap destructor called for Fraggy    ← frag, créé en dernier
ClapTrap destructor called for Fraggy
ScavTrap destructor called for Scavvy    ← scav
ClapTrap destructor called for Scavvy
ClapTrap destructor called for Clappy    ← clap, créé en premier, pas d'enfant
```

### ex03 (bonus, pas codé) en 4 lignes

1. `DiamondTrap` hérite de `ScavTrap` **et** `FragTrap` (**héritage multiple**).
2. Problème : il aurait **deux** `ClapTrap` à l'intérieur (un par parent). C'est le **problème du diamant**.
3. Solution : `class ScavTrap : virtual public ClapTrap` (et pareil pour FragTrap). Il n'y a plus qu'un seul `ClapTrap` partagé. C'est **l'héritage virtuel**.
4. Les détails sont dans `CPP03_GUIDE.md`, section ex03. Le bonus n'est noté que si les 3 exos obligatoires sont parfaits.

---

## 5. Avant l'éval : questions, pièges, checklist

### Les questions de l'éval, avec la réponse courte

| Question | Réponse courte |
| --- | --- |
| C'est quoi l'héritage ? | Une classe enfant récupère les attributs et méthodes d'une classe parent. Relation « est un ». |
| Pourquoi `protected` ? | Pour que les enfants puissent modifier `_hitPoints` etc., sans que le `main` le puisse. |
| Différence `private` / `protected` ? | `private` : la classe seule. `protected` : la classe et ses enfants. |
| Que veut dire `: public ClapTrap` ? | Héritage public : ce qui est public chez le parent reste public chez l'enfant. |
| Ordre de construction ? | Parent puis enfant. |
| Ordre de destruction ? | Enfant puis parent. Et les objets du `main` meurent dans l'ordre inverse de création. |
| Pourquoi 100/50/20 dans le corps du constructeur ? | Ces attributs appartiennent au parent. La liste d'init ne peut initialiser que ses propres attributs et le parent direct. |
| Que se passe-t-il si on oublie `: ClapTrap(other)` dans la copie ? | Le parent est construit par défaut : nom `"default"`, 10 HP. La copie est fausse. |
| Pourquoi `takeDamage` affiche « ClapTrap » pour un ScavTrap ? | Elle n'est pas redéfinie, c'est la fonction du parent qui tourne. |
| C'est quoi redéfinir ? | Écrire dans l'enfant une méthode de même signature que celle du parent. |
| Pourquoi pas de `virtual` ? | Pas demandé en CPP03, pas de pointeur parent vers enfant dans le code. Ce sera nécessaire en CPP04. |
| Pourquoi `if (amount >= _hitPoints)` ? | `unsigned int` : soustraire trop donnerait un nombre énorme (underflow). |
| Pourquoi l'include guard ? | Éviter qu'un `.hpp` soit inclus deux fois (ex02 : ClapTrap.hpp est inclus 3 fois). |
| Différence copie / affectation ? | Copie : crée un nouvel objet. Affectation : remplace le contenu d'un objet existant. |

### Les 5 pièges les plus fréquents

1. **Laisser `private` dans ClapTrap** → l'enfant ne compile pas.
2. **Oublier `public` dans `: public ClapTrap`** → `scav.takeDamage()` ne compile plus dans le `main`.
3. **Oublier `: ClapTrap(name)`** → tous les ScavTrap s'appellent `"default"`.
4. **Oublier `: ClapTrap(other)` dans la copie** → copie fausse.
5. **Message du constructeur identique au parent** → le sujet exige des messages différents pour voir l'ordre. Le tien est bon.

### Tests à faire devant le correcteur (5 min)

1. `make re` dans chaque dossier : aucun warning.
2. `./claptrap` : montre les 2 attaques sans énergie à la fin de la boucle.
3. `./scavtrap` : montre l'ordre parent → enfant à la construction, enfant → parent à la destruction.
4. `./fragtrap` : montre que l'attaque de Fraggy fait 30 dégâts avec le message de ClapTrap.
5. Si on te demande de modifier le `main` : ajoute `ScavTrap s2; s2 = scav;` et montre l'appel à `operator=`.

### Les interdits du module

| Interdit | Sanction |
| --- | --- |
| `printf`, `malloc`, `free` | 0 |
| `using namespace std`, `friend` | -42 |
| STL (`vector`, `map`…) | -42 |
| Code de fonction dans un `.hpp` | 0 à l'exo |
| `.hpp` sans include guard | 0 |

### Checklist finale

- [ ] `make re` sans warning dans ex00, ex01, ex02
- [ ] Pas de `.o` ni d'exécutable dans le rendu (`make fclean` avant le push)
- [ ] Chaque classe a ses 4 fonctions (OCF)
- [ ] Les noms de fichiers correspondent aux classes (`ScavTrap.cpp`, pas `scavTrap.cpp` : déjà corrigé)
- [ ] Tu sais répondre au tableau de questions sans regarder

---

## Lexique : tous les mots en une ligne

**Notions de base (anciens modules)**

| Mot | Définition simple |
| --- | --- |
| Classe | Le plan d'un type d'objet. |
| Objet / instance | Un exemplaire réel créé à partir d'une classe. |
| Attribut | Une donnée stockée dans un objet. |
| Méthode | Une fonction qui appartient à une classe. |
| Constructeur | Fonction appelée automatiquement à la création. |
| Destructeur | Fonction appelée automatiquement à la destruction. |
| Liste d'initialisation | La partie `: a(x), b(y)` qui donne leur valeur aux attributs à leur création. |
| Encapsulation | Cacher les données et n'y donner accès que par des méthodes. |
| Getter | Méthode qui renvoie la valeur d'un attribut caché. |
| Référence `&` | Un autre nom pour une variable existante, sans copie. |
| `const` | Promesse de ne pas modifier. |
| `this` | Pointeur vers l'objet en cours. |
| OCF | Les 4 fonctions obligatoires : constructeur par défaut, de copie, `operator=`, destructeur. |
| Include guard | `#ifndef/#define/#endif` pour éviter une double inclusion. |
| Pile (stack) | Mémoire automatique : l'objet meurt seul en fin de bloc. |
| Tas (heap) | Mémoire manuelle : `new` et `delete`. |
| Underflow | Un `unsigned` qui passe sous 0 et devient un nombre énorme. |

**Notions de CPP03**

| Mot | Définition simple |
| --- | --- |
| Héritage | Une classe récupère tout ce qu'a une autre classe. |
| Classe de base / mère / parent | La classe dont on hérite (`ClapTrap`). |
| Classe dérivée / fille / enfant | La classe qui hérite (`ScavTrap`, `FragTrap`). |
| « Est un » (is-a) | Le test pour savoir si l'héritage a du sens. |
| « A un » (has-a) / composition | Un objet contient un autre objet en attribut. Alternative à l'héritage. |
| `protected` | Accessible par la classe et ses enfants, pas par le reste. |
| Héritage public | Le `public` du parent reste `public` chez l'enfant. |
| Ordre de construction | Parent puis enfant. |
| Ordre de destruction | Enfant puis parent. |
| Redéfinition | L'enfant réécrit une méthode du parent avec la même signature. |
| Opérateur de portée `::` | Précise dans quelle classe chercher (`ClapTrap::operator=`). |
| Upcast | Utiliser un enfant comme s'il était son parent. Automatique. |
| Classes sœurs | Deux enfants du même parent (`ScavTrap` et `FragTrap`). |
| Héritage multiple | Une classe a deux parents ou plus (ex03). |
| Problème du diamant | Avec deux parents qui ont le même grand-parent, on a deux copies du grand-parent. |
| Héritage virtuel | `virtual public` : une seule copie du grand-parent partagée (ex03). |

**À citer pour CPP04**

| Mot | Définition simple |
| --- | --- |
| `virtual` (méthode) | Le programme choisit la méthode selon le vrai type de l'objet, pas celui du pointeur. |
| Polymorphisme | Un même appel (`p->attack()`) qui donne un comportement différent selon l'objet. |
| Destructeur virtuel | Obligatoire dès qu'on fait `delete` sur un pointeur parent qui pointe vers un enfant. |
