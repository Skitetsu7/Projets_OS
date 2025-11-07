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
- La librairie utilise les couleurs 8 bits ![](https://www.google.com/url?sa=i&url=https%3A%2F%2Flearn.adafruit.com%2Fcustom-color-palettes-for-makecode-arcade-games%2Fcolor-palettes&psig=AOvVaw1E6B3KR1QHYsvV_TlZj3E0&ust=1762600301116000&source=images&cd=vfe&opi=89978449&ved=0CBUQjRxqFwoTCNjQp_vz35ADFQAAAAAdAAAAABAE)
- Les couleurs foncée prennent une Majuscule `Red` ≠ `red`
- Pour modifier le fond ajoutez B devant le nom de la couleur `Bred`,`BBlue`
- Pour revenir aux couleurs par défaut de votre terminal, utilisez `normal`
