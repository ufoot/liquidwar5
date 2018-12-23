/********************************************************************/
/*                                                                  */
/*            L   I  QQ  U U I DD    W   W  A  RR    555            */
/*            L   I Q  Q U U I D D   W   W A A R R   5              */
/*            L   I Q  Q U U I D D   W W W AAA RR    55             */
/*            L   I Q Q  U U I D D   WW WW A A R R     5            */
/*            LLL I  Q Q  U  I DD    W   W A A R R   55             */
/*                                                                  */
/*                             b                                    */
/*                             bb  y y                              */
/*                             b b yyy                              */
/*                             bb    y                              */
/*                                 yy                               */
/*                                                                  */
/*                     U U       FFF  O   O  TTT                    */
/*                     U U       F   O O O O  T                     */
/*                     U U TIRET FF  O O O O  T                     */
/*                     U U       F   O O O O  T                     */
/*                      U        F    O   O   T                     */
/*                                                                  */
/********************************************************************/

/*****************************************************************************/
/* Liquid War is a multiplayer wargame                                       */
/* Copyright (C) 1998-2018 Christian Mauduit                                 */
/*                                                                           */
/* This program is free software; you can redistribute it and/or modify      */
/* it under the terms of the GNU General Public License as published by      */
/* the Free Software Foundation; either version 2 of the License, or         */
/* (at your option) any later version.                                       */
/*                                                                           */
/* This program is distributed in the hope that it will be useful,           */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/* GNU General Public License for more details.                              */
/*                                                                           */
/* You should have received a copy of the GNU General Public License         */
/* along with this program; if not, write to the Free Software               */
/* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA */
/*                                                                           */
/* Liquid War homepage : https://ufoot.org/liquidwar/v5                   */
/* Contact author      : ufoot@ufoot.org                                     */
/*****************************************************************************/

/********************************************************************/
/* name          : langde.c                                         */
/* content       : handles German dependent stuff (locale)          */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>
#include <stdlib.h>

#include "lang.h"
#include "langde.h"
#include "base.h"

/*==================================================================*/
/* globals                                                          */
/*==================================================================*/

static char *LW_LANGDE_DATA[LW_LANG_NB_STRINGS];

/*------------------------------------------------------------------*/
/* initializes the string table                                     */
/*------------------------------------------------------------------*/
void
lw_langde_init ()
{
  memset (LW_LANGDE_DATA, 0, sizeof (LW_LANGDE_DATA));

  LW_LANGDE_DATA[LW_LANG_STRING_LANGUAGE] = "Deutsch";
  LW_LANGDE_DATA[LW_LANG_STRING_APPNAME] = "Liquid War " LW_VERSION;

  LW_LANGDE_DATA[LW_LANG_STRING_HELP_CREDITS] =
    "Idee: \n* Thomas Colcombet\n\nProgrammation: \n* Christian Mauduit\n\nMusik: \n* Tim Chadburn\n\nAndere Mitwirkende:\n* Alstar\n* Peter Wang\n* Cort Danger Stratton\n* Jan Gretschuskin (www.game-factor.de)\n* Mouse\n* Rene Stach\n* Roderick Schertler\n* Ryan D. Brown\n* Eduard Bloch\n* Michael Terry\n* Kasper Hviid\n* David Redick\n* Alexandre Pineau\n* Michael Wagner\n* Peter Williams\n* Jan Samohyl\n* Gavin\n* Dave Vasilevsky\n* 2 of Omega\n* 666-REFIZUL-666\n* Thomas Klausner\n* Joan Dolc\n* Mikhail Yakshin\n* Valerie Mauduit\n* Florence Mauduit\n* Joey Dodson\n* Jan Szenborn\n\nFuer weitere Informationen, siehe www.ufoot.org/liquidwar/v5\n";
  LW_LANGDE_DATA[LW_LANG_STRING_HELP_COPYRIGHT] =
    "Liquid War ist ein Kriegspiel fuer mehrere Teilnehmer.\nCopyright (C) 1998-2018 Christian Mauduit (ufoot@ufoot.org)\n\nDieses Programm ist freie Software. Sie koennen es unter den Bedingungen der GNU General Public License, wie von der Free Software Foundation veroeffentlicht, weitergeben und/oder modifizieren, entweder gemaess Version 2 der Lizenz oder (nach Ihrer Option) jeder spaeteren Version.\n\nDie Veroeffentlichung dieses Programms erfolgt in der Hoffnung, dass es Ihnen von Nutzen sein wird, aber OHNE IRGENDEINE GARANTIE, sogar ohne die implizite Garantie der MARKTREIFE oder der VERWENDBARKEIT FUER EINEN BESTIMMTEN ZWECK. Details finden Sie in der GNU General Public License.\n\nSie sollten eine Kopie der GNU General Public License zusammen mit diesem Programm erhalten haben. Falls nicht, schreiben Sie an die Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301, USA.\n\nFuer weitere Informationen, siehe www.ufoot.org/liquidwar/v5\n";
  /*
   * Stupid Microsoft VC++ can't handle strings over 2048 chars correctly,
   * (compiler error C2026) so we had to split this one...
   */
  LW_LANGDE_DATA[LW_LANG_STRING_HELP_LICENSE] =
    "Deutsche Uebersetzung der\nGNU General Public License\n\nErstellt im Auftrag der S.u.S.E. GmbH http://www.suse.de\nvon Katja Lachmann Uebersetzungen,\nueberarbeitet von Peter Gerwinski, G-N-U GmbH http://www.g-n-u.de\n(31. Oktober 1996, 4. Juni 2000)\n\nDiese Uebersetzung wird mit der Absicht angeboten, das Verstaendnis der GNU General Public License (GNU-GPL) zu erleichtern. Es handelt sich jedoch nicht um eine offizielle oder im rechtlichen Sinne anerkannte Uebersetzung.\n\nDie Free Software Foundation (FSF) ist nicht der Herausgeber dieser Uebersetzung, und sie hat diese Uebersetzung auch nicht als rechtskraeftigen Ersatz fuer die Original-GNU-GPL anerkannt. Da die Uebersetzung nicht sorgfaeltig von Anwaelten ueberprueft wurde, koennen die Uebersetzer nicht garantieren, dass die Uebersetzung die rechtlichen Aussagen der GNU-GPL exakt wiedergibt. Wenn Sie sichergehen wollen, dass von Ihnen geplante Aktivitaeten im Sinne der GNU-GPL gestattet sind, "
    "halten Sie sich bitte an die englischsprachige Originalversion.\n\nDie Free Software Foundation moechte Sie darum bitten, diese Uebersetzung nicht als offizielle Lizenzbedingungen fuer von Ihnen geschriebene Programme zu verwenden. Bitte benutzen Sie hierfuer stattdessen die von der Free Software Foundation herausgegebene englischsprachige Originalversion.\n\nThis is a translation of the GNU General Public License into German. This translation is distributed in the hope that it will facilitate understanding, but it is not an official or legally approved translation.\n\nThe Free Software Foundation is not the publisher of this translation and has not approved it as a legal substitute for the authentic GNU General Public License. The translation has not been reviewed carefully by lawyers, and therefore the translator cannot be sure that it exactly represents the legal meaning of the GNU General Public License. If you wish to be sure whether your planned activities are permitted "
    "by the GNU General Public License, please refer to the authentic English version.\n\nThe Free Software Foundation strongly urges you not to use this translation as the official distribution terms for your programs; instead, please use the authentic English version published by the Free Software Foundation.\n\nGNU General Public License\n\nDeutsche Uebersetzung der Version 2, Juni 1991\n\nCopyright (c) 1989, 1991 Free Software Foundation, Inc.\n\n51 Franklin St, Fifth Floor, Boston, MA  02110-1301, USA\n\npeter@gerwinski.de\n\nEs ist jedermann gestattet, diese Lizenzurkunde zu vervielfaeltigen und unveraenderte Kopien zu verbreiten; Aenderungen sind jedoch nicht erlaubt.\n\nDiese Uebersetzung ist kein rechtskraeftiger Ersatz fuer die englischsprachige Originalversion!\n\nVorwort\n\nDie meisten Softwarelizenzen sind daraufhin entworfen worden, Ihnen die Freiheit zu nehmen, die Software weiterzugeben und zu veraendern. Im Gegensatz dazu soll Ihnen die GNU General Public License , "
    "die Allgemeine Oeffentliche GNU-Lizenz, ebendiese Freiheit garantieren. Sie soll sicherstellen, dass die Software fuer alle Benutzer frei ist. Diese Lizenz gilt fuer den Grossteil der von der Free Software Foundation herausgegebenen Software und fuer alle anderen Programme, deren Autoren ihr Datenwerk dieser Lizenz unterstellt haben. Auch Sie koennen diese Moeglichkeit der Lizenzierung fuer Ihre Programme anwenden. (Ein anderer Teil der Software der Free Software Foundation unterliegt stattdessen der GNU Library General Public License , der Allgemeinen Oeffentlichen GNU-Lizenz fuer Bibliotheken.) [Mittlerweile wurde die GNU Library Public License von der GNU Lesser Public License abgeloest - Anmerkung des Uebersetzers.]\n\nDie Bezeichnung ,,freie`` Software bezieht sich auf Freiheit, nicht auf den Preis. Unsere Lizenzen sollen Ihnen die Freiheit garantieren, Kopien freier Software zu verbreiten (und etwas fuer diesen Service zu berechnen, wenn Sie moechten), die Moeglichkeit, "
    "die Software im Quelltext zu erhalten oder den Quelltext auf Wunsch zu bekommen. Die Lizenzen sollen garantieren, dass Sie die Software aendern oder Teile davon in neuen freien Programmen verwenden duerfen - und dass Sie wissen, dass Sie dies alles tun duerfen.\n\nUm Ihre Rechte zu schuetzen, muessen wir Einschraenkungen machen, die es jedem verbieten, Ihnen diese Rechte zu verweigern oder Sie aufzufordern, auf diese Rechte zu verzichten. Aus diesen Einschraenkungen folgen bestimmte Verantwortlichkeiten fuer Sie, wenn Sie Kopien der Software verbreiten oder sie veraendern.\n\nBeispielsweise muessen Sie den Empfaengern alle Rechte gewaehren, die Sie selbst haben, wenn Sie - kostenlos oder gegen Bezahlung - Kopien eines solchen Programms verbreiten. Sie muessen sicherstellen, dass auch die Empfaenger den Quelltext erhalten bzw. erhalten koennen. Und Sie muessen ihnen diese Bedingungen zeigen, damit sie ihre Rechte kennen.\n\nWir schuetzen Ihre Rechte in zwei Schritten: "
    "(1) Wir stellen die Software unter ein Urheberrecht (Copyright), und (2) wir bieten Ihnen diese Lizenz an, die Ihnen das Recht gibt, die Software zu vervielfaeltigen, zu verbreiten und/oder zu veraendern.\n\nUm die Autoren und uns zu schuetzen, wollen wir darueberhinaus sicherstellen, dass jeder erfaehrt, dass fuer diese freie Software keinerlei Garantie besteht. Wenn die Software von jemand anderem modifiziert und weitergegeben wird, moechten wir, dass die Empfaenger wissen, dass sie nicht das Original erhalten haben, damit irgendwelche von anderen verursachte Probleme nicht den Ruf des urspruenglichen Autors schaedigen.\n\nSchliesslich und endlich ist jedes freie Programm permanent durch Software-Patente bedroht. Wir moechten die Gefahr ausschliessen, dass Distributoren eines freien Programms individuell Patente lizensieren - mit dem Ergebnis, dass das Programm proprietaer wuerde. Um dies zu verhindern, haben wir klargestellt, dass jedes Patent entweder fuer freie Benutzung "
    "durch jedermann lizenziert werden muss oder ueberhaupt nicht lizenziert werden darf.\n\nEs folgen die genauen Bedingungen fuer die Vervielfaeltigung, Verbreitung und Bearbeitung:\n\nAllgemeine Oeffentliche GNU-Lizenz\nBedingungen fuer die Vervielfaeltigung,\nVerbreitung und Bearbeitung\n\n0. Diese Lizenz gilt fuer jedes Programm und jedes andere Datenwerk, in dem ein entsprechender Vermerk des Copyright-Inhabers darauf hinweist, dass das Datenwerk unter den Bestimmungen dieser General Public License verbreitet werden darf. Im folgenden wird jedes derartige Programm oder Datenwerk als ,,das Programm`` bezeichnet; die Formulierung ,,auf dem Programm basierendes Datenwerk`` bezeichnet das Programm sowie jegliche Bearbeitung des Programms im urheberrechtlichen Sinne, also ein Datenwerk, welches das Programm, auch auszugsweise, sei es unveraendert oder veraendert und/oder in eine andere Sprache uebersetzt, enthaelt. (Im folgenden wird die Uebersetzung ohne Einschraenkung als ,,Bearbeitung`` eingestuft.) "
    "Jeder Lizenznehmer wird im folgenden als ,,Sie`` angesprochen.\n\nAndere Handlungen als Vervielfaeltigung, Verbreitung und Bearbeitung werden von dieser Lizenz nicht beruehrt; sie fallen nicht in ihren Anwendungsbereich. Der Vorgang der Ausfuehrung des Programms wird nicht eingeschraenkt, und die Ausgaben des Programms unterliegen dieser Lizenz nur, wenn der Inhalt ein auf dem Programm basierendes Datenwerk darstellt (unabhaengig davon, dass die Ausgabe durch die Ausfuehrung des Programmes erfolgte). Ob dies zutrifft, haengt von den Funktionen des Programms ab.\n\n1. Sie duerfen auf beliebigen Medien unveraenderte Kopien des Quelltextes des Programms, wie sie ihn erhalten haben, anfertigen und verbreiten. Voraussetzung hierfuer ist, dass Sie mit jeder Kopie einen entsprechenden Copyright-Vermerk sowie einen Haftungsausschluss veroeffentlichen, alle Vermerke, die sich auf diese Lizenz und das Fehlen einer Garantie beziehen, unveraendert lassen "
    "und desweiteren allen anderen Empfaengern des Programms zusammen mit dem Programm eine Kopie dieser Lizenz zukommen lassen.\n\nSie duerfen fuer den eigentlichen Kopiervorgang eine Gebuehr verlangen. Wenn Sie es wuenschen, duerfen Sie auch gegen Entgeld eine Garantie fuer das Programm anbieten.\n\n2. Sie duerfen Ihre Kopie(n) des Programms oder eines Teils davon veraendern, wodurch ein auf dem Programm basierendes Datenwerk entsteht; Sie duerfen derartige Bearbeitungen unter den Bestimmungen von Paragraph 1 vervielfaeltigen und verbreiten, vorausgesetzt, dass zusaetzlich alle im folgenden genannten Bedingungen erfuellt werden:\n\n1.\nSie muessen die veraenderten Dateien mit einem auffaelligen Vermerk versehen, der auf die von Ihnen vorgenommene Modifizierung und das Datum jeder Aenderung hinweist.\n2.\nSie muessen dafuer sorgen, dass jede von Ihnen verbreitete oder veroeffentlichte Arbeit, die ganz oder teilweise von dem Programm oder Teilen davon abgeleitet ist, "
    "Dritten gegenueber als Ganzes unter den Bedingungen dieser Lizenz ohne Lizenzgebuehren zur Verfuegung gestellt wird.\n3.\nWenn das veraenderte Programm normalerweise bei der Ausfuehrung interaktiv Kommandos einliest, muessen Sie dafuer sorgen, dass es, wenn es auf dem ueblichsten Wege fuer solche interaktive Nutzung gestartet wird, eine Meldung ausgibt oder ausdruckt, die einen geeigneten Copyright-Vermerk enthaelt sowie einen Hinweis, dass es keine Gewaehrleistung gibt (oder anderenfalls, dass Sie Garantie leisten), und dass die Benutzer das Programm unter diesen Bedingungen weiter verbreiten duerfen. Auch muss der Benutzer darauf hingewiesen werden, wie er eine Kopie dieser Lizenz ansehen kann. (Ausnahme: Wenn das Programm selbst interaktiv arbeitet, aber normalerweise keine derartige Meldung ausgibt, muss Ihr auf dem Programm basierendes Datenwerk auch keine solche Meldung ausgeben).\n\nDiese Anforderungen gelten fuer das bearbeitete Datenwerk als Ganzes. "
    "Wenn identifizierbare Teile des Datenwerkes nicht von dem Programm abgeleitet sind und vernuenftigerweise als unabhaengige und eigenstaendige Datenwerke fuer sich selbst zu betrachten sind, dann gelten diese Lizenz und ihre Bedingungen nicht fuer die betroffenen Teile, wenn Sie diese als eigenstaendige Datenwerke weitergeben. Wenn Sie jedoch dieselben Abschnitte als Teil eines Ganzen weitergeben, das ein auf dem Programm basierendes Datenwerk darstellt, dann muss die Weitergabe des Ganzen nach den Bedingungen dieser Lizenz erfolgen, deren Bedingungen fuer weitere Lizenznehmer somit auf das gesamte Ganze ausgedehnt werden - und somit auf jeden einzelnen Teil, unabhaengig vom jeweiligen Autor.\n\nSomit ist es nicht die Absicht dieses Abschnittes, Rechte fuer Datenwerke in Anspruch zu nehmen oder Ihnen die Rechte fuer Datenwerke streitig zu machen, die komplett von Ihnen geschrieben wurden; vielmehr ist es die Absicht, die Rechte zur Kontrolle der Verbreitung von Datenwerken, "
    "die auf dem Programm basieren oder unter seiner auszugsweisen Verwendung zusammengestellt worden sind, auszuueben.\n\nFerner bringt auch das einfache Zusammenlegen eines anderen Datenwerkes, das nicht auf dem Programm basiert, mit dem Programm oder einem auf dem Programm basierenden Datenwerk auf ein- und demselben Speicher- oder Vertriebsmedium dieses andere Datenwerk nicht in den Anwendungsbereich dieser Lizenz.\n\n3. Sie duerfen das Programm (oder ein darauf basierendes Datenwerk gemaess Paragraph 2) als Objectcode oder in ausfuehrbarer Form unter den Bedingungen der Paragraphen 1 und 2 kopieren und weitergeben - vorausgesetzt, dass Sie ausserdem eine der folgenden Leistungen erbringen:\n\n1.\nLiefern Sie das Programm zusammen mit dem vollstaendigen zugehoerigen maschinenlesbaren Quelltext auf einem fuer den Datenaustausch ueblichen Medium aus, wobei die Verteilung unter den Bedingungen der Paragraphen 1 und 2 erfolgen muss. Oder:\n2.\nLiefern Sie das Programm "
    "zusammen mit einem mindestens drei Jahre lang gueltigen schriftlichen Angebot aus, jedem Dritten eine vollstaendige maschinenlesbare Kopie des Quelltextes zur Verfuegung zu stellen - zu nicht hoeheren Kosten als denen, die durch den physikalischen Kopiervorgang anfallen -, wobei der Quelltext unter den Bedingungen der Paragraphen 1 und 2 auf einem fuer den Datenaustausch ueblichen Medium weitergegeben wird. Oder:\n3.\nLiefern Sie das Programm zusammen mit dem schriftlichen Angebot der Zurverfuegungstellung des Quelltextes aus, das Sie selbst erhalten haben. (Diese Alternative ist nur fuer nicht-kommerzielle Verbreitung zulaessig und nur, wenn Sie das Programm als Objectcode oder in ausfuehrbarer Form mit einem entsprechenden Angebot gemaess Absatz b erhalten haben.)\n\nUnter dem Quelltext eines Datenwerkes wird diejenige Form des Datenwerkes verstanden, die fuer Bearbeitungen vorzugsweise verwendet wird. Fuer ein ausfuehrbares Programm bedeutet ,,der komplette Quelltext``: "
    "Der Quelltext aller im Programm enthaltenen Module einschliesslich aller zugehoerigen Modulschnittstellen-Definitionsdateien sowie der zur Compilation und Installation verwendeten Skripte. Als besondere Ausnahme jedoch braucht der verteilte Quelltext nichts von dem zu enthalten, was ueblicherweise (entweder als Quelltext oder in binaerer Form) zusammen mit den Hauptkomponenten des Betriebssystems (Kernel, Compiler usw.) geliefert wird, unter dem das Programm laeuft - es sei denn, diese Komponente selbst gehoert zum ausfuehrbaren Programm.\n\nWenn die Verbreitung eines ausfuehrbaren Programms oder von Objectcode dadurch erfolgt, dass der Kopierzugriff auf eine dafuer vorgesehene Stelle gewaehrt wird, so gilt die Gewaehrung eines gleichwertigen Zugriffs auf den Quelltext als Verbreitung des Quelltextes, auch wenn Dritte nicht dazu gezwungen sind, den Quelltext zusammen mit dem Objectcode zu kopieren.\n\n4. Sie duerfen das Programm nicht vervielfaeltigen, veraendern, weiter lizenzieren oder verbreiten, "
    "sofern es nicht durch diese Lizenz ausdruecklich gestattet ist. Jeder anderweitige Versuch der Vervielfaeltigung, Modifizierung, Weiterlizenzierung und Verbreitung ist nichtig und beendet automatisch Ihre Rechte unter dieser Lizenz. Jedoch werden die Lizenzen Dritter, die von Ihnen Kopien oder Rechte unter dieser Lizenz erhalten haben, nicht beendet, solange diese die Lizenz voll anerkennen und befolgen.\n\n5. Sie sind nicht verpflichtet, diese Lizenz anzunehmen, da Sie sie nicht unterzeichnet haben. Jedoch gibt Ihnen nichts anderes die Erlaubnis, das Programm oder von ihm abgeleitete Datenwerke zu veraendern oder zu verbreiten. Diese Handlungen sind gesetzlich verboten, wenn Sie diese Lizenz nicht anerkennen. Indem Sie das Programm (oder ein darauf basierendes Datenwerk) veraendern oder verbreiten, erklaeren Sie Ihr Einverstaendnis mit dieser Lizenz und mit allen ihren Bedingungen bezueglich der Vervielfaeltigung, Verbreitung und Veraenderung des Programms "
    "oder eines darauf basierenden Datenwerks.\n\n6. Jedesmal, wenn Sie das Programm (oder ein auf dem Programm basierendes Datenwerk) weitergeben, erhaelt der Empfaenger automatisch vom urspruenglichen Lizenzgeber die Lizenz, das Programm entsprechend den hier festgelegten Bestimmungen zu vervielfaeltigen, zu verbreiten und zu veraendern. Sie duerfen keine weiteren Einschraenkungen der Durchsetzung der hierin zugestandenen Rechte des Empfaengers vornehmen. Sie sind nicht dafuer verantwortlich, die Einhaltung dieser Lizenz durch Dritte durchzusetzen.\n\n7. Sollten Ihnen infolge eines Gerichtsurteils, des Vorwurfs einer Patentverletzung oder aus einem anderen Grunde (nicht auf Patentfragen begrenzt) Bedingungen (durch Gerichtsbeschluss, Vergleich oder anderweitig) auferlegt werden, die den Bedingungen dieser Lizenz widersprechen, so befreien Sie diese Umstaende nicht von den Bestimmungen dieser Lizenz. Wenn es Ihnen nicht moeglich ist, "
    "das Programm unter gleichzeitiger Beachtung der Bedingungen in dieser Lizenz und Ihrer anderweitigen Verpflichtungen zu verbreiten, dann duerfen Sie als Folge das Programm ueberhaupt nicht verbreiten. Wenn zum Beispiel ein Patent nicht die gebuehrenfreie Weiterverbreitung des Programms durch diejenigen erlaubt, die das Programm direkt oder indirekt von Ihnen erhalten haben, dann besteht der einzige Weg, sowohl das Patentrecht als auch diese Lizenz zu befolgen, darin, ganz auf die Verbreitung des Programms zu verzichten.\n\nSollte sich ein Teil dieses Paragraphen als ungueltig oder unter bestimmten Umstaenden nicht durchsetzbar erweisen, so soll dieser Paragraph seinem Sinne nach angewandt werden; im uebrigen soll dieser Paragraph als Ganzes gelten.\n\nZweck dieses Paragraphen ist nicht, Sie dazu zu bringen, irgendwelche Patente oder andere Eigentumsansprueche zu verletzen oder die Gueltigkeit solcher Ansprueche zu bestreiten; dieser Paragraph hat einzig den Zweck, "
    "die Integritaet des Verbreitungssystems der freien Software zu schuetzen, das durch die Praxis oeffentlicher Lizenzen verwirklicht wird. Viele Leute haben grosszuegige Beitraege zu dem grossen Angebot der mit diesem System verbreiteten Software im Vertrauen auf die konsistente Anwendung dieses Systems geleistet; es liegt am Autor/Geber, zu entscheiden, ob er die Software mittels irgendeines anderen Systems verbreiten will; ein Lizenznehmer hat auf diese Entscheidung keinen Einfluss.\n\nDieser Paragraph ist dazu gedacht, deutlich klarzustellen, was als Konsequenz aus dem Rest dieser Lizenz betrachtet wird.\n\n8. Wenn die Verbreitung und/oder die Benutzung des Programms in bestimmten Staaten entweder durch Patente oder durch urheberrechtlich geschuetzte Schnittstellen eingeschraenkt ist, kann der Urheberrechtsinhaber, der das Programm unter diese Lizenz gestellt hat, eine explizite geographische Begrenzung der Verbreitung angeben, in der diese Staaten ausgeschlossen werden, "
    "so dass die Verbreitung nur innerhalb und zwischen den Staaten erlaubt ist, die nicht ausgeschlossen sind. In einem solchen Fall beinhaltet diese Lizenz die Beschraenkung, als waere sie in diesem Text niedergeschrieben.\n\n9. Die Free Software Foundation kann von Zeit zu Zeit ueberarbeitete und/oder neue Versionen der General Public License veroeffentlichen. Solche neuen Versionen werden vom Grundprinzip her der gegenwaertigen entsprechen, koennen aber im Detail abweichen, um neuen Problemen und Anforderungen gerecht zu werden.\n\nJede Version dieser Lizenz hat eine eindeutige Versionsnummer. Wenn in einem Programm angegeben wird, dass es dieser Lizenz in einer bestimmten Versionsnummer oder ,,jeder spaeteren Version`` (``any later version'') unterliegt, so haben Sie die Wahl, entweder den Bestimmungen der genannten Version zu folgen oder denen jeder beliebigen spaeteren Version, die von der Free Software Foundation veroeffentlicht wurde. Wenn das Programm keine Versionsnummer angibt, "
    "koennen Sie eine beliebige Version waehlen, die je von der Free Software Foundation veroeffentlicht wurde.\n\n10. Wenn Sie den Wunsch haben, Teile des Programms in anderen freien Programmen zu verwenden, deren Bedingungen fuer die Verbreitung anders sind, schreiben Sie an den Autor, um ihn um die Erlaubnis zu bitten. Fuer Software, die unter dem Copyright der Free Software Foundation steht, schreiben Sie an die Free Software Foundation ; wir machen zu diesem Zweck gelegentlich Ausnahmen. Unsere Entscheidung wird von den beiden Zielen geleitet werden, zum einen den freien Status aller von unserer freien Software abgeleiteten Datenwerke zu erhalten und zum anderen das gemeinschaftliche Nutzen und Wiederverwenden von Software im allgemeinen zu foerdern.\n\nKeine Gewaehrleistung\n\n11. Da das Programm ohne jegliche Kosten lizenziert wird, besteht keinerlei Gewaehrleistung fuer das Programm, soweit dies gesetzlich zulaessig ist. Sofern nicht anderweitig schriftlich bestaetigt, "
    "stellen die Copyright-Inhaber und/oder Dritte das Programm so zur Verfuegung, ,,wie es ist``, ohne irgendeine Gewaehrleistung, weder ausdruecklich noch implizit, einschliesslich - aber nicht begrenzt auf - Marktreife oder Verwendbarkeit fuer einen bestimmten Zweck. Das volle Risiko bezueglich Qualitaet und Leistungsfaehigkeit des Programms liegt bei Ihnen. Sollte sich das Programm als fehlerhaft herausstellen, liegen die Kosten fuer notwendigen Service, Reparatur oder Korrektur bei Ihnen.\n\n12. In keinem Fall, ausser wenn durch geltendes Recht gefordert oder schriftlich zugesichert, ist irgendein Copyright-Inhaber oder irgendein Dritter, der das Programm wie oben erlaubt modifiziert oder verbreitet hat, Ihnen gegenueber fuer irgendwelche Schaeden haftbar, einschliesslich jeglicher allgemeiner oder spezieller Schaeden, Schaeden durch Seiteneffekte (Nebenwirkungen) oder Folgeschaeden, die aus der Benutzung des Programms oder der Unbenutzbarkeit des Programms folgen "
    "(einschliesslich - aber nicht beschraenkt auf - Datenverluste, fehlerhafte Verarbeitung von Daten, Verluste, die von Ihnen oder anderen getragen werden muessen, oder dem Unvermoegen des Programms, mit irgendeinem anderen Programm zusammenzuarbeiten), selbst wenn ein Copyright-Inhaber oder Dritter ueber die Moeglichkeit solcher Schaeden unterrichtet worden war.\n\nEnde der Bedingungen\n\nAnhang: Wie Sie diese Bedingungen auf Ihre eigenen,\nneuen Programme anwenden koennen\n\nWenn Sie ein neues Programm entwickeln und wollen, dass es vom groesstmoeglichen Nutzen fuer die Allgemeinheit ist, dann erreichen Sie das am besten, indem Sie es zu freier Software machen, die jeder unter diesen Bestimmungen weiterverbreiten und veraendern kann.\n\nUm dies zu erreichen, fuegen Sie die folgenden Vermerke zu Ihrem Programm hinzu. Am sichersten ist es, sie an den Anfang einer jeden Quelldatei zu stellen, um den Gewaehrleistungsausschluss moeglichst deutlich darzustellen; "
    "zumindest aber sollte jede Datei eine Copyright-Zeile besitzen sowie einen kurzen Hinweis darauf, wo die vollstaendigen Vermerke zu finden sind.\n\n[eine Zeile mit dem Programmnamen und einer kurzen Beschreibung]\nCopyright (C) [Jahr]  [Name des Autors]\n\nThis program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.\n\nThis program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.\n\nYou should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301, USA.\n\nAuf Deutsch:\n\n[eine Zeile mit dem Programmnamen "
    "und einer kurzen Beschreibung]\nCopyright (C) [Jahr]  [Name des Autors]\n\nDieses Programm ist freie Software. Sie koennen es unter den Bedingungen der GNU General Public License, wie von der Free Software Foundation veroeffentlicht, weitergeben und/oder modifizieren, entweder gemaess Version 2 der Lizenz oder (nach Ihrer Option) jeder spaeteren Version.\n\nDie Veroeffentlichung dieses Programms erfolgt in der Hoffnung, dass es Ihnen von Nutzen sein wird, aber OHNE IRGENDEINE GARANTIE, sogar ohne die implizite Garantie der MARKTREIFE oder der VERWENDBARKEIT FUER EINEN BESTIMMTEN ZWECK. Details finden Sie in der GNU General Public License.\n\nSie sollten eine Kopie der GNU General Public License zusammen mit diesem Programm erhalten haben. Falls nicht, schreiben Sie an die Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301, USA.\n\nFuegen Sie auch einen kurzen Hinweis hinzu, wie Sie elektronisch und per Brief erreichbar sind.\n\nWenn Ihr Programm interaktiv ist, "
    "sorgen Sie dafuer, dass es nach dem Start einen kurzen Vermerk ausgibt:\n\nversion 69, Copyright (C) [Jahr]  [Name des Autors] Gnomovision comes with ABSOLUTELY NO WARRANTY; for details type `show w'. This is free software, and you are welcome to redistribute it under certain conditions; type `show c' for details.\n\nAuf Deutsch:\n\nVersion 69, Copyright (C) [Jahr]  [Name des Autors] Fuer Gnomovision besteht KEINERLEI GARANTIE; geben Sie `show w' fuer Details ein. Gnonovision ist freie Software, die Sie unter bestimmten Bedingungen weitergeben duerfen; geben Sie `show c' fuer Details ein.\n\nDie hypothetischen Kommandos `show w' und `show c' sollten die entsprechenden Teile der GNU-GPL anzeigen. Natuerlich koennen die von Ihnen verwendeten Kommandos anders heissen als `show w' und `show c'; es koennten auch Mausklicks oder Menuepunkte sein - was immer am besten in Ihr Programm passt.\n\nSoweit vorhanden, sollten Sie auch Ihren Arbeitgeber (wenn Sie als Programmierer arbeiten) "
    "oder Ihre Schule einen Copyright-Verzicht fuer das Programm unterschreiben lassen. Hier ein Beispiel. Die Namen muessen Sie natuerlich aendern.\n\nYoyodyne, Inc., hereby disclaims all copyright interest in the program `Gnomovision' (which makes passes at compilers) written by James Hacker.\n\n[Unterschrift von Ty Coon], 1 April 1989\nTy Coon, President of Vice\n\nAuf Deutsch:\n\nDie Yoyodyne GmbH erhebt keinen urheberrechtlichen Anspruch auf das von James Hacker geschriebene Programm ,Gnomovision` (einem Schrittmacher fuer Compiler).\n\n[Unterschrift von Ty Coon], 1. April 1989\nTy Coon, Vizepraesident\n\nDiese General Public License gestattet nicht die Einbindung des Programms in proprietaere Programme. Ist Ihr Programm eine Funktionsbibliothek, so kann es sinnvoller sein, das Binden proprietaerer Programme mit dieser Bibliothek zu gestatten. Wenn Sie dies tun wollen, sollten Sie die GNU Library General Public License anstelle dieser Lizenz verwenden.";

  LW_LANGDE_DATA[LW_LANG_STRING_HELP_VERSION] =
    "Liquid War von U-Foot\n\nv" LW_VERSION " \n\ncompiled am " __DATE__
    " um " __TIME__
    "\n\nWeitere Informationen auf www.ufoot.org/liquidwar/v5\n";

  LW_LANGDE_DATA[LW_LANG_STRING_MENU_PLAY] = "Spielen";
  LW_LANGDE_DATA[LW_LANG_STRING_MENU_NETGAME] = "Netzwerk";
  LW_LANGDE_DATA[LW_LANG_STRING_MENU_MAP] = "Karte";
  LW_LANGDE_DATA[LW_LANG_STRING_MENU_TEAMS] = "Teams";
  LW_LANGDE_DATA[LW_LANG_STRING_MENU_OPTIONS] = "Optionen";
  LW_LANGDE_DATA[LW_LANG_STRING_MENU_ABOUT] = "Informationen";
  LW_LANGDE_DATA[LW_LANG_STRING_MENU_EXIT] = "Quit";
  LW_LANGDE_DATA[LW_LANG_STRING_MENU_EXITDOS] = "Exit";
  LW_LANGDE_DATA[LW_LANG_STRING_MENU_EXITWINDOWS] = "Quit";
  LW_LANGDE_DATA[LW_LANG_STRING_MENU_EXITUNIX] = "Quit";
  LW_LANGDE_DATA[LW_LANG_STRING_MENU_BACK] = "Zurueck zu LW";

  LW_LANGDE_DATA[LW_LANG_STRING_DIALOG_QUIT] = "Quit";
  LW_LANGDE_DATA[LW_LANG_STRING_DIALOG_BACK] = "Zurueck";
  LW_LANGDE_DATA[LW_LANG_STRING_DIALOG_PLAY] = "Spielen";
  LW_LANGDE_DATA[LW_LANG_STRING_DIALOG_MAIN] = "Menue";

  LW_LANGDE_DATA[LW_LANG_STRING_PLAY_GETREADY] = "Macht euch bereit!";

  LW_LANGDE_DATA[LW_LANG_STRING_NETGAME_STARTGAME] = "Starten";
  LW_LANGDE_DATA[LW_LANG_STRING_NETGAME_SERVERADDR] = "Server Addr.";
  LW_LANGDE_DATA[LW_LANG_STRING_NETGAME_SERVERPORT] = "Server Port";
  LW_LANGDE_DATA[LW_LANG_STRING_NETGAME_SEARCH] = "Suche Internetspiele";
  LW_LANGDE_DATA[LW_LANG_STRING_NETGAME_PASSWORD] = "Password";
  LW_LANGDE_DATA[LW_LANG_STRING_NETGAME_WEAKPASSWORD] =
    "*) Achtung! Waehle ein schwaches Wort wie \"hallo\", welches auf deiner Festplatte gespecihert ist und sende es zum Server..";

  LW_LANGDE_DATA[LW_LANG_STRING_LANGUAGE_AUTO] = "Automatisch";

  LW_LANGDE_DATA[LW_LANG_STRING_TEAM_OFF] = "Inaktiv";
  LW_LANGDE_DATA[LW_LANG_STRING_TEAM_HUMAN] = "Mensch";
  LW_LANGDE_DATA[LW_LANG_STRING_TEAM_CPU] = "CPU";

  LW_LANGDE_DATA[LW_LANG_STRING_OPTIONS_GRAPHICS] = "Grafik";
  LW_LANGDE_DATA[LW_LANG_STRING_OPTIONS_SOUND] = "Sound";
  LW_LANGDE_DATA[LW_LANG_STRING_OPTIONS_RULES] = "Regeln";
  LW_LANGDE_DATA[LW_LANG_STRING_OPTIONS_SPEEDS] = "Geschwindigkeit";
  LW_LANGDE_DATA[LW_LANG_STRING_OPTIONS_CONTROLS] = "Steuerung";
  LW_LANGDE_DATA[LW_LANG_STRING_OPTIONS_LANGUAGE] = "Sprache";
  LW_LANGDE_DATA[LW_LANG_STRING_OPTIONS_DEFAULT] = "Standard";

  LW_LANGDE_DATA[LW_LANG_STRING_GRAPHICS_GAME] = "Spiel";
  LW_LANGDE_DATA[LW_LANG_STRING_GRAPHICS_MENU] = "Menue";
  LW_LANGDE_DATA[LW_LANG_STRING_GRAPHICS_ON] = "Ja";
  LW_LANGDE_DATA[LW_LANG_STRING_GRAPHICS_OFF] = "Nein";
  LW_LANGDE_DATA[LW_LANG_STRING_GRAPHICS_FULLSCREEN] = "Vollbild";
  LW_LANGDE_DATA[LW_LANG_STRING_GRAPHICS_WINDOWED] = "Fenster";
  LW_LANGDE_DATA[LW_LANG_STRING_GRAPHICS_ADVANCED] = "erw. Optionen";
  LW_LANGDE_DATA[LW_LANG_STRING_GRAPHICS_VIDEOMODE] = "Videomodus";
  LW_LANGDE_DATA[LW_LANG_STRING_GRAPHICS_BRIGHTNESS] = "Brightness";
  LW_LANGDE_DATA[LW_LANG_STRING_GRAPHICS_VIEWPORTSIZE] = "Portgroesse";
  LW_LANGDE_DATA[LW_LANG_STRING_GRAPHICS_PAGEFLIPPING] = "\"Page flipping\"";
  LW_LANGDE_DATA[LW_LANG_STRING_GRAPHICS_WAVES] = "Wellen (F4)";

  LW_LANGDE_DATA[LW_LANG_STRING_VOLUME_SFX] = "FX Sound";
  LW_LANGDE_DATA[LW_LANG_STRING_VOLUME_CLICK] = "Click";
  LW_LANGDE_DATA[LW_LANG_STRING_VOLUME_GAMEWATER] = "Spielwasser";
  LW_LANGDE_DATA[LW_LANG_STRING_VOLUME_MENUWATER] = "Wassermenue";
  LW_LANGDE_DATA[LW_LANG_STRING_VOLUME_MUSIC] = "Musik";

  LW_LANGDE_DATA[LW_LANG_STRING_RULES_TIME] = "Zeit";
  LW_LANGDE_DATA[LW_LANG_STRING_RULES_ADVANCED] = "erw. Optionen";
  LW_LANGDE_DATA[LW_LANG_STRING_RULES_ARMYSIZE] = "Armeegroesse";
  LW_LANGDE_DATA[LW_LANG_STRING_RULES_CURSOR] = "Cursor";
  LW_LANGDE_DATA[LW_LANG_STRING_RULES_DEFAULT] = "Standard";
  LW_LANGDE_DATA[LW_LANG_STRING_RULES_MINMAPRES] = "Min ";

  LW_LANGDE_DATA[LW_LANG_STRING_ADVANCED_ATTACK] = "Angriff";
  LW_LANGDE_DATA[LW_LANG_STRING_ADVANCED_DEFENSE] = "Abwehr";
  LW_LANGDE_DATA[LW_LANG_STRING_ADVANCED_BASEHEALTH] = "Gesundheit";
  LW_LANGDE_DATA[LW_LANG_STRING_ADVANCED_WINNERHELP] = "Siegerhilfe";
  LW_LANGDE_DATA[LW_LANG_STRING_ADVANCED_CPUSTRENGTH] = "CPU Staerke";
  LW_LANGDE_DATA[LW_LANG_STRING_ADVANCED_CPUVSHUMAN] = "CPU vs Mensch";
  LW_LANGDE_DATA[LW_LANG_STRING_ADVANCED_ALWAYS] = "Immer";
  LW_LANGDE_DATA[LW_LANG_STRING_ADVANCED_RANDOM] = "Normal";
  LW_LANGDE_DATA[LW_LANG_STRING_ADVANCED_NEVER] = "Nie";
  LW_LANGDE_DATA[LW_LANG_STRING_ADVANCED_ASMALGORITHM] = "Algorithmus";
  LW_LANGDE_DATA[LW_LANG_STRING_ADVANCED_ASSEMBLY] = "Assembly";
  LW_LANGDE_DATA[LW_LANG_STRING_ADVANCED_STANDARDC] = "C-Standard";
  LW_LANGDE_DATA[LW_LANG_STRING_ADVANCED_ALLOWNETWORKBOTS] = "Netzbots";
  LW_LANGDE_DATA[LW_LANG_STRING_ADVANCED_YES] = "An";
  LW_LANGDE_DATA[LW_LANG_STRING_ADVANCED_NO] = "Aus";

  LW_LANGDE_DATA[LW_LANG_STRING_SPEEDS_FPS] = "frames/s";
  LW_LANGDE_DATA[LW_LANG_STRING_SPEEDS_NOFPSLIMIT] = "Kein FPS Limit";
  LW_LANGDE_DATA[LW_LANG_STRING_SPEEDS_RPS] = "Runden/s";
  LW_LANGDE_DATA[LW_LANG_STRING_SPEEDS_NORPSLIMIT] = "Kein Rundenlimit";

  LW_LANGDE_DATA[LW_LANG_STRING_CONTROLS_JOYSTICK] = "Joystick";
  LW_LANGDE_DATA[LW_LANG_STRING_CONTROLS_OFF] = "Aus";
  LW_LANGDE_DATA[LW_LANG_STRING_CONTROLS_2JOYSTICKS] = "2 Joysticks";
  LW_LANGDE_DATA[LW_LANG_STRING_CONTROLS_4BUTTONS] = "4 Tasten";
  LW_LANGDE_DATA[LW_LANG_STRING_CONTROLS_STANDARD] = "Standard";
  LW_LANGDE_DATA[LW_LANG_STRING_CONTROLS_MOUSESENSIB] = "Maussensibilitaet";

  LW_LANGDE_DATA[LW_LANG_STRING_ABOUT_CREDITS] = "\"Credits\"";
  LW_LANGDE_DATA[LW_LANG_STRING_ABOUT_COPYRIGHT] = "Copyright";
  LW_LANGDE_DATA[LW_LANG_STRING_ABOUT_LICENSE] = "Lizenz";
  LW_LANGDE_DATA[LW_LANG_STRING_ABOUT_VERSION] = "Version";

  LW_LANGDE_DATA[LW_LANG_STRING_INTERNET_REFRESH] = "Refresh";
  LW_LANGDE_DATA[LW_LANG_STRING_INTERNET_JOINNOW] = "Jetzt teilnehmen";
  LW_LANGDE_DATA[LW_LANG_STRING_INTERNET_PORT] = "Port";
  LW_LANGDE_DATA[LW_LANG_STRING_INTERNET_PASSWORD] = "Passwort";
  LW_LANGDE_DATA[LW_LANG_STRING_INTERNET_YES] = "Ja";
  LW_LANGDE_DATA[LW_LANG_STRING_INTERNET_NO] = "Nein";
  LW_LANGDE_DATA[LW_LANG_STRING_INTERNET_UPTIME] = "Alter";
  LW_LANGDE_DATA[LW_LANG_STRING_INTERNET_DAYS] = "d";
  LW_LANGDE_DATA[LW_LANG_STRING_INTERNET_DAY] = "d";
  LW_LANGDE_DATA[LW_LANG_STRING_INTERNET_HOURS] = "h";
  LW_LANGDE_DATA[LW_LANG_STRING_INTERNET_MINUTES] = "min";
  LW_LANGDE_DATA[LW_LANG_STRING_INTERNET_SECONDS] = "sec";
  LW_LANGDE_DATA[LW_LANG_STRING_INTERNET_UNREACHABLE] = "Zu weit / tot";
  LW_LANGDE_DATA[LW_LANG_STRING_INTERNET_PING] = "Ping";
  LW_LANGDE_DATA[LW_LANG_STRING_INTERNET_MSEC] = "ms";

  LW_LANGDE_DATA[LW_LANG_STRING_WAVE_AMPLI] = "Wellen";
  LW_LANGDE_DATA[LW_LANG_STRING_WAVE_NUMBER] = "Nummer";
  LW_LANGDE_DATA[LW_LANG_STRING_WAVE_SPEED] = "Speed";
  LW_LANGDE_DATA[LW_LANG_STRING_WAVE_WX] = "WX";
  LW_LANGDE_DATA[LW_LANG_STRING_WAVE_HY] = "HY";
  LW_LANGDE_DATA[LW_LANG_STRING_WAVE_WY] = "WY";
  LW_LANGDE_DATA[LW_LANG_STRING_WAVE_HX] = "HX";

  LW_LANGDE_DATA[LW_LANG_STRING_CONNECT_STARTNOW] = "Starten";
  LW_LANGDE_DATA[LW_LANG_STRING_CONNECT_WAITING] = "Warte...";
  LW_LANGDE_DATA[LW_LANG_STRING_CONNECT_WAITINGFOR] = "Warte auf ";
  LW_LANGDE_DATA[LW_LANG_STRING_CONNECT_TEAMS] = " Team(s)";
  LW_LANGDE_DATA[LW_LANG_STRING_CONNECT_SENDMESSAGE] = "Nachricht schreiben";

  LW_LANGDE_DATA[LW_LANG_STRING_NETPLAY_CONNECT] = "Verbinden";
  LW_LANGDE_DATA[LW_LANG_STRING_NETPLAY_PING] = "\"Ping\"";
  LW_LANGDE_DATA[LW_LANG_STRING_NETPLAY_CHECKPROGRAM] = "Programmcheck";
  LW_LANGDE_DATA[LW_LANG_STRING_NETPLAY_CHECKVERSION] = "Versionscheck";
  LW_LANGDE_DATA[LW_LANG_STRING_NETPLAY_SENDPASSWORD] = "Password";
  LW_LANGDE_DATA[LW_LANG_STRING_NETPLAY_FREETEAMS] = "Freie Teams?";
  LW_LANGDE_DATA[LW_LANG_STRING_NETPLAY_RESERVETEAMS] = "Reservierung";
  LW_LANGDE_DATA[LW_LANG_STRING_NETPLAY_SENDCONFIG] = "Sende Konfig";
  LW_LANGDE_DATA[LW_LANG_STRING_NETPLAY_SENDMAP] = "Sende Karte";
  LW_LANGDE_DATA[LW_LANG_STRING_NETPLAY_WAITING] = "Warte...";
  LW_LANGDE_DATA[LW_LANG_STRING_NETPLAY_WHOPLAYS] = "Wer spielt?";
  LW_LANGDE_DATA[LW_LANG_STRING_NETPLAY_RECVCONFIG] = "Empfange Konfig";
  LW_LANGDE_DATA[LW_LANG_STRING_NETPLAY_RECVMAP] = "Empfange Karte";

  LW_LANGDE_DATA[LW_LANG_STRING_LEVEL_USEDEFAULTTEXTURE] =
    "Benutze Standardtextur";
  LW_LANGDE_DATA[LW_LANG_STRING_LEVEL_ON] = "An";
  LW_LANGDE_DATA[LW_LANG_STRING_LEVEL_OFF] = "Aus";
  LW_LANGDE_DATA[LW_LANG_STRING_LEVEL_RANDOMMAP] = "Zufallskarte";

  LW_LANGDE_DATA[LW_LANG_STRING_CODE_FPS] = "FP/s ";
  LW_LANGDE_DATA[LW_LANG_STRING_CODE_RPS] = "Runden/s ";
  LW_LANGDE_DATA[LW_LANG_STRING_CODE_PERCENTCALC] = "% calcul ";
  LW_LANGDE_DATA[LW_LANG_STRING_CODE_PERCENTDISPLAY] = "% rechne ";
  LW_LANGDE_DATA[LW_LANG_STRING_CODE_SPREADGRADTIME] = "Spread gate time ";
  LW_LANGDE_DATA[LW_LANG_STRING_CODE_MOVEARMYTIME] = "Bewegungszeit";
  LW_LANGDE_DATA[LW_LANG_STRING_CODE_DISPLAYTIME] = "Zeitanzeige ";
  LW_LANGDE_DATA[LW_LANG_STRING_CODE_FLIPTIME] = "Fliptime";
  LW_LANGDE_DATA[LW_LANG_STRING_CODE_OTHERCALCTIME] = "Andere Zeit ";
  LW_LANGDE_DATA[LW_LANG_STRING_CODE_GLOBALTIME] = "Gesamtzeit ";
  LW_LANGDE_DATA[LW_LANG_STRING_CODE_STANDARDDISPLAY] = "Standardanzeige";
  LW_LANGDE_DATA[LW_LANG_STRING_CODE_GRADIENT1] = "Gradient 1";
  LW_LANGDE_DATA[LW_LANG_STRING_CODE_GRADIENT2] = "Gradient 2";
  LW_LANGDE_DATA[LW_LANG_STRING_CODE_GRADIENT3] = "Gradient 3";
  LW_LANGDE_DATA[LW_LANG_STRING_CODE_GRADIENT4] = "Gradient 4";
  LW_LANGDE_DATA[LW_LANG_STRING_CODE_GRADIENT5] = "Gradient 5";
  LW_LANGDE_DATA[LW_LANG_STRING_CODE_GRADIENT6] = "Gradient 6";
  LW_LANGDE_DATA[LW_LANG_STRING_CODE_MESHSIZE] = "Meshgroesse";
  LW_LANGDE_DATA[LW_LANG_STRING_CODE_MESHSTRUCTURE] = "Meshaufbau";
  LW_LANGDE_DATA[LW_LANG_STRING_CODE_PAUSEOFF] = "Pause aus";
  LW_LANGDE_DATA[LW_LANG_STRING_CODE_PAUSEON] = "Pause";
  LW_LANGDE_DATA[LW_LANG_STRING_CODE_WAVEEFFECTSOFF] = "Wellen aus";
  LW_LANGDE_DATA[LW_LANG_STRING_CODE_WAVEEFFECTSON] = "Wellen an";
  LW_LANGDE_DATA[LW_LANG_STRING_CODE_CAPTUREOFF] = "Fangvideo aus";
  LW_LANGDE_DATA[LW_LANG_STRING_CODE_CAPTUREON] = "Fangvideo an";

  LW_LANGDE_DATA[LW_LANG_STRING_SCORE_WINNER] = "Spitze!";

  LW_LANGDE_DATA[LW_LANG_STRING_GAME_BACKMEMFAIL] =
    "Keine Einheiten ueberlebten";
  LW_LANGDE_DATA[LW_LANG_STRING_GAME_DISPMEMFAIL] = "Kein Meshniederlage";
  LW_LANGDE_DATA[LW_LANG_STRING_GAME_MESHMEMFAIL] = "Meshniederlage";
  LW_LANGDE_DATA[LW_LANG_STRING_GAME_AREAMEMFAIL] = "Kein Gebiet mehr";
  LW_LANGDE_DATA[LW_LANG_STRING_GAME_ARMYMEMFAIL] = "Keine Armee mehr";

  LW_LANGDE_DATA[LW_LANG_STRING_ERROR_NOTENOUGHTEAMS] =
    "Zu wenig Teams vorhanden. Es muessen mindestens 2 Teams da sein.";
  LW_LANGDE_DATA[LW_LANG_STRING_ERROR_NOTENOUGHBIGDATA] =
    "Nicht genug Speicher vorhanden.";
  LW_LANGDE_DATA[LW_LANG_STRING_ERROR_GRAPHICSINITFAILED] =
    "Graphikmodus kann nicht gestartet werden.";
  LW_LANGDE_DATA[LW_LANG_STRING_ERROR_NONETWORKSUPPORT] =
    "Keine Unterstuetzung fuer Netzwerk auf ihrem PC vorhanden.";
  LW_LANGDE_DATA[LW_LANG_STRING_ERROR_UNABLETOCONNECT] =
    "Verbindung nicht moeglich.\nMoeglicherweise sind ihre Einstellungen nicht korrekt oder sie benutzen eine Firewall, die den Datenverkehr blockiert.";
  LW_LANGDE_DATA[LW_LANG_STRING_ERROR_NOTALIQUIDWARSERVER] =
    "Keine Kommunikation mit dem Server moeglich\nWahrscheinlich ist der server kein LiquidWar Server";
  LW_LANGDE_DATA[LW_LANG_STRING_ERROR_WRONGVERSION] =
    "Falsche Programmversion.\nServer und Programm muessen die gleiche Version haben (Programmversion ist: \""
    LW_VERSION "\").";
  LW_LANGDE_DATA[LW_LANG_STRING_ERROR_NETWORKPROBLEM] =
    "Ein Netzwerkfehler trat auf.\nDies passiert meistens wenn die Verbindung mit demServer zu langsam oder gar nicht mehr vorhanden ist.";
  LW_LANGDE_DATA[LW_LANG_STRING_ERROR_CANTSENDMAP] =
    "Kann die Karte nicht zum Server senden.";
  LW_LANGDE_DATA[LW_LANG_STRING_ERROR_CANTRECEIVEMAP] =
    "Kann die Map nicht vom Server empfangen.";
  LW_LANGDE_DATA[LW_LANG_STRING_ERROR_SERVERFULL] =
    "Dieser Server ist voll und kann derzeit keine weiteren Teams aufnehmen.";
  LW_LANGDE_DATA[LW_LANG_STRING_ERROR_BADPASSWORD] = "Falsches Passwort.";
  LW_LANGDE_DATA[LW_LANG_STRING_ERROR_BADCONFIGRANGE] =
    "Eines der Configparameter ist falsch. Eventuell cheatet oder hackt jemand.";

  /*
   * In the following section we can use accented chars since these
   * labels are used in system popup windows which are supposed to
   * handle chars>127, unlike Liquid War graphical functions.
   */
  LW_LANGDE_DATA[LW_LANG_STRING_POPUP_INITFAIL] =
    "Initalisierung fehlgeschlagen.";
  LW_LANGDE_DATA[LW_LANG_STRING_POPUP_MEMTROUBLE] =
    "Fuer Bilder ist nicht genug Speicher vorhanden.";
  LW_LANGDE_DATA[LW_LANG_STRING_POPUP_BIGDATA] =
    "Nicht genug Speicher vorhanden.";
  LW_LANGDE_DATA[LW_LANG_STRING_POPUP_LOADFAIL] =
    "Die Daten konnten nicht geladen werden. Sind sie sich sicher, dass Liquid Wars richtig installiert ist?";
  LW_LANGDE_DATA[LW_LANG_STRING_POPUP_CUSTOMFAIL] =
    "Die selbsterstellten Daten konnten nicht geladen werden.";
  LW_LANGDE_DATA[LW_LANG_STRING_POPUP_GRAPHICSFAIL] =
    "Graphiken konnten nicht geladen werden.";

  LW_LANGDE_DATA[LW_LANG_STRING_LW6_INFO] =
    "Dieses Programm ist veraltet. Denke ueber ein Upgrade auf Liquid War 6 nach, welches unter folgender Adresse verfuegbar ist:\n\nhttp://www.gnu.org/software/liquidwar6/\n\nLiquid War 5 steht dir weiterhin zur Verfuegung, und kann kostenlos benutzt werden. In Zukunft werden jedoch nur noch groessere Sicherheitsloecher gefixt, eine Weiterentwicklung des Liquid War 5 Baumes sollte nicht erwartet werden.";
  LW_LANGDE_DATA[LW_LANG_STRING_LW6_OK] = "OK";
}

/*------------------------------------------------------------------*/
/* returns the French string associated to an id                    */
/*------------------------------------------------------------------*/
char *
lw_langde_string (int id)
{
  char *result = NULL;

  if (id >= 0 && id < LW_LANG_NB_STRINGS)
    {
      result = LW_LANGDE_DATA[id];
    }

  if (!result)
    {
      result = "??? de";
    }

  return result;
}
