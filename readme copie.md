Nous sommes passé par plusieurs façon de faire un tri par fusion avec multi thread mais finalement, nous sommes revenu sur la façon la plus intuitive de faire le tri. C'est à dire découper le tableau par le nombre de threads et laisser chaque thread traiter une partie du tableau. Jusqu'à ce qu'il se rejoignent tous pour fusionner en un tableau final.

Nous avons tester le tableau et fait la moyenne des temps d'éxécution qui nous a permis de voir que si le tableau est trop petit il est inutile de donner la charge de travail à beaucoup de thread par contre plus le tableau grandit plus avoir un nombre de thread élevé est un avantage
- [ ] m

|                       | taille du tableau |              |             |               |             |             |             |                 |
| --------------------- | ----------------- | ------------ | ----------- | ------------- | ----------- | ----------- | ----------- | --------------- |
| **nombre de threads** |                   | 1            | 2           | 3             | 4           | 5           | 6           | 7               |
|                       | 100               | 58290 ns     | 52280 ns    | **42330 ns**  | 56300 ns    | 57170 ns    | 56430 ns    | 59140 ns        |
|                       | 10000             | 1334798 ns   | 894079 ns   | **784887 ns** | 811548 ns   | 1015580 ns  | 1142318 ns  | 1235411 ns      |
|                       | 1000000           | 149711910 ns | 83644190 ns | 60978150 ns   | 48871640 ns | 50453950 ns | 48686520 ns | **47204920 ns** |

Graphique: 


Sources :
https://youtu.be/_XOZ2IiP2nw
https://youtu.be/SzG1QYYpimg
https://www.youtube.com/shorts/7PcqoMWbxN8?feature=share