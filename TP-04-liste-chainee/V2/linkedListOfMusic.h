#pragma once

#include <stdlib.h>
#include <stdio.h>
#include "linkedList.h"

struct music_s{
	char* name;
	char* artist;
	char* album;
	char* genre;
	int discNumber;
	int trackNumber;
	int year;
};

typedef struct music_s Music;


bool equalsElement(Element e1, Element e2);	

bool estPlusRecent(Element e1, Element e2);

Liste trierListe_r(Liste);
Liste trierListe_aux(Liste, Liste);