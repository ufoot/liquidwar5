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
/* Autor of translation: aso.gdynia@gmail.com (http://aso.uh.net.pl)		 */
/*****************************************************************************/

/********************************************************************/
/* name          : langpl.c                                         */
/* content       : handles Polish dependent stuff (locale)          */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>
#include <stdlib.h>

#include "lang.h"
#include "langpl.h"
#include "base.h"

/*==================================================================*/
/* globals                                                          */
/*==================================================================*/

static char *LW_LANGPL_DATA[LW_LANG_NB_STRINGS];

/*------------------------------------------------------------------*/
/* initializes the string table                                     */
/*------------------------------------------------------------------*/
void
lw_langpl_init ()
{
  memset (LW_LANGPL_DATA, 0, sizeof (LW_LANGPL_DATA));

  LW_LANGPL_DATA[LW_LANG_STRING_LANGUAGE] = "Polski";
  LW_LANGPL_DATA[LW_LANG_STRING_APPNAME] = "Liquid War " LW_VERSION;

  LW_LANGPL_DATA[LW_LANG_STRING_HELP_CREDITS] =
    "Idea:: \n* Thomas Colcombet\n\nKod: \n* Christian Mauduit\n\nMuzyka:\n* Tim Chadburn\n\nInni:\n* Alstar\n* Peter Wang\n* Cort Danger Stratton\n* Jan Gretschuskin (www.game-factor.de)\n* Mouse\n* Rene Stach\n* Roderick Schertler\n* Ryan D. Brown\n* Eduard Bloch\n* Michael Terry\n* Kasper Hviid\n* David Redick\n* Alexandre Pineau\n* Michael Wagner\n* Peter Williams\n* Jan Samohyl\n* Gavin\n* Dave Vasilevsky\n* 2 of Omega\n* 666-REFIZUL-666\n* Thomas Klausner\n* Joan Dolc\n* Mikhail Yakshin\n* Valerie Mauduit\n* Florence Mauduit\n* Joey Dodson\n* Jan Szenborn\n\nWięcej informacji na www.ufoot.org/liquidwar/v5\n";
  LW_LANGPL_DATA[LW_LANG_STRING_HELP_COPYRIGHT] =
    "Liquid War jest wojenną grą multiplayer.\nCopyright (C) 1998-2025 Christian Mauduit (ufoot@ufoot.org)\n\nTen program jest darmowy; możesz go rozpowszechniać i/lub modyfikować zgodnie z zasadami GNU General Public License wydanej przez Free Software Foundation, zgodnie z wersją drugą tej licencji albo (według Twojego wyboru) późniejszą wersją.\n\nTen program jest rozpowszechniany z nadzieją na bycie użytecznym, ale CAŁKOWICIE BEZ GWARANCJI; nawet bez gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH ZASTOSOWAŃ. Zobacz GNU General Public License po więcej szczegółów.\n\nPowinieneś dostać kopię licencji GNU razem z tym programem; jeżeli nie, napisz do Free Software Foundation, Inc., 51 Franklin St, 5 piętro, Boston, MA  02110-1301  USA\n\nWięcej informacji na www.ufoot.org/liquidwar/v5\n";
  /*
   * Stupid Microsoft VC++ can't handle strings over 2048 chars correctly,
   * (compiler error C2026) so we had to split this one...
   */
  LW_LANGPL_DATA[LW_LANG_STRING_HELP_LICENSE] =
    "GNU GENERAL PUBLIC LICENSE\nVersion 2, June 1991\n\nCopyright (C) 1989, 1991 Free Software Foundation, Inc. 675 Mass Ave, Cambridge, MA 02139, USA\n\nEveryone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.\n\nPreamble\n\nThe licenses for most software are designed to take away your freedom to share and change it.  By contrast, the GNU General Public License is intended to guarantee your freedom to share and change free software--to make sure the software is free for all its users.  This General Public License applies to most of the Free Software Foundation's software and to any other program whose authors commit to using it.  (Some other Free Software Foundation software is covered by the GNU Library General Public License instead.)  You can apply it to your programs, too.\n\nWhen we speak of free software, we are referring to freedom, not price.  Our General Public Licenses are designed to make sure that you have the freedom "
    "to distribute copies of free software (and charge for this service if you wish), that you receive source code or can get it if you want it, that you can change the software or use pieces of it in new free programs; and that you know you can do these things. \n\nTo protect your rights, we need to make restrictions that forbid anyone to deny you these rights or to ask you to surrender the rights. These restrictions translate to certain responsibilities for you if you distribute copies of the software, or if you modify it. \n\nFor example, if you distribute copies of such a program, whether gratis or for a fee, you must give the recipients all the rights that you have.  You must make sure that they, too, receive or can get the source code.  And you must show them these terms so they know their rights.\n\nWe protect your rights with two steps: (1) copyright the software, and (2) offer you this license which gives you legal permission to copy, distribute and/or modify the software.\n\nAlso, " "for each author's protection and ours, we want to make certain that everyone understands that there is no warranty for this free software.  If the software is modified by someone else and passed on, we want its recipients to know that what they have is not the original, so that any problems introduced by others will not reflect on the original authors' reputations.\n\nFinally, any free program is threatened constantly by software patents.  We wish to avoid the danger that redistributors of a free program will individually obtain patent licenses, in effect making the program proprietary.  To prevent this, we have made it clear that any patent must be licensed for everyone's free use or not licensed at all.\n\nThe precise terms and conditions for copying, distribution and modification follow.\n\nGNU GENERAL PUBLIC LICENSE\nTERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION\n\n0. This License applies to any program or other work which contains a notice placed by the copyright " "holder saying it may be distributed under the terms of this General Public License.  The \"Program\", below, refers to any such program or work, and a \"work based on the Program\" means either the Program or any derivative work under copyright law: that is to say, a work containing the Program or a portion of it, either verbatim or with modifications and/or translated into another language.  (Hereinafter, translation is included without limitation in the term \"modification\".)  Each licensee is addressed as \"you\".\nActivities other than copying, distribution and modification are not covered by this License; they are outside its scope.  The act of running the Program is not restricted, and the output from the Program is covered only if its contents constitute a work based on the Program (independent of having been made by running the Program). Whether that is true depends on what the Program does. \n\n1. You may copy and distribute verbatim copies of the Program's source code as you " "receive it, in any medium, provided that you conspicuously and appropriately publish on each copy an appropriate copyright notice and disclaimer of warranty; keep intact all the notices that refer to this License and to the absence of any warranty; and give any other recipients of the Program a copy of this License along with the Program. \nYou may charge a fee for the physical act of transferring a copy, and you may at your option offer warranty protection in exchange for a fee.\n\n2. You may modify your copy or copies of the Program or any portion of it, thus forming a work based on the Program, and copy and distribute such modifications or work under the terms of Section 1 above, provided that you also meet all of these conditions: \na) You must cause the modified files to carry prominent notices stating that you changed the files and the date of any change.\nb) You must cause any work that you distribute or publish, that in whole or in part contains or is derived from the Program or " "any part thereof, to be licensed as a whole at no charge to all third parties under the terms of this License.\nc) If the modified program normally reads commands interactively when run, you must cause it, when started running for such interactive use in the most ordinary way, to print or display an announcement including an appropriate copyright notice and a notice that there is no warranty (or else, saying that you provide a warranty) and that users may redistribute the program under these conditions, and telling the user how to view a copy of this License.  (Exception: if the Program itself is interactive but does not normally print such an announcement, your work based on the Program is not required to print an announcement.)\nThese requirements apply to the modified work as a whole.  If identifiable sections of that work are not derived from the Program, and can be reasonably considered independent and separate works in themselves, then this License, and its terms, do not apply to " "those sections when you distribute them as separate works.  But when you distribute the same sections as part of a whole which is a work based on the Program, the distribution of the whole must be on the terms of this License, whose permissions for other licensees extend to the entire whole, and thus to each and every part regardless of who wrote it.\nThus, it is not the intent of this section to claim rights or contest your rights to work written entirely by you; rather, the intent is to exercise the right to control the distribution of derivative or collective works based on the Program. \nIn addition, mere aggregation of another work not based on the Program with the Program (or with a work based on the Program) on a volume of a storage or distribution medium does not bring the other work under the scope of this License. \n\n3. You may copy and distribute the Program (or a work based on it, under Section 2) in object code or executable form under the terms of Sections 1 and 2 above " "provided that you also do one of the following:\na) Accompany it with the complete corresponding machine-readable source code, which must be distributed under the terms of Sections 1 and 2 above on a medium customarily used for software interchange; or,\nb) Accompany it with a written offer, valid for at least three years, to give any third party, for a charge no more than your cost of physically performing source distribution, a complete machine-readable copy of the corresponding source code, to be distributed under the terms of Sections 1 and 2 above on a medium customarily used for software interchange; or,\nc) Accompany it with the information you received as to the offer to distribute corresponding source code.  (This alternative is allowed only for noncommercial distribution and only if you received the program in object code or executable form with such an offer, in accord with Subsection b above.)\nThe source code for a work means the preferred form of the work for making modifications " "to it.  For an executable work, complete source code means all the source code for all modules it contains, plus any associated interface definition files, plus the scripts used to control compilation and installation of the executable.  However, as a special exception, the source code distributed need not include anything that is normally distributed (in either source or binary form) with the major components (compiler, kernel, and so on) of the operating system on which the executable runs, unless that component itself accompanies the executable.\nIf distribution of executable or object code is made by offering access to copy from a designated place, then offering equivalent access to copy the source code from the same place counts as distribution of the source code, even though third parties are not compelled to copy the source along with the object code.\n\n4. You may not copy, modify, sublicense, or distribute the Program except as expressly provided under this License.  Any attempt " "otherwise to copy, modify, sublicense or distribute the Program is void, and will automatically terminate your rights under this License. However, parties who have received copies, or rights, from you under this License will not have their licenses terminated so long as such parties remain in full compliance.\n\n5. You are not required to accept this License, since you have not signed it.  However, nothing else grants you permission to modify or distribute the Program or its derivative works.  These actions are prohibited by law if you do not accept this License.  Therefore, by modifying or distributing the Program (or any work based on the Program), you indicate your acceptance of this License to do so, and all its terms and conditions for copying, distributing or modifying the Program or works based on it.\n\n6. Each time you redistribute the Program (or any work based on the Program), the recipient automatically receives a license from the original licensor to copy, distribute or modify " "the Program subject to these terms and conditions.  You may not impose any further restrictions on the recipients' exercise of the rights granted herein. You are not responsible for enforcing compliance by third parties to this License.\n\n7. If, as a consequence of a court judgment or allegation of patent infringement or for any other reason (not limited to patent issues), conditions are imposed on you (whether by court order, agreement or otherwise) that contradict the conditions of this License, they do not excuse you from the conditions of this License.  If you cannot distribute so as to satisfy simultaneously your obligations under this License and any other pertinent obligations, then as a consequence you may not distribute the Program at all.  For example, if a patent license would not permit royalty-free redistribution of the Program by all those who receive copies directly or indirectly through you, then the only way you could satisfy both it and this License would be to refrain " "entirely from distribution of the Program.\nIf any portion of this section is held invalid or unenforceable under any particular circumstance, the balance of the section is intended to apply and the section as a whole is intended to apply in other circumstances.\nIt is not the purpose of this section to induce you to infringe any patents or other property right claims or to contest validity of any such claims; this section has the sole purpose of protecting the integrity of the free software distribution system, which is implemented by public license practices.  Many people have made generous contributions to the wide range of software distributed through that system in reliance on consistent application of that system; it is up to the author/donor to decide if he or she is willing to distribute software through any other system and a licensee cannot impose that choice.\nThis section is intended to make thoroughly clear what is believed to be a consequence of the rest of this License.\n\n8. " "If the distribution and/or use of the Program is restricted in certain countries either by patents or by copyrighted interfaces, the original copyright holder who places the Program under this License may add an explicit geographical distribution limitation excluding those countries, so that distribution is permitted only in or among countries not thus excluded.  In such case, this License incorporates the limitation as if written in the body of this License.\n\n9. The Free Software Foundation may publish revised and/or new versions of the General Public License from time to time.  Such new versions will be similar in spirit to the present version, but may differ in detail to address new problems or concerns.\nEach version is given a distinguishing version number.  If the Program specifies a version number of this License which applies to it and \"any later version\", you have the option of following the terms and conditions either of that version or of any later version published by the " "Free Software Foundation.  If the Program does not specify a version number of this License, you may choose any version ever published by the Free Software Foundation. \n\n10. If you wish to incorporate parts of the Program into other free programs whose distribution conditions are different, write to the author to ask for permission.  For software which is copyrighted by the Free Software Foundation, write to the Free Software Foundation; we sometimes make exceptions for this.  Our decision will be guided by the two goals of preserving the free status of all derivatives of our free software and of promoting the sharing and reuse of software generally.\n\nNO WARRANTY\n\n11. BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT " "NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION. \n\n12. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR REDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.\n\nEND OF TERMS AND CONDITIONS\n\nMore informations on www.ufoot.org/liquidwar/v5\n";
  LW_LANGPL_DATA[LW_LANG_STRING_HELP_VERSION] =
    "Liquid War by U-Foot\n\nv" LW_VERSION " \n\nskompilowany " __DATE__ " o "
    __TIME__ "\n\nWięcej informacji na www.ufoot.org/liquidwar/v5\n";

  LW_LANGPL_DATA[LW_LANG_STRING_MENU_PLAY] = "Graj";
  LW_LANGPL_DATA[LW_LANG_STRING_MENU_NETGAME] = "Gra sieciowa";
  LW_LANGPL_DATA[LW_LANG_STRING_MENU_MAP] = "Mapy";
  LW_LANGPL_DATA[LW_LANG_STRING_MENU_TEAMS] = "Drużyny";
  LW_LANGPL_DATA[LW_LANG_STRING_MENU_OPTIONS] = "Opcje";
  LW_LANGPL_DATA[LW_LANG_STRING_MENU_ABOUT] = "Informacje";
  LW_LANGPL_DATA[LW_LANG_STRING_MENU_EXIT] = "Wyjście";
  LW_LANGPL_DATA[LW_LANG_STRING_MENU_EXITWINDOWS] = "Wyjście";
  LW_LANGPL_DATA[LW_LANG_STRING_MENU_EXITUNIX] = "Wyjście";
  LW_LANGPL_DATA[LW_LANG_STRING_MENU_BACK] = "Wróć do LW";

  LW_LANGPL_DATA[LW_LANG_STRING_DIALOG_QUIT] = "Zamknij";
  LW_LANGPL_DATA[LW_LANG_STRING_DIALOG_BACK] = "Wróć";
  LW_LANGPL_DATA[LW_LANG_STRING_DIALOG_PLAY] = "Graj";
  LW_LANGPL_DATA[LW_LANG_STRING_DIALOG_MAIN] = "Menu główne";

  LW_LANGPL_DATA[LW_LANG_STRING_PLAY_GETREADY] = "Przygotuj się!";

  LW_LANGPL_DATA[LW_LANG_STRING_NETGAME_STARTGAME] = "Rozpocznij grę";
  LW_LANGPL_DATA[LW_LANG_STRING_NETGAME_SERVERADDR] = "Adres serwera";
  LW_LANGPL_DATA[LW_LANG_STRING_NETGAME_SERVERPORT] = "Port serwera";
  LW_LANGPL_DATA[LW_LANG_STRING_NETGAME_SEARCH] = "Szukaj gier w internecie";
  LW_LANGPL_DATA[LW_LANG_STRING_NETGAME_PASSWORD] = "Hasło (*)";
  LW_LANGPL_DATA[LW_LANG_STRING_NETGAME_WEAKPASSWORD] =
    "(*) UWAGA! Wybierz słabe hasło typu \"hello\", ponieważ jest przechowywane i wysyłane do serwera w postaci czystego tekstu.";

  LW_LANGPL_DATA[LW_LANG_STRING_LANGUAGE_AUTO] = "Automatycznie";

  LW_LANGPL_DATA[LW_LANG_STRING_TEAM_OFF] = "Wyłącz";
  LW_LANGPL_DATA[LW_LANG_STRING_TEAM_HUMAN] = "Człowiek";
  LW_LANGPL_DATA[LW_LANG_STRING_TEAM_CPU] = "Komputer";

  LW_LANGPL_DATA[LW_LANG_STRING_OPTIONS_GRAPHICS] = "Grafika";
  LW_LANGPL_DATA[LW_LANG_STRING_OPTIONS_SOUND] = "Dźwięk";
  LW_LANGPL_DATA[LW_LANG_STRING_OPTIONS_RULES] = "Zasady";
  LW_LANGPL_DATA[LW_LANG_STRING_OPTIONS_SPEEDS] = "Prędkość";
  LW_LANGPL_DATA[LW_LANG_STRING_OPTIONS_CONTROLS] = "Klawisze";
  LW_LANGPL_DATA[LW_LANG_STRING_OPTIONS_LANGUAGE] = "Język";
  LW_LANGPL_DATA[LW_LANG_STRING_OPTIONS_DEFAULT] = "Ust. domyślne";

  LW_LANGPL_DATA[LW_LANG_STRING_GRAPHICS_GAME] = "Gra";
  LW_LANGPL_DATA[LW_LANG_STRING_GRAPHICS_MENU] = "Menu";
  LW_LANGPL_DATA[LW_LANG_STRING_GRAPHICS_ON] = "Wł.";
  LW_LANGPL_DATA[LW_LANG_STRING_GRAPHICS_OFF] = "Wył.";
  LW_LANGPL_DATA[LW_LANG_STRING_GRAPHICS_FULLSCREEN] = "Pełen ekran";
  LW_LANGPL_DATA[LW_LANG_STRING_GRAPHICS_WINDOWED] = "Okno";
  LW_LANGPL_DATA[LW_LANG_STRING_GRAPHICS_ADVANCED] = "Zaawansowane";
  LW_LANGPL_DATA[LW_LANG_STRING_GRAPHICS_VIDEOMODE] = "Tryb grafiki";
  LW_LANGPL_DATA[LW_LANG_STRING_GRAPHICS_BRIGHTNESS] = "Jasność";
  LW_LANGPL_DATA[LW_LANG_STRING_GRAPHICS_VIEWPORTSIZE] =
    "Obszar generowania grafiki";
  LW_LANGPL_DATA[LW_LANG_STRING_GRAPHICS_PAGEFLIPPING] = "Page flipping";
  LW_LANGPL_DATA[LW_LANG_STRING_GRAPHICS_WAVES] = "Fale (F4)";

  LW_LANGPL_DATA[LW_LANG_STRING_VOLUME_SFX] = "Dźwięki";
  LW_LANGPL_DATA[LW_LANG_STRING_VOLUME_CLICK] = "Klik";
  LW_LANGPL_DATA[LW_LANG_STRING_VOLUME_GAMEWATER] = "Woda w grze";
  LW_LANGPL_DATA[LW_LANG_STRING_VOLUME_MENUWATER] = "Woda w menu";
  LW_LANGPL_DATA[LW_LANG_STRING_VOLUME_MUSIC] = "Muzyka";

  LW_LANGPL_DATA[LW_LANG_STRING_RULES_TIME] = "Czas";
  LW_LANGPL_DATA[LW_LANG_STRING_RULES_ADVANCED] = "Zaawansowane";
  LW_LANGPL_DATA[LW_LANG_STRING_RULES_ARMYSIZE] = "Rozmiar armii";
  LW_LANGPL_DATA[LW_LANG_STRING_RULES_CURSOR] = "Kursor";
  LW_LANGPL_DATA[LW_LANG_STRING_RULES_DEFAULT] = "Domyślnie";
  LW_LANGPL_DATA[LW_LANG_STRING_RULES_MINMAPRES] = "Min. ";

  LW_LANGPL_DATA[LW_LANG_STRING_ADVANCED_ATTACK] = "Atak";
  LW_LANGPL_DATA[LW_LANG_STRING_ADVANCED_DEFENSE] = "Obrona";
  LW_LANGPL_DATA[LW_LANG_STRING_ADVANCED_BASEHEALTH] = "Zdrowie bazowe";
  LW_LANGPL_DATA[LW_LANG_STRING_ADVANCED_WINNERHELP] = "Pomoc zwycięscy";
  LW_LANGPL_DATA[LW_LANG_STRING_ADVANCED_CPUSTRENGTH] = "Siła CPU";
  LW_LANGPL_DATA[LW_LANG_STRING_ADVANCED_CPUVSHUMAN] =
    "Komputer przeciwko graczowi";
  LW_LANGPL_DATA[LW_LANG_STRING_ADVANCED_ALWAYS] = "Zawsze";
  LW_LANGPL_DATA[LW_LANG_STRING_ADVANCED_RANDOM] = "Losowo";
  LW_LANGPL_DATA[LW_LANG_STRING_ADVANCED_NEVER] = "Nigdy";
  LW_LANGPL_DATA[LW_LANG_STRING_ADVANCED_ALLOWNETWORKBOTS] =
    "Zezwól na boty w sieci";
  LW_LANGPL_DATA[LW_LANG_STRING_ADVANCED_YES] = "Tak";
  LW_LANGPL_DATA[LW_LANG_STRING_ADVANCED_NO] = "Nie";

  LW_LANGPL_DATA[LW_LANG_STRING_SPEEDS_FPS] = "klatki/s";
  LW_LANGPL_DATA[LW_LANG_STRING_SPEEDS_NOFPSLIMIT] = "Brak limitu klatek";
  LW_LANGPL_DATA[LW_LANG_STRING_SPEEDS_RPS] = "rounds/s";
  LW_LANGPL_DATA[LW_LANG_STRING_SPEEDS_NORPSLIMIT] = "Brak limitu RPS";

  LW_LANGPL_DATA[LW_LANG_STRING_CONTROLS_JOYSTICK] = "Joystick";
  LW_LANGPL_DATA[LW_LANG_STRING_CONTROLS_OFF] = "Wyłączony";
  LW_LANGPL_DATA[LW_LANG_STRING_CONTROLS_2JOYSTICKS] = "2 joystiki";
  LW_LANGPL_DATA[LW_LANG_STRING_CONTROLS_4BUTTONS] = "4 przyciski";
  LW_LANGPL_DATA[LW_LANG_STRING_CONTROLS_STANDARD] = "Standardowo";
  LW_LANGPL_DATA[LW_LANG_STRING_CONTROLS_MOUSESENSIB] =
    "Czułość myszy w grze";

  LW_LANGPL_DATA[LW_LANG_STRING_ABOUT_CREDITS] = "Autorzy";
  LW_LANGPL_DATA[LW_LANG_STRING_ABOUT_COPYRIGHT] = "Prawa autorskie";
  LW_LANGPL_DATA[LW_LANG_STRING_ABOUT_LICENSE] = "Licencja";
  LW_LANGPL_DATA[LW_LANG_STRING_ABOUT_VERSION] = "Wersja";

  LW_LANGPL_DATA[LW_LANG_STRING_INTERNET_REFRESH] = "Odśwież";
  LW_LANGPL_DATA[LW_LANG_STRING_INTERNET_JOINNOW] = "Dołącz";
  LW_LANGPL_DATA[LW_LANG_STRING_INTERNET_PORT] = "Port";
  LW_LANGPL_DATA[LW_LANG_STRING_INTERNET_PASSWORD] = "Hasło";
  LW_LANGPL_DATA[LW_LANG_STRING_INTERNET_YES] = "tak";
  LW_LANGPL_DATA[LW_LANG_STRING_INTERNET_NO] = "nie";
  LW_LANGPL_DATA[LW_LANG_STRING_INTERNET_UPTIME] = "Czas działania";
  LW_LANGPL_DATA[LW_LANG_STRING_INTERNET_DAYS] = "dni";
  LW_LANGPL_DATA[LW_LANG_STRING_INTERNET_DAY] = "dzień";
  LW_LANGPL_DATA[LW_LANG_STRING_INTERNET_HOURS] = "h";
  LW_LANGPL_DATA[LW_LANG_STRING_INTERNET_MINUTES] = "min";
  LW_LANGPL_DATA[LW_LANG_STRING_INTERNET_SECONDS] = "s";
  LW_LANGPL_DATA[LW_LANG_STRING_INTERNET_UNREACHABLE] = "Nieosiągalny";
  LW_LANGPL_DATA[LW_LANG_STRING_INTERNET_PING] = "Ping";
  LW_LANGPL_DATA[LW_LANG_STRING_INTERNET_MSEC] = "ms";

  LW_LANGPL_DATA[LW_LANG_STRING_WAVE_AMPLI] = "Amplituda";
  LW_LANGPL_DATA[LW_LANG_STRING_WAVE_NUMBER] = "Liczba";
  LW_LANGPL_DATA[LW_LANG_STRING_WAVE_SPEED] = "PRędkość";
  LW_LANGPL_DATA[LW_LANG_STRING_WAVE_WX] = "WX";
  LW_LANGPL_DATA[LW_LANG_STRING_WAVE_HY] = "HY";
  LW_LANGPL_DATA[LW_LANG_STRING_WAVE_WY] = "WY";
  LW_LANGPL_DATA[LW_LANG_STRING_WAVE_HX] = "HX";

  LW_LANGPL_DATA[LW_LANG_STRING_CONNECT_STARTNOW] = "Rozpicznij";
  LW_LANGPL_DATA[LW_LANG_STRING_CONNECT_WAITING] = "Oczekiwanie...";
  LW_LANGPL_DATA[LW_LANG_STRING_CONNECT_WAITINGFOR] = "Oczekiwanie na ";
  LW_LANGPL_DATA[LW_LANG_STRING_CONNECT_TEAMS] = " drużyn(ę)";
  LW_LANGPL_DATA[LW_LANG_STRING_CONNECT_SENDMESSAGE] = "Wyślij wiadomość";

  LW_LANGPL_DATA[LW_LANG_STRING_NETPLAY_CONNECT] = "Połącz";
  LW_LANGPL_DATA[LW_LANG_STRING_NETPLAY_PING] = "Ping";
  LW_LANGPL_DATA[LW_LANG_STRING_NETPLAY_CHECKPROGRAM] = "Sprawdź program";
  LW_LANGPL_DATA[LW_LANG_STRING_NETPLAY_CHECKVERSION] = "Sprawdź wersję";
  LW_LANGPL_DATA[LW_LANG_STRING_NETPLAY_SENDPASSWORD] = "Hasło";
  LW_LANGPL_DATA[LW_LANG_STRING_NETPLAY_FREETEAMS] = "Free teams?";
  LW_LANGPL_DATA[LW_LANG_STRING_NETPLAY_RESERVETEAMS] = "Zarezerwuj drużyny";
  LW_LANGPL_DATA[LW_LANG_STRING_NETPLAY_SENDCONFIG] = "Wyślij ustawienia";
  LW_LANGPL_DATA[LW_LANG_STRING_NETPLAY_SENDMAP] = "Wyślij mapę";
  LW_LANGPL_DATA[LW_LANG_STRING_NETPLAY_WAITING] = "Oczekiwanie...";
  LW_LANGPL_DATA[LW_LANG_STRING_NETPLAY_WHOPLAYS] = "Kto gra?";
  LW_LANGPL_DATA[LW_LANG_STRING_NETPLAY_RECVCONFIG] = "Odbierz ustawienia";
  LW_LANGPL_DATA[LW_LANG_STRING_NETPLAY_RECVMAP] = "Odbierz mapę";

  LW_LANGPL_DATA[LW_LANG_STRING_LEVEL_USEDEFAULTTEXTURE] =
    "Użyj domyślnej tekstury";
  LW_LANGPL_DATA[LW_LANG_STRING_LEVEL_ON] = "Wł.";
  LW_LANGPL_DATA[LW_LANG_STRING_LEVEL_OFF] = "Wył.";
  LW_LANGPL_DATA[LW_LANG_STRING_LEVEL_RANDOMMAP] = "Losowa mapa";

  LW_LANGPL_DATA[LW_LANG_STRING_CODE_FPS] = "klatki/s ";
  LW_LANGPL_DATA[LW_LANG_STRING_CODE_RPS] = "rounds/s ";
  LW_LANGPL_DATA[LW_LANG_STRING_CODE_PERCENTCALC] = "% calc ";
  LW_LANGPL_DATA[LW_LANG_STRING_CODE_PERCENTDISPLAY] = "% ekranu ";
  LW_LANGPL_DATA[LW_LANG_STRING_CODE_SPREADGRADTIME] = "Spread grad time ";
  LW_LANGPL_DATA[LW_LANG_STRING_CODE_MOVEARMYTIME] = "Czas ruchu armii ";
  LW_LANGPL_DATA[LW_LANG_STRING_CODE_DISPLAYTIME] = "Czas wyświetlania ";
  LW_LANGPL_DATA[LW_LANG_STRING_CODE_FLIPTIME] = "Flip time ";
  LW_LANGPL_DATA[LW_LANG_STRING_CODE_OTHERCALCTIME] = "Other calc time ";
  LW_LANGPL_DATA[LW_LANG_STRING_CODE_GLOBALTIME] = "Czas globalny ";
  LW_LANGPL_DATA[LW_LANG_STRING_CODE_STANDARDDISPLAY] = "Standardowy ekran";
  LW_LANGPL_DATA[LW_LANG_STRING_CODE_GRADIENT1] = "Gradient 1";
  LW_LANGPL_DATA[LW_LANG_STRING_CODE_GRADIENT2] = "Gradient 2";
  LW_LANGPL_DATA[LW_LANG_STRING_CODE_GRADIENT3] = "Gradient 3";
  LW_LANGPL_DATA[LW_LANG_STRING_CODE_GRADIENT4] = "Gradient 4";
  LW_LANGPL_DATA[LW_LANG_STRING_CODE_GRADIENT5] = "Gradient 5";
  LW_LANGPL_DATA[LW_LANG_STRING_CODE_GRADIENT6] = "Gradient 6";
  LW_LANGPL_DATA[LW_LANG_STRING_CODE_MESHSIZE] = "Mesh size";
  LW_LANGPL_DATA[LW_LANG_STRING_CODE_MESHSTRUCTURE] = "Mesh structure";
  LW_LANGPL_DATA[LW_LANG_STRING_CODE_PAUSEOFF] = "Wył. pauzę";
  LW_LANGPL_DATA[LW_LANG_STRING_CODE_PAUSEON] = "Wł. pauzę";
  LW_LANGPL_DATA[LW_LANG_STRING_CODE_WAVEEFFECTSOFF] = "Efekt fali wył.";
  LW_LANGPL_DATA[LW_LANG_STRING_CODE_WAVEEFFECTSON] = "Efekt fali wł.";
  LW_LANGPL_DATA[LW_LANG_STRING_CODE_CAPTUREOFF] = "Video capture off";
  LW_LANGPL_DATA[LW_LANG_STRING_CODE_CAPTUREON] = "Video capture on";

  LW_LANGPL_DATA[LW_LANG_STRING_SCORE_WINNER] = "Zwycięsca";

  LW_LANGPL_DATA[LW_LANG_STRING_GAME_BACKMEMFAIL] = "Back mem fail";
  LW_LANGPL_DATA[LW_LANG_STRING_GAME_DISPMEMFAIL] = "Disp mem fail";
  LW_LANGPL_DATA[LW_LANG_STRING_GAME_MESHMEMFAIL] = "Mesh mem fail";
  LW_LANGPL_DATA[LW_LANG_STRING_GAME_AREAMEMFAIL] = "Area mem fail";
  LW_LANGPL_DATA[LW_LANG_STRING_GAME_ARMYMEMFAIL] = "Army mem fail";

  LW_LANGPL_DATA[LW_LANG_STRING_ERROR_NOTENOUGHTEAMS] =
    "Za mało drużyn.\nMusisz zaznaczyć min. 2 drużyny z menu \"Drużyny\".";
  LW_LANGPL_DATA[LW_LANG_STRING_ERROR_NOTENOUGHBIGDATA] =
    "Brak pamięci do obsługi poziomu.\nWybierz mniejszy poziom albo zobacz do dokumentacji i wypróbuj opcje z linii komend, np. \"-mem n\".";
  LW_LANGPL_DATA[LW_LANG_STRING_ERROR_GRAPHICSINITFAILED] =
    "Nie udało się uruchomić trybu graficznego";
  LW_LANGPL_DATA[LW_LANG_STRING_ERROR_NONETWORKSUPPORT] =
    "Brak wsparcia sieci dla tej platformy.\nTylko Windows i UNIX obsługują sieć.";
  LW_LANGPL_DATA[LW_LANG_STRING_ERROR_UNABLETOCONNECT] =
    "Nie można się połączyć z serwerem.\nMożliwe że nie jest poprawnie skonfigurowany albo jesteś za firewallem który blokuje połączenie.";
  LW_LANGPL_DATA[LW_LANG_STRING_ERROR_NOTALIQUIDWARSERVER] =
    "Brak komunikacji z serwerem.\nPrawdopodobnie nie jest to serwer Liquid War.";
  LW_LANGPL_DATA[LW_LANG_STRING_ERROR_WRONGVERSION] =
    "Zła wersja klienta/serwera.\nSerwer i klient muszą być w tej samej wersji (wersja tej gry to \""
    LW_VERSION "\").";
  LW_LANGPL_DATA[LW_LANG_STRING_ERROR_NETWORKPROBLEM] =
    "Wystąpił błąd sieci.\nTo występuje często gdy połączenie z serwerem jest za wolne albo zostało utracone.";
  LW_LANGPL_DATA[LW_LANG_STRING_ERROR_CANTSENDMAP] =
    "Nie udało się wysłać mapy na serwer.";
  LW_LANGPL_DATA[LW_LANG_STRING_ERROR_CANTRECEIVEMAP] =
    "Nie udało się odebrać mapy z serwera.";
  LW_LANGPL_DATA[LW_LANG_STRING_ERROR_SERVERFULL] =
    "Serwer jest obecnie pełny i nie akceptuje nowych drużyn.";
  LW_LANGPL_DATA[LW_LANG_STRING_ERROR_BADPASSWORD] = "Nieprawidłowe hasło.";
  LW_LANGPL_DATA[LW_LANG_STRING_ERROR_BADCONFIGRANGE] =
    "Jeden z parametrów gry jest poza zakresem. To może być błąd gry lub ktoś próbuje cheatów i/lub hacków.";

  LW_LANGPL_DATA[LW_LANG_STRING_POPUP_INITFAIL] = "Błąd inicjacji.";
  LW_LANGPL_DATA[LW_LANG_STRING_POPUP_MEMTROUBLE] =
    "Brak wystarczającej pamięci na bitmapy.";
  LW_LANGPL_DATA[LW_LANG_STRING_POPUP_BIGDATA] = "Za mało pamięci.";
  LW_LANGPL_DATA[LW_LANG_STRING_POPUP_LOADFAIL] =
    "Nie można załadować pliku danych. Gra jest zainstalowany poprawnie?";
  LW_LANGPL_DATA[LW_LANG_STRING_POPUP_CUSTOMFAIL] =
    "Nie mozna załadować własnych danych.";
  LW_LANGPL_DATA[LW_LANG_STRING_POPUP_GRAPHICSFAIL] =
    "Nie można zainicjować grafiki.";

  LW_LANGPL_DATA[LW_LANG_STRING_LW6_INFO] =
    "This program is obsolete, please consider upgrading to Liquid War 6, available on\n\nhttp://www.gnu.org/software/liquidwar6/\n\nHowever, Liquid War 5 is still available for your convenience, and you can play it freely. Only major security holes will be fixed, expect no major changes on this legacy branch.";
  LW_LANGPL_DATA[LW_LANG_STRING_LW6_OK] = "OK";
}

/*------------------------------------------------------------------*/
/* returns the English string associated to an id                   */
/*------------------------------------------------------------------*/
char *
lw_langpl_string (int id)
{
  char *result = NULL;

  if (id >= 0 && id < LW_LANG_NB_STRINGS)
    {
      result = LW_LANGPL_DATA[id];
    }

  if (!result)
    {
      result = "??? en";
    }

  return result;
}
