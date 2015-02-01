<?php 
/**
 * オートロードファイル.
 * 
 * @version $Id: autoload.php 536 2011-07-26 14:58:17Z motonari $
 */

/**
 * オートロード関数.
 * 
 * @param string $name_str ロードするクラス名
 * @return void
 */
function __autoload($name_str) {
	$class_path = str_replace('\\', '/', $name_str) . ".inc";
	include($class_path);
}
?>