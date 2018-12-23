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

define("METASERVER_DB_HOST","localhost");
define("METASERVER_DB_USER","www");
define("METASERVER_DB_PASSWORD","dd02c7c2232759874e1c205587017bed");
define("METASERVER_DB_BASE","metaserver");

//----------------------------------------------------------------------
// opens the connection with the database
//----------------------------------------------------------------------
function db_open_connection()
{
    $i=0;
    $connection=0;

    while ($i<5 && !$connection) {
        $connection=mysql_connect(METASERVER_DB_HOST,
        METASERVER_DB_USER,
        METASERVER_DB_PASSWORD);
        sleep($i);
        $i+=1;
    }

    if ($connection) {
        mysql_select_db(METASERVER_DB_BASE);
        if ($i>1) {
            db_handle_error("First attempt to open the metaserver MySQL database failed");
        }
    }
    else {
        db_handle_error("Unable to open connection with the metaserver MySQL database");
    }

    return $connection;
}

//----------------------------------------------------------------------
// closes the connection with the database
//----------------------------------------------------------------------
function db_close_connection($connection)
{
    if ($connection) {
        // Here we do not really close the connection since MySQL shares
        // connections if they are opened with the same parameters,
        // and it interferes with other php code on my web site.
        // Anyway the connection will be closed at the end of the PHP page
        //
        // mysql_close($connection);
    }
}

//----------------------------------------------------------------------
// handles a system error 
//----------------------------------------------------------------------
function db_handle_error($message)
{
    // My provider uses a patched PHP system, which does not support
    // the standard "mail" function but has a replacement called "email".
    // It uses 1 more argument which is a username, used to fill the
    // "From" field. For a standard install, comment the "email" line
    // and uncomment the "mail" line.
    email("webmaster","errors@ufoot.org","[phperror]",$message);
    // mail("errors@xxx.xxx","[phperror]",$message);
}

//----------------------------------------------------------------------
// quotes a string for inclusion in the database
//----------------------------------------------------------------------
function db_quote($field,$size=65535)
{
    $result="";
    $len=strlen($field);

    for ($i=0;($i<$len) && ($i<$size);++$i) {
        // We filter all non ASCII 128 "normal" characters
        $c=substr($field,$i,1);
        $ascii=ord($c);
        if ($ascii>=32 && $ascii<=127) 
            {
                // We replace any ' by '', this is the SQL default
                if ($c=="'") 
                    {
                        $c="''";
                    }
                // We filter $ \ " and %, one never knows...
                else if (strchr("\$\\\"%",$c))
                    {
                        $c="";
                    }

                $result.=$c;
            }
    }

    $result="'".$result."'";

    return $result;
}

//----------------------------------------------------------------------
// Executes a SELECT query in the database
//----------------------------------------------------------------------
function db_exec_query_select($connection,$query)
{
    $i=0;

    if ($connection) {
        $res=mysql_query($query,$connection);

        while ($arr=mysql_fetch_array($res,MYSQL_ASSOC)) {
            $data[$i]=$arr;
            $i++;
        }
    }

    return $data;
}

//----------------------------------------------------------------------
// Executes a query in the database, which is not a SELECT query
//----------------------------------------------------------------------
function db_exec_query($connection,$query)
{
    $rows=-1;

    if ($connection) {
        $res=mysql_query($query,$connection);

        if ($res) {
            $rows=mysql_affected_rows($connection);
        }
    }

    return $rows;
}



?>
