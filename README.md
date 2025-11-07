# Projets_OS
Ici seront mis tous les codes liés aux projets de groupe du cours d’OS.

Utilisation de colors.h
```
#include "colors.h"

printf("%s%sHello %s%s World %s \n",
  red,Bcyan,
  black,Bred,
  normal);
```
- La librairie utilise les couleurs 8 bits ![images/color_palette.png]
- Les couleurs foncée prennent une Majuscule `Red` ≠ `red`
- Pour modifier le fond ajoutez B devant le nom de la couleur `Bred`,`BBlue`
- Pour revenir aux couleurs par défaut de votre terminal, utilisez `normal`
