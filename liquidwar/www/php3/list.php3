<?php
/*****************************************************************************/
/* Meta-server which registers game servers                                  */
/* Copyright (C) 2002-2015 Christian Mauduit                                 */
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

require 'db.php3';
require 'html.php3';
require 'metaserver.php3';

header("Content-Type: text/plain");

$result=metaserver_list($_GET["protocol"],
			$_GET["game"],
			$_GET["version"]);

if ($result) 
{
  for ($i=0;$result[$i];++$i)
  {
    echo db_quote($result[$i]["address"],METASERVER_SIZE_ADDRESS).",";
    echo $result[$i]["port"].",";
    echo db_quote($result[$i]["game"],METASERVER_SIZE_GAME).",";
    echo db_quote($result[$i]["version"],METASERVER_SIZE_VERSION).",";
    echo $result[$i]["uptime"].",";
    echo $result[$i]["busy_players"].",";
    echo $result[$i]["max_players"].",";
    echo $result[$i]["password"].",";
    echo db_quote($result[$i]["comment"],METASERVER_SIZE_COMMENT)."\n";
  }
}

// We use this EOF trick otherwise there's no HTTP data sent if there
// are no servers that match at all, and it ends in a dull "no status
// line" error on the Liquid War client/server, which is stupid.
// Therefore we send this "EOF" which will be ignored by the parser
// anyways...
echo "EOF\n";

?>
