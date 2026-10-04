# Guide C++ pour débutant — CPP00 et CPP01

Ce guide reprend **ton propre code**, exercice par exercice. Pour chaque exercice :

1. **Ce que fait le programme**
2. **Les notions de C++ utilisées** (expliquées depuis zéro)
3. **Le code ligne par ligne** : pourquoi il est écrit comme ça
4. **Les pièges et les améliorations possibles**
5. **À toi de jouer** : un petit exercice pour vérifier que tu sais l'écrire seul

> Tout est compilé en **C++98** (`-std=c++98`), c'est imposé par 42. Donc pas de `auto`,
> pas de `nullptr`, pas de `std::to_string`, etc. On utilise `NULL` et les outils
> « à l'ancienne ».

---

## Sommaire

- [0. Les bases avant de commencer](#0-les-bases-avant-de-commencer)
  - [Le Makefile](#le-makefile)
  - [Du C au C++ : ce qui change](#du-c-au-c--ce-qui-change)
- [CPP00 ex00 — Megaphone](#cpp00-ex00--megaphone)
- [CPP00 ex01 — PhoneBook](#cpp00-ex01--phonebook)
- [CPP00 ex02 — Account (pas encore fait)](#cpp00-ex02--account-pas-encore-fait)
- [CPP01 ex00 — Zombie (stack vs heap)](#cpp01-ex00--zombie-stack-vs-heap)
- [CPP01 ex01 — Zombie Horde (new[] / delete[])](#cpp01-ex01--zombie-horde-new--delete)
- [CPP01 ex02 — HI THIS IS BRAIN (pointeurs vs références)](#cpp01-ex02--hi-this-is-brain-pointeurs-vs-références)
- [CPP01 ex03 — Unnecessary violence (HumanA / HumanB)](#cpp01-ex03--unnecessary-violence-humana--humanb)
- [CPP01 ex04 — Sed is for losers (fichiers)](#cpp01-ex04--sed-is-for-losers-fichiers)
- [CPP01 ex05 — Harl 2.0 (pointeurs sur fonctions membres)](#cpp01-ex05--harl-20-pointeurs-sur-fonctions-membres)
- [CPP01 ex06 — Harl filter (pas encore fait)](#cpp01-ex06--harl-filter-pas-encore-fait)
- [Récap : la boîte à outils du débutant](#récap--la-boîte-à-outils-du-débutant)

---

## 0. Les bases avant de commencer

### Le Makefile

Tous tes exercices ont le même Makefile. Voici celui de `CPP01/ex05` commenté :

```makefile
NAME = harl                               # nom de l'exécutable final
CXX = c++                                 # le compilateur C++ (équivalent de "cc" en C)
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
#           ^ warnings  ^ + de warnings  ^ warning = erreur  ^ norme C++98

OBJ_DIR = obj                             # les .o vont dans un dossier obj/
SRCS = main.cpp Harl.cpp                  # tes fichiers sources
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))
#      main.cpp Harl.cpp  ->  main.o Harl.o  ->  obj/main.o obj/Harl.o

all: $(NAME)

$(NAME): $(OBJS)                          # pour faire "harl", il faut tous les .o
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)  # on les "lie" (link) ensemble

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)        # règle générique : obj/X.o vient de X.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@         # -c = compiler sans linker
	#                      $< = le .cpp   $@ = le .o

$(OBJ_DIR):                               # créer le dossier obj/ si besoin
	@mkdir -p $(OBJ_DIR)
# Le "|" avant $(OBJ_DIR) veut dire : "le dossier doit exister,
# mais ne recompile pas tout juste parce que sa date a changé".

clean:  ; @rm -rf $(OBJ_DIR)              # supprime les .o
fclean: clean ; @rm -f $(NAME)            # supprime aussi l'exécutable
re: fclean all                            # tout refaire
.PHONY: all clean fclean re               # ces règles ne sont pas des fichiers
```

**Pourquoi compiler en deux étapes (`.cpp → .o → exécutable`) ?**
Si tu modifies seulement `Harl.cpp`, `make` ne recompile que `Harl.o` puis refait le lien.
Sur un gros projet, ça fait gagner énormément de temps.

### Du C au C++ : ce qui change

| En C | En C++ | Pourquoi |
|------|--------|----------|
| `#include <stdio.h>` | `#include <iostream>` | Les flux (`std::cout`, `std::cin`) remplacent `printf`/`scanf` |
| `printf("%s\n", s);` | `std::cout << s << std::endl;` | Pas besoin de `%s`, `%d` : le type est détecté automatiquement |
| `char *s` + `strlen`, `strcpy`... | `std::string s;` | La mémoire est gérée toute seule, on peut faire `s1 + s2`, `s == "ADD"`... |
| `malloc` / `free` | `new` / `delete` | `new` appelle le **constructeur**, `delete` appelle le **destructeur** |
| `struct` avec des données | `class` avec données **et** fonctions | On regroupe les données et ce qu'on peut faire avec |
| `#include <ctype.h>` | `#include <cctype>` | Les en-têtes C existent en version C++ : `<cXXX>` |

**`std::` c'est quoi ?** C'est un *namespace* (espace de noms). Toute la bibliothèque standard
est rangée dans la « boîte » `std`. `std::cout` veut dire « le `cout` qui est dans `std` ».
À 42, `using namespace std;` est interdit : c'est pour ça que tu écris `std::` partout.

**`<<` et `>>`**, ce sont les opérateurs de flux :
- `std::cout << x` : « envoie `x` vers la sortie » (comme une flèche qui va vers `cout`)
- `std::cin >> x` : « lis depuis l'entrée et range dans `x` »
- `std::endl` : saut de ligne **+ vidage du buffer** (le texte s'affiche tout de suite)

---

## CPP00 ex00 — Megaphone

### Ce que fait le programme

```
$ ./megaphone "shhhhh... I think the students are asleep..."
SHHHHH... I THINK THE STUDENTS ARE ASLEEP...
$ ./megaphone
* LOUD AND UNBEARABLE FEEDBACK NOISE *
```

Il affiche tous les arguments en MAJUSCULES, collés les uns aux autres.

### Notions utilisées

- `std::cout` et `std::endl` (voir plus haut)
- `main(int ac, char **av)` : exactement comme en C. `ac` = nombre d'arguments
  (le nom du programme compte pour 1), `av` = tableau de chaînes.
- `toupper()` : fonction C qui transforme une lettre minuscule en majuscule.
- Le **cast** `(char)` : `toupper` renvoie un `int`. Si tu fais `std::cout << 65`,
  ça affiche `65`. Si tu fais `std::cout << (char)65`, ça affiche `A`.
  C'est pour ça que le cast est **indispensable** ici.

### Le code expliqué

```cpp
#include <iostream>                 // pour std::cout et std::endl

int main(int ac, char **av)
{
    int i = 1;                      // on commence à 1 : av[0] est "./megaphone"
    int y = 0;                      // index du caractère dans l'argument courant

    if(ac == 1)                     // aucun argument (juste le nom du programme)
        std::cout << "* LOUD AND UNBEARABLE FEEDBACK NOISE *";
    else
    {
        while(i < ac)               // pour chaque argument...
        {
            while(av[i][y] != '\0') // ...pour chaque caractère de l'argument
            {
                std::cout << (char)toupper(av[i][y]);  // affiche en majuscule
                y++;
            }
            y = 0;                  // on remet y à 0 pour l'argument suivant
            i++;
        }
    }
    std::cout << std::endl;         // un seul saut de ligne à la fin, dans les 2 cas
}
```

**Pourquoi `std::endl` est en dehors du `if/else` ?** Parce que les deux cas doivent
finir par un retour à la ligne : on évite de l'écrire deux fois.

### Pièges et améliorations

- Il manque `#include <cctype>` pour `toupper`. Ça compile parce que `<iostream>`
  l'inclut « par hasard » sur ton système, mais ce n'est pas garanti ailleurs.
- `toupper` attend une valeur positive. Un `char` avec un accent (`é`) peut être négatif
  → comportement indéfini. La version propre : `toupper((unsigned char)av[i][y])`.
- Version plus « C++ » possible avec `std::string` :

```cpp
for (int i = 1; i < ac; i++)
{
    std::string arg = av[i];                       // char* -> std::string
    for (size_t j = 0; j < arg.length(); j++)      // .length() au lieu de '\0'
        std::cout << (char)std::toupper((unsigned char)arg[j]);
}
```

### À toi de jouer

Écris `./reverse` qui affiche chaque argument **à l'envers**, séparés par un espace.
(Indice : `std::string`, `.length()`, une boucle qui part de la fin.)

---

## CPP00 ex01 — PhoneBook

C'est **le** premier vrai exercice de programmation orientée objet. Prends le temps de bien
le comprendre, tout le reste en découle.

### Ce que fait le programme

Un répertoire de **8 contacts maximum**. Commandes :
- `ADD` : demande les 5 champs et ajoute un contact (le 9ᵉ remplace le plus ancien)
- `SEARCH` : affiche un tableau résumé, puis le détail d'un contact choisi
- `EXIT` : quitte

### Notions utilisées

#### 1. La classe

Une **classe** est un « plan de construction ». Un **objet** est une chose construite
à partir de ce plan.

```cpp
class Contact { ... };   // le plan
Contact c;               // un objet construit à partir du plan
```

Une classe contient :
- des **attributs** (les données) : `_fname`, `_phone`...
- des **méthodes** (les fonctions qui agissent sur ces données) : `getfName()`, `setPhone()`...

#### 2. `public` / `private` — l'encapsulation

- `private` : accessible **uniquement** depuis les méthodes de la classe.
- `public` : accessible depuis n'importe où.

**Pourquoi tout mettre en `private` ?** Pour protéger les données. Personne de l'extérieur
ne peut écrire `contact._phone = "n'importe quoi"`. Il doit passer par `setPhone()`,
où tu pourrais par exemple vérifier que ce sont bien des chiffres.
C'est le principe d'**encapsulation**.

Le `_` devant les noms (`_fname`) est une **convention** pour repérer d'un coup d'œil
les attributs privés. Ce n'est pas obligatoire.

#### 3. Getters et setters

- **getter** (`getfName`) : renvoie la valeur d'un attribut privé → *lire*
- **setter** (`setfName`) : modifie un attribut privé → *écrire*

#### 4. Constructeur et destructeur

```cpp
Contact(void);    // constructeur : même nom que la classe, pas de type de retour
~Contact(void);   // destructeur : ~ + nom de la classe
```

- Le **constructeur** est appelé **automatiquement** quand l'objet est créé.
  Il sert à initialiser les attributs.
- Le **destructeur** est appelé **automatiquement** quand l'objet est détruit
  (fin du bloc `{}`, ou `delete`). Il sert à libérer des ressources.

#### 5. `this`

Dans une méthode, `this` est un **pointeur vers l'objet sur lequel on a appelé la méthode**.
`this->_count = 0;` veut dire « le `_count` de **cet** objet ». On peut aussi écrire juste
`_count = 0;`, c'est pareil. `this->` devient **indispensable** quand un paramètre a le même
nom qu'un attribut (on le verra dans CPP01).

#### 6. `const` après une méthode

```cpp
std::string getfName(void) const;
```

Le `const` à la fin est une promesse : « cette méthode **ne modifie pas** l'objet ».
Le compilateur vérifie : si tu essaies de modifier un attribut dedans, erreur.
Règle simple : **tous les getters doivent être `const`.**

#### 7. `.hpp` et `.cpp`

- Le `.hpp` (header) contient la **déclaration** : « voici à quoi ressemble la classe ».
- Le `.cpp` contient la **définition** : « voici le code de chaque méthode ».

Dans le `.cpp`, on écrit `Contact::getfName` : le `::` veut dire « le `getfName` qui
appartient à la classe `Contact` ».

#### 8. Les *include guards*

```cpp
#ifndef CONTACT_HPP
# define CONTACT_HPP
...
#endif
```

Si `contact.hpp` est inclus deux fois (par `main.cpp` via `Phonebook.hpp` **et** directement),
sans ces lignes la classe serait déclarée deux fois → erreur. Le guard dit :
« si `CONTACT_HPP` n'est pas encore défini, définis-le et lis le fichier ; sinon, saute tout ».

### Le code expliqué

#### `contact.hpp`

```cpp
#ifndef CONTACT_HPP
# define CONTACT_HPP
#include <string>                       // on utilise std::string dans ce fichier

class Contact
{
public:
    void setfName(std::string fname);   // setters : écrire
    ...
    std::string getfName(void) const;   // getters : lire, const = ne modifie rien
    ...
    bool isEmpty();
    Contact(void);                      // constructeur
    ~Contact(void);                     // destructeur
private:
    std::string _fname;                 // les données, inaccessibles de l'extérieur
    std::string _lname;
    std::string _nick;
    std::string _phone;
    std::string _secret;
};                                      // ⚠️ le ; après la classe est obligatoire !
#endif
```

#### `contact.cpp`

```cpp
Contact::Contact(void) {}               // rien à faire : les std::string sont
                                        // automatiquement initialisées à ""

std::string Contact::getfName(void) const
{
    return (this->_fname);              // renvoie une copie du prénom
}

void Contact::setfName(std::string fname)
{
    this->_fname = fname;               // on stocke la valeur reçue
}
```

#### `Phonebook.hpp`

```cpp
class PhoneBook {
public:
    void addContact(void);
    void searchContact(void);
    std::string GetInput(std::string prompt);
    PhoneBook(void);
    ~PhoneBook(void);
private:
    Contact _contacts[8];   // un tableau FIXE de 8 contacts (le sujet interdit
                            // l'allocation dynamique ici)
    int _count;             // combien de contacts sont remplis (0 à 8)
    int _index;             // où écrire le prochain contact (0 à 7)
    std::string truncate(std::string str);  // privé : outil interne
};
```

**Pourquoi deux variables `_count` et `_index` ?**
- `_count` sert à savoir **combien** de contacts afficher dans `SEARCH`.
- `_index` sert à savoir **où** écrire le prochain. Une fois arrivé à 8, il revient à 0
  et écrase le plus ancien.

**Pourquoi `truncate` est privée ?** Parce que c'est un détail interne de l'affichage.
Le `main` n'a pas besoin de la connaître.

#### `Phonebook.cpp`

**Le constructeur** : contrairement aux `std::string`, les `int` ne sont **pas**
initialisés automatiquement (ils contiennent n'importe quoi). Il faut le faire :

```cpp
PhoneBook::PhoneBook(void)
{
    this->_count = 0;
    this->_index = 0;
}
```

**`GetInput`** : demande une saisie non vide.

```cpp
std::string PhoneBook::GetInput(std::string prompt)
{
    std::string input;
    while (true) {
        std::cout << prompt;
        if (!std::getline(std::cin, input))  // lit une LIGNE entière (espaces compris)
            return "";                       // échec = Ctrl+D (EOF)
        if (!input.empty())
            return input;                    // OK, on renvoie la saisie
        std::cout << "Field cannot be empty! Please try again." << std::endl;
    }
}
```

**Pourquoi `std::getline` et pas `std::cin >> input` ?**
`std::cin >>` s'arrête au premier espace. Si on tape `Jean Pierre`, il ne lirait que `Jean`.
`getline` lit toute la ligne jusqu'à `Entrée`.

**Pourquoi `if (!std::getline(...))` ?** `getline` renvoie le flux, et un flux peut se tester
comme un booléen : `false` si la lecture a échoué (fin de fichier = Ctrl+D). Sans ce test,
Ctrl+D ferait une boucle infinie.

**`addContact`** :

```cpp
void PhoneBook::addContact()
{
    Contact newContact;                          // objet temporaire
    std::string fname = GetInput("Enter first name: ");
    ...
    newContact.setfName(fname);                  // on remplit via les setters
    ...
    _contacts[_index] = newContact;              // copie dans le tableau
    if(_count < 8)
        _count++;                                // on ne dépasse jamais 8
    _index = (_index + 1) % 8;                   // 0,1,...,7,0,1,... (tableau circulaire)
    std::cout << "Contact added" << std::endl;
}
```

**Le `% 8`** : le modulo donne le reste de la division. `7 + 1 = 8`, `8 % 8 = 0`.
Donc après la case 7, on revient à la case 0 → le plus ancien est remplacé.

**`truncate`** :

```cpp
if (str.length() > 10)
    return (str.substr(0, 9) + ".");   // 9 premiers caractères + "." = 10 caractères
return (str);
```

`substr(début, longueur)` extrait un morceau de chaîne. Et on peut **additionner** des
`std::string` avec `+` : c'est un énorme avantage par rapport aux `char*` du C.

**`searchContact`** : l'affichage en colonnes.

```cpp
std::cout << std::setw(10) << "index" << "|" ...
```

`std::setw(10)` (de `<iomanip>`) dit : « le **prochain** élément affiché prendra
10 caractères, aligné à droite ». ⚠️ Il ne s'applique qu'au prochain élément, c'est pour ça
qu'on le répète avant chaque colonne.

La vérification de l'index :

```cpp
std::string input = GetInput("Enter index: ");
if (input.length() != 1 || !isdigit(input[0]) || input[0] == '0')  // 1 seul chiffre, pas 0
    ...
int index = input[0] - '0';          // '3' - '0' = 3 (astuce ASCII, comme en C)
if (index < 1 || index > _count)     // doit correspondre à un contact existant
    ...
Contact contact = _contacts[index - 1];  // -1 car l'utilisateur compte à partir de 1
```

**Pourquoi lire l'index en `std::string` et pas en `int` ?** Parce que si l'utilisateur
tape `abc` dans un `std::cin >> int`, le flux passe en erreur et tout se bloque.
Lire une chaîne puis la vérifier est beaucoup plus sûr.

#### `main.cpp`

```cpp
PhoneBook phoneBook;                // le constructeur est appelé ici (_count = 0...)
std::string command;
while (true)
{
    std::cout << "Enter command (ADD, SEARCH, EXIT): ";
    if (!std::getline(std::cin, command))
        break;                      // Ctrl+D -> on quitte proprement
    if (command == "ADD")           // == fonctionne directement sur std::string !
        phoneBook.addContact();     // (en C il faudrait strcmp)
    ...
}
return (0);                         // le destructeur de phoneBook est appelé ici
```

### Pièges et améliorations

- **Ctrl+D pendant un ADD** : `GetInput` renvoie `""`, mais `addContact` continue et
  ajoute un contact avec des champs vides, puis la boucle de `main` s'arrête. Pour bien
  faire, `addContact` devrait vérifier `std::cin.eof()` et abandonner :
  ```cpp
  if (std::cin.eof()) return;
  ```
- **Passer les `std::string` par référence constante** : `setfName(std::string fname)` fait
  une **copie** de la chaîne. La bonne habitude en C++ :
  ```cpp
  void setfName(const std::string& fname);
  ```
  (les références sont expliquées dans CPP01 ex02).
- `isEmpty()` n'est jamais utilisée → elle pourrait être supprimée. Et si tu la gardes,
  elle devrait être `const`.

### À toi de jouer

Écris une classe `Counter` avec un attribut privé `int _value`, un constructeur qui le met à 0,
des méthodes `increment()`, `reset()` et un getter `getValue() const`. Sépare bien le `.hpp`
et le `.cpp`, avec les include guards.

---

## CPP00 ex02 — Account (pas encore fait)

Le dossier est vide. Les fichiers fournis par le sujet sont `Account.hpp` et `tests.cpp`
(à la racine). Le but : écrire `Account.cpp` pour que la sortie soit identique au fichier
de log fourni (sauf les timestamps).

La notion clé ici est **`static`** dans une classe :

```cpp
static int _nbAccounts;      // UNE SEULE variable partagée par TOUS les Account
int        _amount;          // une variable PAR objet Account
```

- Un attribut `static` existe **une seule fois**, peu importe combien d'objets existent.
  Parfait pour compter le nombre total de comptes.
- Il doit être **défini** une fois dans le `.cpp` :
  ```cpp
  int Account::_nbAccounts = 0;
  ```
- Une méthode `static` (`getNbAccounts()`) s'appelle **sans objet** :
  `Account::getNbAccounts()`. Elle n'a donc pas de `this` et ne peut accéder qu'aux
  attributs `static`.

Pour le timestamp, regarde `std::time` et `std::localtime` dans `<ctime>`.

---

## CPP01 ex00 — Zombie (stack vs heap)

### Ce que fait le programme

Crée des zombies de deux façons différentes, pour montrer la différence entre
**la pile (stack)** et **le tas (heap)**.

### Notions utilisées

#### Stack vs Heap

| | **Stack (pile)** | **Heap (tas)** |
|---|---|---|
| Création | `Zombie z("Bob");` | `Zombie* z = new Zombie("Bob");` |
| Destruction | **automatique** à la fin du bloc `{}` | **manuelle** avec `delete z;` |
| Durée de vie | limitée à la fonction | jusqu'au `delete` |
| Si on oublie | impossible d'oublier | **fuite mémoire** |
| Quand l'utiliser | par défaut, dès que possible | quand l'objet doit **survivre** à la fonction |

#### `new` / `delete` vs `malloc` / `free`

`new Zombie("Stan")` fait deux choses :
1. alloue la mémoire (comme `malloc`)
2. **appelle le constructeur** (ce que `malloc` ne fait pas)

`delete z` fait l'inverse :
1. **appelle le destructeur**
2. libère la mémoire

⚠️ Ne mélange jamais : `new` → `delete`, `malloc` → `free`.

#### La liste d'initialisation

```cpp
Zombie::Zombie(std::string name) : name(name) {}
//                               ^^^^^^^^^^^^ liste d'initialisation
```

C'est la manière **recommandée** d'initialiser les attributs en C++. Ça se lit :
« initialise l'attribut `name` avec le paramètre `name` ».

**Pourquoi ne pas écrire `this->name = name;` dans les `{}` ?**
- Avec la liste, l'attribut est **construit directement** avec la bonne valeur.
- Dans les `{}`, il est d'abord construit vide, **puis** on lui assigne la valeur
  (deux étapes au lieu d'une).
- Et surtout : certaines choses **ne peuvent être initialisées que dans la liste**
  (les références et les `const`). Tu le verras dans l'ex03.

Le fait que le paramètre et l'attribut aient le même nom (`name(name)`) fonctionne :
à l'extérieur des parenthèses c'est l'attribut, à l'intérieur c'est le paramètre.

### Le code expliqué

```cpp
// Zombie.hpp
class Zombie {
public:
    Zombie(std::string name);   // pas de constructeur sans paramètre :
    ~Zombie();                  // un zombie DOIT avoir un nom
    void announce();
private:
    std::string name;
};

Zombie* newZombie(std::string name);   // fonctions libres (hors classe),
void randomChump(std::string name);    // déclarées ici pour être utilisables partout
```

```cpp
// Zombie.cpp
Zombie::~Zombie() {
    std::cout << name << " is destroyed" << std::endl;  // pour VOIR quand il meurt
}
```

Le message dans le destructeur est demandé par le sujet : c'est ce qui te permet de
**constater** à quel moment chaque zombie est détruit.

```cpp
// newZombie.cpp — HEAP
Zombie* newZombie(std::string name) {
    return new Zombie(name);   // l'objet survit après la fin de la fonction
}                              // c'est à l'appelant de faire delete
```

```cpp
// randomChump.cpp — STACK
void randomChump(std::string name) {
    Zombie zombie(name);       // créé sur la pile
    zombie.announce();
}                              // <- fin du bloc : destructeur appelé AUTOMATIQUEMENT
```

**Pourquoi `newZombie` utilise le heap ?** Parce qu'elle **renvoie** le zombie. Si elle le
créait sur la stack, il serait détruit à la fin de la fonction et on renverrait un pointeur
vers un objet mort (*dangling pointer*) → crash ou comportement bizarre.

**Pourquoi `randomChump` utilise la stack ?** Parce que le zombie ne sert qu'à l'intérieur
de la fonction. Pas besoin de heap, et pas de risque d'oublier le `delete`.

Dans le `main`, remarque la différence entre `.` et `->` :

```cpp
Zombie* heapZombie1 = newZombie("Stan");
heapZombie1->announce();     // -> car heapZombie1 est un POINTEUR
                             // (équivalent à (*heapZombie1).announce())
Zombie zombie("Kyle");
zombie.announce();           // . car zombie est un OBJET
```

Ton `main` montre très bien la différence : « TemporaryZombie » est détruit tout de suite,
alors que « Survivor » est toujours vivant jusqu'au `delete`.

### À toi de jouer

Ajoute un `std::cout` dans le **constructeur** (« X is born »), lance le programme
et observe l'ordre exact des naissances et des morts. Puis retire un `delete` et lance
`valgrind ./zombie` pour voir la fuite mémoire.

---

## CPP01 ex01 — Zombie Horde (new[] / delete[])

### Ce que fait le programme

Crée **N zombies d'un coup** en une seule allocation, leur donne un nom,
les fait parler, puis les détruit tous.

### Notions utilisées

#### `new[]` et `delete[]`

```cpp
Zombie* horde = new Zombie[N];   // alloue N zombies d'un coup
delete[] horde;                  // ⚠️ avec les [] !
```

- `new Zombie[N]` appelle le **constructeur par défaut** (sans paramètre) N fois.
- `delete[]` appelle le destructeur **N fois**, puis libère.
- `delete` sans `[]` sur un tableau = **comportement indéfini** (souvent un seul destructeur
  appelé + crash). Règle : `new` ↔ `delete`, `new[]` ↔ `delete[]`.

#### Pourquoi un constructeur par défaut est nécessaire

En C++98, `new Zombie[N]` **ne peut pas** passer de paramètre aux constructeurs.
Il faut donc un constructeur `Zombie()` sans paramètre. C'est pour ça que tu as ajouté :

```cpp
Zombie();                     // constructeur par défaut (pour new[])
Zombie(std::string name);     // toujours dispo pour un zombie seul
void setName(std::string name);  // pour donner le nom APRÈS la création
```

Avoir deux constructeurs avec le même nom mais des paramètres différents s'appelle la
**surcharge** (*overloading*). Le compilateur choisit le bon selon les arguments.

### Le code expliqué

```cpp
Zombie* zombieHorde(int N, std::string name)
{
    if (N <= 0)
        return NULL;                  // on refuse une taille absurde
    Zombie* horde = new Zombie[N];    // N constructeurs par défaut (name = "")
    for (int i = 0; i < N; i++)
        horde[i].setName(name);       // on donne le nom à chacun
    return horde;                     // pointeur vers le 1er zombie
}
```

`horde[i]` est un **objet** (pas un pointeur), donc on utilise `.` et pas `->`.

```cpp
void Zombie::setName(std::string name) {
    this->name = name;   // ICI this-> est OBLIGATOIRE :
}                        // sans lui, "name = name" assignerait le paramètre à lui-même
```

C'est l'exemple parfait de l'utilité de `this->`.

### Pièges et améliorations

- Dans le `main`, `std::cin >> n;` n'est pas vérifié. Si on tape `abc`, en C++98
  `n` n'est pas modifié… et il n'a jamais été initialisé → valeur aléatoire.
  Version sûre :
  ```cpp
  int n = 0;
  if (!(std::cin >> n)) {
      std::cout << "Invalid number" << std::endl;
      return 1;
  }
  ```

### À toi de jouer

Modifie `zombieHorde` pour que chaque zombie ait un nom différent : `Randy0`, `Randy1`...
(Indice : en C++98 pas de `std::to_string`, utilise `std::stringstream` de `<sstream>`.)

---

## CPP01 ex02 — HI THIS IS BRAIN (pointeurs vs références)

### Ce que fait le programme

Affiche les adresses et les valeurs d'une chaîne, d'un pointeur vers elle,
et d'une **référence** vers elle. Résultat : les trois adresses sont **identiques**.

### Notions utilisées : la référence

Une **référence** est un **autre nom** (un alias) pour une variable qui existe déjà.

```cpp
std::string  brain = "HI THIS IS BRAIN";
std::string* stringPTR = &brain;   // pointeur : contient l'ADRESSE de brain
std::string& stringREF = brain;    // référence : EST brain, sous un autre nom
```

| | Pointeur `T*` | Référence `T&` |
|---|---|---|
| Peut être `NULL` | oui | **non**, toujours liée à quelque chose |
| Peut changer de cible | oui (`ptr = &autre;`) | **non**, liée pour toujours |
| Doit être initialisé à la déclaration | non | **oui** |
| Accès à la valeur | `*ptr` | directement `ref` |
| Accès à un membre | `ptr->membre` | `ref.membre` |

**Attention aux deux sens de `&`** :
- Dans une **déclaration** (`std::string& ref = ...`) : « ref est une référence »
- Dans une **expression** (`&brain`) : « l'adresse de brain »

### Le code expliqué

```cpp
std::cout << &brain     << std::endl;  // adresse de brain           -> 0x7ffd...
std::cout << stringPTR  << std::endl;  // contenu du pointeur = adresse -> 0x7ffd... (même)
std::cout << &stringREF << std::endl;  // adresse de la référence = adresse de brain (même)

std::cout << brain      << std::endl;  // HI THIS IS BRAIN
std::cout << *stringPTR << std::endl;  // on "déréférence" le pointeur avec *
std::cout << stringREF  << std::endl;  // pas d'étoile : la référence s'utilise comme brain
```

**La leçon** : une référence n'est pas une copie, c'est la **même** variable.
Si tu fais `stringREF = "autre";`, `brain` change aussi.

### Pourquoi les références sont partout en C++

Le cas d'usage n°1, c'est le **passage de paramètres** :

```cpp
void f(std::string s);         // COPIE toute la chaîne (lent si elle est grande)
void f(std::string& s);        // pas de copie, mais f peut MODIFIER l'original
void f(const std::string& s);  // pas de copie ET interdiction de modifier ✅
```

**Règle d'or** : pour passer un objet (`std::string`, une classe...) à une fonction qui
ne fait que le lire → `const T&`. Pour les petits types (`int`, `char`, `bool`) → par valeur.

### À toi de jouer

Écris une fonction `void swap(int& a, int& b)` qui échange deux entiers.
Compare avec la version C `void swap(int* a, int* b)` : laquelle est la plus lisible ?

---

## CPP01 ex03 — Unnecessary violence (HumanA / HumanB)

### Ce que fait le programme

Une arme (`Weapon`) et deux humains :
- `HumanA` reçoit son arme **à la construction** et l'a **toujours**.
- `HumanB` peut ne **pas avoir** d'arme, et la reçoit plus tard via `setWeapon`.

Quand on change le type de l'arme (`club.setType(...)`), l'humain doit voir le changement
→ il ne doit pas avoir une **copie** de l'arme, mais un **lien** vers elle.

### Notions utilisées

C'est **l'application directe** de l'ex02 : quand utiliser une référence ou un pointeur ?

- **`HumanA` → référence `Weapon&`** : il a **toujours** une arme, et ce sera **toujours
  la même**. Une référence ne peut être ni nulle, ni changée → parfait.
- **`HumanB` → pointeur `Weapon*`** : il peut **ne pas avoir** d'arme au début.
  Une référence ne peut pas être « vide », mais un pointeur peut valoir `NULL` → obligatoire.

### Le code expliqué

#### `Weapon`

```cpp
class Weapon {
private:
    std::string type;
public:
    Weapon();
    Weapon(const std::string& type);
    const std::string& getType() const;
    void setType(const std::string& type);
};
```

Décortiquons `const std::string& getType() const;` :

```
const std::string&   getType()   const;
^^^^^^^^^^^^^^^^^^               ^^^^^
renvoie une référence            la méthode ne modifie
constante vers type :            pas l'objet Weapon
pas de copie, et l'appelant
ne peut pas modifier type à travers elle
```

C'est exactement ce que demande le sujet, et c'est la manière idiomatique d'écrire
un getter qui renvoie un objet en C++.

#### `HumanA` : la référence

```cpp
class HumanA {
private:
    std::string name;
    Weapon& weapon;          // référence : HumanA ne POSSÈDE pas l'arme, il la "tient"
public:
    HumanA(const std::string& name, Weapon& weapon);
    void attack() const;
};

HumanA::HumanA(const std::string& name, Weapon& weapon) : name(name), weapon(weapon) {}
```

**Ici la liste d'initialisation est OBLIGATOIRE.** Une référence doit être liée **au moment
de sa création**. Si tu écrivais :

```cpp
HumanA::HumanA(const std::string& name, Weapon& weapon) {
    this->weapon = weapon;   // ❌ ERREUR : la référence aurait déjà dû être liée
}
```

ça ne compile pas (et même si ça compilait, `=` sur une référence **copierait** l'arme
au lieu de changer la cible).

Le paramètre est `Weapon&` et **pas** `const Weapon&` parce que l'attribut est un `Weapon&`
non-const : on ne peut pas lier une référence modifiable à quelque chose de `const`.

#### `HumanB` : le pointeur

```cpp
class HumanB {
private:
    std::string name;
    Weapon* weapon;          // pointeur : peut être NULL
...
};

HumanB::HumanB(const std::string& name) : name(name), weapon(NULL) {}
//                                                    ^^^^^^^^^^^^ pas d'arme au départ

void HumanB::setWeapon(Weapon& weapon) {
    this->weapon = &weapon;  // on stocke l'ADRESSE de l'arme reçue
}

void HumanB::attack() const {
    if (weapon)              // on vérifie TOUJOURS un pointeur avant de l'utiliser
        std::cout << name << " attacks with their " << weapon->getType() << std::endl;
    else
        std::cout << name << " has no weapon to attack with" << std::endl;
}
```

**Pourquoi `setWeapon` prend une référence et pas un pointeur ?** Pour l'appel, c'est plus
agréable : `jim.setWeapon(club);` au lieu de `jim.setWeapon(&club);`. Et ça garantit qu'on
ne passe pas `NULL`. À l'intérieur, on prend l'adresse avec `&weapon`.

**Pourquoi `attack()` est `const` ?** Attaquer ne modifie pas l'humain (ni son nom, ni le
lien vers l'arme). Et `getType()` est `const` aussi, donc on peut l'appeler depuis une
méthode `const`.

#### `main`

```cpp
{
    Weapon club = Weapon("crude spiked club");
    HumanA bob("Bob", club);
    bob.attack();                         // crude spiked club
    club.setType("some other type of club");
    bob.attack();                         // some other type of club  <- grâce à la référence !
}
```

Les accolades `{ }` créent un **bloc** : `club` et `bob` sont détruits à la fin.
Ça permet de tester les deux humains séparément avec des variables du même nom.

### À toi de jouer

Ajoute à `HumanB` une méthode `dropWeapon()` qui remet le pointeur à `NULL`. Pourquoi
est-ce **impossible** à faire pour `HumanA` ?

---

## CPP01 ex04 — Sed is for losers (fichiers)

### Ce que fait le programme

```
$ ./replace monfichier hello bonjour
```

Lit `monfichier`, remplace **toutes** les occurrences de `hello` par `bonjour`,
et écrit le résultat dans `monfichier.replace`. Le sujet **interdit** `std::string::replace`.

### Notions utilisées

#### Les flux de fichiers (`<fstream>`)

| Classe | Rôle | Équivalent C |
|---|---|---|
| `std::ifstream` | **i**nput **f**ile stream : lire un fichier | `fopen(f, "r")` |
| `std::ofstream` | **o**utput **f**ile stream : écrire un fichier | `fopen(f, "w")` |

Ils s'utilisent **exactement comme** `std::cin` et `std::cout` : avec `>>`, `<<`, `getline`.
Et ils se **ferment tout seuls** à la destruction (pas besoin de `close()`).
C'est le principe **RAII** : la ressource est libérée automatiquement par le destructeur.

#### `std::stringstream` (`<sstream>`)

Un flux qui écrit/lit dans une **chaîne en mémoire** au lieu d'un fichier ou de l'écran.

#### `std::cerr`

Comme `std::cout`, mais vers la **sortie d'erreur** (fd 2). Les messages d'erreur doivent
y aller.

#### `find` et `npos`

- `str.find(s1, pos)` cherche `s1` dans `str` **à partir de** la position `pos`.
  Renvoie la position trouvée, ou `std::string::npos` (« pas trouvé ») sinon.
- `npos` est une valeur spéciale (le plus grand `size_t` possible).

### Le code expliqué

#### Les vérifications

```cpp
if (ac != 4) { std::cerr << "Usage: ..." << std::endl; return 1; }

std::string s1 = av[2];
if (s1.empty()) { ... return 1; }   // ⚠️ important : sinon boucle infinie
```

**Pourquoi refuser `s1` vide ?** `find("")` trouve une chaîne vide **partout**, à chaque
position. La boucle n'avancerait jamais (`pos = found + 0`) → boucle infinie.

#### Ouvrir et lire le fichier

```cpp
std::ifstream infile(filename.c_str());   // ouvre le fichier en lecture
if (!infile) { ... }                       // le flux est "faux" si l'ouverture a échoué

std::stringstream buffer;
buffer << infile.rdbuf();                  // copie TOUT le fichier dans buffer
```

**Pourquoi `.c_str()` ?** En C++98, le constructeur de `ifstream` n'accepte que des
`const char*`, pas des `std::string` (c'est corrigé en C++11). `.c_str()` convertit une
`std::string` en `const char*`.

**Pourquoi `rdbuf()` plutôt qu'une boucle `getline` ?** `getline` **supprime** les `\n`.
Il faudrait les remettre à la main, et on risquerait d'en ajouter un en trop à la fin.
`rdbuf()` copie le fichier **octet par octet, à l'identique**. C'est plus simple et plus juste.

#### L'algorithme de remplacement

```cpp
static std::string replaceAll(const std::string& content,
                              const std::string& s1, const std::string& s2)
{
    std::string result;
    size_t      pos = 0;        // où on en est dans content
    size_t      found;

    while ((found = content.find(s1, pos)) != std::string::npos) {
        result += content.substr(pos, found - pos) + s2;  // texte avant + remplacement
        pos = found + s1.length();                        // on saute s1
    }
    return result + content.substr(pos);                  // le reste après le dernier s1
}
```

Exemple avec `content = "aXbXc"`, `s1 = "X"`, `s2 = "--"` :

| Tour | `pos` | `found` | ajouté à `result` | `result` |
|---|---|---|---|---|
| 1 | 0 | 1 | `"a"` + `"--"` | `a--` |
| 2 | 2 | 3 | `"b"` + `"--"` | `a--b--` |
| 3 | 4 | npos | (sortie de boucle) | |
| fin | | | `"c"` | `a--b--c` |

**Pourquoi construire une nouvelle chaîne au lieu de modifier `content` ?**
Si `s2` contient `s1` (ex. remplacer `a` par `aa`), modifier sur place pourrait retrouver
le `a` qu'on vient d'insérer → boucle infinie. En construisant `result` à part et en avançant
`pos` dans l'original, ce problème n'existe pas.

**Pourquoi `static` devant la fonction ?** Ici, `static` (sur une fonction libre) veut dire
« visible **uniquement** dans ce fichier `.cpp` ». C'est une fonction utilitaire privée.
(À ne pas confondre avec `static` dans une classe, vu dans CPP00 ex02.)

**Pourquoi `const std::string&` ?** Le contenu du fichier peut être énorme : on évite de le
copier, et on promet de ne pas le modifier (voir ex02).

#### Écrire le résultat

```cpp
std::ofstream outfile((filename + ".replace").c_str());  // crée/écrase le fichier
if (!outfile) { ... }
outfile << replaceAll(buffer.str(), s1, av[3]);          // .str() = le contenu en std::string
return 0;   // infile et outfile se ferment ici automatiquement (destructeurs)
```

### À toi de jouer

Écris `./count fichier mot` qui affiche le nombre d'occurrences de `mot` dans le fichier.
Tu peux réutiliser presque toute la structure (`ifstream`, `rdbuf`, la boucle `find`).

---

## CPP01 ex05 — Harl 2.0 (pointeurs sur fonctions membres)

### Ce que fait le programme

```
$ ./harl WARNING
WARNING: I think I deserve to have some extra bacon for free...
```

Harl a 4 niveaux de plainte. Selon le niveau donné, on appelle la bonne méthode.
**Contrainte du sujet** : pas de forêt de `if/else if`. Il faut utiliser des
**pointeurs sur fonctions membres**.

### Notions utilisées

#### Rappel : pointeur sur fonction en C

```c
void hello(void) { ... }
void (*f)(void) = &hello;   // f pointe sur hello
f();                        // appelle hello
```

#### Pointeur sur fonction **membre** en C++

Une méthode a besoin d'un **objet** pour être appelée (elle utilise `this`).
La syntaxe est donc un peu différente :

```cpp
void (Harl::*f)(void) = &Harl::debug;
//    ^^^^^^ "pointeur sur une méthode de Harl"
//   qui ne prend rien (void) et ne renvoie rien (void)

(this->*f)();     // appeler f sur l'objet this
(harl.*f)();      // ou sur un objet harl
```

- `&Harl::debug` : le `&` est **obligatoire** pour une méthode (contrairement au C).
- `->*` (avec un pointeur d'objet) et `.*` (avec un objet) : opérateurs spéciaux pour
  appeler un pointeur de méthode.
- Les **parenthèses** autour de `(this->*f)` sont obligatoires à cause des priorités
  d'opérateurs : sans elles, le compilateur lirait `this->*(f())`.

### Le code expliqué

```cpp
class Harl {
private:              // les 4 plaintes sont privées : on ne peut passer
    void debug(void); // QUE par complain()
    void info(void);
    void warning(void);
    void error(void);
public:
    void complain(std::string level);
};
```

```cpp
void Harl::complain(std::string level)
{
    std::string levels[] = {"DEBUG", "INFO", "WARNING", "ERROR"};
    void (Harl::*functions[])() = {&Harl::debug, &Harl::info,
                                   &Harl::warning, &Harl::error};
    // deux tableaux "parallèles" : levels[i] correspond à functions[i]

    for (int i = 0; i < 4; i++)
    {
        if (levels[i] == level)        // on cherche l'index du niveau
        {
            (this->*functions[i])();   // on appelle la méthode correspondante
            return;
        }
    }
    std::cout << "Unknown level: " << level << std::endl;  // aucun ne correspond
}
```

`void (Harl::*functions[])()` se lit de l'intérieur vers l'extérieur :
« `functions` est un **tableau** `[]` de **pointeurs sur méthodes de Harl** `Harl::*`
qui ne prennent rien `()` et renvoient `void` ».

**Pourquoi c'est mieux qu'une suite de `if` ?** Pour ajouter un niveau, il suffit d'ajouter
une entrée dans chaque tableau : la logique ne change pas. Le code est « piloté par les
données ».

**Pourquoi `complain` peut appeler des méthodes privées ?** Parce que `complain` est
elle-même une méthode de `Harl`. `private` bloque l'accès depuis **l'extérieur** de la classe,
pas depuis l'intérieur.

### Pièges et améliorations

- Pour rendre le code plus lisible, on peut créer un alias de type avec `typedef` :
  ```cpp
  typedef void (Harl::*HarlFn)(void);
  HarlFn functions[] = {&Harl::debug, &Harl::info, &Harl::warning, &Harl::error};
  ```
- `complain(std::string level)` → `complain(const std::string& level)` (voir ex02).
- `#include <iostream>` dans le `.hpp` n'est pas nécessaire (le `.hpp` n'utilise que
  `std::string`) : il vaut mieux le mettre dans `Harl.cpp`. Règle : un header n'inclut
  que ce dont **il** a besoin.

### À toi de jouer

Crée une classe `Calculator` avec 4 méthodes privées `add`, `sub`, `mul`, `div`
(`int f(int a, int b)`) et une méthode publique `compute(char op, int a, int b)` qui choisit
la bonne opération avec un tableau de pointeurs sur méthodes.
(La syntaxe du pointeur devient `int (Calculator::*)(int, int)`.)

---

## CPP01 ex06 — Harl filter (pas encore fait)

Le dossier est vide. Le but : `./harlFilter WARNING` affiche le niveau donné **et tous
les niveaux au-dessus** (ici WARNING puis ERROR). La notion imposée est le **`switch`**,
et l'astuce est le **fall-through** : quand on n'écrit pas `break`, l'exécution continue
dans le `case` suivant.

```cpp
switch (index)        // index = 0 pour DEBUG, 1 pour INFO, etc.
{
    case 0:
        debug();      // pas de break -> on continue...
    case 1:
        info();       // ...ici
    case 2:
        warning();
    case 3:
        error();
        break;
    default:
        std::cout << "[ Probably complaining about insignificant problems ]" << std::endl;
}
```

Tu peux réutiliser ta boucle de l'ex05 pour trouver l'index du niveau, puis faire le `switch`
dessus (un `switch` ne fonctionne qu'avec des entiers, pas avec des `std::string`).
Attention : `-Wextra` peut afficher un warning `-Wimplicit-fallthrough`. Si c'est le cas,
regarde comment l'éviter tout en restant en C++98.

---

## Récap : la boîte à outils du débutant

### Comment écrire une classe (modèle à recopier)

```cpp
// MaClasse.hpp
#ifndef MACLASSE_HPP
# define MACLASSE_HPP
# include <string>

class MaClasse
{
public:
    MaClasse(void);                              // constructeur par défaut
    MaClasse(const std::string& name);           // constructeur avec paramètre
    ~MaClasse(void);                             // destructeur

    const std::string& getName(void) const;      // getter : const partout
    void               setName(const std::string& name);  // setter

private:
    std::string _name;
    int         _value;
};

#endif
```

```cpp
// MaClasse.cpp
#include "MaClasse.hpp"

MaClasse::MaClasse(void) : _name(""), _value(0) {}
MaClasse::MaClasse(const std::string& name) : _name(name), _value(0) {}
MaClasse::~MaClasse(void) {}

const std::string& MaClasse::getName(void) const { return _name; }
void MaClasse::setName(const std::string& name) { _name = name; }
```

### Les règles à retenir

1. **Attributs en `private`**, accès par getters/setters.
2. **Initialise tes attributs dans la liste d'initialisation** (`: _a(x), _b(y)`).
   Les `int`, `float`, pointeurs... ne sont **pas** initialisés automatiquement.
3. **Les getters et toute méthode qui ne modifie rien → `const`** à la fin.
4. **Paramètres objets → `const T&`** ; petits types (`int`, `char`) → par valeur.
5. **Stack par défaut**, heap (`new`) seulement si l'objet doit survivre à la fonction.
6. `new` ↔ `delete`, `new[]` ↔ `delete[]`. Jamais avec `malloc`/`free`.
7. **Référence** si le lien existe toujours et ne change pas ; **pointeur** s'il peut être
   `NULL` ou changer.
8. Un pointeur se **vérifie** avant d'être utilisé (`if (ptr)`).
9. Toujours **tester les flux** : `if (!std::getline(std::cin, s))`, `if (!infile)`.
10. **Include guards** dans chaque `.hpp`, et le `;` après `}` d'une classe.

### Lire une déclaration compliquée

On part du **nom** et on lit vers la droite, puis vers la gauche :

```cpp
const std::string& getType() const;
// getType est une méthode () const qui renvoie une référence & vers une std::string const

void (Harl::*functions[])();
// functions est un tableau [] de pointeurs * sur membres de Harl,
// vers des fonctions () qui renvoient void
```

### Les outils pour vérifier ton code

- `valgrind --leak-check=full ./prog` : détecte les fuites mémoire et les accès invalides.
- `c++ -Wall -Wextra -Werror -std=c++98 -g` : le `-g` permet d'avoir les numéros de ligne
  dans valgrind et gdb.
- Ajoute des `std::cout` dans les **constructeurs et destructeurs** pour voir exactement
  quand tes objets naissent et meurent : c'est le meilleur moyen de comprendre le C++.
