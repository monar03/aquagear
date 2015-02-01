<?php 
\aquagear\command\AuthenticationCommand::getInstance()->logout();
\aquagear\utility\HttpUtility::redirect('/top');
?>