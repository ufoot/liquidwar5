<?php
/*****************************************************************************/
/* Meta-server which registers game servers                                  */
/* Copyright (C) 2002-2016 Christian Mauduit                                 */
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
/* Homepage       : https://ufoot.org                                     */
/* Contact author : ufoot@ufoot.org                                          */
/*****************************************************************************/

require 'db.php';
require 'html.php';
require 'metaserver.php';
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2//EN">
<HTML>
<HEAD>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=iso-8859-1">
<META HTTP-EQUIV="Content-Language" CONTENT="en" />
<TITLE>U-Foot's meta-server</TITLE>
<LINK REL="shortcut icon" HREF="/favicon.ico" TYPE="image/x-icon" />
<META NAME="viewport" CONTENT="width=device-width; initial-scale=1.0;" />
</HEAD>
<BODY>
<CENTER>
<H1>U-Foot's meta-server</H1>
</CENTER>
<HR>
<H2>Introduction</H2>
I call meta-server the software which I run on my web site to allow the
<A HREF="https://ufoot.org/liquidwar/v5">Liquid War</A> and
<A HREF="https://ufoot.org/u61">U61</A>
servers to register themselves so that clients can automatically
find out which servers are running in the world.
<HR>
<H2>List of active servers</H2>
You can use this list "as is" but you can
also get it directly from the game.
<?php
$result=metaserver_list("1.0","","");
?>
<TABLE BORDER=1>
<TR>
<TH>Address</TH>
<TH>Port</TH>
<TH>Game</TH>
<TH>Version</TH>
<TH>Uptime</TH>
<TH>Players</TH>
<TH>Password</TH>
<TH>Comment</TH>
</TR>
<?php
    if ($result) {
        for ($i=0;$result[$i];++$i) {
            echo "<TR>";
            echo "<TD>".html_format($result[$i]["address"])."</TD>";
            echo "<TD>".html_format($result[$i]["port"])."</TD>";
            echo "<TD>".html_format($result[$i]["game"])."</TD>";
            echo "<TD>".html_format($result[$i]["version"])."</TD>";
            echo "<TD>".html_uptime($result[$i]["uptime"])."</TD>";
            echo "<TD>".html_format($result[$i]["busy_players"])."/".html_format($result[$i]["max_players"])."</TD>";
            echo "<TD>".(html_format($result[$i]["password"]) ? "yes" : "no")."</TD>";
            echo "<TD>".html_format($result[$i]["comment"])."</TD>";
            echo "</TR>\n";
        }
    }
?>
</TABLE>
<HR>
<H2>Chat</H2>
This chat-box is not moderated, so I'm not responsible
for its content. Still, if you think something's going wrong,
<A HREF="mailto:ufoot@ufoot.org">contact me</A>. 
<?php
    if (isset($_POST['post']) && $_POST['game'] && $_POST['message']) {
        if (metaserver_write("1.0",$_POST['game'],$_POST['message'])) {
            echo "<P>Message successfully posted.";
        }
        else {
            echo "<P><B>Message not posted.</B>";
        }
    }
?>
<?php
$result=metaserver_read("1.0","",0);
?>
<TABLE BORDER=1>
<TR>
<TH>Address</TH>
<TH>Age</TH>
<TH>Game</TH>
<TH>Message</TH>
</TR>
<?php
    if ($result) {
        for ($i=0;$result[$i];++$i) {
            echo "<TR>";
            echo "<TD>".html_format($result[$i]["address"])."</TD>";
            echo "<TD>".html_uptime($result[$i]["age"])."</TD>";
            echo "<TD>".html_format($result[$i]["game"])."</TD>";
            echo "<TD>".html_format($result[$i]["message"])."</TD>";
            echo "</TR>\n";
        }
    }
?>
</TABLE>
<FORM METHOD="POST" ACTION="index.php">
<TEXTAREA COLS=70 ROWS=15 NAME="message"></TEXTAREA>
<BR>
<SELECT NAME="game">
<OPTION VALUE="none">Choose a game</OPTION>
<OPTION VALUE="liquidwar">Liquid War</OPTION>
<OPTION VALUE="u61">U61</OPTION>
</SELECT>
<INPUT TYPE="SUBMIT" NAME="post" VALUE="Post">
<INPUT TYPE="SUBMIT" NAME="refresh" VALUE="Refresh">
</FORM>
<HR>
<CENTER>
More informations on
<A HREF="https://ufoot.org">www.ufoot.org</A>.
</CENTER>
</BODY
</HTML>
