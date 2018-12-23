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
/* Homepage       : http://www.ufoot.org                                     */
/* Contact author : ufoot@ufoot.org                                          */
/*****************************************************************************/

// Returns the list of available games

require 'db.php';
require 'html.php';
require 'metaserver.php';

header("Content-Type: application/json");

$result=metaserver_list($_GET["protocol"],
$_GET["game"],
$_GET["version"]);

echo "\"servers\":[\n";
if ($result) {
    for ($i=0;$result[$i];++$i) {
        echo "    {";
        echo "\"address\":";
        echo json_encode($result[$i]["address"],METASERVER_SIZE_ADDRESS).",";
        echo "\"port\":";
        echo $result[$i]["port"].",";
        echo "\"game\":";
        echo json_encode($result[$i]["game"],METASERVER_SIZE_GAME).",";
        echo "\"version\":";
        echo json_encode($result[$i]["version"],METASERVER_SIZE_VERSION).",";
        echo "\"uptime\":";
        echo $result[$i]["uptime"].",";
        echo "\"busy_players\":";
        echo $result[$i]["busy_players"].",";
        echo "\"max_players\":";
        echo $result[$i]["max_players"].",";
        echo "\"password\":";
        echo $result[$i]["password"].",";
        echo "\"comment\":";
        echo json_encode($result[$i]["comment"],METASERVER_SIZE_COMMENT);
        if ($i<count($result)-1) {
            echo "},\n";
        } else {
            echo "}\n";
        }
    }
}

echo "]\n";

?>
