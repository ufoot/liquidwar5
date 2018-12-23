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

//----------------------------------------------------------------------
// Transforms text to HTML code, to avoid buggy and/or malicious HTML 
//----------------------------------------------------------------------
function html_format($text)
{
    $html=htmlentities($text);

    /*
     * Now if the HTML string is empty, we replace it by &nbsp; which
     * avoids weird behaviors with cell borders in HTML tables.
     */
    if ($html=="") {
        $html="&nbsp;";
    }

    return $html;
}

//----------------------------------------------------------------------
// Transforms a number of second into a readable uptime
//----------------------------------------------------------------------
function html_uptime($timestamp)
{
    $seconds=$timestamp%60;
    $minutes=floor($timestamp/60)%60;
    $hours=floor($timestamp/3600)%24;
    $days=floor($timestamp/(3600*24));

    if ($days>0) {
        if ($days>1) {
            $uptime=$days." days";
        }
        else{
            $uptime="1 day";
        }
    }
    else if ($hours>0) {
        $uptime=$hours." h";
    }
    else if ($minutes>0) {
        $uptime=$minutes." min";
    }
    else {
        $uptime=$seconds." sec";
    }

    return $uptime;
}

//----------------------------------------------------------------------
// Transforms a UNIX timestamp into a readable date
//----------------------------------------------------------------------
function html_date($timestamp)
{
    $date=date("Y-m-d H:i:s",$timestamp);

    return $date;
}

?>
