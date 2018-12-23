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

error_reporting(0);

define("METASERVER_SIZE_ADDRESS",15);
define("METASERVER_SIZE_GAME",25);
define("METASERVER_SIZE_VERSION",25);
define("METASERVER_SIZE_COMMENT",100);
define("METASERVER_SIZE_MESSAGE",1000);

// 5 minutes
define("METASERVER_LIST_TIME_LIMIT",5*60);
// 5 seconds
define("METASERVER_CHAT_WRITE_LIMIT",5);
// 2 days
define("METASERVER_CHAT_PURGE_LIMIT",2*24*60*60);
// 80 messages
define("METASERVER_CHAT_MAX_MESSAGES",80);

//----------------------------------------------------------------------
// Cleans up the database, ie removes old inactive servers (high-level)
//----------------------------------------------------------------------
function metaserver_cleanup_list()
{
    $connection=db_open_connection();

    $ok=metaserver_cleanup_list_ex($connection);

    db_close_connection($connection);

    return $ok;
}

//----------------------------------------------------------------------
// Cleans up the database, ie removes old inactive servers (low-level)
//----------------------------------------------------------------------
function metaserver_cleanup_list_ex($connection)
{
    $now=time();

    $limit=$now-METASERVER_LIST_TIME_LIMIT;

    $query="DELETE FROM metaserver_list ".
        "WHERE last_ping<".$limit;

    db_exec_query($connection,$query);

    $ok=1;

    return $ok;
}

//----------------------------------------------------------------------
// Registers a server in the database
//----------------------------------------------------------------------
function metaserver_register($protocol,
$game,
$version,
$port,
$busy_players,
$max_players,
$password,
$comment)
{
    $connection=db_open_connection();

    metaserver_cleanup_list_ex($connection);

    $game=db_quote($game,METASERVER_SIZE_GAME);
    $version=db_quote($version,METASERVER_SIZE_VERSION);
    $ping=time();
    $real_address=$_SERVER['HTTP_X_FORWARDED_FOR'] ? $_SERVER['HTTP_X_FORWARDED_FOR'] : $_SERVER['REMOTE_ADDR'];
    $address=db_quote($real_address,METASERVER_SIZE_ADDRESS);
    $port=floor($port);
    $busy_players=floor($busy_players);
    $max_players=floor($max_players);
    $password=floor($password);
    $comment=db_quote($comment,METASERVER_SIZE_COMMENT);
  
    $query="SELECT ".
        "address,".
        "port ".
        "FROM metaserver_list ".
        "WHERE ".
        "address=".$address." AND ".
        "port=".$port;

    $data=db_exec_query_select($connection,$query);

    if ($data && $data[0]) {
        $query="UPDATE metaserver_list SET ".
            "game=".$game.",".
            "version=".$version.",".
            "last_ping=".$ping.",".
            "busy_players=".$busy_players.",".
            "max_players=".$max_players.",".
            "password=".$password.",".
            "comment=".$comment." ".
            "WHERE ".
            "address=".$address." AND ".
            "port=".$port;
    }
    else {
        $query="INSERT INTO metaserver_list ".
            "(address,".
            "port,".
            "game,".
            "version,".
            "first_ping,".
            "last_ping,".
            "busy_players,".
            "max_players,".
            "password,".
            "comment) ".
            "VALUES ".
            "(".$address.",".
            $port.",".
            $game.",".
            $version.",".
            $ping.",".
            $ping.",".
            $busy_players.",".
            $max_players.",".
            $password.",".
            $comment.")";
    }

    if (db_exec_query($connection,$query)==1) {
        $ok=1;
    }
    else {
        $ok=0;
    }
   
    db_close_connection($connection);

    return $ok;
}

//----------------------------------------------------------------------
// Returns the list of available servers
//----------------------------------------------------------------------
function metaserver_list($protocol,
$game,
$version)
{
    $connection=db_open_connection();

    metaserver_cleanup_list_ex($connection);

    $query="SELECT ".
        "address,".
        "port,".
        "game,".
        "version,".
        "first_ping,".
        "last_ping,".
        "busy_players,".
        "max_players,".
        "password,".
        "comment ".
        "FROM metaserver_list ";

    if ($game) {
        $query.="WHERE game=".db_quote($game,METASERVER_SIZE_GAME." ");
        if ($version) {
            $query.="AND version=".db_quote($version,METASERVER_SIZE_VERSION." ");
        }
    }

    $query.="ORDER BY busy_players DESC, first_ping DESC";

    $data=db_exec_query_select($connection,$query);

    $now=time();
    for ($i=0;$data[$i];++$i) {
        $data[$i]["uptime"]=$now-$data[$i]["first_ping"];
    }

    db_close_connection($connection);

    return $data;
}

//----------------------------------------------------------------------
// Cleans up the database, ie removes old inactive servers (high-level)
//----------------------------------------------------------------------
function metaserver_cleanup_chat()
{
    $connection=db_open_connection();

    $ok=metaserver_cleanup_chat_ex($connection);

    db_close_connection($connection);

    return $ok;
}

//----------------------------------------------------------------------
// Cleans up the database, ie removes old inactive servers (low-level)
//----------------------------------------------------------------------
function metaserver_cleanup_chat_ex($connection)
{
    $now=time();

    $limit=$now-METASERVER_CHAT_PURGE_LIMIT;

    $query="DELETE FROM metaserver_chat ".
        "WHERE post_date<".$limit;

    db_exec_query($connection,$query);

    $ok=1;

    return $ok;
}

//----------------------------------------------------------------------
// Registers a server in the database
//----------------------------------------------------------------------
function metaserver_write($protocol,
$game,
$message)
{
    if ($game && $message) {
        $ok=0;

        $connection=db_open_connection();

        metaserver_cleanup_chat_ex($connection);

        $address=db_quote($_SERVER['REMOTE_ADDR'],METASERVER_SIZE_ADDRESS);
        $post_date=time();
        $game=db_quote($game,METASERVER_SIZE_GAME);
        $message=db_quote($message,METASERVER_SIZE_MESSAGE);
  
        $query="SELECT ".
            "MAX(post_date) AS max_post_date ".
            "FROM metaserver_chat ".
            "WHERE ".
            "address=".$address;

        $forbidden=0;
  
        $data=db_exec_query_select($connection,$query);

        if ($data && $data[0]) {
            if ($data[0]["max_post_date"]>$post_date-METASERVER_CHAT_WRITE_LIMIT) {
                $forbidden=1;
            }
        }
      
        if (!$forbidden) {
            $query="INSERT INTO metaserver_chat ".
                "(address,".
                "post_date,".
                "game,".
                "message)".
                "VALUES ".
                "(".$address.",".
                $post_date.",".
                $game.",".
                $message.")";

            if (db_exec_query($connection,$query)==1) {
                $ok=1;
            }
        }
      
        db_close_connection($connection);
    }
    else {
        $ok=1;
    }

    return $ok;
}

//----------------------------------------------------------------------
// Returns the list of available servers
//----------------------------------------------------------------------
function metaserver_read($protocol,
$game,
$limit)
{
    $connection=db_open_connection();

    metaserver_cleanup_chat_ex($connection);

    if (!$limit) {
        $limit=METASERVER_CHAT_MAX_MESSAGES;
    }

    $query="SELECT ".
        "address,".
        "post_date,".
        "game,".
        "message ".
        "FROM metaserver_chat ";

    if ($game) {
        $query.="WHERE game=".db_quote($game,METASERVER_SIZE_GAME." ");
    }

    $query.="ORDER BY post_date DESC ".
        "LIMIT ".$limit;

    $data=db_exec_query_select($connection,$query);

    $now=time();
    for ($i=0;$data[$i];++$i) {
        $data[$i]["age"]=$now-$data[$i]["post_date"];
    }

    db_close_connection($connection);

    return $data;
}

?>
