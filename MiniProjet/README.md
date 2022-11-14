# Description du projet

J'ai modifié le jeu asteroids.
Le vaisseau a maintenant la capacité de repousser les asteroides dans un petit rayon autour de lui, cette capacité a un temps de recharge de quelques secondes.
Repousser l'équivalent de 4 asteroides ou plus d'un seul coup repousse tous les astéroides.
Des bonus apparaissent parfois sur le terrain : ils réduisent la vitesse des asteroides durant un court laps de temps.
Toutes les 20 secondes, de nouveaux asteroides apparaissent à l'écran et la vitesse de tous les asteroides augmente.


# Comment compiler

gcc -o asteroids2 -Wall asteroids2.c -std=c99 /home/kb/Raylib.git/src/libraylib.a -lm -lrt -ldl -lX11 -IGL -Ipthread

(Aussi j'ai dû compiler raylib avec "make PLATFORM = PLATFORM_DESKTOP GRAPHICS=GRAPHICS_API_OPENGL_21 -B" car j'ai OpenGL en version 2.1)

# Comment exécuter

./asteroids2

# Comment utiliser le projet

Pour accélerer et ralentir utiliser les flèches directionnelles.

Pour repousser les asteroides appuyer sur la barre espace.

# URL vidéo de démo

https://youtu.be/Vb-G9isPBeY

# Quelques détails sur votre implémentation

Voici une liste des modifications/ajouts importants:

[ligne 33 à 36] : quelques déclarations de constantes.
[ligne 60 à 67] : déclaration du type Bonus, c'est une struct contenant les variables nécessaires.
[ligne 80 à 81] : déclaration de variables globales pour augmenter la difficulté.
[ligne 166 à 172] : initialisation du bonus.
[ligne 257 à 260] : déclaration de la fonction distance.
[ligne 262 à 317] : déclaration de la fonction explosion permettant de repousser les asteroides en ligne droite par rapport à la position du vaisseau.
[ligne 330 à 342] : détermination du niveau de difficulté en fonction du temps écoulé depuis le début de la partie, remise en jeu d'asteroides et augmentation de la vitesse de tous les asteroides en fonction.
[ligne 345 à 350] : activation du pouvoir du joueur si la barre d'espace est activée et réduire le temps de chaque temps de recharge et durée actuelle du bonus (la variable cooldown a été ajoutée à la struct player).
[ligne 352 à 359] : determination de la collision entre le vaisseau et le bonus, activation du bonus pour un certain temps si c'est le cas.
[ligne 490 : ajout d'un timer pour connaitre le temps de recharge de la compétence.

# Extensions / améliorations possibles

On pourrait ajouter un systeme de points en fonction du nombre de bonus récupérés, des asteroides avec des trajectoires non linéaires, ajouter des collisions entre asteroides les faisant changer de trajectoire, ajouter un systeme de tir pour le vaisseau et des bonus le modifier : augmenter la cadence et le nombre de tirs, ou encore un asteroide allié qui graviterai autour du vaisseau.
