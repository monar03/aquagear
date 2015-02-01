<?php 
namespace aquagear\handler;

/**
 * 終了処理のハンドラ. <br />
 * 終了時にこのハンドラが呼ばれる.
 * 
 * @package aquagear 
 * @subpackage handler
 * 
 * @author motonari
 * @version $Id$
 */
class ShutdownHandler extends CoreHandler {
	/**
	 * (non-PHPdoc)
	 * @see aquagear/handler/\aquagear\handler\CoreHandler#initHandler()
	 */
	public static function initHandler() {
		@register_shutdown_function(array('\aquagear\handler\ShutdownHandler',  'shutdownHandler'));
	}
	
	/**
	 * PHPの終了時に、このメソッドが呼ばれる.
	 * 
	 * @return void
	 * 
	 * @see http://jp.php.net/manual/ja/function.register-shutdown-function.php
	 */
	public static function shutdownHandler() {
		$error = error_get_last();
		switch($error['type']) {
			case E_ERROR:
			case E_PARSE:
				\aquagear\log\Logger::getLogInstance()->fatal('php fatal error ['.$error['type'].'] ['. $error['message'] . "]\n" . $error['file'] . ':' . $error['line']);
				\aquagear\utility\HttpUtility::errorRedirect();
				break;
			default:
				break;
		}
	}
}
?>