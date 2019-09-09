### README ###

Za izradu ovog programa koristio sam "3rd party" knjiznice: glm, glfw, glew.
Takoder sam koristio vlastite klase koje su spremljene u direktoriju enkapsulacije_H.
Program je preveden i povezan u linux sustavu s naredbom:
g++ -std=c++11  main.cpp enkapsulacije_H/*cpp -lglfw -lGLU -lGL -lGLEW

### Koristenje ###

Program se sastoji od 4 stanja koja se mijenjaju pritiskom tipki:
1) Jednostavna mapa sjena pod utjecajem direckcijskog svijetla
2) Variance mapa sjena pod utjecajem direckcijskog svijetla
3) Mapa sjena pod utjecajem pozicijskog svijetla
4) Volumne sjene pod utjecajem pozicijskog svijetla

Svijetlo je programirano da se mice pos x osi kako bi se bolje primjetili nedostaci i prednosti svake tehnike.

Za prijelaz izmedu stanja potrebno je pritisnuti tipke 1 - 4.
Za zaustavljanje svijetla potrebno je pritisnuti tipku L.
Pritiskom tipke Q u drugom i trecem stanju ukljucuje / iskljucuje se PCF filtriranje sjena.