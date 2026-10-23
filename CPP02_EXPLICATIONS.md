# CPP02 — Fixed-point expliqué

18/10/2026

## Démarrer ici

**Action tout de suite (2 min) :** ouvre `CPP02/ex00/Fixed.cpp` ligne 29 et ajoute le `;` manquant après `n << _bits`. Ton ex00 ne compile pas aujourd'hui (détails dans la section ex00).

**Le module en 5 lignes :**

1. Tu construis UNE classe, `Fixed`, qui représente un nombre à virgule avec un simple `int`.
2. ex00 : la classe sait naître, être copiée, être affectée et mourir (forme canonique).
3. ex01 : elle sait se convertir depuis et vers `int` / `float`, et s'afficher avec `<<`.
4. ex02 : elle se comporte comme un vrai nombre : `a + b`, `a < b`, `++a`, `Fixed::max(a, b)`.
5. ex03 (bonus, absent de ton dépôt) : tu l'utilises pour savoir si un point est dans un triangle.

**État de ton code au 18/10/2026 :**

| Exo | Compile | Sortie conforme | À faire |
| --- | --- | --- | --- |
| ex00 | Non | — | 3 corrections (section ex00) |
| ex01 | Oui | Oui | Rien |
| ex02 | Oui | Oui (les nombres) | Décider pour les messages « called » |
| ex03 | Absent | — | Bonus, facultatif |

**Comment lire ce doc :** chaque section suit le même ordre. 1) Pourquoi on le fait. 2) Ton code, ligne par ligne. 3) Ce qu'on te demandera à l'éval. Lis une section, ferme, reviens plus tard.

## Sommaire

1. [Pourquoi la virgule fixe](#pourquoi-la-virgule-fixe)
2. [ex00 — La forme canonique](#ex00--la-forme-canonique)
3. [ex01 — Conversions et affichage](#ex01--conversions-et-affichage)
4. [ex02 — Surcharge des opérateurs](#ex02--surcharge-des-opérateurs)
5. [ex03 — BSP (bonus)](#ex03--bsp-bonus-pas-encore-codé)
6. [Avant l'éval](#avant-léval--pièges-questions-checklist)

## Pourquoi la virgule fixe

**L'idée en une phrase :** on stocke un nombre à virgule dans un `int`, en le multipliant par 256. `_value = valeur réelle × 256`.

### Le problème que ça résout

Un `float` stocke un nombre en notation scientifique binaire : un signe, un exposant, une mantisse (norme IEEE 754). Conséquences :

- **Imprécis :** `0.1f + 0.2f` ne vaut pas exactement `0.3f`. 0.1 n'a pas d'écriture exacte en binaire.
- **Précision variable :** près de 0, les floats sont très serrés. Vers 16 millions, l'écart entre deux floats vaut déjà 1.
- **Pas toujours dispo :** certains processeurs (microcontrôleurs, vieilles consoles) n'ont pas d'unité de calcul flottant. Les calculs en float y sont très lents.

La virgule fixe donne un **pas constant** (toujours 1/256 ici), des calculs **exacts et reproductibles**, et n'utilise que des opérations sur entiers. On la trouve dans les jeux vidéo (Doom, PlayStation 1), le traitement du son, l'embarqué, et les logiciels financiers (centimes stockés en entiers).

### Comment c'est rangé dans l'int

Ton `int` fait 32 bits. Les 8 bits de droite sont la partie après la virgule. Les 24 bits de gauche sont la partie entière (avec le signe). On appelle ça du **Q24.8**.

| Bits | 31 … 8 (24 bits) | 7 … 0 (8 bits) |
| --- | --- | --- |
| Rôle | Partie entière + signe | Partie fractionnaire |
| Exemple 1.5 | `…000001` (= 1) | `10000000` (= 128/256 = 0.5) |

Donc `_value` de 1.5 = 384 = 1 × 256 + 128.

| Valeur réelle | `_value` stocké | Calcul |
| --- | --- | --- |
| 0 | 0 | 0 × 256 |
| 0.00390625 | 1 | le plus petit pas = 1/256 |
| 0.5 | 128 | 0.5 × 256 |
| 1 | 256 | 1 × 256 |
| 10 | 2560 | 10 × 256 |
| 42.42 | 10860 | arrondi de 10859.52 |
| -1 | -256 | -1 × 256 |

**Limites :** précision 1/256 ≈ 0.0039. Plage d'environ -8 388 608 à +8 388 607.996 (2^23).

### Pourquoi 256 et `<< 8`

256 = 2^8 = `1 << 8`. Décaler un entier de 8 bits vers la gauche = le multiplier par 256. Vers la droite = le diviser par 256 (en tronquant). Le processeur fait un décalage plus vite qu'une multiplication. C'est pour ça que `_bits = 8` et pas 10 : une puissance de 2.

### Les 4 calculs à comprendre

| Opération | Formule sur `_value` | Pourquoi |
| --- | --- | --- |
| a + b | `a + b` | (A×256) + (B×256) = (A+B)×256. L'échelle reste bonne. |
| a - b | `a - b` | Même raison. |
| a × b | `(a * b) >> 8` | (A×256) × (B×256) = A×B×256². Un 256 de trop, on divise par 256. |
| a / b | `(a << 8) / b` | (A×256) / (B×256) = A/B, sans 256. On remultiplie avant de diviser pour ne pas perdre la virgule. |

**Exemple du main de l'ex02 :** `5.05 × 2`. `5.05` → 1293 (arrondi de 1292.8). `2` → 512. 1293 × 512 = 662016. `>> 8` → 2586. 2586 / 256 = **10.1016**. Le vrai résultat (10.1) n'est pas atteint : c'est la perte de précision normale, et c'est exactement ce que le sujet affiche.

## ex00 — La forme canonique

**But de l'exo :** apprendre les 4 fonctions que TOUTE classe doit avoir à 42 (de CPP02 à CPP09). On les appelle la **forme canonique orthodoxe** (Orthodox Canonical Form, OCF).

### Ton ex00 ne compile pas : 3 corrections (10 min)

1. `ex00/Fixed.cpp:29` : il manque `;` après `this->_value = n << _bits`.
2. `ex00/Fixed.hpp` : `std::ostream` est utilisé sans `#include <iostream>`. Ajoute-le sous `# define FIXED_HPP`.
3. `ex00/main.cpp` : il manque `#include "Fixed.hpp"` en haut.

**Plus simple :** ton ex00 contient déjà le code de l'ex01 (constructeurs int/float, `toFloat`, `toInt`, `<<`). L'ex00 ne le demande pas. Tu peux supprimer ces 5 fonctions de l'ex00 : moins de code = moins de questions à l'éval.

### Pourquoi ces 4 fonctions existent

En C++, si tu ne les écris pas, le compilateur les génère tout seul. Pour une classe avec un simple `int`, la version automatique marche. Mais dès qu'une classe contient un pointeur (CPP03 et suivants), la version automatique copie l'adresse, pas les données. Deux objets partagent alors la même mémoire, et le deuxième `delete` plante (double free). 42 t'oblige à les écrire pour que tu saches toujours ce qui se passe.

| Fonction | Prototype | Quand elle s'exécute | Exemple |
| --- | --- | --- | --- |
| Constructeur par défaut | `Fixed();` | Création sans argument | `Fixed a;` |
| Constructeur de copie | `Fixed(const Fixed &other);` | Création à partir d'un autre objet | `Fixed b(a);` ou `Fixed b = a;` |
| Opérateur d'affectation | `Fixed &operator=(const Fixed &other);` | Objet déjà existant qui reçoit une valeur | `c = b;` |
| Destructeur | `~Fixed();` | Fin de vie de l'objet | Fin du bloc `{ }` |

**À retenir :** copie = l'objet naît. Affectation = l'objet existait déjà. Piège : `Fixed b = a;` appelle la **copie**, pas `operator=`, car `b` naît sur cette ligne.

### Ton code, fonction par fonction

**Les attributs privés**

```cpp
private:
	int _value;              // le nombre × 256
	static const int _bits;  // vaut 8, partagé par tous les objets
```

- `private` : personne hors de la classe ne peut toucher `_value` directement. C'est l'**encapsulation**.
- `static` : `_bits` existe une seule fois en mémoire, pas une fois par objet.
- `const` : on ne peut pas le modifier.
- `const int Fixed::_bits = 8;` dans le `.cpp` : la **définition**. Le `.hpp` ne fait que la déclarer. Sans elle : `undefined reference to Fixed::_bits`.

**Constructeur par défaut**

```cpp
Fixed::Fixed() : _value(0) {
	std::cout << "Default constructor called" << std::endl;
}
```

- `: _value(0)` est une **liste d'initialisation**. `_value` est créé directement avec 0, avant d'entrer dans les `{ }`.
- Pourquoi pas `_value = 0;` dans le corps ? Ça marche ici. Mais un attribut `const` ou une référence ne peut être rempli QUE par la liste d'initialisation (tu en auras besoin dans l'ex03 avec `Point`).
- Sans initialisation, `_value` contiendrait n'importe quoi (une valeur résiduelle de la mémoire).

**Constructeur de copie**

```cpp
Fixed::Fixed(const Fixed &other) {
	std::cout << "Copy constructor called" << std::endl;
	*this = other;
}
```

- `const Fixed &other` : une **référence**. Si on passait `Fixed other` (par valeur), il faudrait copier `other` pour appeler… le constructeur de copie. Boucle infinie : le compilateur refuse.
- `const` : on promet de ne pas modifier l'original.
- `*this = other;` : on réutilise `operator=`. C'est pour ça que la sortie affiche `Copy assignment operator called` juste après `Copy constructor called`. Le sujet montre exactement cette sortie.
- `this` = un pointeur vers l'objet en cours. `*this` = l'objet lui-même.

**Opérateur d'affectation**

```cpp
Fixed& Fixed::operator=(const Fixed &other) {
	std::cout << "Copy assignment operator called" << std::endl;
	if (this != &other)
		this->_value = other.getRawBits();
	return *this;
}
```

- `if (this != &other)` : protection contre l'**auto-affectation** (`a = a;`). Ici ça ne casserait rien. Avec de la mémoire dynamique, on libérerait la mémoire avant de la recopier depuis… soi-même, déjà libérée.
- `return *this;` en `Fixed&` : permet d'enchaîner `a = b = c;` (ça se lit `a = (b = c)`).
- `other.getRawBits()` : produit la ligne `getRawBits member function called` vue dans la sortie. Tu pourrais écrire `other._value` : dans une méthode de `Fixed`, on a accès aux privés d'un AUTRE `Fixed`. Le privé est par classe, pas par objet.

**Destructeur**

```cpp
Fixed::~Fixed() {
	std::cout << "Destructor called" << std::endl;
}
```

Rien à libérer ici (pas de `new`). Il est appelé automatiquement, dans l'ordre inverse de création (le dernier créé meurt en premier).

**getRawBits / setRawBits**

```cpp
int Fixed::getRawBits() const { ... return this->_value; }
void Fixed::setRawBits(int const raw) { this->_value = raw; }
```

- Un **getter** et un **setter** : la seule porte d'accès à `_value` depuis l'extérieur.
- Le `const` après `getRawBits()` : la méthode promet de ne pas modifier l'objet. Sans lui, impossible de l'appeler sur un `Fixed const` (et le main de l'ex01 a des `Fixed const`).
- « Raw bits » = la valeur brute stockée (× 256), pas le nombre réel.

### La sortie de l'ex00, expliquée

| Ligne du main | Ce qui s'affiche |
| --- | --- |
| `Fixed a;` | Default constructor called |
| `Fixed b( a );` | Copy constructor called, puis Copy assignment operator called, puis getRawBits member function called |
| `Fixed c;` | Default constructor called |
| `c = b;` | Copy assignment operator called, puis getRawBits member function called |
| 3 × `std::cout << x.getRawBits()` | getRawBits member function called, puis 0 (trois fois) |
| `return 0;` | Destructor called × 3 (c, puis b, puis a) |

## ex01 — Conversions et affichage

**But de l'exo :** rendre `Fixed` utile. On peut le créer depuis un `int` ou un `float`, le reconvertir, et l'afficher avec `std::cout <<`. Ton ex01 compile et donne la sortie du sujet.

### Constructeur depuis un int

```cpp
Fixed::Fixed(const int n) {
	std::cout << "Int constructor called" << std::endl;
	this->_value = n << this->_bits;
}
```

- `n << 8` = `n × 256`. Un entier n'a pas de partie décimale, donc les 8 bits de droite restent à 0.
- Exemple : `Fixed b(10)` → `_value` = 2560.
- Ce constructeur sert aussi de **conversion implicite** : `Fixed x = 10;` marche, et dans l'ex02 `Fixed(5.05f) * Fixed(2)` aussi.
- Détail technique : en C++98, décaler à gauche un nombre négatif est théoriquement indéfini. En pratique, tous les compilateurs donnent le bon résultat. Si on te le demande : `n * (1 << _bits)` évite le problème.

### Constructeur depuis un float

```cpp
Fixed::Fixed(const float n) {
	std::cout << "Float constructor called" << std::endl;
	this->_value = static_cast<int>(n * (1 << this->_bits) + (n < 0 ? -0.5f : 0.5f));
}
```

Lecture de l'intérieur vers l'extérieur :

1. `(1 << this->_bits)` = 256.
2. `n * 256` : on passe à l'échelle. `42.42f × 256 = 10859.52`.
3. `+ 0.5f` (ou `- 0.5f` si négatif) : **arrondi** à l'entier le plus proche. 10859.52 + 0.5 = 10860.02.
4. `static_cast<int>(...)` : coupe la partie décimale → 10860.

**Pourquoi arrondir ?** Sans arrondi, 10859.52 devient 10859 → `c` s'afficherait `42.418` au lieu de `42.4219`. Ton `+ 0.5` fait la même chose que `roundf()` de `<cmath>`, que le sujet autorise. Les deux sont acceptés.

**Pourquoi `-0.5` pour les négatifs ?** `static_cast<int>` coupe vers 0. Pour -2.7 : -2.7 - 0.5 = -3.2 → -3. Correct. Avec +0.5 on aurait -2.2 → -2. Faux.

**`static_cast<int>`** = la conversion propre du C++. Le cast C `(int)x` marche aussi, mais `static_cast` est vérifié à la compilation et facile à repérer.

### toFloat

```cpp
float Fixed::toFloat() const {
	return static_cast<float>(this->_value) / (1 << this->_bits);
}
```

- L'inverse : on divise par 256.
- Le `static_cast<float>` est **indispensable**. `int / int` = division entière : 10860 / 256 = 42, la virgule disparaît. `float / int` = division décimale → 42.421875.
- `const` : ne modifie pas l'objet. Obligatoire, car `operator<<` reçoit un `const Fixed &`.

### toInt

```cpp
int Fixed::toInt() const {
	return this->_value >> this->_bits;
}
```

- `>> 8` = diviser par 256 en jetant les 8 bits de décimales.
- 1234.43 → `_value` 316015 → `>> 8` → 1234.
- Pour un négatif, `>>` arrondit vers le bas : -1.5 donne -2, pas -1. Bon à savoir si un évaluateur teste.

### L'opérateur <<

```cpp
std::ostream &operator<<(std::ostream &out, const Fixed &fixed) {
	out << fixed.toFloat();
	return out;
}
```

**Pourquoi une fonction libre, hors de la classe ?** Dans `std::cout << a`, l'objet de gauche est `std::cout`, pas `a`. Une méthode membre s'appelle toujours sur l'objet de gauche. Il faudrait donc l'ajouter dans la classe `std::ostream`, ce qui est impossible. On écrit une fonction normale qui prend les deux côtés en paramètres.

**Pourquoi pas de `friend` ?** `friend` donnerait accès à `_value`. Inutile : `toFloat()` est public. Et `friend` est interdit dans ce module.

**Pourquoi retourner `out` ?** Pour enchaîner : `std::cout << "a is " << a << std::endl;` se lit `((std::cout << "a is ") << a) << std::endl`. Chaque `<<` rend le flux pour le suivant.

**Pourquoi `std::ostream` et pas `std::cout` ?** `ostream` est le type général. Ta fonction marche aussi avec `std::cerr` ou un fichier.

**Pourquoi des références ?** Un flux ne peut pas être copié. Et `const Fixed &` évite une copie inutile de l'objet (et donc un « Copy constructor called » de plus).

### La sortie de l'ex01, expliquée

| Ligne du main | Messages | Pourquoi |
| --- | --- | --- |
| `Fixed a;` | Default | Pas d'argument |
| `Fixed const b( 10 );` | Int | 10 est un `int` |
| `Fixed const c( 42.42f );` | Float | Le `f` fait un `float` |
| `Fixed const d( b );` | Copy, Copy assignment, getRawBits | Copie qui appelle `operator=` |
| `a = Fixed( 1234.4321f );` | Float, Copy assignment, getRawBits, Destructor | Un objet **temporaire** est créé, copié dans `a`, puis détruit à la fin de la ligne |
| Les 8 `std::cout` | Les valeurs | `a is 1234.43`, `c is 42.4219`, `c is 42 as integer`… |
| `return 0;` | Destructor × 4 | d, c, b, a |

**Pourquoi 1234.43 et pas 1234.4321 ?** Deux raisons. 1) `std::cout` affiche 6 chiffres significatifs par défaut. 2) La précision de 1/256 donne 1234.43359375 de toute façon.

## ex02 — Surcharge des opérateurs

**But de l'exo :** que `Fixed` s'utilise comme un `int` ou un `float` : `a + b`, `a < b`, `++a`. Ça s'appelle la **surcharge d'opérateurs**. `a + b` n'est qu'une écriture courte pour `a.operator+(b)`. Ton ex02 compile et affiche les bons nombres depuis la correction d'aujourd'hui.

### Bloc 1 — Les 6 comparaisons

```cpp
bool Fixed::operator>(const Fixed &other) const {
	return (this->_value > other._value);
}
```

- On compare les `_value` directement. Comme les deux sont × 256, l'ordre reste le même que celui des vrais nombres.
- Retourne `bool` : vrai ou faux.
- `const` à la fin : comparer ne modifie pas l'objet. Obligatoire pour comparer des `Fixed const`, et pour que `min`/`max` en version `const` puissent les appeler.
- `other._value` est accessible : le privé est par classe, pas par objet.
- Les 6 : `>` `<` `>=` `<=` `==` `!=`.

### Bloc 2 — Les 4 opérations

```cpp
Fixed Fixed::operator+(const Fixed& other) const {
	Fixed result;
	result.setRawBits(_value + other._value);
	return result;
}
```

- On crée un **nouvel** objet `result`. `a + b` ne doit modifier ni `a` ni `b` (d'où le `const`).
- `-` : pareil avec `_value - other._value`.
- `*` : `(static_cast<long>(_value) * other._value) >> _bits`. Le `>> 8` retire le 256 en trop. Le `long` évite le débordement : 100.0 × 100.0 donne 25600 × 25600 = 655 360 000, à la limite de l'`int` (2 147 483 647). À 200 × 200, l'`int` déborde.
- `/` : `(_value << _bits) / other._value`. Division par 0 = crash. Le sujet l'accepte.

**Le bug corrigé aujourd'hui.** Ces 4 fonctions retournaient `Fixed&` au lieu de `Fixed` :

1. `result` est une variable **locale**. Elle vit sur la pile de la fonction.
2. À `return`, la fonction se termine et `result` est détruit.
3. Une référence (`Fixed&`) n'est qu'un autre nom pour un objet existant. Ici elle désigne un objet mort.
4. L'appelant lit une case mémoire libérée : comportement indéfini (valeurs fausses, aléatoires ou crash).

**Règle :** on retourne une référence seulement vers un objet qui survit à la fonction (`*this`, un paramètre reçu par référence). Un nouvel objet se retourne **par valeur**.

### Bloc 3 — ++ et -- (le plus piégeux)

```cpp
Fixed& Fixed::operator++() {        // ++a (pré)
	_value++;
	return *this;
}

Fixed Fixed::operator++(int) {      // a++ (post)
	Fixed temp(*this);
	_value++;
	return temp;
}
```

**De combien ça augmente ?** `_value++` ajoute 1 à la valeur brute, donc 1/256 = 0.00390625 au nombre réel. Pas 1. C'est ce que demande le sujet (« le plus petit ε représentable »), et c'est pour ça que la sortie affiche `0.00390625`.

| | `++a` (pré) | `a++` (post) |
| --- | --- | --- |
| Prototype | `Fixed &operator++();` | `Fixed operator++(int);` |
| Ordre | 1. modifie 2. retourne | 1. copie 2. modifie 3. retourne la copie |
| Valeur rendue | Après le changement | Avant le changement |
| Type de retour | Référence : `*this` survit | Valeur : `temp` est local |
| Coût | Aucune copie | Une copie (+ messages Copy) |

**Le `int` du post-incrément** ne sert à rien d'autre qu'à distinguer les deux versions pour le compilateur. On ne lui donne pas de nom et on ne l'utilise pas. C'est une convention du langage.

`--a` et `a--` : la même chose avec `_value--`.

### Bloc 4 — min et max

```cpp
Fixed& Fixed::min(Fixed& a, Fixed& b) { return (a < b) ? a : b; }
const Fixed& Fixed::min(const Fixed& a, const Fixed& b) { return (a < b) ? a : b; }
```

- **`static`** : la fonction appartient à la classe, pas à un objet. On l'appelle `Fixed::max(a, b)`, sans objet devant. Elle n'a pas de `this`.
- **`? :`** = un `if` en une ligne : si `a < b`, renvoie `a`, sinon `b`.
- **Retour par référence** : OK ici, `a` et `b` existent chez l'appelant. On renvoie l'un des deux, pas une copie.
- **Pourquoi deux versions ?** Dans le main, `b` est `Fixed const`. Une référence non-const (`Fixed&`) ne peut pas désigner un objet const : sinon on pourrait le modifier à travers elle. Il faut donc la version `const Fixed&`. La version non-const permet, elle, de modifier le résultat : `Fixed::min(a, c) = 0;`.
- Le compilateur choisit tout seul : si un des deux arguments est `const`, il prend la version `const`.

### La sortie de l'ex02, expliquée

| Ligne du main | Affiche | Pourquoi |
| --- | --- | --- |
| `Fixed const b(Fixed(5.05f) * Fixed(2));` | rien | b = 10.1015625 (calcul détaillé dans « Pourquoi la virgule fixe ») |
| `std::cout << a` | 0 | Constructeur par défaut |
| `std::cout << ++a` | 0.00390625 | Incrémente, puis affiche |
| `std::cout << a` | 0.00390625 | `a` a bien changé |
| `std::cout << a++` | 0.00390625 | Affiche la copie d'avant |
| `std::cout << a` | 0.0078125 | Mais `a` a bien augmenté (2/256) |
| `std::cout << b` | 10.1016 | 6 chiffres significatifs |
| `Fixed::max( a, b )` | 10.1016 | b est le plus grand, version `const` utilisée |

### Les messages « ... called » dans l'ex02

Ton ex02 affiche encore `Default constructor called`, `Copy constructor called`, etc. au milieu des nombres. La sortie du sujet n'en montre aucun. Deux options :

- **Les garder** : la plupart des évaluateurs acceptent, car les nombres sont justes. Sache expliquer chaque message.
- **Les retirer** (dans les constructeurs, le destructeur, `operator=` et `getRawBits`) : la sortie devient identique à celle du sujet. Option la plus sûre.

### Un détail qui diffère de l'ex01

Dans l'ex02, `_bits` est initialisé dans le `.hpp` : `static const int _bits = 8;`. Dans l'ex01, il est défini dans le `.cpp`. Les deux sont valides en C++98 pour une constante entière `static const`. La version `.cpp` est plus sûre si un évaluateur considère que `= 8` dans le `.hpp` est « du code dans le header ».

## ex03 — BSP (bonus, pas encore codé)

**Ton dépôt n'a pas d'ex03.** Le module est validé sans lui. Ne le commence que si ex00, ex01 et ex02 passent tous les trois. Compte environ 2 h.

**But de l'exo :** utiliser ta classe `Fixed` dans un vrai calcul. On te donne un triangle ABC et un point P. Tu dois dire si P est **strictement** à l'intérieur. BSP = Binary Space Partitioning, une technique de découpage de l'espace utilisée dans les moteurs 3D (Doom, Quake).

### La classe Point

```cpp
class Point {
private:
	Fixed const _x;
	Fixed const _y;
public:
	Point();                           // (0, 0)
	Point(const float x, const float y);
	Point(const Point &other);
	Point &operator=(const Point &other);
	~Point();
	Fixed getX() const;
	Fixed getY() const;
};
```

- **Pourquoi `const` ?** Un point géométrique ne bouge pas. Le sujet l'impose.
- **Conséquence 1 :** `_x` et `_y` ne peuvent être remplis QUE dans la liste d'initialisation : `Point::Point(const float x, const float y) : _x(x), _y(y) {}`. Dans le corps `{ }`, il est trop tard : ils existent déjà et sont constants.
- **Conséquence 2 :** `operator=` ne peut rien copier. Il retourne juste `*this` (avec `(void)other;` pour éviter le warning). C'est LA question classique de l'éval pour cet exo.

### La fonction bsp

Prototype imposé : `bool bsp(Point const a, Point const b, Point const c, Point const point);`

**Le principe (produit vectoriel) :** pour chaque côté du triangle, on regarde de quel côté se trouve P (à gauche ou à droite).

```
cross(A, B, P) = (Bx - Ax) * (Py - Ay) - (By - Ay) * (Px - Ax)
```

1. Calcule `d1 = cross(A, B, P)`, `d2 = cross(B, C, P)`, `d3 = cross(C, A, P)`.
2. Un des trois vaut 0 → P est sur un bord ou un sommet → `false`.
3. Les trois ont le même signe (tous > 0 ou tous < 0) → `true`.
4. Sinon → `false`.

**Pourquoi ça marche :** le signe du produit vectoriel dit si P est à gauche (positif) ou à droite (négatif) du segment. Si P est du même côté des trois segments, il est enfermé dedans. Peu importe le sens dans lequel tu donnes A, B, C.

**Pourquoi `Fixed` ici :** avec des floats, un point pile sur un bord peut donner 0.0000001 au lieu de 0, et passer pour « dedans ». Avec `Fixed`, 0 est exactement 0.

### Les 5 tests à écrire

| Test | P pour le triangle (0,0) (10,0) (0,10) | Attendu |
| --- | --- | --- |
| Dedans | (2, 2) | true |
| Dehors | (20, 20) | false |
| Sommet | (0, 0) | false |
| Sur un bord | (5, 0) | false |
| Sommets dans l'autre ordre | (2, 2) avec (0,10) (10,0) (0,0) | true |

## Avant l'éval : pièges, questions, checklist

### Les 5 pièges les plus fréquents

| Symptôme | Cause | Correction |
| --- | --- | --- |
| Valeur bizarre ou crash après `a + b` | Retour `Fixed&` vers une variable locale | Retour par valeur `Fixed` (corrigé dans ton ex02) |
| `c is 42.418` au lieu de `42.4219` | Pas d'arrondi dans le constructeur float | `roundf(f * 256)` ou `+ 0.5f` |
| `c is 42` partout | Division entière dans `toFloat` | Caster en `float` avant de diviser |
| Erreur de compilation avec un `Fixed const` | Méthode sans `const` à la fin | Ajouter `const` (`toFloat() const`, comparaisons…) |
| `undefined reference to Fixed::_bits` | Constante déclarée mais pas définie | `const int Fixed::_bits = 8;` dans le `.cpp` |

### Les questions de l'éval, avec la réponse courte

1. **Pourquoi × 256 ?** 8 bits de partie fractionnaire. 256 = 2^8, donc on multiplie et divise avec des décalages `<<` et `>>`.
2. **Avantage de la virgule fixe sur le float ?** Pas constant, calculs exacts et reproductibles, que des opérations entières. Inconvénient : précision et plage plus petites.
3. **Copie ou affectation ?** Copie : l'objet naît (`Fixed b(a);`, `Fixed b = a;`). Affectation : il existait déjà (`b = a;`).
4. **Pourquoi le constructeur de copie prend une référence ?** Par valeur, il faudrait copier l'argument, donc appeler le constructeur de copie : récursion infinie.
5. **Pourquoi `if (this != &other)` ?** Protège contre `a = a;`. Indispensable quand la classe gère de la mémoire.
6. **Pourquoi `operator=` retourne `*this` ?** Pour enchaîner `a = b = c;`.
7. **Pourquoi `<<` est hors de la classe ?** L'objet de gauche est `std::cout`. On ne peut pas ajouter de méthode à `std::ostream`.
8. **`++a` vs `a++` ?** Pré : modifie puis rend l'objet (référence). Post : copie, modifie, rend la copie (valeur, car la copie est locale).
9. **À quoi sert le `int` de `operator++(int)` ?** À rien, sauf distinguer pré et post pour le compilateur.
10. **Pourquoi `min`/`max` sont `static` ?** Ils ne dépendent d'aucun objet en particulier. On les appelle `Fixed::max(a, b)`.
11. **Pourquoi une version `const` de `min`/`max` ?** Pour accepter des `Fixed const`. Une référence non-const ne peut pas désigner un objet const.
12. **Pourquoi `a + b` retourne par valeur ?** Le résultat est un nouvel objet local. Une référence vers lui pointerait vers un objet détruit.
13. **De combien `++` augmente le nombre ?** De 1/256 = 0.00390625, le plus petit pas représentable.
14. **(ex03) Pourquoi `operator=` de `Point` ne copie rien ?** `_x` et `_y` sont `const` : impossible de les modifier après la construction.

### Les interdits du module

| Interdit | Sanction |
| --- | --- |
| `printf`, `malloc`, `free` | 0 |
| Corps de fonction dans un `.hpp` | 0 à l'exo |
| `.hpp` sans include guard | 0 |
| `using namespace`, `friend` | -42 |
| STL (`vector`, `map`, `<algorithm>`) | -42 |

### Checklist finale

- [ ] ex00 : corriger les 3 erreurs (section ex00), `make re` passe
- [ ] ex00 : la sortie correspond à celle du sujet
- [ ] ex01 : `make re`, sortie conforme (déjà OK le 18/10)
- [ ] ex02 : décider pour les messages « called » (garder ou retirer)
- [ ] ex02 : ajouter des tests pour `-`, `/`, `--`, `min`
- [ ] Supprimer les dossiers `obj/` et les exécutables avant de rendre (`make fclean`)
- [ ] Relire les 14 questions ci-dessus à voix haute, sans notes
- [ ] (bonus) ex03

**Prochaine action (2 min) :** ajoute le `;` ligne 29 de `ex00/Fixed.cpp`.
