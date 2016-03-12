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
/* Copyright (C) 1998-2016 Christian Mauduit                                 */
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
/* Liquid War homepage : http://www.ufoot.org/liquidwar/v5                   */
/* Contact author      : ufoot@ufoot.org                                     */
/*****************************************************************************/

/********************************************************************/
/* name          : langen.c                                         */
/* content       : handles English dependent stuff (locale)         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>
#include <stdlib.h>

#include "lang.h"
#include "langen.h"
#include "base.h"

/*==================================================================*/
/* globals                                                          */
/*==================================================================*/

static char *LW_LANGEN_DATA[LW_LANG_NB_STRINGS];

/*------------------------------------------------------------------*/
/* initializes the string table                                     */
/*------------------------------------------------------------------*/
void
lw_langen_init ()
{
  memset (LW_LANGEN_DATA, 0, sizeof (LW_LANGEN_DATA));

  LW_LANGEN_DATA[LW_LANG_STRING_LANGUAGE] = "English";
  LW_LANGEN_DATA[LW_LANG_STRING_APPNAME] = "Liquid War " LW_VERSION;

  LW_LANGEN_DATA[LW_LANG_STRING_HELP_CREDITS] =
    "Idea: \n* Thomas Colcombet\n\nCoding: \n* Christian Mauduit\n\nMusic:\n* Tim Chadburn\n\nOther contributors:\n* Alstar\n* Peter Wang\n* Cort Danger Stratton\n* Jan Gretschuskin (www.game-factor.de)\n* Mouse\n* Rene Stach\n* Roderick Schertler\n* Ryan D. Brown\n* Eduard Bloch\n* Michael Terry\n* Kasper Hviid\n* David Redick\n* Alexandre Pineau\n* Michael Wagner\n* Peter Williams\n* Jan Samohyl\n* Gavin\n* Dave Vasilevsky\n* 2 of Omega\n* 666-REFIZUL-666\n* Thomas Klausner\n* Joan Dolc\n* Mikhail Yakshin\n* Valerie Mauduit\n* Florence Mauduit\n* Joey Dodson\n* Jan Szenborn\n\nMore informations on www.ufoot.org/liquidwar/v5\n";
  LW_LANGEN_DATA[LW_LANG_STRING_HELP_COPYRIGHT] =
    "Liquid War is a multiplayer wargame.\nCopyright (C) 1998-2016 Christian Mauduit (ufoot@ufoot.org)\n\nThis program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.\n\nThis program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.\n\nYou should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA\n\nMore informations on www.ufoot.org/liquidwar/v5\n";
  /*
   * Stupid Microsoft VC++ can't handle strings over 2048 chars correctly,
   * (compiler error C2026) so we had to split this one...
   */
  LW_LANGEN_DATA[LW_LANG_STRING_HELP_LICENSE] =
    "GNU GENERAL PUBLIC LICENSE\nVersion 2, June 1991\n\nCopyright (C) 1989, 1991 Free Software Foundation, Inc. 675 Mass Ave, Cambridge, MA 02139, USA\n\nEveryone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.\n\nPreamble\n\nThe licenses for most software are designed to take away your freedom to share and change it.  By contrast, the GNU General Public License is intended to guarantee your freedom to share and change free software--to make sure the software is free for all its users.  This General Public License applies to most of the Free Software Foundation's software and to any other program whose authors commit to using it.  (Some other Free Software Foundation software is covered by the GNU Library General Public License instead.)  You can apply it to your programs, too.\n\nWhen we speak of free software, we are referring to freedom, not price.  Our General Public Licenses are designed to make sure that you have the freedom "
    "to distribute copies of free software (and charge for this service if you wish), that you receive source code or can get it if you want it, that you can change the software or use pieces of it in new free programs; and that you know you can do these things. \n\nTo protect your rights, we need to make restrictions that forbid anyone to deny you these rights or to ask you to surrender the rights. These restrictions translate to certain responsibilities for you if you distribute copies of the software, or if you modify it. \n\nFor example, if you distribute copies of such a program, whether gratis or for a fee, you must give the recipients all the rights that you have.  You must make sure that they, too, receive or can get the source code.  And you must show them these terms so they know their rights.\n\nWe protect your rights with two steps: (1) copyright the software, and (2) offer you this license which gives you legal permission to copy, distribute and/or modify the software.\n\nAlso, " "for each author's protection and ours, we want to make certain that everyone understands that there is no warranty for this free software.  If the software is modified by someone else and passed on, we want its recipients to know that what they have is not the original, so that any problems introduced by others will not reflect on the original authors' reputations.\n\nFinally, any free program is threatened constantly by software patents.  We wish to avoid the danger that redistributors of a free program will individually obtain patent licenses, in effect making the program proprietary.  To prevent this, we have made it clear that any patent must be licensed for everyone's free use or not licensed at all.\n\nThe precise terms and conditions for copying, distribution and modification follow.\n\nGNU GENERAL PUBLIC LICENSE\nTERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION\n\n0. This License applies to any program or other work which contains a notice placed by the copyright " "holder saying it may be distributed under the terms of this General Public License.  The \"Program\", below, refers to any such program or work, and a \"work based on the Program\" means either the Program or any derivative work under copyright law: that is to say, a work containing the Program or a portion of it, either verbatim or with modifications and/or translated into another language.  (Hereinafter, translation is included without limitation in the term \"modification\".)  Each licensee is addressed as \"you\".\nActivities other than copying, distribution and modification are not covered by this License; they are outside its scope.  The act of running the Program is not restricted, and the output from the Program is covered only if its contents constitute a work based on the Program (independent of having been made by running the Program). Whether that is true depends on what the Program does. \n\n1. You may copy and distribute verbatim copies of the Program's source code as you " "receive it, in any medium, provided that you conspicuously and appropriately publish on each copy an appropriate copyright notice and disclaimer of warranty; keep intact all the notices that refer to this License and to the absence of any warranty; and give any other recipients of the Program a copy of this License along with the Program. \nYou may charge a fee for the physical act of transferring a copy, and you may at your option offer warranty protection in exchange for a fee.\n\n2. You may modify your copy or copies of the Program or any portion of it, thus forming a work based on the Program, and copy and distribute such modifications or work under the terms of Section 1 above, provided that you also meet all of these conditions: \na) You must cause the modified files to carry prominent notices stating that you changed the files and the date of any change.\nb) You must cause any work that you distribute or publish, that in whole or in part contains or is derived from the Program or " "any part thereof, to be licensed as a whole at no charge to all third parties under the terms of this License.\nc) If the modified program normally reads commands interactively when run, you must cause it, when started running for such interactive use in the most ordinary way, to print or display an announcement including an appropriate copyright notice and a notice that there is no warranty (or else, saying that you provide a warranty) and that users may redistribute the program under these conditions, and telling the user how to view a copy of this License.  (Exception: if the Program itself is interactive but does not normally print such an announcement, your work based on the Program is not required to print an announcement.)\nThese requirements apply to the modified work as a whole.  If identifiable sections of that work are not derived from the Program, and can be reasonably considered independent and separate works in themselves, then this License, and its terms, do not apply to " "those sections when you distribute them as separate works.  But when you distribute the same sections as part of a whole which is a work based on the Program, the distribution of the whole must be on the terms of this License, whose permissions for other licensees extend to the entire whole, and thus to each and every part regardless of who wrote it.\nThus, it is not the intent of this section to claim rights or contest your rights to work written entirely by you; rather, the intent is to exercise the right to control the distribution of derivative or collective works based on the Program. \nIn addition, mere aggregation of another work not based on the Program with the Program (or with a work based on the Program) on a volume of a storage or distribution medium does not bring the other work under the scope of this License. \n\n3. You may copy and distribute the Program (or a work based on it, under Section 2) in object code or executable form under the terms of Sections 1 and 2 above " "provided that you also do one of the following:\na) Accompany it with the complete corresponding machine-readable source code, which must be distributed under the terms of Sections 1 and 2 above on a medium customarily used for software interchange; or,\nb) Accompany it with a written offer, valid for at least three years, to give any third party, for a charge no more than your cost of physically performing source distribution, a complete machine-readable copy of the corresponding source code, to be distributed under the terms of Sections 1 and 2 above on a medium customarily used for software interchange; or,\nc) Accompany it with the information you received as to the offer to distribute corresponding source code.  (This alternative is allowed only for noncommercial distribution and only if you received the program in object code or executable form with such an offer, in accord with Subsection b above.)\nThe source code for a work means the preferred form of the work for making modifications " "to it.  For an executable work, complete source code means all the source code for all modules it contains, plus any associated interface definition files, plus the scripts used to control compilation and installation of the executable.  However, as a special exception, the source code distributed need not include anything that is normally distributed (in either source or binary form) with the major components (compiler, kernel, and so on) of the operating system on which the executable runs, unless that component itself accompanies the executable.\nIf distribution of executable or object code is made by offering access to copy from a designated place, then offering equivalent access to copy the source code from the same place counts as distribution of the source code, even though third parties are not compelled to copy the source along with the object code.\n\n4. You may not copy, modify, sublicense, or distribute the Program except as expressly provided under this License.  Any attempt " "otherwise to copy, modify, sublicense or distribute the Program is void, and will automatically terminate your rights under this License. However, parties who have received copies, or rights, from you under this License will not have their licenses terminated so long as such parties remain in full compliance.\n\n5. You are not required to accept this License, since you have not signed it.  However, nothing else grants you permission to modify or distribute the Program or its derivative works.  These actions are prohibited by law if you do not accept this License.  Therefore, by modifying or distributing the Program (or any work based on the Program), you indicate your acceptance of this License to do so, and all its terms and conditions for copying, distributing or modifying the Program or works based on it.\n\n6. Each time you redistribute the Program (or any work based on the Program), the recipient automatically receives a license from the original licensor to copy, distribute or modify " "the Program subject to these terms and conditions.  You may not impose any further restrictions on the recipients' exercise of the rights granted herein. You are not responsible for enforcing compliance by third parties to this License.\n\n7. If, as a consequence of a court judgment or allegation of patent infringement or for any other reason (not limited to patent issues), conditions are imposed on you (whether by court order, agreement or otherwise) that contradict the conditions of this License, they do not excuse you from the conditions of this License.  If you cannot distribute so as to satisfy simultaneously your obligations under this License and any other pertinent obligations, then as a consequence you may not distribute the Program at all.  For example, if a patent license would not permit royalty-free redistribution of the Program by all those who receive copies directly or indirectly through you, then the only way you could satisfy both it and this License would be to refrain " "entirely from distribution of the Program.\nIf any portion of this section is held invalid or unenforceable under any particular circumstance, the balance of the section is intended to apply and the section as a whole is intended to apply in other circumstances.\nIt is not the purpose of this section to induce you to infringe any patents or other property right claims or to contest validity of any such claims; this section has the sole purpose of protecting the integrity of the free software distribution system, which is implemented by public license practices.  Many people have made generous contributions to the wide range of software distributed through that system in reliance on consistent application of that system; it is up to the author/donor to decide if he or she is willing to distribute software through any other system and a licensee cannot impose that choice.\nThis section is intended to make thoroughly clear what is believed to be a consequence of the rest of this License.\n\n8. " "If the distribution and/or use of the Program is restricted in certain countries either by patents or by copyrighted interfaces, the original copyright holder who places the Program under this License may add an explicit geographical distribution limitation excluding those countries, so that distribution is permitted only in or among countries not thus excluded.  In such case, this License incorporates the limitation as if written in the body of this License.\n\n9. The Free Software Foundation may publish revised and/or new versions of the General Public License from time to time.  Such new versions will be similar in spirit to the present version, but may differ in detail to address new problems or concerns.\nEach version is given a distinguishing version number.  If the Program specifies a version number of this License which applies to it and \"any later version\", you have the option of following the terms and conditions either of that version or of any later version published by the " "Free Software Foundation.  If the Program does not specify a version number of this License, you may choose any version ever published by the Free Software Foundation. \n\n10. If you wish to incorporate parts of the Program into other free programs whose distribution conditions are different, write to the author to ask for permission.  For software which is copyrighted by the Free Software Foundation, write to the Free Software Foundation; we sometimes make exceptions for this.  Our decision will be guided by the two goals of preserving the free status of all derivatives of our free software and of promoting the sharing and reuse of software generally.\n\nNO WARRANTY\n\n11. BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT " "NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION. \n\n12. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR REDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.\n\nEND OF TERMS AND CONDITIONS\n\nMore informations on www.ufoot.org/liquidwar/v5\n";
  LW_LANGEN_DATA[LW_LANG_STRING_HELP_VERSION] =
    "Liquid War by U-Foot\n\nv" LW_VERSION " \n\ncompiled on " __DATE__ " at "
    __TIME__ "\n\nMore informations on www.ufoot.org/liquidwar/v5\n";

  LW_LANGEN_DATA[LW_LANG_STRING_MENU_PLAY] = "Play";
  LW_LANGEN_DATA[LW_LANG_STRING_MENU_NETGAME] = "Net game";
  LW_LANGEN_DATA[LW_LANG_STRING_MENU_MAP] = "Map";
  LW_LANGEN_DATA[LW_LANG_STRING_MENU_TEAMS] = "Teams";
  LW_LANGEN_DATA[LW_LANG_STRING_MENU_OPTIONS] = "Options";
  LW_LANGEN_DATA[LW_LANG_STRING_MENU_ABOUT] = "About";
  LW_LANGEN_DATA[LW_LANG_STRING_MENU_EXIT] = "Exit";
  LW_LANGEN_DATA[LW_LANG_STRING_MENU_EXITDOS] = "Exit to DOS";
  LW_LANGEN_DATA[LW_LANG_STRING_MENU_EXITWINDOWS] = "Exit";
  LW_LANGEN_DATA[LW_LANG_STRING_MENU_EXITUNIX] = "Exit";
  LW_LANGEN_DATA[LW_LANG_STRING_MENU_BACK] = "Back to LW";

  LW_LANGEN_DATA[LW_LANG_STRING_DIALOG_QUIT] = "Quit";
  LW_LANGEN_DATA[LW_LANG_STRING_DIALOG_BACK] = "Back";
  LW_LANGEN_DATA[LW_LANG_STRING_DIALOG_PLAY] = "Play";
  LW_LANGEN_DATA[LW_LANG_STRING_DIALOG_MAIN] = "Main";

  LW_LANGEN_DATA[LW_LANG_STRING_PLAY_GETREADY] = "Get ready!";

  LW_LANGEN_DATA[LW_LANG_STRING_NETGAME_STARTGAME] = "Start game";
  LW_LANGEN_DATA[LW_LANG_STRING_NETGAME_SERVERADDR] = "Server addr";
  LW_LANGEN_DATA[LW_LANG_STRING_NETGAME_SERVERPORT] = "Server port";
  LW_LANGEN_DATA[LW_LANG_STRING_NETGAME_SEARCH] = "Search for internet games";
  LW_LANGEN_DATA[LW_LANG_STRING_NETGAME_PASSWORD] = "Password (*)";
  LW_LANGEN_DATA[LW_LANG_STRING_NETGAME_WEAKPASSWORD] =
    "(*) WARNING! Choose a weak password like \"hello\", for it is stored on your hard drive and sent to the server as plain text.";

  LW_LANGEN_DATA[LW_LANG_STRING_LANGUAGE_AUTO] = "Automatic";

  LW_LANGEN_DATA[LW_LANG_STRING_TEAM_OFF] = "Off";
  LW_LANGEN_DATA[LW_LANG_STRING_TEAM_HUMAN] = "Human";
  LW_LANGEN_DATA[LW_LANG_STRING_TEAM_CPU] = "Cpu";

  LW_LANGEN_DATA[LW_LANG_STRING_OPTIONS_GRAPHICS] = "Graphics";
  LW_LANGEN_DATA[LW_LANG_STRING_OPTIONS_SOUND] = "Sound";
  LW_LANGEN_DATA[LW_LANG_STRING_OPTIONS_RULES] = "Rules";
  LW_LANGEN_DATA[LW_LANG_STRING_OPTIONS_SPEEDS] = "Speeds";
  LW_LANGEN_DATA[LW_LANG_STRING_OPTIONS_CONTROLS] = "Controls";
  LW_LANGEN_DATA[LW_LANG_STRING_OPTIONS_LANGUAGE] = "Language";
  LW_LANGEN_DATA[LW_LANG_STRING_OPTIONS_DEFAULT] = "Defaults";

  LW_LANGEN_DATA[LW_LANG_STRING_GRAPHICS_GAME] = "Game";
  LW_LANGEN_DATA[LW_LANG_STRING_GRAPHICS_MENU] = "Menu";
  LW_LANGEN_DATA[LW_LANG_STRING_GRAPHICS_ON] = "On";
  LW_LANGEN_DATA[LW_LANG_STRING_GRAPHICS_OFF] = "Off";
  LW_LANGEN_DATA[LW_LANG_STRING_GRAPHICS_FULLSCREEN] = "Fullscreen";
  LW_LANGEN_DATA[LW_LANG_STRING_GRAPHICS_WINDOWED] = "Windowed";
  LW_LANGEN_DATA[LW_LANG_STRING_GRAPHICS_ADVANCED] = "Advanced";
  LW_LANGEN_DATA[LW_LANG_STRING_GRAPHICS_VIDEOMODE] = "Video mode";
  LW_LANGEN_DATA[LW_LANG_STRING_GRAPHICS_BRIGHTNESS] = "Brightness";
  LW_LANGEN_DATA[LW_LANG_STRING_GRAPHICS_VIEWPORTSIZE] = "Viewport size";
  LW_LANGEN_DATA[LW_LANG_STRING_GRAPHICS_PAGEFLIPPING] = "Page flipping";
  LW_LANGEN_DATA[LW_LANG_STRING_GRAPHICS_WAVES] = "Waves (F4)";

  LW_LANGEN_DATA[LW_LANG_STRING_VOLUME_SFX] = "Sound fx";
  LW_LANGEN_DATA[LW_LANG_STRING_VOLUME_CLICK] = "Click";
  LW_LANGEN_DATA[LW_LANG_STRING_VOLUME_GAMEWATER] = "Game water";
  LW_LANGEN_DATA[LW_LANG_STRING_VOLUME_MENUWATER] = "Menu water";
  LW_LANGEN_DATA[LW_LANG_STRING_VOLUME_MUSIC] = "Music";

  LW_LANGEN_DATA[LW_LANG_STRING_RULES_TIME] = "Time";
  LW_LANGEN_DATA[LW_LANG_STRING_RULES_ADVANCED] = "Advanced";
  LW_LANGEN_DATA[LW_LANG_STRING_RULES_ARMYSIZE] = "Army size";
  LW_LANGEN_DATA[LW_LANG_STRING_RULES_CURSOR] = "Cursor";
  LW_LANGEN_DATA[LW_LANG_STRING_RULES_DEFAULT] = "Defaults";
  LW_LANGEN_DATA[LW_LANG_STRING_RULES_MINMAPRES] = "Min ";

  LW_LANGEN_DATA[LW_LANG_STRING_ADVANCED_ATTACK] = "Attack";
  LW_LANGEN_DATA[LW_LANG_STRING_ADVANCED_DEFENSE] = "Defense";
  LW_LANGEN_DATA[LW_LANG_STRING_ADVANCED_BASEHEALTH] = "Base health";
  LW_LANGEN_DATA[LW_LANG_STRING_ADVANCED_WINNERHELP] = "Winner help";
  LW_LANGEN_DATA[LW_LANG_STRING_ADVANCED_CPUSTRENGTH] = "CPU strength";
  LW_LANGEN_DATA[LW_LANG_STRING_ADVANCED_CPUVSHUMAN] = "CPU vs human";
  LW_LANGEN_DATA[LW_LANG_STRING_ADVANCED_ALWAYS] = "Always";
  LW_LANGEN_DATA[LW_LANG_STRING_ADVANCED_RANDOM] = "Random";
  LW_LANGEN_DATA[LW_LANG_STRING_ADVANCED_NEVER] = "Never";
  LW_LANGEN_DATA[LW_LANG_STRING_ADVANCED_ASMALGORITHM] = "Algorithm";
  LW_LANGEN_DATA[LW_LANG_STRING_ADVANCED_ASSEMBLY] = "Assembly";
  LW_LANGEN_DATA[LW_LANG_STRING_ADVANCED_STANDARDC] = "Standard C";
  LW_LANGEN_DATA[LW_LANG_STRING_ADVANCED_ALLOWNETWORKBOTS] = "Allow net bots";
  LW_LANGEN_DATA[LW_LANG_STRING_ADVANCED_YES] = "Yes";
  LW_LANGEN_DATA[LW_LANG_STRING_ADVANCED_NO] = "No";

  LW_LANGEN_DATA[LW_LANG_STRING_SPEEDS_FPS] = "frames/s";
  LW_LANGEN_DATA[LW_LANG_STRING_SPEEDS_NOFPSLIMIT] = "No fps limit";
  LW_LANGEN_DATA[LW_LANG_STRING_SPEEDS_RPS] = "rounds/s";
  LW_LANGEN_DATA[LW_LANG_STRING_SPEEDS_NORPSLIMIT] = "No rps limit";

  LW_LANGEN_DATA[LW_LANG_STRING_CONTROLS_JOYSTICK] = "Joystick";
  LW_LANGEN_DATA[LW_LANG_STRING_CONTROLS_OFF] = "Off";
  LW_LANGEN_DATA[LW_LANG_STRING_CONTROLS_2JOYSTICKS] = "2 joysticks";
  LW_LANGEN_DATA[LW_LANG_STRING_CONTROLS_4BUTTONS] = "4 buttons";
  LW_LANGEN_DATA[LW_LANG_STRING_CONTROLS_STANDARD] = "Standard";
  LW_LANGEN_DATA[LW_LANG_STRING_CONTROLS_MOUSESENSIB] =
    "In-game mouse sensibility";

  LW_LANGEN_DATA[LW_LANG_STRING_ABOUT_CREDITS] = "Credits";
  LW_LANGEN_DATA[LW_LANG_STRING_ABOUT_COPYRIGHT] = "Copyright";
  LW_LANGEN_DATA[LW_LANG_STRING_ABOUT_LICENSE] = "License";
  LW_LANGEN_DATA[LW_LANG_STRING_ABOUT_VERSION] = "Version";

  LW_LANGEN_DATA[LW_LANG_STRING_INTERNET_REFRESH] = "Refresh";
  LW_LANGEN_DATA[LW_LANG_STRING_INTERNET_JOINNOW] = "Join now";
  LW_LANGEN_DATA[LW_LANG_STRING_INTERNET_PORT] = "Port";
  LW_LANGEN_DATA[LW_LANG_STRING_INTERNET_PASSWORD] = "Password";
  LW_LANGEN_DATA[LW_LANG_STRING_INTERNET_YES] = "yes";
  LW_LANGEN_DATA[LW_LANG_STRING_INTERNET_NO] = "no";
  LW_LANGEN_DATA[LW_LANG_STRING_INTERNET_UPTIME] = "Uptime";
  LW_LANGEN_DATA[LW_LANG_STRING_INTERNET_DAYS] = "days";
  LW_LANGEN_DATA[LW_LANG_STRING_INTERNET_DAY] = "day";
  LW_LANGEN_DATA[LW_LANG_STRING_INTERNET_HOURS] = "h";
  LW_LANGEN_DATA[LW_LANG_STRING_INTERNET_MINUTES] = "min";
  LW_LANGEN_DATA[LW_LANG_STRING_INTERNET_SECONDS] = "sec";
  LW_LANGEN_DATA[LW_LANG_STRING_INTERNET_UNREACHABLE] = "Unreachable";
  LW_LANGEN_DATA[LW_LANG_STRING_INTERNET_PING] = "Ping";
  LW_LANGEN_DATA[LW_LANG_STRING_INTERNET_MSEC] = "ms";

  LW_LANGEN_DATA[LW_LANG_STRING_WAVE_AMPLI] = "Ampli";
  LW_LANGEN_DATA[LW_LANG_STRING_WAVE_NUMBER] = "Number";
  LW_LANGEN_DATA[LW_LANG_STRING_WAVE_SPEED] = "Speed";
  LW_LANGEN_DATA[LW_LANG_STRING_WAVE_WX] = "WX";
  LW_LANGEN_DATA[LW_LANG_STRING_WAVE_HY] = "HY";
  LW_LANGEN_DATA[LW_LANG_STRING_WAVE_WY] = "WY";
  LW_LANGEN_DATA[LW_LANG_STRING_WAVE_HX] = "HX";

  LW_LANGEN_DATA[LW_LANG_STRING_CONNECT_STARTNOW] = "Start now";
  LW_LANGEN_DATA[LW_LANG_STRING_CONNECT_WAITING] = "Waiting...";
  LW_LANGEN_DATA[LW_LANG_STRING_CONNECT_WAITINGFOR] = "Waiting for ";
  LW_LANGEN_DATA[LW_LANG_STRING_CONNECT_TEAMS] = " team(s)";
  LW_LANGEN_DATA[LW_LANG_STRING_CONNECT_SENDMESSAGE] = "Send message";

  LW_LANGEN_DATA[LW_LANG_STRING_NETPLAY_CONNECT] = "Connect";
  LW_LANGEN_DATA[LW_LANG_STRING_NETPLAY_PING] = "Ping";
  LW_LANGEN_DATA[LW_LANG_STRING_NETPLAY_CHECKPROGRAM] = "Check program";
  LW_LANGEN_DATA[LW_LANG_STRING_NETPLAY_CHECKVERSION] = "Check version";
  LW_LANGEN_DATA[LW_LANG_STRING_NETPLAY_SENDPASSWORD] = "Password";
  LW_LANGEN_DATA[LW_LANG_STRING_NETPLAY_FREETEAMS] = "Free teams?";
  LW_LANGEN_DATA[LW_LANG_STRING_NETPLAY_RESERVETEAMS] = "Reserve teams";
  LW_LANGEN_DATA[LW_LANG_STRING_NETPLAY_SENDCONFIG] = "Send config";
  LW_LANGEN_DATA[LW_LANG_STRING_NETPLAY_SENDMAP] = "Send map";
  LW_LANGEN_DATA[LW_LANG_STRING_NETPLAY_WAITING] = "Waiting...";
  LW_LANGEN_DATA[LW_LANG_STRING_NETPLAY_WHOPLAYS] = "Who plays?";
  LW_LANGEN_DATA[LW_LANG_STRING_NETPLAY_RECVCONFIG] = "Receive config";
  LW_LANGEN_DATA[LW_LANG_STRING_NETPLAY_RECVMAP] = "Receive map";

  LW_LANGEN_DATA[LW_LANG_STRING_LEVEL_USEDEFAULTTEXTURE] =
    "Use default texture";
  LW_LANGEN_DATA[LW_LANG_STRING_LEVEL_ON] = "On";
  LW_LANGEN_DATA[LW_LANG_STRING_LEVEL_OFF] = "Off";
  LW_LANGEN_DATA[LW_LANG_STRING_LEVEL_RANDOMMAP] = "Random map";

  LW_LANGEN_DATA[LW_LANG_STRING_CODE_FPS] = "frames/s ";
  LW_LANGEN_DATA[LW_LANG_STRING_CODE_RPS] = "rounds/s ";
  LW_LANGEN_DATA[LW_LANG_STRING_CODE_PERCENTCALC] = "% calc ";
  LW_LANGEN_DATA[LW_LANG_STRING_CODE_PERCENTDISPLAY] = "% display ";
  LW_LANGEN_DATA[LW_LANG_STRING_CODE_SPREADGRADTIME] = "Spread grad time ";
  LW_LANGEN_DATA[LW_LANG_STRING_CODE_MOVEARMYTIME] = "Move army time ";
  LW_LANGEN_DATA[LW_LANG_STRING_CODE_DISPLAYTIME] = "Display time ";
  LW_LANGEN_DATA[LW_LANG_STRING_CODE_FLIPTIME] = "Flip time ";
  LW_LANGEN_DATA[LW_LANG_STRING_CODE_OTHERCALCTIME] = "Other calc time ";
  LW_LANGEN_DATA[LW_LANG_STRING_CODE_GLOBALTIME] = "Global time ";
  LW_LANGEN_DATA[LW_LANG_STRING_CODE_STANDARDDISPLAY] = "Standard display";
  LW_LANGEN_DATA[LW_LANG_STRING_CODE_GRADIENT1] = "Gradient 1";
  LW_LANGEN_DATA[LW_LANG_STRING_CODE_GRADIENT2] = "Gradient 2";
  LW_LANGEN_DATA[LW_LANG_STRING_CODE_GRADIENT3] = "Gradient 3";
  LW_LANGEN_DATA[LW_LANG_STRING_CODE_GRADIENT4] = "Gradient 4";
  LW_LANGEN_DATA[LW_LANG_STRING_CODE_GRADIENT5] = "Gradient 5";
  LW_LANGEN_DATA[LW_LANG_STRING_CODE_GRADIENT6] = "Gradient 6";
  LW_LANGEN_DATA[LW_LANG_STRING_CODE_MESHSIZE] = "Mesh size";
  LW_LANGEN_DATA[LW_LANG_STRING_CODE_MESHSTRUCTURE] = "Mesh structure";
  LW_LANGEN_DATA[LW_LANG_STRING_CODE_PAUSEOFF] = "Pause off";
  LW_LANGEN_DATA[LW_LANG_STRING_CODE_PAUSEON] = "Pause on";
  LW_LANGEN_DATA[LW_LANG_STRING_CODE_WAVEEFFECTSOFF] = "Wave effects off";
  LW_LANGEN_DATA[LW_LANG_STRING_CODE_WAVEEFFECTSON] = "Wave effects on";
  LW_LANGEN_DATA[LW_LANG_STRING_CODE_CAPTUREOFF] = "Video capture off";
  LW_LANGEN_DATA[LW_LANG_STRING_CODE_CAPTUREON] = "Video capture on";

  LW_LANGEN_DATA[LW_LANG_STRING_SCORE_WINNER] = "Winner";

  LW_LANGEN_DATA[LW_LANG_STRING_GAME_BACKMEMFAIL] = "Back mem fail";
  LW_LANGEN_DATA[LW_LANG_STRING_GAME_DISPMEMFAIL] = "Disp mem fail";
  LW_LANGEN_DATA[LW_LANG_STRING_GAME_MESHMEMFAIL] = "Mesh mem fail";
  LW_LANGEN_DATA[LW_LANG_STRING_GAME_AREAMEMFAIL] = "Area mem fail";
  LW_LANGEN_DATA[LW_LANG_STRING_GAME_ARMYMEMFAIL] = "Army mem fail";

  LW_LANGEN_DATA[LW_LANG_STRING_ERROR_NOTENOUGHTEAMS] =
    "Not enough teams.\nYou must select at least 2 teams from the \"Teams\" menu.";
  LW_LANGEN_DATA[LW_LANG_STRING_ERROR_NOTENOUGHBIGDATA] =
    "Not enough memory to expand level.\nChoose a smaller level or look at the documentation and try command line options such as \"-mem n\".";
  LW_LANGEN_DATA[LW_LANG_STRING_ERROR_GRAPHICSINITFAILED] =
    "Unable to start graphic mode";
  LW_LANGEN_DATA[LW_LANG_STRING_ERROR_NONETWORKSUPPORT] =
    "No network support for this platform.\nOnly Windows and UNIX have it.";
  LW_LANGEN_DATA[LW_LANG_STRING_ERROR_UNABLETOCONNECT] =
    "Unable to connect to the server.\nMaybe it's not correctly set up, or you are behind a firewall which forbids the connection.";
  LW_LANGEN_DATA[LW_LANG_STRING_ERROR_NOTALIQUIDWARSERVER] =
    "Unable to communicate with the server.\nIt doesn't seem to be a Liquid War server.";
  LW_LANGEN_DATA[LW_LANG_STRING_ERROR_WRONGVERSION] =
    "Wrong client/server versions.\nServer and client must have the same version (this client's version is \""
    LW_VERSION "\").";
  LW_LANGEN_DATA[LW_LANG_STRING_ERROR_NETWORKPROBLEM] =
    "A network error has occured.\nThis often happens when the connection with the server is too slow or simply lost.";
  LW_LANGEN_DATA[LW_LANG_STRING_ERROR_CANTSENDMAP] =
    "Unable to send the map to the server.";
  LW_LANGEN_DATA[LW_LANG_STRING_ERROR_CANTRECEIVEMAP] =
    "Unable to receive the map from the server.";
  LW_LANGEN_DATA[LW_LANG_STRING_ERROR_SERVERFULL] =
    "This server is full and doesn't accept new teams for now.";
  LW_LANGEN_DATA[LW_LANG_STRING_ERROR_BADPASSWORD] = "Incorrect password.";
  LW_LANGEN_DATA[LW_LANG_STRING_ERROR_BADCONFIGRANGE] =
    "One of the config parameters is out of range. Either there's a bug in the game, or someone is cheating and/or trying to hack something.";

  LW_LANGEN_DATA[LW_LANG_STRING_POPUP_INITFAIL] = "Init failed.";
  LW_LANGEN_DATA[LW_LANG_STRING_POPUP_MEMTROUBLE] =
    "Not enough memory for bitmaps.";
  LW_LANGEN_DATA[LW_LANG_STRING_POPUP_BIGDATA] = "Not enough memory.";
  LW_LANGEN_DATA[LW_LANG_STRING_POPUP_LOADFAIL] =
    "Unable to load datafile. Are you sure Liquid War is correctly installed?";
  LW_LANGEN_DATA[LW_LANG_STRING_POPUP_CUSTOMFAIL] =
    "Unable to load custom data.";
  LW_LANGEN_DATA[LW_LANG_STRING_POPUP_GRAPHICSFAIL] =
    "Unable to initialize graphics.";

  LW_LANGEN_DATA[LW_LANG_STRING_LW6_INFO] =
    "This program is obsolete, please consider upgrading to Liquid War 6, available on\n\nhttp://www.gnu.org/software/liquidwar6/\n\nHowever, Liquid War 5 is still available for your convenience, and you can play it freely. Only major security holes will be fixed, expect no major changes on this legacy branch.";
  LW_LANGEN_DATA[LW_LANG_STRING_LW6_OK] = "OK";
}

/*------------------------------------------------------------------*/
/* returns the English string associated to an id                   */
/*------------------------------------------------------------------*/
char *
lw_langen_string (int id)
{
  char *result = NULL;

  if (id >= 0 && id < LW_LANG_NB_STRINGS)
    {
      result = LW_LANGEN_DATA[id];
    }

  if (!result)
    {
      result = "??? en";
    }

  return result;
}
