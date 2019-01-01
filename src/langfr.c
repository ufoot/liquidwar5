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
/* Copyright (C) 1998-2019 Christian Mauduit                                 */
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
/* name          : langfr.c                                         */
/* content       : handles French dependent stuff (locale)          */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>
#include <stdlib.h>

#include "lang.h"
#include "langfr.h"
#include "base.h"

/*==================================================================*/
/* globals                                                          */
/*==================================================================*/

static char *LW_LANGFR_DATA[LW_LANG_NB_STRINGS];

/*------------------------------------------------------------------*/
/* initializes the string table                                     */
/*------------------------------------------------------------------*/
void
lw_langfr_init ()
{
  memset (LW_LANGFR_DATA, 0, sizeof (LW_LANGFR_DATA));

  LW_LANGFR_DATA[LW_LANG_STRING_LANGUAGE] = "Francais";
  LW_LANGFR_DATA[LW_LANG_STRING_APPNAME] = "Liquid War " LW_VERSION;

  LW_LANGFR_DATA[LW_LANG_STRING_HELP_CREDITS] =
    "Idee: \n* Thomas Colcombet\n\nProgrammation: \n* Christian Mauduit\n\nMusique: \n* Tim Chadburn\n\nAutres contributeurs:\n* Alstar\n* Peter Wang\n* Cort Danger Stratton\n* Jan Gretschuskin (www.game-factor.de)\n* Mouse\n* Rene Stach\n* Roderick Schertler\n* Ryan D. Brown\n* Eduard Bloch\n* Michael Terry\n* Kasper Hviid\n* David Redick\n* Alexandre Pineau\n* Michael Wagner\n* Peter Williams\n* Jan Samohyl\n* Gavin\n* Dave Vasilevsky\n* 2 of Omega\n* 666-REFIZUL-666\n* Thomas Klausner\n* Joan Dolc\n* Mikhail Yakshin\n* Valerie Mauduit\n* Florence Mauduit\n* Joey Dodson\n* Jan Szenborn\n\nPlus d'informations sur www.ufoot.org/liquidwar/v5\n";
  LW_LANGFR_DATA[LW_LANG_STRING_HELP_COPYRIGHT] =
    "Liquid War est un \"wargame\" multijoueur.\nCopyright (C) 1998-2019 Christian Mauduit (ufoot@ufoot.org)\n\nCe programme est un logiciel libre ; vous pouvez le redistribuer et/ou le modifier conformement aux dispositions de la Licence Publique Generale GNU, telle que publiee par la Free Software Foundation ; version 2 de la licence, ou encore (a votre choix) toute version ulterieure.\n\nCe programme est distribue dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE ; sans meme la garantie implicite de COMMERCIALISATION ou D'ADAPTATION A UN OBJET PARTICULIER. Pour plus de detail, voir la Licence Publique Generale GNU .\n\nVous devez avoir recu un exemplaire de la Licence Publique Generale GNU en meme temps que ce programme ; si ce n'est pas le cas, ecrivez a la Free Software Foundation Inc., 675 Mass Ave, Cambridge, MA 02139, Etats-Unis.\n\nPlus d'informations sur www.ufoot.org/liquidwar/v5\n";
  /*
   * Stupid Microsoft VC++ can't handle strings over 2048 chars correctly,
   * (compiler error C2026) so we had to split this one...
   */
  LW_LANGFR_DATA[LW_LANG_STRING_HELP_LICENSE] =
    "Traduction de la GPL\n\nNotice d'accompagnement de la traduction non officielle a conserver dans toute reproduction de cette traduction\n\nThis is an unofficial translation of the GNU General Public License into french. It was not published by the Free Software Foundation, and does not legally state the distribution terms for software that uses the GNU GPL--only the original English text of the GNU GPL does that. However, we hope that this translation will help french speakers understand the GNU GPL better.\n\nCeci est une traduction non officielle de la GNU General Public License en francais. Elle n'a pas ete publiee par la Free Software Foundation, et ne determine pas les termes de distribution pour les logiciels qui utilisent la GNU GPL--seul le texte anglais original de la GNU GPL en a le droit. Cependant, nous esperons que cette traduction aidera les francophones a mieux comprendre la GPL.\n\nCette traduction est sous Copyright 2001 APRIL (http://www.april.org). "
    "\nLa version la plus a jour de ce document est disponible sur http://www.april.org/gnu/gpl_french.html\n\nIl est permis a tout le monde de reproduire et distribuer des copies conformes de cette traduction, mais aucune modification ne doit y etre apportee, et la presente notice doit etre preservee.\n\nNous autorisons la FSF a apporter toute modification qu'elle jugera necessaire pour rendre la traduction plus claire.\n\nGNU GENERAL PUBLIC LICENSE (TRADUCTION NON OFFICIELLE)\n\nVersion 2, juin 1991\n\nCopyright (C) 1989, 1991, Free Software Foundation Inc. 675 Mass Ave, Cambridge, MA02139, Etats-Unis.\n\nIl est permis a tout le monde de reproduire et distribuer des copies conformes de ce document de licence, mais aucune modification ne doit y etre apportee.\n\nPreambule\n\nLes licences relatives a la plupart des logiciels sont destinees a supprimer votre liberte de les partager et de les modifier. Par contraste, la licence publique generale GNU General Public License veut garantir votre "
    "liberte de partager et de modifier les logiciels libres, pour qu'ils soient vraiment libres pour tous leurs utilisateurs. La presente licence publique generale s'applique a la plupart des logiciels de la Free Software Foundation, ainsi qu'a tout autre programme dont les auteurs s'engagent a l'utiliser. (Certains autres logiciels sont couverts par la Licence Publique Generale pour Bibliotheques GNU a la place). Vous pouvez aussi l'appliquer a vos programmes.\n\nQuand nous parlons de logiciels libres, nous parlons de liberte, non de gratuite. Nos licences publiques generales veulent vous garantir :\n\n* que vous avez toute liberte de distribuer des copies des logiciels libres (et de facturer ce service, si vous le souhaitez) ;\n\n* que vous recevez les codes sources ou pouvez les obtenir si vous le souhaitez ;\n\n* que vous pouvez modifier les logiciels ou en utiliser des elements dans de nouveaux programmes libres ;\n\n* et que vous savez que vous pouvez le faire.\n\nPour proteger vos droits, "
    "nous devons apporter des restrictions, qui vont interdire a quiconque de vous denier ces droits, ou de vous demander de vous en desister. Ces restrictions se traduisent par certaines responsabilites pour ce qui vous concerne, si vous distribuez des copies de logiciels, ou si vous les modifiez.\n\nPar exemple, si vous distribuez des copies d'un tel programme, gratuitement ou contre une remuneration, vous devez transferer aux destinataires tous les droits dont vous disposez. Vous devez vous garantir qu'eux-memes, par ailleurs, recoivent ou peuvent recevoir le code source. Et vous devez leur montrer les presentes dispositions, de facon qu'ils connaissent leurs droits.\n\nNous protegeons vos droits en deux etapes :\n\n1. Nous assurons le droit d'auteur (copyright) du logiciel, et\n\n2. Nous vous proposons cette licence, qui vous donne l'autorisation legale de dupliquer, distribuer et/ou modifier le logiciel.\n\nDe meme, pour la protection de chacun des auteurs, et pour notre propre protection, "
    "nous souhaitons nous assurer que tout le monde comprenne qu'il n'y a aucune garantie portant sur ce logiciel libre. Si le logiciel est modifie par quelqu'un d'autre puis transmis a des tiers, nous souhaitons que les destinataires sachent que ce qu'ils possedent n'est pas l'original, de facon que tous problemes introduits par d'autres ne se traduisent pas par une repercussion negative sur la reputation de l'auteur original.\n\nEnfin, tout programme libre est en permanence menace par des brevets de logiciels. Nous souhaitons eviter le danger que des sous-distributeurs d'un programme libre obtiennent a titre individuel des licences de brevets, avec comme consequence qu'ils ont un droit de propriete sur le programme. Pour eviter cette situation, nous avons fait tout ce qui est necessaire pour que tous brevets doivent faire l'objet d'une concession de licence qui en permette l'utilisation libre par quiconque, ou bien qu'il ne soit pas concede du tout.\n\nNous presentons ci-dessous les clauses "
    "et dispositions concernant la duplication, la distribution et la modification.\n\nCONDITIONS D'EXPLOITATION PORTANT SUR LA DUPLICATION, LA DISTRIBUTION ET LA MODIFICATION\n\n1. Le present contrat de licence s'applique a tout programme ou autre ouvrage contenant un avis, appose par le detenteur du droit de propriete, disant qu'il peut etre distribue au titre des dispositions de la presente Licence Publique Generale. Ci-apres, le \"Programme\" designe l'un quelconque de ces programmes ou ouvrages, et un \"ouvrage fonde sur le programme\" designe soit le programme, soit un ouvrage qui en derive au titre de la loi sur le droit d'auteur ; plus precisement, il s'agira d'un ouvrage contenant le programme ou une version de ce dernier, soit mot a mot, soit avec des modifications et/ou traduit en une autre langue (ci-apres, le terme \"modification\" englobe, sans aucune limitation, les traductions qui en sont faites). Chaque titulaire de licence sera appele \"concessionnaire\".\nLes activites autres "
    "que la duplication, la distribution et la modification ne sont pas couvertes par la presente licence ; elles n'entrent pas dans le cadre de cette derniere. L'execution du programme n'est soumise a aucune restriction, et les resultats du programme ne sont couverts que si son contenu constitue un ouvrage fonde sur le programme (independamment du fait qu'il a ete realise par execution du programme). La veracite de ce qui precede depend de ce que fait le programme.\n\n2. Le concessionnaire peut dupliquer et distribuer des copies mot a mot du code source du programme tel qu'il les recoit, et ce sur un support quelconque, du moment qu'il appose, d'une maniere parfaitement visible et appropriee, sur chaque exemplaire, un avis approprie de droits d'auteur (Copyright) et de renonciation a garantie ; qu'il maintient intacts tous les avis qui se rapportent a la presente licence et a l'absence de toute garantie ; et qu'il transmet a tout destinataire du programme un exemplaire de la presente licence "
    "en meme temps que le programme.\nLe concessionnaire peut facturer l'acte physique de transfert d'un exemplaire, et il peut, a sa discretion, proposer en echange d'une remuneration une protection en garantie.\n\n3. Le concessionnaire peut modifier son ou ses exemplaires du programme ou de toute portion de ce dernier, en formant ainsi un ouvrage fonde sur le programme, et dupliquer et distribuer ces modifications ou cet ouvrage selon les dispositions de la section 1 ci-dessus, du moment que le concessionnaire satisfait aussi a toutes ces conditions :\na. Le concessionnaire doit faire en sorte que les fichiers modifies portent un avis, parfaitement visible, disant que le concessionnaire a modifie les fichiers, avec la date de tout changement.\nb. Le concessionnaire doit faire en sorte que tout ouvrage qu'il distribue ou publie, et qui, en totalite ou en partie, contient le programme ou une partie quelconque de ce dernier ou en derive, soit concede en bloc, a titre gracieux, a tous tiers au "
    "titre des dispositions de la presente licence.\nc. Si le programme modifie lit normalement des instructions interactives lors de son execution, le concessionnaire doit, quand il commence l'execution du programme pour une telle utilisation interactive de la maniere la plus usuelle, faire en sorte que ce programme imprime ou affiche une annonce, comprenant un avis approprie de droits d'auteur, et un avis selon lequel il n'y a aucune garantie (ou autrement, que le concessionnaire fournit une garantie), et que les utilisateurs peuvent redistribuer le programme au titre de ces dispositions, et disant a l'utilisateur comment visualiser une copie de cette licence (exception : si le programme par lui-meme est interactif mais n'imprime normalement pas une telle annonce, l'ouvrage du concessionnaire se fondant sur le programme n'a pas besoin d'imprimer une annonce).\nLes exigences ci-dessus s'appliquent a l'ouvrage modifie pris en bloc. Si des sections identifiables de cet ouvrage ne derivent pas "
    "du programme et peuvent etre considerees raisonnablement comme representant des ouvrages independants et distincts par eux-memes, alors la presente licence, et ses dispositions, ne s'appliquent pas a ces sections quand le concessionnaire les distribue sous forme d'ouvrages distincts. Mais quand le concessionnaire distribue ces memes sections en tant qu'element d'un tout qui represente un ouvrage se fondant sur le programme, la distribution de ce tout doit se faire conformement aux dispositions de la presente licence, dont les autorisations, portant sur d'autres concessionnaires, s'etendent a la totalite dont il est question, et ainsi a chacune de ces parties, independamment de celui qu'il a ecrite.\nAinsi, cette section n'a pas pour but de revendiquer des droits ou de contester vos droits sur un ouvrage entierement ecrit par le concessionnaire ; bien plus, l'intention est d'exercer le droit de surveiller la distribution d'ouvrages derivee ou collective se fondant sur le programme.\nDe "
    "plus, un simple assemblage d'un autre ouvrage ne se fondant pas sur le programme, avec le programme (ou avec un ouvrage se fondant sur le programme) sur un volume d'un support de stockage ou distribution, ne fait pas entrer l'autre ouvrage dans le cadre de la presente licence.\n\n4. Le concessionnaire peut dupliquer et distribuer le programme (ou un ouvrage se fondant sur ce dernier, au titre de la Section 2), en code objet ou sous une forme executable, au titre des dispositions des Sections 1 et 2 ci-dessus, du moment que le concessionnaire effectue aussi l'une des operations suivantes :\na. Lui joindre le code source complet correspondant, exploitable par une machine, code qui doit etre distribue au titre des Sections 1 et 2 ci-dessus sur un support couramment utilise pour l'echange de logiciels ; ou bien\nb. Lui joindre une offre ecrite, dont la validite se prolonge pendant au moins 3 ans, de transmettre a un tiers quelconque, pour un montant non superieur au cout pour le concessionnaire, "
    "de realisation physique de la distribution de la source, un exemplaire complet, exploitable par une machine, du code source correspondant, qui devra etre distribue au titre des dispositions des Sections 1 et 2 ci-dessus sur un support couramment utilise pour l'echange des logiciels ; ou bien\nc. Lui joindre les informations que le concessionnaire a recues, pour proposer une distribution du code source correspondant (cette variante n'est autorisee que pour la distribution non commerciale, et seulement si le concessionnaire a recu le programme sous forme executable ou sous forme d'un code objet, avec une telle offre, conformement a l'alinea b) ci-dessus).\nLe code source d'un ouvrage represente la forme preferee de l'ouvrage pour y effectuer des modifications. Pour un ouvrage executable, le code source complet represente la totalite du code source pour tous les modules qu'il contient, plus tous fichiers de definitions d'interface associes, plus les informations en code machine pour commander "
    "la compilation et l'installation du programme executable. Cependant, a titre d'exceptions speciales, le code source distribue n'a pas besoin de comprendre quoi que ce soit qui est normalement distribue (sous forme source ou sous forme binaire) avec les composants principaux (compilateur, noyau de systeme d'exploitation, etc.) du systeme d'exploitation sur lequel est execute le programme executable, a moins que le composant, par lui-meme, soit joint au programme executable.\nSi la distribution de l*executable ou du code objet est realisee de telle sorte qu'elle offre d'acceder a une copie a partir d'un lieu designe, alors le fait d'offrir un acces equivalent a la duplication du code source a partir de ce meme lieu s'entend comme distribution du code source, meme si des tiers ne sont pas contraints de dupliquer la source en meme temps que le code objet.\n\n5. Le concessionnaire ne peut dupliquer, modifier, conceder en sous-licence ou distribuer le programme, sauf si cela est expressement "
    "prevu par les dispositions de la presente licence. Toute tentative pour autrement dupliquer, modifier, conceder en sous-licence ou distribuer le programme est repetee nulle, et met automatiquement fin aux droits du concessionnaire au titre de la presente licence. Cependant, les parties qui ont recu des copies, ou des droits, de la part du concessionnaire au titre de la presente licence, ne verront pas expirer leur contrat de licence, tant que ces parties agissent d'une maniere parfaitement conforme.\n\n6. Il n'est pas exige du concessionnaire qu'il accepte la presente licence, car il ne l'a pas signee. Cependant, rien d'autre n'octroie au concessionnaire l'autorisation de modifier ou de distribuer le programme ou ses ouvrages derives. Ces actions sont interdites par la loi si le concessionnaire n'accepte pas la presente licence. En consequence, par le fait de modifier ou de distribuer le programme (ou un ouvrage quelconque se fondant sur le programme), le concessionnaire indique qu'il "
    "accepte la presente licence, et qu'il a la volonte de se conformer a toutes les clauses et dispositions concernant la duplication, la distribution ou la modification du programme ou d'ouvrages se fondant sur ce dernier.\n\n7. Chaque fois que le concessionnaire redistribue le programme (ou tout ouvrage se fondant sur le programme), le destinataire recoit automatiquement une licence de l'emetteur initial de la licence, pour dupliquer, distribuer ou modifier le programme, sous reserve des presentes clauses et dispositions. Le concessionnaire ne peut imposer aucune restriction plus poussee sur l'exercice, par le destinataire, des droits octroyes au titre des presentes. Le concessionnaire n'a pas pour responsabilite d'exiger que des tiers se conforment a la presente licence.\n\n8. Si, en consequence une decision de justice ou une allegation d'infraction au droit des brevets, ou pour toute autre raison (qui n'est pas limitee a des problemes de proprietes industrielles), des conditions sont imposees "
    "au concessionnaire (par autorite de justice, par convention ou autrement), qui entrent en contradiction avec les dispositions de la presente licence, elles n'exemptent pas le concessionnaire de respecter les dispositions de la presente licence. Si le concessionnaire ne peut proceder a la distribution de facon a satisfaire simultanement a ces obligations au titre de la presente licence et a toutes autres obligations pertinentes, alors, en consequence de ce qui precede, le concessionnaire peut ne pas proceder du tout a la distribution du programme. Par exemple, si une licence de brevet ne permettait pas une redistribution du programme, sans redevances, par tous ceux qui recoivent des copies directement ou indirectement par l'intermediaire du concessionnaire, alors le seul moyen par lequel le concessionnaire pourrait satisfaire tant a cette licence de brevet qu'a la presente licence, consisterait a s'abstenir completement de distribuer le programme.\nSi une partie quelconque de cette section "
    "est consideree comme nulle ou non executoire dans certaines circonstances particulieres, le reste de cette section est repute s'appliquer, et la section dans son ensemble est consideree comme s'appliquant dans les autres circonstances.\nLa presente section n'a pas pour objet de pousser le concessionnaire a enfreindre tous brevets ou autres revendications a droit de propriete, ou encore a contester la validite de une ou plusieurs quelconques de ces revendications ; la presente section a pour objet unique de proteger l'integrite du systeme de distribution des logiciels libres, systeme qui est mis en oeuvre par les pratiques liees aux licences publiques. De nombreuses personnes ont apporte une forte contribution a la gamme etendue des logiciels distribues par ce systeme, en comptant sur l'application systematique de ce systeme ; c'est a l'auteur/donateur de decider s'il a la volonte de distribuer le logiciel par un quelconque autre systeme, et un concessionnaire ne peut imposer ce choix.\nLa "
    "presente section veut rendre parfaitement claire ce que l'on pense etre une consequence du reste de la presente licence.\n\n9. Si la distribution et/ou l'utilisation du Programme est restreinte dans certains pays, sous l'effet de brevets ou d'interfaces presentant un droit d'auteur, le detenteur du droit d'auteur original, qui soumet le Programme aux dispositions de la presente licence, pourra ajouter une limitation expresse de distribution geographique excluant ces pays, de facon que la distribution ne soit autorisee que dans les pays ou parmi les pays qui ne sont pas ainsi exclus. Dans ce cas, la limitation fait partie integrante de la presente licence, comme si elle etait ecrite dans le corps de la presente licence.\nLa Free Software Foundation peut, de temps a autre, publier des versions revisees et/ou nouvelles du General Public License. Ces nouvelles versions seront analogues, du point de vue de leur esprit, a la presente version, mais pourront en differer dans le detail, pour resoudre "
    "de nouveaux problemes ou de nouvelles situations.\nChaque version recoit un numero de version qui lui est propre. Si le programme specifie un numero de version de la presente licence, qui s'applique a cette dernier et \"a toute autre version ulterieure\", le concessionnaire a le choix de respecter les clauses et dispositions de cette version, ou une quelconque version ulterieure publiee par la Free Software Foundation. Si le programme ne specifie pas de numero de version de la presente licence, le concessionnaire pourra choisir une version quelconque publiee a tout moment par la Free Software Foundation.\n\n10. Si le concessionnaire souhaite incorporer des parties du programme dans d'autres programmes libres dont les conditions de distribution sont differentes, il devrait ecrire a l'auteur pour demander son autorisation. Pour un logiciel soumis a droit d'auteur par la Free Software Foundation, il devra ecrire a la Free Software Foundation ; nous faisons quelquefois des exceptions a cette "
    "regle. Notre decision va etre guidee par le double objectif de proteger le statut libre de tous les derives de nos logiciels libres, et de favoriser le partage et la reutilisation des logiciels en general.\n\nABSENCE DE GARANTIE\n\n11. COMME LA LICENCE DU PROGRAMME EST CONCEDEE A TITRE GRATUIT, IL N'Y A AUCUNE GARANTIE S'APPLIQUANT AU PROGRAMME, DANS LA MESURE AUTORISEE PAR LA LOI EN VIGUEUR. SAUF MENTION CONTRAIRE ECRITE, LES DETENTEURS DU DROIT D'AUTEUR ET/OU LES AUTRES PARTIES METTENT LE PROGRAMME A DISPOSITON \"EN L'ETAT\", SANS AUCUNE GARANTIE DE QUELQUE NATURE QUE CE SOIT, EXPRESSE OU IMPLICITE, Y COMPRIS, MAIS SANS LIMITATION, LES GARANTIES IMPLICITES DE COMMERCIALISATION ET DE L'APTITUDE A UN OBJET PARTICULIER. C'EST LE CONCESSIONNAIRE QUI PREND LA TOTALITE DU RISQUE QUANT A LA QUALITE ET AUX PERFORMANCES DU PROGRAMME. SI LE PROGRAMME SE REVELAIT DEFECTUEUX, C'EST LE CONCESSIONNAIRE QUI PRENDRAIT A SA CHARGE LE COUT DE L'ENSEMBLE DES OPERATIONS NECESSAIRES D'ENTRETIEN, REPARATION "
    "OU CORRECTION.\n\n12. EN AUCUN CAS, SAUF SI LA LOI EN VIGUEUR L'EXIGE OU SI UNE CONVENTION ECRITE EXISTE A CE SUJET, AUCUN DETENTEUR DE DROITS D'AUTEUR, OU AUCUNE PARTIE AYANT LE POUVOIR DE MODIFIER ET/OU DE REDISTRIBUER LE PROGRAMME CONFORMEMENT AUX AUTORISATIONS CI-DESSUS, N'EST RESPONSABLE VIS-A-VIS DU CONCESSIONNAIRE POUR CE QUI EST DES DOMMAGES, Y COMPRIS TOUS DOMMAGES GENERAUX, SPECIAUX, ACCIDENTELS OU INDIRECTS, RESULTANT DE L'UTILISATION OU DU PROGRAMME OU DE L'IMPOSSIBILITE D'UTILISER LE PROGRAMME (Y COMPRIS, MAIS SANS LIMITATION, LA PERTE DE DONNEES, OU LE FAIT QUE DES DONNEES SONT RENDUES IMPRECISES, OU ENCORE LES PERTES EPROUVEES PAR LE CONCESSIONNAIRE OU PAR DES TIERS, OU ENCORE UN MANQUEMENT DU PROGRAMME A FONCTIONNER AVEC TOUS AUTRES PROGRAMMES), MEME SI CE DETENTEUR OU CETTE AUTRE PARTIE A ETE AVISE DE LA POSSIBILITE DE TELS DOMMAGES.\n\nFIN DES CONDITIONS D'EXPLOITATION\n\nPlus d'informations sur www.ufoot.org/liquidwar/v5\n";
  LW_LANGFR_DATA[LW_LANG_STRING_HELP_VERSION] =
    "Liquid War par U-Foot\n\nv" LW_VERSION " \n\ncompilee le " __DATE__ " a "
    __TIME__ "\n\nPlus d'informations sur www.ufoot.org/liquidwar/v5\n";

  LW_LANGFR_DATA[LW_LANG_STRING_MENU_PLAY] = "Jouer";
  LW_LANGFR_DATA[LW_LANG_STRING_MENU_NETGAME] = "Jeu reseau";
  LW_LANGFR_DATA[LW_LANG_STRING_MENU_MAP] = "Tableau";
  LW_LANGFR_DATA[LW_LANG_STRING_MENU_TEAMS] = "Equipes";
  LW_LANGFR_DATA[LW_LANG_STRING_MENU_OPTIONS] = "Options";
  LW_LANGFR_DATA[LW_LANG_STRING_MENU_ABOUT] = "A propos";
  LW_LANGFR_DATA[LW_LANG_STRING_MENU_EXIT] = "Quitter";
  LW_LANGFR_DATA[LW_LANG_STRING_MENU_EXITDOS] = "Revenir a DOS";
  LW_LANGFR_DATA[LW_LANG_STRING_MENU_EXITWINDOWS] = "Quitter";
  LW_LANGFR_DATA[LW_LANG_STRING_MENU_EXITUNIX] = "Quitter";
  LW_LANGFR_DATA[LW_LANG_STRING_MENU_BACK] = "Retour a LW";

  LW_LANGFR_DATA[LW_LANG_STRING_DIALOG_QUIT] = "Quitter";
  LW_LANGFR_DATA[LW_LANG_STRING_DIALOG_BACK] = "Retour";
  LW_LANGFR_DATA[LW_LANG_STRING_DIALOG_PLAY] = "Jouer";
  LW_LANGFR_DATA[LW_LANG_STRING_DIALOG_MAIN] = "Accueil";

  LW_LANGFR_DATA[LW_LANG_STRING_PLAY_GETREADY] = "C'est parti!";

  LW_LANGFR_DATA[LW_LANG_STRING_NETGAME_STARTGAME] = "Demarrer";
  LW_LANGFR_DATA[LW_LANG_STRING_NETGAME_SERVERADDR] = "Addr serveur";
  LW_LANGFR_DATA[LW_LANG_STRING_NETGAME_SERVERPORT] = "Port serveur";
  LW_LANGFR_DATA[LW_LANG_STRING_NETGAME_SEARCH] =
    "Trouver un serveur sur le Net";
  LW_LANGFR_DATA[LW_LANG_STRING_NETGAME_PASSWORD] = "Mot de passe";
  LW_LANGFR_DATA[LW_LANG_STRING_NETGAME_WEAKPASSWORD] =
    "ATTENTION! Choisir un mot de passe simple genre \"bonjour\", car il est stocke sur le disque dur et envoye au serveur en clair.";

  LW_LANGFR_DATA[LW_LANG_STRING_LANGUAGE_AUTO] = "Automatique";

  LW_LANGFR_DATA[LW_LANG_STRING_TEAM_OFF] = "Inactif";
  LW_LANGFR_DATA[LW_LANG_STRING_TEAM_HUMAN] = "Joueur";
  LW_LANGFR_DATA[LW_LANG_STRING_TEAM_CPU] = "Ordi";

  LW_LANGFR_DATA[LW_LANG_STRING_OPTIONS_GRAPHICS] = "Affichage";
  LW_LANGFR_DATA[LW_LANG_STRING_OPTIONS_SOUND] = "Son";
  LW_LANGFR_DATA[LW_LANG_STRING_OPTIONS_RULES] = "Regles";
  LW_LANGFR_DATA[LW_LANG_STRING_OPTIONS_SPEEDS] = "Vitesses";
  LW_LANGFR_DATA[LW_LANG_STRING_OPTIONS_CONTROLS] = "Controles";
  LW_LANGFR_DATA[LW_LANG_STRING_OPTIONS_LANGUAGE] = "Langue";
  LW_LANGFR_DATA[LW_LANG_STRING_OPTIONS_DEFAULT] = "Remise a zero";

  LW_LANGFR_DATA[LW_LANG_STRING_GRAPHICS_GAME] = "Jeu";
  LW_LANGFR_DATA[LW_LANG_STRING_GRAPHICS_MENU] = "Menu";
  LW_LANGFR_DATA[LW_LANG_STRING_GRAPHICS_ON] = "Oui";
  LW_LANGFR_DATA[LW_LANG_STRING_GRAPHICS_OFF] = "Non";
  LW_LANGFR_DATA[LW_LANG_STRING_GRAPHICS_FULLSCREEN] = "Plein ecran";
  LW_LANGFR_DATA[LW_LANG_STRING_GRAPHICS_WINDOWED] = "Fenetre";
  LW_LANGFR_DATA[LW_LANG_STRING_GRAPHICS_ADVANCED] = "Opt avancees";
  LW_LANGFR_DATA[LW_LANG_STRING_GRAPHICS_VIDEOMODE] = "Mode video";
  LW_LANGFR_DATA[LW_LANG_STRING_GRAPHICS_BRIGHTNESS] = "Luminosite";
  LW_LANGFR_DATA[LW_LANG_STRING_GRAPHICS_VIEWPORTSIZE] = "Surface";
  LW_LANGFR_DATA[LW_LANG_STRING_GRAPHICS_PAGEFLIPPING] = "\"Page flipping\"";
  LW_LANGFR_DATA[LW_LANG_STRING_GRAPHICS_WAVES] = "Vagues (F4)";

  LW_LANGFR_DATA[LW_LANG_STRING_VOLUME_SFX] = "Bruits";
  LW_LANGFR_DATA[LW_LANG_STRING_VOLUME_CLICK] = "Clics";
  LW_LANGFR_DATA[LW_LANG_STRING_VOLUME_GAMEWATER] = "Eau (jeu)";
  LW_LANGFR_DATA[LW_LANG_STRING_VOLUME_MENUWATER] = "Eau (menu)";
  LW_LANGFR_DATA[LW_LANG_STRING_VOLUME_MUSIC] = "Musique";

  LW_LANGFR_DATA[LW_LANG_STRING_RULES_TIME] = "Temps";
  LW_LANGFR_DATA[LW_LANG_STRING_RULES_ADVANCED] = "Opt avancees";
  LW_LANGFR_DATA[LW_LANG_STRING_RULES_ARMYSIZE] = "Taille armees";
  LW_LANGFR_DATA[LW_LANG_STRING_RULES_CURSOR] = "Curseur";
  LW_LANGFR_DATA[LW_LANG_STRING_RULES_DEFAULT] = "Remise a zero";
  LW_LANGFR_DATA[LW_LANG_STRING_RULES_MINMAPRES] = "Mini ";

  LW_LANGFR_DATA[LW_LANG_STRING_ADVANCED_ATTACK] = "Attaque";
  LW_LANGFR_DATA[LW_LANG_STRING_ADVANCED_DEFENSE] = "Defense";
  LW_LANGFR_DATA[LW_LANG_STRING_ADVANCED_BASEHEALTH] = "Force depart";
  LW_LANGFR_DATA[LW_LANG_STRING_ADVANCED_WINNERHELP] = "Aide vainqueur";
  LW_LANGFR_DATA[LW_LANG_STRING_ADVANCED_CPUSTRENGTH] = "Force ordi";
  LW_LANGFR_DATA[LW_LANG_STRING_ADVANCED_CPUVSHUMAN] = "Ordi vs joueur";
  LW_LANGFR_DATA[LW_LANG_STRING_ADVANCED_ALWAYS] = "Toujours";
  LW_LANGFR_DATA[LW_LANG_STRING_ADVANCED_RANDOM] = "Au hasard";
  LW_LANGFR_DATA[LW_LANG_STRING_ADVANCED_NEVER] = "Jamais";
  LW_LANGFR_DATA[LW_LANG_STRING_ADVANCED_ASMALGORITHM] = "Algorithme";
  LW_LANGFR_DATA[LW_LANG_STRING_ADVANCED_ASSEMBLY] = "Assembleur";
  LW_LANGFR_DATA[LW_LANG_STRING_ADVANCED_STANDARDC] = "C standard";
  LW_LANGFR_DATA[LW_LANG_STRING_ADVANCED_ALLOWNETWORKBOTS] = "Bots reseau";
  LW_LANGFR_DATA[LW_LANG_STRING_ADVANCED_YES] = "Oui";
  LW_LANGFR_DATA[LW_LANG_STRING_ADVANCED_NO] = "Non";

  LW_LANGFR_DATA[LW_LANG_STRING_SPEEDS_FPS] = "affich/s";
  LW_LANGFR_DATA[LW_LANG_STRING_SPEEDS_NOFPSLIMIT] = "Pas de limite";
  LW_LANGFR_DATA[LW_LANG_STRING_SPEEDS_RPS] = "tours/s";
  LW_LANGFR_DATA[LW_LANG_STRING_SPEEDS_NORPSLIMIT] = "Pas de limite";

  LW_LANGFR_DATA[LW_LANG_STRING_CONTROLS_JOYSTICK] = "Joystick";
  LW_LANGFR_DATA[LW_LANG_STRING_CONTROLS_OFF] = "Aucun";
  LW_LANGFR_DATA[LW_LANG_STRING_CONTROLS_2JOYSTICKS] = "2 joysticks";
  LW_LANGFR_DATA[LW_LANG_STRING_CONTROLS_4BUTTONS] = "4 boutons";
  LW_LANGFR_DATA[LW_LANG_STRING_CONTROLS_STANDARD] = "Standard";
  LW_LANGFR_DATA[LW_LANG_STRING_CONTROLS_MOUSESENSIB] =
    "Sensibilite de la souris (jeu)";

  LW_LANGFR_DATA[LW_LANG_STRING_ABOUT_CREDITS] = "\"Credits\"";
  LW_LANGFR_DATA[LW_LANG_STRING_ABOUT_COPYRIGHT] = "Copyright";
  LW_LANGFR_DATA[LW_LANG_STRING_ABOUT_LICENSE] = "Licence";
  LW_LANGFR_DATA[LW_LANG_STRING_ABOUT_VERSION] = "Version";

  LW_LANGFR_DATA[LW_LANG_STRING_INTERNET_REFRESH] = "Rafraichir";
  LW_LANGFR_DATA[LW_LANG_STRING_INTERNET_JOINNOW] = "Se connecter";
  LW_LANGFR_DATA[LW_LANG_STRING_INTERNET_PORT] = "Port";
  LW_LANGFR_DATA[LW_LANG_STRING_INTERNET_PASSWORD] = "Mot de passe";
  LW_LANGFR_DATA[LW_LANG_STRING_INTERNET_YES] = "oui";
  LW_LANGFR_DATA[LW_LANG_STRING_INTERNET_NO] = "non";
  LW_LANGFR_DATA[LW_LANG_STRING_INTERNET_UPTIME] = "Age";
  LW_LANGFR_DATA[LW_LANG_STRING_INTERNET_DAYS] = "j";
  LW_LANGFR_DATA[LW_LANG_STRING_INTERNET_DAY] = "j";
  LW_LANGFR_DATA[LW_LANG_STRING_INTERNET_HOURS] = "h";
  LW_LANGFR_DATA[LW_LANG_STRING_INTERNET_MINUTES] = "min";
  LW_LANGFR_DATA[LW_LANG_STRING_INTERNET_SECONDS] = "sec";
  LW_LANGFR_DATA[LW_LANG_STRING_INTERNET_UNREACHABLE] = "Pas joignable";
  LW_LANGFR_DATA[LW_LANG_STRING_INTERNET_PING] = "Ping";
  LW_LANGFR_DATA[LW_LANG_STRING_INTERNET_MSEC] = "ms";

  LW_LANGFR_DATA[LW_LANG_STRING_WAVE_AMPLI] = "Taille";
  LW_LANGFR_DATA[LW_LANG_STRING_WAVE_NUMBER] = "Nombre";
  LW_LANGFR_DATA[LW_LANG_STRING_WAVE_SPEED] = "Rapid";
  LW_LANGFR_DATA[LW_LANG_STRING_WAVE_WX] = "WX";
  LW_LANGFR_DATA[LW_LANG_STRING_WAVE_HY] = "HY";
  LW_LANGFR_DATA[LW_LANG_STRING_WAVE_WY] = "WY";
  LW_LANGFR_DATA[LW_LANG_STRING_WAVE_HX] = "HX";

  LW_LANGFR_DATA[LW_LANG_STRING_CONNECT_STARTNOW] = "Commencer";
  LW_LANGFR_DATA[LW_LANG_STRING_CONNECT_WAITING] = "En attente...";
  LW_LANGFR_DATA[LW_LANG_STRING_CONNECT_WAITINGFOR] = "Il manque ";
  LW_LANGFR_DATA[LW_LANG_STRING_CONNECT_TEAMS] = " equipe(s)";
  LW_LANGFR_DATA[LW_LANG_STRING_CONNECT_SENDMESSAGE] = "Envoyer";

  LW_LANGFR_DATA[LW_LANG_STRING_NETPLAY_CONNECT] = "Connexion";
  LW_LANGFR_DATA[LW_LANG_STRING_NETPLAY_PING] = "\"Ping\"";
  LW_LANGFR_DATA[LW_LANG_STRING_NETPLAY_CHECKPROGRAM] = "Verif prog";
  LW_LANGFR_DATA[LW_LANG_STRING_NETPLAY_CHECKVERSION] = "Verif version";
  LW_LANGFR_DATA[LW_LANG_STRING_NETPLAY_SENDPASSWORD] = "Mot de passe";
  LW_LANGFR_DATA[LW_LANG_STRING_NETPLAY_FREETEAMS] = "Place libre?";
  LW_LANGFR_DATA[LW_LANG_STRING_NETPLAY_RESERVETEAMS] = "Reservation";
  LW_LANGFR_DATA[LW_LANG_STRING_NETPLAY_SENDCONFIG] = "Envoi conf";
  LW_LANGFR_DATA[LW_LANG_STRING_NETPLAY_SENDMAP] = "Envoi tab";
  LW_LANGFR_DATA[LW_LANG_STRING_NETPLAY_WAITING] = "En attente...";
  LW_LANGFR_DATA[LW_LANG_STRING_NETPLAY_WHOPLAYS] = "Qui joue?";
  LW_LANGFR_DATA[LW_LANG_STRING_NETPLAY_RECVCONFIG] = "Lecture conf";
  LW_LANGFR_DATA[LW_LANG_STRING_NETPLAY_RECVMAP] = "Lecture tab";

  LW_LANGFR_DATA[LW_LANG_STRING_LEVEL_USEDEFAULTTEXTURE] =
    "Texture par defaut";
  LW_LANGFR_DATA[LW_LANG_STRING_LEVEL_ON] = "Oui";
  LW_LANGFR_DATA[LW_LANG_STRING_LEVEL_OFF] = "Non";
  LW_LANGFR_DATA[LW_LANG_STRING_LEVEL_RANDOMMAP] = "Tableau au hasard";

  LW_LANGFR_DATA[LW_LANG_STRING_CODE_FPS] = "affich/s ";
  LW_LANGFR_DATA[LW_LANG_STRING_CODE_RPS] = "tours/s ";
  LW_LANGFR_DATA[LW_LANG_STRING_CODE_PERCENTCALC] = "% calcul ";
  LW_LANGFR_DATA[LW_LANG_STRING_CODE_PERCENTDISPLAY] = "% affichage ";
  LW_LANGFR_DATA[LW_LANG_STRING_CODE_SPREADGRADTIME] = "Temps calc gradient ";
  LW_LANGFR_DATA[LW_LANG_STRING_CODE_MOVEARMYTIME] = "Temps deplac armees ";
  LW_LANGFR_DATA[LW_LANG_STRING_CODE_DISPLAYTIME] = "Temps affichage ";
  LW_LANGFR_DATA[LW_LANG_STRING_CODE_FLIPTIME] = "Temps \"page flip\" ";
  LW_LANGFR_DATA[LW_LANG_STRING_CODE_OTHERCALCTIME] = "Temps restant ";
  LW_LANGFR_DATA[LW_LANG_STRING_CODE_GLOBALTIME] = "Temps total ";
  LW_LANGFR_DATA[LW_LANG_STRING_CODE_STANDARDDISPLAY] = "Affichage standard";
  LW_LANGFR_DATA[LW_LANG_STRING_CODE_GRADIENT1] = "Gradient 1";
  LW_LANGFR_DATA[LW_LANG_STRING_CODE_GRADIENT2] = "Gradient 2";
  LW_LANGFR_DATA[LW_LANG_STRING_CODE_GRADIENT3] = "Gradient 3";
  LW_LANGFR_DATA[LW_LANG_STRING_CODE_GRADIENT4] = "Gradient 4";
  LW_LANGFR_DATA[LW_LANG_STRING_CODE_GRADIENT5] = "Gradient 5";
  LW_LANGFR_DATA[LW_LANG_STRING_CODE_GRADIENT6] = "Gradient 6";
  LW_LANGFR_DATA[LW_LANG_STRING_CODE_MESHSIZE] = "Forme du maillage";
  LW_LANGFR_DATA[LW_LANG_STRING_CODE_MESHSTRUCTURE] = "Structure du maillage";
  LW_LANGFR_DATA[LW_LANG_STRING_CODE_PAUSEOFF] = "Pause terminee";
  LW_LANGFR_DATA[LW_LANG_STRING_CODE_PAUSEON] = "Pause";
  LW_LANGFR_DATA[LW_LANG_STRING_CODE_WAVEEFFECTSOFF] =
    "Pas d'effets de vague";
  LW_LANGFR_DATA[LW_LANG_STRING_CODE_WAVEEFFECTSON] = "Effets de vague";
  LW_LANGFR_DATA[LW_LANG_STRING_CODE_CAPTUREOFF] = "Capture video desactivee";
  LW_LANGFR_DATA[LW_LANG_STRING_CODE_CAPTUREON] = "Capture video activee";

  LW_LANGFR_DATA[LW_LANG_STRING_SCORE_WINNER] = "Bravo!";

  LW_LANGFR_DATA[LW_LANG_STRING_GAME_BACKMEMFAIL] = "Pb mem \"back\"";
  LW_LANGFR_DATA[LW_LANG_STRING_GAME_DISPMEMFAIL] = "Pb mem \"disp\"";
  LW_LANGFR_DATA[LW_LANG_STRING_GAME_MESHMEMFAIL] = "Pb mem \"mesh\"";
  LW_LANGFR_DATA[LW_LANG_STRING_GAME_AREAMEMFAIL] = "Pb mem \"area\"";
  LW_LANGFR_DATA[LW_LANG_STRING_GAME_ARMYMEMFAIL] = "Pb mem \"army\"";

  LW_LANGFR_DATA[LW_LANG_STRING_ERROR_NOTENOUGHTEAMS] =
    "Pas assez d'equipes.\nIl faut selectionner au moins 2 equipes dans le menu \"Equipes\"";
  LW_LANGFR_DATA[LW_LANG_STRING_ERROR_NOTENOUGHBIGDATA] =
    "Pas assez de memoire.\nIl faut choisir un plus petit tableau ou bien lire la documentation et essayer des options du type \"-mem n\".";
  LW_LANGFR_DATA[LW_LANG_STRING_ERROR_GRAPHICSINITFAILED] =
    "Impossible d'initialiser le mode video.";
  LW_LANGFR_DATA[LW_LANG_STRING_ERROR_NONETWORKSUPPORT] =
    "Pas de support reseau pour cette plate-forme.\nLe reseau est disponiblre sous Windows et UNIX uniquement.";
  LW_LANGFR_DATA[LW_LANG_STRING_ERROR_UNABLETOCONNECT] =
    "Impossible de joindre le serveur.\nIl est peut-etre mal configure, ou bien vous etes derriere un pare-feu (firewall) qui vous empeche de vous connecter.";
  LW_LANGFR_DATA[LW_LANG_STRING_ERROR_NOTALIQUIDWARSERVER] =
    "Impossible de communiquer avec le serveur.\nIl semble que ce ne soit pas un serveur Liquid War.";
  LW_LANGFR_DATA[LW_LANG_STRING_ERROR_WRONGVERSION] =
    "Probleme de version client/serveur.\nLe client et le serveur doivent avoir la meme version (la version de ce client est \""
    LW_VERSION "\").";
  LW_LANGFR_DATA[LW_LANG_STRING_ERROR_NETWORKPROBLEM] =
    "Il y a eu une erreur reseau.\nCela peut arriver si la connexion est trop lente ou bien si elle a ete coupee.";
  LW_LANGFR_DATA[LW_LANG_STRING_ERROR_CANTSENDMAP] =
    "Impossible d'envoyer le tableau au serveur.";
  LW_LANGFR_DATA[LW_LANG_STRING_ERROR_CANTRECEIVEMAP] =
    "Impossible de recevoir le tableau du serveur.";
  LW_LANGFR_DATA[LW_LANG_STRING_ERROR_SERVERFULL] =
    "Ce serveur est plein, il n'accepte plus de nouvelles equipes.";
  LW_LANGFR_DATA[LW_LANG_STRING_ERROR_BADPASSWORD] =
    "Mot de passe incorrect.";
  LW_LANGFR_DATA[LW_LANG_STRING_ERROR_BADCONFIGRANGE] =
    "Un des parametres de configuration est en dehors des valeurs admises. Soit il y a un bug dans le jeu, soit quelqu'un est en train d'essayer de tricher et/ou de hacker quelque chose.";

  /*
   * In the following section we can use accented chars since these
   * labels are used in system popup windows which are supposed to
   * handle chars>127, unlike Liquid War graphical functions.
   */
  LW_LANGFR_DATA[LW_LANG_STRING_POPUP_INITFAIL] =
    "Echec lors de l'initialisation.";
  LW_LANGFR_DATA[LW_LANG_STRING_POPUP_MEMTROUBLE] =
    "Pas assez de mémoire pour les bitmaps.";
  LW_LANGFR_DATA[LW_LANG_STRING_POPUP_BIGDATA] = "Pas assez de mémoire.";
  LW_LANGFR_DATA[LW_LANG_STRING_POPUP_LOADFAIL] =
    "Impossible de charger le fichier de données. Etes-vous sûr que Liquid War est correctement installé?";
  LW_LANGFR_DATA[LW_LANG_STRING_POPUP_CUSTOMFAIL] =
    "Impossible de charger les fichiers de données utilisateur.";
  LW_LANGFR_DATA[LW_LANG_STRING_POPUP_GRAPHICSFAIL] =
    "Impossible de passer en mode graphique.";

  LW_LANGFR_DATA[LW_LANG_STRING_LW6_INFO] =
    "Ce programme est obsolete, vous devriez essayer Liquid War 6, disponible sur\n\nhttp://www.gnu.org/software/liquidwar6/\n\nDans tous les cas, Liquid War 5 reste encore disponible et vous pouvez y jouer librement. Seules les failles de securite majeures seront corrigees, ne pas s'attendre a de gros changement sur cette vieille version.";
  LW_LANGFR_DATA[LW_LANG_STRING_LW6_OK] = "OK";
}

/*------------------------------------------------------------------*/
/* returns the French string associated to an id                    */
/*------------------------------------------------------------------*/
char *
lw_langfr_string (int id)
{
  char *result = NULL;

  if (id >= 0 && id < LW_LANG_NB_STRINGS)
    {
      result = LW_LANGFR_DATA[id];
    }

  if (!result)
    {
      result = "??? fr";
    }

  return result;
}
