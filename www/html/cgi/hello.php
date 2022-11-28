<!DOCTYPE html>
<html>
<head>
    <title>Document</title>
</head>
<body>
<?php

if ($_SERVER['REQUEST_METHOD'] == 'GET')
{
    echo("arguments are: ". $_GET["fname"] . " " . $_GET["lname"] . "\n");
}
else if ($_SERVER['REQUEST_METHOD'] == 'POST')
{
    echo("arguments are: ". $_POST["fname"] . " " . $_POST["lname"] . "\n");
}
?> 
</body>
</html>