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
/* Copyright (C) 1998-2025 Christian Mauduit                                 */
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
/* name          : langdk.c                                         */
/* content       : handles Danish dependent stuff (locale)          */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>
#include <stdlib.h>

#include "lang.h"
#include "langdk.h"
#include "base.h"

/*==================================================================*/
/* globals                                                          */
/*==================================================================*/
static char *LW_LANGDK_DATA[LW_LANG_NB_STRINGS];

/*------------------------------------------------------------------*/
/* initializes the string table                                     */
/*------------------------------------------------------------------*/
void
lw_langdk_init ()
{
  memset (LW_LANGDK_DATA, 0, sizeof (LW_LANGDK_DATA));

  LW_LANGDK_DATA[LW_LANG_STRING_LANGUAGE] = "Dansk";
  LW_LANGDK_DATA[LW_LANG_STRING_APPNAME] = "Liquid War " LW_VERSION;

  LW_LANGDK_DATA[LW_LANG_STRING_HELP_CREDITS] =
    "Idea: \n* Thomas Colcombet\n\nCoding: \n* Christian Mauduit\n\nMusic:\n* Tim Chadburn\n\nOther contributors:\n* Alstar\n* Peter Wang\n* Cort Danger Stratton\n* Jan Gretschuskin (www.game-factor.de)\n* Mouse\n* Rene Stach\n* Roderick Schertler\n* Ryan D. Brown\n* Eduard Bloch\n* Michael Terry\n* Kasper Hviid\n* David Redick\n* Alexandre Pineau\n* Michael Wagner\n* Peter Williams\n* Jan Samohyl\n* Gavin\n* Dave Vasilevsky\n* 2 of Omega\n* 666-REFIZUL-666\n* Thomas Klausner\n* Joan Dolc\n* Mikhail Yakshin\n* Valerie Mauduit\n* Florence Mauduit\n* Joey Dodson\n* Jan Szenborn\n\nMore informations on www.ufoot.org/liquidwar/v5\n";
  LW_LANGDK_DATA[LW_LANG_STRING_HELP_COPYRIGHT] =
    "Liquid War er et flerpersoners krigsspil.\nCopyright (C) 1998-2025 Christian Mauduit (ufoot@ufoot.org)\n\nDette program er frit software; Du kan kopiere og aendre det under betingelserne i GNU General Public License, udgivet af Free Software Foundation; enten version 2 af Licensen, eller (dit valg) hvilken som helst senere version.\n\nDette program er udgivet i haabet om at det er brugbart, men UDEN NOGEN SOM HELST GARANTI; ikke engang nogen indforstaaet garanti for BRUGBARHED eller BRUGBARHED TIL ET SPECIFIKT FORMAAL. Laes GNU General Public License for flere detaljer.\n\nDu skulle have modtaget en kopi af GNU General Public License sammen med dette program; hvis ikke, skriv til Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA\n\nMere information paa www.ufoot.org/liquidwar/v5\n";
  /*
   * Stupid Microsoft VC++ can't handle strings over 2048 chars correctly,
   * (compiler error C2026) so we had to split this one...
   */
  LW_LANGDK_DATA[LW_LANG_STRING_HELP_LICENSE] =
    "GNU GENERAL PUBLIC LICENSE\n2. version, juni 1991 \n\nCopyright (C) 1989, 1991 Free Software Foundation, Inc. 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA \n\nEnhver har tilladelse til at kopiere og distribuere ordrette eksemplarer af dette licensdokument, men det er ikke tilladt at aendre det. \n\nForord \n\nDe fleste licenser paa software har til formaal at fjerne Deres ret til at dele softwaren med andre og aendre i det. I modsaetning hertil har GNU General Public License til formaal at garantere Deres ret til at dele og aendre frit software - for at sikre, at det paagaeldende software er tilgaengeligt for alle brugere. Denne licens, General Public License, er gaeldende for stoerstedelen af Free Software Foundations software samt for ethvert andet program, hvis ophavsmaend oensker at anvende licensen. (En del af Free Software Foundations programmer er i stedet omfattet af \"The GNU Library General Public License\".) De kan ogsaa anvende licensen til Deres programmer. \n\nVi taler om frit software - ikke gratis software. Dvs. vi taler om frihedsgraden, og ikke om prisen. Vores licenser, General Public Licenses, er udarbejdet med henblik paa at sikre Deres frihed til at distribuere kopier af frit software (og evt. tage betaling for denne ydelse) og at soerge for, at De modtager den paagaeldende kildetekst eller kan faa den, hvis De vil; at De kan aendre softwaren eller bruge dele af det til nye programmer; og at De er klar over, at De har disse rettigheder. \n\nFor at beskytte Deres rettigheder er vi noedt til at lave restriktioner, der forbyder andre at naegte Dem disse rettigheder eller kraeve, at De afstaar disse rettigheder. Disse restriktioner indebaerer visse forpligtelser for Dem, hvis De distribuerer kopier af softwaren eller aendrer det. \n\nHvis De f.eks. distribuerer kopier af et saadant program, enten gratis eller mod gebyr, er De forpligtet til at give modtagere alle de rettigheder, som De selv har. De skal sikre, at ogsaa modtagerne faar e"
    "ller kan faa fat paa kildeteksten. Desuden er De forpligtet til, at vise modtagerne disse betingelser, saa de ogsaa er bekendt med deres rettigheder. \n\nVi beskytter Deres rettigheder med en totrinsmodel: (1) vi tager ophavsret paa softwaren og (2) vi tilbyder Dem denne licens, der giver Dem juridisk ret til at kopiere, distribuere og/eller aendre softwaren. \n\nDesuden vil vi med henblik paa at beskytte os selv og de enkelte softwareproducenter sikre os, at alle er bekendte med, at der ikke ydes nogen garanti paa dette frie software. Hvis programmet er aendret af en anden og givet videre, vil vi have, at modtagerne skal vide, at deres version ikke er det originale program. Saaledes undgaar vi, at eventuelle problemer indarbejdet af andre ikke skader de oprindelige softwareproducenters ry. \n\nEndelig er ethvert frit program under konstant trussel fra softwarepatenter. Vi oensker at undgaa faren for, at de, der videregiver et frit program, personligt udtager patentlicens, hvorved programmet faktisk bliver beskyttet af ophavsret. For at undgaa dette har vi gjort det klart, at ethvert patent skal registreres til alles frie afbenyttelse eller slet ikke registreres. \n\nNedenfor foelger de betingelser og vilkaar, der gaelder i forhold til at kopiere, distribuere og aendre software under General Public License. \n\nBETINGELSER OG VILKAAR FOR KOPIERING, DISTRIBUTION OG AENDRING AF SOFTWARE UNDER GENERAL PUBLIC LICENSE \n\n0. Denne licens gaelder for ethvert program eller andet vaerk, som ved siden af navnet paa ophavsretindehaveren har en meddelelse om, at det paagaeldende produkt maa distribueres under de gaeldende betingelser for denne General Public License. I det foelgende henviser ordet \"Program\" til ethvert saadant program eller vaerk, og et \"Vaerk baseret paa Programmet\" betegner enten Programmet eller ethvert afledt vaerk i henhold til lovgivningen om ophavsret. Det betyder med andre ord et vaerk, der indeholder Programmet eller en del af det, enten ordret e"
    "ller aendret og/eller oversat til et andet sprog. (I det foelgende omfattes oversaettelser, uden begraensninger, af termen \"aendring\".) Enhver licenstager tiltales som \"De\". \n\nAktiviteter ud over kopiering, distribution og aendring er ikke daekket af denne licens men falder uden for dens omraade. Der er ingen restriktioner paa at koere Programmet, og outputtet fra Programmet er kun omfattet, hvis indholdet heraf udgoer et Vaerk baseret paa Programmet (uafhaengigt af at vaere blevet lavet ved at koere Programmet). Om dette er tilfaeldet, afhaenger af, hvad Programmet goer. \n\n1. De har ret til at kopiere og distribuere ordrette kopier af Programmets kildetekst, saaledes som De har modtaget den, i ethvert medium, under forudsaetning af, at alle kopier tydeligt og paa behoerig vis forsynes med en passende ophavsretmeddelelse og garantifraskrivelse; at alle meddelelser, der refererer til denne licens og den manglende garanti, bibeholdes; og at en kopi af denne licens vedlaegges, naar De distribuerer Programmet. \n\nDe har ret til at tage et gebyr for den fysiske handling at overfoere en kopi, og De kan eventuelt tilbyde garantibeskyttelse mod et gebyr. \n\n2. De har ret til at aendre Deres kopi eller kopier, eller en hvilken som helst del deraf, hvorved der skabes et Vaerk baseret paa Programmet, samt ret til at kopiere og distribuere saadanne aendringer eller vaerk i henhold til paragraf 1 ovenfor, forudsat De ogsaa opfylder foelgende betingelser: \nDe er forpligtet til at sikre, at de aendrede filer har en tydelig meddelelse om, at De har foretaget aendringer i filerne samt datoen herfor. \nDe er forpligtet til at sikre, at ethvert vaerk, som De distribuerer eller offentliggoer, der helt eller delvis indeholder Programmet, eller som er afledt af Programmet eller dele heraf, skal registreres til alles frie afbenyttelse i henhold til denne licens. \nHvis det aendrede program normalt laeser kommandoer interaktivt, naar det koeres, skal De sikre, at der ved starte"
    "n paa programkoerslen for interaktiv brug paa den mest normale vis udskrives eller vises en meddelelse, som indeholder behoerig ophavsretmeddelelse og garantifraskrivelse (eller alternativt tilkendegive, at De oensker at tilbyde garanti) samt fortaeller, at brugere har ret til videredistribuere Programmet under disse betingelser. Desuden skal meddelelsen orientere brugeren om, hvordan en kopi af denne licens kan vises. (Undtagelse: Hvis selve Programmet er interaktivt, men ikke normalt printer en saadan besked, kraeves det ikke, at Deres Vaerk baseret paa Programmet printer den omtalte besked.) \nDisse krav er gaeldende for det aendrede vaerk i sin helhed. Hvis identificerbare sektioner af vaerket ikke er afledt af Programmet, og hvis disse i sig selv med rimelighed kan anses for at vaere selvstaendige og separate vaerker, saa gaelder denne licens og dens betingelser ikke for de sektioner, naar De distribuerer dem som separate vaerker. Hvis De derimod distribuerer de samme sektioner som en del af et hele, der udgoer et Vaerk baseret paa Programmet, skal denne distribution vaere i overensstemmelse med licensens betingelser. De rettigheder, som licensen yder alle, udvides til det paagaeldende vaerk i sin helhed og gaelder saaledes for enhver del af vaerket, uanset ophavsmanden. \n\nDet er saaledes ikke vores hensigt med denne paragraf at paaberaabe os rettigheder eller bestride Deres rettigheder til vaerker skrevet af Dem alene. Hensigten er derimod at udoeve vores ret til at kontrollere distributionen af afledte vaerker eller kollektive vaerker baseret paa Programmet. \n\nHertil kommer, at forekomsten af et andet vaerk paa et lagrings- eller distributionsmedium, der indeholder Programmet eller et Vaerk baseret paa Programmet, medfoerer ikke, at det andet vaerk omfattes af betingelserne for denne licens. \n\n3. De har ret til at kopiere og distribuere Programmet (eller et Vaerk baseret paa Programmet i henhold til paragraf 2) i objektkode eller i eksekverbar form i h"
    "enhold til betingelserne i paragraf 1 og 2 under forudsaetning af, at De ogsaa foelger �n af disse retningslinjer: \nProgrammet ledsages af den fuldstaendige maskinlaesbare kildetekst, der skal distribueres under de anfoerte betingelser i paragraf 1 og 2 ovenfor i et medium, der saedvanligvis bruges til udveksling af software; eller \nProgrammet ledsages af et skriftligt tilbud - gaeldende i mindst tre aar - om at give tredjemand den fuldstaendige maskinlaesbare kildetekst, der skal distribueres under de anfoerte betingelser i paragraf 1 og 2 ovenfor i et medium, der saedvanligvis bruges til udveksling af software, mod et gebyr, der maksimalt daekker Deres omkostninger ved rent fysisk at udfoere distributionen,; eller \nProgrammet ledsages af den information, som De modtog vedroerende tilbuddet om at distribuere kildeteksten. (Dette alternativ gaelder udelukkende for ukommerciel distribution, og kun hvis De har modtaget Programmet i objektkode eller i eksekverbar form med et saadant tilbud i henhold til stykke b ovenfor.) \nKildeteksten for et vaerk betyder den form af et givet vaerk, der foretraekkes som grundlag for at aendre det. For et eksekverbart vaerk betyder den fuldstaendige kildetekst hele kildeteksten for alle de moduler, det indeholder, plus eventuelle tilhoerende graenseflade-definitionsfiler plus de skripter, der anvendes til at kontrollere kompileringen og installeringen af det eksekverbare software. Helt undtagelsesvis behoever den kildetekst, der distribueres, dog ikke at indeholde noget, der normalt distribueres (enten i kildeform eller i binaer form) sammen med de stoerre komponenter i operativsystemet (compiler, kerneprogram, osv.), som det eksekverbare program koeres paa, medmindre den komponent selv ledsager det eksekverbare program. \n\nHvis distributionen af det eksekverbare program eller objektkoden foretages ved tilbud om adgang til at kopiere fra et angivet sted, regnes tilbud om tilsvarende adgang til at kopiere kildeteksten fra det samm"
    "e sted som distribution af kildeteksten, ogsaa selv om tredjemand ikke er tvunget til at kopiere kildeteksten sammen med objektkoden. \n\n4. De har kun ret til at kopiere, aendre, udstede underlicenser for eller distribuere Programmet som udtrykkeligt anfoert i de specifikke betingelser for licensen. Herudover er ethvert forsoeg paa at kopiere, aendre, udstede underlicenser for eller distribuere Programmet ugyldigt og medfoerer automatisk ophoer af Deres rettigheder i henhold til denne licens. Paa den anden side vil tredjemand, der maatte have modtaget kopier eller rettigheder fra Dem i henhold til denne licens, ikke miste sin licens saa laenge alle betingelser for licensen overholdes. \n\n5. De er ikke forpligtet til at acceptere denne licens, eftersom De ikke har skrevet under paa den. De har dog ingen anden mulighed for at faa tilladelse til at aendre eller distribuere Programmet eller dets afledte vaerker, og de handlinger er forbudt ved lov, hvis De ikke accepterer denne licens. Ved at aendre eller distribuere Programmet (eller et Vaerk baseret paa Programmet) tilkendegiver De derfor Deres accept af licensen samt alle dens betingelser og vilkaar for at kopiere, distribuere eller aendre Programmet eller Vaerker baseret paa Programmet. \n\n6. Hver gang De distribuerer Programmet (eller et Vaerk baseret paa Programmet), faar modtageren automatisk licens fra den oprindelige licensgiver til at kopiere, distribuere eller aendre Programmet i henhold til disse betingelser og vilkaar. De har ikke ret til at paalaegge modtagerne yderligere restriktioner i deres brug af de rettigheder, der herved ydes. De er ikke ansvarlig for at haandhaeve tredjemands overholdelse af denne licens. \n\n7. Hvis De som foelge af en retsafgoerelse eller paastand om kraenkelse af patentret eller af en hvilken som helst anden grund (ikke begraenset til patentudstedelse) paatvinges vilkaar (det vaere sig ved retskendelse, kontrakt eller andet), der strider mod vilkaarene for denne licens, er D"
    "e ikke dermed fritaget for vilkaarene for denne licens. Hvis De ikke kan distribuere Programmet, saaledes at De paa samme tid opfylder Deres forpligtelser i henhold til denne licens og i overensstemmelse med andre relevante forpligtelser, foelger det, at De ikke har tilladelse til at distribuere Programmet overhovedet. Hvis f.eks. en patentlicens ikke ville tillade afgiftsfri videredistribution af Programmet via alle dem, der modtager kopier direkte eller indirekte igennem Dem, ville den eneste maade, hvorpaa De kunne overholde baade patentlicensen og denne licens, at De fuldstaendigt afholder Dem fra at distribuere Programmet. \n\nDersom en del af denne paragraf erklaeres ugyldig eller bliver umulig at haandhaeve i en bestemt situation, er det hensigten, at resten af paragraffen skal finde anvendelse, ligesom det er hensigten, at paragraffen som helhed skal gaelde i andre situationer.  \nDet er saaledes ikke formaalet med denne paragraf at tilskynde Dem til at kraenke andre patenter eller ejendomskrav eller at bestride gyldigheden af saadanne krav. Denne paragraf har udelukkende til formaal at beskytte integriteten af distributionssystemet for frit software, som er implementeret gennem offentlig licenspraksis. Der er mange mennesker, der har ydet store bidrag til det brede udvalg af software, der distribueres via dette system i tiltro til, at systemet anvendes konsekvent. Det er op til softwareproducenten/donoren at beslutte, om han eller hun er villig til at distribuere software via et andet system, og en licenstager har ikke ret til at paatvinge nogen et saadant valg. \n\nFormaalet med denne paragraf er at goere det fuldstaendigt klart, hvad konsekvensen af resten af denne licens menes at vaere. \n\n8. Hvis distributionen og/eller brugen af Programmet er underlagt restriktioner i visse lande pga. enten patenter eller graenseflader, der er belagt med ophavsret, kan den oprindelige ophavsretindehaver, der har placeret Programmet under denne licens, tilfoeje en udt"
    "rykkelig geografisk distributionsbegraensning, der udelukker de paagaeldende lande, saaledes at distribution kun er tilladt i eller imellem lande, der ikke paa denne vis er udelukket. I tilfaelde heraf inkorporerer denne licens en saadan begraensning som om den var indarbejdet i selve licenstekstens ordlyd. \n\n9. The Free Software Foundation offentliggoer formodentligt reviderede og/eller nye versioner af sin General Public License fra tid til anden. Saadanne nye versioner vil udtrykke den samme grundlaeggende tankegang som den nuvaerende version, men visse detaljer vil sikkert aendres for at tage hoejde for nye vinkler eller problemer. \n\nDet er let at kende forskel paa de forskellige versioner, idet de alle er tydeligt forsynet med et nummer. Hvis Programmet specificerer et versionsnummer for denne licens som gaeldende for Programmet, tillige med ordene \"alle senere versioner\", har De mulighed for at vaelge, om De vil foelge vilkaar og betingelser for enten den version eller en anden senere version udgivet af The Free Software Foundation. Hvis Programmet ikke specificerer et versionsnummer for denne licens, kan De vaelge blandt samtlige versioner, der nogensinde er udgivet af The Free Software Foundation. \n\n10. Hvis De oensker at inkorporere dele af Programmet i andre frie programmer, der er underlagt andre distributionsbetingelser, foreslaar vi, at De skriver til ophavsmanden og anmoder om tilladelse. Hvis det drejer sig om software, hvor ophavsretten tilhoerer The Free Software Foundation, bedes De skrive til The Free Software Foundation. Der er visse undtagelser. Vores afgoerelse vil vaere baseret paa to maal - at bevare den frie status for alle afledte vaerker af vores frie software og at fremme udvekslingen og genbrugen af software generelt. \n\n\nINGEN GARANTI \n\n11. IDET LICENSEN GIVER GRATIS BRUGSTILLADELSE PAA PROGRAMMET, YDES DER INGEN GARANTI PAA PROGRAMMET I DET OMFANG, DET ER TILLADT EFTER GAELDENDE LOV. MEDMINDRE DET UDTRYKKELIGT MEDDELES SKR"
    "IFTLIGT, YDER OPHAVSRETINDEHAVERNE OG/ELLER ANDRE PARTER PROGRAMMET \"SOM BESET\" UDEN GARANTI AF NOGEN ART, DET VAERE SIG UDTRYKTE ELLER UNDERFORSTAAEDE GARANTIER, HERUNDER, MEN IKKE BEGRAENSET TIL, DE UNDERFORSTAAEDE GARANTIER VEDROERENDE SALGBARHED OG SPECIFIK BRUGSEGNETHED. DEN FULDE RISIKO, HVAD ANGAAR PROGRAMMETS KVALITET OG FUNKTION, PAAHVILER DEM. SKULLE DET VISE SIG, AT PROGRAMMET ER DEFEKT, SKAL DE ERHOLDE UDGIFTERNE TIL AL NOEDVENDIG SERVICE, REPARATION ELLER JUSTERING. \n\n12. UNDER INGEN OMSTAENDIGHEDER - MEDMINDRE DET KRAEVES AF GAELDENDE LOV ELLER ER SKRIFTLIGT AFTALT - SKAL EN OPHAVSRETINDEHAVER ELLER EN ANDEN PART, DER HAR TILLADELSE TIL AT AENDRE OG/ELLER DISTRIBUERE PROGRAMMET SAALEDES SOM BESKREVET OVENFOR, VAERE ERSTATNINGSANSVARLIG OVER FOR DEM VEDROERENDE SKADER, HERUNDER GENERELLE, SPECIFIKKE OG TILFAELDIGE SKADER SAMT FOELGESKADER I FORBINDELSE MED BRUG AF ELLER MANGLENDE BRUG AF PROGRAMMET (HERUNDER MEN IKKE BEGRAENSET TIL TAB AF DATA ELLER DATA, DER ER BLEVET UNOEJAGTIGE, ELLER TAB, DER ER PAAFOERT DEM ELLER TREDJEMAND, ELLER PROGRAMMETS MANGLENDE EVNE TIL AT KOERE SAMMEN MED ANDRE PROGRAMMER) SELV OM DEN PAAGAELDENDE OPHAVSRETINDEHAVER ELLER ANDEN PART ER BLEVET OPLYST OM MULIGHEDEN FOR, AT SAADANNE TAB KUNNE OPSTAA. \n\nHER SLUTTER BETINGELSER OG VILKAAR FOR GENERAL PUBLIC LICENSE\n\nMore informations on www.ufoot.org/liquidwar/v5\n";

  LW_LANGDK_DATA[LW_LANG_STRING_HELP_VERSION] =
    "Liquid War by U-Foot\n\nv" LW_VERSION " \n\ncompiled on " __DATE__
    " at " __TIME__ "\n\nMere information paa www.ufoot.org/liquidwar/v5\n";

  LW_LANGDK_DATA[LW_LANG_STRING_MENU_PLAY] = "Start";
  LW_LANGDK_DATA[LW_LANG_STRING_MENU_NETGAME] = "Netspil";
  LW_LANGDK_DATA[LW_LANG_STRING_MENU_MAP] = "Baner";
  LW_LANGDK_DATA[LW_LANG_STRING_MENU_TEAMS] = "Hold";
  LW_LANGDK_DATA[LW_LANG_STRING_MENU_OPTIONS] = "Indstillinger";
  LW_LANGDK_DATA[LW_LANG_STRING_MENU_ABOUT] = "Om spillet";
  LW_LANGDK_DATA[LW_LANG_STRING_MENU_EXIT] = "Afslut";
  LW_LANGDK_DATA[LW_LANG_STRING_MENU_EXITWINDOWS] = "Afslut";
  LW_LANGDK_DATA[LW_LANG_STRING_MENU_EXITUNIX] = "Afslut";
  LW_LANGDK_DATA[LW_LANG_STRING_MENU_BACK] = "Tilbage til LW";

  LW_LANGDK_DATA[LW_LANG_STRING_DIALOG_QUIT] = "Afslut";
  LW_LANGDK_DATA[LW_LANG_STRING_DIALOG_BACK] = "Tilbage";
  LW_LANGDK_DATA[LW_LANG_STRING_DIALOG_PLAY] = "Start";
  LW_LANGDK_DATA[LW_LANG_STRING_DIALOG_MAIN] = "Menu";

  LW_LANGDK_DATA[LW_LANG_STRING_PLAY_GETREADY] = "Lad kampen begynde!";

  LW_LANGDK_DATA[LW_LANG_STRING_NETGAME_STARTGAME] = "Start spil";
  LW_LANGDK_DATA[LW_LANG_STRING_NETGAME_SERVERADDR] = "Server addr";
  LW_LANGDK_DATA[LW_LANG_STRING_NETGAME_SERVERPORT] = "Server port";
  LW_LANGDK_DATA[LW_LANG_STRING_NETGAME_SEARCH] = "Find internet-spil";
  LW_LANGDK_DATA[LW_LANG_STRING_NETGAME_PASSWORD] = "kodeord (*)";
  LW_LANGDK_DATA[LW_LANG_STRING_NETGAME_WEAKPASSWORD] =
    "(*) ADVARSEL! Benyt et svagt kodeord som \"hallo\", da det opbevares paa din harddisk og sendes til serveren som ren tekst.";

  LW_LANGDK_DATA[LW_LANG_STRING_LANGUAGE_AUTO] = "Automatisk";

  LW_LANGDK_DATA[LW_LANG_STRING_TEAM_OFF] = "Inaktiv";
  LW_LANGDK_DATA[LW_LANG_STRING_TEAM_HUMAN] = "Spiller";
  LW_LANGDK_DATA[LW_LANG_STRING_TEAM_CPU] = "CPU";

  LW_LANGDK_DATA[LW_LANG_STRING_OPTIONS_GRAPHICS] = "Grafik";
  LW_LANGDK_DATA[LW_LANG_STRING_OPTIONS_SOUND] = "Lyd";
  LW_LANGDK_DATA[LW_LANG_STRING_OPTIONS_RULES] = "Regler";
  LW_LANGDK_DATA[LW_LANG_STRING_OPTIONS_SPEEDS] = "Fart";
  LW_LANGDK_DATA[LW_LANG_STRING_OPTIONS_CONTROLS] = "Kontrol";
  LW_LANGDK_DATA[LW_LANG_STRING_OPTIONS_LANGUAGE] = "Sprog";
  LW_LANGDK_DATA[LW_LANG_STRING_OPTIONS_DEFAULT] = "Standard";

  LW_LANGDK_DATA[LW_LANG_STRING_GRAPHICS_GAME] = "Spil";
  LW_LANGDK_DATA[LW_LANG_STRING_GRAPHICS_MENU] = "Menu";
  LW_LANGDK_DATA[LW_LANG_STRING_GRAPHICS_ON] = "Til";
  LW_LANGDK_DATA[LW_LANG_STRING_GRAPHICS_OFF] = "Fra";
  LW_LANGDK_DATA[LW_LANG_STRING_GRAPHICS_FULLSCREEN] = "Fuldskaerm";
  LW_LANGDK_DATA[LW_LANG_STRING_GRAPHICS_WINDOWED] = "Windowed";
  LW_LANGDK_DATA[LW_LANG_STRING_GRAPHICS_ADVANCED] = "Advanceret";
  LW_LANGDK_DATA[LW_LANG_STRING_GRAPHICS_VIDEOMODE] = "Video tilstand";
  LW_LANGDK_DATA[LW_LANG_STRING_GRAPHICS_BRIGHTNESS] = "Lysstyrke";
  LW_LANGDK_DATA[LW_LANG_STRING_GRAPHICS_VIEWPORTSIZE] = "Rammebredte";
  LW_LANGDK_DATA[LW_LANG_STRING_GRAPHICS_PAGEFLIPPING] = "Page flipping";
  LW_LANGDK_DATA[LW_LANG_STRING_GRAPHICS_WAVES] = "Waves (F4)";

  LW_LANGDK_DATA[LW_LANG_STRING_VOLUME_SFX] = "Lydeffekter";
  LW_LANGDK_DATA[LW_LANG_STRING_VOLUME_CLICK] = "Klik";
  LW_LANGDK_DATA[LW_LANG_STRING_VOLUME_GAMEWATER] = "Vand i spil";
  LW_LANGDK_DATA[LW_LANG_STRING_VOLUME_MENUWATER] = "Vand i menu";
  LW_LANGDK_DATA[LW_LANG_STRING_VOLUME_MUSIC] = "MusiK";

  LW_LANGDK_DATA[LW_LANG_STRING_RULES_TIME] = "Tid";
  LW_LANGDK_DATA[LW_LANG_STRING_RULES_ADVANCED] = "Advanceret";
  LW_LANGDK_DATA[LW_LANG_STRING_RULES_ARMYSIZE] = "Antal soldat";
  LW_LANGDK_DATA[LW_LANG_STRING_RULES_CURSOR] = "Curser";
  LW_LANGDK_DATA[LW_LANG_STRING_RULES_DEFAULT] = "Standard";
  LW_LANGDK_DATA[LW_LANG_STRING_RULES_MINMAPRES] = "Min ";

  LW_LANGDK_DATA[LW_LANG_STRING_ADVANCED_ATTACK] = "Angrib";
  LW_LANGDK_DATA[LW_LANG_STRING_ADVANCED_DEFENSE] = "Forsvar";
  LW_LANGDK_DATA[LW_LANG_STRING_ADVANCED_BASEHEALTH] = "Basalt helbred";
  LW_LANGDK_DATA[LW_LANG_STRING_ADVANCED_WINNERHELP] = "Vinderhjaelp";
  LW_LANGDK_DATA[LW_LANG_STRING_ADVANCED_CPUSTRENGTH] = "CPU styrke";
  LW_LANGDK_DATA[LW_LANG_STRING_ADVANCED_CPUVSHUMAN] = "CPU vs spiller";
  LW_LANGDK_DATA[LW_LANG_STRING_ADVANCED_ALWAYS] = "Altid";
  LW_LANGDK_DATA[LW_LANG_STRING_ADVANCED_RANDOM] = "Tilfaeldigt";
  LW_LANGDK_DATA[LW_LANG_STRING_ADVANCED_NEVER] = "Aldrig";
  LW_LANGDK_DATA[LW_LANG_STRING_ADVANCED_ALLOWNETWORKBOTS] = "Bots i netspil";
  LW_LANGDK_DATA[LW_LANG_STRING_ADVANCED_YES] = "Ja";
  LW_LANGDK_DATA[LW_LANG_STRING_ADVANCED_NO] = "Nej";

  LW_LANGDK_DATA[LW_LANG_STRING_SPEEDS_FPS] = "frames/s";
  LW_LANGDK_DATA[LW_LANG_STRING_SPEEDS_NOFPSLIMIT] = "Ingen fps-graense";
  LW_LANGDK_DATA[LW_LANG_STRING_SPEEDS_RPS] = "Omgange/s";
  LW_LANGDK_DATA[LW_LANG_STRING_SPEEDS_NORPSLIMIT] = "Ingen rps-graense";

  LW_LANGDK_DATA[LW_LANG_STRING_CONTROLS_JOYSTICK] = "Joystick";
  LW_LANGDK_DATA[LW_LANG_STRING_CONTROLS_OFF] = "Fra";
  LW_LANGDK_DATA[LW_LANG_STRING_CONTROLS_2JOYSTICKS] = "2 joysticks";
  LW_LANGDK_DATA[LW_LANG_STRING_CONTROLS_4BUTTONS] = "4 knapper";
  LW_LANGDK_DATA[LW_LANG_STRING_CONTROLS_STANDARD] = "Standard";
  LW_LANGDK_DATA[LW_LANG_STRING_CONTROLS_MOUSESENSIB] =
    "In-game mouse sensibility";

  LW_LANGDK_DATA[LW_LANG_STRING_ABOUT_CREDITS] = "Credits";
  LW_LANGDK_DATA[LW_LANG_STRING_ABOUT_COPYRIGHT] = "Copyright";
  LW_LANGDK_DATA[LW_LANG_STRING_ABOUT_LICENSE] = "Licens";
  LW_LANGDK_DATA[LW_LANG_STRING_ABOUT_VERSION] = "Version";

  LW_LANGDK_DATA[LW_LANG_STRING_INTERNET_REFRESH] = "Opdater";
  LW_LANGDK_DATA[LW_LANG_STRING_INTERNET_JOINNOW] = "Log ind";
  LW_LANGDK_DATA[LW_LANG_STRING_INTERNET_PORT] = "Port";
  LW_LANGDK_DATA[LW_LANG_STRING_INTERNET_PASSWORD] = "Kodeord";
  LW_LANGDK_DATA[LW_LANG_STRING_INTERNET_YES] = "Ja";
  LW_LANGDK_DATA[LW_LANG_STRING_INTERNET_NO] = "Nej";
  LW_LANGDK_DATA[LW_LANG_STRING_INTERNET_UPTIME] = "Uptime";
  LW_LANGDK_DATA[LW_LANG_STRING_INTERNET_DAYS] = "dage";
  LW_LANGDK_DATA[LW_LANG_STRING_INTERNET_DAY] = "dage";
  LW_LANGDK_DATA[LW_LANG_STRING_INTERNET_HOURS] = "t";
  LW_LANGDK_DATA[LW_LANG_STRING_INTERNET_MINUTES] = "min";
  LW_LANGDK_DATA[LW_LANG_STRING_INTERNET_SECONDS] = "sek";
  LW_LANGDK_DATA[LW_LANG_STRING_INTERNET_UNREACHABLE] = "Unreachable";
  LW_LANGDK_DATA[LW_LANG_STRING_INTERNET_PING] = "Ping";
  LW_LANGDK_DATA[LW_LANG_STRING_INTERNET_MSEC] = "ms";

  LW_LANGDK_DATA[LW_LANG_STRING_WAVE_AMPLI] = "Ampli";
  LW_LANGDK_DATA[LW_LANG_STRING_WAVE_NUMBER] = "Antal";
  LW_LANGDK_DATA[LW_LANG_STRING_WAVE_SPEED] = "Fart";
  LW_LANGDK_DATA[LW_LANG_STRING_WAVE_WX] = "WX";
  LW_LANGDK_DATA[LW_LANG_STRING_WAVE_HY] = "HY";
  LW_LANGDK_DATA[LW_LANG_STRING_WAVE_WY] = "WY";
  LW_LANGDK_DATA[LW_LANG_STRING_WAVE_HX] = "HX";

  LW_LANGDK_DATA[LW_LANG_STRING_CONNECT_STARTNOW] = "Start nu";
  LW_LANGDK_DATA[LW_LANG_STRING_CONNECT_WAITING] = "Vent...";
  LW_LANGDK_DATA[LW_LANG_STRING_CONNECT_WAITINGFOR] = "Venter paa ";
  LW_LANGDK_DATA[LW_LANG_STRING_CONNECT_TEAMS] = " hold";
  LW_LANGDK_DATA[LW_LANG_STRING_CONNECT_SENDMESSAGE] = "Send besked";

  LW_LANGDK_DATA[LW_LANG_STRING_NETPLAY_CONNECT] = "Connect";
  LW_LANGDK_DATA[LW_LANG_STRING_NETPLAY_PING] = "Ping";
  LW_LANGDK_DATA[LW_LANG_STRING_NETPLAY_CHECKPROGRAM] = "Check program";
  LW_LANGDK_DATA[LW_LANG_STRING_NETPLAY_CHECKVERSION] = "Check version";
  LW_LANGDK_DATA[LW_LANG_STRING_NETPLAY_SENDPASSWORD] = "Kodeord";
  LW_LANGDK_DATA[LW_LANG_STRING_NETPLAY_FREETEAMS] = "Frie hold?";
  LW_LANGDK_DATA[LW_LANG_STRING_NETPLAY_RESERVETEAMS] = "Reserver hold";
  LW_LANGDK_DATA[LW_LANG_STRING_NETPLAY_SENDCONFIG] = "Send config";
  LW_LANGDK_DATA[LW_LANG_STRING_NETPLAY_SENDMAP] = "Send bane";
  LW_LANGDK_DATA[LW_LANG_STRING_NETPLAY_WAITING] = "venter...";
  LW_LANGDK_DATA[LW_LANG_STRING_NETPLAY_WHOPLAYS] = "Hvem spiller?";
  LW_LANGDK_DATA[LW_LANG_STRING_NETPLAY_RECVCONFIG] = "Modtag konfig";
  LW_LANGDK_DATA[LW_LANG_STRING_NETPLAY_RECVMAP] = "Modtag bane";

  LW_LANGDK_DATA[LW_LANG_STRING_LEVEL_USEDEFAULTTEXTURE] =
    "Brug standard-texture";
  LW_LANGDK_DATA[LW_LANG_STRING_LEVEL_ON] = "Til";
  LW_LANGDK_DATA[LW_LANG_STRING_LEVEL_OFF] = "Fra";
  LW_LANGDK_DATA[LW_LANG_STRING_LEVEL_RANDOMMAP] = "Tilfaeldig bane";

  LW_LANGDK_DATA[LW_LANG_STRING_CODE_FPS] = "frames/s ";
  LW_LANGDK_DATA[LW_LANG_STRING_CODE_RPS] = "rounds/s ";
  LW_LANGDK_DATA[LW_LANG_STRING_CODE_PERCENTCALC] = "% calc ";
  LW_LANGDK_DATA[LW_LANG_STRING_CODE_PERCENTDISPLAY] = "% display ";
  LW_LANGDK_DATA[LW_LANG_STRING_CODE_SPREADGRADTIME] = "Spread grad time ";
  LW_LANGDK_DATA[LW_LANG_STRING_CODE_MOVEARMYTIME] = "Move army time ";
  LW_LANGDK_DATA[LW_LANG_STRING_CODE_DISPLAYTIME] = "Vis tid ";
  LW_LANGDK_DATA[LW_LANG_STRING_CODE_FLIPTIME] = "Flip tid ";
  LW_LANGDK_DATA[LW_LANG_STRING_CODE_OTHERCALCTIME] = "Other calc time ";
  LW_LANGDK_DATA[LW_LANG_STRING_CODE_GLOBALTIME] = "Global tid ";
  LW_LANGDK_DATA[LW_LANG_STRING_CODE_STANDARDDISPLAY] = "Standard display";
  LW_LANGDK_DATA[LW_LANG_STRING_CODE_GRADIENT1] = "Gradient 1";
  LW_LANGDK_DATA[LW_LANG_STRING_CODE_GRADIENT2] = "Gradient 2";
  LW_LANGDK_DATA[LW_LANG_STRING_CODE_GRADIENT3] = "Gradient 3";
  LW_LANGDK_DATA[LW_LANG_STRING_CODE_GRADIENT4] = "Gradient 4";
  LW_LANGDK_DATA[LW_LANG_STRING_CODE_GRADIENT5] = "Gradient 5";
  LW_LANGDK_DATA[LW_LANG_STRING_CODE_GRADIENT6] = "Gradient 6";
  LW_LANGDK_DATA[LW_LANG_STRING_CODE_MESHSIZE] = "Gitter-stoerrelse";
  LW_LANGDK_DATA[LW_LANG_STRING_CODE_MESHSTRUCTURE] = "gitterstruktur";
  LW_LANGDK_DATA[LW_LANG_STRING_CODE_PAUSEOFF] = "Pause Fra";
  LW_LANGDK_DATA[LW_LANG_STRING_CODE_PAUSEON] = "Pause til";
  LW_LANGDK_DATA[LW_LANG_STRING_CODE_WAVEEFFECTSOFF] = "Wave-effekter fra";
  LW_LANGDK_DATA[LW_LANG_STRING_CODE_WAVEEFFECTSON] = "Wave-effekter til";
  LW_LANGDK_DATA[LW_LANG_STRING_CODE_CAPTUREOFF] = "Video capture fra";
  LW_LANGDK_DATA[LW_LANG_STRING_CODE_CAPTUREON] = "Video capture til";

  LW_LANGDK_DATA[LW_LANG_STRING_SCORE_WINNER] = "Vinder";

  LW_LANGDK_DATA[LW_LANG_STRING_GAME_BACKMEMFAIL] = "Back mem fail";
  LW_LANGDK_DATA[LW_LANG_STRING_GAME_DISPMEMFAIL] = "Disp mem fail";
  LW_LANGDK_DATA[LW_LANG_STRING_GAME_MESHMEMFAIL] = "Mesh mem fail";
  LW_LANGDK_DATA[LW_LANG_STRING_GAME_AREAMEMFAIL] = "Area mem fail";
  LW_LANGDK_DATA[LW_LANG_STRING_GAME_ARMYMEMFAIL] = "Army mem fail";

  LW_LANGDK_DATA[LW_LANG_STRING_ERROR_NOTENOUGHTEAMS] =
    "Ikke nok hold.\nDu skal mindst vaelge 2 hold fra \"Hold\" menuen.";
  LW_LANGDK_DATA[LW_LANG_STRING_ERROR_NOTENOUGHBIGDATA] =
    "Ikke nok hukommelse til at udvide level.\nBrug et mindre level eller laes dokumentationen og brug kommandolinie-parametre som \"-mem n\".";
  LW_LANGDK_DATA[LW_LANG_STRING_ERROR_GRAPHICSINITFAILED] =
    "Kan ikke starte grafisk tilstand";
  LW_LANGDK_DATA[LW_LANG_STRING_ERROR_NONETWORKSUPPORT] =
    "Intet netvaerkssupport paa denne platform.\nKun Windows og UNIX har det.";
  LW_LANGDK_DATA[LW_LANG_STRING_ERROR_UNABLETOCONNECT] =
    "Kan ikke faa forbindelse med serveren.\nMaaske er det ikke korrekt sat op, eller du er bag en firewall der forbyder forbindelsen.";
  LW_LANGDK_DATA[LW_LANG_STRING_ERROR_NOTALIQUIDWARSERVER] =
    "Kan ikke kommunikere med serveren.\nDet er nok ikke nogen Liquid War server.";
  LW_LANGDK_DATA[LW_LANG_STRING_ERROR_WRONGVERSION] =
    "Forkert client/server version.\nServer og client skal have samme version (Denne klient's version er \""
    LW_VERSION "\").";
  LW_LANGDK_DATA[LW_LANG_STRING_ERROR_NETWORKPROBLEM] =
    "Der er sket en netvaerksfejl.\nDette sker ofte hvis serveren er for langsom eller mistet.";
  LW_LANGDK_DATA[LW_LANG_STRING_ERROR_CANTSENDMAP] =
    "Kan ikke sende banen til serveren.";
  LW_LANGDK_DATA[LW_LANG_STRING_ERROR_CANTRECEIVEMAP] =
    "Kan ikke modtage banen fra serveren.";
  LW_LANGDK_DATA[LW_LANG_STRING_ERROR_SERVERFULL] =
    "Serveren er fuld og accepterer ikke flere hold.";
  LW_LANGDK_DATA[LW_LANG_STRING_ERROR_BADPASSWORD] = "Forkert kodeord.";
  LW_LANGDK_DATA[LW_LANG_STRING_ERROR_BADCONFIGRANGE] =
    "En af konfigurationsparametrene er is \"out of range\". Der en enten en fejl i spillet, nogen snyder og/eller proever at hacke noget.";

  LW_LANGDK_DATA[LW_LANG_STRING_POPUP_INITFAIL] = "Init failed.";
  LW_LANGDK_DATA[LW_LANG_STRING_POPUP_MEMTROUBLE] =
    "Der er ikke nok hukommelse til bitmaps.";
  LW_LANGDK_DATA[LW_LANG_STRING_POPUP_BIGDATA] =
    "Der er ikke nok hukommelse.";
  LW_LANGDK_DATA[LW_LANG_STRING_POPUP_LOADFAIL] =
    "Kan ikke loade datafil. Er du sikker paa at Liquid War er korrekt installeret?";
  LW_LANGDK_DATA[LW_LANG_STRING_POPUP_CUSTOMFAIL] =
    "Kan ikke loade custom data.";
  LW_LANGDK_DATA[LW_LANG_STRING_POPUP_GRAPHICSFAIL] =
    "Kan ikke initialisere grafik.";

  LW_LANGDK_DATA[LW_LANG_STRING_LW6_INFO] =
    "This program is obsolete, please consider upgrading to Liquid War 6, available on\n\nhttp://www.gnu.org/software/liquidwar6/\n\nHowever, Liquid War 5 is still available for your convenience, and you can play it freely. Only major security holes will be fixed, expect no major changes on this legacy branch.";
  LW_LANGDK_DATA[LW_LANG_STRING_LW6_OK] = "OK";
}


/*------------------------------------------------------------------*/
/* returns the English string associated to an id                   */
/*------------------------------------------------------------------*/
char *
lw_langdk_string (int id)
{
  char *result = NULL;
  if (id >= 0 && id < LW_LANG_NB_STRINGS)

    {
      result = LW_LANGDK_DATA[id];
    }
  if (!result)

    {
      result = "??? en";
    }
  return result;
}
