# CPP01 — Mémoire, pointeurs, références (préparation à l'éval)

25/10/2026

## Démarrer ici

**Action tout de suite (2 min) :** lance `cd CPP01/ex00 && make && ./zombie`, puis lis la section [ex00](#ex00--zombie-pile-vs-tas) en regardant la sortie à côté.

**Le module en 5 lignes :**

1. ex00 et ex01 : où vit un objet (**pile** ou **tas**), et `new` / `delete`.
2. ex02 : la différence entre un **pointeur** et une **référence**.
3. ex03 : quand utiliser l'un ou l'autre dans une classe (`HumanA` / `HumanB`).
4. ex04 : lire et écrire des **fichiers** en C++ (`ifstream` / `ofstream`).
5. ex05 : les **pointeurs sur fonctions membres** (Harl).

**État de ton code au 25/10/2026** (compilé et testé) :

| Exo | Compile | Sortie OK | À faire |
| --- | --- | --- | --- |
| ex00 | Oui | Oui | Rien |
| ex01 | Oui | Presque | `announce()` doit afficher le nom (5 min, voir [ex01](#ex01--zombiehorde-new--delete)) |
| ex02 | Oui | Oui | Rien |
| ex03 | Oui | Oui | Rien |
| ex04 | Oui | Oui | Rien |
| ex05 | Oui | Oui | Rien |
| ex06 | Absent | — | Facultatif d'après le sujet, vérifie dans ton PDF |

**Comment lire ce doc :**

- Section 1 = rappels de CPP00 (rapide).
- Une section par exo, avec **ton** code.
- La section [Avant l'éval](#avant-léval--questions-pièges-checklist) = questions + réponses courtes. Peu de temps ? Lis seulement celle-là.
- Le [lexique](#lexique--tous-les-mots-en-une-ligne) à la fin donne chaque mot en une phrase.

Lis une section, ferme, reviens plus tard.

## Sommaire

1. [Rappels de CPP00](#1-rappels-de-cpp00)
2. [ex00 — Zombie (pile vs tas)](#ex00--zombie-pile-vs-tas)
3. [ex01 — zombieHorde (new[] / delete[])](#ex01--zombiehorde-new--delete)
4. [ex02 — HI THIS IS BRAIN (pointeur vs référence)](#ex02--hi-this-is-brain-pointeur-vs-référence)
5. [ex03 — HumanA / HumanB](#ex03--humana--humanb)
6. [ex04 — Sed is for losers (fichiers)](#ex04--sed-is-for-losers-fichiers)
7. [ex05 — Harl 2.0 (pointeurs sur fonctions membres)](#ex05--harl-20-pointeurs-sur-fonctions-membres)
8. [ex06 — Harl filter (pas codé)](#ex06--harl-filter-pas-codé)
9. [Avant l'éval](#avant-léval--questions-pièges-checklist)
10. [Lexique](#lexique--tous-les-mots-en-une-ligne)

---

## 1. Rappels de CPP00

Le correcteur peut te poser ces questions même en CPP01.

- **Classe** = le plan. **Objet** = une chose construite avec ce plan. `Zombie z("Stan");` crée un objet.
- **`.hpp`** = les déclarations (ce qui existe). **`.cpp`** = le code (comment ça marche).
- **Include guard** (`#ifndef ZOMBIE_HPP` / `#define` / `#endif`) = empêche d'inclure deux fois le même header.
- **`private`** = utilisable seulement par la classe. **`public`** = utilisable par tout le monde.
- **Constructeur** = appelé à la naissance de l'objet. **Destructeur** (`~Zombie()`) = appelé à sa mort.

**Liste d'initialisation** = la partie après `:` dans un constructeur. Tu l'utilises partout :

```cpp
Zombie::Zombie(std::string name) : name(name) {
}
```

Elle initialise les attributs **avant** d'entrer dans `{ }`. Pour une **référence** (ex03, `HumanA`), c'est **obligatoire**.

---

## ex00 — Zombie (pile vs tas)

### Ce que le sujet demande

- Une classe `Zombie` avec un nom et `announce()` : affiche `<name>: BraiiiiiiinnnzzzZ...`
- `Zombie* newZombie(std::string name);` : crée un zombie et le **renvoie** pour l'utiliser ailleurs.
- `void randomChump(std::string name);` : crée un zombie qui s'annonce, puis meurt.
- Un destructeur qui affiche un message, pour **voir** quand chaque zombie meurt.

### La notion : pile (stack) et tas (heap)

| | Pile (stack) | Tas (heap) |
| --- | --- | --- |
| Comment | `Zombie z("Kyle");` | `Zombie *z = new Zombie("Stan");` |
| Durée de vie | Jusqu'à la fin du bloc `{ }` | Jusqu'au `delete` |
| Qui libère ? | Automatique | **Toi**, avec `delete` |
| Si tu oublies | Rien | **Fuite mémoire** (leak) |
| Ton code | `randomChump` | `newZombie` |

### Ton code

`newZombie.cpp` : le zombie doit survivre après la fonction, donc **tas** :

```cpp
Zombie* newZombie(std::string name) {
    return new Zombie(name);
}
```

`randomChump.cpp` : le zombie ne sert que dans la fonction, donc **pile** :

```cpp
void randomChump(std::string name) {
    Zombie zombie(name);
    zombie.announce();
}   // <- zombie meurt ici, le destructeur s'affiche
```

### La sortie, expliquée

```
Creating stack zombie...
TemporaryZombie: BraiiiiiiinnnzzzZ...
TemporaryZombie is destroyed      <- fin de randomChump : mort automatique
Heap zombie still alive:
Survivor: BraiiiiiiinnnzzzZ...    <- toujours vivant : il est sur le tas
Cleaning up heap zombie...
Survivor is destroyed             <- seulement quand tu fais delete
```

### Question type de l'éval

**« Pourquoi ne pas tout mettre sur le tas ? »** La pile est plus rapide et se libère seule. Le tas sert seulement quand l'objet doit vivre plus longtemps que la fonction qui le crée.

**« Pourquoi `newZombie` ne peut pas renvoyer un zombie de la pile ? »** Il serait détruit à la fin de la fonction. Le pointeur renvoyé viserait de la mémoire morte (*dangling pointer*).

---

## ex01 — zombieHorde (new[] / delete[])

### Ce que le sujet demande

- `Zombie* zombieHorde(int N, std::string name);`
- Créer **N zombies en une seule allocation**, leur donner à tous le nom `name`, renvoyer le premier.
- Le main appelle `announce()` sur chacun, puis libère tout.

### La notion : `new[]` et `delete[]`

| Tu alloues avec | Tu libères avec |
| --- | --- |
| `new Zombie(...)` | `delete z;` |
| `new Zombie[N]` | `delete[] horde;` |

Mélanger les deux (`new[]` puis `delete`) = **comportement indéfini**. Seul le premier destructeur peut être appelé, ou le programme plante.

**Contrainte importante :** `new Zombie[N]` ne peut appeler que le **constructeur par défaut**. Impossible de passer un nom. C'est pour ça que tu as ajouté :

- `Zombie();` (constructeur par défaut)
- `setName()` pour donner le nom après coup.

### Ton code

```cpp
Zombie* zombieHorde(int N, std::string name)
{
	if (N <= 0)
		return NULL;
	Zombie* horde = new Zombie[N];      // 1 seule allocation
	for (int i = 0; i < N; i++)
		horde[i].setName(name);
	return horde;
}
```

Dans le main : `delete[] horde;` → les N destructeurs s'affichent (`Randy is destroyed` × N).

### À corriger avant l'éval (5 min)

Ton `announce()` d'ex01 n'affiche pas le nom :

```cpp
std::cout << "BraiiiiiiinnnzzzZ..." << std::endl;
```

Le sujet veut `<name>: BraiiiiiiinnnzzzZ...`, comme en ex00. Aujourd'hui c'est ton main qui affiche `Zombie Randy1: `, donc la sortie *semble* juste, mais le zombie ne s'annonce pas lui-même. Un correcteur peut le relever.

1. Dans `ex01/Zombie.cpp`, remplace la ligne par `std::cout << name << ": BraiiiiiiinnnzzzZ..." << std::endl;`
2. Dans `ex01/main.cpp`, supprime la ligne `std::cout << "Zombie " << name << i + 1 << ": ";`

### Tests à montrer

- `3` → 3 annonces, puis 3 destructeurs.
- `0` ou `abc` → `No zombies were created.` (testé : marche).
- Avec valgrind (sur les Mac de l'école) : `valgrind ./horde` → `0 bytes lost`.

---

## ex02 — HI THIS IS BRAIN (pointeur vs référence)

### Ce que le sujet demande

Une string `brain`, un pointeur `stringPTR` dessus, une référence `stringREF` dessus. Afficher les 3 adresses, puis les 3 valeurs.

### Ta sortie

```
Address of brain: 0x7ffd34109ab0
Address held by stringPTR: 0x7ffd34109ab0
Address held by stringREF: 0x7ffd34109ab0
Value of brain: HI THIS IS BRAIN
...
```

**Les 3 adresses sont identiques.** C'est le but de l'exo : le pointeur et la référence visent la même string.

### La notion : pointeur vs référence

| | Pointeur `std::string *p` | Référence `std::string &r` |
| --- | --- | --- |
| C'est quoi | Une variable qui **contient une adresse** | Un **autre nom** pour la même variable |
| Peut être `NULL` | Oui | Non |
| Peut changer de cible | Oui (`p = &autre;`) | Non, jamais |
| Initialisation | Plus tard possible | **Obligatoire** à la déclaration |
| Lire la valeur | `*p` | `r` (direct) |

Image : le pointeur est un **papier avec une adresse écrite dessus**. La référence est un **surnom** de la personne.

### Question type de l'éval

**« Pourquoi `&stringREF` donne l'adresse de `brain` ? »** Parce que `stringREF` *est* `brain` sous un autre nom. Elle n'a pas d'adresse à elle.

---

## ex03 — HumanA / HumanB

### Ce que le sujet demande

- `Weapon` : un `type`, `getType()` (renvoie une **référence const**) et `setType()`.
- `HumanA` : reçoit son arme **dans le constructeur**. Il est **toujours** armé.
- `HumanB` : n'a **pas forcément** d'arme. Il la reçoit plus tard avec `setWeapon()`.
- `attack()` affiche `<name> attacks with their <weapon type>`.

### La notion : référence ou pointeur dans une classe ?

C'est **LA** question de l'éval sur cet exo.

| | HumanA | HumanB |
| --- | --- | --- |
| Attribut | `Weapon& weapon;` | `Weapon* weapon;` |
| Pourquoi | Toujours une arme, jamais changée | Peut ne pas avoir d'arme (`NULL`) |
| Init | Liste d'initialisation, obligatoire | `NULL` au départ, puis `setWeapon` |

Règle à dire au correcteur : **référence si ça existe toujours et ne change jamais. Pointeur si ça peut être absent ou changer.**

### Ton code

`HumanA` : la référence **doit** être initialisée dans la liste d'initialisation. Dans `{ }`, ce serait trop tard :

```cpp
HumanA::HumanA(const std::string& name, Weapon& weapon) : name(name), weapon(weapon) {
}
```

`HumanB` : le pointeur commence à `NULL`, et `attack()` vérifie avant d'utiliser :

```cpp
HumanB::HumanB(const std::string& name) : name(name), weapon(NULL) {
}

void HumanB::attack() const {
    if (weapon)
        std::cout << name << " attacks with their " << weapon->getType() << std::endl;
    else
        std::cout << name << " has no weapon to attack with" << std::endl;
}
```

Tu as ajouté `dropWeapon()` en plus : c'est permis, et ça montre bien l'intérêt du pointeur.

### Pourquoi le type change dans la sortie

```
Bob attacks with their crude spiked club
Bob attacks with their some other type of club
```

Bob ne garde **pas une copie** de l'arme : il garde une **référence vers `club`**. Quand le main fait `club.setType(...)`, Bob voit le changement. Si `weapon` était un `Weapon` tout court (une copie), la 2e ligne afficherait encore `crude spiked club`.

### Le `const` dans `getType()`

```cpp
const std::string& getType() const;
```

1. **Premier `const`** (avant) : on renvoie une référence, mais on ne peut pas modifier le type à travers elle.
2. **Dernier `const`** (après) : la méthode ne modifie pas l'objet. Obligatoire pour l'appeler dans `attack() const`.
3. **Le `&`** : pas de copie de la string, plus rapide.

---

## ex04 — Sed is for losers (fichiers)

### Ce que le sujet demande

- `./replace <filename> <s1> <s2>`
- Copier le fichier dans `<filename>.replace`, en remplaçant **chaque** `s1` par `s2`.
- **Interdit :** `std::string::replace`. Interdit aussi : les fonctions fichier du C (`fopen`…).
- Gérer les erreurs (mauvais arguments, fichier introuvable).

### Les outils C++ utilisés

| Outil | Header | Ce qu'il fait dans ton code |
| --- | --- | --- |
| `std::ifstream` | `<fstream>` | Ouvre le fichier en lecture |
| `std::ofstream` | `<fstream>` | Crée `<filename>.replace` |
| `std::stringstream` | `<sstream>` | Reçoit tout le fichier d'un coup (`buffer << infile.rdbuf();`) |
| `find(s1, pos)` | `<string>` | Cherche `s1` à partir de `pos`, renvoie `npos` si absent |
| `substr(début, longueur)` | `<string>` | Copie un morceau de la string |

`.c_str()` sur le nom de fichier : en C++98, `ifstream` ne prend pas de `std::string`, seulement un `const char*`.

### Ton algorithme (`replaceAll`)

1. Cherche `s1` à partir de `pos`.
2. Copie tout ce qu'il y a **avant**, puis `s2` à la place de `s1`.
3. Avance `pos` **juste après** le `s1` trouvé.
4. Recommence jusqu'à `npos`, puis copie la fin.

L'étape 3 évite une boucle infinie si `s2` contient `s1` (exemple : remplacer `a` par `aa`).

### Tests à montrer (tous testés, ils marchent)

| Commande | Résultat |
| --- | --- |
| `./replace t.txt aa X` sur `aaa hello aa` | `Xa hello X` |
| `./replace t.txt "" X` | `Error: s1 must not be empty` |
| `./replace nope a b` | `Error: cannot open nope` |
| `./replace t.txt` (2 arguments) | Message d'usage |

**Pourquoi refuser `s1` vide ?** `find("")` trouve une chaîne vide à chaque position : la boucle ne s'arrêterait jamais.

---

## ex05 — Harl 2.0 (pointeurs sur fonctions membres)

### Ce que le sujet demande

- Une classe `Harl` avec 4 fonctions **privées** : `debug`, `info`, `warning`, `error`.
- Une fonction publique `complain(std::string level)` qui appelle la bonne.
- **Sans** une forêt de `if / else if / else`. Il faut des **pointeurs sur fonctions membres**.

### La notion : pointeur sur fonction membre

Une fonction a une adresse, comme une variable. On peut la ranger dans un tableau.

```cpp
void (Harl::*functions[])() = {&Harl::debug, &Harl::info, &Harl::warning, &Harl::error};
```

Lis-le en 4 morceaux :

1. `void` → les fonctions renvoient `void`.
2. `(Harl::*functions[])` → un tableau de pointeurs vers des fonctions **de Harl**.
3. `()` → les fonctions ne prennent aucun paramètre.
4. `&Harl::debug` → l'adresse de la fonction `debug` de la classe.

### L'appel

```cpp
(this->*functions[i])();
```

Une fonction membre a besoin d'un **objet** pour être appelée. `this->*` veut dire « appelle-la sur l'objet actuel ». Les parenthèses autour sont obligatoires.

### Ton algorithme

1. Un tableau `levels` : `"DEBUG"`, `"INFO"`, `"WARNING"`, `"ERROR"`.
2. Un tableau `functions` : les 4 fonctions, **dans le même ordre**.
3. Boucle : si `levels[i] == level`, appelle `functions[i]` et `return`.
4. Rien trouvé → `Unknown level: ...`.

### Question type de l'éval

**« Pourquoi les 4 fonctions sont privées ? »** Le sujet l'exige. Seul `complain` doit être utilisé de l'extérieur. C'est l'**encapsulation**.

---

## ex06 — Harl filter (pas codé)

Pas dans ton dépôt. D'après le sujet de CPP01, tu valides le module sans ex06. **Vérifie cette phrase dans ton PDF.**

Si un correcteur en parle, voici l'idée en 3 lignes :

1. `./harlFilter WARNING` affiche WARNING **et tous les niveaux au-dessus** (ERROR).
2. On utilise un `switch` **sans `break`** : le programme « tombe » dans les cas suivants (*fallthrough*).
3. Niveau inconnu → `[ Probably complaining about insignificant problems ]`.

---

## Avant l'éval : questions, pièges, checklist

### Les questions de l'éval, avec la réponse courte

| Question | Réponse courte |
| --- | --- |
| Pile ou tas, la différence ? | Pile = libérée seule à la fin du bloc. Tas = vit jusqu'au `delete`. |
| Quand utiliser le tas ? | Quand l'objet doit survivre à la fonction qui le crée (`newZombie`). |
| `delete` ou `delete[]` ? | `delete[]` pour ce qui vient de `new[]`. Sinon, comportement indéfini. |
| Pourquoi un constructeur par défaut en ex01 ? | `new Zombie[N]` ne peut appeler que lui. |
| Pointeur ou référence ? | Référence = toujours valide, ne change pas. Pointeur = peut être `NULL` ou changer. |
| Pourquoi HumanA a une référence ? | Il a toujours une arme, reçue à la construction. |
| Pourquoi HumanB a un pointeur ? | Il peut ne pas avoir d'arme : `NULL` au départ. |
| Pourquoi la liste d'initialisation dans HumanA ? | Une référence doit être initialisée à sa création. Dans `{ }` c'est trop tard. |
| Pourquoi `.c_str()` en ex04 ? | En C++98, `ifstream` veut un `const char*`. |
| C'est quoi `this->*` ? | Appeler une fonction membre via un pointeur, sur l'objet actuel. |

### Les 5 pièges les plus fréquents

1. **`new[]` libéré avec `delete`** au lieu de `delete[]`.
2. **Renvoyer l'adresse d'une variable locale** (de la pile) : elle meurt à la fin de la fonction.
3. **Référence non initialisée** : erreur de compilation. Elle va dans la liste d'initialisation.
4. **`find` sans avancer `pos`** en ex04 : boucle infinie si `s2` contient `s1`.
5. **Oublier de vérifier `NULL`** avant `weapon->` dans HumanB : crash.

### Tests à faire devant le correcteur (5 min)

1. `cd ex00 && make && ./zombie` → montre l'ordre des destructeurs (pile vs tas).
2. `cd ex01 && make && valgrind ./horde` (sur Mac : `leaks -atExit -- ./horde`) → 0 fuite.
3. `cd ex03 && make && ./violence` → le type d'arme change pour Bob et Jim.
4. `cd ex04 && make && ./replace` avec un `s1` vide et un fichier introuvable.
5. `cd ex05 && make && ./harl WARNING` puis `./harl n_importe_quoi`.

### Les interdits du module

- `printf`, `malloc`, `free` → 0.
- `using namespace` et `friend` → -42.
- STL (`vector`, `map`, `<algorithm>`) → -42.
- ex04 : `std::string::replace` et les fonctions fichier du C.
- Du code de fonction dans un `.hpp` → 0.

### Checklist finale

- [ ] ex01 : `announce()` affiche le nom (voir [la correction](#à-corriger-avant-léval-5-min))
- [ ] `make re` dans chaque dossier, sans warning
- [ ] `make fclean` partout avant de pousser (pas de `obj/` ni d'exécutable dans le dépôt)
- [ ] Tu sais répondre au tableau de questions **sans regarder**
- [ ] Tu sais dessiner pile vs tas sur une feuille

---

## Lexique : tous les mots en une ligne

| Mot | En une ligne |
| --- | --- |
| Pile (stack) | Mémoire automatique, libérée à la fin du bloc `{ }`. |
| Tas (heap) | Mémoire que tu demandes avec `new` et rends avec `delete`. |
| `new` / `delete` | Créer / détruire un objet sur le tas. |
| `new[]` / `delete[]` | Pareil, pour un tableau d'objets. |
| Fuite mémoire (leak) | Mémoire du tas jamais libérée. |
| Dangling pointer | Pointeur vers de la mémoire déjà libérée ou morte. |
| Pointeur | Variable qui contient une adresse. Peut être `NULL`. |
| Référence | Autre nom pour une variable existante. Jamais `NULL`, jamais changée. |
| `&` dans un type | Déclare une référence (`Weapon& w`). |
| `&` devant une variable | Donne son adresse (`&brain`). |
| `*` devant un pointeur | Donne la valeur pointée (`*stringPTR`). |
| `->` | Accès à un membre via un pointeur (`weapon->getType()`). |
| Liste d'initialisation | Le `: a(x), b(y)` du constructeur, avant `{ }`. |
| `const` après une méthode | La méthode ne modifie pas l'objet. |
| `ifstream` / `ofstream` | Fichier en lecture / en écriture. |
| `npos` | Valeur renvoyée par `find` quand rien n'est trouvé. |
| Pointeur sur fonction membre | Adresse d'une méthode, appelée avec `(this->*ptr)()`. |
| `switch` fallthrough | Un `case` sans `break` continue dans le suivant (ex06). |
