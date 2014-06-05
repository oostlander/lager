#ifndef AVL_BAUM_H
#define AVL_BAUM_H
#include "schluesseltyp.h"
#include "datensatz.h"
/* Knoten im binaeren Suchbaum */
typedef struct avl_knoten {
	/* im Knoten abgelegte Daten */
	Datensatz avl_daten;
	int avl_beta; /* Balance-Index */
	/* Zeiger auf linken und rechten Sohn */
	struct avl_knoten *avl_lsohn;
	struct avl_knoten *avl_rsohn;

} AVL_knoten;
/* AVL-Baum = Zeiger auf (Wurzel-)Knoten */
typedef AVL_knoten *AVL_baum;
/* Deklaration der Operationen fuer AVL-Baeume */
void avl_create(AVL_baum *b);
AVL_knoten *avl_suchen(SchluesselTyp s,
	AVL_baum b);
void avl_einfuegen(Datensatz d, AVL_baum *b);
void avl_loeschen(SchluesselTyp s, AVL_baum *b);
void avl_write(AVL_baum b, int tiefe);
#endif