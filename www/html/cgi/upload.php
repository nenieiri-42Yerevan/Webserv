<?php


var_dump(getenv());
$upload_dir = "/www/html";
$upload_dir .= getenv("SCRIPT_NAME");
$upload_dir .= getenv("UPLOAD_DIR");

$body = file_get_contents("php://stdin");
echo "body is " . $body ;


?>