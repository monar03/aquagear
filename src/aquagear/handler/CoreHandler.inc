<?php 
namespace aquagear\handler;

/**
 * ハンドラの基底クラス.
 * 
 * @package aquagear 
 * @subpackage handler
 * 
 * @author motonari
 * @version $Id: CoreHandler.inc 534 2011-07-26 13:48:33Z motonari $
 */
abstract class CoreHandler extends \aquagear\Object {
	/**
	 * ハンドラの初期化.
	 * 
	 * @return void
	 */
	abstract public static function initHandler();
}
?>