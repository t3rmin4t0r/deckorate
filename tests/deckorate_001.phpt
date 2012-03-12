--TEST--
Deckorate: basic tests
--SKIPIF--
<?php if (!extension_loaded("deckorate")) die("skip"); ?>
--INI--
--FILE--
<?php
echo serialize(array("Hello World" => 1)) . "\n";

if(deckorate("serialize", "__serialize")) 
{
	function serialize($v)
	{
		return str_rot13(__serialize($v));
	}
}

echo serialize(array("Hello World" => 1)) . "\n";
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
a:1:{s:11:"Hello World";i:1;}
n:1:{f:11:"Uryyb Jbeyq";v:1;}
===DONE===
