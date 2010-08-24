var wshShell;
var strText;
var intDelay;
var strTitle;
var intType;

wshShell=WScript.CreateObject("WScript.Shell");
strText="Someone just connected on your server. Ready for a game?";
intDelay=5;
strTitle="Liquid War Server"
intType=0|64; // OK|Information 

wshShell.Popup(strText,intDelay,strTitle,intType);

