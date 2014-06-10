#include "stdafx.h"
#include "avl_baum.h"


bool gleich(SchluesselTyp s1, SchluesselTyp s2)
{
	return *s1.Teilenummer == *s2.Teilenummer;
}

bool kleiner(SchluesselTyp s1, SchluesselTyp s2)
{
	return *s1.Teilenummer < *s2.Teilenummer;
}

void avl_create(AVL_baum *b)
{
	*b = NULL;
}

void datensatz_ausgeben(AVL_knoten *a)
{
	printf("T.nr.:%i ", a->avl_daten.Teilenummer);
	printf("Bez.:%s ", a->avl_daten.Bezeichner);
	printf("E.nr.:%i ", a->avl_daten.Elementnummer);
	printf("Br.:%f ", a->avl_daten.Breite);
	printf("Gew.:%f ", a->avl_daten.Gewicht);
	printf("\n"); 
}
void avl_write(AVL_baum b, int tiefe)
{
	int i;
	if (b != NULL) {
		/* rechten Teilbaum ausgeben */
		avl_write(b->avl_rsohn, tiefe + 1);
		/* aktuellen Knoten ausgeben */
		for (i = 0; i < 4 * tiefe; i++)
			printf(" ");
		datensatz_ausgeben(b);
		printf("\n");
		/* linken Teilbaum ausgeben */
		avl_write(b->avl_lsohn, tiefe + 1);
	}
}

AVL_knoten *avl_suchen(SchluesselTyp s, AVL_baum b)
{
	AVL_knoten *links; /* wo kommt der Schluessel im linken Teilbaum vor ? */
	if (b == NULL) /* Suche erfolglos beendet */
		return NULL;
	else
		if (gleich(b->avl_daten.schluessel, s))
		{
		links = avl_suchen(s, b->avl_lsohn);
		if (links == NULL)
			return b; /* Schluessel kommt im linken Teilbaum nicht vor */
		else
			return links;

		}
		else /* noch nicht gefunden; weitersuchen */
			if (kleiner(s, b->avl_daten.schluessel))
				return avl_suchen(s, b->avl_lsohn);
			else
				return avl_suchen(s, b->avl_rsohn);
}

/* ------------------------------------------------ */
/* Minimum und Maximum zweier (ganzer) Zahlen */
#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))
/* ------------------------------------------------ */
/* Fuehrt in dem ueber v erreichbaren Binaerbaum
eine Linksrotation an der Wurzel durch. */
void avl_linksrotation(AVL_baum *v)
{
	AVL_baum w; /* rechter Sohn von v */
	int beta_v, beta_w; /* Balanceindizes in v, w */
	beta_v = (*v)->avl_beta;
	w = (*v)->avl_rsohn;
	beta_w = w->avl_beta;
	/* Rotation durchfuehren */
	(*v)->avl_rsohn = w->avl_lsohn;
	w->avl_lsohn = *v;
	*v = w;
	/* Balanceindizes aktualisieren */
	if (beta_w <= 0) {
		(*v)->avl_beta = MIN(beta_w + beta_v - 2, beta_w - 1);
		(*v)->avl_lsohn->avl_beta = beta_v - 1;
	}
	else {
		(*v)->avl_beta = MIN(beta_v - 2, beta_w - 1);
		(*v)->avl_lsohn->avl_beta = beta_v - beta_w - 1;
	}
}
/* ------------------------------------------------ */
/* Fuehrt in dem ueber v erreichbaren AVL-Baum eine
Rechtsrotation an der Wurzel durch. */
void avl_rechtsrotation(AVL_baum *v)
{
	AVL_baum w; /* linker Sohn von v */
	int beta_v, beta_w; /* Balanceindizes in v, w */
	beta_v = (*v)->avl_beta;
	w = (*v)->avl_lsohn;
	beta_w = w->avl_beta;
	/* Rotation durchfuehren */
	(*v)->avl_lsohn = w->avl_rsohn;
	w->avl_rsohn = *v;
	*v = w;
	/* Balanceindizes aktualisieren */
	if (beta_w >= 0) {
		(*v)->avl_beta = MAX(beta_w + 1, beta_v + beta_w + 1);
		(*v)->avl_rsohn->avl_beta = beta_v + 1;
	}
	else {
		(*v)->avl_rsohn->avl_beta = beta_v - beta_w + 1;
		(*v)->avl_beta = MAX(beta_w + 1, beta_v + 2);
	}
}
// ------------------------------------------------ 
// Prueft, ob in dem ueber v zugaenglichen Knoten
//die AVL-Balancierung verletzt ist und stellt sie
//ggf. wieder her. 
void avl_balancekorrektur(AVL_baum *v)
{
	if ((*v)->avl_beta > 1) {
		// AVL-Balancierung verletzt 
		if ((*v)->avl_rsohn->avl_beta == -1)
			// Doppelrotation 
			avl_rechtsrotation(&(*v)->avl_rsohn);
		avl_linksrotation(v);
	}
	else if ((*v)->avl_beta < -1) {
		// voellig symmetrisch 
		if ((*v)->avl_lsohn->avl_beta == 1)
			avl_linksrotation(&(*v)->avl_lsohn);
		avl_rechtsrotation(v);
	}
}

// ------------------------------------------------ //
// Fuegt zunaechst den Datensatz d in den AVL-Baum
//b ein. Dabei wird ein Pfad von der Wurzel bis zu
//einem Blatt durchlaufen. Auf dem durchlaufenen
//Pfad wird anschliessend die Balancierung
//geprueft und ggf. durch geeignete Rotationen
//wieder hergestellt. hoehe_geaendert gibt an, ob
//im gerade betrachteten Knoten die Balancierung
//noch geprueft werden muss.
void avl_einfuegen(Datensatz d, AVL_baum *b)
{
	static bool hoehe_geaendert;
	// hat sich die Hoehe des Teil-
	//baums (evtl.) geaendert? 
	bool hier_eingefuegt;
	// wurde Stelle zum Einfuegen in
	//aktueller Rekursion gefunden ?
	bool links_eingefuegt;
	// wurde im linken Teilbaum eingefuegt ? 
	if (*b == NULL) {
		// Stelle zum Einfuegen gefunden 
		hier_eingefuegt = true;
		(*b) = (AVL_knoten *)
			malloc(sizeof(AVL_knoten));
		(*b)->avl_daten = d;
		(*b)->avl_beta = 0;
		(*b)->avl_lsohn = NULL;
		(*b)->avl_rsohn = NULL;
		hoehe_geaendert = true;
	}
	else { // noch nicht gefunden; weitersuchen 
		hier_eingefuegt = false;
		if (kleiner(d.schluessel,(*b)->avl_daten.schluessel))
		{
			avl_einfuegen(d, &((*b)->avl_lsohn));
			links_eingefuegt = true;
		}
		else {
			avl_einfuegen(d, &((*b)->avl_rsohn));
			links_eingefuegt = false;
		}
		// nach dem Einfuegen entlang des
		//Pfades zur Wurzel den Balanceindex
		//korrigieren und ggf.die AVL -
		//Balancierung wiederherstellen 
		if (hoehe_geaendert
			&& (!hier_eingefuegt)) {
			// aendert sich die Hoehe ? 
			hoehe_geaendert = ((*b)->avl_beta == 0);
			// Balanceindex aktualisieren
			if (links_eingefuegt)
				(*b)->avl_beta = (*b)->avl_beta - 1;
			else
				(*b)->avl_beta = (*b)->avl_beta + 1;
			avl_balancekorrektur(b);
		}
	}
}

// ------------------------------------------------ */
/* Liefert einen Zeiger auf einen Knoten mit
minimalem Schluessel im AVL-Baum b und entfernt
diesen Knoten aus b. h_geaendert gibt an, ob
sich dabei die Hoehe des Baumes geaendert hat. */
void avl_minimum_entfernen(AVL_baum *b,
	AVL_knoten **min,
	bool *h_geaendert)
{
	bool hier_entfernt = false;
	if (*b == NULL) {
		printf("Fehler in avl_minimum_entfernen : "
			"b = NULL\n");
		exit(EXIT_FAILURE);
	}
	else
		if ((*b)->avl_lsohn == NULL) {
		/* Minimum erreicht */
		*min = *b;
		*b = (*b)->avl_rsohn; /* Knoten entfernen */
		*h_geaendert = true;
		hier_entfernt = true;
		}
		else /* weiter nach Minimum suchen */
			avl_minimum_entfernen(&(*b)->avl_lsohn,
			min, h_geaendert);
	// Balancekorrektur vornehmen
	if ((*h_geaendert) && (!hier_entfernt)) {
		/* Balanceindex aktualisieren;
		es wurde immer nach links gegangen */
		(*b)->avl_beta++;
		avl_balancekorrektur(b);
		/* hat sich die Hoehe geaendert ? */
		*h_geaendert = ((*b)->avl_beta == 0);
	}
}
/* ------------------------------------------------ */
/* Loescht den "am weitesten links stehenden"
 Knoten mit Schluessel s aus dem AVL-Baum b.
 Existiert kein solcher Knoten, so bleibt der
 Baum unveraendert.
 Zunaechst wird von der Wurzel aus nach dem zu
 loeschenden Knoten gesucht; dieser wird durch
 das Minimum seines rechten Sohnes ersetzt.
 Schliesslich wird auf dem Weg zurueck an die
 Wurzel die AVL-Balancierung geprueft und ggf.
 wiederhergestellt. Bei Verlassen der Prozedur
 gibt h_geaendert an, ob sich die Hoehe des
 gerade betrachteten Teilbaums geaendert hat (und
 damit auch im Vater des gerade besuchten Knotens
 die AVL-Balancierung geprueft werden muss). */
void avl_loeschen(SchluesselTyp s, AVL_baum *b)
{
	bool hier_geloescht;
	/* wurde in aktueller Rekursion geloescht ? */
	AVL_knoten *weg; /* wegfallender Knoten */
	static bool h_geaendert;
	/* hat sich Hoehe des Baumes geaendert ? */
	static bool weiter_links;
	/* wurde ein weiter links stehender Knoten
	mit dem gleichen Schluessel gefunden ? */
	bool links_gegangen;
	/* wurde im linken Teilbaum gesucht ? */
	AVL_knoten *min_r;
	/* zeigt auf das Minimum im rechten Teilbaum */
	bool h_r_geaendert;
	/* hat sich die Hoehe des rechten Teilbaumes
	beim Entfernen des Minimums geandert ? */
	if (*b == NULL)
		/* der gesuchte Knoten kommt nicht vor */
		h_geaendert = false;
	else {
		if (gleich((*b)->avl_daten.schluessel, s)) {
			/* Schluessel gefunden; gibt es einen
			noch weiter links stehenden ? */
			weiter_links = false;
			avl_loeschen(s, &(*b)->avl_lsohn);
			if (weiter_links)
				/* ja, und dieser wurde entfernt */
				hier_geloescht = false;
			else { /* nein; aktuellen Knoten loeschen */
				hier_geloescht = true;
				h_geaendert = true;
				weg = *b;
				if (weg->avl_lsohn == NULL)
					/* hoechstens rechter Sohn vorhan-
					den; Knoten problemlos entfernbar */
					* b = weg->avl_rsohn;
				else if (weg->avl_rsohn == NULL)
					/* nur linker Sohn vorhanden;
					Knoten problemlos entfernbar */
					* b = weg->avl_lsohn;
				else { /* beide Soehne vorhanden;
				Knoten durch Minimum des
				rechten Teilbaums ersetzen */
					avl_minimum_entfernen(&weg->avl_rsohn, &min_r, &h_r_geaendert);
					min_r->avl_lsohn = weg->avl_lsohn;
					min_r->avl_rsohn = weg->avl_rsohn;
					min_r->avl_beta = weg->avl_beta;
					*b = min_r;
					free(weg);
					h_geaendert = h_r_geaendert;
					hier_geloescht = true;
					links_gegangen = true;
				}
			}
			weiter_links = true;
		}
		else { /* (*b)->avl_daten.schluessel != s :
		weiter nach dem Schluessel suchen */
			hier_geloescht = false;
			links_gegangen =
				kleiner(s, (*b)->avl_daten.schluessel);
			if (links_gegangen)
				avl_loeschen(s, &(*b)->avl_lsohn);
			else
				avl_loeschen(s, &(*b)->avl_rsohn);
		}
	}
	/* Balanceindex aktualisieren */
	if ((h_geaendert) && (!hier_geloescht)) {
		if (links_gegangen)
			(*b)->avl_beta = (*b)->avl_beta + 1;
		else
			(*b)->avl_beta = (*b)->avl_beta - 1;
		avl_balancekorrektur(b);
		/* hat sich die Hoehe geaendert ? */
		h_geaendert = ((*b)->avl_beta == 0);
	}
}