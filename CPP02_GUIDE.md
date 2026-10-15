# CPP02 — Guide pas à pas

Version en ligne (modifiable) : https://claude.ai/code/artifact/344420dc-3970-469f-9600-39d9b53e64bd

## Commence ici (5 min)

1. Lance `mkdir ex00 ex01 ex02 ex03`
2. Crée `ex00/Fixed.hpp` et écris juste l'include guard
3. Descends à la section **ex00** et coche les cases une par une

## Le plan : 4 exos, 1 seule classe

Tu codes la classe `Fixed` dans ex00, puis tu lui ajoutes des fonctions à chaque exo.

| Exo | Ce que tu ajoutes | Temps estimé | Obligatoire |
| --- | --- | --- | --- |
| ex00 | La coquille : 4 fonctions canoniques + get/set | ~1 h | Oui |
| ex01 | Conversions int / float + `<<` | ~1 h 30 | Oui |
| ex02 | 18 opérateurs + `min` / `max` | 2 à 3 h | Oui |
| ex03 | Classe `Point` + fonction `bsp` | ~2 h | Non (bonus) |

Pour démarrer un exo, copie le précédent : `cp ex00/* ex01/`.

## Les 5 interdits (0 ou -42)

| Si tu fais ça | Note |
| --- | --- |
| `printf`, `malloc`, `free` | 0 |
| Du code de fonction dans un `.hpp` | 0 à l'exo |
| Un `.hpp` sans include guard | 0 |
| `using namespace` ou `friend` | -42 |
| `vector`, `map`, `<algorithm>` (STL) | -42 |

## Les 5 règles de base

1. Compile avec `c++ -Wall -Wextra -Werror -std=c++98`
2. Un Makefile par exo (`all`, `clean`, `fclean`, `re`)
3. Fichiers nommés comme la classe : `Fixed.hpp` + `Fixed.cpp`
4. Chaque message finit par `std::endl`
5. Chaque classe a ses 4 fonctions canoniques (section suivante)

Ce guide te dit quoi coder, pas le code. Le sujet l'exige, et à l'éval tu dois tout expliquer.

## À lire avant ex00 (10 min)

### Notion 1 : la virgule fixe = un int × 256

**Ce que tu stockes = valeur réelle × 256.** C'est tout. 256 = `1 << 8`.

| Valeur réelle | Stocké dans `_value` |
| --- | --- |
| 0 | 0 |
| 1 | 256 |
| 10 | 2560 |
| 0.5 | 128 |
| 0.00390625 (le plus petit pas) | 1 |

Les 3 conversions dont tu auras besoin :

1. **int → Fixed** : `n << 8`
2. **float → Fixed** : `roundf(f * 256)`
3. **Fixed → int ou float** : l'inverse (`>> 8`, ou diviser par 256.0f)

### Notion 2 : les 4 fonctions canoniques (OCF)

Chaque classe doit avoir ces 4 fonctions, de ce module jusqu'au module 09.

| Fonction | Prototype pour `Fixed` | Appelée par |
| --- | --- | --- |
| Constructeur par défaut | `Fixed();` | `Fixed a;` |
| Constructeur de copie | `Fixed(const Fixed &other);` | `Fixed b(a);` |
| Opérateur d'affectation | `Fixed &operator=(const Fixed &other);` | `c = b;` |
| Destructeur | `~Fixed();` | Fin de l'objet |

**La différence à retenir :** copie = l'objet naît. Affectation = l'objet existe déjà.

`operator=` fait toujours ces 3 choses :

1. Vérifie `if (this != &other)`
2. Copie les membres
3. Retourne `*this`

## ex00 — la coquille (~1 h)

**Fini quand :** ta sortie est identique à celle du sujet, ligne par ligne.

**Fichiers dans `ex00/` :** `Makefile`, `main.cpp`, `Fixed.hpp`, `Fixed.cpp`

### Étape 1 — `Fixed.hpp` (15 min)

- [ ] **Privé** : `int _value;`
- [ ] **Privé** : `static const int _bits;` (vaudra 8)
- [ ] **Public** : les 4 prototypes canoniques
- [ ] **Public** : `int getRawBits( void ) const;` et `void setRawBits( int const raw );`
- [ ] Vérifie : aucun `{ }` de fonction dans ce fichier

### Étape 2 — `Fixed.cpp` (30 min)

- [ ] En haut : `const int Fixed::_bits = 8;`
- [ ] Constructeur par défaut : `_value` à 0, affiche `Default constructor called`
- [ ] Copie : affiche `Copy constructor called` / `operator=` : affiche `Copy assignment operator called`
- [ ] Destructeur : affiche `Destructor called`
- [ ] `getRawBits` : affiche `getRawBits member function called`, retourne `_value` / `setRawBits` : change `_value`

### Étape 3 — tester (15 min)

- [ ] Recopie le main du sujet dans `main.cpp`
- [ ] Écris le Makefile, lance `make`
- [ ] Compare ta sortie à celle du sujet

**Si tu as une ligne `Copy assignment operator called` en plus après la copie :** c'est normal si ton constructeur de copie fait `*this = other;`. Les deux versions sont acceptées.

## ex01 — conversions int / float (~1 h 30)

**Fini quand :** `c is 42.4219` et `a is 1234.43` s'affichent.

**Départ :** `cp ex00/* ex01/`. **Autorisé :** `roundf` (inclus `<cmath>`).

### Étape 1 — 2 nouveaux constructeurs (20 min)

- [ ] `Fixed(const int n);` : stocke `n << 8`, affiche `Int constructor called`
- [ ] `Fixed(const float f);` : stocke `roundf(f * 256)`, affiche `Float constructor called`

### Étape 2 — 2 fonctions de sortie (20 min)

- [ ] `float toFloat( void ) const;` : `_value` divisé par `256.0f` (le `.0f` garde la virgule)
- [ ] `int toInt( void ) const;` : `_value >> 8`

### Étape 3 — l'opérateur `<<` (20 min)

- [ ] Déclare-le dans `Fixed.hpp`, **après** la classe (fonction libre, pas membre)
- [ ] Prototype : `std::ostream &operator<<(std::ostream &out, const Fixed &f);`
- [ ] Il écrit `f.toFloat()` dans `out`, puis retourne `out`
- [ ] Pas de `friend` : `toFloat()` est public, ça suffit

### Étape 4 — tester (15 min)

- [ ] Recopie le main du sujet
- [ ] Compare avec la sortie du sujet

```
a is 1234.43
b is 10
c is 42.4219
d is 10
a is 1234 as integer
```

**Si tu vois `42.418` :** il te manque `roundf`. **Si tu vois `42` :** tu divises par un int au lieu de `256.0f`.

## ex02 — les opérateurs (2 à 3 h)

**Fini quand :** la dernière ligne affichée est `10.1016`.

**Départ :** `cp ex01/* ex02/`. Fais un bloc, compile, teste, puis passe au suivant.

### Bloc 1 — 6 comparaisons (20 min)

- [ ] `>` `<` `>=` `<=` `==` `!=`
- [ ] Prototype type : `bool operator>(const Fixed &other) const;`
- [ ] Compare juste les deux `_value`

### Bloc 2 — 4 opérations (40 min)

- [ ] Prototype type : `Fixed operator+(const Fixed &other) const;` (retourne un **nouvel** objet)
- [ ] `+` et `-` : sur les `_value`
- [ ] `*` et `/` : le plus simple est de passer par `toFloat()`, puis tu crées un `Fixed` avec le résultat
- [ ] Division par 0 : le sujet accepte un crash

### Bloc 3 — `++` et `--` (40 min, le plus piégeux)

Ils changent `_value` de **1**, donc la valeur réelle de 0.00390625.

| Écriture | Prototype | Retourne |
| --- | --- | --- |
| `++a` | `Fixed &operator++();` | L'objet **après** changement (référence) |
| `a++` | `Fixed operator++(int);` | Une copie d'**avant** le changement (valeur) |
| `--a` | `Fixed &operator--();` | L'objet après changement (référence) |
| `a--` | `Fixed operator--(int);` | Une copie d'avant le changement (valeur) |

- [ ] Pré : 1. modifie, 2. retourne `*this`
- [ ] Post : 1. copie `*this`, 2. modifie, 3. retourne la copie

Le `int` du post ne sert qu'à différencier les deux versions. Tu ne l'utilises pas.

### Bloc 4 — `min` et `max` (20 min)

- [ ] `static Fixed &min(Fixed &a, Fixed &b);`
- [ ] `static const Fixed &min(const Fixed &a, const Fixed &b);`
- [ ] Les deux mêmes pour `max`
- [ ] Utilise tes comparaisons du bloc 1

La version `const` est obligatoire : dans le main, `b` est `const`.

### Bloc 5 — tester (30 min)

- [ ] Recopie le main du sujet, compare avec la sortie ci-dessous
- [ ] Ajoute tes tests : `-`, `/`, `--`, `min`

```
0
0.00390625
0.00390625
0.00390625
0.0078125
10.1016
10.1016
```

## ex03 — BSP, bonus (~2 h)

**Fais-le seulement si ex00, ex01 et ex02 marchent.** Le module est validé sans ex03.

**Fini quand :** tes 5 tests (étape 3) donnent le bon résultat.

**Fichiers :** ton `Fixed` d'ex02 + `Point.hpp`, `Point.cpp`, `bsp.cpp`, `main.cpp`, `Makefile`

### Étape 1 — la classe `Point` (40 min)

- [ ] **Privé** : `Fixed const x;` et `Fixed const y;`
- [ ] `Point();` (x = y = 0) et `Point(const float x, const float y);`
- [ ] Initialise x et y dans la **liste d'initialisation** : `Point() : x(0), y(0) {}` (dans le `.cpp`)
- [ ] Copie, `operator=`, destructeur
- [ ] Getters `getX()` et `getY()`

**`operator=` ne peut rien copier :** x et y sont `const`. Il retourne juste `*this`. Sache l'expliquer à l'éval.

### Étape 2 — la fonction `bsp` (40 min)

Prototype imposé dans `bsp.cpp` : `bool bsp( Point const a, Point const b, Point const c, Point const point);`

1. Pour chaque côté (AB, BC, CA), calcule le signe du produit vectoriel avec le point
2. Un résultat vaut 0 → le point est sur un bord → `false`
3. Les 3 signes sont identiques → `true`, sinon `false`

La formule, à toi de la trouver : cherche « point in triangle cross product ».

### Étape 3 — 5 tests dans le main (30 min)

- [ ] Un point au milieu → `true`
- [ ] Un point dehors → `false`
- [ ] Un sommet → `false`
- [ ] Un point sur un bord → `false`
- [ ] Les sommets dans l'autre ordre → même résultat

## Avant de rendre (20 min)

### Si ça ne compile pas : les 5 causes les plus fréquentes

| Erreur | Cause | Correction |
| --- | --- | --- |
| Erreur sur un `Fixed const` | Méthode sans `const` à la fin | Ajoute `const` : `toFloat() const` |
| `undefined reference to Fixed::_bits` | Constante pas définie | Ajoute `const int Fixed::_bits = 8;` dans le `.cpp` |
| `roundf` inconnu | Include manquant | `#include <cmath>` |
| `ostream` inconnu | Include manquant | `#include <iostream>` dans `Fixed.hpp` |
| Valeur bizarre après `a++` | Référence vers une variable locale | Le post-incrément retourne par valeur |

### Checklist par dossier (à refaire pour chaque exo)

- [ ] `make re` compile sans warning
- [ ] La sortie du main = la sortie du sujet
- [ ] Aucun corps de fonction dans un `.hpp`, include guard partout
- [ ] Aucun `printf`, `malloc`, `friend`, `using namespace`, STL
- [ ] Noms de dossiers et de fichiers exacts

### À savoir expliquer à l'éval, sans notes

- [ ] Pourquoi × 256
- [ ] Copie vs affectation
- [ ] `++a` vs `a++` : pourquoi l'un retourne une référence
- [ ] Pourquoi `min` / `max` existent en version `const`
- [ ] (ex03) Pourquoi `operator=` de `Point` ne copie rien

**Prochaine action :** lance `mkdir ex00 ex01 ex02 ex03` et ouvre `ex00/Fixed.hpp`.
Footer
