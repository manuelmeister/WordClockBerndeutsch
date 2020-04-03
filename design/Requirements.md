# Requirements

1. Es soll jede Minute ablesbar sein
2. Im Optimalfall sollen die Wörter so aneinander gereit werden, so dass es möglichst kompakt ist (`eisächsibni = eis sächsi sibni`).
3. Durch das Req 2. muss aber auch besonders darauf geachtet werden, dass Wörter die gleichzeitig aufleuchten, nicht vertikal oder horizontal anliegen. Dies weil sonst die Lesbarkeit noch schlechter ist. 
4. Es soll sprachlich der gesprochenen Umgangsform entsprechen:  
  niemand spricht von `es ist dreizehn ab zwölf` sondern eher von `es ist bald viertel nach zwölf` oder `es ist ungefähr viertel nach zwölf`)
5. Der Lesefluss geht von Oben nach Unten
6. Falls möglich sollte jeder Buchstabe einmal vorkommen. Dadurch könnte man in einem Spezialfall trotzdem alles auf der Uhr schreiben, wenn auch nur mühsam durch einzelnes aufleuchten.
7. Damit falls nötig jede Minute ausgeschrieben werden kann, kommt zehn und zwanzig am Schluss der Minutenzahlenblocks. 
8. Falls diese Anforderungen dazu führen, dass es "überschüssige" Buchstabenfelder gibt, sollen diese mit Buchstaben oder Zeichen gefüllt werden, die:
   1. für die Anzeige des Status der Uhr nützlich sind
   2. ein weiteres Wort ergeben
   3. ein fehlender Buchstabe aus dem Alphabet (siehe Punkt 6)

## Umsetzung

| Kategorie  | Wörter         | 
| ---------- | -------------- | 
| Meta       | Es ist         | 
| Modifikator | ungefähr, bald | 
| Minuten    | viertel, ein, sechs, sieben, elf, zwei (i unterhalb) & zwölf, fünf, acht, drei, zwanzig, neun, zehn | 
| Modifikator | ab, vor        |
| Aufteilung | halb           |
| Stunden    | zwölf, vier, fünf, eins, sechs, sieben, drei, elf, acht, neun, zehn |
| Modifikator | gewesen        |

Daneben gibt es noch folgende Wörter:

```
     f i s c h
       n e i
               l a c h t
🐈 m e i s t e r.i o / z
                       y
           j a         t
         I Q
                 f i x 📶
🐘

             n i d        
```

Ich habe gerne Katzen und meine Freundin gerne Elefanten.