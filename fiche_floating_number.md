Floating Point en C — Fiche de révision
1. Le problème de base

Un ordinateur travaille avec des bits (0 et 1).

Les nombres entiers sont relativement simples à représenter :

5 = 101 en binaire


Pour les nombres à virgule, c'est différent.

Certains nombres décimaux ne peuvent pas être représentés exactement en binaire.

Par exemple :

0.1 en décimal


devient une représentation binaire infinie :

0.000110011001100110011...


Comme un float possède un nombre limité de bits, l'ordinateur doit arrondir.

Donc :

0.1 ≠ exactement la valeur stockée en mémoire


Il stocke une valeur très proche.

2. Pourquoi 0.1 + 0.2 peut être différent de 0.3

Mathématiquement :

0.1 + 0.2 = 0.3


Mais l'ordinateur stocke approximativement :

0.1 → approximation de 0.1
0.2 → approximation de 0.2


Le calcul devient donc :

approximation de 0.1
+
approximation de 0.2
=
approximation de 0.3


Le résultat peut être légèrement différent de 0.3.

Ce n'est pas un bug du langage C.

C'est une conséquence de la représentation binaire des nombres flottants.

3. float et double

En général :

Type	Taille	Précision approximative
float	32 bits	~7 chiffres significatifs
double	64 bits	~15-16 chiffres significatifs

Exemple :

float a = 0.1f;
double b = 0.1;


Un double possède davantage de bits et permet donc généralement de représenter les nombres avec une meilleure précision.

Mais attention :

double ne signifie pas "exact".

Certains nombres restent impossibles à représenter exactement.

4. Comment un float est stocké

Un float classique utilise 32 bits.

Ces bits sont divisés en trois parties :

┌────────┬──────────┬───────────────────────┐
│ Signe  │ Exposant │       Mantisse        │
│ 1 bit  │  8 bits  │        23 bits        │
└────────┴──────────┴───────────────────────┘

Signe

Le bit de signe indique si le nombre est positif ou négatif.

0 → positif
1 → négatif

Exposant

L'exposant indique essentiellement la taille du nombre.

Cela permet de représenter des nombres très grands ou très petits.

L'idée est similaire à la notation scientifique :

3000 = 3 × 10³


En binaire, les nombres flottants utilisent une puissance de 2.

Mantisse

La mantisse contient les chiffres significatifs du nombre.

Comme elle possède un nombre limité de bits, elle limite la précision.

5. Pourquoi les erreurs d'arrondi existent

Un float possède seulement un nombre limité de bits.

Il ne peut donc pas stocker une infinité de chiffres.

Prenons :

1 / 3 = 0.333333333333333...


Si on ne peut conserver que quelques chiffres :

0.333333


on obtient une approximation.

Le principe est similaire avec les nombres flottants en binaire.

À retenir :

nombre réel
    ↓
représentation binaire
    ↓
nombre limité de bits
    ↓
arrondi
    ↓
approximation

6. Précision et exactitude

Ces deux notions sont différentes.

Exactitude

L'exactitude indique si la valeur est proche de la vraie valeur.

Précision

La précision indique combien de chiffres significatifs peuvent être conservés.

Un nombre peut donc être :

très précis mais légèrement incorrect ;
peu précis mais suffisamment proche pour l'utilisation.

Pour les nombres flottants, il faut toujours garder à l'esprit que la représentation est une approximation.

7. Comparer des float

Attention à ceci :

if (a == b)


Avec des nombres flottants, deux calculs mathématiquement équivalents peuvent produire des valeurs légèrement différentes.

Par exemple :

a = 0.3000000119
b = 0.3000000000


Pourtant, les deux valeurs peuvent être considérées comme "suffisamment proches" selon le contexte.

On utilise donc souvent une tolérance :

#include <math.h>

#define EPSILON 1e-7

if (fabs(a - b) < EPSILON)
{
    // a et b sont suffisamment proches
}


L'idée est :

|a - b| < tolérance


On ne demande plus une égalité parfaite.

Attention : une valeur fixe comme EPSILON n'est pas adaptée à tous les cas. Pour des nombres très grands ou très petits, une comparaison relative peut être nécessaire.

8. Perte de précision

Les erreurs deviennent particulièrement importantes dans certaines opérations.

Soustraction de nombres proches

Exemple :

123456.789
-123456.788
------------
     0.001


Les deux grands nombres sont très proches.

Une grande partie de leurs chiffres s'annule pendant la soustraction.

Il peut alors rester moins de chiffres significatifs utiles.

On parle de perte de significativité.

À retenir :

Soustraire deux nombres très proches peut provoquer une perte importante de précision.
9. Ajouter un très petit nombre à un grand nombre

Exemple :

float x = 1.0;
x += 0.00000001;


Mathématiquement :

1.0 + 0.00000001
= 1.00000001


Mais un float n'a pas suffisamment de précision pour toujours représenter cette différence.

Le résultat peut donc rester :

1.0


Le petit nombre est alors trop petit pour modifier la valeur représentée.

10. Les calculs intermédiaires sont importants

Un résultat final peut être représentable alors qu'une étape intermédiaire ne l'est pas.

Exemple :

sqrt(a * a + b * b)


Supposons :

a = 1e200
b = 1e200


Le résultat final est environ :

1.414 × 10²⁰⁰


Ce résultat peut être représenté par un double.

Mais le calcul commence par :

a * a


soit :

1e200 * 1e200
= 1e400


1e400 est trop grand pour un double.

Le calcul intermédiaire peut donc produire :

+inf


et le résultat final sera alors incorrect.

À retenir :

Il faut faire attention aux valeurs intermédiaires, pas seulement au résultat final.
11. inf

Les nombres flottants peuvent représenter l'infini.

Par exemple :

+inf
-inf


Cela peut arriver lorsqu'un calcul dépasse la valeur maximale représentable.

On parle d'overflow.

nombre trop grand
       ↓
    overflow
       ↓
      +inf

12. NaN

NaN signifie :

Not a Number


C'est une valeur spéciale utilisée pour représenter un résultat qui n'est pas un nombre valide.

On peut donc rencontrer :

NaN
+inf
-inf


dans les calculs flottants.

13. Afficher un nombre flottant

Avec printf, on peut utiliser différents formats.

%f

Affichage en notation décimale :

printf("%f\n", value);


Exemple :

123.456000

%e

Affichage en notation scientifique :

printf("%e\n", value);


Exemple :

1.234560e+02


Cette notation est particulièrement pratique pour les très grands ou très petits nombres.

14. L'affichage ne représente pas forcément exactement la valeur

Il faut distinguer :

valeur stockée


et

valeur affichée


Par exemple, un nombre peut être affiché comme :

0.300000


alors que la valeur réellement stockée est légèrement différente de 0.3.

À l'inverse, si on demande beaucoup de chiffres à printf, on peut voir apparaître les erreurs d'arrondi :

0.30000000000000004


Cela ne signifie pas nécessairement que le calcul est "cassé".

Cela montre simplement davantage la représentation réelle du nombre.

15. Résumé du fonctionnement

Un nombre flottant peut être vu comme :

             FLOAT
               │
       ┌───────┼───────┐
       │       │       │
     Signe  Exposant Mantisse
       │       │       │
      +/-    Taille  Précision


Le nombre de bits étant limité :

bits limités
     ↓
précision limitée
     ↓
arrondis
     ↓
petites erreurs possibles

16. Les erreurs les plus fréquentes
Erreur 1 — Penser qu'un float est exact

Faux :

float = approximation

Erreur 2 — Penser que double est toujours exact

Faux :

double = meilleure précision


mais pas une précision infinie.

Erreur 3 — Comparer naïvement avec ==

À éviter lorsque les valeurs proviennent de calculs flottants :

if (a == b)


Préférer une comparaison avec une tolérance adaptée.

Erreur 4 — Ignorer les calculs intermédiaires

Même si le résultat final semble raisonnable, une étape intermédiaire peut provoquer :

overflow
perte de précision
arrondi

Erreur 5 — Croire aveuglément ce que montre printf

L'affichage est une représentation de la valeur.

Il ne faut pas confondre :

valeur affichée


avec :

valeur mathématique exacte

17. Ce qu'il faut retenir pour un examen

Si tu n'as que 1 minute pour réviser, retiens ces points :

Un ordinateur stocke les nombres flottants en binaire.
Certains nombres décimaux ne peuvent pas être représentés exactement en binaire.
Un float est donc généralement une approximation.
Un float utilise 32 bits : signe + exposant + mantisse.
Un double utilise généralement 64 bits et offre davantage de précision.
Les erreurs d'arrondi peuvent apparaître pendant les calculs.
== peut être problématique pour comparer des résultats flottants.
Une comparaison avec une tolérance est souvent préférable.
Un nombre trop grand peut provoquer un overflow et donner +inf ou -inf.
NaN signifie Not a Number.
Les calculs intermédiaires peuvent perdre de la précision.
%f affiche en notation décimale et %e en notation scientifique.
18. La phrase à retenir
Un nombre flottant n'est pas une valeur réelle exacte : c'est une approximation binaire stockée avec un nombre limité de bits.

C'est cette limitation qui explique la majorité des problèmes présentés dans les trois articles.

Sources
Understanding Floating Point
Understanding Floating Point Representation
Understanding Floating Point Printing